

asm void __gmpn_add_n()
{
	mtctr	r6		//# copy size into CTR
	addic	r0,r0,0		//# clear cy
	lwz	r8,0(r4)	//# load least significant s1 limb
	lwz	r0,0(r5)	//# load least significant s2 limb
	addi	r3,r3,-4	//# offset res_ptr, it's updated before it's used
	bdz	Lend		//# If done, skip loop
Loop:	lwz	r9,4(r4)	//# load s1 limb
	lwz	r10,4(r5)	//# load s2 limb
	adde	r7,r0,r8	//# add limbs with cy, set cy
	stw	r7,4(r3)	//# store result limb
	bdz	Lexit		//# decrement CTR and exit if done
	lwzu	r8,8(r4)	//# load s1 limb and update s1_ptr
	lwzu	r0,8(r5)	//# load s2 limb and update s2_ptr
	adde	r7,r10,r9	//# add limbs with cy, set cy
	stwu	r7,8(r3)	//# store result limb and update res_ptr
	bdnz	Loop		//# decrement CTR and loop back

Lend:	adde	r7,r0,r8
	stw	r7,4(r3)	//# store ultimate result limb
	li	r3,0		//# load cy into ...
	addze	r3,r3		//# ... return value register
	blr
Lexit:	adde	r7,r10,r9
	stw	r7,8(r3)
	li	r3,0		//# load cy into ...
	addze	r3,r3		//# ... return value register
}

asm void __gmpn_addmul_1()
{
	cmpi	cr0,r5,9	//# more than 9 limbs?
	bgt	cr0,Lbig	//# branch if more than 9 limbs

	mtctr	r5
	lwz	r0,0(r4)
	mullw	r7,r0,r6
	mulhwu	r10,r0,r6
	lwz	r9,0(r3)
	addc	r8,r7,r9
	addi	r3,r3,-4
	bdz	Lend
Lloop:
	lwzu	r0,4(r4)
	stwu	r8,4(r3)
	mullw	r8,r0,r6
	adde	r7,r8,r10
	mulhwu	r10,r0,r6
	lwz	r9,4(r3)
	addze	r10,r10
	addc	r8,r7,r9
	bdnz	Lloop
Lend:	stw	r8,4(r3)
	addze	r3,r10
	blr

Lbig:	stmw	r30,-32(r1)
	addi	r5,r5,-1
	srwi	r0,r5,2
	mtctr	r0

	lwz	r7,0(r4)
	mullw	r8,r7,r6
	mulhwu	r0,r7,r6
	lwz	r7,0(r3)
	addc	r8,r8,r7
	stw	r8,0(r3)

LloopU:
	lwz	r7,4(r4)
	lwz	r12,8(r4)
	lwz	r30,12(r4)
	lwzu	r31,16(r4)
	mullw	r8,r7,r6
	mullw	r9,r12,r6
	mullw	r10,r30,r6
	mullw	r11,r31,r6
	adde	r8,r8,r0	//# add cy_limb
	mulhwu	r0,r7,r6
	lwz	r7,4(r3)
	adde	r9,r9,r0
	mulhwu	r0,r12,r6
	lwz	r12,8(r3)
	adde	r10,r10,r0
	mulhwu	r0,r30,r6
	lwz	r30,12(r3)
	adde	r11,r11,r0
	mulhwu	r0,r31,r6
	lwz	r31,16(r3)
	addze	r0,r0		//# new cy_limb
	addc	r8,r8,r7
	stw	r8,4(r3)
	adde	r9,r9,r12
	stw	r9,8(r3)
	adde	r10,r10,r30
	stw	r10,12(r3)
	adde	r11,r11,r31
	stwu	r11,16(r3)
	bdnz	LloopU

	andi.	r31,r5,3
	mtctr	r31
	beq	cr0,Lendx

LloopE:
	lwzu	r7,4(r4)
	mullw	r8,r7,r6
	adde	r8,r8,r0	//# add cy_limb
	mulhwu	r0,r7,r6
	lwz	r7,4(r3)
	addze	r0,r0		//# new cy_limb
	addc	r8,r8,r7
	stwu	r8,4(r3)
	bdnz	LloopE
Lendx:
	addze	r3,r0
	lmw	r30,-32(r1)
}

