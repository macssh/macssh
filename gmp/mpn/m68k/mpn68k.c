/*
#define MOTOROLA_SYNTAX 1
#include "asm_syntax.h"
*/

#pragma  code68020 on

#if __MC68020__
#ifndef mc68020
#define mc68020 1
#endif
#endif

/* disable link a6 sequence */
#pragma a6frames off

#define R(r)r
#define MEM(base)(base)
#define MEM_DISP(base,displacement)displacement(base)
#define MEM_INDX(base,idx,size_suffix)(base,idx.size_suffix)
#define MEM_INDX1(base,idx,size_suffix,scale)(base,idx.size_suffix*scale)
#define MEM_PREDEC(memory_base)-(memory_base)
#define MEM_POSTINC(memory_base)(memory_base)+
#define L(label) label
#define lea LEA
#define movel MOVE.L
#define moveml MOVEM.L
#define moveql MOVEQ.L
#define cmpl CMP.L
#define orl OR.L
#define clrl CLR.L
#define eorw EOR.W
#define lsrl LSR.L
#define lsll LSL.L
#define asrl ASR.L
#define asll ASL.L
#define roxrl ROXR.L
#define roxll ROXL.L
#define addl ADD.L
#define addxl ADDX.L
#define addql ADDQ.L
#define subl SUB.L
#define subxl SUBX.L
#define subql SUBQ.L
#define negl NEG.L
#define mulul MULU.L
#define bcc BCC
#define bcs BCS
#define bls BLS
#define beq BEQ
#define bne BNE
#define bra BRA
#define dbf DBF
#define rts RTS
#define d0 D0
#define d1 D1
#define d2 D2
#define d3 D3
#define d4 D4
#define d5 D5
#define d6 D6
#define d7 D7
#define a0 A0
#define a1 A1
#define a2 A2
#define a3 A3
#define a4 A4
#define a5 A5
#define a6 A6
#define a7 A7
#define sp SP


asm void __gmpn_add_n()
{
/* Save used registers on the stack.  */
	movel	R(d2),MEM_PREDEC(sp)
	movel	R(a2),MEM_PREDEC(sp)

/* Copy the arguments to registers.  Better use movem?  */
	movel	MEM_DISP(sp,12),R(a2)
	movel	MEM_DISP(sp,16),R(a0)
	movel	MEM_DISP(sp,20),R(a1)
	movel	MEM_DISP(sp,24),R(d2)

	eorw	#1,R(d2)
	lsrl	#1,R(d2)
	bcc	L(L1)
	subql	#1,R(d2)	/* clears cy as side effect */

L(Loop:)
	movel	MEM_POSTINC(a0),R(d0)
	movel	MEM_POSTINC(a1),R(d1)
	addxl	R(d1),R(d0)
	movel	R(d0),MEM_POSTINC(a2)
L(L1:)	movel	MEM_POSTINC(a0),R(d0)
	movel	MEM_POSTINC(a1),R(d1)
	addxl	R(d1),R(d0)
	movel	R(d0),MEM_POSTINC(a2)

	dbf	R(d2),L(Loop)		/* loop until 16 lsb of %4 == -1 */
	subxl	R(d0),R(d0)	/* d0 <= -cy; save cy as 0 or -1 in d0 */
	subl	#0x10000,R(d2)
	bcs	L(L2)
	addl	R(d0),R(d0)	/* restore cy */
	bra	L(Loop)

L(L2:)
	negl	R(d0)

/* Restore used registers from stack frame.  */
	movel	MEM_POSTINC(sp),R(a2)
	movel	MEM_POSTINC(sp),R(d2)
	rts
}

