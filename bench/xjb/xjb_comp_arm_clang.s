	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 26, 0	sdk_version 26, 2
	.globl	__ZN8xjb_comp10xjb32_compEfPc   ; -- Begin function _ZN8xjb_comp10xjb32_compEfPc
	.p2align	2
__ZN8xjb_comp10xjb32_compEfPc:          ; @_ZN8xjb_comp10xjb32_compEfPc
	.cfi_startproc
; %bb.0:
	fmov	w8, s0
	mov	w9, #45                         ; =0x2d
	strb	w9, [x0]
	lsr	w9, w8, #31
	add	x0, x0, x9
	and	w10, w8, #0x7fffff
	ubfx	w8, w8, #23, #8
	cbz	w8, LBB0_8
; %bb.1:
	cmp	w8, #255
	b.eq	LBB0_10
; %bb.2:
	sub	x8, x8, #150
	cbz	w10, LBB0_16
; %bb.3:
	orr	x10, x10, #0x800000
LBB0_4:
	mov	x12, #0                         ; =0x0
	mov	w9, #0                          ; =0x0
	mov	w11, #1233                      ; =0x4d1
	smull	x11, w8, w11
LBB0_5:
	asr	x11, x11, #12
	mvn	w13, w11
	mov	w14, #21099                     ; =0x526b
	movk	w14, #3, lsl #16
	mul	w13, w13, w14
	asr	w13, w13, #16
	mov	w15, #31                        ; =0x1f
	sub	w15, w15, w11
	lsr	w16, w15, #4
	and	w17, w15, #0xfffffff0
	and	w1, w15, #0xf
Lloh0:
	adrp	x2, __ZZN8xjb_comp10xjb32_compEfPcE21pow10_base_table_pow5@PAGE