asm void __gmpn_lshift()
{
	cmpi	cr0,r5,12	//# more than 12 limbs?
	slwi	r0,r5,2
	add	r4,r4,r0	//# make r4 point at end of s1
	add	r7,r3,r0	//# make r7 point at end of res
	bgt	LBIG		//# branch if more than 12 limbs

	mtctr	r5		//# copy size into CTR
	subfic	r8,r6,32
	lwzu	r11,-4(r4)	//# load first s1 limb
	srw	r3,r11,r8	//# compute function return value
	bdz	Lend1

Loop:	lwzu	r10,-4(r4)
	slw	r9,r11,r6
	srw	r12,r10,r8
	or	r9,r9,r12
	stwu	r9,-4(r7)
	bdz	Lend2
	lwzu	r11,-4(r4)
	slw	r9,r10,r6
	srw	r12,r11,r8
	or	r9,r9,r12
	stwu	r9,-4(r7)
	bdnz	Loop

Lend1:	slw	r0,r11,r6
	stw	r0,-4(r7)
	blr
Lend2:	slw	r0,r10,r6
	stw	r0,-4(r7)
	blr

LBIG:
	stmw	r24,-32(r1)	//# save registers we are supposed to preserve
	lwzu	r9,-4(r4)
	subfic	r8,r6,32
	srw	r3,r9,r8	//# compute function return value
	slw	r0,r9,r6
	addi	r5,r5,-1

	andi.	r10,r5,3	//# count for spill loop
	beq	Le
	mtctr	r10
	lwzu	r28,-4(r4)
	bdz	Lxe0

Loop0:	slw	r12,r28,r6
	srw	r24,r28,r8
	lwzu	r28,-4(r4)
	or	r24,r0,r24
	stwu	r24,-4(r7)
	mr	r0,r12
	bdnz	Loop0		//# taken at most once!

Lxe0:	slw	r12,r28,r6
	srw	r24,r28,r8
	or	r24,r0,r24
	stwu	r24,-4(r7)
	mr	r0,r12

Le:	srwi	r5,r5,2		//# count for unrolled loop
	addi	r5,r5,-1
	mtctr	r5
	lwz	r28,-4(r4)
	lwz	r29,-8(r4)
	lwz	r30,-12(r4)
	lwzu	r31,-16(r4)

LoopU:	slw	r9,r28,r6
	srw	r24,r28,r8
	lwz	r28,-4(r4)
	slw	r10,r29,r6
	srw	r25,r29,r8
	lwz	r29,-8(r4)
	slw	r11,r30,r6
	srw	r26,r30,r8
	lwz	r30,-12(r4)
	slw	r12,r31,r6
	srw	r27,r31,r8
	lwzu	r31,-16(r4)
	or	r24,r0,r24
	stw	r24,-4(r7)
	or	r25,r9,r25
	stw	r25,-8(r7)
	or	r26,r10,r26
	stw	r26,-12(r7)
	or	r27,r11,r27
	stwu	r27,-16(r7)
	mr	r0,r12
	bdnz	LoopU

	slw	r9,r28,r6
	srw	r24,r28,r8
	slw	r10,r29,r6
	srw	r25,r29,r8
	slw	r11,r30,r6
	srw	r26,r30,r8
	slw	r12,r31,r6
	srw	r27,r31,r8
	or	r24,r0,r24
	stw	r24,-4(r7)
	or	r25,r9,r25
	stw	r25,-8(r7)
	or	r26,r10,r26
	stw	r26,-12(r7)
	or	r27,r11,r27
	stwu	r27,-16(r7)
	mr	r0,r12

	stw	r0,-4(r7)
	lmw	r24,-32(r1)	//# restore registers
}

asm void __gmpn_mul_1()
{
	mtctr	r5
	addi	r3,r3,-4	//# adjust res_ptr, it's offset before it's used
	li	r12,0		//# clear upper product reg
	addic	r0,r0,0		//# clear cy
//# Start software pipeline
	lwz	r8,0(r4)
	bdz	Lend3
	stmw	r30,-8(r1)	//# save registers we are supposed to preserve
	lwzu	r9,4(r4)
	mullw	r11,r8,r6
	mulhwu	r0,r8,r6
	bdz	Lend1
//# Software pipelined main loop
Loop:	lwz	r8,4(r4)
	mullw	r10,r9,r6
	adde	r30,r11,r12
	mulhwu	r12,r9,r6
	stw	r30,4(r3)
	bdz	Lend2
	lwzu	r9,8(r4)
	mullw	r11,r8,r6
	adde	r31,r10,r0
	mulhwu	r0,r8,r6
	stwu	r31,8(r3)
	bdnz	Loop
//# Finish software pipeline
Lend1:	mullw	r10,r9,r6
	adde	r30,r11,r12
	mulhwu	r12,r9,r6
	stw	r30,4(r3)
	adde	r31,r10,r0
	stwu	r31,8(r3)
	addze	r3,r12
	lmw	r30,-8(r1)	//# restore registers from stack
	blr
Lend2:	mullw	r11,r8,r6
	adde	r31,r10,r0
	mulhwu	r0,r8,r6
	stwu	r31,8(r3)
	adde	r30,r11,r12
	stw	r30,4(r3)
	addze	r3,r0
	lmw	r30,-8(r1)	//# restore registers from stack
	blr
Lend3:	mullw	r11,r8,r6
	stw	r11,4(r3)
	mulhwu	r3,r8,r6
}


asm void __gmpn_rshift()
{
	mtctr	r5		//# copy size into CTR
	addi	r7,r3,-4	//# move adjusted res_ptr to free return reg
	subfic	r8,r6,32
	lwz	r11,0(r4)	//# load first s1 limb
	slw	r3,r11,r8	//# compute function return value
	bdz	Lend1

Loop:	lwzu	r10,4(r4)
	srw	r9,r11,r6
	slw	r12,r10,r8
	or	r9,r9,r12
	stwu	r9,4(r7)
	bdz	Lend2
	lwzu	r11,4(r4)
	srw	r9,r10,r6
	slw	r12,r11,r8
	or	r9,r9,r12
	stwu	r9,4(r7)
	bdnz	Loop

Lend1:	srw	r0,r11,r6
	stw	r0,4(r7)
	blr

Lend2:	srw	r0,r10,r6
	stw	r0,4(r7)
}

