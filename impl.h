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

#ifndef M4TH_IMPL_H
#define M4TH_IMPL_H

#include "m4th.h"

typedef struct m4string_s m4string;
typedef struct m4eval_arg_s m4eval_arg;

struct m4string_s {
    const char *addr;
    m4int len;
};

struct m4eval_arg_s {
    const m4word *w; /* NULL if parsed word is a number */
    m4int n;
    m4int err;
};

void m4th_string_print(m4string str, FILE *out);

m4int m4th_string_equals(m4string a, const m4countedstring *b);

/* warning: str must end with '\0' */
m4int m4th_string_to_int(m4string str, m4int *out_n);

/** temporary C implementation of (read) */
m4string m4th_read(m4th *m);

/** temporary C implementation of (parse) */
m4eval_arg m4th_parse(m4th *m, m4string key);

/** temporary C implementation of (eval) */
m4int m4th_eval(m4th *m, m4eval_arg arg);

/** temporary C implementation of (repl) */
m4int m4th_repl(m4th *m);

#endif /* M4TH_IMPL_H */