Lloh1:
	add	x2, x2, __ZZN8xjb_comp10xjb32_compEfPcE21pow10_base_table_pow5@PAGEOFF
	ldr	x16, [x2, w16, uxtw #3]
	mov	w3, #45728                      ; =0xb2a0
	movk	w3, #65429, lsl #16
	madd	w14, w17, w14, w3
	add	w14, w1, w14, asr #16
	and	x17, x15, #0xc
	add	x17, x2, x17
	ldr	w17, [x17, #40]
	and	x15, x15, #0x3
	add	x15, x2, x15
	ldrb	w15, [x15, #56]
	umull	x15, w15, w17
	umulh	x17, x15, x16
	mul	x15, x15, x16
	sub	w14, w13, w14
	mvn	w16, w14
	lsr	x15, x15, x14
	lsl	x1, x17, #1
	lsl	x16, x1, x16
	orr	x15, x16, x15
	lsr	x16, x17, x14
	tst	x14, #0x40
	csel	x14, x16, x15, ne
	mvn	w15, w10
	and	x15, x15, #0x1
	mov	w16, #28                        ; =0x1c
	add	w13, w13, w8
	sub	w16, w16, w13
	add	w13, w13, #37
	lsl	x10, x10, x13
	umulh	x10, x14, x10
	and	x13, x10, #0xfffffffff
	lsr	x14, x14, x16
	eor	x16, x13, #0xfffffffff
	add	x14, x14, x15
	lsr	x12, x14, x12
	cmp	x12, x13
	cset	w12, hi
	cmp	x14, x16
	lsr	x10, x10, #36
	cinc	x10, x10, hi
	mov	w14, #808464432                 ; =0x30303030
	str	w14, [x0]
	csinc	w14, w12, wzr, ls
	mov	w12, #16960                     ; =0x4240
	movk	w12, #15, lsl #16
	cmp	x10, x12
	cset	w12, lo
	mov	w15, #38528                     ; =0x9680
	movk	w15, #152, lsl #16
	cmp	x10, x15
	cinc	x15, x12, lo
	mov	x12, #4300                      ; =0x10cc
	movk	x12, #47815, lsl #16
	movk	x12, #36280, lsl #32
	movk	x12, #6, lsl #48
	umulh	x12, x10, x12
	mov	w16, #-10000                    ; =0xffffd8f0
	umaddl	x12, w12, w16, x10
	fmov	d0, x12
	mov	w12, #45056                     ; =0xb000
	movk	w12, #327, lsl #16
	dup.2s	v1, w12
	sqdmulh.2s	v1, v0, v1
	mov	w12, #65436                     ; =0xff9c
	dup.2s	v2, w12
	mla.2s	v0, v1, v2
	mov	w12, #3296                      ; =0xce0
	dup.4h	v1, w12
	sqdmulh.4h	v1, v0, v1
	movi.4h	v2, #246
	mla.4h	v0, v1, v2
	fmov	x12, d0
	rev	x12, x12
	lsl	x16, x15, #3
	lsr	x12, x12, x16
	clz	x16, x12
	orr	x12, x12, #0x3030303030303030
	mov	w17, #7                         ; =0x7
	sub	w16, w17, w16, lsr #3
	sxtw	x16, w16
	mov	w17, #8                         ; =0x8
	cmp	w14, #0
	sub	x14, x17, x15
	add	x11, x14, x11
	csel	x15, x16, x14, ne
	add	x16, x13, x13, lsl #2
	add	x13, x16, x13, lsr #32
	mov	x16, #-7                        ; =0xfffffffffffffff9
	movk	x16, #33155, lsl #32
	movk	x16, #1, lsl #48
	add	x13, x13, x16
	lsr	x13, x13, #35
	cbnz	w9, LBB0_11
LBB0_6:
	mov	w8, #1                          ; =0x1
	sub	x9, x8, x11
	cmn	x11, #3
	cset	w16, lo
	csel	x9, xzr, x9, lo
	cmp	x11, #7
	csinc	x17, x8, x11, hs
	cmn	x11, #3
	ccmp	x15, #0, #4, lo
	cinc	x1, x15, ne
	add	x8, x11, #3
	add	x15, x15, #2
	cmp	x8, x15
	csel	x15, x8, x15, hi
	cmp	x11, #6
	add	x8, x0, x9
	add	x9, x8, x17
	str	x12, [x8]
	str	x13, [x8, x14]
	ldr	x12, [x9]
	str	x12, [x9, w16, uxtw]
	csinc	x9, x15, x1, ls
	mov	w12, #46                        ; =0x2e
	strb	w12, [x0, x17]
	lsr	x10, x10, #5
	cmp	x10, #3124
	b.ls	LBB0_15
LBB0_7:
	mov	w10, #11109                     ; =0x2b65
	movk	w10, #12336, lsl #16
	add	x12, x10, #512
	cmp	x11, #0
	csel	x10, x12, x10, lt
	cneg	x12, x11, mi
	mov	w13, #103                       ; =0x67
	mul	x13, x12, x13
	lsr	x13, x13, #10
	mov	x14, #-65536                    ; =0xffffffffffff0000
	movk	x14, #62977, lsl #16
	add	x11, x11, #3
	cmp	x11, #9
	cset	w15, hi
	add	x10, x10, x12, lsl #24
	madd	x10, x13, x14, x10
	cmp	x11, #10
	csel	x10, xzr, x10, lo
	add	x8, x8, x9
	str	x10, [x8]
	add	x0, x8, w15, uxtw #2
	ret
LBB0_8:
	cbz	w10, LBB0_17
; %bb.9:
	mov	x8, #-149                       ; =0xffffffffffffff6b
	b	LBB0_4
LBB0_10:
	mov	w8, #24942                      ; =0x616e
	movk	w8, #110, lsl #16
	mov	w9, #28265                      ; =0x6e69
	movk	w9, #102, lsl #16
	cmp	w10, #0
	csel	w8, w9, w8, eq
	str	w8, [x0], #3
	ret
LBB0_11:
	cmn	x8, #119
	b.eq	LBB0_14
; %bb.12:
	cmp	x8, #67
	b.eq	LBB0_14
; %bb.13:
	cmp	x8, #64
	b.ne	LBB0_6
LBB0_14:
	add	x13, x13, #1
	b	LBB0_6
LBB0_15:
	add	x10, x11, #1
	ldur	x11, [x8, #2]
	and	x11, x11, #0xf0f0f0f0f0f0f0f
	rbit	x11, x11
	clz	x11, x11
	ubfx	x11, x11, #3, #29
	mov	w11, w11
	add	x12, x11, #2
	sub	x11, x10, x12
	add	x10, x8, x12
	ldrb	w13, [x10]
	strb	w13, [x8]
	ldur	x10, [x10, #1]
	stur	x10, [x8, #2]
	sub	x9, x9, x12
	cmp	x9, #1
	cinc	x9, x9, ne
	b	LBB0_7
LBB0_16:
	mov	w9, #1233                       ; =0x4d1
	smull	x9, w8, w9
	sub	x11, x9, #512
	mov	w12, #1                         ; =0x1
	mov	w10, #8388608                   ; =0x800000
	mov	w9, #1                          ; =0x1
	b	LBB0_5
LBB0_17:
	mov	w8, #11824                      ; =0x2e30
	movk	w8, #48, lsl #16
	str	w8, [x0], #3
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
	.p2align	3, 0x0                          ; @_ZZN8xjb_comp10xjb32_compEfPcE21pow10_base_table_pow5
__ZZN8xjb_comp10xjb32_compEfPcE21pow10_base_table_pow5:
	.quad	-3480967307441105733            ; 0xcfb11ead453994bb
	.quad	-1831394126398103204            ; 0xe69594bec44de15c
	.quad	-9223372036854775808            ; 0x8000000000000000
	.quad	-8206744073709551616            ; 0x8e1bc9bf04000000
	.quad	-7078060301547948642            ; 0x9dc5ada82b70b59e
	.quad	2684354560001                   ; 0x27100000001
	.quad	1048576000000390625             ; 0xe8d4a510005f5e1
	.quad	2098791681                      ; 0x7d190501

.subsections_via_symbols
