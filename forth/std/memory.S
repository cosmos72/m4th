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

/* clang-format off */

DICT_WORDS_FORTH_MEMORY(WORDNAME)

WORD_START(allocate, LASTWORD)
    WORD_DSTACK(1,2)
    WORD_RSTACK(0,0)
    WORD_STACK_NONE()
    WORD_ASM_NONE()
    WORD_FLAGS(M6FLAG_INLINE)
    WORD_CODE(
        _c_locals_save_,
            _c_arg_1_, _c_call_, CELL(malloc), _c_ret_1_,       /* ( a-addr|0          ) */
        _c_locals_load_,
        dup, zero_equal,                                        /* ( a-addr|0 t|f      ) */
        _lit_, T(M6ERR_FAILED_TO_ALLOCATE), and,                /* ( a-addr 0 | 0 err  ) */
        exit
    )
WORD_END(allocate)
WORD_START(free, allocate)
    WORD_DSTACK(1,0)
    WORD_RSTACK(0,0)
    WORD_STACK_NONE()
    WORD_ASM_NONE()
    WORD_FLAGS(M6FLAG_INLINE)
    WORD_CODE(
        _c_locals_save_,
            _c_arg_1_, _c_call_, CELL(free), _c_ret_0_,         /* (                   ) */
        _c_locals_load_,
        zero, exit                                              /* ( 0                 ) */
    )
WORD_END(free)
WORD_START(resize, free)
    WORD_DSTACK(1,1)
    WORD_RSTACK(0,0)
    WORD_STACK_NONE()
    WORD_ASM_NONE()
    WORD_FLAGS(M6FLAG_INLINE)
    WORD_CODE(
        trail,                                                  /* ( a-addr1 a-addr1 u ) */
        _c_locals_save_,
            _c_arg_2_, _c_call_, CELL(realloc), _c_ret_1_,      /* ( a-addr1 a-addr2   ) */
        _c_locals_load_,
        _q_if_, T(3),                                           /* ( a-addr1 a-addr2   ) */
            nip, zero,                                          /* ( a-addr2 0         ) */
        _else_, T(4),                                           /* ( a-addr1 0         ) */
            drop, _lit_, T(M6ERR_FAILED_TO_ALLOCATE),           /* ( a-addr1 err       ) */
        then, exit
    )
WORD_END(resize)

#undef LASTWORD
#define LASTWORD resize