asm void __gmpn_addmul_1()
{
#define res_ptr a0
#define s1_ptr a1
#define s1_size d2
#define s2_limb d4

/* Save used registers on the stack.  */
	moveml	R(d2)-R(d5),MEM_PREDEC(sp)

/* Copy the arguments to registers.  Better use movem?  */
	movel	MEM_DISP(sp,20),R(res_ptr)
	movel	MEM_DISP(sp,24),R(s1_ptr)
	movel	MEM_DISP(sp,28),R(s1_size)
	movel	MEM_DISP(sp,32),R(s2_limb)

	eorw	#1,R(s1_size)
	clrl	R(d1)
	clrl	R(d5)
	lsrl	#1,R(s1_size)
	bcc	L(L1)
	subql	#1,R(s1_size)
	subl	R(d0),R(d0)		/* (d0,cy) <= (0,0) */

L(Loop:)
	movel	MEM_POSTINC(s1_ptr),R(d3)
	mulul	R(s2_limb),R(d1):R(d3)
	addxl	R(d0),R(d3)
	addxl	R(d5),R(d1)
	addl	R(d3),MEM_POSTINC(res_ptr)
L(L1:)	movel	MEM_POSTINC(s1_ptr),R(d3)
	mulul	R(s2_limb),R(d0):R(d3)
	addxl	R(d1),R(d3)
	addxl	R(d5),R(d0)
	addl	R(d3),MEM_POSTINC(res_ptr)

	dbf	R(s1_size),L(Loop)
	addxl	R(d5),R(d0)
	subl	#0x10000,R(s1_size)
	bcc	L(Loop)

/* Restore used registers from stack frame.  */
	moveml	MEM_POSTINC(sp),R(d2)-R(d5)
	rts

#undef res_ptr
#undef s1_ptr
#undef s1_size
#undef s2_limb
}

asm void __gmpn_lshift()
{
#define res_ptr a1
#define s_ptr a0
#define s_size d6
#define cnt d4

/* Save used registers on the stack.  */
	moveml	R(d2)-R(d6)/R(a2),MEM_PREDEC(sp)

/* Copy the arguments to registers.  */
	movel	MEM_DISP(sp,28),R(res_ptr)
	movel	MEM_DISP(sp,32),R(s_ptr)
	movel	MEM_DISP(sp,36),R(s_size)
	movel	MEM_DISP(sp,40),R(cnt)

	moveql	#1,R(d5)
	cmpl	R(d5),R(cnt)
	bne	L(Lnormal)
	cmpl	R(s_ptr),R(res_ptr)
	bls	L(Lspecial)		/* jump if s_ptr >= res_ptr */
#if (defined (__mc68020__) || defined (__NeXT__) || defined(mc68020))
	lea	MEM_INDX1(s_ptr,s_size,l,4),R(a2)
#else /* not mc68020 */
	movel	R(s_size),R(d0)
	asll	#2,R(d0)
	lea	MEM_INDX(s_ptr,d0,l),R(a2)
#endif
	cmpl	R(res_ptr),R(a2)
	bls	L(Lspecial)		/* jump if res_ptr >= s_ptr + s_size */

L(Lnormal:)
	moveql	#32,R(d5)
	subl	R(cnt),R(d5)

#if (defined (__mc68020__) || defined (__NeXT__) || defined(mc68020))
	lea	MEM_INDX1(s_ptr,s_size,l,4),R(s_ptr)
	lea	MEM_INDX1(res_ptr,s_size,l,4),R(res_ptr)
#else /* not mc68000 */
	movel	R(s_size),R(d0)
	asll	#2,R(d0)
	addl	R(s_size),R(s_ptr)
	addl	R(s_size),R(res_ptr)
#endif
	movel	MEM_PREDEC(s_ptr),R(d2)
	movel	R(d2),R(d0)
	lsrl	R(d5),R(d0)		/* compute carry limb */

	lsll	R(cnt),R(d2)
	movel	R(d2),R(d1)
	subql	#1,R(s_size)
	beq	L(Lend)
	lsrl	#1,R(s_size)
	bcs	L(L1)
	subql	#1,R(s_size)

L(Loop:)
	movel	MEM_PREDEC(s_ptr),R(d2)
	movel	R(d2),R(d3)
	lsrl	R(d5),R(d3)
	orl	R(d3),R(d1)
	movel	R(d1),MEM_PREDEC(res_ptr)
	lsll	R(cnt),R(d2)
L(L1:)
	movel	MEM_PREDEC(s_ptr),R(d1)
	movel	R(d1),R(d3)
	lsrl	R(d5),R(d3)
	orl	R(d3),R(d2)
	movel	R(d2),MEM_PREDEC(res_ptr)
	lsll	R(cnt),R(d1)

	dbf	R(s_size),L(Loop)
	subl	#0x10000,R(s_size)
	bcc	L(Loop)

L(Lend:)
	movel	R(d1),MEM_PREDEC(res_ptr) /* store least significant limb */

/* Restore used registers from stack frame.  */
	moveml	MEM_POSTINC(sp),R(d2)-R(d6)/R(a2)
	rts

/* We loop from least significant end of the arrays, which is only
   permissable if the source and destination don't overlap, since the
   function is documented to work for overlapping source and destination.  */

L(Lspecial:)
	clrl	R(d0)			/* initialize carry */
	eorw	#1,R(s_size)
	lsrl	#1,R(s_size)
	bcc	L(LL1)
	subql	#1,R(s_size)

L(LLoop:)
	movel	MEM_POSTINC(s_ptr),R(d2)
	addxl	R(d2),R(d2)
	movel	R(d2),MEM_POSTINC(res_ptr)
L(LL1:)
	movel	MEM_POSTINC(s_ptr),R(d2)
	addxl	R(d2),R(d2)
	movel	R(d2),MEM_POSTINC(res_ptr)

	dbf	R(s_size),L(LLoop)
	addxl	R(d0),R(d0)		/* save cy in lsb */
	subl	#0x10000,R(s_size)
	bcs	L(LLend)
	lsrl	#1,R(d0)		/* restore cy */
	bra	L(LLoop)

L(LLend:)
/* Restore used registers from stack frame.  */
	moveml	MEM_POSTINC(sp),R(d2)-R(d6)/R(a2)
	rts

#undef res_ptr
#undef s_ptr
#undef s_size
#undef cnt
}

