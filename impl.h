/**
 * Copyright (C) 2020 Massimiliano Ghilardi
 *
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
    union {
        m4cell num;
        const m4word *w;
    };
    m4cell err;
};

/** wrapper around string>number */
m4pair m4string_to_number(m4th *m, m4string str);

/** temporary C implementation of (read) */
m4string m4th_parse_name(m4th *m);

/** temporary C implementation of (parse) */
m4pair m4th_resolve(m4th *m, m4string key);

/** temporary C implementation of (eval) */
m4cell m4th_eval(m4th *m, m4pair arg);

/** temporary C implementation of (eval) */
m4cell m4th_repl(m4th *m);

/** used for testing and benchmark */
extern m4cell m4th_crctable[256];
void m4th_crcinit(m4cell table[256]);
uint32_t m4th_crc1byte(uint32_t crc, unsigned char byte);
uint32_t m4th_crcstring(m4string str);

#endif /* M4TH_IMPL_H */
