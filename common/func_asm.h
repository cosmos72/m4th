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

#ifndef M4TH_COMMON_FUNC_ASM_H
#define M4TH_COMMON_FUNC_ASM_H

/* clang-format off */

#define FUNC_ALIGN()                                                                               \
    .p2align P_FUNC_ALIGN;

#define FUNC_SYM(name)                                                                             \
    m4##name

#define FUNC_SYM_NEXT(name)                                                                        \
    .Lfunc.name.next

#define FUNC_DEF_SYM(name)                                                                         \
    FUNC_ALIGN()                                                                                   \
    .globl FUNC_SYM(name);                                                                         \
    .type FUNC_SYM(name), @function;                                                               \
    FUNC_SYM(name):

#define FUNC_START(name)                                                                           \
    FUNC_DEF_SYM(name)                                                                             \
    .cfi_startproc;

#define FUNC_RAWEND(name)                                                                          \
    .cfi_endproc;                                                                                  \
    .size FUNC_SYM(name), . - FUNC_SYM(name);

#define FUNC_END(name)                                                                             \
    FUNC_SYM_NEXT(name):                                                                           \
    NEXT()                                                                                         \
    FUNC_RAWEND(name)

/* clang-format on */

#endif /* M4TH_COMMON_FUNC_ASM_H */