asm void __gmpn_mul_1()
{
#define res_ptr a0
#define s1_ptr a1
#define s1_size d2
#define s2_limb d4

/* Save used registers on the stack.  */
	moveml	R(d2)-R(d4),MEM_PREDEC(sp)
#if 0
	movel	R(d2),MEM_PREDEC(sp)
	movel	R(d3),MEM_PREDEC(sp)
	movel	R(d4),MEM_PREDEC(sp)
#endif

/* Copy the arguments to registers.  Better use movem?  */
	movel	MEM_DISP(sp,16),R(res_ptr)
	movel	MEM_DISP(sp,20),R(s1_ptr)
	movel	MEM_DISP(sp,24),R(s1_size)
	movel	MEM_DISP(sp,28),R(s2_limb)

	eorw	#1,R(s1_size)
	clrl	R(d1)
	lsrl	#1,R(s1_size)
	bcc	L(L1)
	subql	#1,R(s1_size)
	subl	R(d0),R(d0)	/* (d0,cy) <= (0,0) */

L(Loop:)
	movel	MEM_POSTINC(s1_ptr),R(d3)
	mulul	R(s2_limb),R(d1):R(d3)
	addxl	R(d0),R(d3)
	movel	R(d3),MEM_POSTINC(res_ptr)
L(L1:)	movel	MEM_POSTINC(s1_ptr),R(d3)
	mulul	R(s2_limb),R(d0):R(d3)
	addxl	R(d1),R(d3)
	movel	R(d3),MEM_POSTINC(res_ptr)

	dbf	R(s1_size),L(Loop)
	clrl	R(d3)
	addxl	R(d3),R(d0)
	subl	#0x10000,R(s1_size)
	bcc	L(Loop)

/* Restore used registers from stack frame.  */
	moveml	MEM_POSTINC(sp),R(d2)-R(d4)
#if 0
	movel	MEM_POSTINC(sp),R(d4)
	movel	MEM_POSTINC(sp),R(d3)
	movel	MEM_POSTINC(sp),R(d2)
#endif
	rts
#undef res_ptr
#undef s1_ptr
#undef s1_size
#undef s2_limb
}


