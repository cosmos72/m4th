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

#ifndef M4TH_COMMON_ASM_H
#define M4TH_COMMON_ASM_H

#include "../m4th_macro.h" /* also define public macros */
#include "countedstring_asm.h"
#include "func_asm.h"
#include "m4th_asm.h"
#include "word_asm.h"

#define CAT2_(a, b) a##b
#define CAT2(a, b) CAT2_(a, b)

/* works only for 1..10 arguments. broken for zero arguments */
#define COUNT_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, n, ...) n
#define COUNT_ARGS(...) COUNT_ARGS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define WRAP_ARGS_COMMA_1(x, _1) x(_1)
#define WRAP_ARGS_COMMA_2(x, _1, _2) x(_1), x(_2)
#define WRAP_ARGS_COMMA_3(x, _1, _2, _3) x(_1), x(_2), x(_3)
#define WRAP_ARGS_COMMA_4(x, _1, _2, _3, _4) x(_1), x(_2), x(_3), x(_4)
#define WRAP_ARGS_COMMA_5(x, _1, _2, _3, _4, _5) x(_1), x(_2), x(_3), x(_4), x(_5)
#define WRAP_ARGS_COMMA_6(x, _1, _2, _3, _4, _5, _6) x(_1), x(_2), x(_3), x(_4), x(_5), x(_6)
#define WRAP_ARGS_COMMA_7(x, _1, _2, _3, _4, _5, _6, _7)                                           \
    x(_1), x(_2), x(_3), x(_4), x(_5), x(_6), x(_7)
#define WRAP_ARGS_COMMA_8(x, _1, _2, _3, _4, _5, _6, _7, _8)                                       \
    x(_1), x(_2), x(_3), x(_4), x(_5), x(_6), x(_7), x(_8)
#define WRAP_ARGS_COMMA_9(x, _1, _2, _3, _4, _5, _6, _7, _8, _9)                                   \
    x(_1), x(_2), x(_3), x(_4), x(_5), x(_6), x(_7), x(_8), x(_9)
#define WRAP_ARGS_COMMA_10(x, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10)                             \
    x(_1), x(_2), x(_3), x(_4), x(_5), x(_6), x(_7), x(_8), x(_9), x(_10)

#define WRAP_ARGS_COMMA(x, ...) CAT2(WRAP_ARGS_COMMA_, COUNT_ARGS(__VA_ARGS__))(x, __VA_ARGS__)

/* internal m4th macros used (mostly) by assembly */

#define M4FLAG_CONSUMES_IP_SZ CAT2(M4FLAG_CONSUMES_IP_, SZ)

/* clang-format off */

/* align functions at 8 bytes. */
/* on x86_64, aligning at 16 bytes should be faster, but wastes more memory */
#define P_FUNC_ALIGN 3
#define P_DATA_ALIGN 3

#define DATA_ALIGN()                                                                               \
    .p2align P_DATA_ALIGN, 0;

/* expand AT(addr) -> AT0(addr) and AT(addr, i) -> ATx(addr, i) */
#define AT_0(addr, i)                   AT0(addr)
#define AT_x(addr, i)                   ATx(addr, i)
#define AT_(addr, i, kind, ...)         AT_##kind(addr, i)
#define AT(...)                         AT_(__VA_ARGS__, x, 0)

#if 0
/* expand IPUSH(a) -> IPUSH1(a) and IPUSH(a, b) -> IPUSH2(a, b) */
#define IPUSH_1(a, b)                   IPUSH1(a)
#define IPUSH_2(a, b)                   IPUSH2(a, b)
#define IPUSH_(a, b, kind, ...)         IPUSH_##kind(a, b)
#define IPUSH(...)                      IPUSH_(__VA_ARGS__, 2, 1)
#endif /* 0 */

/* clang-format on */

#endif /* M4TH_COMMON_ASM_H */
