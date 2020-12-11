/**
 * Copyright (C) 2020 Massimiliano Ghilardi
 *
 * This file is part of m64th.
 *
 * m64th is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * m64th is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with m64th.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef M64TH_T_TESTASM_C
#define M64TH_T_TESTASM_C

#include "../include/m64th.h"
#include "../include/word_fwd.h"
#include "testcommon.h"

#include <assert.h> /* assert() */
#include <stdio.h>  /* fprintf() fputs() */

static m6testasm testasm_a[] = {
#if 0
    {"(asm/if)", {CALL(_asm_if_), m6bye}, {}, {2, {2, m6_asm_if_}}, {}},
#endif
};

/*
 */
static m6code m6testasm_init(const m6testasm *t, m6countedcode *code_buf) {
    m6slice t_code_in = {(m6cell *)t->code, m6test_code_n};
    m6code code = {code_buf->data, code_buf->n};

    m6slice_copy_to_code(t_code_in, &code);
    return code;
}

static m6cell m6testasm_run(m64th *m, const m6testasm *t, const m6code t_code,
                            m6string *ret_asm_codegen) {

#undef M64TH_TEST_VERBOSE
#ifdef M64TH_TEST_VERBOSE
    printf("%s\n", t->input);
#endif

    m64th_clear(m);
    m64th_asm_reserve(m, m6mem_getpagesize());

    m6countedstack_copy(&t->dbefore, &m->dstack);

    m->ip = t_code.addr;
    m64th_run(m);

    {
        const m6char *beg = m->asm_.start;
        const m6char *end = m->asm_here;
        *ret_asm_codegen = m6string_make(beg, end - beg);
    }
    return m6countedstack_equal(&t->dafter, &m->dstack) &&
           m6string_equals(t->asm_codegen, *ret_asm_codegen);
}

static void m6testasm_failed(m64th *m, const m6testasm *t, const m6string actual_asm_codegen,
                             FILE *out) {
    const m6countedstack empty = {};
    if (out == NULL) {
        return;
    }
    fprintf(out, "  asm   test failed: %s", t->name);
    fputs("\n    expected  data  stack ", out);
    m6countedstack_print(&t->dafter, out);
    fputs("\n      actual  data  stack ", out);
    m6stack_print(&m->dstack, m6mode_user, out);

    fputs("\n... expected return stack ", out);
    m6countedstack_print(&empty, out);
    fputs("\n      actual return stack ", out);
    m6stack_print(&m->rstack, m6mode_user, out);

    fputs("\n... expected asm codegen  ", out);
    m6string_print(t->asm_codegen, m6mode_c_disasm, out);
    fputs("\n      actual asm codegen  ", out);
    m6string_print(actual_asm_codegen, m6mode_c_disasm, out);
    fputc('\n', out);
}

m6cell m64th_testasm(m64th *m, FILE *out) {
    const m6testasm *t[] = {testasm_a};
    const m6cell n[] = {N_OF(testasm_a)};

    m6countedcode code_buf;
    m6string actual_asm_codegen = {};
    m6cell i, j, run = 0, fail = 0;

    for (i = 0; i < (m6cell)N_OF(t); i++) {
        for (j = 0; j < n[i]; j++) {
            const m6testasm *tc = &t[i][j];
            m6code t_code = m6testasm_init(tc, &code_buf);

            if (!m6testasm_run(m, tc, t_code, &actual_asm_codegen)) {
                fail++, m6testasm_failed(m, tc, actual_asm_codegen, out);
            }
            m6testasm_forget_all(m);
            run++;
        }
    }
    if (out != NULL) {
        if (fail == 0) {
            fprintf(out, "all %3u   asm   tests passed\n", (unsigned)run);
        } else {
            fprintf(out, "\n  asm   tests failed: %3u of %3u\n", (unsigned)fail, (unsigned)run);
        }
    }
    return fail;
}

#endif /* M64TH_T_TESTASM_C */