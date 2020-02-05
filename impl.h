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

typedef struct m4pair_s m4pair;

struct m4pair_s {
    m4cell num;
    m4cell err;
};

/** temporary C implementation of (number) */
m4pair m4string_to_int(m4string str);

/** temporary C implementation of (read) */
m4string m4th_read(m4th *m);

/** temporary C implementation of (parse) */
m4pair m4th_parse(m4th *m, m4string key);

/** temporary C implementation of (eval) */
m4cell m4th_eval(m4th *m, m4pair arg);

/** temporary C implementation of (repl) */
m4cell m4th_repl(m4th *m);

#endif /* M4TH_IMPL_H */