asm void __gmpn_sub_n()
{
	mtctr	r6		//# copy size into CTR
	addic	r0,r6,-1	//# set cy
	lwz	r8,0(r4)	//# load least significant s1 limb
	lwz	r0,0(r5)	//# load least significant s2 limb
	addi	r3,r3,-4	//# offset res_ptr, it's updated before it's used
	bdz	Lend		//# If done, skip loop
Loop:	lwz	r9,4(r4)	//# load s1 limb
	lwz	r10,4(r5)	//# load s2 limb
	subfe	r7,r0,r8	//# subtract limbs with cy, set cy
	stw	r7,4(r3)	//# store result limb
	bdz	Lexit		//# decrement CTR and exit if done
	lwzu	r8,8(r4)	//# load s1 limb and update s1_ptr
	lwzu	r0,8(r5)	//# load s2 limb and update s2_ptr
	subfe	r7,r10,r9	//# subtract limbs with cy, set cy
	stwu	r7,8(r3)	//# store result limb and update res_ptr
	bdnz	Loop		//# decrement CTR and loop back

Lend:	subfe	r7,r0,r8
	stw	r7,4(r3)	//# store ultimate result limb
	subfe	r3,r0,r0	//# load !cy into ...
	subfic	r3,r3,0		//# ... return value register
	blr
Lexit:	subfe	r7,r10,r9
	stw	r7,8(r3)
	subfe	r3,r0,r0	//# load !cy into ...
	subfic	r3,r3,0		//# ... return value register
}

asm void __gmpn_submul_1()
{
	cmpi	cr0,r5,9	//# more than 9 limbs?
	bgt	cr0,Lbig	//# branch if more than 9 limbs

	mtctr	r5
	lwz	r0,0(r4)
	mullw	r7,r0,r6
	mulhwu	r10,r0,r6
	lwz	r9,0(r3)
	subfc	r8,r7,r9
	addc	r7,r7,r8	//# invert cy (r7 is junk)
	addi	r3,r3,-4
	bdz	Lend
Lloop:
	lwzu	r0,4(r4)
	stwu	r8,4(r3)
	mullw	r8,r0,r6
	adde	r7,r8,r10
	mulhwu	r10,r0,r6
	lwz	r9,4(r3)
	addze	r10,r10
	subfc	r8,r7,r9
	addc	r7,r7,r8	//# invert cy (r7 is junk)
	bdnz	Lloop
Lend:	stw	r8,4(r3)
	addze	r3,r10
	blr

Lbig:	stmw	r30,-32(r1)
	addi	r5,r5,-1
	srwi	r0,r5,2
	mtctr	r0

	lwz	r7,0(r4)
	mullw	r8,r7,r6
	mulhwu	r0,r7,r6
	lwz	r7,0(r3)
	subfc	r7,r8,r7
	addc	r8,r8,r7
	stw	r7,0(r3)

LloopU:
	lwz	r7,4(r4)
	lwz	r12,8(r4)
	lwz	r30,12(r4)
	lwzu	r31,16(r4)
	mullw	r8,r7,r6
	mullw	r9,r12,r6
	mullw	r10,r30,r6
	mullw	r11,r31,r6
	adde	r8,r8,r0	//# add cy_limb
	mulhwu	r0,r7,r6
	lwz	r7,4(r3)
	adde	r9,r9,r0
	mulhwu	r0,r12,r6
	lwz	r12,8(r3)
	adde	r10,r10,r0
	mulhwu	r0,r30,r6
	lwz	r30,12(r3)
	adde	r11,r11,r0
	mulhwu	r0,r31,r6
	lwz	r31,16(r3)
	addze	r0,r0		//# new cy_limb
	subfc	r7,r8,r7
	stw	r7,4(r3)
	subfe	r12,r9,r12
	stw	r12,8(r3)
	subfe	r30,r10,r30
	stw	r30,12(r3)
	subfe	r31,r11,r31
	stwu	r31,16(r3)
	subfe	r11,r11,r11	//# invert ...
	addic	r11,r11,1	//# ... carry
	bdnz	LloopU

	andi.	r31,r5,3
	mtctr	r31
	beq	cr0,Lendx

LloopE:
	lwzu	r7,4(r4)
	mullw	r8,r7,r6
	adde	r8,r8,r0	//# add cy_limb
	mulhwu	r0,r7,r6
	lwz	r7,4(r3)
	addze	r0,r0		//# new cy_limb
	subfc	r7,r8,r7
	addc	r8,r8,r7
	stwu	r7,4(r3)
	bdnz	LloopE
Lendx:
	addze	r3,r0
	lmw	r30,-32(r1)
}

/*
asm void __umul_ppmm()
{
	mullw	r0,r4,r5
	mulhwu	r9,r4,r5
	stw	r0,0(r3)
	mr	r3,r9
}
*/