asm void __gmpn_rshift()
{
#define res_ptr a1
#define s_ptr a0
#define s_size d6
#define cnt d4

/* Save used registers on the stack.  */
	moveml	R(d2)-R(d6)/R(a2),MEM_PREDEC(sp)

/* Copy the arguments to registers.  */
	movel	MEM_DISP(sp,28),R(res_ptr)
	movel	MEM_DISP(sp,32),R(s_ptr)
	movel	MEM_DISP(sp,36),R(s_size)
	movel	MEM_DISP(sp,40),R(cnt)

	moveql	#1,R(d5)
	cmpl	R(d5),R(cnt)
	bne	L(Lnormal)
	cmpl	R(res_ptr),R(s_ptr)
	bls	L(Lspecial)		/* jump if res_ptr >= s_ptr */
#if (defined (__mc68020__) || defined (__NeXT__) || defined(mc68020))
	lea	MEM_INDX1(res_ptr,s_size,l,4),R(a2)
#else /* not mc68020 */
	movel	R(s_size),R(d0)
	asll	#2,R(d0)
	lea	MEM_INDX(res_ptr,d0,l),R(a2)
#endif
	cmpl	R(s_ptr),R(a2)
	bls	L(Lspecial)		/* jump if s_ptr >= res_ptr + s_size */

L(Lnormal:)
	moveql	#32,R(d5)
	subl	R(cnt),R(d5)
	movel	MEM_POSTINC(s_ptr),R(d2)
	movel	R(d2),R(d0)
	lsll	R(d5),R(d0)		/* compute carry limb */

	lsrl	R(cnt),R(d2)
	movel	R(d2),R(d1)
	subql	#1,R(s_size)
	beq	L(Lend)
	lsrl	#1,R(s_size)
	bcs	L(L1)
	subql	#1,R(s_size)

L(Loop:)
	movel	MEM_POSTINC(s_ptr),R(d2)
	movel	R(d2),R(d3)
	lsll	R(d5),R(d3)
	orl	R(d3),R(d1)
	movel	R(d1),MEM_POSTINC(res_ptr)
	lsrl	R(cnt),R(d2)
L(L1:)
	movel	MEM_POSTINC(s_ptr),R(d1)
	movel	R(d1),R(d3)
	lsll	R(d5),R(d3)
	orl	R(d3),R(d2)
	movel	R(d2),MEM_POSTINC(res_ptr)
	lsrl	R(cnt),R(d1)

	dbf	R(s_size),L(Loop)
	subl	#0x10000,R(s_size)
	bcc	L(Loop)

L(Lend:)
	movel	R(d1),MEM(res_ptr) /* store most significant limb */

/* Restore used registers from stack frame.  */
	moveml	MEM_POSTINC(sp),R(d2)-R(d6)/R(a2)
	rts

/* We loop from most significant end of the arrays, which is only
   permissable if the source and destination don't overlap, since the
   function is documented to work for overlapping source and destination.  */

L(Lspecial:)
#if (defined (__mc68020__) || defined (__NeXT__) || defined(mc68020))
	lea	MEM_INDX1(s_ptr,s_size,l,4),R(s_ptr)
	lea	MEM_INDX1(res_ptr,s_size,l,4),R(res_ptr)
#else /* not mc68000 */
	movel	R(s_size),R(d0)
	asll	#2,R(d0)
	addl	R(s_size),R(s_ptr)
	addl	R(s_size),R(res_ptr)
#endif

	clrl	R(d0)			/* initialize carry */
	eorw	#1,R(s_size)
	lsrl	#1,R(s_size)
	bcc	L(LL1)
	subql	#1,R(s_size)

L(LLoop:)
	movel	MEM_PREDEC(s_ptr),R(d2)
	roxrl	#1,R(d2)
	movel	R(d2),MEM_PREDEC(res_ptr)
L(LL1:)
	movel	MEM_PREDEC(s_ptr),R(d2)
	roxrl	#1,R(d2)
	movel	R(d2),MEM_PREDEC(res_ptr)

	dbf	R(s_size),L(LLoop)
	roxrl	#1,R(d0)		/* save cy in msb */
	subl	#0x10000,R(s_size)
	bcs	L(LLend)
	addl	R(d0),R(d0)		/* restore cy */
	bra	L(LLoop)

L(LLend:)
/* Restore used registers from stack frame.  */
	moveml	MEM_POSTINC(sp),R(d2)-R(d6)/R(a2)
	rts

#undef res_ptr
#undef s_ptr
#undef s_size
#undef cnt
}

