/**
 * This file is part of m4th.
 *
 * m4th is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * m4th is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with m4th.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "m4th.h"

#include <errno.h>  /* errno */
#include <stdio.h>  /* fprintf() */
#include <stdlib.h> /* exit(), free(), malloc() */
#include <string.h> /* memset() */

#ifdef __unix__
#include <sys/mman.h> /* mmap(), munmap() */
#include <unistd.h>   /* sysconf() */
#endif

enum {
    dstack_n = 256,
    rstack_n = 64,
    code_n = 1024,
    inbuf_n = 1024,
    outbuf_n = 1024,
};

static void m4th_oom(size_t bytes) {
    fprintf(stderr, "failed to allocate %lu bytes: %s\n", (unsigned long)bytes, strerror(errno));
    exit(1);
}

#ifdef __unix__
static size_t m4th_page = 0;

static size_t m4th_getpagesize() {
    if (m4th_page == 0) {
        m4th_page =
#if defined(_SC_PAGESIZE)
            sysconf(_SC_PAGESIZE);
#elif defined(_SC_PAGE_SIZE)
            sysconf(_SC_PAGE_SIZE);
#elif defined(PAGESIZE)
            PAGESIZE;
#elif defined(PAGE_SIZE)
            PAGE_SIZE;
#else
            4096;
#endif
    }
    return m4th_page;
}

static size_t m4th_round_to_page(size_t bytes) {
    const size_t page = m4th_getpagesize();
    return (bytes + page - 1) / page * page;
}

void *m4th_mmap(size_t bytes) {
    void *ptr = NULL;
    if (bytes != 0) {
        bytes = m4th_round_to_page(bytes);
        if ((ptr = mmap(NULL, bytes, PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS
#ifdef MAP_STACK
                            | MAP_STACK /* for BSD */
#endif
                        ,
                        -1, 0)) == (void *)-1) {
            m4th_oom(bytes);
        }
        memset(ptr, '\xFF', bytes);
    }
    return ptr;
}

void m4th_munmap(void *ptr, size_t bytes) {
    if (bytes != 0) {
        bytes = m4th_round_to_page(bytes);
        munmap(ptr, bytes);
    }
}
#else /* ! __unix__ */
void *m4th_mmap(size_t bytes) {
    return m4th_alloc(bytes);
}
void m4th_munmap(void *ptr, size_t bytes) {
    m4th_free(ptr);
}
#endif

void *m4th_alloc(size_t bytes) {
    void *ptr = NULL;
    if (bytes != 0) {
        if ((ptr = malloc(bytes)) == NULL) {
            m4th_oom(bytes);
        }
        memset(ptr, '\xFF', bytes);
    }
    return ptr;
}

void m4th_free(void *ptr) {
    free(ptr);
}

static m4stack m4stack_alloc(m4int size) {
    m4int *p = (m4int *)m4th_mmap(size * sizeof(m4int));
    m4stack ret = {p, p + size - 1, p + size - 1};
    return ret;
}

static void m4stack_free(m4stack *arg) {
    if (arg) {
        m4th_munmap(arg->start, (arg->end - arg->start + 1) / sizeof(m4int));
    }
}

static m4code m4code_alloc(m4int size) {
    m4instr *p = (m4instr *)m4th_alloc(size * sizeof(m4instr));
    m4code ret = {p, p, p + size};
    return ret;
}

static void m4code_free(m4code *arg) {
    if (arg) {
        m4th_free(arg->start);
    }
}

static m4cspan m4cspan_alloc(m4int size) {
    m4char *p = (m4char *)m4th_alloc(size * sizeof(m4char));
    m4cspan ret = {p, p, p + size};
    return ret;
}

static void m4cspan_free(m4cspan *arg) {
    if (arg) {
        m4th_free(arg->start);
    }
}

void m4th_stack_print(const m4stack *stack, FILE *out) {
    const m4int *lo = stack->curr;
    const m4int *hi = stack->end;
    fprintf(out, "<%ld> ", (long)(hi - lo));
    while (hi != lo) {
        fprintf(out, "%ld ", (long)*--hi);
    }
    fputc('\n', out);
}

void m4th_flags_print(m4flags fl, FILE *out) {
    m4char printed = 0;
    if (out == NULL) {
        return;
    }
    if (!fl) {
        fputc('0', out);
    }
    if (fl & m4flag_immediate) {
        fputs("immediate", out);
        printed++;
    }
    if (fl & m4flag_inline) {
        fputs(printed++ ? "|inline" : "inline", out);
    }
    if (fl & m4flag_inline_native) {
        fputs(printed++ ? "|inline_native" : "inline_native", out);
    }
}

void m4th_wordname_print(const m4wordname *n, FILE *out) {
    if (n == NULL || out == NULL) {
        return;
    }
    fwrite(n->name, 1, n->name_len, out);
}

void m4th_word_print(const m4word *w, FILE *out) {
    if (w == NULL || out == NULL) {
        return;
    }
    m4th_wordname_print(m4th_word_name(w), out);
    fputs(" {\n\tflags:\t", out);
    m4th_flags_print((m4flags)w->flags, out);
    fprintf(out, "\n\tinline_native_len:\t%u\n\tcode_len:\t%u\n\tdata_len:\t%u\n}\n",
            (unsigned)w->inline_native_len, (unsigned)w->code_len, (unsigned)w->data_len);
}

const m4word *m4th_word_prev(const m4word *w) {
    if (w == NULL || w->prev_off == 0) {
        return NULL;
    }
    return (const m4word *)((const m4char *)w - w->prev_off);
}

const m4wordname *m4th_word_name(const m4word *w) {
    if (w == NULL || w->name_off == 0) {
        return NULL;
    }
    return (const m4wordname *)((const m4char *)w - w->name_off);
}

m4th *m4th_new() {
    m4th *m = (m4th *)m4th_alloc(sizeof(m4th));
    m->dstack = m4stack_alloc(dstack_n);
    m->rstack = m4stack_alloc(rstack_n);
    m->code = m4code_alloc(code_n);
    m->ip = m->code.start;
    m->c_sp = NULL;
    m->in = m4cspan_alloc(inbuf_n);
    m->out = m4cspan_alloc(outbuf_n);
    return m;
}

void m4th_del(m4th *m) {
    if (m) {
        m4cspan_free(&m->out);
        m4cspan_free(&m->in);
        m4code_free(&m->code);
        m4stack_free(&m->rstack);
        m4stack_free(&m->dstack);
        m4th_free(m);
    }
}

void m4th_clear(m4th *m) {
    m->dstack.curr = m->dstack.end;
    m->rstack.curr = m->rstack.end;
    m->code.curr = m->code.start;
    m->ip = m->code.start;
    m->c_sp = NULL;
    m->in.curr = m->in.start;
    m->out.curr = m->out.start;
}
