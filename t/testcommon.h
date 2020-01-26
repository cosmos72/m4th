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

#ifndef M4TH_T_TEST_IMPL_H
#define M4TH_T_TEST_IMPL_H

#include "../test.h"

enum {
    tfalse = (m4cell)0,
    ttrue = (m4cell)-1,
    m4enum_per_m4cell = SZ / SZe, /* # of m4enum needed to store an m4cell */
    callsz = 1 + m4enum_per_m4cell,
};

/** padding needed for element-to-element conversion from m4cell[] to m4enum[] */
#define CELL(n) (m4cell)(n), 0, 0, 0
/* store numeric constant as wide as m4enum in m4enum[] */
#define E(n) (n)
#define XT(name) CELL(m4word_##name.code)
#define CALLXT(name) m4_call_, XT(name)

#endif /* M4TH_T_TEST_IMPL_H */