asm void __gmpn_sub_n()
{
/* Save used registers on the stack.  */
	movel	R(d2),MEM_PREDEC(sp)
	movel	R(a2),MEM_PREDEC(sp)

/* Copy the arguments to registers.  Better use movem?  */
	movel	MEM_DISP(sp,12),R(a2)
	movel	MEM_DISP(sp,16),R(a0)
	movel	MEM_DISP(sp,20),R(a1)
	movel	MEM_DISP(sp,24),R(d2)

	eorw	#1,R(d2)
	lsrl	#1,R(d2)
	bcc	L(L1)
	subql	#1,R(d2)	/* clears cy as side effect */

L(Loop:)
	movel	MEM_POSTINC(a0),R(d0)
	movel	MEM_POSTINC(a1),R(d1)
	subxl	R(d1),R(d0)
	movel	R(d0),MEM_POSTINC(a2)
L(L1:)	movel	MEM_POSTINC(a0),R(d0)
	movel	MEM_POSTINC(a1),R(d1)
	subxl	R(d1),R(d0)
	movel	R(d0),MEM_POSTINC(a2)

	dbf	R(d2),L(Loop)		/* loop until 16 lsb of %4 == -1 */
	subxl	R(d0),R(d0)	/* d0 <= -cy; save cy as 0 or -1 in d0 */
	subl	#0x10000,R(d2)
	bcs	L(L2)
	addl	R(d0),R(d0)	/* restore cy */
	bra	L(Loop)

L(L2:)
	negl	R(d0)

/* Restore used registers from stack frame.  */
	movel	MEM_POSTINC(sp),R(a2)
	movel	MEM_POSTINC(sp),R(d2)
	rts
}

asm void __gmpn_submul_1()
{
#define res_ptr a0
#define s1_ptr a1
#define s1_size d2
#define s2_limb d4

/* Save used registers on the stack.  */
	moveml	R(d2)-R(d5),MEM_PREDEC(sp)

/* Copy the arguments to registers.  Better use movem?  */
	movel	MEM_DISP(sp,20),R(res_ptr)
	movel	MEM_DISP(sp,24),R(s1_ptr)
	movel	MEM_DISP(sp,28),R(s1_size)
	movel	MEM_DISP(sp,32),R(s2_limb)

	eorw	#1,R(s1_size)
	clrl	R(d1)
	clrl	R(d5)
	lsrl	#1,R(s1_size)
	bcc	L(L1)
	subql	#1,R(s1_size)
	subl	R(d0),R(d0)	/* (d0,cy) <= (0,0) */

L(Loop:)
	movel	MEM_POSTINC(s1_ptr),R(d3)
	mulul	R(s2_limb),R(d1):R(d3)
	addxl	R(d0),R(d3)
	addxl	R(d5),R(d1)
	subl	R(d3),MEM_POSTINC(res_ptr)
L(L1:)	movel	MEM_POSTINC(s1_ptr),R(d3)
	mulul	R(s2_limb),R(d0):R(d3)
	addxl	R(d1),R(d3)
	addxl	R(d5),R(d0)
	subl	R(d3),MEM_POSTINC(res_ptr)

	dbf	R(s1_size),L(Loop)
	addxl	R(d5),R(d0)
	subl	#0x10000,R(s1_size)
	bcc	L(Loop)

/* Restore used registers from stack frame.  */
	moveml	MEM_POSTINC(sp),R(d2)-R(d5)
	rts

#undef res_ptr
#undef s1_ptr
#undef s1_size
#undef s2_limb
}

#pragma a6frames reset
