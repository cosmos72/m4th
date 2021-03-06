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

DICT_WORDS_FORTH_TOOLS(WORDNAME)

WORD_START(dot_s, LASTWORD)
    WORD_DSTACK(0,0)
    WORD_RSTACK(0,0)
    WORD_STACK_NONE()
    WORD_ASM_NONE()
    WORD_FLAGS(M6FLAG_INLINE)
    WORD_CODE(
        sp_fetch, depth, one_minus, minus_one,                       /* ( dstk depth -1 ) */
        _c_locals_save_,
            _c_arg_3_, _c_call_, CELL(m6slice_print_stdout), _c_ret_0_,
        _c_locals_load_, exit
    )
WORD_END(dot_s)
WORD_START(see, dot_s)
    WORD_DSTACK(0,0)
    WORD_RSTACK(0,0)
    WORD_STACK_NONE()
    WORD_ASM_NONE()
    WORD_FLAGS(M6FLAG_INLINE)
    WORD_CODE(
        CALL(parse_name), two_dup, CALL(string_to_name),       /* ( c-addr u nt|0 wid|0 -1|0|1 ) */
        two_drop,                                              /* ( c-addr u nt|0 ) */
        _q_if0_, T(6),                                         /* ( c-addr u nt|0 ) */
            drop, ex_message_store,                            /* (               ) */
            _lit_, T(M6ERR_UNDEFINED_WORD), throw,             /* (               ) */
        then,                                                  /* ( c-addr u nt   ) */
        nip, nip,                                              /* ( nt            ) */
        CALL(see_name), exit
    )
WORD_END(see)

#undef LASTWORD
#define LASTWORD see
