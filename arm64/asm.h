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

#ifndef M4TH_ARM64_ASM_H
#define M4TH_ARM64_ASM_H

#include "../asm_gcc.h"

#define SZ   8    /* width of registers and cells, in bytes */
#define SZ2  16   /* SZ * 2 */
#define SZ3  24   /* SZ * 3 */
#define SZ4  32   /* SZ * 4 */
#define SZ5  40   /* SZ * 5 */
#define SZ6  48   /* SZ * 6 */

#define IMM(constant) constant /* immediate constant */

#define REG1 x0 /* scratch register 1 */
#define REG2 x1 /* scratch register 2 */
#define REG3 x2 /* scratch register 3 */
#define REG4 x3 /* scratch register 4 */
/* additional scratch registers: x4 .. x10 */

#define DTOP x11 /* value of first data stack element */
#define DSTK x12 /* pointer to second data stack element */

#define IP   x13 /* instruction pointer */
#define RSTK x14 /* pointer to return stack */
#define M4TH x15 /* pointer to C struct m4th */

#define DPUSH(val)    str       val, [DSTK, -SZ]!; /* push val to second data stack element */
#define DPOP(val)     ldr       val, [DSTK], SZ;   /* pop second data stack element into val */

#define RPUSH(val)    str       val, [RSTK, -SZ]!; /* push val to return stack */
#define RPOP(val)     ldr       val, [RSTK], SZ;   /* pop first return stack element into val */

#define NEXT()        ldr       REG1, [IP, SZ]!;   /* jump to next instruction */ \
                      br        REG1;


#define ADD2(src,dst)    add  dst, dst, src;    /* dst += src     */
#define DIV3(a,b,dst)    sdiv dst, a, b;        /* dst  = a / b   */
#define LOAD(addr,dst)   ldr  dst, [addr];      /* dst  = addr[0] */
#define IGET(addr,i,dst) ldr  dst, [addr, i];   /* dst  = addr[i] */
#define ISET(src,addr,i) str  src, [addr, i];   /* addr[i] = src  */
#define MOVE(src,dst)    mov  dst, src;         /* dst  = src     */
#define MUL2(src,dst)    mul  dst, dst, src;    /* dst *= src     */
#define NEG1(dst)        neg  dst, dst;         /* dst  = -dst    */
#define SUB2(src,dst)    sub  dst, dst, src;    /* dst -= src     */
#define SUB3(a,b,dst)    sub  dst, a, b;        /* dst  = a - b   */
#define STOR(src,addr)   str  src, [addr];      /* addr[0] = src  */
#define ZERO(dst)        mov  dst, 0;           /* dst  = 0       */


#endif /* M4TH_ARM64_ASM_H */

