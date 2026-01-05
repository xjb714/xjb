	.file	"main.cpp"
                                        # Start of file scope inline assembly
	.globl	_ZSt21ios_base_library_initv

                                        # End of file scope inline assembly
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          # -- Begin function _ZN3xjb5xjb64EdPc
.LCPI0_0:
	.long	27370496                        # 0x1a1a400
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
.LCPI0_3:
	.byte	7                               # 0x7
	.byte	15                              # 0xf
	.byte	23                              # 0x17
	.byte	31                              # 0x1f
	.byte	39                              # 0x27
	.byte	47                              # 0x2f
	.byte	55                              # 0x37
	.byte	63                              # 0x3f
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
.LCPI0_4:
	.quad	4048780183313844224             # 0x3830282018100800
	.quad	8678551244950751296             # 0x7870686058504840
.LCPI0_6:
	.zero	16,48
.LCPI0_8:
	.byte	7                               # 0x7
	.byte	15                              # 0xf
	.byte	23                              # 0x17
	.byte	31                              # 0x1f
	.byte	39                              # 0x27
	.byte	47                              # 0x2f
	.byte	55                              # 0x37
	.byte	63                              # 0x3f
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
.LCPI0_1:
	.quad	45035996                        # 0x2af31dc
	.quad	450359963                       # 0x1ad7f29b
	.quad	4503599628                      # 0x10c6f7a0c
	.quad	45035996274                     # 0xa7c5ac472
	.quad	450359962738                    # 0x68db8bac72
	.quad	4503599627371                   # 0x4189374bc6b
	.quad	45035996273705                  # 0x28f5c28f5c29
	.quad	450359962737050                 # 0x199999999999a
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0
.LCPI0_2:
	.quad	160                             # 0xa0
.LCPI0_5:
	.quad	10                              # 0xa
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2, 0x0
.LCPI0_7:
	.long	27370496                        # 0x1a1a400
	.text
	.globl	_ZN3xjb5xjb64EdPc
	.p2align	4
	.type	_ZN3xjb5xjb64EdPc,@function
_ZN3xjb5xjb64EdPc:                      # 
	.cfi_startproc
# %bb.0:
	vmovq	%xmm0, %rdx
	movb	$63, %cl
	movabsq	$-9218868437227405312, %r8      # imm = 0x8010000000000000
	movb	$45, (%rdi)
	movq	%rdx, %rsi
	shrq	$63, %rsi
	bzhiq	%rcx, %rdx, %rcx
	leaq	(%rcx,%r8), %r9
	leaq	(%rdi,%rsi), %rax
	orq	$2, %r8
	cmpq	%r8, %r9
	jb	.LBB0_12
# %bb.1:
	movb	$52, %cl
	movabsq	$4503599627370496, %r10         # imm = 0x10000000000000
	movabsq	$3472328296227680304, %r8       # imm = 0x3030303030303030
	bzhiq	%rcx, %rdx, %r11
	movl	$2868, %ecx                     # imm = 0xB34
	orq	%r11, %r10
	bextrq	%rcx, %rdx, %r9
	movl	$1075, %ecx                     # imm = 0x433
	tzcntq	%r10, %rdx
	subq	%r9, %rcx
	cmpq	%rdx, %rcx
	jbe	.LBB0_9
# %bb.2:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r13
	.cfi_def_cfa_offset 40
	pushq	%r12
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	leaq	-1075(%r9), %rcx
	movq	%rcx, %r15
	testq	%r9, %r9
	je	.LBB0_16
.LBB0_3:
	imulq	$315653, %rcx, %r14             # imm = 0x4D105
	xorl	%r12d, %r12d
	testq	%r11, %r11
	sete	%r12b
	je	.LBB0_17
.LBB0_4:
	sarq	$20, %r14
	vmovd	.LCPI0_7(%rip), %xmm2           # xmm2 = [27370496,0,0,0]
	vmovdqu64	.LCPI0_1(%rip), %zmm4   # zmm4 = [45035996,450359963,4503599628,45035996274,450359962738,4503599627371,45035996273705,450359962737050]
                                        # AlignMOV convert to UnAlignMOV 
	vmovdqu	.LCPI0_4(%rip), %xmm3           # xmm3 = [4048780183313844224,8678551244950751296]
                                        # AlignMOV convert to UnAlignMOV 
	movq	%r14, %rcx
	notq	%rcx
	imull	$217707, %ecx, %edx             # imm = 0x3526B
	shlq	$4, %rcx
	movq	_ZL12pow10_double+4688(%rcx), %r13
	shrl	$16, %edx
	addl	%edx, %r15d
	leal	7(%r15), %edx
	shlxq	%rdx, %r10, %rdx
	notl	%r10d
	vmovdqa64	%zmm2, %zmm5
	mulxq	_ZL12pow10_double+4696(%rcx), %rcx, %rcx
	mulxq	%r13, %rbp, %rbx
	addq	%rbp, %rcx
	adcq	$0, %rbx
	negb	%r15b
	andl	$1, %r10d
	shrxq	%r15, %r13, %rdx
	shrdq	$6, %rbx, %rcx
	shrq	$6, %rbx
	movabsq	$999999999999999, %r15          # imm = 0x38D7EA4C67FFF
	addq	%rdx, %r10
	movq	%rcx, %rdx
	addq	%r10, %rdx
	shrxq	%r12, %r10, %r12
	adcq	$0, %rbx
	xorl	%r13d, %r13d
	leaq	(%rbx,%rbx), %rdx
	cmpq	%r15, %rbx
	movabsq	$-6067343680855748867, %r15     # imm = 0xABCC77118461CEFD
	leaq	(%rdx,%rdx,4), %rdx
	seta	%r13b
	cmovaq	%rbx, %rdx
	mulxq	%r15, %r15, %r15
	movq	%r8, (%rax)
	movabsq	$4611686018427387904, %r8       # imm = 0x4000000000000000
	shrq	$26, %r15
	movl	%r15d, %ebp
	imull	$-100000000, %r15d, %r15d       # imm = 0xFA0A1F00
	vpbroadcastq	%rbp, %zmm0
	movq	%rcx, %rbp
	vpmadd52luq	%zmm4, %zmm0, %zmm5
	vpxor	%xmm0, %xmm0, %xmm0
	addl	%r15d, %edx
	leaq	15(%r13), %r15
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm4, %zmm1, %zmm2
	vpbroadcastq	.LCPI0_5(%rip), %zmm1   # zmm1 = [10,10,10,10,10,10,10,10]
	vpxor	%xmm4, %xmm4, %xmm4
	vpmadd52huq	%zmm5, %zmm1, %zmm0
	vpmadd52huq	%zmm2, %zmm1, %zmm4
	vpxor	%xmm1, %xmm1, %xmm1
	vpermt2b	%zmm4, %zmm3, %zmm0
	vpcmpgtb	%xmm1, %xmm0, %k0
	kmovw	%k0, %edx
	lzcntq	%rdx, %rdx
	xorq	$63, %rdx
	cmpq	%rcx, %r12
	movq	%r15, %r12
	cmovaq	%rdx, %r12
	addq	%r10, %rbp
	cmovbq	%rdx, %r12
	cmpq	%r8, %rcx
	je	.LBB0_18
# %bb.5:
	movabsq	$-9223372036854775802, %rbp     # imm = 0x8000000000000006
.LBB0_6:
	vpaddb	.LCPI0_6(%rip), %xmm0, %xmm0
	leaq	15(%r13,%r14), %r8
	movl	$10, %r14d
	movq	%rcx, %rdx
	mulxq	%r14, %rdx, %r14
	addq	%rbp, %rdx
	adcq	$12336, %r14                    # imm = 0x3030
	testq	%r11, %r11
	je	.LBB0_19
.LBB0_7:
	leaq	3(%r8), %rcx
	movl	$19, %r11d
	cmpq	$19, %rcx
	cmovbq	%rcx, %r11
	shll	$2, %r11d
	leaq	(%r11,%r11,4), %rdx
	movzbl	_ZL12double_table+15209(%r11,%r11,4), %r10d
	movzbl	_ZL12double_table+15192(%r12,%rdx), %edx
	movzbl	_ZL12double_table+15210(%r11,%r11,4), %r12d
	movzbl	_ZL12double_table+15211(%r11,%r11,4), %r11d
	leaq	(%rax,%r10), %rcx
	vmovdqu	%xmm0, (%rax,%r10)
	movq	%r14, (%r15,%rcx)
	vmovups	(%r12,%rcx), %xmm0
	vmovups	%xmm0, (%r11,%rcx)
	movb	$46, (%rax,%r12)
	testq	%r9, %r9
	je	.LBB0_20
.LBB0_8:
	movq	_ZL12double_table+12464(,%r8,8), %rax
	movq	%rax, (%rcx,%rdx)
	shrq	$56, %rax
	addq	%rcx, %rax
	addq	%rdx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%r12
	.cfi_def_cfa_offset 40
	popq	%r13
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	.cfi_restore %rbx
	.cfi_restore %r12
	.cfi_restore %r13
	.cfi_restore %r14
	.cfi_restore %r15
	.cfi_restore %rbp
	vzeroupper
	retq
.LBB0_9:
	shrxq	%rcx, %r10, %rdx
	cmpq	$99999999, %rdx                 # imm = 0x5F5E0FF
	ja	.LBB0_11
# %bb.10:
	vmovd	.LCPI0_7(%rip), %xmm1           # xmm1 = [27370496,0,0,0]
	vpbroadcastq	%rdx, %zmm0
	vpmadd52luq	.LCPI0_1(%rip), %zmm0, %zmm1
	vpmullq	.LCPI0_2(%rip){1to8}, %zmm1, %zmm0
	vmovq	.LCPI0_8(%rip), %xmm1           # xmm1 = [7,15,23,31,39,47,55,63,0,0,0,0,0,0,0,0]
	vpermb	%zmm0, %zmm1, %zmm0
	vmovq	%xmm0, %rcx
	tzcntq	%rcx, %rdx
	orq	%rcx, %r8
	movl	%edx, %esi
	andb	$56, %dl
	shrl	$3, %esi
	shrxq	%rdx, %r8, %rcx
	movq	%rcx, (%rax)
	subq	%rsi, %rax
	movl	$12334, 8(%rax)                 # imm = 0x302E
	addq	$10, %rax
	vzeroupper
	retq
.LBB0_11:
	movabsq	$3022314549036573, %rcx         # imm = 0xABCC77118461D
	movl	$4294957296, %edi               # imm = 0xFFFFD8F0
	mulxq	%rcx, %rcx, %rcx
	shrq	$14, %rcx
	imulq	$-100000000, %rcx, %rsi         # imm = 0xFA0A1F00
	addq	%rdx, %rsi
	imulq	$109951163, %rcx, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %rsi, %r11          # imm = 0x68DB8BB
	shrq	$40, %rdx
	imulq	%rdi, %rdx
	shrq	$40, %r11
	imulq	%rdi, %r11
	movabsq	$545460846719, %rdi             # imm = 0x7F0000007F
	addq	%rcx, %rdx
	imulq	$5243, %rdx, %r10               # imm = 0x147B
	addq	%rsi, %r11
	shrq	$19, %r10
	andq	%rdi, %r10
	imulq	$65436, %r10, %rsi              # imm = 0xFF9C
	addq	%rdx, %rsi
	imulq	$5243, %r11, %rdx               # imm = 0x147B
	imulq	$103, %rsi, %r9
	shrq	$19, %rdx
	andq	%rdi, %rdx
	shrq	$10, %r9
	movabsq	$4222189076152335, %rdi         # imm = 0xF000F000F000F
	imulq	$65436, %rdx, %rdx              # imm = 0xFF9C
	andq	%rdi, %r9
	imulq	$246, %r9, %rcx
	addq	%r11, %rdx
	imulq	$103, %rdx, %r9
	addq	%rsi, %rcx
	shrq	$10, %r9
	andq	%rdi, %r9
	imulq	$246, %r9, %rsi
	addq	%rdx, %rsi
	lzcntq	%rcx, %rdx
	orq	%r8, %rcx
	movl	%edx, %edi
	bswapq	%rcx
	andb	$56, %dl
	shrl	$3, %edi
	orq	%r8, %rsi
	shrxq	%rdx, %rcx, %rcx
	movq	%rcx, (%rax)
	subq	%rdi, %rax
	movbeq	%rsi, 8(%rax)
	movl	$12334, 16(%rax)                # imm = 0x302E
	addq	$18, %rax
	retq
.LBB0_12:
	movabsq	$9218868437227405312, %rdx      # imm = 0x7FF0000000000000
	cmpq	%rdx, %rcx
	je	.LBB0_25
# %bb.13:
	cmpq	$1, %rcx
	je	.LBB0_24
# %bb.14:
	testq	%rcx, %rcx
	jne	.LBB0_26
# %bb.15:
	movl	$3, %edx
	movl	$3157552, (%rax)                # imm = 0x302E30
	addq	%rdx, %rax
	retq
.LBB0_16:
	.cfi_def_cfa_offset 56
	.cfi_offset %rbx, -56
	.cfi_offset %rbp, -16
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movq	$-1074, %r15                    # imm = 0xFBCE
	movq	%r11, %r10
	jmp	.LBB0_3
.LBB0_17:
	addq	$-131237, %r14                  # imm = 0xFFFDFF5B
	jmp	.LBB0_4
.LBB0_18:
	xorl	%ebp, %ebp
	jmp	.LBB0_6
.LBB0_19:
	shrq	$54, %rcx
	shrq	$55, %r10
	leaq	(%rcx,%rcx,4), %rcx
	leaq	(%r10,%r10,4), %r10
	movl	%ecx, %edx
	shrl	$9, %ecx
	andl	$511, %edx                      # imm = 0x1FF
	addq	$12337, %rcx                    # imm = 0x3031
	cmpl	%r10d, %edx
	cmovaq	%rcx, %r14
	jmp	.LBB0_7
.LBB0_20:
	movabsq	$99999999999999, %rax           # imm = 0x5AF3107A3FFF
	cmpq	%rax, %rbx
	ja	.LBB0_8
# %bb.21:
	addq	%r10, %rsi
	xorl	%eax, %eax
	leaq	2(%rdi,%rsi), %r9
.LBB0_22:                               # =>This Inner Loop Header: Depth=1
	movzbl	(%r9,%rax), %r10d
	incq	%rax
	cmpb	$48, %r10b
	je	.LBB0_22
# %bb.23:
	addq	%rdi, %rsi
	movb	%r10b, (%rcx)
	subq	%rax, %r8
	xorl	%edi, %edi
	vmovups	2(%rax,%rsi), %xmm0
	leaq	-2(%rdx), %rsi
	cmpq	%rax, %rsi
	notq	%rax
	setne	%dil
	addq	%rdx, %rax
	addq	%rdi, %rax
	movq	%rax, %rdx
	vmovups	%xmm0, 2(%rcx)
	jmp	.LBB0_8
.LBB0_24:
	.cfi_def_cfa_offset 8
	.cfi_restore %rbx
	.cfi_restore %rbp
	.cfi_restore %r12
	.cfi_restore %r13
	.cfi_restore %r14
	.cfi_restore %r15
	movabsq	$57390211622197, %rcx           # imm = 0x3432332D6535
	movl	$6, %edx
	movq	%rcx, (%rax)
	addq	%rdx, %rax
	retq
.LBB0_25:
	movl	$3, %edx
	movl	$6712937, (%rax)                # imm = 0x666E69
	addq	%rdx, %rax
	retq
.LBB0_26:
	movl	$3, %edx
	movabsq	$9218868437227405313, %rsi      # imm = 0x7FF0000000000001
	cmpq	%rsi, %rcx
	jb	.LBB0_28
# %bb.27:
	movl	$7233902, (%rax)                # imm = 0x6E616E
.LBB0_28:
	addq	%rdx, %rax
	retq
.Lfunc_end0:
	.size	_ZN3xjb5xjb64EdPc, .Lfunc_end0-_ZN3xjb5xjb64EdPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          # -- Begin function _ZN3xjb5xjb32EfPc
.LCPI1_0:
	.long	27370496                        # 0x1a1a400
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
.LCPI1_3:
	.byte	7                               # 0x7
	.byte	15                              # 0xf
	.byte	23                              # 0x17
	.byte	31                              # 0x1f
	.byte	39                              # 0x27
	.byte	47                              # 0x2f
	.byte	55                              # 0x37
	.byte	63                              # 0x3f
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
.LCPI1_5:
	.byte	7                               # 0x7
	.byte	15                              # 0xf
	.byte	23                              # 0x17
	.byte	31                              # 0x1f
	.byte	39                              # 0x27
	.byte	47                              # 0x2f
	.byte	55                              # 0x37
	.byte	63                              # 0x3f
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.byte	0                               # 0x0
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
.LCPI1_1:
	.quad	45035996                        # 0x2af31dc
	.quad	450359963                       # 0x1ad7f29b
	.quad	4503599628                      # 0x10c6f7a0c
	.quad	45035996274                     # 0xa7c5ac472
	.quad	450359962738                    # 0x68db8bac72
	.quad	4503599627371                   # 0x4189374bc6b
	.quad	45035996273705                  # 0x28f5c28f5c29
	.quad	450359962737050                 # 0x199999999999a
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0
.LCPI1_2:
	.quad	160                             # 0xa0
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2, 0x0
.LCPI1_4:
	.long	27370496                        # 0x1a1a400
	.text
	.globl	_ZN3xjb5xjb32EfPc
	.p2align	4
	.type	_ZN3xjb5xjb32EfPc,@function
_ZN3xjb5xjb32EfPc:                      # 
	.cfi_startproc
# %bb.0:
	vmovd	%xmm0, %eax
	movl	$2071, %ecx                     # imm = 0x817
	movb	$45, (%rdi)
	movl	%eax, %esi
	shrl	$31, %esi
	movl	%eax, %r9d
	andl	$8388607, %r9d                  # imm = 0x7FFFFF
	addq	%rdi, %rsi
	bextrl	%ecx, %eax, %edi
	je	.LBB1_1
# %bb.5:
	cmpl	$255, %edi
	je	.LBB1_6
# %bb.7:
	leaq	-150(%rdi), %rax
	leaq	8388608(%r9), %rdx
	imulq	$1233, %rax, %rcx               # imm = 0x4D1
	testq	%r9, %r9
	je	.LBB1_9
# %bb.8:
	movzbl	_ZL11float_table+1084(%rdi), %r10d
	sarq	$12, %rcx
	xorl	%r11d, %r11d
	xorl	%r8d, %r8d
.LBB1_10:
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movq	_ZL11float_table+360(,%rcx,8), %rbx
	movb	$65, %r14b
	shlxq	%r10, %rdx, %rdx
	notl	%r9d
	xorl	%r15d, %r15d
	vmovd	.LCPI1_4(%rip), %xmm1           # xmm1 = [27370496,0,0,0]
	subb	%r10b, %r14b
	andl	$1, %r9d
	mulxq	%rbx, %rdx, %rdx
	shrxq	%r14, %rbx, %r10
	movl	$808464432, (%rsi)              # imm = 0x30303030
	addq	%r9, %r10
	movb	$36, %r9b
	bzhiq	%r9, %rdx, %rbx
	shrxq	%r11, %r10, %r9
	leaq	(%rbx,%r10), %r14
	shrq	$36, %r14
	cmpq	%rbx, %r9
	seta	%r15b
	shrq	$36, %rdx
	xorl	%r12d, %r12d
	addq	%r14, %rdx
	vpbroadcastq	%rdx, %zmm0
	vpmadd52luq	.LCPI1_1(%rip), %zmm0, %zmm1
	cmpq	$1000000, %rdx                  # imm = 0xF4240
	setb	%r12b
	cmpq	$10000000, %rdx                 # imm = 0x989680
	adcq	$0, %r12
	vpmullq	.LCPI1_2(%rip){1to8}, %zmm1, %zmm0
	vmovq	.LCPI1_5(%rip), %xmm1           # xmm1 = [7,15,23,31,39,47,55,63,0,0,0,0,0,0,0,0]
	leal	(,%r12,8), %r10d
	vpermb	%zmm0, %zmm1, %zmm0
	vmovq	%xmm0, %r9
	shrxq	%r10, %r9, %r11
	movabsq	$3472328296227680304, %r10      # imm = 0x3030303030303030
	lzcntq	%r11, %r9
	orq	%r11, %r10
	movl	$8, %r11d
	shrl	$3, %r9d
	subq	%r12, %r11
	xorq	$7, %r9
	orq	%r14, %r15
	leaq	(%rbx,%rbx,4), %r15
	movabsq	$423878912376825, %r14          # imm = 0x18183FFFFFFF9
	cmoveq	%r11, %r9
	shrq	$32, %rbx
	addq	%r11, %rcx
	addq	%rbx, %r14
	addq	%r15, %r14
	shrq	$35, %r14
	testb	%r8b, %r8b
	jne	.LBB1_11
.LBB1_12:
	leaq	3(%rcx), %r8
	xorl	%eax, %eax
	movl	$10, %ebx
	cmpq	$10, %r8
	cmovbq	%r8, %rbx
	setae	%al
	shll	$2, %ebx
	shll	$2, %eax
	testl	%edi, %edi
	movzbl	_ZL11float_table+961(%rbx,%rbx,2), %r15d
	leaq	(%rbx,%rbx,2), %r12
	movzbl	_ZL11float_table+952(%r9,%r12), %r9d
	movzbl	_ZL11float_table+962(%rbx,%rbx,2), %r12d
	movzbl	_ZL11float_table+963(%rbx,%rbx,2), %ebx
	leaq	(%rsi,%r15), %r8
	movq	%r10, (%rsi,%r15)
	movq	%r14, (%r11,%r8)
	movq	(%r12,%r8), %r10
	movq	%r10, (%rbx,%r8)
	movb	$46, (%rsi,%r12)
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	.cfi_restore %rbx
	.cfi_restore %r12
	.cfi_restore %r14
	.cfi_restore %r15
	je	.LBB1_13
.LBB1_15:
	movl	_ZL11float_table+796(,%rcx,4), %ecx
	addq	%r8, %rax
	addq	%r9, %rax
	movq	%rcx, (%r8,%r9)
	vzeroupper
	retq
.LBB1_1:
	testq	%r9, %r9
	je	.LBB1_3
# %bb.2:
	movzbl	_ZL11float_table+1084(%rdi), %r10d
	movq	$-45, %rcx
	movq	$-149, %rax
	xorl	%r11d, %r11d
	xorl	%r8d, %r8d
	movq	%r9, %rdx
	jmp	.LBB1_10
.LBB1_6:
	testq	%r9, %r9
	movl	$6712937, %eax                  # imm = 0x666E69
	movl	$7233902, %ecx                  # imm = 0x6E616E
	cmovel	%eax, %ecx
	movl	%ecx, (%rsi)
	addq	$3, %rsi
	movq	%rsi, %rax
	retq
.LBB1_11:
	.cfi_def_cfa_offset 40
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	cmpq	$-119, %rax
	sete	%r8b
	cmpq	$64, %rax
	sete	%bl
	orb	%r8b, %bl
	cmpq	$67, %rax
	sete	%al
	orb	%bl, %al
	movzbl	%al, %eax
	addq	%rax, %r14
	jmp	.LBB1_12
.LBB1_13:
	.cfi_def_cfa_offset 8
	.cfi_restore %rbx
	.cfi_restore %r12
	.cfi_restore %r14
	.cfi_restore %r15
	cmpl	$99999, %edx                    # imm = 0x1869F
	ja	.LBB1_15
# %bb.14:
	movabsq	$1085102592571150095, %rdx      # imm = 0xF0F0F0F0F0F0F0F
	andq	2(%r8), %rdx
	tzcntq	%rdx, %rdx
	shrl	$3, %edx
	movzbl	2(%r8,%rdx), %edi
	leaq	2(%rdx), %rsi
	subq	%rsi, %rcx
	subq	%rsi, %r9
	incq	%rcx
	movb	%dil, (%r8)
	movq	3(%r8,%rdx), %rdx
	movq	%rdx, 2(%r8)
	xorl	%edx, %edx
	cmpq	$1, %r9
	setne	%dl
	addq	%rdx, %r9
	jmp	.LBB1_15
.LBB1_9:
	addq	$-512, %rcx                     # imm = 0xFE00
	movb	$1, %r8b
	movl	$1, %r11d
	sarq	$12, %rcx
	imull	$129371, %ecx, %r10d            # imm = 0x1F95B
	addl	$129371, %r10d                  # imm = 0x1F95B
	shrl	$9, %r10d
	addl	%edi, %r10d
	addb	$-113, %r10b
	jmp	.LBB1_10
.LBB1_3:
	movl	$3157552, (%rsi)                # imm = 0x302E30
	addq	$3, %rsi
	movq	%rsi, %rax
	retq
.Lfunc_end1:
	.size	_ZN3xjb5xjb32EfPc, .Lfunc_end1-_ZN3xjb5xjb32EfPc
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt13random_deviceD2Ev,"axG",@progbits,_ZNSt13random_deviceD2Ev,comdat
	.weak	_ZNSt13random_deviceD2Ev        # -- Begin function _ZNSt13random_deviceD2Ev
	.p2align	4
	.type	_ZNSt13random_deviceD2Ev,@function
_ZNSt13random_deviceD2Ev:               # 
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception0
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
.Ltmp0:
	callq	_ZNSt13random_device7_M_finiEv
.Ltmp1:
# %bb.1:
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.LBB2_2:
	.cfi_def_cfa_offset 16
.Ltmp2:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end2:
	.size	_ZNSt13random_deviceD2Ev, .Lfunc_end2-_ZNSt13random_deviceD2Ev
	.cfi_endproc
	.section	.gcc_except_table._ZNSt13random_deviceD2Ev,"aG",@progbits,_ZNSt13random_deviceD2Ev,comdat
	.p2align	2, 0x0
GCC_except_table2:
.Lexception0:
	.byte	255                             # @LPStart Encoding = omit
	.byte	3                               # @TType Encoding = udata4
	.uleb128 .Lttbase0-.Lttbaseref0
.Lttbaseref0:
	.byte	1                               # Call site Encoding = uleb128
	.uleb128 .Lcst_end0-.Lcst_begin0
.Lcst_begin0:
	.uleb128 .Ltmp0-.Lfunc_begin0           # >> Call Site 1 <<
	.uleb128 .Ltmp1-.Ltmp0                  #   Call between .Ltmp0 and .Ltmp1
	.uleb128 .Ltmp2-.Lfunc_begin0           #     jumps to .Ltmp2
	.byte	1                               #   On action: 1
.Lcst_end0:
	.byte	1                               # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                               #   No further actions
	.p2align	2, 0x0
                                        # >> Catch TypeInfos <<
	.long	0                               # TypeInfo 1
.Lttbase0:
	.p2align	2, 0x0
                                        # -- End function
	.text
	.globl	_Z9get_cyclev                   # -- Begin function _Z9get_cyclev
	.p2align	4
	.type	_Z9get_cyclev,@function
_Z9get_cyclev:                          # 
	.cfi_startproc
# %bb.0:
	#APP
	rdtsc
	#NO_APP
	shlq	$32, %rdx
	orq	%rdx, %rax
	retq
.Lfunc_end3:
	.size	_Z9get_cyclev, .Lfunc_end3-_Z9get_cyclev
	.cfi_endproc
                                        # -- End function
	.globl	_Z5getnsv                       # -- Begin function _Z5getnsv
	.p2align	4
	.type	_Z5getnsv,@function
_Z5getnsv:                              # 
	.cfi_startproc
# %bb.0:
	jmp	_ZNSt6chrono3_V212system_clock3nowEv # TAILCALL
.Lfunc_end4:
	.size	_Z5getnsv, .Lfunc_end4-_Z5getnsv
	.cfi_endproc
                                        # -- End function
	.globl	_Z25gen_double_filter_NaN_Infv  # -- Begin function _Z25gen_double_filter_NaN_Infv
	.p2align	4
	.type	_Z25gen_double_filter_NaN_Infv,@function
_Z25gen_double_filter_NaN_Infv:         # 
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movabsq	$9218868437227405312, %rbx      # imm = 0x7FF0000000000000
	.p2align	4
.LBB5_1:                                # =>This Inner Loop Header: Depth=1
	movl	$gen, %edi
	callq	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	andnq	%rbx, %rax, %rcx
	je	.LBB5_1
# %bb.2:
	vmovq	%rax, %xmm0
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end5:
	.size	_Z25gen_double_filter_NaN_Infv, .Lfunc_end5-_Z25gen_double_filter_NaN_Infv
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
.LCPI6_0:
	.quad	1                               # 0x1
.LCPI6_1:
	.quad	1073741823                      # 0x3fffffff
.LCPI6_2:
	.quad	9223372035781033984             # 0x7fffffffc0000000
.LCPI6_3:
	.quad	-5403634167711393303            # 0xb5026f5aa96619e9
	.section	.text._ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv,"axG",@progbits,_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv,comdat
	.weak	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	.p2align	4
	.type	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv,@function
_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv: # 
	.cfi_startproc
# %bb.0:
	movq	2496(%rdi), %rcx
	cmpq	$312, %rcx                      # imm = 0x138
	jb	.LBB6_4
# %bb.1:
	vpbroadcastq	.LCPI6_0(%rip), %zmm0   # zmm0 = [1,1,1,1,1,1,1,1]
	vpbroadcastq	.LCPI6_1(%rip), %zmm1   # zmm1 = [1073741823,1073741823,1073741823,1073741823,1073741823,1073741823,1073741823,1073741823]
	vpbroadcastq	.LCPI6_2(%rip), %zmm2   # zmm2 = [9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984]
	vpbroadcastq	.LCPI6_3(%rip), %zmm3   # zmm3 = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	xorl	%eax, %eax
	.p2align	4
.LBB6_2:                                # =>This Inner Loop Header: Depth=1
	leaq	8(%rdi,%rax,8), %rcx
	leaq	16(%rax), %rdx
	vmovdqu64	(%rcx), %zmm4
	vpsrlq	$1, -8(%rcx), %zmm8
	vmovdqu64	64(%rcx), %zmm5
	vmovdqu64	128(%rcx), %zmm6
	vpsrlq	$1, %zmm4, %zmm7
	vpandq	%zmm2, %zmm8, %zmm8
	vptestmq	%zmm0, %zmm4, %k1
	vpsrlq	$1, %zmm5, %zmm4
	vpandq	%zmm1, %zmm7, %zmm7
	vpandq	%zmm1, %zmm4, %zmm4
	vpternlogq	$86, 1240(%rcx), %zmm7, %zmm8 # zmm8 = mem ^ (zmm8 | zmm7)
	vpxorq	%zmm3, %zmm8, %zmm8 {%k1}
	vptestmq	%zmm0, %zmm5, %k1
	vmovdqu64	%zmm8, -8(%rcx)
	vpsrlq	$1, 56(%rcx), %zmm7
	vpandq	%zmm2, %zmm7, %zmm7
	vpternlogq	$86, 1304(%rcx), %zmm4, %zmm7 # zmm7 = mem ^ (zmm7 | zmm4)
	vpsrlq	$1, %zmm6, %zmm4
	vpandq	%zmm1, %zmm4, %zmm4
	vpxorq	%zmm3, %zmm7, %zmm7 {%k1}
	vptestmq	%zmm0, %zmm6, %k1
	vmovdqu64	%zmm7, 56(%rcx)
	vpsrlq	$1, 120(%rcx), %zmm5
	vpandq	%zmm2, %zmm5, %zmm5
	vpternlogq	$86, 1368(%rcx), %zmm4, %zmm5 # zmm5 = mem ^ (zmm5 | zmm4)
	vpxorq	%zmm3, %zmm5, %zmm5 {%k1}
	vmovdqu64	%zmm5, 120(%rcx)
	cmpq	$143, %rdx
	ja	.LBB6_6
# %bb.3:                                #   in Loop: Header=BB6_2 Depth=1
	vmovdqu64	200(%rdi,%rax,8), %zmm4
	vpsrlq	$1, 192(%rdi,%rax,8), %zmm6
	vpsrlq	$1, %zmm4, %zmm5
	vpandq	%zmm2, %zmm6, %zmm6
	vptestmq	%zmm0, %zmm4, %k1
	vpandq	%zmm1, %zmm5, %zmm5
	vpternlogq	$86, 1440(%rdi,%rax,8), %zmm5, %zmm6 # zmm6 = mem ^ (zmm6 | zmm5)
	vpxorq	%zmm3, %zmm6, %zmm6 {%k1}
	vmovdqu64	%zmm6, 192(%rdi,%rax,8)
	addq	$32, %rax
	jmp	.LBB6_2
.LBB6_6:
	vmovdqu	1224(%rdi), %ymm4
	vpsrlq	$1, 1216(%rdi), %ymm6
	leaq	2464(%rdi), %rax
	movl	$181, %ecx
	vpandq	.LCPI6_2(%rip){1to4}, %ymm6, %ymm6
	vptestmq	.LCPI6_0(%rip){1to4}, %ymm4, %k1
	vpsrlq	$1, %ymm4, %ymm5
	vpandq	.LCPI6_1(%rip){1to4}, %ymm5, %ymm5
	vpternlogq	$86, 2464(%rdi), %ymm5, %ymm6 # ymm6 = mem ^ (ymm6 | ymm5)
	vpxorq	.LCPI6_3(%rip){1to4}, %ymm6, %ymm6 {%k1}
	vmovdqu	%ymm6, 1216(%rdi)
	.p2align	4
.LBB6_7:                                # =>This Inner Loop Header: Depth=1
	leaq	-192(%rdi,%rcx,8), %rdx
	leaq	-165(%rcx), %rsi
	vmovdqu64	(%rdx), %zmm4
	vpsrlq	$1, -8(%rdx), %zmm8
	vmovdqu64	64(%rdx), %zmm5
	vmovdqu64	128(%rdx), %zmm6
	vpsrlq	$1, %zmm4, %zmm7
	vpandq	%zmm2, %zmm8, %zmm8
	vptestmq	%zmm0, %zmm4, %k1
	vpsrlq	$1, %zmm5, %zmm4
	vpandq	%zmm1, %zmm7, %zmm7
	vpandq	%zmm1, %zmm4, %zmm4
	vpternlogq	$86, -1256(%rdx), %zmm7, %zmm8 # zmm8 = mem ^ (zmm8 | zmm7)
	vpxorq	%zmm3, %zmm8, %zmm8 {%k1}
	vptestmq	%zmm0, %zmm5, %k1
	vmovdqu64	%zmm8, -8(%rdx)
	vpsrlq	$1, 56(%rdx), %zmm7
	vpandq	%zmm2, %zmm7, %zmm7
	vpternlogq	$86, -1192(%rdx), %zmm4, %zmm7 # zmm7 = mem ^ (zmm7 | zmm4)
	vpsrlq	$1, %zmm6, %zmm4
	vpandq	%zmm1, %zmm4, %zmm4
	vpxorq	%zmm3, %zmm7, %zmm7 {%k1}
	vptestmq	%zmm0, %zmm6, %k1
	vmovdqu64	%zmm7, 56(%rdx)
	vpsrlq	$1, 120(%rdx), %zmm5
	vpandq	%zmm2, %zmm5, %zmm5
	vpternlogq	$86, -1128(%rdx), %zmm4, %zmm5 # zmm5 = mem ^ (zmm5 | zmm4)
	vpxorq	%zmm3, %zmm5, %zmm5 {%k1}
	vmovdqu64	%zmm5, 120(%rdx)
	cmpq	$143, %rsi
	ja	.LBB6_9
# %bb.8:                                #   in Loop: Header=BB6_7 Depth=1
	vmovdqu64	(%rdi,%rcx,8), %zmm4
	vpsrlq	$1, -8(%rdi,%rcx,8), %zmm6
	vpsrlq	$1, %zmm4, %zmm5
	vpandq	%zmm2, %zmm6, %zmm6
	vptestmq	%zmm0, %zmm4, %k1
	vpandq	%zmm1, %zmm5, %zmm5
	vpternlogq	$86, -1256(%rdi,%rcx,8), %zmm5, %zmm6 # zmm6 = mem ^ (zmm6 | zmm5)
	vpxorq	%zmm3, %zmm6, %zmm6 {%k1}
	vmovdqu64	%zmm6, -8(%rdi,%rcx,8)
	addq	$32, %rcx
	jmp	.LBB6_7
.LBB6_9:
	movb	$7, %cl
	movabsq	$-5403634167711393303, %rsi     # imm = 0xB5026F5AA96619E9
	kmovd	%ecx, %k1
	movq	$-2147483648, %rcx              # imm = 0x80000000
	vmovdqu64	2472(%rdi), %zmm1 {%k1} {z}
	vmovdqu64	(%rax), %zmm0 {%k1} {z}
	vptestmq	.LCPI6_0(%rip){1to8}, %zmm1, %k2
	vpsrlq	$1, %zmm1, %zmm2
	vpandq	.LCPI6_1(%rip){1to8}, %zmm2, %zmm2
	vpsrlq	$1, %zmm0, %zmm0
	vpbroadcastq	.LCPI6_3(%rip), %zmm1 {%k2} {z} # zmm1 {%k2} {z} = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	vpternlogq	$236, .LCPI6_2(%rip){1to8}, %zmm2, %zmm0 # zmm0 = (zmm0 & m64bcst) | zmm2
	vpternlogq	$150, 1216(%rdi), %zmm0, %zmm1 # zmm1 = zmm1 ^ zmm0 ^ mem
	vmovdqu64	%zmm1, (%rax) {%k1}
	movq	(%rdi), %rax
	andq	2488(%rdi), %rcx
	movl	%eax, %edx
	andl	$2147483646, %edx               # imm = 0x7FFFFFFE
	orq	%rcx, %rdx
	movl	%eax, %ecx
	andl	$1, %ecx
	shrq	%rdx
	xorq	1240(%rdi), %rdx
	negq	%rcx
	andq	%rcx, %rsi
	movl	$1, %ecx
	xorq	%rdx, %rsi
	movq	%rsi, 2488(%rdi)
	jmp	.LBB6_5
.LBB6_4:
	movq	(%rdi,%rcx,8), %rax
	incq	%rcx
.LBB6_5:
	movq	%rcx, 2496(%rdi)
	movq	%rax, %rcx
	shrq	$29, %rcx
	movabsq	$22906492245, %rdx              # imm = 0x555555555
	andq	%rcx, %rdx
	movabsq	$8202884508482404352, %rcx      # imm = 0x71D67FFFEDA60000
	xorq	%rax, %rdx
	movq	%rdx, %rax
	shlq	$17, %rax
	andq	%rax, %rcx
	xorq	%rdx, %rcx
	movl	%ecx, %edx
	andl	$134201207, %edx                # imm = 0x7FFBF77
	shlq	$37, %rdx
	xorq	%rcx, %rdx
	movq	%rdx, %rax
	shrq	$43, %rax
	xorq	%rdx, %rax
	vzeroupper
	retq
.Lfunc_end6:
	.size	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv, .Lfunc_end6-_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	.cfi_endproc
                                        # -- End function
	.text
	.globl	_Z35gen_double_filter_NaN_Inf_subnormalv # -- Begin function _Z35gen_double_filter_NaN_Inf_subnormalv
	.p2align	4
	.type	_Z35gen_double_filter_NaN_Inf_subnormalv,@function
_Z35gen_double_filter_NaN_Inf_subnormalv: # 
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$gen, %edi
	callq	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	vmovq	%rax, %xmm0
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end7:
	.size	_Z35gen_double_filter_NaN_Inf_subnormalv, .Lfunc_end7-_Z35gen_double_filter_NaN_Inf_subnormalv
	.cfi_endproc
                                        # -- End function
	.globl	_Z24gen_float_filter_NaN_Infv   # -- Begin function _Z24gen_float_filter_NaN_Infv
	.p2align	4
	.type	_Z24gen_float_filter_NaN_Infv,@function
_Z24gen_float_filter_NaN_Infv:          # 
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	.p2align	4
.LBB8_1:                                # =>This Inner Loop Header: Depth=1
	movl	$gen, %edi
	callq	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	movl	%eax, %ecx
	notl	%ecx
	testl	$2139095040, %ecx               # imm = 0x7F800000
	je	.LBB8_1
# %bb.2:
	vmovd	%eax, %xmm0
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end8:
	.size	_Z24gen_float_filter_NaN_Infv, .Lfunc_end8-_Z24gen_float_filter_NaN_Infv
	.cfi_endproc
                                        # -- End function
	.globl	_Z34gen_float_filter_NaN_Inf_subnormalv # -- Begin function _Z34gen_float_filter_NaN_Inf_subnormalv
	.p2align	4
	.type	_Z34gen_float_filter_NaN_Inf_subnormalv,@function
_Z34gen_float_filter_NaN_Inf_subnormalv: # 
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$gen, %edi
	callq	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	vmovd	%eax, %xmm0
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end9:
	.size	_Z34gen_float_filter_NaN_Inf_subnormalv, .Lfunc_end9-_Z34gen_float_filter_NaN_Inf_subnormalv
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _Z9init_datav
.LCPI10_0:
	.quad	1                               # 0x1
.LCPI10_1:
	.quad	1073741823                      # 0x3fffffff
.LCPI10_2:
	.quad	9223372035781033984             # 0x7fffffffc0000000
.LCPI10_3:
	.quad	-5403634167711393303            # 0xb5026f5aa96619e9
	.text
	.globl	_Z9init_datav
	.p2align	4
	.type	_Z9init_datav,@function
_Z9init_datav:                          # 
	.cfi_startproc
# %bb.0:
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movabsq	$8589934592, %rdi               # imm = 0x200000000
	callq	_Znam
	vpbroadcastq	.LCPI10_0(%rip), %zmm0  # zmm0 = [1,1,1,1,1,1,1,1]
	vpbroadcastq	.LCPI10_1(%rip), %zmm1  # zmm1 = [1073741823,1073741823,1073741823,1073741823,1073741823,1073741823,1073741823,1073741823]
	vpbroadcastq	.LCPI10_2(%rip), %zmm2  # zmm2 = [9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984]
	vpbroadcastq	.LCPI10_3(%rip), %zmm3  # zmm3 = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	vpbroadcastq	.LCPI10_1(%rip), %ymm4  # ymm4 = [1073741823,1073741823,1073741823,1073741823]
	vpbroadcastq	.LCPI10_2(%rip), %ymm5  # ymm5 = [9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984]
	vpbroadcastq	.LCPI10_0(%rip), %ymm6  # ymm6 = [1,1,1,1]
	vpbroadcastq	.LCPI10_3(%rip), %ymm7  # ymm7 = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	movq	gen+2496(%rip), %r11
	movb	$7, %dl
	movq	$-2147483648, %rsi              # imm = 0x80000000
	movabsq	$-5403634167711393303, %rdi     # imm = 0xB5026F5AA96619E9
	movabsq	$22906492245, %r8               # imm = 0x555555555
	movabsq	$8202884508482404352, %r9       # imm = 0x71D67FFFEDA60000
	movabsq	$9218868437227405312, %r10      # imm = 0x7FF0000000000000
	xorl	%ecx, %ecx
	movq	%rax, data(%rip)
	jmp	.LBB10_1
	.p2align	4
.LBB10_12:                              #   in Loop: Header=BB10_1 Depth=1
	kmovd	%edx, %k1
	vmovdqu64	gen+2472(%rip), %zmm9 {%k1} {z}
	vmovdqu64	gen+2464(%rip), %zmm8 {%k1} {z}
	vpsrlq	$1, %zmm9, %zmm10
	vptestmq	%zmm0, %zmm9, %k2
	vpsrlq	$1, %zmm8, %zmm8
	vpandq	%zmm1, %zmm10, %zmm10
	vpbroadcastq	.LCPI10_3(%rip), %zmm9 {%k2} {z} # zmm9 {%k2} {z} = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	vpternlogq	$236, %zmm2, %zmm10, %zmm8 # zmm8 = (zmm8 & zmm2) | zmm10
	vpternlogq	$150, gen+1216(%rip), %zmm8, %zmm9 # zmm9 = zmm9 ^ zmm8 ^ mem
	vmovdqu64	%zmm9, gen+2464(%rip) {%k1}
	movq	gen+2488(%rip), %r11
	movq	gen(%rip), %rbx
	movl	%ebx, %r14d
	andq	%rsi, %r11
	andl	$2147483646, %r14d              # imm = 0x7FFFFFFE
	orq	%r11, %r14
	movl	%ebx, %r11d
	andl	$1, %r11d
	shrq	%r14
	xorq	gen+1240(%rip), %r14
	negq	%r11
	andq	%rdi, %r11
	xorq	%r14, %r11
	movq	%r11, gen+2488(%rip)
	movl	$1, %r11d
.LBB10_6:                               #   in Loop: Header=BB10_1 Depth=1
	movq	%rbx, %r14
	shrq	$29, %r14
	andq	%r8, %r14
	xorq	%rbx, %r14
	movq	%r14, %r15
	shlq	$17, %r15
	andq	%r9, %r15
	xorq	%r14, %r15
	movl	%r15d, %ebx
	andl	$134201207, %ebx                # imm = 0x7FFBF77
	shlq	$37, %rbx
	xorq	%r15, %rbx
	andnq	%r10, %rbx, %r14
	jne	.LBB10_7
.LBB10_1:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB10_3 Depth 2
                                        #     Child Loop BB10_10 Depth 2
	cmpq	$312, %r11                      # imm = 0x138
	jb	.LBB10_5
# %bb.2:                                #   in Loop: Header=BB10_1 Depth=1
	movl	$180, %r11d
	.p2align	4
.LBB10_3:                               #   Parent Loop BB10_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	leaq	gen-1432(,%r11,8), %rbx
	leaq	-164(%r11), %r14
	vmovdqu64	(%rbx), %zmm8
	vpsrlq	$1, -8(%rbx), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, 1240(%rbx), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, -8(%rbx)
	vmovdqu64	64(%rbx), %zmm8
	vpsrlq	$1, 56(%rbx), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, 1304(%rbx), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, 56(%rbx)
	vmovdqu64	128(%rbx), %zmm8
	vpsrlq	$1, 120(%rbx), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, 1368(%rbx), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, 120(%rbx)
	cmpq	$143, %r14
	ja	.LBB10_9
# %bb.4:                                #   in Loop: Header=BB10_3 Depth=2
	vmovdqu64	gen-1240(,%r11,8), %zmm8
	vpsrlq	$1, gen-1248(,%r11,8), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, gen(,%r11,8), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, gen-1248(,%r11,8)
	addq	$32, %r11
	jmp	.LBB10_3
	.p2align	4
.LBB10_9:                               #   in Loop: Header=BB10_1 Depth=1
	vmovdqu	gen+1224(%rip), %ymm8
	vpsrlq	$1, gen+1216(%rip), %ymm10
	movl	$181, %r11d
	vpsrlq	$1, %ymm8, %ymm9
	vpand	%ymm5, %ymm10, %ymm10
	vptestmq	%ymm6, %ymm8, %k1
	vpand	%ymm4, %ymm9, %ymm9
	vpternlogq	$86, gen+2464(%rip), %ymm9, %ymm10 # ymm10 = mem ^ (ymm10 | ymm9)
	vpxorq	%ymm7, %ymm10, %ymm10 {%k1}
	vmovdqu	%ymm10, gen+1216(%rip)
	.p2align	4
.LBB10_10:                              #   Parent Loop BB10_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	leaq	gen-192(,%r11,8), %rbx
	leaq	-165(%r11), %r14
	vmovdqu64	(%rbx), %zmm8
	vpsrlq	$1, -8(%rbx), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, -1256(%rbx), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, -8(%rbx)
	vmovdqu64	64(%rbx), %zmm8
	vpsrlq	$1, 56(%rbx), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, -1192(%rbx), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, 56(%rbx)
	vmovdqu64	128(%rbx), %zmm8
	vpsrlq	$1, 120(%rbx), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, -1128(%rbx), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, 120(%rbx)
	cmpq	$143, %r14
	ja	.LBB10_12
# %bb.11:                               #   in Loop: Header=BB10_10 Depth=2
	vmovdqu64	gen(,%r11,8), %zmm8
	vpsrlq	$1, gen-8(,%r11,8), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, gen-1256(,%r11,8), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, gen-8(,%r11,8)
	addq	$32, %r11
	jmp	.LBB10_10
	.p2align	4
.LBB10_5:                               #   in Loop: Header=BB10_1 Depth=1
	movq	gen(,%r11,8), %rbx
	incq	%r11
	jmp	.LBB10_6
.LBB10_7:                               #   in Loop: Header=BB10_1 Depth=1
	movq	%rbx, %r14
	shrq	$43, %r14
	xorq	%rbx, %r14
	movq	%r14, (%rax,%rcx,8)
	incq	%rcx
	cmpq	$1073741824, %rcx               # imm = 0x40000000
	jne	.LBB10_1
# %bb.8:
	movq	%r11, gen+2496(%rip)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	vzeroupper
	retq
.Lfunc_end10:
	.size	_Z9init_datav, .Lfunc_end10-_Z9init_datav
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function main
.LCPI11_0:
	.quad	0x3e10000000000000              #  9.3132257461547852E-10
	.text
	.globl	main
	.p2align	4
	.type	main,@function
main:                                   # 
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r13
	.cfi_def_cfa_offset 40
	pushq	%r12
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	subq	$152, %rsp
	.cfi_def_cfa_offset 208
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	vstmxcsr	80(%rsp)
	movl	$gen, %edi
	orl	$32832, 80(%rsp)                # imm = 0x8040
	vldmxcsr	80(%rsp)
	callq	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	movq	%rax, %r12
	callq	_ZNSt6chrono3_V212system_clock3nowEv
	leaq	(%r12,%r12,2), %rcx
	movq	%rax, 16(%rsp)                  # 8-byte Spill
	#APP
	rdtsc
	#NO_APP
	movq	%rax, (%rsp)                    # 8-byte Spill
	leaq	(,%r12,8), %r13
	leaq	(%r12,%r12), %rax
	movq	%rdx, 8(%rsp)                   # 8-byte Spill
	leaq	(,%r12,4), %rdx
	leaq	(%r12,%r12,8), %r15
	movl	$134217728, %ebx                # imm = 0x8000000
	xorl	%ebp, %ebp
	movq	%rax, 40(%rsp)                  # 8-byte Spill
	leaq	(%rax,%rax,2), %rax
	movq	%rcx, 64(%rsp)                  # 8-byte Spill
	leaq	(%r12,%r12,4), %rcx
	movq	%r13, 72(%rsp)                  # 8-byte Spill
	subq	%r12, %r13
	leaq	80(%rsp), %r12
	movq	%rdx, 56(%rsp)                  # 8-byte Spill
	movq	%rax, 32(%rsp)                  # 8-byte Spill
	movq	%rcx, 48(%rsp)                  # 8-byte Spill
	.p2align	4
.LBB11_1:                               # =>This Inner Loop Header: Depth=1
	movq	40(%rsp), %rax                  # 8-byte Reload
	movq	%r12, %rdi
	addq	%rbp, %rax
	vmovq	%rax, %xmm0
	callq	_ZN3xjb5xjb64EdPc
	movq	64(%rsp), %rax                  # 8-byte Reload
	movq	%r12, %rdi
	addq	%rbp, %rax
	vmovq	%rax, %xmm0
	callq	_ZN3xjb5xjb64EdPc
	movq	56(%rsp), %rax                  # 8-byte Reload
	movq	%r12, %rdi
	addq	%rbp, %rax
	vmovq	%rax, %xmm0
	callq	_ZN3xjb5xjb64EdPc
	movq	48(%rsp), %rax                  # 8-byte Reload
	movq	%r12, %rdi
	addq	%rbp, %rax
	vmovq	%rax, %xmm0
	callq	_ZN3xjb5xjb64EdPc
	movq	32(%rsp), %rax                  # 8-byte Reload
	movq	%r12, %rdi
	addq	%rbp, %rax
	vmovq	%rax, %xmm0
	callq	_ZN3xjb5xjb64EdPc
	leaq	(%r13,%rbp), %rax
	movq	%r12, %rdi
	vmovq	%rax, %xmm0
	callq	_ZN3xjb5xjb64EdPc
	movq	72(%rsp), %rax                  # 8-byte Reload
	movq	%r12, %rdi
	leaq	(%rax,%rbp), %r14
	vmovq	%r14, %xmm0
	callq	_ZN3xjb5xjb64EdPc
	addq	%r15, %rbp
	movq	%r12, %rdi
	vmovq	%rbp, %xmm0
	vmovq	%xmm0, 24(%rsp)                 # 8-byte Folded Spill
	callq	_ZN3xjb5xjb64EdPc
	decq	%rbx
	movq	%r14, %rbp
	jne	.LBB11_1
# %bb.2:
	movq	8(%rsp), %rcx                   # 8-byte Reload
	shlq	$32, %rcx
	orq	(%rsp), %rcx                    # 8-byte Folded Reload
	#APP
	rdtsc
	#NO_APP
	movq	%rdx, %r15
	shlq	$32, %r15
	orq	%rax, %r15
	subq	%rcx, %r15
	callq	_ZNSt6chrono3_V212system_clock3nowEv
	subq	16(%rsp), %rax                  # 8-byte Folded Reload
	vmovsd	.LCPI11_0(%rip), %xmm1          # xmm1 = [9.3132257461547852E-10,0.0E+0]
	movl	$.L.str.10, %edi
	vcvtsi2sd	%rax, %xmm15, %xmm0
	movb	$2, %al
	vmulsd	%xmm1, %xmm0, %xmm0
	vcvtusi2sd	%r15, %xmm15, %xmm2
	vmulsd	%xmm1, %xmm2, %xmm1
	callq	printf
	vmovsd	24(%rsp), %xmm0                 # 8-byte Reload
                                        # xmm0 = mem[0],zero
	leaq	80(%rsp), %rsi
	movl	$.L.str.11, %edi
	movb	$1, %al
	callq	printf
	xorl	%eax, %eax
	addq	$152, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%r12
	.cfi_def_cfa_offset 40
	popq	%r13
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end11:
	.size	main, .Lfunc_end11-main
	.cfi_endproc
                                        # -- End function
	.section	.text.__clang_call_terminate,"axG",@progbits,__clang_call_terminate,comdat
	.hidden	__clang_call_terminate          # -- Begin function __clang_call_terminate
	.weak	__clang_call_terminate
	.p2align	4
	.type	__clang_call_terminate,@function
__clang_call_terminate:                 # 
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	callq	__cxa_begin_catch
	callq	_ZSt9terminatev
.Lfunc_end12:
	.size	__clang_call_terminate, .Lfunc_end12-__clang_call_terminate
	.cfi_endproc
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	4                               # -- Begin function _GLOBAL__sub_I_main.cpp
	.type	_GLOBAL__sub_I_main.cpp,@function
_GLOBAL__sub_I_main.cpp:                # 
.Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception1
# %bb.0:
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$40, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	leaq	24(%rsp), %r14
	movq	%r14, 8(%rsp)
	movl	$1634100580, 24(%rsp)           # imm = 0x61666564
	movl	$1953264993, 27(%rsp)           # imm = 0x746C7561
	movq	$7, 16(%rsp)
	movb	$0, 31(%rsp)
.Ltmp3:
	leaq	8(%rsp), %rsi
	movl	$rd, %edi
	callq	_ZNSt13random_device7_M_initERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Ltmp4:
# %bb.1:
	movq	8(%rsp), %rdi
	cmpq	%r14, %rdi
	je	.LBB13_3
# %bb.2:
	movq	24(%rsp), %rsi
	incq	%rsi
	callq	_ZdlPvm
.LBB13_3:
	movl	$_ZNSt13random_deviceD2Ev, %edi
	movl	$rd, %esi
	movl	$__dso_handle, %edx
	movabsq	$6364136223846793005, %rbx      # imm = 0x5851F42D4C957F2D
	callq	__cxa_atexit
	movl	$rd, %edi
	callq	_ZNSt13random_device9_M_getvalEv
	movl	%eax, %edx
	xorl	%eax, %eax
	movq	%rdx, gen(%rip)
	.p2align	4
.LBB13_4:                               # =>This Inner Loop Header: Depth=1
	movq	%rdx, %rcx
	shrq	$62, %rcx
	xorq	%rdx, %rcx
	imulq	%rbx, %rcx
	leaq	1(%rax,%rcx), %rdx
	leaq	gen+8(,%rax,8), %rcx
	movq	%rdx, %rsi
	shrq	$62, %rsi
	movq	%rdx, (%rcx)
	xorq	%rdx, %rsi
	imulq	%rbx, %rsi
	leaq	2(%rax,%rsi), %rdx
	movq	%rdx, %rsi
	shrq	$62, %rsi
	movq	%rdx, 8(%rcx)
	xorq	%rdx, %rsi
	imulq	%rbx, %rsi
	leaq	3(%rax,%rsi), %rdx
	movq	%rdx, %rsi
	shrq	$62, %rsi
	movq	%rdx, 16(%rcx)
	xorq	%rdx, %rsi
	imulq	%rbx, %rsi
	leaq	4(%rax,%rsi), %rdx
	movq	%rdx, %rsi
	shrq	$62, %rsi
	movq	%rdx, 24(%rcx)
	xorq	%rdx, %rsi
	imulq	%rbx, %rsi
	leaq	5(%rax,%rsi), %rdx
	movq	%rdx, %rsi
	shrq	$62, %rsi
	movq	%rdx, 32(%rcx)
	xorq	%rdx, %rsi
	imulq	%rbx, %rsi
	leaq	6(%rax,%rsi), %rsi
	movq	%rsi, %rdx
	shrq	$62, %rdx
	movq	%rsi, 40(%rcx)
	xorq	%rsi, %rdx
	imulq	%rbx, %rdx
	leaq	7(%rax,%rdx), %rdx
	movq	%rdx, %rsi
	shrq	$62, %rsi
	movq	%rdx, 48(%rcx)
	xorq	%rdx, %rsi
	imulq	%rbx, %rsi
	leaq	8(%rax,%rsi), %rdx
	addq	$8, %rax
	movq	%rdx, 56(%rcx)
	cmpq	$304, %rax                      # imm = 0x130
	jne	.LBB13_4
# %bb.5:
	addq	%rax, %rsi
	movq	%rsi, %rax
	shrq	$62, %rax
	xorq	%rsi, %rax
	imulq	%rbx, %rax
	addq	$305, %rax                      # imm = 0x131
	movq	%rax, %rcx
	shrq	$62, %rcx
	movq	%rax, gen+2440(%rip)
	xorq	%rax, %rcx
	imulq	%rbx, %rcx
	addq	$306, %rcx                      # imm = 0x132
	movq	%rcx, %rax
	shrq	$62, %rax
	movq	%rcx, gen+2448(%rip)
	xorq	%rcx, %rax
	imulq	%rbx, %rax
	addq	$307, %rax                      # imm = 0x133
	movq	%rax, %rcx
	shrq	$62, %rcx
	movq	%rax, gen+2456(%rip)
	xorq	%rax, %rcx
	imulq	%rbx, %rcx
	addq	$308, %rcx                      # imm = 0x134
	movq	%rcx, %rax
	shrq	$62, %rax
	movq	%rcx, gen+2464(%rip)
	xorq	%rcx, %rax
	imulq	%rbx, %rax
	addq	$309, %rax                      # imm = 0x135
	movq	%rax, %rcx
	shrq	$62, %rcx
	movq	%rax, gen+2472(%rip)
	xorq	%rax, %rcx
	imulq	%rbx, %rcx
	addq	$310, %rcx                      # imm = 0x136
	movq	%rcx, %rax
	shrq	$62, %rax
	movq	%rcx, gen+2480(%rip)
	xorq	%rcx, %rax
	imulq	%rbx, %rax
	addq	$311, %rax                      # imm = 0x137
	movq	%rax, gen+2488(%rip)
	movq	$312, gen+2496(%rip)            # imm = 0x138
	addq	$40, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.LBB13_6:
	.cfi_def_cfa_offset 64
.Ltmp5:
	movq	8(%rsp), %rdi
	movq	%rax, %rbx
	cmpq	%r14, %rdi
	je	.LBB13_8
# %bb.7:
	movq	24(%rsp), %rsi
	incq	%rsi
	callq	_ZdlPvm
.LBB13_8:
	movq	%rbx, %rdi
	callq	_Unwind_Resume@PLT
.Lfunc_end13:
	.size	_GLOBAL__sub_I_main.cpp, .Lfunc_end13-_GLOBAL__sub_I_main.cpp
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2, 0x0
GCC_except_table13:
.Lexception1:
	.byte	255                             # @LPStart Encoding = omit
	.byte	255                             # @TType Encoding = omit
	.byte	1                               # Call site Encoding = uleb128
	.uleb128 .Lcst_end1-.Lcst_begin1
.Lcst_begin1:
	.uleb128 .Ltmp3-.Lfunc_begin1           # >> Call Site 1 <<
	.uleb128 .Ltmp4-.Ltmp3                  #   Call between .Ltmp3 and .Ltmp4
	.uleb128 .Ltmp5-.Lfunc_begin1           #     jumps to .Ltmp5
	.byte	0                               #   On action: cleanup
	.uleb128 .Ltmp4-.Lfunc_begin1           # >> Call Site 2 <<
	.uleb128 .Lfunc_end13-.Ltmp4            #   Call between .Ltmp4 and .Lfunc_end13
	.byte	0                               #     has no landing pad
	.byte	0                               #   On action: cleanup
.Lcst_end1:
	.p2align	2, 0x0
                                        # -- End function
	.type	_ZL12double_table,@object       # 
	.section	.rodata,"a",@progbits
	.p2align	3, 0x0
_ZL12double_table:
	.quad	-3720041912917459700            # 0xcc5fc196fefd7d0c
	.quad	2185351144835019465             # 0x1e53ed49a96272c9
	.quad	-38366372719436721              # 0xff77b1fcbebcdc4f
	.quad	2731688931043774331             # 0x25e8e89c13bb0f7b
	.quad	-6941508010590729807            # 0x9faacf3df73609b1
	.quad	8624834609543440813             # 0x77b191618c54e9ad
	.quad	-4065198994811024355            # 0xc795830d75038c1d
	.quad	-3054014793352862696            # 0xd59df5b9ef6a2418
	.quad	-469812725086392539             # 0xf97ae3d0d2446f25
	.quad	5405853545163697438             # 0x4b0573286b44ad1e
	.quad	-7211161980820077193            # 0x9becce62836ac577
	.quad	5684501474941004851             # 0x4ee367f9430aec33
	.quad	-4402266457597708587            # 0xc2e801fb244576d5
	.quad	2493940825248868160             # 0x229c41f793cda740
	.quad	-891147053569747830             # 0xf3a20279ed56d48a
	.quad	7729112049988473104             # 0x6b43527578c11110
	.quad	-7474495936122174250            # 0x9845418c345644d6
	.quad	-9004363024039368022            # 0x830a13896b78aaaa
	.quad	-4731433901725329908            # 0xbe5691ef416bd60c
	.quad	2579604275232953684             # 0x23cc986bc656d554
	.quad	-1302606358729274481            # 0xedec366b11c6cb8f
	.quad	3224505344041192105             # 0x2cbfbe86b7ec8aa9
	.quad	-7731658001846878407            # 0x94b3a202eb1c3f39
	.quad	8932844867666826922             # 0x7bf7d71432f3d6aa
	.quad	-5052886483881210105            # 0xb9e08a83a5e34f07
	.quad	-2669001970698630060            # 0xdaf5ccd93fb0cc54
	.quad	-1704422086424124727            # 0xe858ad248f5c22c9
	.quad	-3336252463373287575            # 0xd1b3400f8f9cff69
	.quad	-7982792831656159810            # 0x91376c36d99995be
	.quad	2526528228819083170             # 0x23100809b9c21fa2
	.quad	-5366805021142811859            # 0xb58547448ffffb2d
	.quad	-6065211750830921845            # 0xabd40a0c2832a78b
	.quad	-2096820258001126919            # 0xe2e69915b3fff9f9
	.quad	1641857348316123501             # 0x16c90c8f323f516d
	.quad	-8228041688891786181            # 0x8dd01fad907ffc3b
	.quad	-5891368184943504668            # 0xae3da7d97f6792e4
	.quad	-5673366092687344822            # 0xb1442798f49ffb4a
	.quad	-7364210231179380835            # 0x99cd11cfdf41779d
	.quad	-2480021597431793123            # 0xdd95317f31c7fa1d
	.quad	4629795266307937668             # 0x40405643d711d584
	.quad	-8467542526035952558            # 0x8a7d3eef7f1cfc52
	.quad	5199465050656154995             # 0x482835ea666b2573
	.quad	-5972742139117552794            # 0xad1c8eab5ee43b66
	.quad	-2724040723534582064            # 0xda3243650005eed0
	.quad	-2854241655469553088            # 0xd863b256369d4a40
	.quad	-8016736922845615485            # 0x90bed43e40076a83
	.quad	-8701430062309552536            # 0x873e4f75e2224e68
	.quad	6518754469289960082             # 0x5a7744a6e804a292
	.quad	-6265101559459552766            # 0xa90de3535aaae202
	.quad	8148443086612450103             # 0x711515d0a205cb37
	.quad	-3219690930897053053            # 0xd3515c2831559a83
	.quad	962181821410786820              # 0xd5a5b44ca873e04
	.quad	-8929835859451740015            # 0x8412d9991ed58091
	.quad	-1704479370831952189            # 0xe858790afe9486c3
	.quad	-6550608805887287114            # 0xa5178fff668ae0b6
	.quad	7092772823314835571             # 0x626e974dbe39a873
	.quad	-3576574988931720989            # 0xce5d73ff402d98e3
	.quad	-357406007711231344             # 0xfb0a3d212dc81290
	.quad	-9152888395723407474            # 0x80fa687f881c7f8e
	.quad	8999993282035256218             # 0x7ce66634bc9d0b9a
	.quad	-6829424476226871438            # 0xa139029f6a239f72
	.quad	2026619565689294465             # 0x1c1fffc1ebc44e81
	.quad	-3925094576856201394            # 0xc987434744ac874e
	.quad	-6690097579743157727            # 0xa327ffb266b56221
	.quad	-294682202642863838             # 0xfbe9141915d7a922
	.quad	5472436080603216553             # 0x4bf1ff9f0062baa9
	.quad	-7101705404292871755            # 0x9d71ac8fada6c9b5
	.quad	8031958568804398250             # 0x6f773fc3603db4aa
	.quad	-4265445736938701790            # 0xc4ce17b399107c22
	.quad	-3795109844276665900            # 0xcb550fb4384d21d4
	.quad	-720121152745989333             # 0xf6019da07f549b2b
	.quad	9091170749936331337             # 0x7e2a53a146606a49
	.quad	-7367604748107325189            # 0x99c102844f94e0fb
	.quad	3376138709496513134             # 0x2eda7444cbfc426e
	.quad	-4597819916706768583            # 0xc0314325637a1939
	.quad	-391512631556746487             # 0xfa911155fefb5309
	.quad	-1135588877456072824            # 0xf03d93eebc589f88
	.quad	8733981247408842699             # 0x793555ab7eba27cb
	.quad	-7627272076051127371            # 0x96267c7535b763b5
	.quad	5458738279630526687             # 0x4bc1558b2f3458df
	.quad	-4922404076636521310            # 0xbbb01b9283253ca2
	.quad	-7011635205744005353            # 0x9eb1aaedfb016f17
	.quad	-1541319077368263733            # 0xea9c227723ee8bcb
	.quad	5070514048102157021             # 0x465e15a979c1cadd
	.quad	-7880853450996246689            # 0x92a1958a7675175f
	.quad	863228270850154186              # 0xbfacd89ec191eca
	.quad	-5239380795317920458            # 0xb749faed14125d36
	.quad	-3532650679864695172            # 0xcef980ec671f667c
	.quad	-1937539975720012668            # 0xe51c79a85916f484
	.quad	-9027499368258256869            # 0x82b7e12780e7401b
	.quad	-8128491512466089774            # 0x8f31cc0937ae58d2
	.quad	-3336344095947716591            # 0xd1b2ecb8b0908811
	.quad	-5548928372155224313            # 0xb2fe3f0b8599ef07
	.quad	-8782116138362033642            # 0x861fa7e6dcb4aa16
	.quad	-2324474446766642487            # 0xdfbdcece67006ac9
	.quad	7469098900757009563             # 0x67a791e093e1d49b
	.quad	-8370325556870233411            # 0x8bd6a141006042bd
	.quad	-2249342214667950879            # 0xe0c8bb2c5c6d24e1
	.quad	-5851220927660403859            # 0xaecc49914078536d
	.quad	6411694268519837209             # 0x58fae9f773886e19
	.quad	-2702340141148116920            # 0xda7f5bf590966848
	.quad	-5820440219632367201            # 0xaf39a475506a899f
	.quad	-8606491615858654931            # 0x888f99797a5e012d
	.quad	7891439908798240260             # 0x6d8406c952429604
	.quad	-6146428501395930760            # 0xaab37fd7d8f58178
	.quad	-3970758169284363388            # 0xc8e5087ba6d33b84
	.quad	-3071349608317525546            # 0xd5605fcdcf32e1d6
	.quad	-351761693178066331             # 0xfb1e4a9a90880a65
	.quad	-8837122532839535322            # 0x855c3be0a17fcd26
	.quad	6697677969404790400             # 0x5cf2eea09a550680
	.quad	-6434717147622031249            # 0xa6b34ad8c9dfc06f
	.quad	-851274575098787809             # 0xf42faa48c0ea481f
	.quad	-3431710416100151157            # 0xd0601d8efc57b08b
	.quad	-1064093218873484761            # 0xf13b94daf124da27
	.quad	-9062348037703676329            # 0x823c12795db6ce57
	.quad	8558313775058847833             # 0x76c53d08d6b70859
	.quad	-6716249028702207507            # 0xa2cb1717b52481ed
	.quad	6086206200396171887             # 0x54768c4b0c64ca6f
	.quad	-3783625267450371480            # 0xcb7ddcdda26da268
	.quad	-6227300304786948854            # 0xa9942f5dcf7dfd0a
	.quad	-117845565885576446             # 0xfe5d54150b090b02
	.quad	-3172439362556298163            # 0xd3f93b35435d7c4d
	.quad	-6991182506319567135            # 0x9efa548d26e5a6e1
	.quad	-4288617610811380304            # 0xc47bc5014a1a6db0
	.quad	-4127292114472071014            # 0xc6b8e9b0709f109a
	.quad	3862600023340550428             # 0x359ab6419ca1091c
	.quad	-547429124662700864             # 0xf867241c8cc6d4c0
	.quad	-4395122007679087773            # 0xc30163d203c94b63
	.quad	-7259672230555269896            # 0x9b407691d7fc44f8
	.quad	8782263791269039902             # 0x79e0de63425dcf1e
	.quad	-4462904269766699466            # 0xc21094364dfb5636
	.quad	-7468914334623251739            # 0x985915fc12f542e5
	.quad	-966944318780986428             # 0xf294b943e17a2bc4
	.quad	4498915137003099038             # 0x3e6f5b7b17b2939e
	.quad	-7521869226879198374            # 0x979cf3ca6cec5b5a
	.quad	-6411550076227838909            # 0xa705992ceecf9c43
	.quad	-4790650515171610063            # 0xbd8430bd08277231
	.quad	5820620459997365076             # 0x50c6ff782a838354
	.quad	-1376627125537124675            # 0xece53cec4a314ebd
	.quad	-6559282480285457367            # 0xa4f8bf5635246429
	.quad	-7777920981101784778            # 0x940f4613ae5ed136
	.quad	-8711237568605798758            # 0x871b7795e136be9a
	.quad	-5110715207949843068            # 0xb913179899f68584
	.quad	2946011094524915264             # 0x28e2557b59846e40
	.quad	-1776707991509915931            # 0xe757dd7ec07426e5
	.quad	3682513868156144080             # 0x331aeada2fe589d0
	.quad	-8027971522334779313            # 0x9096ea6f3848984f
	.quad	4607414176811284002             # 0x3ff0d2c85def7622
	.quad	-5423278384491086237            # 0xb4bca50b065abe63
	.quad	1147581702586717098             # 0xfed077a756b53aa
	.quad	-2167411962186469893            # 0xe1ebce4dc7f16dfb
	.quad	-3177208890193991531            # 0xd3e8495912c62895
	.quad	-8272161504007625539            # 0x8d3360f09cf6e4bd
	.quad	7237616480483531101             # 0x64712dd7abbbd95d
	.quad	-5728515861582144020            # 0xb080392cc4349dec
	.quad	-4788037454677749836            # 0xbd8d794d96aacfb4
	.quad	-2548958808550292121            # 0xdca04777f541c567
	.quad	-1373360799919799391            # 0xecf0d7a0fc5583a1
	.quad	-8510628282985014432            # 0x89e42caaf9491b60
	.quad	-858350499949874619             # 0xf41686c49db57245
	.quad	-6026599335303880135            # 0xac5d37d5b79b6239
	.quad	3538747893490044630             # 0x311c2875c522ced6
	.quad	-2921563150702462265            # 0xd77485cb25823ac7
	.quad	9035120885289943692             # 0x7d633293366b828c
	.quad	-8743505996830120772            # 0x86a8d39ef77164bc
	.quad	-5882264492762254952            # 0xae5dff9c02033198
	.quad	-6317696477610263061            # 0xa8530886b54dbdeb
	.quad	-2741144597525430787            # 0xd9f57f830283fdfd
	.quad	-3285434578585440922            # 0xd267caa862a12d66
	.quad	-3426430746906788484            # 0xd072df63c324fd7c
	.quad	-8970925639256982432            # 0x8380dea93da4bc60
	.quad	4776009810824339054             # 0x4247cb9e59f71e6e
	.quad	-6601971030643840136            # 0xa46116538d0deb78
	.quad	5970012263530423817             # 0x52d9be85f074e609
	.quad	-3640777769877412266            # 0xcd795be870516656
	.quad	7462515329413029772             # 0x67902e276c921f8c
	.quad	-9193015133814464522            # 0x806bd9714632dff6
	.quad	52386062455755703               # 0xba1cd8a3db53b7
	.quad	-6879582898840692749            # 0xa086cfcd97bf97f3
	.quad	-9157889458785081179            # 0x80e8a40eccd228a5
	.quad	-3987792605123478032            # 0xc8a883c0fdaf7df0
	.quad	6999382250228200142             # 0x6122cd128006b2ce
	.quad	-373054737976959636             # 0xfad2a4b13d1b5d6c
	.quad	8749227812785250178             # 0x796b805720085f82
	.quad	-7150688238876681629            # 0x9cc3a6eec6311a63
	.quad	-3755104653863994447            # 0xcbe3303674053bb1
	.quad	-4326674280168464132            # 0xc3f490aa77bd60fc
	.quad	-4693880817329993059            # 0xbedbfc4411068a9d
	.quad	-796656831783192261             # 0xf4f1b4d515acb93b
	.quad	-1255665003235103419            # 0xee92fb5515482d45
	.quad	-7415439547505577019            # 0x991711052d8bf3c5
	.quad	8438581409832836171             # 0x751bdd152d4d1c4b
	.quad	-4657613415954583370            # 0xbf5cd54678eef0b6
	.quad	-3286831292991118498            # 0xd262d45a78a0635e
	.quad	-1210330751515841308            # 0xef340a98172aace4
	.quad	-8720225134666286027            # 0x86fb897116c87c35
	.quad	-7673985747338482674            # 0x9580869f0e7aac0e
	.quad	-3144297699952734815            # 0xd45d35e6ae3d4da1
	.quad	-4980796165745715438            # 0xbae0a846d2195712
	.quad	-8542058143368306422            # 0x8974836059cca10a
	.quad	-1614309188754756393            # 0xe998d258869facd7
	.quad	3157485376071780684             # 0x2bd1a438703fc94c
	.quad	-7926472270612804602            # 0x91ff83775423cc06
	.quad	8890957387685944784             # 0x7b6306a34627ddd0
	.quad	-5296404319838617848            # 0xb67f6455292cbf08
	.quad	1890324697752655171             # 0x1a3bc84c17b1d543
	.quad	-2008819381370884406            # 0xe41f3d6a7377eeca
	.quad	2362905872190818964             # 0x20caba5f1d9e4a94
	.quad	-8173041140997884610            # 0x8e938662882af53e
	.quad	6088502188546649757             # 0x547eb47b7282ee9d
	.quad	-5604615407819967859            # 0xb23867fb2a35b28d
	.quad	-1612744301171463612            # 0xe99e619a4f23aa44
	.quad	-2394083241347571919            # 0xdec681f9f4c31f31
	.quad	7207441660390446293             # 0x6405fa00e2ec94d5
	.quad	-8413831053483314306            # 0x8b3c113c38f9f37e
	.quad	-2412877989897052923            # 0xde83bc408dd3dd05
	.quad	-5905602798426754978            # 0xae0b158b4738705e
	.quad	-7627783505798704058            # 0x9624ab50b148d446
	.quad	-2770317479606055818            # 0xd98ddaee19068c76
	.quad	4300328673033783640             # 0x3badd624dd9b0958
	.quad	-8648977452394866743            # 0x87f8a8d4cfa417c9
	.quad	-1923980597781273129            # 0xe54ca5d70a80e5d7
	.quad	-6199535797066195524            # 0xa9f6d30a038d1dbc
	.quad	6818396289628184397             # 0x5e9fcf4ccd211f4d
	.quad	-3137733727905356501            # 0xd47487cc8470652b
	.quad	8522995362035230496             # 0x7647c32000696720
	.quad	-8878612607581929669            # 0x84c8d4dfd2c63f3b
	.quad	3021029092058325108             # 0x29ecd9f40041e074
	.quad	-6486579741050024183            # 0xa5fb0a17c777cf09
	.quad	-835399653354481519             # 0xf468107100525891
	.quad	-3496538657885142324            # 0xcf79cc9db955c2cc
	.quad	8179122470161673909             # 0x7182148d4066eeb5
	.quad	-9102865688819295809            # 0x81ac1fe293d599bf
	.quad	-4111420493003729615            # 0xc6f14cd848405531
	.quad	-6766896092596731857            # 0xa21727db38cb002f
	.quad	-5139275616254662019            # 0xb8ada00e5a506a7d
	.quad	-3846934097318526917            # 0xca9cf1d206fdc03b
	.quad	-6424094520318327523            # 0xa6d90811f0e4851d
	.quad	-196981603220770742             # 0xfd442e4688bd304a
	.quad	-8030118150397909404            # 0x908f4a166d1da664
	.quad	-7040642529654063570            # 0x9e4a9cec15763e2e
	.quad	-7324666853212387329            # 0x9a598e4e043287ff
	.quad	-4189117143640191558            # 0xc5dd44271ad3cdba
	.quad	4679224488766679550             # 0x40eff1e1853f29fe
	.quad	-624710411122851544             # 0xf7549530e188c128
	.quad	-3374341425896426371            # 0xd12bee59e68ef47d
	.quad	-7307973034592864071            # 0x9a94dd3e8cf578b9
	.quad	-9026492418826348337            # 0x82bb74f8301958cf
	.quad	-4523280274813692185            # 0xc13a148e3032d6e7
	.quad	-2059743486678159614            # 0xe36a52363c1faf02
	.quad	-1042414325089727327            # 0xf18899b1bc3f8ca1
	.quad	-2574679358347699518            # 0xdc44e6c3cb279ac2
	.quad	-7569037980822161435            # 0x96f5600f15a7b7e5
	.quad	3002511419460075706             # 0x29ab103a5ef8c0ba
	.quad	-4849611457600313890            # 0xbcb2b812db11a5de
	.quad	8364825292752482536             # 0x7415d448f6b6f0e8
	.quad	-1450328303573004458            # 0xebdf661791d60f56
	.quad	1232659579085827362             # 0x111b495b3464ad22
	.quad	-7823984217374209643            # 0x936b9fcebb25c995
	.quad	-3841273781498745803            # 0xcab10dd900beec35
	.quad	-5168294253290374149            # 0xb84687c269ef3bfb
	.quad	4421779809981343555             # 0x3d5d514f40eea743
	.quad	-1848681798185579782            # 0xe65829b3046b0afa
	.quad	915538744049291539              # 0xcb4a5a3112a5113
	.quad	-8072955151507069220            # 0x8ff71a0fe2c2e6dc
	.quad	5183897733458195116             # 0x47f0e785eaba72ac
	.quad	-5479507920956448621            # 0xb3f4e093db73a093
	.quad	6479872166822743895             # 0x59ed216765690f57
	.quad	-2237698882768172872            # 0xe0f218b8d25088b8
	.quad	3488154190101041965             # 0x306869c13ec3532d
	.quad	-8316090829371189901            # 0x8c974f7383725573
	.quad	2180096368813151228             # 0x1e414218c73a13fc
	.quad	-5783427518286599473            # 0xafbd2350644eeacf
	.quad	-1886565557410948869            # 0xe5d1929ef90898fb
	.quad	-2617598379430861437            # 0xdbac6c247d62a583
	.quad	-2358206946763686086            # 0xdf45f746b74abf3a
	.quad	-8553528014785370254            # 0x894bc396ce5da772
	.quad	7749492695127472004             # 0x6b8bba8c328eb784
	.quad	-6080224000054324913            # 0xab9eb47c81f5114f
	.quad	463493832054564197              # 0x66ea92f3f326565
	.quad	-2988593981640518238            # 0xd686619ba27255a2
	.quad	-4032318728359182658            # 0xc80a537b0efefebe
	.quad	-8785400266166405755            # 0x8613fd0145877585
	.quad	-4826042214438183113            # 0xbd06742ce95f5f37
	.quad	-6370064314280619289            # 0xa798fc4196e952e7
	.quad	3190819268807046917             # 0x2c48113823b73705
	.quad	-3350894374423386208            # 0xd17f3b51fca3a7a0
	.quad	-623161932418579258             # 0xf75a15862ca504c6
	.quad	-9011838011655698236            # 0x82ef85133de648c4
	.quad	-7307005235402693892            # 0x9a984d73dbe722fc
	.quad	-6653111496142234891            # 0xa3ab66580d5fdaf5
	.quad	-4522070525825979461            # 0xc13e60d0d2e0ebbb
	.quad	-3704703351750405709            # 0xcc963fee10b7d1b3
	.quad	3570783879572301481             # 0x318df905079926a9
	.quad	-19193171260619233              # 0xffbbcfe994e5c61f
	.quad	-148206168962011053             # 0xfdf17746497f7053
	.quad	-6929524759678968877            # 0x9fd561f1fd0f9bd3
	.quad	-92628855601256908              # 0xfeb6ea8bedefa634
	.quad	-4050219931171323192            # 0xc7caba6e7c5382c8
	.quad	-115786069501571135             # 0xfe64a52ee96b8fc1
	.quad	-451088895536766085             # 0xf9bd690a1b68637b
	.quad	4466953431550423985             # 0x3dfdce7aa3c673b1
	.quad	-7199459587351560659            # 0x9c1661a651213e2d
	.quad	486002885505321039              # 0x6bea10ca65c084f
	.quad	-4387638465762062920            # 0xc31bfa0fe5698db8
	.quad	5219189625309039203             # 0x486e494fcff30a63
	.quad	-872862063775190746             # 0xf3e2f893dec3f126
	.quad	6523987031636299003             # 0x5a89dba3c3efccfb
	.quad	-7463067817500576073            # 0x986ddb5c6b3a76b7
	.quad	-534194123654701027             # 0xf89629465a75e01d
	.quad	-4717148753448332187            # 0xbe89523386091465
	.quad	-667742654568376284             # 0xf6bbb397f1135824
	.quad	-1284749923383027329            # 0xee2ba6c0678b597f
	.quad	8388693718644305453             # 0x746aa07ded582e2d
	.quad	-7720497729755473937            # 0x94db483840b717ef
	.quad	-6286281471915778851            # 0xa8c2a44eb4571cdd
	.quad	-5038936143766954517            # 0xba121a4650e4ddeb
	.quad	-7857851839894723564            # 0x92f34d62616ce414
	.quad	-1686984161281305242            # 0xe896a0d7e51e1566
	.quad	8624429273841147160             # 0x77b020baf9c81d18
	.quad	-7971894128441897632            # 0x915e2486ef32cd60
	.quad	778582277723329071              # 0xace1474dc1d122f
	.quad	-5353181642124984136            # 0xb5b5ada8aaff80b8
	.quad	973227847154161339              # 0xd819992132456bb
	.quad	-2079791034228842266            # 0xe3231912d5bf60e6
	.quad	1216534808942701674             # 0x10e1fff697ed6c6a
	.quad	-8217398424034108273            # 0x8df5efabc5979c8f
	.quad	-3851351762838199358            # 0xca8d3ffa1ef463c2
	.quad	-5660062011615247437            # 0xb1736b96b6fd83b3
	.quad	-4814189703547749197            # 0xbd308ff8a6b17cb3
	.quad	-2463391496091671392            # 0xddd0467c64bce4a0
	.quad	-6017737129434686497            # 0xac7cb3f6d05ddbdf
	.quad	-8457148712698376476            # 0x8aa22c0dbef60ee4
	.quad	7768129340171790700             # 0x6bcdf07a423aa96c
	.quad	-5959749872445582691            # 0xad4ab7112eb3929d
	.quad	-8736582398494813241            # 0x86c16c98d2c953c7
	.quad	-2838001322129590460            # 0xd89d64d57a607744
	.quad	-1697355961263740744            # 0xe871c7bf077ba8b8
	.quad	-8691279853972075893            # 0x87625f056c7c4a8b
	.quad	1244995533423855987             # 0x11471cd764ad4973
	.quad	-6252413799037706963            # 0xa93af6c6c79b5d2d
	.quad	-3055441601647567920            # 0xd598e40d3dd89bd0
	.quad	-3203831230369745799            # 0xd389b47879823479
	.quad	5404070034795315908             # 0x4aff1d108d4ec2c4
	.quad	-8919923546622172981            # 0x843610cb4bf160cb
	.quad	-3539985255894009413            # 0xcedf722a585139bb
	.quad	-6538218414850328322            # 0xa54394fe1eedb8fe
	.quad	-4424981569867511767            # 0xc2974eb4ee658829
	.quad	-3561087000135522498            # 0xce947a3da6a9273e
	.quad	8303831092947774003             # 0x733d226229feea33
	.quad	-9143208402725783417            # 0x811ccc668829b887
	.quad	578208414664970848              # 0x806357d5a3f5260
	.quad	-6817324484979841368            # 0xa163ff802a3426a8
	.quad	-3888925500096174344            # 0xca07c2dcb0cf26f8
	.quad	-3909969587797413806            # 0xc9bcff6034c13052
	.quad	-249470856692830026             # 0xfc89b393dd02f0b6
	.quad	-275775966319379353             # 0xfc2c3f3841f17c67
	.quad	-4923524589293425437            # 0xbbac2078d443ace3
	.quad	-7089889006590693952            # 0x9d9ba7832936edc0
	.quad	-3077202868308390898            # 0xd54b944b84aa4c0e
	.quad	-4250675239810979535            # 0xc5029163f384a931
	.quad	765182433041899282              # 0xa9e795e65d4df12
	.quad	-701658031336336515             # 0xf64335bcf065d37d
	.quad	5568164059729762006             # 0x4d4617b5ff4a16d6
	.quad	-7356065297226292178            # 0x99ea0196163fa42e
	.quad	5785945546544795206             # 0x504bced1bf8e4e46
	.quad	-4583395603105477319            # 0xc06481fb9bcf8d39
	.quad	-1990940103673781801            # 0xe45ec2862f71e1d7
	.quad	-1117558485454458744            # 0xf07da27a82c37088
	.quad	6734696907262548557             # 0x5d767327bb4e5a4d
	.quad	-7616003081050118571            # 0x964e858c91ba2655
	.quad	4209185567039092848             # 0x3a6a07f8d510f870
	.quad	-4908317832885260310            # 0xbbe226efb628afea
	.quad	-8573576096483297652            # 0x890489f70a55368c
	.quad	-1523711272679187483            # 0xeadab0aba3b2dbe5
	.quad	3118087934678041647             # 0x2b45ac74ccea842f
	.quad	-7869848573065574033            # 0x92c8ae6b464fc96f
	.quad	4254647968387469982             # 0x3b0b8bc90012929e
	.quad	-5225624697904579637            # 0xb77ada0617e3bbcb
	.quad	706623942056949573              # 0x9ce6ebb40173745
	.quad	-1920344853953336643            # 0xe55990879ddcaabd
	.quad	-3728406090856200938            # 0xcc420a6a101d0516
	.quad	-8117744561361917258            # 0x8f57fa54c2a9eab6
	.quad	-6941939825212513490            # 0x9fa946824a12232e
	.quad	-5535494683275008668            # 0xb32df8e9f3546564
	.quad	5157633273766521850             # 0x47939822dc96abfa
	.quad	-2307682335666372931            # 0xdff9772470297ebd
	.quad	6447041592208152312             # 0x59787e2b93bc56f8
	.quad	-8359830487432564938            # 0x8bfbea76c619ef36
	.quad	6335244004343789147             # 0x57eb4edb3c55b65b
	.quad	-5838102090863318269            # 0xaefae51477a06b03
	.quad	-1304317031425039374            # 0xede622920b6b23f2
	.quad	-2685941595151759932            # 0xdab99e59958885c4
	.quad	-1630396289281299218            # 0xe95fab368e45ecee
	.quad	-8596242524610931813            # 0x88b402f7fd75539b
	.quad	1286845328412881941             # 0x11dbcb0218ebb415
	.quad	-6133617137336276863            # 0xaae103b5fcd2a881
	.quad	-3003129357911285478            # 0xd652bdc29f26a11a
	.quad	-3055335403242958174            # 0xd59944a37c0752a2
	.quad	5469460339465668960             # 0x4be76d3346f04960
	.quad	-8827113654667930715            # 0x857fcae62d8493a5
	.quad	8030098730593431004             # 0x6f70a4400c562ddc
	.quad	-6422206049907525490            # 0xa6dfbd9fb8e5b88e
	.quad	-3797434642040374957            # 0xcb4ccd500f6bb953
	.quad	-3416071543957018958            # 0xd097ad07a71f26b2
	.quad	9088264752731695016             # 0x7e2000a41346a7a8
	.quad	-9052573742614218705            # 0x825ecc24c873782f
	.quad	-8154892584824854327            # 0x8ed400668c0c28c9
	.quad	-6704031159840385477            # 0xa2f67f2dfa90563b
	.quad	8253128342678483707             # 0x728900802f0f32fb
	.quad	-3768352931373093942            # 0xcbb41ef979346bca
	.quad	5704724409920716730             # 0x4f2b40a03ad2ffba
	.quad	-98755145788979524              # 0xfea126b7d78186bc
	.quad	-2092466524453879895            # 0xe2f610c84987bfa9
	.quad	-6979250993759194058            # 0x9f24b832e6b0f436
	.quad	998051431430019018              # 0xdd9ca7d2df4d7ca
	.quad	-4112377723771604669            # 0xc6ede63fa05d3143
	.quad	-7975807747567252036            # 0x91503d1c79720dbc
	.quad	-528786136287117932             # 0xf8a95fcf88747d94
	.quad	8476984389250486571             # 0x75a44c6397ce912b
	.quad	-7248020362820530564            # 0x9b69dbe1b548ce7c
	.quad	-3925256793573221701            # 0xc986afbe3ee11abb
	.quad	-4448339435098275301            # 0xc24452da229b021b
	.quad	-294884973539139223             # 0xfbe85badce996169
	.quad	-948738275445456222             # 0xf2d56790ab41c2a2
	.quad	-368606216923924028             # 0xfae27299423fb9c4
	.quad	-7510490449794491995            # 0x97c560ba6b0919a5
	.quad	-2536221894791146469            # 0xdccd879fc967d41b
	.quad	-4776427043815727089            # 0xbdb6b8e905cb600f
	.quad	6053094668365842721             # 0x5400e987bbc1c921
	.quad	-1358847786342270957            # 0xed246723473e3813
	.quad	2954682317029915497             # 0x290123e9aab23b69
	.quad	-7766808894105001205            # 0x9436c0760c86e30b
	.quad	-459166561069996766             # 0xf9a0b6720aaf6522
	.quad	-5096825099203863602            # 0xb94470938fa89bce
	.quad	-573958201337495958             # 0xf808e40e8d5b3e6a
	.quad	-1759345355577441598            # 0xe7958cb87392c2c2
	.quad	-5329133770099257851            # 0xb60b1d1230b20e05
	.quad	-8017119874876982855            # 0x90bd77f3483bb9b9
	.quad	-5636551615525730109            # 0xb1c6f22b5e6f48c3
	.quad	-5409713825168840664            # 0xb4ecd5f01a4aa828
	.quad	2177682517447613172             # 0x1e38aeb6360b1af4
	.quad	-2150456263033662926            # 0xe2280b6c20dd5232
	.quad	2722103146809516465             # 0x25c6da63c38de1b1
	.quad	-8261564192037121185            # 0x8d590723948a535f
	.quad	6313000485183335695             # 0x579c487e5a38ad0f
	.quad	-5715269221619013577            # 0xb0af48ec79ace837
	.quad	3279564588051781714             # 0x2d835a9df0c6d852
	.quad	-2532400508596379068            # 0xdcdb1b2798182244
	.quad	-512230283362660762             # 0xf8e431456cf88e66
	.quad	-8500279345513818773            # 0x8a08f0f8bf0f156b
	.quad	1985699082112030976             # 0x1b8e9ecb641b5900
	.quad	-6013663163464885563            # 0xac8b2d36eed2dac5
	.quad	-2129562165787349184            # 0xe272467e3d222f40
	.quad	-2905392935903719049            # 0xd7adf884aa879177
	.quad	6561419329620589328             # 0x5b0ed81dcc6abb10
	.quad	-8733399612580906262            # 0x86ccbb52ea94baea
	.quad	-7428327965055601430            # 0x98e947129fc2b4ea
	.quad	-6305063497298744923            # 0xa87fea27a539e9a5
	.quad	4549648098962661925             # 0x3f2398d747b36225
	.quad	-3269643353196043250            # 0xd29fe4b18e88640e
	.quad	-8147997931578836306            # 0x8eec7f0d19a03aae
	.quad	-8961056123388608887            # 0x83a3eeeef9153e89
	.quad	1825030320404309165             # 0x1953cf68300424ad
	.quad	-6589634135808373205            # 0xa48ceaaab75a8e2b
	.quad	6892973918932774360             # 0x5fa8c3423c052dd8
	.quad	-3625356651333078602            # 0xcdb02555653131b6
	.quad	4004531380238580046             # 0x3792f412cb06794e
	.quad	-9183376934724255983            # 0x808e17555f3ebf11
	.quad	-2108853905778275375            # 0xe2bbd88bbee40bd1
	.quad	-6867535149977932074            # 0xa0b19d2ab70e6ed6
	.quad	6587304654631931589             # 0x5b6aceaeae9d0ec5
	.quad	-3972732919045027189            # 0xc8de047564d20a8b
	.quad	-989241218564861322             # 0xf245825a5a445276
	.quad	-354230130378896082             # 0xfb158592be068d2e
	.quad	-1236551523206076653            # 0xeed6e2f0f0d56713
	.quad	-7138922859127891907            # 0x9ced737bb6c4183d
	.quad	6144684325637283948             # 0x55464dd69685606c
	.quad	-4311967555482476980            # 0xc428d05aa4751e4c
	.quad	-6154202648235558777            # 0xaa97e14c3c26b887
	.quad	-778273425925708321             # 0xf53304714d9265df
	.quad	-3081067291867060567            # 0xd53dd99f4b3066a9
	.quad	-7403949918844649557            # 0x993fe2c6d07b7fab
	.quad	-1925667057416912854            # 0xe546a8038efe402a
	.quad	-4643251380128424042            # 0xbf8fdb78849a5f96
	.quad	-2407083821771141068            # 0xde98520472bdd034
	.quad	-1192378206733142148            # 0xef73d256a5c0f77c
	.quad	-7620540795641314239            # 0x963e66858f6d4441
	.quad	-7662765406849295699            # 0x95a8637627989aad
	.quad	-2456994988062127447            # 0xdde7001379a44aa9
	.quad	-4966770740134231719            # 0xbb127c53b17ec159
	.quad	6152128301777116499             # 0x5560c018580d5d53
	.quad	-1596777406740401745            # 0xe9d71b689dde71af
	.quad	-6144897678060768089            # 0xaab8f01e6e10b4a7
	.quad	-7915514906853832947            # 0x9226712162ab070d
	.quad	-3840561048787980055            # 0xcab3961304ca70e9
	.quad	-5282707615139903279            # 0xb6b00d69bb55c8d1
	.quad	4422670725869800739             # 0x3d607b97c5fd0d23
	.quad	-1991698500497491195            # 0xe45c10c42a2b3b05
	.quad	-8306719647944912789            # 0x8cb89a7db77c506b
	.quad	-8162340590452013853            # 0x8eb98a7a9a5b04e3
	.quad	8643358275316593219             # 0x77f3608e92adb243
	.quad	-5591239719637629412            # 0xb267ed1940f1c61c
	.quad	6192511825718353620             # 0x55f038b237591ed4
	.quad	-2377363631119648861            # 0xdf01e85f912e37a3
	.quad	7740639782147942025             # 0x6b6c46dec52f6689
	.quad	-8403381297090862394            # 0x8b61313bbabce2c6
	.quad	2532056854628769814             # 0x2323ac4b3b3da016
	.quad	-5892540602936190089            # 0xae397d8aa96c1b77
	.quad	-6058300968568813541            # 0xabec975e0a0d081b
	.quad	-2753989735242849707            # 0xd9c7dced53c72255
	.quad	-7572876210711016926            # 0x96e7bd358c904a22
	.quad	-8638772612167862923            # 0x881cea14545c7575
	.quad	9102010423587778133             # 0x7e50d64177da2e55
	.quad	-6186779746782440750            # 0xaa242499697392d2
	.quad	-2457545025797441046            # 0xdde50bd1d5d0b9ea
	.quad	-3121788665050663033            # 0xd4ad2dbfc3d07787
	.quad	-7683617300674189211            # 0x955e4ec64b44e865
	.quad	-8868646943297746252            # 0x84ec3c97da624ab4
	.quad	-4802260812921368257            # 0xbd5af13bef0b113f
	.quad	-6474122660694794911            # 0xa6274bbdd0fadd61
	.quad	-1391139997724322417            # 0xecb1ad8aeacdd58f
	.quad	-3480967307441105734            # 0xcfb11ead453994ba
	.quad	7484447039699372787             # 0x67de18eda5814af3
	.quad	-9093133594791772940            # 0x81ceb32c4b43fcf4
	.quad	-9157278655470055720            # 0x80eacf948770ced8
	.quad	-6754730975062328271            # 0xa2425ff75e14fc31
	.quad	-6834912300910181746            # 0xa1258379a94d028e
	.quad	-3831727700400522434            # 0xcad2f7f5359a3b3e
	.quad	679731660717048625              # 0x96ee45813a04331
	.quad	-177973607073265139             # 0xfd87b5f28300ca0d
	.quad	-8373707460958465027            # 0x8bca9d6e188853fd
	.quad	-7028762532061872568            # 0x9e74d1b791e07e48
	.quad	8601490892183123070             # 0x775ea264cf55347e
	.quad	-4174267146649952806            # 0xc612062576589dda
	.quad	-7694880458480647778            # 0x95364afe032a819e
	.quad	-606147914885053103             # 0xf79687aed3eec551
	.quad	4216457482181353989             # 0x3a83ddbd83f52205
	.quad	-7296371474444240046            # 0x9abe14cd44753b52
	.quad	-4282243101277735613            # 0xc4926a9672793543
	.quad	-4508778324627912153            # 0xc16d9a0095928a27
	.quad	8482254178684994196             # 0x75b7053c0f178294
	.quad	-1024286887357502287            # 0xf1c90080baf72cb1
	.quad	5991131704928854841             # 0x5324c68b12dd6339
	.quad	-7557708332239520786            # 0x971da05074da7bee
	.quad	-3173071712060547580            # 0xd3f6fc16ebca5e04
	.quad	-4835449396872013078            # 0xbce5086492111aea
	.quad	-8578025658503072379            # 0x88f4bb1ca6bcf585
	.quad	-1432625727662628443            # 0xec1e4a7db69561a5
	.quad	3112525982153323238             # 0x2b31e9e3d06c32e6
	.quad	-7812920107430224633            # 0x9392ee8e921d5d07
	.quad	4251171748059520976             # 0x3aff322e62439fd0
	.quad	-5154464115860392887            # 0xb877aa3236a4b449
	.quad	702278666647013315              # 0x9befeb9fad487c3
	.quad	-1831394126398103205            # 0xe69594bec44de15b
	.quad	5489534351736154548             # 0x4c2ebe687989a9b4
	.quad	-8062150356639896359            # 0x901d7cf73ab0acd9
	.quad	1125115960621402641             # 0xf9d37014bf60a11
	.quad	-5466001927372482545            # 0xb424dc35095cd80f
	.quad	6018080969204141205             # 0x538484c19ef38c95
	.quad	-2220816390788215277            # 0xe12e13424bb40e13
	.quad	2910915193077788602             # 0x2865a5f206b06fba
	.quad	-8305539271883716405            # 0x8cbccc096f5088cb
	.quad	-486521013540076076             # 0xf93f87b7442e45d4
	.quad	-5770238071427257602            # 0xafebff0bcb24aafe
	.quad	-608151266925095095             # 0xf78f69a51539d749
	.quad	-2601111570856684098            # 0xdbe6fecebdedd5be
	.quad	-5371875102083756772            # 0xb573440e5a884d1c
	.quad	-8543223759426509417            # 0x89705f4136b4a597
	.quad	3560107088838733873             # 0x31680a88f8953031
	.quad	-6067343680855748868            # 0xabcc77118461cefc
	.quad	-161552157378970562             # 0xfdc20d2b36ba7c3e
	.quad	-2972493582642298180            # 0xd6bf94d5e57a42bc
	.quad	4409745821703674701             # 0x3d32907604691b4d
	.quad	-8775337516792518219            # 0x8637bd05af6c69b5
	.quad	-6467280898289979120            # 0xa63f9a49c2c1b110
	.quad	-6357485877563259869            # 0xa7c5ac471b478423
	.quad	1139270913992301908             # 0xfcf80dc33721d54
	.quad	-3335171328526686933            # 0xd1b71758e219652b
	.quad	-3187597375937010519            # 0xd3c36113404ea4a9
	.quad	-9002011107970261189            # 0x83126e978d4fdf3b
	.quad	7231123676894144234             # 0x645a1cac083126ea
	.quad	-6640827866535438582            # 0xa3d70a3d70a3d70a
	.quad	4427218577690292388             # 0x3d70a3d70a3d70a4
	.quad	-3689348814741910324            # 0xcccccccccccccccc
	.quad	-3689348814741910323            # 0xcccccccccccccccd
	.quad	-9223372036854775808            # 0x8000000000000000
	.quad	0                               # 0x0
	.quad	-6917529027641081856            # 0xa000000000000000
	.quad	0                               # 0x0
	.quad	-4035225266123964416            # 0xc800000000000000
	.quad	0                               # 0x0
	.quad	-432345564227567616             # 0xfa00000000000000
	.quad	0                               # 0x0
	.quad	-7187745005283311616            # 0x9c40000000000000
	.quad	0                               # 0x0
	.quad	-4372995238176751616            # 0xc350000000000000
	.quad	0                               # 0x0
	.quad	-854558029293551616             # 0xf424000000000000
	.quad	0                               # 0x0
	.quad	-7451627795949551616            # 0x9896800000000000
	.quad	0                               # 0x0
	.quad	-4702848726509551616            # 0xbebc200000000000
	.quad	0                               # 0x0
	.quad	-1266874889709551616            # 0xee6b280000000000
	.quad	0                               # 0x0
	.quad	-7709325833709551616            # 0x9502f90000000000
	.quad	0                               # 0x0
	.quad	-5024971273709551616            # 0xba43b74000000000
	.quad	0                               # 0x0
	.quad	-1669528073709551616            # 0xe8d4a51000000000
	.quad	0                               # 0x0
	.quad	-7960984073709551616            # 0x9184e72a00000000
	.quad	0                               # 0x0
	.quad	-5339544073709551616            # 0xb5e620f480000000
	.quad	0                               # 0x0
	.quad	-2062744073709551616            # 0xe35fa931a0000000
	.quad	0                               # 0x0
	.quad	-8206744073709551616            # 0x8e1bc9bf04000000
	.quad	0                               # 0x0
	.quad	-5646744073709551616            # 0xb1a2bc2ec5000000
	.quad	0                               # 0x0
	.quad	-2446744073709551616            # 0xde0b6b3a76400000
	.quad	0                               # 0x0
	.quad	-8446744073709551616            # 0x8ac7230489e80000
	.quad	0                               # 0x0
	.quad	-5946744073709551616            # 0xad78ebc5ac620000
	.quad	0                               # 0x0
	.quad	-2821744073709551616            # 0xd8d726b7177a8000
	.quad	0                               # 0x0
	.quad	-8681119073709551616            # 0x878678326eac9000
	.quad	0                               # 0x0
	.quad	-6239712823709551616            # 0xa968163f0a57b400
	.quad	0                               # 0x0
	.quad	-3187955011209551616            # 0xd3c21bcecceda100
	.quad	0                               # 0x0
	.quad	-8910000909647051616            # 0x84595161401484a0
	.quad	0                               # 0x0
	.quad	-6525815118631426616            # 0xa56fa5b99019a5c8
	.quad	0                               # 0x0
	.quad	-3545582879861895366            # 0xcecb8f27f4200f3a
	.quad	0                               # 0x0
	.quad	-9133518327554766460            # 0x813f3978f8940984
	.quad	4611686018427387904             # 0x4000000000000000
	.quad	-6805211891016070171            # 0xa18f07d736b90be5
	.quad	5764607523034234880             # 0x5000000000000000
	.quad	-3894828845342699810            # 0xc9f2c9cd04674ede
	.quad	-6629298651489370112            # 0xa400000000000000
	.quad	-256850038250986858             # 0xfc6f7c4045812296
	.quad	5548434740920451072             # 0x4d00000000000000
	.quad	-7078060301547948643            # 0x9dc5ada82b70b59d
	.quad	-1143914305352105984            # 0xf020000000000000
	.quad	-4235889358507547899            # 0xc5371912364ce305
	.quad	7793479155164643328             # 0x6c28000000000000
	.quad	-683175679707046970             # 0xf684df56c3e01bc6
	.quad	-4093209111326359552            # 0xc732000000000000
	.quad	-7344513827457986212            # 0x9a130b963a6c115c
	.quad	4359273333062107136             # 0x3c7f400000000000
	.quad	-4568956265895094861            # 0xc097ce7bc90715b3
	.quad	5449091666327633920             # 0x4b9f100000000000
	.quad	-1099509313941480672            # 0xf0bdc21abb48db20
	.quad	2199678564482154496             # 0x1e86d40000000000
	.quad	-7604722348854507276            # 0x96769950b50d88f4
	.quad	1374799102801346560             # 0x1314448000000000
	.quad	-4894216917640746191            # 0xbc143fa4e250eb31
	.quad	1718498878501683200             # 0x17d955a000000000
	.quad	-1506085128623544835            # 0xeb194f8e1ae525fd
	.quad	6759809616554491904             # 0x5dcfab0800000000
	.quad	-7858832233030797378            # 0x92efd1b8d0cf37be
	.quad	6530724019560251392             # 0x5aa1cae500000000
	.quad	-5211854272861108819            # 0xb7abc627050305ad
	.quad	-1059967012404461568            # 0xf14a3d9e40000000
	.quad	-1903131822648998119            # 0xe596b7b0c643c719
	.quad	7898413271349198848             # 0x6d9ccd05d0000000
	.quad	-8106986416796705681            # 0x8f7e32ce7bea5c6f
	.quad	-1981020733047832576            # 0xe4820023a2000000
	.quad	-5522047002568494197            # 0xb35dbf821ae4f38b
	.quad	-2476275916309790720            # 0xdda2802c8a800000
	.quad	-2290872734783229842            # 0xe0352f62a19e306e
	.quad	-3095344895387238400            # 0xd50b2037ad200000
	.quad	-8349324486880600507            # 0x8c213d9da502de45
	.quad	4982938468024057856             # 0x4526f422cc340000
	.quad	-5824969590173362730            # 0xaf298d050e4395d6
	.quad	-7606384970252091392            # 0x9670b12b7f410000
	.quad	-2669525969289315508            # 0xdaf3f04651d47b4c
	.quad	4327076842467049472             # 0x3c0cdd765f114000
	.quad	-8585982758446904049            # 0x88d8762bf324cd0f
	.quad	-6518949010312869888            # 0xa5880a69fb6ac800
	.quad	-6120792429631242157            # 0xab0e93b6efee0053
	.quad	-8148686262891087360            # 0x8eea0d047a457a00
	.quad	-3039304518611664792            # 0xd5d238a4abe98068
	.quad	8260886245095692416             # 0x72a4904598d6d880
	.quad	-8817094351773372351            # 0x85a36366eb71f041
	.quad	5163053903184807760             # 0x47a6da2b7f864750
	.quad	-6409681921289327535            # 0xa70c3c40a64e6c51
	.quad	-7381240676301154012            # 0x999090b65f67d924
	.quad	-3400416383184271515            # 0xd0cf4b50cfe20765
	.quad	-3178808521666707               # 0xfff4b4e3f741cf6d
	.quad	-9042789267131251553            # 0x82818f1281ed449f
	.quad	-4613672773753429595            # 0xbff8f10e7a8921a5
	.quad	-6691800565486676537            # 0xa321f2d7226895c7
	.quad	-5767090967191786994            # 0xaff72d52192b6a0e
	.quad	-3753064688430957767            # 0xcbea6f8ceb02bb39
	.quad	-7208863708989733743            # 0x9bf4f8a69f764491
	.quad	-79644842111309304              # 0xfee50b7025c36a08
	.quad	212292400617608629              # 0x2f236d04753d5b5
	.quad	-6967307053960650171            # 0x9f4f2726179a2245
	.quad	132682750386005393              # 0x1d762422c946591
	.quad	-4097447799023424810            # 0xc722f0ef9d80aad6
	.quad	4777539456409894646             # 0x424d3ad2b7b97ef6
	.quad	-510123730351893109             # 0xf8ebad2b84e0d58b
	.quad	-3251447716342407501            # 0xd2e0898765a7deb3
	.quad	-7236356359111015049            # 0x9b934c3b330c8577
	.quad	7191217214140771120             # 0x63cc55f49f88eb30
	.quad	-4433759430461380907            # 0xc2781f49ffcfa6d5
	.quad	4377335499248575996             # 0x3cbf6b71c76b25fc
	.quad	-930513269649338230             # 0xf316271c7fc3908a
	.quad	-8363388681221443717            # 0x8bef464e3945ef7b
	.quad	-7499099821171918250            # 0x97edd871cfda3a56
	.quad	-7532960934977096275            # 0x97758bf0e3cbb5ad
	.quad	-4762188758037509908            # 0xbde94e8e43d0c8ec
	.quad	4418856886560793368             # 0x3d52eeed1cbea318
	.quad	-1341049929119499481            # 0xed63a231d4c4fb27
	.quad	5523571108200991710             # 0x4ca7aaa863ee4bde
	.quad	-7755685233340769032            # 0x945e455f24fb1cf8
	.quad	-8076983103442849941            # 0x8fe8caa93e74ef6b
	.quad	-5082920523248573386            # 0xb975d6b6ee39e436
	.quad	-5484542860876174523            # 0xb3e2fd538e122b45
	.quad	-1741964635633328828            # 0xe7d34c64a9c85d44
	.quad	6979379479186945559             # 0x60dbbca87196b617
	.quad	-8006256924911912374            # 0x90e40fbeea1d3a4a
	.quad	-4861259862362934834            # 0xbc8955e946fe31ce
	.quad	-5396135137712502563            # 0xb51d13aea4a488dd
	.quad	7758483227328495170             # 0x6babab6398bdbe42
	.quad	-2133482903713240300            # 0xe264589a4dcdab14
	.quad	-4136954021121544750            # 0xc696963c7eed2dd2
	.quad	-8250955842461857044            # 0x8d7eb76070a08aec
	.quad	-279753253987271517             # 0xfc1e1de5cf543ca3
	.quad	-5702008784649933400            # 0xb0de65388cc8ada8
	.quad	4261994450943298508             # 0x3b25a55f43294bcc
	.quad	-2515824962385028846            # 0xdd15fe86affad912
	.quad	5327493063679123135             # 0x49ef0eb713f39ebf
	.quad	-8489919629131724885            # 0x8a2dbf142dfcc7ab
	.quad	7941369183226839864             # 0x6e3569326c784338
	.quad	-6000713517987268202            # 0xacb92ed9397bf996
	.quad	5315025460606161925             # 0x49c2c37f07965405
	.quad	-2889205879056697349            # 0xd7e77a8f87daf7fb
	.quad	-2579590211097073401            # 0xdc33745ec97be907
	.quad	-8723282702051517699            # 0x86f0ac99b4e8dafd
	.quad	7611128154919104932             # 0x69a028bb3ded71a4
	.quad	-6292417359137009220            # 0xa8acd7c0222311bc
	.quad	-4321147861633282547            # 0xc40832ea0d68ce0d
	.quad	-3253835680493873621            # 0xd2d80db02aabd62b
	.quad	-789748808614215279             # 0xf50a3fa490c30191
	.quad	-8951176327949752869            # 0x83c7088e1aab65db
	.quad	8729779031470891259             # 0x792667c6da79e0fb
	.quad	-6577284391509803182            # 0xa4b8cab1a1563f52
	.quad	6300537770911226169             # 0x577001b891185939
	.quad	-3609919470959866074            # 0xcde6fd5e09abcf26
	.quad	-1347699823215743097            # 0xed4c0226b55e6f87
	.quad	-9173728696990998152            # 0x80b05e5ac60b6178
	.quad	6075216638131242421             # 0x544f8158315b05b5
	.quad	-6855474852811359786            # 0xa0dc75f1778e39d6
	.quad	7594020797664053026             # 0x696361ae3db1c722
	.quad	-3957657547586811828            # 0xc913936dd571c84c
	.quad	269153960225290474              # 0x3bc3a19cd1e38ea
	.quad	-335385916056126881             # 0xfb5878494ace3a5f
	.quad	336442450281613092              # 0x4ab48a04065c724
	.quad	-7127145225176161157            # 0x9d174b2dcec0e47b
	.quad	7127805559067090039             # 0x62eb0d64283f9c77
	.quad	-4297245513042813542            # 0xc45d1df942711d9a
	.quad	4298070930406474645             # 0x3ba5d0bd324f8395
	.quad	-759870872876129024             # 0xf5746577930d6500
	.quad	-3850783373846682502            # 0xca8f44ec7ee3647a
	.quad	-7392448323188662496            # 0x9968bf6abbe85f20
	.quad	9122475437414293196             # 0x7e998b13cf4e1ecc
	.quad	-4628874385558440216            # 0xbfc2ef456ae276e8
	.quad	-7043649776941685121            # 0x9e3fedd8c321a67f
	.quad	-1174406963520662366            # 0xefb3ab16c59b14a2
	.quad	-4192876202749718497            # 0xc5cfe94ef3ea101f
	.quad	-7651533379841495835            # 0x95d04aee3b80ece5
	.quad	-4926390635932268013            # 0xbba1f1d158724a13
	.quad	-4952730706374481889            # 0xbb445da9ca61281f
	.quad	3065383741939440792             # 0x2a8a6e45ae8edc98
	.quad	-1579227364540714458            # 0xea1575143cf97226
	.quad	-779956341003086914             # 0xf52d09d71a3293be
	.quad	-7904546130479028392            # 0x924d692ca61be758
	.quad	6430056314514152535             # 0x593c2626705f9c57
	.quad	-5268996644671397586            # 0xb6e0c377cfa2e12e
	.quad	8037570393142690669             # 0x6f8b2fb00c77836d
	.quad	-1974559787411859078            # 0xe498f455c38b997a
	.quad	823590954573587528              # 0xb6dfb9c0f956448
	.quad	-8151628894773493780            # 0x8edf98b59a373fec
	.quad	5126430365035880109             # 0x4724bd4189bd5ead
	.quad	-5577850100039479321            # 0xb2977ee300c50fe7
	.quad	6408037956294850136             # 0x58edec91ec2cb658
	.quad	-2360626606621961247            # 0xdf3d5e9bc0f653e1
	.quad	3398361426941174766             # 0x2f2967b66737e3ee
	.quad	-8392920656779807636            # 0x8b865b215899f46c
	.quad	-4793553135802847627            # 0xbd79e0d20082ee75
	.quad	-5879464802547371641            # 0xae67f1e9aec07187
	.quad	-1380255401326171630            # 0xecd8590680a3aa12
	.quad	-2737644984756826647            # 0xda01ee641a708de9
	.quad	-1725319251657714538            # 0xe80e6f4820cc9496
	.quad	-8628557143114098510            # 0x884134fe908658b2
	.quad	3533361486141316318             # 0x3109058d147fdcde
	.quad	-6174010410465235234            # 0xaa51823e34a7eede
	.quad	-4806670179178130410            # 0xbd4b46f0599fd416
	.quad	-3105826994654156138            # 0xd4e5e2cdc1d1ea96
	.quad	7826720331309500699             # 0x6c9e18ac7007c91b
	.quad	-8858670899299929442            # 0x850fadc09923329e
	.quad	280014188641050033              # 0x3e2cf6bc604ddb1
	.quad	-6461652605697523899            # 0xa6539930bf6bff45
	.quad	-8873354301053463267            # 0x84db8346b786151d
	.quad	-3465379738694516970            # 0xcfe87f7cef46ff16
	.quad	-1868320839462053276            # 0xe612641865679a64
	.quad	-9083391364325154962            # 0x81f14fae158c5f6e
	.quad	5749828502977298559             # 0x4fcb7e8f3f60c07f
	.quad	-6742553186979055799            # 0xa26da3999aef7749
	.quad	-2036086408133152610            # 0xe3be5e330f38f09e
	.quad	-3816505465296431844            # 0xcb090c8001ab551c
	.quad	6678264026688335046             # 0x5cadf5bfd3072cc6
	.quad	-158945813193151901             # 0xfdcb4fa002162a63
	.quad	8347830033360418807             # 0x73d9732fc7c8f7f7
	.quad	-7016870160886801794            # 0x9e9f11c4014dda7e
	.quad	2911550761636567803             # 0x2867e7fddcdd9afb
	.quad	-4159401682681114339            # 0xc646d63501a1511d
	.quad	-5583933584809066055            # 0xb281e1fd541501b9
	.quad	-587566084924005019             # 0xf7d88bc24209a565
	.quad	2243455055843443239             # 0x1f225a7ca91a4227
	.quad	-7284757830718584993            # 0x9ae757596946075f
	.quad	3708002419115845977             # 0x3375788de9b06959
	.quad	-4494261269970843337            # 0xc1a12d2fc3978937
	.quad	23317005467419567               # 0x52d6b1641c83af
	.quad	-1006140569036166268            # 0xf209787bb47d6b84
	.quad	-4582539761593113445            # 0xc0678c5dbd23a49b
	.quad	-7546366883288685774            # 0x9745eb4d50ce6332
	.quad	-558244341782001951             # 0xf840b7ba963646e1
	.quad	-4821272585683469313            # 0xbd176620a501fbff
	.quad	-5309491445654890343            # 0xb650e5a93bc3d899
	.quad	-1414904713676948737            # 0xec5d3fa8ce427aff
	.quad	-6636864307068612929            # 0xa3e51f138ab4cebf
	.quad	-7801844473689174817            # 0x93ba47c980e98cdf
	.quad	-4148040191917883080            # 0xc66f336c36b10138
	.quad	-5140619573684080617            # 0xb8a8d9bbe123f017
	.quad	-5185050239897353851            # 0xb80b0047445d4185
	.quad	-1814088448677712867            # 0xe6d3102ad96cec1d
	.quad	-6481312799871692314            # 0xa60dc059157491e6
	.quad	-8051334308064652398            # 0x9043ea1ac7e41392
	.quad	-8662506518347195600            # 0x87c89837ad68db30
	.quad	-5452481866653427593            # 0xb454e4a179dd1877
	.quad	3006924907348169212             # 0x29babe4598c311fc
	.quad	-2203916314889396588            # 0xe16a1dc9d8545e94
	.quad	-853029884242176389             # 0xf4296dd6fef3d67b
	.quad	-8294976724446954723            # 0x8ce2529e2734bb1d
	.quad	1772699331562333709             # 0x1899e4a65f58660d
	.quad	-5757034887131305500            # 0xb01ae745b101e9e4
	.quad	6827560182880305040             # 0x5ec05dcff72e7f90
	.quad	-2584607590486743971            # 0xdc21a1171d42645d
	.quad	8534450228600381300             # 0x76707543f4fa1f74
	.quad	-8532908771695296838            # 0x899504ae72497eba
	.quad	7639874402088932265             # 0x6a06494a791c53a9
	.quad	-6054449946191733143            # 0xabfa45da0edbde69
	.quad	326470965756389523              # 0x487db9d17636893
	.quad	-2956376414312278525            # 0xd6f8d7509292d603
	.quad	5019774725622874807             # 0x45a9d2845d3c42b7
	.quad	-8765264286586255934            # 0x865b86925b9bc5c2
	.quad	831516194300602803              # 0xb8a2392ba45a9b3
	.quad	-6344894339805432014            # 0xa7f26836f282b732
	.quad	-8183976793979022305            # 0x8e6cac7768d7141f
	.quad	-3319431906329402113            # 0xd1ef0244af2364ff
	.quad	3605087062808385831             # 0x3207d795430cd927
	.quad	-8992173969096958177            # 0x8335616aed761f1f
	.quad	9170708441896323001             # 0x7f44e6bd49e807b9
	.quad	-6628531442943809817            # 0xa402b9c5a8d3a6e7
	.quad	6851699533943015847             # 0x5f16206c9c6209a7
	.quad	-3673978285252374367            # 0xcd036837130890a1
	.quad	3952938399001381904             # 0x36dba887c37a8c10
	.quad	-9213765455923815836            # 0x802221226be55a64
	.quad	-4446942528265218166            # 0xc2494954da2c978a
	.quad	-6905520801477381891            # 0xa02aa96b06deb0fd
	.quad	-946992141904134803             # 0xf2db9baa10b7bd6d
	.quad	-4020214983419339459            # 0xc83553c5c8965d3d
	.quad	8039631859474607304             # 0x6f92829494e5acc8
	.quad	-413582710846786420             # 0xfa42a8b73abbf48c
	.quad	-3785518230938904582            # 0xcb772339ba1f17fa
	.quad	-7176018221920323369            # 0x9c69a97284b578d7
	.quad	-60105885123121412              # 0xff2a760414536efc
	.quad	-4358336758973016307            # 0xc38413cf25e2d70d
	.quad	-75132356403901765              # 0xfef5138519684abb
	.quad	-836234930288882479             # 0xf46518c2ef5b8cd1
	.quad	9129456591349898602             # 0x7eb258665fc25d6a
	.quad	-7440175859071633406            # 0x98bf2f79d5993802
	.quad	-1211618658047395230            # 0xef2f773ffbd97a62
	.quad	-4688533805412153853            # 0xbeeefb584aff8603
	.quad	-6126209340986631941            # 0xaafb550ffacfd8fb
	.quad	-1248981238337804412            # 0xeeaaba2e5dbf6784
	.quad	-7657761676233289927            # 0x95ba2a53f983cf39
	.quad	-7698142301602209614            # 0x952ab45cfa97a0b2
	.quad	-2480258038432112252            # 0xdd945a747bf26184
	.quad	-5010991858575374113            # 0xba756174393d88df
	.quad	-7712008566467528219            # 0x94f971119aeef9e5
	.quad	-1652053804791829737            # 0xe912b9d1478ceb17
	.quad	8806733365625141342             # 0x7a37cd5601aab85e
	.quad	-7950062655635975442            # 0x91abb422ccb812ee
	.quad	-6025006692552756421            # 0xac62e055c10ab33b
	.quad	-5325892301117581398            # 0xb616a12b7fe617aa
	.quad	6303799689591218186             # 0x577b986b314d600a
	.quad	-2045679357969588844            # 0xe39c49765fdf9d94
	.quad	-1343622424865753076            # 0xed5a7e85fda0b80c
	.quad	-8196078626372074883            # 0x8e41ade9fbebc27d
	.quad	1466078993672598280             # 0x14588f13be847308
	.quad	-5633412264537705700            # 0xb1d219647ae6b31c
	.quad	6444284760518135753             # 0x596eb2d8ae258fc9
	.quad	-2430079312244744221            # 0xde469fbd99a05fe3
	.quad	8055355950647669692             # 0x6fca5f8ed9aef3bc
	.quad	-8436328597794046994            # 0x8aec23d680043bee
	.quad	2728754459941099605             # 0x25de7bb9480d5855
	.quad	-5933724728815170839            # 0xada72ccc20054ae9
	.quad	-5812428961928401301            # 0xaf561aa79a10ae6b
	.quad	-2805469892591575644            # 0xd910f7ff28069da4
	.quad	1957835834444274181             # 0x1b2ba1518094da05
	.quad	-8670947710510816634            # 0x87aa9aff79042286
	.quad	-7999724640327104445            # 0x90fb44d2f05d0843
	.quad	-6226998619711132888            # 0xa99541bf57452b28
	.quad	3835402254873283156             # 0x353a1607ac744a54
	.quad	-3172062256211528206            # 0xd3fa922f2d1675f2
	.quad	4794252818591603945             # 0x42889b8997915ce9
	.quad	-8900067937773286985            # 0x847c9b5d7c2e09b7
	.quad	7608094030047140370             # 0x69956135febada12
	.quad	-6513398903789220827            # 0xa59bc234db398c25
	.quad	4898431519131537558             # 0x43fab9837e699096
	.quad	-3530062611309138130            # 0xcf02b2c21207ef2e
	.quad	-7712018656367741764            # 0x94f967e45e03f4bc
	.quad	-9123818159709293187            # 0x8161afb94b44f57d
	.quad	2097517367411243254             # 0x1d1be0eebac278f6
	.quad	-6793086681209228580            # 0xa1ba1ba79e1632dc
	.quad	7233582727691441971             # 0x6462d92a69731733
	.quad	-3879672333084147821            # 0xca28a291859bbf93
	.quad	9041978409614302463             # 0x7d7b8f7503cfdcff
	.quad	-237904397927796872             # 0xfcb2cb35e702af78
	.quad	6690786993590490175             # 0x5cda735244c3d43f
	.quad	-7066219276345954901            # 0x9defbf01b061adab
	.quad	4181741870994056360             # 0x3a0888136afa64a8
	.quad	-4221088077005055722            # 0xc56baec21c7a1916
	.quad	615491320315182545              # 0x88aaa1845b8fdd1
	.quad	-664674077828931749             # 0xf6c69a72a3989f5b
	.quad	-8454007886460797626            # 0x8aad549e57273d46
	.quad	-7332950326284164199            # 0x9a3c2087a63f6399
	.quad	3939617107816777292             # 0x36ac54e2f678864c
	.quad	-4554501889427817345            # 0xc0cb28a98fcf3c7f
	.quad	-8910536670511192098            # 0x84576a1bb416a7de
	.quad	-1081441343357383777            # 0xf0fdf2d3f3c30b9f
	.quad	7308573235570561494             # 0x656d44a2a11c51d6
	.quad	-7593429867239446717            # 0x969eb7c47859e743
	.quad	-6961356773836868826            # 0x9f644ae5a4b1b326
	.quad	-4880101315621920492            # 0xbc4665b596706114
	.quad	-8701695967296086033            # 0x873d5d9f0dde1fef
	.quad	-1488440626100012711            # 0xeb57ff22fc0c7959
	.quad	-6265433940692719637            # 0xa90cb506d155a7eb
	.quad	-7847804418953589800            # 0x9316ff75dd87cbd8
	.quad	695789805494438131              # 0x9a7f12442d588f3
	.quad	-5198069505264599346            # 0xb7dcbf5354e9bece
	.quad	869737256868047664              # 0xc11ed6d538aeb30
	.quad	-1885900863153361279            # 0xe5d3ef282a242e81
	.quad	-8136200465769716229            # 0x8f1668c8a86da5fb
	.quad	-8096217067111932656            # 0x8fa475791a569d10
	.quad	-473439272678684739             # 0xf96e017d694487bd
	.quad	-5508585315462527915            # 0xb38d92d760ec4455
	.quad	4019886927579031981             # 0x37c981dcc395a9ad
	.quad	-2274045625900771990            # 0xe070f78d3927556a
	.quad	-8810199395808373736            # 0x85bbe253f47b1418
	.quad	-8338807543829064350            # 0x8c469ab843b89562
	.quad	-7812217631593927537            # 0x93956d7478ccec8f
	.quad	-5811823411358942533            # 0xaf58416654a6babb
	.quad	4069786015789754291             # 0x387ac8d1970027b3
	.quad	-2653093245771290262            # 0xdb2e51bfe9d0696a
	.quad	475546501309804959              # 0x6997b05fcc0319f
	.quad	-8575712306248138270            # 0x88fcf317f22241e2
	.quad	4908902581746016004             # 0x441fece3bdf81f04
	.quad	-6107954364382784934            # 0xab3c2fddeeaad25a
	.quad	-3087243809672255804            # 0xd527e81cad7626c4
	.quad	-3023256937051093263            # 0xd60b3bd56a5586f1
	.quad	-8470740780517707659            # 0x8a71e223d8d3b075
	.quad	-8807064613298015146            # 0x85c7056562757456
	.quad	-682526969396179382             # 0xf6872d5667844e4a
	.quad	-6397144748195131028            # 0xa738c6bebb12d16c
	.quad	-5464844730172612132            # 0xb428f8ac016561dc
	.quad	-3384744916816525881            # 0xd106f86e69d785c7
	.quad	-2219369894288377261            # 0xe13336d701beba53
	.quad	-9032994600651410532            # 0x82a45b450226b39c
	.quad	-1387106183930235788            # 0xecc0024661173474
	.quad	-6679557232386875260            # 0xa34d721642b06084
	.quad	2877803288514593169             # 0x27f002d7f95d0191
	.quad	-3737760522056206171            # 0xcc20ce9bd35c78a5
	.quad	3597254110643241461             # 0x31ec038df7b441f5
	.quad	-60514634142869810              # 0xff290242c83396ce
	.quad	9108253656731439730             # 0x7e67047175a15272
	.quad	-6955350673980375487            # 0x9f79a169bd203e41
	.quad	1080972517029761927             # 0xf0062c6e984d387
	.quad	-4082502324048081455            # 0xc75809c42c684dd1
	.quad	5962901664714590313             # 0x52c07b78a3e60869
	.quad	-491441886632713915             # 0xf92e0c3537826145
	.quad	-6381430974388925821            # 0xa7709a56ccdf8a83
	.quad	-7224680206786528053            # 0x9bbcc7a142b17ccb
	.quad	-8600080377420466542            # 0x88a66076400bb692
	.quad	-4419164240055772162            # 0xc2abf989935ddbfe
	.quad	7696643601933968438             # 0x6acff893d00ea436
	.quad	-912269281642327298             # 0xf356f7ebf83552fe
	.quad	397432465562684740              # 0x583f6b8c4124d44
	.quad	-7487697328667536418            # 0x98165af37b2153de
	.quad	-4363290727450709941            # 0xc3727a337a8b704b
	.quad	-4747935642407032618            # 0xbe1bf1b059e9a8d6
	.quad	8380944645968776285             # 0x744f18c0592e4c5d
	.quad	-1323233534581402868            # 0xeda2ee1c7064130c
	.quad	1252808770606194548             # 0x1162def06f79df74
	.quad	-7744549986754458649            # 0x9485d4d1c63e8be7
	.quad	-8440366555225904215            # 0x8addcb5645ac2ba9
	.quad	-5069001465015685407            # 0xb9a74a0637ce2ee1
	.quad	7896285879677171347             # 0x6d953e2bd7173693
	.quad	-1724565812842218855            # 0xe8111c87c5c1ba99
	.quad	-3964700705685699528            # 0xc8fa8db6ccdd0438
	.quad	-7995382660667468640            # 0x910ab1d4db9914a0
	.quad	2133748077373825699             # 0x1d9c9892400a22a3
	.quad	-5382542307406947896            # 0xb54d5e4a127f59c8
	.quad	2667185096717282124             # 0x2503beb6d00cab4c
	.quad	-2116491865831296966            # 0xe2a0b5dc971f303a
	.quad	3333981370896602654             # 0x2e44ae64840fd61e
	.quad	-8240336443785642460            # 0x8da471a9de737e24
	.quad	6695424375237764563             # 0x5ceaecfed289e5d3
	.quad	-5688734536304665171            # 0xb10d8e1456105dad
	.quad	8369280469047205704             # 0x7425a83e872c5f48
	.quad	-2499232151953443560            # 0xdd50f1996b947518
	.quad	-3373457468973156582            # 0xd12f124e28f7771a
	.quad	-8479549122611984081            # 0x8a5296ffe33cc92f
	.quad	-9025939945749304720            # 0x82bd6b70d99aaa70
	.quad	-5987750384837592197            # 0xace73cbfdc0bfb7b
	.quad	7164319141522920716             # 0x636cc64d1001550c
	.quad	-2873001962619602342            # 0xd8210befd30efa5a
	.quad	4343712908476262991             # 0x3c47f7e05401aa4f
	.quad	-8713155254278333320            # 0x8714a775e3e95c78
	.quad	7326506586225052274             # 0x65acfaec34810a72
	.quad	-6279758049420528746            # 0xa8d9d1535ce3b396
	.quad	9158133232781315342             # 0x7f1839a741a14d0e
	.quad	-3238011543348273028            # 0xd31045a8341ca07c
	.quad	2224294504121868369             # 0x1ede48111209a051
	.quad	-8941286242233752499            # 0x83ea2b892091e44d
	.quad	-7833187971778608077            # 0x934aed0aab460433
	.quad	-6564921784364802720            # 0xa4e4b66b68b65d60
	.quad	-568112927868484288             # 0xf81da84d56178540
	.quad	-3594466212028615495            # 0xce1de40642e3f4b9
	.quad	3901544858591782543             # 0x36251260ab9d668f
	.quad	-9164070410158966541            # 0x80d2ae83e9ce78f3
	.quad	-4479063491021217766            # 0xc1d72b7c6b42601a
	.quad	-6843401994271320272            # 0xa1075a24e4421730
	.quad	-5598829363776522208            # 0xb24cf65b8612f820
	.quad	-3942566474411762436            # 0xc94930ae1d529cfc
	.quad	-2386850686293264856            # 0xdee033f26797b628
	.quad	-316522074587315140             # 0xfb9b7cd9a4a7443c
	.quad	1628122660560806834             # 0x169840ef017da3b2
	.quad	-7115355324258153819            # 0x9d412e0806e88aa5
	.quad	-8205795374004271537            # 0x8e1f289560ee864f
	.quad	-4282508136895304370            # 0xc491798a08a2ad4e
	.quad	-1033872180650563613            # 0xf1a6f2bab92a27e3
	.quad	-741449152691742558             # 0xf5b5d7ec8acb58a2
	.quad	-5904026244240592420            # 0xae10af696774b1dc
	.quad	-7380934748073420955            # 0x9991a6f3d6bf1765
	.quad	-5995859411864064214            # 0xacca6da1e0a8ef2a
	.quad	-4614482416664388289            # 0xbff610b0cc6edd3f
	.quad	1728547772024695540             # 0x17fd090a58d32af4
	.quad	-1156417002403097458            # 0xeff394dcff8a948e
	.quad	-2451001303396518479            # 0xddfc4b4cef07f5b1
	.quad	-7640289654143017767            # 0x95f83d0a1fb69cd9
	.quad	5385653213018257807             # 0x4abdaf101564f98f
	.quad	-4938676049251384305            # 0xbb764c4ca7a4440f
	.quad	-7102991539009341454            # 0x9d6d1ad41abe37f2
	.quad	-1561659043136842477            # 0xea53df5fd18d5513
	.quad	-8878739423761676818            # 0x84c86189216dc5ee
	.quad	-7893565929601608404            # 0x92746b9be2f8552c
	.quad	3674159897003727797             # 0x32fd3cf5b4e49bb5
	.quad	-5255271393574622601            # 0xb7118682dbb66a77
	.quad	4592699871254659746             # 0x3fbc8c33221dc2a2
	.quad	-1957403223540890347            # 0xe4d5e82392a40515
	.quad	1129188820640936779             # 0xfabaf3feaa5334b
	.quad	-8140906042354138323            # 0x8f05b1163ba6832d
	.quad	3011586022114279439             # 0x29cb4d87f2a7400f
	.quad	-5564446534515285000            # 0xb2c71d5bca9023f8
	.quad	8376168546070237203             # 0x743e20e9ef511013
	.quad	-2343872149716718346            # 0xdf78e4b2bd342cf6
	.quad	-7976533391121755113            # 0x914da9246b255417
	.quad	-8382449121214030822            # 0x8bab8eefb6409c1a
	.quad	1932195658189984911             # 0x1ad089b6c2f7548f
	.quad	-5866375383090150624            # 0xae9672aba3d0c320
	.quad	-6808127464117294670            # 0xa184ac2473b529b2
	.quad	-2721283210435300376            # 0xda3c0f568cc4f3e8
	.quad	-3898473311719230433            # 0xc9e5d72d90a2741f
	.quad	-8618331034163144591            # 0x8865899617fb1871
	.quad	9092669226243950739             # 0x7e2fa67c7a658893
	.quad	-6161227774276542835            # 0xaa7eebfb9df9de8d
	.quad	-2469221522477225288            # 0xddbb901b98feeab8
	.quad	-3089848699418290639            # 0xd51ea6fa85785631
	.quad	6136845133758244198             # 0x552a74227f3ea566
	.quad	-8848684464777513506            # 0x8533285c936b35de
	.quad	-3082000819042179232            # 0xd53a88958f872760
	.quad	-6449169562544503978            # 0xa67ff273b8460356
	.quad	-8464187042230111944            # 0x8a892abaf368f138
	.quad	-3449775934753242068            # 0xd01fef10a657842c
	.quad	3254824252494523782             # 0x2d2b7569b0432d86
	.quad	-9073638986861858149            # 0x8213f56a67f6b29b
	.quad	-7189106879045698444            # 0x9c3b29620e29fc74
	.quad	-6730362715149934782            # 0xa298f2c501f45f42
	.quad	-8986383598807123056            # 0x8349f3ba91b47b90
	.quad	-3801267375510030573            # 0xcb3f2f7642717713
	.quad	2602078556773259892             # 0x241c70a936219a74
	.quad	-139898200960150313             # 0xfe0efb53d30dd4d7
	.quad	-1359087822460813039            # 0xed238cd383aa0111
	.quad	-7004965403241175802            # 0x9ec95d1463e8a506
	.quad	-849429889038008149             # 0xf4363804324a40ab
	.quad	-4144520735624081848            # 0xc67bb4597ce2ce48
	.quad	-5673473379724898090            # 0xb143c6053edcd0d6
	.quad	-568964901102714406             # 0xf81aa16fdc1b81da
	.quad	-2480155706228734709            # 0xdd94b7868e94050b
	.quad	-7273132090830278360            # 0x9b10a4e5e9913128
	.quad	-3855940325606653145            # 0xca7cf2b4191c8327
	.quad	-4479729095110460046            # 0xc1d4ce1f63f57d72
	.quad	-208239388580928527             # 0xfd1c2f611f63a3f1
	.quad	-987975350460687153             # 0xf24a01a73cf2dccf
	.quad	-4871985254153548563            # 0xbc633b39673c8ced
	.quad	-7535013621679011327            # 0x976e41088617ca01
	.quad	-3044990783845967852            # 0xd5be0503e085d814
	.quad	-4807081008671376254            # 0xbd49d14aa79dbc82
	.quad	5417133557047315993             # 0x4b2d8644d8a74e19
	.quad	-1397165242411832414            # 0xec9c459d51852ba2
	.quad	-2451955090545630817            # 0xddf8e7d60ed1219f
	.quad	-7790757304148477115            # 0x93e1ab8252f33b45
	.quad	-3838314940804713212            # 0xcabb90e5c942b504
	.quad	-5126760611758208489            # 0xb8da1662e7b00a17
	.quad	4425478360848884292             # 0x3d6a751f3b936244
	.quad	-1796764746270372707            # 0xe7109bfba19c0c9d
	.quad	920161932633717461              # 0xcc512670a783ad5
	.quad	-8040506994060064798            # 0x906a617d450187e2
	.quad	2880944217109767366             # 0x27fb2b80668b24c6
	.quad	-5438947724147693094            # 0xb484f9dc9641e9da
	.quad	-5622191765467566601            # 0xb1f9f660802dedf7
	.quad	-2186998636757228463            # 0xe1a63853bbd26451
	.quad	6807318348447705460             # 0x5e7873f8a0396974
	.quad	-8284403175614349646            # 0x8d07e33455637eb2
	.quad	-2662955059861265943            # 0xdb0b487b6423e1e9
	.quad	-5743817951090549153            # 0xb049dc016abc5e5f
	.quad	-7940379843253970333            # 0x91ce1a9a3d2cda63
	.quad	-2568086420435798537            # 0xdc5c5301c56b75f7
	.quad	8521269269642088700             # 0x7641a140cc7810fc
	.quad	-8522583040413455942            # 0x89b9b3e11b6329ba
	.quad	-6203421752542164322            # 0xa9e904c87fcb0a9e
	.quad	-6041542782089432023            # 0xac2820d9623bf429
	.quad	6080780864604458309             # 0x546345fa9fbdcd45
	.quad	-2940242459184402125            # 0xd732290fbacaf133
	.quad	-6234081974526590826            # 0xa97c177947ad4096
	.quad	-8755180564631333184            # 0x867f59a9d4bed6c0
	.quad	5327070802775656542             # 0x49ed8eabcccc485e
	.quad	-6332289687361778576            # 0xa81f301449ee8c70
	.quad	6658838503469570677             # 0x5c68f256bfff5a75
	.quad	-3303676090774835316            # 0xd226fc195c6a2f8c
	.quad	8323548129336963346             # 0x73832eec6fff3112
	.quad	-8982326584375353929            # 0x83585d8fd9c25db7
	.quad	-4021154456019173716            # 0xc831fd53c5ff7eac
	.quad	-6616222212041804507            # 0xa42e74f3d032f525
	.quad	-5026443070023967146            # 0xba3e7ca8b77f5e56
	.quad	-3658591746624867729            # 0xcd3a1230c43fb26f
	.quad	2940318199324816876             # 0x28ce1bd2e55f35ec
	.quad	-9204148869281624187            # 0x80444b5e7aa7cf85
	.quad	8755227902219092404             # 0x7980d163cf5b81b4
	.quad	-6893500068174642330            # 0xa0555e361951c366
	.quad	-2891023177508298208            # 0xd7e105bcc3326220
	.quad	-4005189066790915008            # 0xc86ab5c39fa63440
	.quad	-8225464990312760664            # 0x8dd9472bf3fefaa8
	.quad	-394800315061255856             # 0xfa856334878fc150
	.quad	-5670145219463562926            # 0xb14f98f6f0feb952
	.quad	-7164279224554366766            # 0x9c935e00d4b9d8d2
	.quad	7985374283903742932             # 0x6ed1bf9a569f33d4
	.quad	-4343663012265570553            # 0xc3b8358109e84f07
	.quad	758345818024902857              # 0xa862f80ec4700c9
	.quad	-817892746904575288             # 0xf4a642e14c6262c8
	.quad	-3663753745896259333            # 0xcd27bb612758c0fb
	.quad	-7428711994456441411            # 0x98e7e9cccfbd7dbd
	.quad	-9207375118826243939            # 0x8038d51cb897789d
	.quad	-4674203974643163860            # 0xbf21e44003acdd2c
	.quad	-2285846861678029116            # 0xe0470a63e6bd56c4
	.quad	-1231068949876566920            # 0xeeea5d5004981478
	.quad	1754377441329851509             # 0x1858ccfce06cac75
	.quad	-7686947121313936181            # 0x95527a5202df0ccb
	.quad	1096485900831157193             # 0xf37801e0c43ebc9
	.quad	-4996997883215032323            # 0xbaa718e68396cffd
	.quad	-3241078642388441413            # 0xd30560258f54e6bb
	.quad	-1634561335591402499            # 0xe950df20247c83fd
	.quad	5172023733869224042             # 0x47c6b82ef32a206a
	.quad	-7939129862385708418            # 0x91d28b7416cdd27e
	.quad	5538357842881958978             # 0x4cdc331d57fa5442
	.quad	-5312226309554747619            # 0xb6472e511c81471d
	.quad	-2300424733252327085            # 0xe0133fe4adf8e953
	.quad	-2028596868516046619            # 0xe3d8f9e563a198e5
	.quad	6347841120289366951             # 0x58180fddd97723a7
	.quad	-8185402070463610993            # 0x8e679c2f5e44ff8f
	.quad	6273243709394548297             # 0x570f09eaa7ea7649
	.quad	-5620066569652125837            # 0xb201833b35d63f73
	.quad	3229868618315797467             # 0x2cd2cc6551e513db
	.quad	-2413397193637769393            # 0xde81e40a034bcf4f
	.quad	-574350245532641070             # 0xf8077f7ea65e58d2
	.quad	-8425902273664687727            # 0x8b112e86420f6191
	.quad	-358968903457900669             # 0xfb04afaf27faf783
	.quad	-5920691823653471754            # 0xadd57a27d29339f6
	.quad	8774660907532399972             # 0x79c5db9af1f9b564
	.quad	-2789178761139451788            # 0xd94ad8b1c7380874
	.quad	1744954097560724157             # 0x18375281ae7822bd
	.quad	-8660765753353239224            # 0x87cec76f1c830548
	.quad	-8132775725879323210            # 0x8f2293910d0b15b6
	.quad	-6214271173264161126            # 0xa9c2794ae3a3c69a
	.quad	-5554283638921766109            # 0xb2eb3875504ddb23
	.quad	-3156152948152813503            # 0xd433179d9c8cb841
	.quad	6892203506629956076             # 0x5fa60692a46151ec
	.quad	-8890124620236590296            # 0x849feec281d7f328
	.quad	-2609901835997359308            # 0xdbc7c41ba6bcd334
	.quad	-6500969756868349965            # 0xa5c7ea73224deff3
	.quad	1349308723430688769             # 0x12b9b522906c0801
	.quad	-3514526177658049553            # 0xcf39e50feae16bef
	.quad	-2925050114139026943            # 0xd768226b34870a01
	.quad	-9114107888677362827            # 0x81842f29f2cce375
	.quad	-1828156321336891839            # 0xe6a1158300d46641
	.quad	-6780948842419315629            # 0xa1e53af46f801c53
	.quad	6938176635183661009             # 0x60495ae3c1097fd1
	.quad	-3864500034596756632            # 0xca5e89b18b602368
	.quad	4061034775552188357             # 0x385bb19cb14bdfc5
	.quad	-218939024818557886             # 0xfcf62c1dee382c42
	.quad	5076293469440235446             # 0x46729e03dd9ed7b6
	.quad	360288194370153829              # 0x500003432332d65
	.quad	360288190075186533              # 0x500003332332d65
	.quad	360288185780219237              # 0x500003232332d65
	.quad	360288181485251941              # 0x500003132332d65
	.quad	360288177190284645              # 0x500003032332d65
	.quad	360288215828213093              # 0x500003931332d65
	.quad	360288211533245797              # 0x500003831332d65
	.quad	360288207238278501              # 0x500003731332d65
	.quad	360288202943311205              # 0x500003631332d65
	.quad	360288198648343909              # 0x500003531332d65
	.quad	360288194353376613              # 0x500003431332d65
	.quad	360288190058409317              # 0x500003331332d65
	.quad	360288185763442021              # 0x500003231332d65
	.quad	360288181468474725              # 0x500003131332d65
	.quad	360288177173507429              # 0x500003031332d65
	.quad	360288215811435877              # 0x500003930332d65
	.quad	360288211516468581              # 0x500003830332d65
	.quad	360288207221501285              # 0x500003730332d65
	.quad	360288202926533989              # 0x500003630332d65
	.quad	360288198631566693              # 0x500003530332d65
	.quad	360288194336599397              # 0x500003430332d65
	.quad	360288190041632101              # 0x500003330332d65
	.quad	360288185746664805              # 0x500003230332d65
	.quad	360288181451697509              # 0x500003130332d65
	.quad	360288177156730213              # 0x500003030332d65
	.quad	360288215962365285              # 0x500003939322d65
	.quad	360288211667397989              # 0x500003839322d65
	.quad	360288207372430693              # 0x500003739322d65
	.quad	360288203077463397              # 0x500003639322d65
	.quad	360288198782496101              # 0x500003539322d65
	.quad	360288194487528805              # 0x500003439322d65
	.quad	360288190192561509              # 0x500003339322d65
	.quad	360288185897594213              # 0x500003239322d65
	.quad	360288181602626917              # 0x500003139322d65
	.quad	360288177307659621              # 0x500003039322d65
	.quad	360288215945588069              # 0x500003938322d65
	.quad	360288211650620773              # 0x500003838322d65
	.quad	360288207355653477              # 0x500003738322d65
	.quad	360288203060686181              # 0x500003638322d65
	.quad	360288198765718885              # 0x500003538322d65
	.quad	360288194470751589              # 0x500003438322d65
	.quad	360288190175784293              # 0x500003338322d65
	.quad	360288185880816997              # 0x500003238322d65
	.quad	360288181585849701              # 0x500003138322d65
	.quad	360288177290882405              # 0x500003038322d65
	.quad	360288215928810853              # 0x500003937322d65
	.quad	360288211633843557              # 0x500003837322d65
	.quad	360288207338876261              # 0x500003737322d65
	.quad	360288203043908965              # 0x500003637322d65
	.quad	360288198748941669              # 0x500003537322d65
	.quad	360288194453974373              # 0x500003437322d65
	.quad	360288190159007077              # 0x500003337322d65
	.quad	360288185864039781              # 0x500003237322d65
	.quad	360288181569072485              # 0x500003137322d65
	.quad	360288177274105189              # 0x500003037322d65
	.quad	360288215912033637              # 0x500003936322d65
	.quad	360288211617066341              # 0x500003836322d65
	.quad	360288207322099045              # 0x500003736322d65
	.quad	360288203027131749              # 0x500003636322d65
	.quad	360288198732164453              # 0x500003536322d65
	.quad	360288194437197157              # 0x500003436322d65
	.quad	360288190142229861              # 0x500003336322d65
	.quad	360288185847262565              # 0x500003236322d65
	.quad	360288181552295269              # 0x500003136322d65
	.quad	360288177257327973              # 0x500003036322d65
	.quad	360288215895256421              # 0x500003935322d65
	.quad	360288211600289125              # 0x500003835322d65
	.quad	360288207305321829              # 0x500003735322d65
	.quad	360288203010354533              # 0x500003635322d65
	.quad	360288198715387237              # 0x500003535322d65
	.quad	360288194420419941              # 0x500003435322d65
	.quad	360288190125452645              # 0x500003335322d65
	.quad	360288185830485349              # 0x500003235322d65
	.quad	360288181535518053              # 0x500003135322d65
	.quad	360288177240550757              # 0x500003035322d65
	.quad	360288215878479205              # 0x500003934322d65
	.quad	360288211583511909              # 0x500003834322d65
	.quad	360288207288544613              # 0x500003734322d65
	.quad	360288202993577317              # 0x500003634322d65
	.quad	360288198698610021              # 0x500003534322d65
	.quad	360288194403642725              # 0x500003434322d65
	.quad	360288190108675429              # 0x500003334322d65
	.quad	360288185813708133              # 0x500003234322d65
	.quad	360288181518740837              # 0x500003134322d65
	.quad	360288177223773541              # 0x500003034322d65
	.quad	360288215861701989              # 0x500003933322d65
	.quad	360288211566734693              # 0x500003833322d65
	.quad	360288207271767397              # 0x500003733322d65
	.quad	360288202976800101              # 0x500003633322d65
	.quad	360288198681832805              # 0x500003533322d65
	.quad	360288194386865509              # 0x500003433322d65
	.quad	360288190091898213              # 0x500003333322d65
	.quad	360288185796930917              # 0x500003233322d65
	.quad	360288181501963621              # 0x500003133322d65
	.quad	360288177206996325              # 0x500003033322d65
	.quad	360288215844924773              # 0x500003932322d65
	.quad	360288211549957477              # 0x500003832322d65
	.quad	360288207254990181              # 0x500003732322d65
	.quad	360288202960022885              # 0x500003632322d65
	.quad	360288198665055589              # 0x500003532322d65
	.quad	360288194370088293              # 0x500003432322d65
	.quad	360288190075120997              # 0x500003332322d65
	.quad	360288185780153701              # 0x500003232322d65
	.quad	360288181485186405              # 0x500003132322d65
	.quad	360288177190219109              # 0x500003032322d65
	.quad	360288215828147557              # 0x500003931322d65
	.quad	360288211533180261              # 0x500003831322d65
	.quad	360288207238212965              # 0x500003731322d65
	.quad	360288202943245669              # 0x500003631322d65
	.quad	360288198648278373              # 0x500003531322d65
	.quad	360288194353311077              # 0x500003431322d65
	.quad	360288190058343781              # 0x500003331322d65
	.quad	360288185763376485              # 0x500003231322d65
	.quad	360288181468409189              # 0x500003131322d65
	.quad	360288177173441893              # 0x500003031322d65
	.quad	360288215811370341              # 0x500003930322d65
	.quad	360288211516403045              # 0x500003830322d65
	.quad	360288207221435749              # 0x500003730322d65
	.quad	360288202926468453              # 0x500003630322d65
	.quad	360288198631501157              # 0x500003530322d65
	.quad	360288194336533861              # 0x500003430322d65
	.quad	360288190041566565              # 0x500003330322d65
	.quad	360288185746599269              # 0x500003230322d65
	.quad	360288181451631973              # 0x500003130322d65
	.quad	360288177156664677              # 0x500003030322d65
	.quad	360288215962299749              # 0x500003939312d65
	.quad	360288211667332453              # 0x500003839312d65
	.quad	360288207372365157              # 0x500003739312d65
	.quad	360288203077397861              # 0x500003639312d65
	.quad	360288198782430565              # 0x500003539312d65
	.quad	360288194487463269              # 0x500003439312d65
	.quad	360288190192495973              # 0x500003339312d65
	.quad	360288185897528677              # 0x500003239312d65
	.quad	360288181602561381              # 0x500003139312d65
	.quad	360288177307594085              # 0x500003039312d65
	.quad	360288215945522533              # 0x500003938312d65
	.quad	360288211650555237              # 0x500003838312d65
	.quad	360288207355587941              # 0x500003738312d65
	.quad	360288203060620645              # 0x500003638312d65
	.quad	360288198765653349              # 0x500003538312d65
	.quad	360288194470686053              # 0x500003438312d65
	.quad	360288190175718757              # 0x500003338312d65
	.quad	360288185880751461              # 0x500003238312d65
	.quad	360288181585784165              # 0x500003138312d65
	.quad	360288177290816869              # 0x500003038312d65
	.quad	360288215928745317              # 0x500003937312d65
	.quad	360288211633778021              # 0x500003837312d65
	.quad	360288207338810725              # 0x500003737312d65
	.quad	360288203043843429              # 0x500003637312d65
	.quad	360288198748876133              # 0x500003537312d65
	.quad	360288194453908837              # 0x500003437312d65
	.quad	360288190158941541              # 0x500003337312d65
	.quad	360288185863974245              # 0x500003237312d65
	.quad	360288181569006949              # 0x500003137312d65
	.quad	360288177274039653              # 0x500003037312d65
	.quad	360288215911968101              # 0x500003936312d65
	.quad	360288211617000805              # 0x500003836312d65
	.quad	360288207322033509              # 0x500003736312d65
	.quad	360288203027066213              # 0x500003636312d65
	.quad	360288198732098917              # 0x500003536312d65
	.quad	360288194437131621              # 0x500003436312d65
	.quad	360288190142164325              # 0x500003336312d65
	.quad	360288185847197029              # 0x500003236312d65
	.quad	360288181552229733              # 0x500003136312d65
	.quad	360288177257262437              # 0x500003036312d65
	.quad	360288215895190885              # 0x500003935312d65
	.quad	360288211600223589              # 0x500003835312d65
	.quad	360288207305256293              # 0x500003735312d65
	.quad	360288203010288997              # 0x500003635312d65
	.quad	360288198715321701              # 0x500003535312d65
	.quad	360288194420354405              # 0x500003435312d65
	.quad	360288190125387109              # 0x500003335312d65
	.quad	360288185830419813              # 0x500003235312d65
	.quad	360288181535452517              # 0x500003135312d65
	.quad	360288177240485221              # 0x500003035312d65
	.quad	360288215878413669              # 0x500003934312d65
	.quad	360288211583446373              # 0x500003834312d65
	.quad	360288207288479077              # 0x500003734312d65
	.quad	360288202993511781              # 0x500003634312d65
	.quad	360288198698544485              # 0x500003534312d65
	.quad	360288194403577189              # 0x500003434312d65
	.quad	360288190108609893              # 0x500003334312d65
	.quad	360288185813642597              # 0x500003234312d65
	.quad	360288181518675301              # 0x500003134312d65
	.quad	360288177223708005              # 0x500003034312d65
	.quad	360288215861636453              # 0x500003933312d65
	.quad	360288211566669157              # 0x500003833312d65
	.quad	360288207271701861              # 0x500003733312d65
	.quad	360288202976734565              # 0x500003633312d65
	.quad	360288198681767269              # 0x500003533312d65
	.quad	360288194386799973              # 0x500003433312d65
	.quad	360288190091832677              # 0x500003333312d65
	.quad	360288185796865381              # 0x500003233312d65
	.quad	360288181501898085              # 0x500003133312d65
	.quad	360288177206930789              # 0x500003033312d65
	.quad	360288215844859237              # 0x500003932312d65
	.quad	360288211549891941              # 0x500003832312d65
	.quad	360288207254924645              # 0x500003732312d65
	.quad	360288202959957349              # 0x500003632312d65
	.quad	360288198664990053              # 0x500003532312d65
	.quad	360288194370022757              # 0x500003432312d65
	.quad	360288190075055461              # 0x500003332312d65
	.quad	360288185780088165              # 0x500003232312d65
	.quad	360288181485120869              # 0x500003132312d65
	.quad	360288177190153573              # 0x500003032312d65
	.quad	360288215828082021              # 0x500003931312d65
	.quad	360288211533114725              # 0x500003831312d65
	.quad	360288207238147429              # 0x500003731312d65
	.quad	360288202943180133              # 0x500003631312d65
	.quad	360288198648212837              # 0x500003531312d65
	.quad	360288194353245541              # 0x500003431312d65
	.quad	360288190058278245              # 0x500003331312d65
	.quad	360288185763310949              # 0x500003231312d65
	.quad	360288181468343653              # 0x500003131312d65
	.quad	360288177173376357              # 0x500003031312d65
	.quad	360288215811304805              # 0x500003930312d65
	.quad	360288211516337509              # 0x500003830312d65
	.quad	360288207221370213              # 0x500003730312d65
	.quad	360288202926402917              # 0x500003630312d65
	.quad	360288198631435621              # 0x500003530312d65
	.quad	360288194336468325              # 0x500003430312d65
	.quad	360288190041501029              # 0x500003330312d65
	.quad	360288185746533733              # 0x500003230312d65
	.quad	360288181451566437              # 0x500003130312d65
	.quad	360288177156599141              # 0x500003030312d65
	.quad	289356277018602853              # 0x404000039392d65
	.quad	289356277001825637              # 0x404000038392d65
	.quad	289356276985048421              # 0x404000037392d65
	.quad	289356276968271205              # 0x404000036392d65
	.quad	289356276951493989              # 0x404000035392d65
	.quad	289356276934716773              # 0x404000034392d65
	.quad	289356276917939557              # 0x404000033392d65
	.quad	289356276901162341              # 0x404000032392d65
	.quad	289356276884385125              # 0x404000031392d65
	.quad	289356276867607909              # 0x404000030392d65
	.quad	289356277018537317              # 0x404000039382d65
	.quad	289356277001760101              # 0x404000038382d65
	.quad	289356276984982885              # 0x404000037382d65
	.quad	289356276968205669              # 0x404000036382d65
	.quad	289356276951428453              # 0x404000035382d65
	.quad	289356276934651237              # 0x404000034382d65
	.quad	289356276917874021              # 0x404000033382d65
	.quad	289356276901096805              # 0x404000032382d65
	.quad	289356276884319589              # 0x404000031382d65
	.quad	289356276867542373              # 0x404000030382d65
	.quad	289356277018471781              # 0x404000039372d65
	.quad	289356277001694565              # 0x404000038372d65
	.quad	289356276984917349              # 0x404000037372d65
	.quad	289356276968140133              # 0x404000036372d65
	.quad	289356276951362917              # 0x404000035372d65
	.quad	289356276934585701              # 0x404000034372d65
	.quad	289356276917808485              # 0x404000033372d65
	.quad	289356276901031269              # 0x404000032372d65
	.quad	289356276884254053              # 0x404000031372d65
	.quad	289356276867476837              # 0x404000030372d65
	.quad	289356277018406245              # 0x404000039362d65
	.quad	289356277001629029              # 0x404000038362d65
	.quad	289356276984851813              # 0x404000037362d65
	.quad	289356276968074597              # 0x404000036362d65
	.quad	289356276951297381              # 0x404000035362d65
	.quad	289356276934520165              # 0x404000034362d65
	.quad	289356276917742949              # 0x404000033362d65
	.quad	289356276900965733              # 0x404000032362d65
	.quad	289356276884188517              # 0x404000031362d65
	.quad	289356276867411301              # 0x404000030362d65
	.quad	289356277018340709              # 0x404000039352d65
	.quad	289356277001563493              # 0x404000038352d65
	.quad	289356276984786277              # 0x404000037352d65
	.quad	289356276968009061              # 0x404000036352d65
	.quad	289356276951231845              # 0x404000035352d65
	.quad	289356276934454629              # 0x404000034352d65
	.quad	289356276917677413              # 0x404000033352d65
	.quad	289356276900900197              # 0x404000032352d65
	.quad	289356276884122981              # 0x404000031352d65
	.quad	289356276867345765              # 0x404000030352d65
	.quad	289356277018275173              # 0x404000039342d65
	.quad	289356277001497957              # 0x404000038342d65
	.quad	289356276984720741              # 0x404000037342d65
	.quad	289356276967943525              # 0x404000036342d65
	.quad	289356276951166309              # 0x404000035342d65
	.quad	289356276934389093              # 0x404000034342d65
	.quad	289356276917611877              # 0x404000033342d65
	.quad	289356276900834661              # 0x404000032342d65
	.quad	289356276884057445              # 0x404000031342d65
	.quad	289356276867280229              # 0x404000030342d65
	.quad	289356277018209637              # 0x404000039332d65
	.quad	289356277001432421              # 0x404000038332d65
	.quad	289356276984655205              # 0x404000037332d65
	.quad	289356276967877989              # 0x404000036332d65
	.quad	289356276951100773              # 0x404000035332d65
	.quad	289356276934323557              # 0x404000034332d65
	.quad	289356276917546341              # 0x404000033332d65
	.quad	289356276900769125              # 0x404000032332d65
	.quad	289356276883991909              # 0x404000031332d65
	.quad	289356276867214693              # 0x404000030332d65
	.quad	289356277018144101              # 0x404000039322d65
	.quad	289356277001366885              # 0x404000038322d65
	.quad	289356276984589669              # 0x404000037322d65
	.quad	289356276967812453              # 0x404000036322d65
	.quad	289356276951035237              # 0x404000035322d65
	.quad	289356276934258021              # 0x404000034322d65
	.quad	289356276917480805              # 0x404000033322d65
	.quad	289356276900703589              # 0x404000032322d65
	.quad	289356276883926373              # 0x404000031322d65
	.quad	289356276867149157              # 0x404000030322d65
	.quad	289356277018078565              # 0x404000039312d65
	.quad	289356277001301349              # 0x404000038312d65
	.quad	289356276984524133              # 0x404000037312d65
	.quad	289356276967746917              # 0x404000036312d65
	.quad	289356276950969701              # 0x404000035312d65
	.quad	289356276934192485              # 0x404000034312d65
	.quad	289356276917415269              # 0x404000033312d65
	.quad	289356276900638053              # 0x404000032312d65
	.quad	289356276883860837              # 0x404000031312d65
	.quad	289356276867083621              # 0x404000030312d65
	.quad	289356277018013029              # 0x404000039302d65
	.quad	289356277001235813              # 0x404000038302d65
	.quad	289356276984458597              # 0x404000037302d65
	.quad	289356276967681381              # 0x404000036302d65
	.quad	289356276950904165              # 0x404000035302d65
	.quad	289356276934126949              # 0x404000034302d65
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	289356276967746405              # 0x404000036312b65
	.quad	289356276984523621              # 0x404000037312b65
	.quad	289356277001300837              # 0x404000038312b65
	.quad	289356277018078053              # 0x404000039312b65
	.quad	289356276867148645              # 0x404000030322b65
	.quad	289356276883925861              # 0x404000031322b65
	.quad	289356276900703077              # 0x404000032322b65
	.quad	289356276917480293              # 0x404000033322b65
	.quad	289356276934257509              # 0x404000034322b65
	.quad	289356276951034725              # 0x404000035322b65
	.quad	289356276967811941              # 0x404000036322b65
	.quad	289356276984589157              # 0x404000037322b65
	.quad	289356277001366373              # 0x404000038322b65
	.quad	289356277018143589              # 0x404000039322b65
	.quad	289356276867214181              # 0x404000030332b65
	.quad	289356276883991397              # 0x404000031332b65
	.quad	289356276900768613              # 0x404000032332b65
	.quad	289356276917545829              # 0x404000033332b65
	.quad	289356276934323045              # 0x404000034332b65
	.quad	289356276951100261              # 0x404000035332b65
	.quad	289356276967877477              # 0x404000036332b65
	.quad	289356276984654693              # 0x404000037332b65
	.quad	289356277001431909              # 0x404000038332b65
	.quad	289356277018209125              # 0x404000039332b65
	.quad	289356276867279717              # 0x404000030342b65
	.quad	289356276884056933              # 0x404000031342b65
	.quad	289356276900834149              # 0x404000032342b65
	.quad	289356276917611365              # 0x404000033342b65
	.quad	289356276934388581              # 0x404000034342b65
	.quad	289356276951165797              # 0x404000035342b65
	.quad	289356276967943013              # 0x404000036342b65
	.quad	289356276984720229              # 0x404000037342b65
	.quad	289356277001497445              # 0x404000038342b65
	.quad	289356277018274661              # 0x404000039342b65
	.quad	289356276867345253              # 0x404000030352b65
	.quad	289356276884122469              # 0x404000031352b65
	.quad	289356276900899685              # 0x404000032352b65
	.quad	289356276917676901              # 0x404000033352b65
	.quad	289356276934454117              # 0x404000034352b65
	.quad	289356276951231333              # 0x404000035352b65
	.quad	289356276968008549              # 0x404000036352b65
	.quad	289356276984785765              # 0x404000037352b65
	.quad	289356277001562981              # 0x404000038352b65
	.quad	289356277018340197              # 0x404000039352b65
	.quad	289356276867410789              # 0x404000030362b65
	.quad	289356276884188005              # 0x404000031362b65
	.quad	289356276900965221              # 0x404000032362b65
	.quad	289356276917742437              # 0x404000033362b65
	.quad	289356276934519653              # 0x404000034362b65
	.quad	289356276951296869              # 0x404000035362b65
	.quad	289356276968074085              # 0x404000036362b65
	.quad	289356276984851301              # 0x404000037362b65
	.quad	289356277001628517              # 0x404000038362b65
	.quad	289356277018405733              # 0x404000039362b65
	.quad	289356276867476325              # 0x404000030372b65
	.quad	289356276884253541              # 0x404000031372b65
	.quad	289356276901030757              # 0x404000032372b65
	.quad	289356276917807973              # 0x404000033372b65
	.quad	289356276934585189              # 0x404000034372b65
	.quad	289356276951362405              # 0x404000035372b65
	.quad	289356276968139621              # 0x404000036372b65
	.quad	289356276984916837              # 0x404000037372b65
	.quad	289356277001694053              # 0x404000038372b65
	.quad	289356277018471269              # 0x404000039372b65
	.quad	289356276867541861              # 0x404000030382b65
	.quad	289356276884319077              # 0x404000031382b65
	.quad	289356276901096293              # 0x404000032382b65
	.quad	289356276917873509              # 0x404000033382b65
	.quad	289356276934650725              # 0x404000034382b65
	.quad	289356276951427941              # 0x404000035382b65
	.quad	289356276968205157              # 0x404000036382b65
	.quad	289356276984982373              # 0x404000037382b65
	.quad	289356277001759589              # 0x404000038382b65
	.quad	289356277018536805              # 0x404000039382b65
	.quad	289356276867607397              # 0x404000030392b65
	.quad	289356276884384613              # 0x404000031392b65
	.quad	289356276901161829              # 0x404000032392b65
	.quad	289356276917939045              # 0x404000033392b65
	.quad	289356276934716261              # 0x404000034392b65
	.quad	289356276951493477              # 0x404000035392b65
	.quad	289356276968270693              # 0x404000036392b65
	.quad	289356276985047909              # 0x404000037392b65
	.quad	289356277001825125              # 0x404000038392b65
	.quad	289356277018602341              # 0x404000039392b65
	.quad	360288177156598629              # 0x500003030312b65
	.quad	360288181451565925              # 0x500003130312b65
	.quad	360288185746533221              # 0x500003230312b65
	.quad	360288190041500517              # 0x500003330312b65
	.quad	360288194336467813              # 0x500003430312b65
	.quad	360288198631435109              # 0x500003530312b65
	.quad	360288202926402405              # 0x500003630312b65
	.quad	360288207221369701              # 0x500003730312b65
	.quad	360288211516336997              # 0x500003830312b65
	.quad	360288215811304293              # 0x500003930312b65
	.quad	360288177173375845              # 0x500003031312b65
	.quad	360288181468343141              # 0x500003131312b65
	.quad	360288185763310437              # 0x500003231312b65
	.quad	360288190058277733              # 0x500003331312b65
	.quad	360288194353245029              # 0x500003431312b65
	.quad	360288198648212325              # 0x500003531312b65
	.quad	360288202943179621              # 0x500003631312b65
	.quad	360288207238146917              # 0x500003731312b65
	.quad	360288211533114213              # 0x500003831312b65
	.quad	360288215828081509              # 0x500003931312b65
	.quad	360288177190153061              # 0x500003032312b65
	.quad	360288181485120357              # 0x500003132312b65
	.quad	360288185780087653              # 0x500003232312b65
	.quad	360288190075054949              # 0x500003332312b65
	.quad	360288194370022245              # 0x500003432312b65
	.quad	360288198664989541              # 0x500003532312b65
	.quad	360288202959956837              # 0x500003632312b65
	.quad	360288207254924133              # 0x500003732312b65
	.quad	360288211549891429              # 0x500003832312b65
	.quad	360288215844858725              # 0x500003932312b65
	.quad	360288177206930277              # 0x500003033312b65
	.quad	360288181501897573              # 0x500003133312b65
	.quad	360288185796864869              # 0x500003233312b65
	.quad	360288190091832165              # 0x500003333312b65
	.quad	360288194386799461              # 0x500003433312b65
	.quad	360288198681766757              # 0x500003533312b65
	.quad	360288202976734053              # 0x500003633312b65
	.quad	360288207271701349              # 0x500003733312b65
	.quad	360288211566668645              # 0x500003833312b65
	.quad	360288215861635941              # 0x500003933312b65
	.quad	360288177223707493              # 0x500003034312b65
	.quad	360288181518674789              # 0x500003134312b65
	.quad	360288185813642085              # 0x500003234312b65
	.quad	360288190108609381              # 0x500003334312b65
	.quad	360288194403576677              # 0x500003434312b65
	.quad	360288198698543973              # 0x500003534312b65
	.quad	360288202993511269              # 0x500003634312b65
	.quad	360288207288478565              # 0x500003734312b65
	.quad	360288211583445861              # 0x500003834312b65
	.quad	360288215878413157              # 0x500003934312b65
	.quad	360288177240484709              # 0x500003035312b65
	.quad	360288181535452005              # 0x500003135312b65
	.quad	360288185830419301              # 0x500003235312b65
	.quad	360288190125386597              # 0x500003335312b65
	.quad	360288194420353893              # 0x500003435312b65
	.quad	360288198715321189              # 0x500003535312b65
	.quad	360288203010288485              # 0x500003635312b65
	.quad	360288207305255781              # 0x500003735312b65
	.quad	360288211600223077              # 0x500003835312b65
	.quad	360288215895190373              # 0x500003935312b65
	.quad	360288177257261925              # 0x500003036312b65
	.quad	360288181552229221              # 0x500003136312b65
	.quad	360288185847196517              # 0x500003236312b65
	.quad	360288190142163813              # 0x500003336312b65
	.quad	360288194437131109              # 0x500003436312b65
	.quad	360288198732098405              # 0x500003536312b65
	.quad	360288203027065701              # 0x500003636312b65
	.quad	360288207322032997              # 0x500003736312b65
	.quad	360288211617000293              # 0x500003836312b65
	.quad	360288215911967589              # 0x500003936312b65
	.quad	360288177274039141              # 0x500003037312b65
	.quad	360288181569006437              # 0x500003137312b65
	.quad	360288185863973733              # 0x500003237312b65
	.quad	360288190158941029              # 0x500003337312b65
	.quad	360288194453908325              # 0x500003437312b65
	.quad	360288198748875621              # 0x500003537312b65
	.quad	360288203043842917              # 0x500003637312b65
	.quad	360288207338810213              # 0x500003737312b65
	.quad	360288211633777509              # 0x500003837312b65
	.quad	360288215928744805              # 0x500003937312b65
	.quad	360288177290816357              # 0x500003038312b65
	.quad	360288181585783653              # 0x500003138312b65
	.quad	360288185880750949              # 0x500003238312b65
	.quad	360288190175718245              # 0x500003338312b65
	.quad	360288194470685541              # 0x500003438312b65
	.quad	360288198765652837              # 0x500003538312b65
	.quad	360288203060620133              # 0x500003638312b65
	.quad	360288207355587429              # 0x500003738312b65
	.quad	360288211650554725              # 0x500003838312b65
	.quad	360288215945522021              # 0x500003938312b65
	.quad	360288177307593573              # 0x500003039312b65
	.quad	360288181602560869              # 0x500003139312b65
	.quad	360288185897528165              # 0x500003239312b65
	.quad	360288190192495461              # 0x500003339312b65
	.quad	360288194487462757              # 0x500003439312b65
	.quad	360288198782430053              # 0x500003539312b65
	.quad	360288203077397349              # 0x500003639312b65
	.quad	360288207372364645              # 0x500003739312b65
	.quad	360288211667331941              # 0x500003839312b65
	.quad	360288215962299237              # 0x500003939312b65
	.quad	360288177156664165              # 0x500003030322b65
	.quad	360288181451631461              # 0x500003130322b65
	.quad	360288185746598757              # 0x500003230322b65
	.quad	360288190041566053              # 0x500003330322b65
	.quad	360288194336533349              # 0x500003430322b65
	.quad	360288198631500645              # 0x500003530322b65
	.quad	360288202926467941              # 0x500003630322b65
	.quad	360288207221435237              # 0x500003730322b65
	.quad	360288211516402533              # 0x500003830322b65
	.quad	360288215811369829              # 0x500003930322b65
	.quad	360288177173441381              # 0x500003031322b65
	.quad	360288181468408677              # 0x500003131322b65
	.quad	360288185763375973              # 0x500003231322b65
	.quad	360288190058343269              # 0x500003331322b65
	.quad	360288194353310565              # 0x500003431322b65
	.quad	360288198648277861              # 0x500003531322b65
	.quad	360288202943245157              # 0x500003631322b65
	.quad	360288207238212453              # 0x500003731322b65
	.quad	360288211533179749              # 0x500003831322b65
	.quad	360288215828147045              # 0x500003931322b65
	.quad	360288177190218597              # 0x500003032322b65
	.quad	360288181485185893              # 0x500003132322b65
	.quad	360288185780153189              # 0x500003232322b65
	.quad	360288190075120485              # 0x500003332322b65
	.quad	360288194370087781              # 0x500003432322b65
	.quad	360288198665055077              # 0x500003532322b65
	.quad	360288202960022373              # 0x500003632322b65
	.quad	360288207254989669              # 0x500003732322b65
	.quad	360288211549956965              # 0x500003832322b65
	.quad	360288215844924261              # 0x500003932322b65
	.quad	360288177206995813              # 0x500003033322b65
	.quad	360288181501963109              # 0x500003133322b65
	.quad	360288185796930405              # 0x500003233322b65
	.quad	360288190091897701              # 0x500003333322b65
	.quad	360288194386864997              # 0x500003433322b65
	.quad	360288198681832293              # 0x500003533322b65
	.quad	360288202976799589              # 0x500003633322b65
	.quad	360288207271766885              # 0x500003733322b65
	.quad	360288211566734181              # 0x500003833322b65
	.quad	360288215861701477              # 0x500003933322b65
	.quad	360288177223773029              # 0x500003034322b65
	.quad	360288181518740325              # 0x500003134322b65
	.quad	360288185813707621              # 0x500003234322b65
	.quad	360288190108674917              # 0x500003334322b65
	.quad	360288194403642213              # 0x500003434322b65
	.quad	360288198698609509              # 0x500003534322b65
	.quad	360288202993576805              # 0x500003634322b65
	.quad	360288207288544101              # 0x500003734322b65
	.quad	360288211583511397              # 0x500003834322b65
	.quad	360288215878478693              # 0x500003934322b65
	.quad	360288177240550245              # 0x500003035322b65
	.quad	360288181535517541              # 0x500003135322b65
	.quad	360288185830484837              # 0x500003235322b65
	.quad	360288190125452133              # 0x500003335322b65
	.quad	360288194420419429              # 0x500003435322b65
	.quad	360288198715386725              # 0x500003535322b65
	.quad	360288203010354021              # 0x500003635322b65
	.quad	360288207305321317              # 0x500003735322b65
	.quad	360288211600288613              # 0x500003835322b65
	.quad	360288215895255909              # 0x500003935322b65
	.quad	360288177257327461              # 0x500003036322b65
	.quad	360288181552294757              # 0x500003136322b65
	.quad	360288185847262053              # 0x500003236322b65
	.quad	360288190142229349              # 0x500003336322b65
	.quad	360288194437196645              # 0x500003436322b65
	.quad	360288198732163941              # 0x500003536322b65
	.quad	360288203027131237              # 0x500003636322b65
	.quad	360288207322098533              # 0x500003736322b65
	.quad	360288211617065829              # 0x500003836322b65
	.quad	360288215912033125              # 0x500003936322b65
	.quad	360288177274104677              # 0x500003037322b65
	.quad	360288181569071973              # 0x500003137322b65
	.quad	360288185864039269              # 0x500003237322b65
	.quad	360288190159006565              # 0x500003337322b65
	.quad	360288194453973861              # 0x500003437322b65
	.quad	360288198748941157              # 0x500003537322b65
	.quad	360288203043908453              # 0x500003637322b65
	.quad	360288207338875749              # 0x500003737322b65
	.quad	360288211633843045              # 0x500003837322b65
	.quad	360288215928810341              # 0x500003937322b65
	.quad	360288177290881893              # 0x500003038322b65
	.quad	360288181585849189              # 0x500003138322b65
	.quad	360288185880816485              # 0x500003238322b65
	.quad	360288190175783781              # 0x500003338322b65
	.quad	360288194470751077              # 0x500003438322b65
	.quad	360288198765718373              # 0x500003538322b65
	.quad	360288203060685669              # 0x500003638322b65
	.quad	360288207355652965              # 0x500003738322b65
	.quad	360288211650620261              # 0x500003838322b65
	.quad	360288215945587557              # 0x500003938322b65
	.quad	360288177307659109              # 0x500003039322b65
	.quad	360288181602626405              # 0x500003139322b65
	.quad	360288185897593701              # 0x500003239322b65
	.quad	360288190192560997              # 0x500003339322b65
	.quad	360288194487528293              # 0x500003439322b65
	.quad	360288198782495589              # 0x500003539322b65
	.quad	360288203077462885              # 0x500003639322b65
	.quad	360288207372430181              # 0x500003739322b65
	.quad	360288211667397477              # 0x500003839322b65
	.quad	360288215962364773              # 0x500003939322b65
	.quad	360288177156729701              # 0x500003030332b65
	.quad	360288181451696997              # 0x500003130332b65
	.quad	360288185746664293              # 0x500003230332b65
	.quad	360288190041631589              # 0x500003330332b65
	.quad	360288194336598885              # 0x500003430332b65
	.quad	360288198631566181              # 0x500003530332b65
	.quad	360288202926533477              # 0x500003630332b65
	.quad	360288207221500773              # 0x500003730332b65
	.quad	360288211516468069              # 0x500003830332b65
	.quad	65600                           # 0x10040
	.quad	4                               # 0x4
	.quad	0                               # 0x0
	.quad	9017094859390976                # 0x20090000000000
	.quad	0                               # 0x0
	.quad	256                             # 0x100
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	4194304                         # 0x400000
	.quad	0                               # 0x0
	.quad	131072                          # 0x20000
	.quad	8388608                         # 0x800000
	.quad	0                               # 0x0
	.quad	2251799813685248                # 0x8000000000000
	.quad	1125900980584448                # 0x4000040000000
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	16777216                        # 0x1000000
	.quad	9007199254740992                # 0x20000000000000
	.quad	0                               # 0x0
	.quad	281474976972800                 # 0x1000000040000
	.quad	144115188092633088              # 0x200000001000000
	.quad	1056768                         # 0x102000
	.quad	4294967296                      # 0x100000000
	.quad	2305843009213693952             # 0x2000000000000000
	.quad	131072                          # 0x20000
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	0                               # 0x0
	.quad	1073741824                      # 0x40000000
	.quad	0                               # 0x0
	.quad	32768                           # 0x8000
	.ascii	"\001\002\003\004\005\006\007\b\t\n\013\f\r\016\017\020\021\004\001\001"
	.ascii	"\001\002\003\004\005\006\007\b\t\n\013\f\r\016\017\020\021\003\001\001"
	.ascii	"\001\002\003\004\005\006\007\b\t\n\013\f\r\016\017\020\021\002\001\001"
	.ascii	"\003\003\004\005\006\007\b\t\n\013\f\r\016\017\020\021\022\000\001\002"
	.ascii	"\004\004\004\005\006\007\b\t\n\013\f\r\016\017\020\021\022\000\002\003"
	.ascii	"\005\005\005\005\006\007\b\t\n\013\f\r\016\017\020\021\022\000\003\004"
	.ascii	"\006\006\006\006\006\007\b\t\n\013\f\r\016\017\020\021\022\000\004\005"
	.ascii	"\007\007\007\007\007\007\b\t\n\013\f\r\016\017\020\021\022\000\005\006"
	.ascii	"\b\b\b\b\b\b\b\t\n\013\f\r\016\017\020\021\022\000\006\007"
	.ascii	"\t\t\t\t\t\t\t\t\n\013\f\r\016\017\020\021\022\000\007\b"
	.ascii	"\n\n\n\n\n\n\n\n\n\013\f\r\016\017\020\021\022\000\b\t"
	.ascii	"\013\013\013\013\013\013\013\013\013\013\f\r\016\017\020\021\022\000\t\n"
	.ascii	"\f\f\f\f\f\f\f\f\f\f\f\r\016\017\020\021\022\000\n\013"
	.ascii	"\r\r\r\r\r\r\r\r\r\r\r\r\016\017\020\021\022\000\013\f"
	.ascii	"\016\016\016\016\016\016\016\016\016\016\016\016\016\017\020\021\022\000\f\r"
	.ascii	"\017\017\017\017\017\017\017\017\017\017\017\017\017\017\020\021\022\000\r\016"
	.ascii	"\020\020\020\020\020\020\020\020\020\020\020\020\020\020\020\021\022\000\016\017"
	.ascii	"\021\021\021\021\021\021\021\021\021\021\021\021\021\021\021\021\022\000\017\020"
	.ascii	"\022\022\022\022\022\022\022\022\022\022\022\022\022\022\022\022\022\000\020\021"
	.ascii	"\001\003\004\005\006\007\b\t\n\013\f\r\016\017\020\021\022\000\001\002"
	.size	_ZL12double_table, 15592

	.type	_ZL12pow10_double,@object       # 
	.p2align	4, 0x0
_ZL12pow10_double:
	.quad	-3720041912917459700            # 0xcc5fc196fefd7d0c
	.quad	2185351144835019465             # 0x1e53ed49a96272c9
	.quad	-38366372719436721              # 0xff77b1fcbebcdc4f
	.quad	2731688931043774331             # 0x25e8e89c13bb0f7b
	.quad	-6941508010590729807            # 0x9faacf3df73609b1
	.quad	8624834609543440813             # 0x77b191618c54e9ad
	.quad	-4065198994811024355            # 0xc795830d75038c1d
	.quad	-3054014793352862696            # 0xd59df5b9ef6a2418
	.quad	-469812725086392539             # 0xf97ae3d0d2446f25
	.quad	5405853545163697438             # 0x4b0573286b44ad1e
	.quad	-7211161980820077193            # 0x9becce62836ac577
	.quad	5684501474941004851             # 0x4ee367f9430aec33
	.quad	-4402266457597708587            # 0xc2e801fb244576d5
	.quad	2493940825248868160             # 0x229c41f793cda740
	.quad	-891147053569747830             # 0xf3a20279ed56d48a
	.quad	7729112049988473104             # 0x6b43527578c11110
	.quad	-7474495936122174250            # 0x9845418c345644d6
	.quad	-9004363024039368022            # 0x830a13896b78aaaa
	.quad	-4731433901725329908            # 0xbe5691ef416bd60c
	.quad	2579604275232953684             # 0x23cc986bc656d554
	.quad	-1302606358729274481            # 0xedec366b11c6cb8f
	.quad	3224505344041192105             # 0x2cbfbe86b7ec8aa9
	.quad	-7731658001846878407            # 0x94b3a202eb1c3f39
	.quad	8932844867666826922             # 0x7bf7d71432f3d6aa
	.quad	-5052886483881210105            # 0xb9e08a83a5e34f07
	.quad	-2669001970698630060            # 0xdaf5ccd93fb0cc54
	.quad	-1704422086424124727            # 0xe858ad248f5c22c9
	.quad	-3336252463373287575            # 0xd1b3400f8f9cff69
	.quad	-7982792831656159810            # 0x91376c36d99995be
	.quad	2526528228819083170             # 0x23100809b9c21fa2
	.quad	-5366805021142811859            # 0xb58547448ffffb2d
	.quad	-6065211750830921845            # 0xabd40a0c2832a78b
	.quad	-2096820258001126919            # 0xe2e69915b3fff9f9
	.quad	1641857348316123501             # 0x16c90c8f323f516d
	.quad	-8228041688891786181            # 0x8dd01fad907ffc3b
	.quad	-5891368184943504668            # 0xae3da7d97f6792e4
	.quad	-5673366092687344822            # 0xb1442798f49ffb4a
	.quad	-7364210231179380835            # 0x99cd11cfdf41779d
	.quad	-2480021597431793123            # 0xdd95317f31c7fa1d
	.quad	4629795266307937668             # 0x40405643d711d584
	.quad	-8467542526035952558            # 0x8a7d3eef7f1cfc52
	.quad	5199465050656154995             # 0x482835ea666b2573
	.quad	-5972742139117552794            # 0xad1c8eab5ee43b66
	.quad	-2724040723534582064            # 0xda3243650005eed0
	.quad	-2854241655469553088            # 0xd863b256369d4a40
	.quad	-8016736922845615485            # 0x90bed43e40076a83
	.quad	-8701430062309552536            # 0x873e4f75e2224e68
	.quad	6518754469289960082             # 0x5a7744a6e804a292
	.quad	-6265101559459552766            # 0xa90de3535aaae202
	.quad	8148443086612450103             # 0x711515d0a205cb37
	.quad	-3219690930897053053            # 0xd3515c2831559a83
	.quad	962181821410786820              # 0xd5a5b44ca873e04
	.quad	-8929835859451740015            # 0x8412d9991ed58091
	.quad	-1704479370831952189            # 0xe858790afe9486c3
	.quad	-6550608805887287114            # 0xa5178fff668ae0b6
	.quad	7092772823314835571             # 0x626e974dbe39a873
	.quad	-3576574988931720989            # 0xce5d73ff402d98e3
	.quad	-357406007711231344             # 0xfb0a3d212dc81290
	.quad	-9152888395723407474            # 0x80fa687f881c7f8e
	.quad	8999993282035256218             # 0x7ce66634bc9d0b9a
	.quad	-6829424476226871438            # 0xa139029f6a239f72
	.quad	2026619565689294465             # 0x1c1fffc1ebc44e81
	.quad	-3925094576856201394            # 0xc987434744ac874e
	.quad	-6690097579743157727            # 0xa327ffb266b56221
	.quad	-294682202642863838             # 0xfbe9141915d7a922
	.quad	5472436080603216553             # 0x4bf1ff9f0062baa9
	.quad	-7101705404292871755            # 0x9d71ac8fada6c9b5
	.quad	8031958568804398250             # 0x6f773fc3603db4aa
	.quad	-4265445736938701790            # 0xc4ce17b399107c22
	.quad	-3795109844276665900            # 0xcb550fb4384d21d4
	.quad	-720121152745989333             # 0xf6019da07f549b2b
	.quad	9091170749936331337             # 0x7e2a53a146606a49
	.quad	-7367604748107325189            # 0x99c102844f94e0fb
	.quad	3376138709496513134             # 0x2eda7444cbfc426e
	.quad	-4597819916706768583            # 0xc0314325637a1939
	.quad	-391512631556746487             # 0xfa911155fefb5309
	.quad	-1135588877456072824            # 0xf03d93eebc589f88
	.quad	8733981247408842699             # 0x793555ab7eba27cb
	.quad	-7627272076051127371            # 0x96267c7535b763b5
	.quad	5458738279630526687             # 0x4bc1558b2f3458df
	.quad	-4922404076636521310            # 0xbbb01b9283253ca2
	.quad	-7011635205744005353            # 0x9eb1aaedfb016f17
	.quad	-1541319077368263733            # 0xea9c227723ee8bcb
	.quad	5070514048102157021             # 0x465e15a979c1cadd
	.quad	-7880853450996246689            # 0x92a1958a7675175f
	.quad	863228270850154186              # 0xbfacd89ec191eca
	.quad	-5239380795317920458            # 0xb749faed14125d36
	.quad	-3532650679864695172            # 0xcef980ec671f667c
	.quad	-1937539975720012668            # 0xe51c79a85916f484
	.quad	-9027499368258256869            # 0x82b7e12780e7401b
	.quad	-8128491512466089774            # 0x8f31cc0937ae58d2
	.quad	-3336344095947716591            # 0xd1b2ecb8b0908811
	.quad	-5548928372155224313            # 0xb2fe3f0b8599ef07
	.quad	-8782116138362033642            # 0x861fa7e6dcb4aa16
	.quad	-2324474446766642487            # 0xdfbdcece67006ac9
	.quad	7469098900757009563             # 0x67a791e093e1d49b
	.quad	-8370325556870233411            # 0x8bd6a141006042bd
	.quad	-2249342214667950879            # 0xe0c8bb2c5c6d24e1
	.quad	-5851220927660403859            # 0xaecc49914078536d
	.quad	6411694268519837209             # 0x58fae9f773886e19
	.quad	-2702340141148116920            # 0xda7f5bf590966848
	.quad	-5820440219632367201            # 0xaf39a475506a899f
	.quad	-8606491615858654931            # 0x888f99797a5e012d
	.quad	7891439908798240260             # 0x6d8406c952429604
	.quad	-6146428501395930760            # 0xaab37fd7d8f58178
	.quad	-3970758169284363388            # 0xc8e5087ba6d33b84
	.quad	-3071349608317525546            # 0xd5605fcdcf32e1d6
	.quad	-351761693178066331             # 0xfb1e4a9a90880a65
	.quad	-8837122532839535322            # 0x855c3be0a17fcd26
	.quad	6697677969404790400             # 0x5cf2eea09a550680
	.quad	-6434717147622031249            # 0xa6b34ad8c9dfc06f
	.quad	-851274575098787809             # 0xf42faa48c0ea481f
	.quad	-3431710416100151157            # 0xd0601d8efc57b08b
	.quad	-1064093218873484761            # 0xf13b94daf124da27
	.quad	-9062348037703676329            # 0x823c12795db6ce57
	.quad	8558313775058847833             # 0x76c53d08d6b70859
	.quad	-6716249028702207507            # 0xa2cb1717b52481ed
	.quad	6086206200396171887             # 0x54768c4b0c64ca6f
	.quad	-3783625267450371480            # 0xcb7ddcdda26da268
	.quad	-6227300304786948854            # 0xa9942f5dcf7dfd0a
	.quad	-117845565885576446             # 0xfe5d54150b090b02
	.quad	-3172439362556298163            # 0xd3f93b35435d7c4d
	.quad	-6991182506319567135            # 0x9efa548d26e5a6e1
	.quad	-4288617610811380304            # 0xc47bc5014a1a6db0
	.quad	-4127292114472071014            # 0xc6b8e9b0709f109a
	.quad	3862600023340550428             # 0x359ab6419ca1091c
	.quad	-547429124662700864             # 0xf867241c8cc6d4c0
	.quad	-4395122007679087773            # 0xc30163d203c94b63
	.quad	-7259672230555269896            # 0x9b407691d7fc44f8
	.quad	8782263791269039902             # 0x79e0de63425dcf1e
	.quad	-4462904269766699466            # 0xc21094364dfb5636
	.quad	-7468914334623251739            # 0x985915fc12f542e5
	.quad	-966944318780986428             # 0xf294b943e17a2bc4
	.quad	4498915137003099038             # 0x3e6f5b7b17b2939e
	.quad	-7521869226879198374            # 0x979cf3ca6cec5b5a
	.quad	-6411550076227838909            # 0xa705992ceecf9c43
	.quad	-4790650515171610063            # 0xbd8430bd08277231
	.quad	5820620459997365076             # 0x50c6ff782a838354
	.quad	-1376627125537124675            # 0xece53cec4a314ebd
	.quad	-6559282480285457367            # 0xa4f8bf5635246429
	.quad	-7777920981101784778            # 0x940f4613ae5ed136
	.quad	-8711237568605798758            # 0x871b7795e136be9a
	.quad	-5110715207949843068            # 0xb913179899f68584
	.quad	2946011094524915264             # 0x28e2557b59846e40
	.quad	-1776707991509915931            # 0xe757dd7ec07426e5
	.quad	3682513868156144080             # 0x331aeada2fe589d0
	.quad	-8027971522334779313            # 0x9096ea6f3848984f
	.quad	4607414176811284002             # 0x3ff0d2c85def7622
	.quad	-5423278384491086237            # 0xb4bca50b065abe63
	.quad	1147581702586717098             # 0xfed077a756b53aa
	.quad	-2167411962186469893            # 0xe1ebce4dc7f16dfb
	.quad	-3177208890193991531            # 0xd3e8495912c62895
	.quad	-8272161504007625539            # 0x8d3360f09cf6e4bd
	.quad	7237616480483531101             # 0x64712dd7abbbd95d
	.quad	-5728515861582144020            # 0xb080392cc4349dec
	.quad	-4788037454677749836            # 0xbd8d794d96aacfb4
	.quad	-2548958808550292121            # 0xdca04777f541c567
	.quad	-1373360799919799391            # 0xecf0d7a0fc5583a1
	.quad	-8510628282985014432            # 0x89e42caaf9491b60
	.quad	-858350499949874619             # 0xf41686c49db57245
	.quad	-6026599335303880135            # 0xac5d37d5b79b6239
	.quad	3538747893490044630             # 0x311c2875c522ced6
	.quad	-2921563150702462265            # 0xd77485cb25823ac7
	.quad	9035120885289943692             # 0x7d633293366b828c
	.quad	-8743505996830120772            # 0x86a8d39ef77164bc
	.quad	-5882264492762254952            # 0xae5dff9c02033198
	.quad	-6317696477610263061            # 0xa8530886b54dbdeb
	.quad	-2741144597525430787            # 0xd9f57f830283fdfd
	.quad	-3285434578585440922            # 0xd267caa862a12d66
	.quad	-3426430746906788484            # 0xd072df63c324fd7c
	.quad	-8970925639256982432            # 0x8380dea93da4bc60
	.quad	4776009810824339054             # 0x4247cb9e59f71e6e
	.quad	-6601971030643840136            # 0xa46116538d0deb78
	.quad	5970012263530423817             # 0x52d9be85f074e609
	.quad	-3640777769877412266            # 0xcd795be870516656
	.quad	7462515329413029772             # 0x67902e276c921f8c
	.quad	-9193015133814464522            # 0x806bd9714632dff6
	.quad	52386062455755703               # 0xba1cd8a3db53b7
	.quad	-6879582898840692749            # 0xa086cfcd97bf97f3
	.quad	-9157889458785081179            # 0x80e8a40eccd228a5
	.quad	-3987792605123478032            # 0xc8a883c0fdaf7df0
	.quad	6999382250228200142             # 0x6122cd128006b2ce
	.quad	-373054737976959636             # 0xfad2a4b13d1b5d6c
	.quad	8749227812785250178             # 0x796b805720085f82
	.quad	-7150688238876681629            # 0x9cc3a6eec6311a63
	.quad	-3755104653863994447            # 0xcbe3303674053bb1
	.quad	-4326674280168464132            # 0xc3f490aa77bd60fc
	.quad	-4693880817329993059            # 0xbedbfc4411068a9d
	.quad	-796656831783192261             # 0xf4f1b4d515acb93b
	.quad	-1255665003235103419            # 0xee92fb5515482d45
	.quad	-7415439547505577019            # 0x991711052d8bf3c5
	.quad	8438581409832836171             # 0x751bdd152d4d1c4b
	.quad	-4657613415954583370            # 0xbf5cd54678eef0b6
	.quad	-3286831292991118498            # 0xd262d45a78a0635e
	.quad	-1210330751515841308            # 0xef340a98172aace4
	.quad	-8720225134666286027            # 0x86fb897116c87c35
	.quad	-7673985747338482674            # 0x9580869f0e7aac0e
	.quad	-3144297699952734815            # 0xd45d35e6ae3d4da1
	.quad	-4980796165745715438            # 0xbae0a846d2195712
	.quad	-8542058143368306422            # 0x8974836059cca10a
	.quad	-1614309188754756393            # 0xe998d258869facd7
	.quad	3157485376071780684             # 0x2bd1a438703fc94c
	.quad	-7926472270612804602            # 0x91ff83775423cc06
	.quad	8890957387685944784             # 0x7b6306a34627ddd0
	.quad	-5296404319838617848            # 0xb67f6455292cbf08
	.quad	1890324697752655171             # 0x1a3bc84c17b1d543
	.quad	-2008819381370884406            # 0xe41f3d6a7377eeca
	.quad	2362905872190818964             # 0x20caba5f1d9e4a94
	.quad	-8173041140997884610            # 0x8e938662882af53e
	.quad	6088502188546649757             # 0x547eb47b7282ee9d
	.quad	-5604615407819967859            # 0xb23867fb2a35b28d
	.quad	-1612744301171463612            # 0xe99e619a4f23aa44
	.quad	-2394083241347571919            # 0xdec681f9f4c31f31
	.quad	7207441660390446293             # 0x6405fa00e2ec94d5
	.quad	-8413831053483314306            # 0x8b3c113c38f9f37e
	.quad	-2412877989897052923            # 0xde83bc408dd3dd05
	.quad	-5905602798426754978            # 0xae0b158b4738705e
	.quad	-7627783505798704058            # 0x9624ab50b148d446
	.quad	-2770317479606055818            # 0xd98ddaee19068c76
	.quad	4300328673033783640             # 0x3badd624dd9b0958
	.quad	-8648977452394866743            # 0x87f8a8d4cfa417c9
	.quad	-1923980597781273129            # 0xe54ca5d70a80e5d7
	.quad	-6199535797066195524            # 0xa9f6d30a038d1dbc
	.quad	6818396289628184397             # 0x5e9fcf4ccd211f4d
	.quad	-3137733727905356501            # 0xd47487cc8470652b
	.quad	8522995362035230496             # 0x7647c32000696720
	.quad	-8878612607581929669            # 0x84c8d4dfd2c63f3b
	.quad	3021029092058325108             # 0x29ecd9f40041e074
	.quad	-6486579741050024183            # 0xa5fb0a17c777cf09
	.quad	-835399653354481519             # 0xf468107100525891
	.quad	-3496538657885142324            # 0xcf79cc9db955c2cc
	.quad	8179122470161673909             # 0x7182148d4066eeb5
	.quad	-9102865688819295809            # 0x81ac1fe293d599bf
	.quad	-4111420493003729615            # 0xc6f14cd848405531
	.quad	-6766896092596731857            # 0xa21727db38cb002f
	.quad	-5139275616254662019            # 0xb8ada00e5a506a7d
	.quad	-3846934097318526917            # 0xca9cf1d206fdc03b
	.quad	-6424094520318327523            # 0xa6d90811f0e4851d
	.quad	-196981603220770742             # 0xfd442e4688bd304a
	.quad	-8030118150397909404            # 0x908f4a166d1da664
	.quad	-7040642529654063570            # 0x9e4a9cec15763e2e
	.quad	-7324666853212387329            # 0x9a598e4e043287ff
	.quad	-4189117143640191558            # 0xc5dd44271ad3cdba
	.quad	4679224488766679550             # 0x40eff1e1853f29fe
	.quad	-624710411122851544             # 0xf7549530e188c128
	.quad	-3374341425896426371            # 0xd12bee59e68ef47d
	.quad	-7307973034592864071            # 0x9a94dd3e8cf578b9
	.quad	-9026492418826348337            # 0x82bb74f8301958cf
	.quad	-4523280274813692185            # 0xc13a148e3032d6e7
	.quad	-2059743486678159614            # 0xe36a52363c1faf02
	.quad	-1042414325089727327            # 0xf18899b1bc3f8ca1
	.quad	-2574679358347699518            # 0xdc44e6c3cb279ac2
	.quad	-7569037980822161435            # 0x96f5600f15a7b7e5
	.quad	3002511419460075706             # 0x29ab103a5ef8c0ba
	.quad	-4849611457600313890            # 0xbcb2b812db11a5de
	.quad	8364825292752482536             # 0x7415d448f6b6f0e8
	.quad	-1450328303573004458            # 0xebdf661791d60f56
	.quad	1232659579085827362             # 0x111b495b3464ad22
	.quad	-7823984217374209643            # 0x936b9fcebb25c995
	.quad	-3841273781498745803            # 0xcab10dd900beec35
	.quad	-5168294253290374149            # 0xb84687c269ef3bfb
	.quad	4421779809981343555             # 0x3d5d514f40eea743
	.quad	-1848681798185579782            # 0xe65829b3046b0afa
	.quad	915538744049291539              # 0xcb4a5a3112a5113
	.quad	-8072955151507069220            # 0x8ff71a0fe2c2e6dc
	.quad	5183897733458195116             # 0x47f0e785eaba72ac
	.quad	-5479507920956448621            # 0xb3f4e093db73a093
	.quad	6479872166822743895             # 0x59ed216765690f57
	.quad	-2237698882768172872            # 0xe0f218b8d25088b8
	.quad	3488154190101041965             # 0x306869c13ec3532d
	.quad	-8316090829371189901            # 0x8c974f7383725573
	.quad	2180096368813151228             # 0x1e414218c73a13fc
	.quad	-5783427518286599473            # 0xafbd2350644eeacf
	.quad	-1886565557410948869            # 0xe5d1929ef90898fb
	.quad	-2617598379430861437            # 0xdbac6c247d62a583
	.quad	-2358206946763686086            # 0xdf45f746b74abf3a
	.quad	-8553528014785370254            # 0x894bc396ce5da772
	.quad	7749492695127472004             # 0x6b8bba8c328eb784
	.quad	-6080224000054324913            # 0xab9eb47c81f5114f
	.quad	463493832054564197              # 0x66ea92f3f326565
	.quad	-2988593981640518238            # 0xd686619ba27255a2
	.quad	-4032318728359182658            # 0xc80a537b0efefebe
	.quad	-8785400266166405755            # 0x8613fd0145877585
	.quad	-4826042214438183113            # 0xbd06742ce95f5f37
	.quad	-6370064314280619289            # 0xa798fc4196e952e7
	.quad	3190819268807046917             # 0x2c48113823b73705
	.quad	-3350894374423386208            # 0xd17f3b51fca3a7a0
	.quad	-623161932418579258             # 0xf75a15862ca504c6
	.quad	-9011838011655698236            # 0x82ef85133de648c4
	.quad	-7307005235402693892            # 0x9a984d73dbe722fc
	.quad	-6653111496142234891            # 0xa3ab66580d5fdaf5
	.quad	-4522070525825979461            # 0xc13e60d0d2e0ebbb
	.quad	-3704703351750405709            # 0xcc963fee10b7d1b3
	.quad	3570783879572301481             # 0x318df905079926a9
	.quad	-19193171260619233              # 0xffbbcfe994e5c61f
	.quad	-148206168962011053             # 0xfdf17746497f7053
	.quad	-6929524759678968877            # 0x9fd561f1fd0f9bd3
	.quad	-92628855601256908              # 0xfeb6ea8bedefa634
	.quad	-4050219931171323192            # 0xc7caba6e7c5382c8
	.quad	-115786069501571135             # 0xfe64a52ee96b8fc1
	.quad	-451088895536766085             # 0xf9bd690a1b68637b
	.quad	4466953431550423985             # 0x3dfdce7aa3c673b1
	.quad	-7199459587351560659            # 0x9c1661a651213e2d
	.quad	486002885505321039              # 0x6bea10ca65c084f
	.quad	-4387638465762062920            # 0xc31bfa0fe5698db8
	.quad	5219189625309039203             # 0x486e494fcff30a63
	.quad	-872862063775190746             # 0xf3e2f893dec3f126
	.quad	6523987031636299003             # 0x5a89dba3c3efccfb
	.quad	-7463067817500576073            # 0x986ddb5c6b3a76b7
	.quad	-534194123654701027             # 0xf89629465a75e01d
	.quad	-4717148753448332187            # 0xbe89523386091465
	.quad	-667742654568376284             # 0xf6bbb397f1135824
	.quad	-1284749923383027329            # 0xee2ba6c0678b597f
	.quad	8388693718644305453             # 0x746aa07ded582e2d
	.quad	-7720497729755473937            # 0x94db483840b717ef
	.quad	-6286281471915778851            # 0xa8c2a44eb4571cdd
	.quad	-5038936143766954517            # 0xba121a4650e4ddeb
	.quad	-7857851839894723564            # 0x92f34d62616ce414
	.quad	-1686984161281305242            # 0xe896a0d7e51e1566
	.quad	8624429273841147160             # 0x77b020baf9c81d18
	.quad	-7971894128441897632            # 0x915e2486ef32cd60
	.quad	778582277723329071              # 0xace1474dc1d122f
	.quad	-5353181642124984136            # 0xb5b5ada8aaff80b8
	.quad	973227847154161339              # 0xd819992132456bb
	.quad	-2079791034228842266            # 0xe3231912d5bf60e6
	.quad	1216534808942701674             # 0x10e1fff697ed6c6a
	.quad	-8217398424034108273            # 0x8df5efabc5979c8f
	.quad	-3851351762838199358            # 0xca8d3ffa1ef463c2
	.quad	-5660062011615247437            # 0xb1736b96b6fd83b3
	.quad	-4814189703547749197            # 0xbd308ff8a6b17cb3
	.quad	-2463391496091671392            # 0xddd0467c64bce4a0
	.quad	-6017737129434686497            # 0xac7cb3f6d05ddbdf
	.quad	-8457148712698376476            # 0x8aa22c0dbef60ee4
	.quad	7768129340171790700             # 0x6bcdf07a423aa96c
	.quad	-5959749872445582691            # 0xad4ab7112eb3929d
	.quad	-8736582398494813241            # 0x86c16c98d2c953c7
	.quad	-2838001322129590460            # 0xd89d64d57a607744
	.quad	-1697355961263740744            # 0xe871c7bf077ba8b8
	.quad	-8691279853972075893            # 0x87625f056c7c4a8b
	.quad	1244995533423855987             # 0x11471cd764ad4973
	.quad	-6252413799037706963            # 0xa93af6c6c79b5d2d
	.quad	-3055441601647567920            # 0xd598e40d3dd89bd0
	.quad	-3203831230369745799            # 0xd389b47879823479
	.quad	5404070034795315908             # 0x4aff1d108d4ec2c4
	.quad	-8919923546622172981            # 0x843610cb4bf160cb
	.quad	-3539985255894009413            # 0xcedf722a585139bb
	.quad	-6538218414850328322            # 0xa54394fe1eedb8fe
	.quad	-4424981569867511767            # 0xc2974eb4ee658829
	.quad	-3561087000135522498            # 0xce947a3da6a9273e
	.quad	8303831092947774003             # 0x733d226229feea33
	.quad	-9143208402725783417            # 0x811ccc668829b887
	.quad	578208414664970848              # 0x806357d5a3f5260
	.quad	-6817324484979841368            # 0xa163ff802a3426a8
	.quad	-3888925500096174344            # 0xca07c2dcb0cf26f8
	.quad	-3909969587797413806            # 0xc9bcff6034c13052
	.quad	-249470856692830026             # 0xfc89b393dd02f0b6
	.quad	-275775966319379353             # 0xfc2c3f3841f17c67
	.quad	-4923524589293425437            # 0xbbac2078d443ace3
	.quad	-7089889006590693952            # 0x9d9ba7832936edc0
	.quad	-3077202868308390898            # 0xd54b944b84aa4c0e
	.quad	-4250675239810979535            # 0xc5029163f384a931
	.quad	765182433041899282              # 0xa9e795e65d4df12
	.quad	-701658031336336515             # 0xf64335bcf065d37d
	.quad	5568164059729762006             # 0x4d4617b5ff4a16d6
	.quad	-7356065297226292178            # 0x99ea0196163fa42e
	.quad	5785945546544795206             # 0x504bced1bf8e4e46
	.quad	-4583395603105477319            # 0xc06481fb9bcf8d39
	.quad	-1990940103673781801            # 0xe45ec2862f71e1d7
	.quad	-1117558485454458744            # 0xf07da27a82c37088
	.quad	6734696907262548557             # 0x5d767327bb4e5a4d
	.quad	-7616003081050118571            # 0x964e858c91ba2655
	.quad	4209185567039092848             # 0x3a6a07f8d510f870
	.quad	-4908317832885260310            # 0xbbe226efb628afea
	.quad	-8573576096483297652            # 0x890489f70a55368c
	.quad	-1523711272679187483            # 0xeadab0aba3b2dbe5
	.quad	3118087934678041647             # 0x2b45ac74ccea842f
	.quad	-7869848573065574033            # 0x92c8ae6b464fc96f
	.quad	4254647968387469982             # 0x3b0b8bc90012929e
	.quad	-5225624697904579637            # 0xb77ada0617e3bbcb
	.quad	706623942056949573              # 0x9ce6ebb40173745
	.quad	-1920344853953336643            # 0xe55990879ddcaabd
	.quad	-3728406090856200938            # 0xcc420a6a101d0516
	.quad	-8117744561361917258            # 0x8f57fa54c2a9eab6
	.quad	-6941939825212513490            # 0x9fa946824a12232e
	.quad	-5535494683275008668            # 0xb32df8e9f3546564
	.quad	5157633273766521850             # 0x47939822dc96abfa
	.quad	-2307682335666372931            # 0xdff9772470297ebd
	.quad	6447041592208152312             # 0x59787e2b93bc56f8
	.quad	-8359830487432564938            # 0x8bfbea76c619ef36
	.quad	6335244004343789147             # 0x57eb4edb3c55b65b
	.quad	-5838102090863318269            # 0xaefae51477a06b03
	.quad	-1304317031425039374            # 0xede622920b6b23f2
	.quad	-2685941595151759932            # 0xdab99e59958885c4
	.quad	-1630396289281299218            # 0xe95fab368e45ecee
	.quad	-8596242524610931813            # 0x88b402f7fd75539b
	.quad	1286845328412881941             # 0x11dbcb0218ebb415
	.quad	-6133617137336276863            # 0xaae103b5fcd2a881
	.quad	-3003129357911285478            # 0xd652bdc29f26a11a
	.quad	-3055335403242958174            # 0xd59944a37c0752a2
	.quad	5469460339465668960             # 0x4be76d3346f04960
	.quad	-8827113654667930715            # 0x857fcae62d8493a5
	.quad	8030098730593431004             # 0x6f70a4400c562ddc
	.quad	-6422206049907525490            # 0xa6dfbd9fb8e5b88e
	.quad	-3797434642040374957            # 0xcb4ccd500f6bb953
	.quad	-3416071543957018958            # 0xd097ad07a71f26b2
	.quad	9088264752731695016             # 0x7e2000a41346a7a8
	.quad	-9052573742614218705            # 0x825ecc24c873782f
	.quad	-8154892584824854327            # 0x8ed400668c0c28c9
	.quad	-6704031159840385477            # 0xa2f67f2dfa90563b
	.quad	8253128342678483707             # 0x728900802f0f32fb
	.quad	-3768352931373093942            # 0xcbb41ef979346bca
	.quad	5704724409920716730             # 0x4f2b40a03ad2ffba
	.quad	-98755145788979524              # 0xfea126b7d78186bc
	.quad	-2092466524453879895            # 0xe2f610c84987bfa9
	.quad	-6979250993759194058            # 0x9f24b832e6b0f436
	.quad	998051431430019018              # 0xdd9ca7d2df4d7ca
	.quad	-4112377723771604669            # 0xc6ede63fa05d3143
	.quad	-7975807747567252036            # 0x91503d1c79720dbc
	.quad	-528786136287117932             # 0xf8a95fcf88747d94
	.quad	8476984389250486571             # 0x75a44c6397ce912b
	.quad	-7248020362820530564            # 0x9b69dbe1b548ce7c
	.quad	-3925256793573221701            # 0xc986afbe3ee11abb
	.quad	-4448339435098275301            # 0xc24452da229b021b
	.quad	-294884973539139223             # 0xfbe85badce996169
	.quad	-948738275445456222             # 0xf2d56790ab41c2a2
	.quad	-368606216923924028             # 0xfae27299423fb9c4
	.quad	-7510490449794491995            # 0x97c560ba6b0919a5
	.quad	-2536221894791146469            # 0xdccd879fc967d41b
	.quad	-4776427043815727089            # 0xbdb6b8e905cb600f
	.quad	6053094668365842721             # 0x5400e987bbc1c921
	.quad	-1358847786342270957            # 0xed246723473e3813
	.quad	2954682317029915497             # 0x290123e9aab23b69
	.quad	-7766808894105001205            # 0x9436c0760c86e30b
	.quad	-459166561069996766             # 0xf9a0b6720aaf6522
	.quad	-5096825099203863602            # 0xb94470938fa89bce
	.quad	-573958201337495958             # 0xf808e40e8d5b3e6a
	.quad	-1759345355577441598            # 0xe7958cb87392c2c2
	.quad	-5329133770099257851            # 0xb60b1d1230b20e05
	.quad	-8017119874876982855            # 0x90bd77f3483bb9b9
	.quad	-5636551615525730109            # 0xb1c6f22b5e6f48c3
	.quad	-5409713825168840664            # 0xb4ecd5f01a4aa828
	.quad	2177682517447613172             # 0x1e38aeb6360b1af4
	.quad	-2150456263033662926            # 0xe2280b6c20dd5232
	.quad	2722103146809516465             # 0x25c6da63c38de1b1
	.quad	-8261564192037121185            # 0x8d590723948a535f
	.quad	6313000485183335695             # 0x579c487e5a38ad0f
	.quad	-5715269221619013577            # 0xb0af48ec79ace837
	.quad	3279564588051781714             # 0x2d835a9df0c6d852
	.quad	-2532400508596379068            # 0xdcdb1b2798182244
	.quad	-512230283362660762             # 0xf8e431456cf88e66
	.quad	-8500279345513818773            # 0x8a08f0f8bf0f156b
	.quad	1985699082112030976             # 0x1b8e9ecb641b5900
	.quad	-6013663163464885563            # 0xac8b2d36eed2dac5
	.quad	-2129562165787349184            # 0xe272467e3d222f40
	.quad	-2905392935903719049            # 0xd7adf884aa879177
	.quad	6561419329620589328             # 0x5b0ed81dcc6abb10
	.quad	-8733399612580906262            # 0x86ccbb52ea94baea
	.quad	-7428327965055601430            # 0x98e947129fc2b4ea
	.quad	-6305063497298744923            # 0xa87fea27a539e9a5
	.quad	4549648098962661925             # 0x3f2398d747b36225
	.quad	-3269643353196043250            # 0xd29fe4b18e88640e
	.quad	-8147997931578836306            # 0x8eec7f0d19a03aae
	.quad	-8961056123388608887            # 0x83a3eeeef9153e89
	.quad	1825030320404309165             # 0x1953cf68300424ad
	.quad	-6589634135808373205            # 0xa48ceaaab75a8e2b
	.quad	6892973918932774360             # 0x5fa8c3423c052dd8
	.quad	-3625356651333078602            # 0xcdb02555653131b6
	.quad	4004531380238580046             # 0x3792f412cb06794e
	.quad	-9183376934724255983            # 0x808e17555f3ebf11
	.quad	-2108853905778275375            # 0xe2bbd88bbee40bd1
	.quad	-6867535149977932074            # 0xa0b19d2ab70e6ed6
	.quad	6587304654631931589             # 0x5b6aceaeae9d0ec5
	.quad	-3972732919045027189            # 0xc8de047564d20a8b
	.quad	-989241218564861322             # 0xf245825a5a445276
	.quad	-354230130378896082             # 0xfb158592be068d2e
	.quad	-1236551523206076653            # 0xeed6e2f0f0d56713
	.quad	-7138922859127891907            # 0x9ced737bb6c4183d
	.quad	6144684325637283948             # 0x55464dd69685606c
	.quad	-4311967555482476980            # 0xc428d05aa4751e4c
	.quad	-6154202648235558777            # 0xaa97e14c3c26b887
	.quad	-778273425925708321             # 0xf53304714d9265df
	.quad	-3081067291867060567            # 0xd53dd99f4b3066a9
	.quad	-7403949918844649557            # 0x993fe2c6d07b7fab
	.quad	-1925667057416912854            # 0xe546a8038efe402a
	.quad	-4643251380128424042            # 0xbf8fdb78849a5f96
	.quad	-2407083821771141068            # 0xde98520472bdd034
	.quad	-1192378206733142148            # 0xef73d256a5c0f77c
	.quad	-7620540795641314239            # 0x963e66858f6d4441
	.quad	-7662765406849295699            # 0x95a8637627989aad
	.quad	-2456994988062127447            # 0xdde7001379a44aa9
	.quad	-4966770740134231719            # 0xbb127c53b17ec159
	.quad	6152128301777116499             # 0x5560c018580d5d53
	.quad	-1596777406740401745            # 0xe9d71b689dde71af
	.quad	-6144897678060768089            # 0xaab8f01e6e10b4a7
	.quad	-7915514906853832947            # 0x9226712162ab070d
	.quad	-3840561048787980055            # 0xcab3961304ca70e9
	.quad	-5282707615139903279            # 0xb6b00d69bb55c8d1
	.quad	4422670725869800739             # 0x3d607b97c5fd0d23
	.quad	-1991698500497491195            # 0xe45c10c42a2b3b05
	.quad	-8306719647944912789            # 0x8cb89a7db77c506b
	.quad	-8162340590452013853            # 0x8eb98a7a9a5b04e3
	.quad	8643358275316593219             # 0x77f3608e92adb243
	.quad	-5591239719637629412            # 0xb267ed1940f1c61c
	.quad	6192511825718353620             # 0x55f038b237591ed4
	.quad	-2377363631119648861            # 0xdf01e85f912e37a3
	.quad	7740639782147942025             # 0x6b6c46dec52f6689
	.quad	-8403381297090862394            # 0x8b61313bbabce2c6
	.quad	2532056854628769814             # 0x2323ac4b3b3da016
	.quad	-5892540602936190089            # 0xae397d8aa96c1b77
	.quad	-6058300968568813541            # 0xabec975e0a0d081b
	.quad	-2753989735242849707            # 0xd9c7dced53c72255
	.quad	-7572876210711016926            # 0x96e7bd358c904a22
	.quad	-8638772612167862923            # 0x881cea14545c7575
	.quad	9102010423587778133             # 0x7e50d64177da2e55
	.quad	-6186779746782440750            # 0xaa242499697392d2
	.quad	-2457545025797441046            # 0xdde50bd1d5d0b9ea
	.quad	-3121788665050663033            # 0xd4ad2dbfc3d07787
	.quad	-7683617300674189211            # 0x955e4ec64b44e865
	.quad	-8868646943297746252            # 0x84ec3c97da624ab4
	.quad	-4802260812921368257            # 0xbd5af13bef0b113f
	.quad	-6474122660694794911            # 0xa6274bbdd0fadd61
	.quad	-1391139997724322417            # 0xecb1ad8aeacdd58f
	.quad	-3480967307441105734            # 0xcfb11ead453994ba
	.quad	7484447039699372787             # 0x67de18eda5814af3
	.quad	-9093133594791772940            # 0x81ceb32c4b43fcf4
	.quad	-9157278655470055720            # 0x80eacf948770ced8
	.quad	-6754730975062328271            # 0xa2425ff75e14fc31
	.quad	-6834912300910181746            # 0xa1258379a94d028e
	.quad	-3831727700400522434            # 0xcad2f7f5359a3b3e
	.quad	679731660717048625              # 0x96ee45813a04331
	.quad	-177973607073265139             # 0xfd87b5f28300ca0d
	.quad	-8373707460958465027            # 0x8bca9d6e188853fd
	.quad	-7028762532061872568            # 0x9e74d1b791e07e48
	.quad	8601490892183123070             # 0x775ea264cf55347e
	.quad	-4174267146649952806            # 0xc612062576589dda
	.quad	-7694880458480647778            # 0x95364afe032a819e
	.quad	-606147914885053103             # 0xf79687aed3eec551
	.quad	4216457482181353989             # 0x3a83ddbd83f52205
	.quad	-7296371474444240046            # 0x9abe14cd44753b52
	.quad	-4282243101277735613            # 0xc4926a9672793543
	.quad	-4508778324627912153            # 0xc16d9a0095928a27
	.quad	8482254178684994196             # 0x75b7053c0f178294
	.quad	-1024286887357502287            # 0xf1c90080baf72cb1
	.quad	5991131704928854841             # 0x5324c68b12dd6339
	.quad	-7557708332239520786            # 0x971da05074da7bee
	.quad	-3173071712060547580            # 0xd3f6fc16ebca5e04
	.quad	-4835449396872013078            # 0xbce5086492111aea
	.quad	-8578025658503072379            # 0x88f4bb1ca6bcf585
	.quad	-1432625727662628443            # 0xec1e4a7db69561a5
	.quad	3112525982153323238             # 0x2b31e9e3d06c32e6
	.quad	-7812920107430224633            # 0x9392ee8e921d5d07
	.quad	4251171748059520976             # 0x3aff322e62439fd0
	.quad	-5154464115860392887            # 0xb877aa3236a4b449
	.quad	702278666647013315              # 0x9befeb9fad487c3
	.quad	-1831394126398103205            # 0xe69594bec44de15b
	.quad	5489534351736154548             # 0x4c2ebe687989a9b4
	.quad	-8062150356639896359            # 0x901d7cf73ab0acd9
	.quad	1125115960621402641             # 0xf9d37014bf60a11
	.quad	-5466001927372482545            # 0xb424dc35095cd80f
	.quad	6018080969204141205             # 0x538484c19ef38c95
	.quad	-2220816390788215277            # 0xe12e13424bb40e13
	.quad	2910915193077788602             # 0x2865a5f206b06fba
	.quad	-8305539271883716405            # 0x8cbccc096f5088cb
	.quad	-486521013540076076             # 0xf93f87b7442e45d4
	.quad	-5770238071427257602            # 0xafebff0bcb24aafe
	.quad	-608151266925095095             # 0xf78f69a51539d749
	.quad	-2601111570856684098            # 0xdbe6fecebdedd5be
	.quad	-5371875102083756772            # 0xb573440e5a884d1c
	.quad	-8543223759426509417            # 0x89705f4136b4a597
	.quad	3560107088838733873             # 0x31680a88f8953031
	.quad	-6067343680855748868            # 0xabcc77118461cefc
	.quad	-161552157378970562             # 0xfdc20d2b36ba7c3e
	.quad	-2972493582642298180            # 0xd6bf94d5e57a42bc
	.quad	4409745821703674701             # 0x3d32907604691b4d
	.quad	-8775337516792518219            # 0x8637bd05af6c69b5
	.quad	-6467280898289979120            # 0xa63f9a49c2c1b110
	.quad	-6357485877563259869            # 0xa7c5ac471b478423
	.quad	1139270913992301908             # 0xfcf80dc33721d54
	.quad	-3335171328526686933            # 0xd1b71758e219652b
	.quad	-3187597375937010519            # 0xd3c36113404ea4a9
	.quad	-9002011107970261189            # 0x83126e978d4fdf3b
	.quad	7231123676894144234             # 0x645a1cac083126ea
	.quad	-6640827866535438582            # 0xa3d70a3d70a3d70a
	.quad	4427218577690292388             # 0x3d70a3d70a3d70a4
	.quad	-3689348814741910324            # 0xcccccccccccccccc
	.quad	-3689348814741910323            # 0xcccccccccccccccd
	.quad	-9223372036854775808            # 0x8000000000000000
	.quad	0                               # 0x0
	.quad	-6917529027641081856            # 0xa000000000000000
	.quad	0                               # 0x0
	.quad	-4035225266123964416            # 0xc800000000000000
	.quad	0                               # 0x0
	.quad	-432345564227567616             # 0xfa00000000000000
	.quad	0                               # 0x0
	.quad	-7187745005283311616            # 0x9c40000000000000
	.quad	0                               # 0x0
	.quad	-4372995238176751616            # 0xc350000000000000
	.quad	0                               # 0x0
	.quad	-854558029293551616             # 0xf424000000000000
	.quad	0                               # 0x0
	.quad	-7451627795949551616            # 0x9896800000000000
	.quad	0                               # 0x0
	.quad	-4702848726509551616            # 0xbebc200000000000
	.quad	0                               # 0x0
	.quad	-1266874889709551616            # 0xee6b280000000000
	.quad	0                               # 0x0
	.quad	-7709325833709551616            # 0x9502f90000000000
	.quad	0                               # 0x0
	.quad	-5024971273709551616            # 0xba43b74000000000
	.quad	0                               # 0x0
	.quad	-1669528073709551616            # 0xe8d4a51000000000
	.quad	0                               # 0x0
	.quad	-7960984073709551616            # 0x9184e72a00000000
	.quad	0                               # 0x0
	.quad	-5339544073709551616            # 0xb5e620f480000000
	.quad	0                               # 0x0
	.quad	-2062744073709551616            # 0xe35fa931a0000000
	.quad	0                               # 0x0
	.quad	-8206744073709551616            # 0x8e1bc9bf04000000
	.quad	0                               # 0x0
	.quad	-5646744073709551616            # 0xb1a2bc2ec5000000
	.quad	0                               # 0x0
	.quad	-2446744073709551616            # 0xde0b6b3a76400000
	.quad	0                               # 0x0
	.quad	-8446744073709551616            # 0x8ac7230489e80000
	.quad	0                               # 0x0
	.quad	-5946744073709551616            # 0xad78ebc5ac620000
	.quad	0                               # 0x0
	.quad	-2821744073709551616            # 0xd8d726b7177a8000
	.quad	0                               # 0x0
	.quad	-8681119073709551616            # 0x878678326eac9000
	.quad	0                               # 0x0
	.quad	-6239712823709551616            # 0xa968163f0a57b400
	.quad	0                               # 0x0
	.quad	-3187955011209551616            # 0xd3c21bcecceda100
	.quad	0                               # 0x0
	.quad	-8910000909647051616            # 0x84595161401484a0
	.quad	0                               # 0x0
	.quad	-6525815118631426616            # 0xa56fa5b99019a5c8
	.quad	0                               # 0x0
	.quad	-3545582879861895366            # 0xcecb8f27f4200f3a
	.quad	0                               # 0x0
	.quad	-9133518327554766460            # 0x813f3978f8940984
	.quad	4611686018427387904             # 0x4000000000000000
	.quad	-6805211891016070171            # 0xa18f07d736b90be5
	.quad	5764607523034234880             # 0x5000000000000000
	.quad	-3894828845342699810            # 0xc9f2c9cd04674ede
	.quad	-6629298651489370112            # 0xa400000000000000
	.quad	-256850038250986858             # 0xfc6f7c4045812296
	.quad	5548434740920451072             # 0x4d00000000000000
	.quad	-7078060301547948643            # 0x9dc5ada82b70b59d
	.quad	-1143914305352105984            # 0xf020000000000000
	.quad	-4235889358507547899            # 0xc5371912364ce305
	.quad	7793479155164643328             # 0x6c28000000000000
	.quad	-683175679707046970             # 0xf684df56c3e01bc6
	.quad	-4093209111326359552            # 0xc732000000000000
	.quad	-7344513827457986212            # 0x9a130b963a6c115c
	.quad	4359273333062107136             # 0x3c7f400000000000
	.quad	-4568956265895094861            # 0xc097ce7bc90715b3
	.quad	5449091666327633920             # 0x4b9f100000000000
	.quad	-1099509313941480672            # 0xf0bdc21abb48db20
	.quad	2199678564482154496             # 0x1e86d40000000000
	.quad	-7604722348854507276            # 0x96769950b50d88f4
	.quad	1374799102801346560             # 0x1314448000000000
	.quad	-4894216917640746191            # 0xbc143fa4e250eb31
	.quad	1718498878501683200             # 0x17d955a000000000
	.quad	-1506085128623544835            # 0xeb194f8e1ae525fd
	.quad	6759809616554491904             # 0x5dcfab0800000000
	.quad	-7858832233030797378            # 0x92efd1b8d0cf37be
	.quad	6530724019560251392             # 0x5aa1cae500000000
	.quad	-5211854272861108819            # 0xb7abc627050305ad
	.quad	-1059967012404461568            # 0xf14a3d9e40000000
	.quad	-1903131822648998119            # 0xe596b7b0c643c719
	.quad	7898413271349198848             # 0x6d9ccd05d0000000
	.quad	-8106986416796705681            # 0x8f7e32ce7bea5c6f
	.quad	-1981020733047832576            # 0xe4820023a2000000
	.quad	-5522047002568494197            # 0xb35dbf821ae4f38b
	.quad	-2476275916309790720            # 0xdda2802c8a800000
	.quad	-2290872734783229842            # 0xe0352f62a19e306e
	.quad	-3095344895387238400            # 0xd50b2037ad200000
	.quad	-8349324486880600507            # 0x8c213d9da502de45
	.quad	4982938468024057856             # 0x4526f422cc340000
	.quad	-5824969590173362730            # 0xaf298d050e4395d6
	.quad	-7606384970252091392            # 0x9670b12b7f410000
	.quad	-2669525969289315508            # 0xdaf3f04651d47b4c
	.quad	4327076842467049472             # 0x3c0cdd765f114000
	.quad	-8585982758446904049            # 0x88d8762bf324cd0f
	.quad	-6518949010312869888            # 0xa5880a69fb6ac800
	.quad	-6120792429631242157            # 0xab0e93b6efee0053
	.quad	-8148686262891087360            # 0x8eea0d047a457a00
	.quad	-3039304518611664792            # 0xd5d238a4abe98068
	.quad	8260886245095692416             # 0x72a4904598d6d880
	.quad	-8817094351773372351            # 0x85a36366eb71f041
	.quad	5163053903184807760             # 0x47a6da2b7f864750
	.quad	-6409681921289327535            # 0xa70c3c40a64e6c51
	.quad	-7381240676301154012            # 0x999090b65f67d924
	.quad	-3400416383184271515            # 0xd0cf4b50cfe20765
	.quad	-3178808521666707               # 0xfff4b4e3f741cf6d
	.quad	-9042789267131251553            # 0x82818f1281ed449f
	.quad	-4613672773753429595            # 0xbff8f10e7a8921a5
	.quad	-6691800565486676537            # 0xa321f2d7226895c7
	.quad	-5767090967191786994            # 0xaff72d52192b6a0e
	.quad	-3753064688430957767            # 0xcbea6f8ceb02bb39
	.quad	-7208863708989733743            # 0x9bf4f8a69f764491
	.quad	-79644842111309304              # 0xfee50b7025c36a08
	.quad	212292400617608629              # 0x2f236d04753d5b5
	.quad	-6967307053960650171            # 0x9f4f2726179a2245
	.quad	132682750386005393              # 0x1d762422c946591
	.quad	-4097447799023424810            # 0xc722f0ef9d80aad6
	.quad	4777539456409894646             # 0x424d3ad2b7b97ef6
	.quad	-510123730351893109             # 0xf8ebad2b84e0d58b
	.quad	-3251447716342407501            # 0xd2e0898765a7deb3
	.quad	-7236356359111015049            # 0x9b934c3b330c8577
	.quad	7191217214140771120             # 0x63cc55f49f88eb30
	.quad	-4433759430461380907            # 0xc2781f49ffcfa6d5
	.quad	4377335499248575996             # 0x3cbf6b71c76b25fc
	.quad	-930513269649338230             # 0xf316271c7fc3908a
	.quad	-8363388681221443717            # 0x8bef464e3945ef7b
	.quad	-7499099821171918250            # 0x97edd871cfda3a56
	.quad	-7532960934977096275            # 0x97758bf0e3cbb5ad
	.quad	-4762188758037509908            # 0xbde94e8e43d0c8ec
	.quad	4418856886560793368             # 0x3d52eeed1cbea318
	.quad	-1341049929119499481            # 0xed63a231d4c4fb27
	.quad	5523571108200991710             # 0x4ca7aaa863ee4bde
	.quad	-7755685233340769032            # 0x945e455f24fb1cf8
	.quad	-8076983103442849941            # 0x8fe8caa93e74ef6b
	.quad	-5082920523248573386            # 0xb975d6b6ee39e436
	.quad	-5484542860876174523            # 0xb3e2fd538e122b45
	.quad	-1741964635633328828            # 0xe7d34c64a9c85d44
	.quad	6979379479186945559             # 0x60dbbca87196b617
	.quad	-8006256924911912374            # 0x90e40fbeea1d3a4a
	.quad	-4861259862362934834            # 0xbc8955e946fe31ce
	.quad	-5396135137712502563            # 0xb51d13aea4a488dd
	.quad	7758483227328495170             # 0x6babab6398bdbe42
	.quad	-2133482903713240300            # 0xe264589a4dcdab14
	.quad	-4136954021121544750            # 0xc696963c7eed2dd2
	.quad	-8250955842461857044            # 0x8d7eb76070a08aec
	.quad	-279753253987271517             # 0xfc1e1de5cf543ca3
	.quad	-5702008784649933400            # 0xb0de65388cc8ada8
	.quad	4261994450943298508             # 0x3b25a55f43294bcc
	.quad	-2515824962385028846            # 0xdd15fe86affad912
	.quad	5327493063679123135             # 0x49ef0eb713f39ebf
	.quad	-8489919629131724885            # 0x8a2dbf142dfcc7ab
	.quad	7941369183226839864             # 0x6e3569326c784338
	.quad	-6000713517987268202            # 0xacb92ed9397bf996
	.quad	5315025460606161925             # 0x49c2c37f07965405
	.quad	-2889205879056697349            # 0xd7e77a8f87daf7fb
	.quad	-2579590211097073401            # 0xdc33745ec97be907
	.quad	-8723282702051517699            # 0x86f0ac99b4e8dafd
	.quad	7611128154919104932             # 0x69a028bb3ded71a4
	.quad	-6292417359137009220            # 0xa8acd7c0222311bc
	.quad	-4321147861633282547            # 0xc40832ea0d68ce0d
	.quad	-3253835680493873621            # 0xd2d80db02aabd62b
	.quad	-789748808614215279             # 0xf50a3fa490c30191
	.quad	-8951176327949752869            # 0x83c7088e1aab65db
	.quad	8729779031470891259             # 0x792667c6da79e0fb
	.quad	-6577284391509803182            # 0xa4b8cab1a1563f52
	.quad	6300537770911226169             # 0x577001b891185939
	.quad	-3609919470959866074            # 0xcde6fd5e09abcf26
	.quad	-1347699823215743097            # 0xed4c0226b55e6f87
	.quad	-9173728696990998152            # 0x80b05e5ac60b6178
	.quad	6075216638131242421             # 0x544f8158315b05b5
	.quad	-6855474852811359786            # 0xa0dc75f1778e39d6
	.quad	7594020797664053026             # 0x696361ae3db1c722
	.quad	-3957657547586811828            # 0xc913936dd571c84c
	.quad	269153960225290474              # 0x3bc3a19cd1e38ea
	.quad	-335385916056126881             # 0xfb5878494ace3a5f
	.quad	336442450281613092              # 0x4ab48a04065c724
	.quad	-7127145225176161157            # 0x9d174b2dcec0e47b
	.quad	7127805559067090039             # 0x62eb0d64283f9c77
	.quad	-4297245513042813542            # 0xc45d1df942711d9a
	.quad	4298070930406474645             # 0x3ba5d0bd324f8395
	.quad	-759870872876129024             # 0xf5746577930d6500
	.quad	-3850783373846682502            # 0xca8f44ec7ee3647a
	.quad	-7392448323188662496            # 0x9968bf6abbe85f20
	.quad	9122475437414293196             # 0x7e998b13cf4e1ecc
	.quad	-4628874385558440216            # 0xbfc2ef456ae276e8
	.quad	-7043649776941685121            # 0x9e3fedd8c321a67f
	.quad	-1174406963520662366            # 0xefb3ab16c59b14a2
	.quad	-4192876202749718497            # 0xc5cfe94ef3ea101f
	.quad	-7651533379841495835            # 0x95d04aee3b80ece5
	.quad	-4926390635932268013            # 0xbba1f1d158724a13
	.quad	-4952730706374481889            # 0xbb445da9ca61281f
	.quad	3065383741939440792             # 0x2a8a6e45ae8edc98
	.quad	-1579227364540714458            # 0xea1575143cf97226
	.quad	-779956341003086914             # 0xf52d09d71a3293be
	.quad	-7904546130479028392            # 0x924d692ca61be758
	.quad	6430056314514152535             # 0x593c2626705f9c57
	.quad	-5268996644671397586            # 0xb6e0c377cfa2e12e
	.quad	8037570393142690669             # 0x6f8b2fb00c77836d
	.quad	-1974559787411859078            # 0xe498f455c38b997a
	.quad	823590954573587528              # 0xb6dfb9c0f956448
	.quad	-8151628894773493780            # 0x8edf98b59a373fec
	.quad	5126430365035880109             # 0x4724bd4189bd5ead
	.quad	-5577850100039479321            # 0xb2977ee300c50fe7
	.quad	6408037956294850136             # 0x58edec91ec2cb658
	.quad	-2360626606621961247            # 0xdf3d5e9bc0f653e1
	.quad	3398361426941174766             # 0x2f2967b66737e3ee
	.quad	-8392920656779807636            # 0x8b865b215899f46c
	.quad	-4793553135802847627            # 0xbd79e0d20082ee75
	.quad	-5879464802547371641            # 0xae67f1e9aec07187
	.quad	-1380255401326171630            # 0xecd8590680a3aa12
	.quad	-2737644984756826647            # 0xda01ee641a708de9
	.quad	-1725319251657714538            # 0xe80e6f4820cc9496
	.quad	-8628557143114098510            # 0x884134fe908658b2
	.quad	3533361486141316318             # 0x3109058d147fdcde
	.quad	-6174010410465235234            # 0xaa51823e34a7eede
	.quad	-4806670179178130410            # 0xbd4b46f0599fd416
	.quad	-3105826994654156138            # 0xd4e5e2cdc1d1ea96
	.quad	7826720331309500699             # 0x6c9e18ac7007c91b
	.quad	-8858670899299929442            # 0x850fadc09923329e
	.quad	280014188641050033              # 0x3e2cf6bc604ddb1
	.quad	-6461652605697523899            # 0xa6539930bf6bff45
	.quad	-8873354301053463267            # 0x84db8346b786151d
	.quad	-3465379738694516970            # 0xcfe87f7cef46ff16
	.quad	-1868320839462053276            # 0xe612641865679a64
	.quad	-9083391364325154962            # 0x81f14fae158c5f6e
	.quad	5749828502977298559             # 0x4fcb7e8f3f60c07f
	.quad	-6742553186979055799            # 0xa26da3999aef7749
	.quad	-2036086408133152610            # 0xe3be5e330f38f09e
	.quad	-3816505465296431844            # 0xcb090c8001ab551c
	.quad	6678264026688335046             # 0x5cadf5bfd3072cc6
	.quad	-158945813193151901             # 0xfdcb4fa002162a63
	.quad	8347830033360418807             # 0x73d9732fc7c8f7f7
	.quad	-7016870160886801794            # 0x9e9f11c4014dda7e
	.quad	2911550761636567803             # 0x2867e7fddcdd9afb
	.quad	-4159401682681114339            # 0xc646d63501a1511d
	.quad	-5583933584809066055            # 0xb281e1fd541501b9
	.quad	-587566084924005019             # 0xf7d88bc24209a565
	.quad	2243455055843443239             # 0x1f225a7ca91a4227
	.quad	-7284757830718584993            # 0x9ae757596946075f
	.quad	3708002419115845977             # 0x3375788de9b06959
	.quad	-4494261269970843337            # 0xc1a12d2fc3978937
	.quad	23317005467419567               # 0x52d6b1641c83af
	.quad	-1006140569036166268            # 0xf209787bb47d6b84
	.quad	-4582539761593113445            # 0xc0678c5dbd23a49b
	.quad	-7546366883288685774            # 0x9745eb4d50ce6332
	.quad	-558244341782001951             # 0xf840b7ba963646e1
	.quad	-4821272585683469313            # 0xbd176620a501fbff
	.quad	-5309491445654890343            # 0xb650e5a93bc3d899
	.quad	-1414904713676948737            # 0xec5d3fa8ce427aff
	.quad	-6636864307068612929            # 0xa3e51f138ab4cebf
	.quad	-7801844473689174817            # 0x93ba47c980e98cdf
	.quad	-4148040191917883080            # 0xc66f336c36b10138
	.quad	-5140619573684080617            # 0xb8a8d9bbe123f017
	.quad	-5185050239897353851            # 0xb80b0047445d4185
	.quad	-1814088448677712867            # 0xe6d3102ad96cec1d
	.quad	-6481312799871692314            # 0xa60dc059157491e6
	.quad	-8051334308064652398            # 0x9043ea1ac7e41392
	.quad	-8662506518347195600            # 0x87c89837ad68db30
	.quad	-5452481866653427593            # 0xb454e4a179dd1877
	.quad	3006924907348169212             # 0x29babe4598c311fc
	.quad	-2203916314889396588            # 0xe16a1dc9d8545e94
	.quad	-853029884242176389             # 0xf4296dd6fef3d67b
	.quad	-8294976724446954723            # 0x8ce2529e2734bb1d
	.quad	1772699331562333709             # 0x1899e4a65f58660d
	.quad	-5757034887131305500            # 0xb01ae745b101e9e4
	.quad	6827560182880305040             # 0x5ec05dcff72e7f90
	.quad	-2584607590486743971            # 0xdc21a1171d42645d
	.quad	8534450228600381300             # 0x76707543f4fa1f74
	.quad	-8532908771695296838            # 0x899504ae72497eba
	.quad	7639874402088932265             # 0x6a06494a791c53a9
	.quad	-6054449946191733143            # 0xabfa45da0edbde69
	.quad	326470965756389523              # 0x487db9d17636893
	.quad	-2956376414312278525            # 0xd6f8d7509292d603
	.quad	5019774725622874807             # 0x45a9d2845d3c42b7
	.quad	-8765264286586255934            # 0x865b86925b9bc5c2
	.quad	831516194300602803              # 0xb8a2392ba45a9b3
	.quad	-6344894339805432014            # 0xa7f26836f282b732
	.quad	-8183976793979022305            # 0x8e6cac7768d7141f
	.quad	-3319431906329402113            # 0xd1ef0244af2364ff
	.quad	3605087062808385831             # 0x3207d795430cd927
	.quad	-8992173969096958177            # 0x8335616aed761f1f
	.quad	9170708441896323001             # 0x7f44e6bd49e807b9
	.quad	-6628531442943809817            # 0xa402b9c5a8d3a6e7
	.quad	6851699533943015847             # 0x5f16206c9c6209a7
	.quad	-3673978285252374367            # 0xcd036837130890a1
	.quad	3952938399001381904             # 0x36dba887c37a8c10
	.quad	-9213765455923815836            # 0x802221226be55a64
	.quad	-4446942528265218166            # 0xc2494954da2c978a
	.quad	-6905520801477381891            # 0xa02aa96b06deb0fd
	.quad	-946992141904134803             # 0xf2db9baa10b7bd6d
	.quad	-4020214983419339459            # 0xc83553c5c8965d3d
	.quad	8039631859474607304             # 0x6f92829494e5acc8
	.quad	-413582710846786420             # 0xfa42a8b73abbf48c
	.quad	-3785518230938904582            # 0xcb772339ba1f17fa
	.quad	-7176018221920323369            # 0x9c69a97284b578d7
	.quad	-60105885123121412              # 0xff2a760414536efc
	.quad	-4358336758973016307            # 0xc38413cf25e2d70d
	.quad	-75132356403901765              # 0xfef5138519684abb
	.quad	-836234930288882479             # 0xf46518c2ef5b8cd1
	.quad	9129456591349898602             # 0x7eb258665fc25d6a
	.quad	-7440175859071633406            # 0x98bf2f79d5993802
	.quad	-1211618658047395230            # 0xef2f773ffbd97a62
	.quad	-4688533805412153853            # 0xbeeefb584aff8603
	.quad	-6126209340986631941            # 0xaafb550ffacfd8fb
	.quad	-1248981238337804412            # 0xeeaaba2e5dbf6784
	.quad	-7657761676233289927            # 0x95ba2a53f983cf39
	.quad	-7698142301602209614            # 0x952ab45cfa97a0b2
	.quad	-2480258038432112252            # 0xdd945a747bf26184
	.quad	-5010991858575374113            # 0xba756174393d88df
	.quad	-7712008566467528219            # 0x94f971119aeef9e5
	.quad	-1652053804791829737            # 0xe912b9d1478ceb17
	.quad	8806733365625141342             # 0x7a37cd5601aab85e
	.quad	-7950062655635975442            # 0x91abb422ccb812ee
	.quad	-6025006692552756421            # 0xac62e055c10ab33b
	.quad	-5325892301117581398            # 0xb616a12b7fe617aa
	.quad	6303799689591218186             # 0x577b986b314d600a
	.quad	-2045679357969588844            # 0xe39c49765fdf9d94
	.quad	-1343622424865753076            # 0xed5a7e85fda0b80c
	.quad	-8196078626372074883            # 0x8e41ade9fbebc27d
	.quad	1466078993672598280             # 0x14588f13be847308
	.quad	-5633412264537705700            # 0xb1d219647ae6b31c
	.quad	6444284760518135753             # 0x596eb2d8ae258fc9
	.quad	-2430079312244744221            # 0xde469fbd99a05fe3
	.quad	8055355950647669692             # 0x6fca5f8ed9aef3bc
	.quad	-8436328597794046994            # 0x8aec23d680043bee
	.quad	2728754459941099605             # 0x25de7bb9480d5855
	.quad	-5933724728815170839            # 0xada72ccc20054ae9
	.quad	-5812428961928401301            # 0xaf561aa79a10ae6b
	.quad	-2805469892591575644            # 0xd910f7ff28069da4
	.quad	1957835834444274181             # 0x1b2ba1518094da05
	.quad	-8670947710510816634            # 0x87aa9aff79042286
	.quad	-7999724640327104445            # 0x90fb44d2f05d0843
	.quad	-6226998619711132888            # 0xa99541bf57452b28
	.quad	3835402254873283156             # 0x353a1607ac744a54
	.quad	-3172062256211528206            # 0xd3fa922f2d1675f2
	.quad	4794252818591603945             # 0x42889b8997915ce9
	.quad	-8900067937773286985            # 0x847c9b5d7c2e09b7
	.quad	7608094030047140370             # 0x69956135febada12
	.quad	-6513398903789220827            # 0xa59bc234db398c25
	.quad	4898431519131537558             # 0x43fab9837e699096
	.quad	-3530062611309138130            # 0xcf02b2c21207ef2e
	.quad	-7712018656367741764            # 0x94f967e45e03f4bc
	.quad	-9123818159709293187            # 0x8161afb94b44f57d
	.quad	2097517367411243254             # 0x1d1be0eebac278f6
	.quad	-6793086681209228580            # 0xa1ba1ba79e1632dc
	.quad	7233582727691441971             # 0x6462d92a69731733
	.quad	-3879672333084147821            # 0xca28a291859bbf93
	.quad	9041978409614302463             # 0x7d7b8f7503cfdcff
	.quad	-237904397927796872             # 0xfcb2cb35e702af78
	.quad	6690786993590490175             # 0x5cda735244c3d43f
	.quad	-7066219276345954901            # 0x9defbf01b061adab
	.quad	4181741870994056360             # 0x3a0888136afa64a8
	.quad	-4221088077005055722            # 0xc56baec21c7a1916
	.quad	615491320315182545              # 0x88aaa1845b8fdd1
	.quad	-664674077828931749             # 0xf6c69a72a3989f5b
	.quad	-8454007886460797626            # 0x8aad549e57273d46
	.quad	-7332950326284164199            # 0x9a3c2087a63f6399
	.quad	3939617107816777292             # 0x36ac54e2f678864c
	.quad	-4554501889427817345            # 0xc0cb28a98fcf3c7f
	.quad	-8910536670511192098            # 0x84576a1bb416a7de
	.quad	-1081441343357383777            # 0xf0fdf2d3f3c30b9f
	.quad	7308573235570561494             # 0x656d44a2a11c51d6
	.quad	-7593429867239446717            # 0x969eb7c47859e743
	.quad	-6961356773836868826            # 0x9f644ae5a4b1b326
	.quad	-4880101315621920492            # 0xbc4665b596706114
	.quad	-8701695967296086033            # 0x873d5d9f0dde1fef
	.quad	-1488440626100012711            # 0xeb57ff22fc0c7959
	.quad	-6265433940692719637            # 0xa90cb506d155a7eb
	.quad	-7847804418953589800            # 0x9316ff75dd87cbd8
	.quad	695789805494438131              # 0x9a7f12442d588f3
	.quad	-5198069505264599346            # 0xb7dcbf5354e9bece
	.quad	869737256868047664              # 0xc11ed6d538aeb30
	.quad	-1885900863153361279            # 0xe5d3ef282a242e81
	.quad	-8136200465769716229            # 0x8f1668c8a86da5fb
	.quad	-8096217067111932656            # 0x8fa475791a569d10
	.quad	-473439272678684739             # 0xf96e017d694487bd
	.quad	-5508585315462527915            # 0xb38d92d760ec4455
	.quad	4019886927579031981             # 0x37c981dcc395a9ad
	.quad	-2274045625900771990            # 0xe070f78d3927556a
	.quad	-8810199395808373736            # 0x85bbe253f47b1418
	.quad	-8338807543829064350            # 0x8c469ab843b89562
	.quad	-7812217631593927537            # 0x93956d7478ccec8f
	.quad	-5811823411358942533            # 0xaf58416654a6babb
	.quad	4069786015789754291             # 0x387ac8d1970027b3
	.quad	-2653093245771290262            # 0xdb2e51bfe9d0696a
	.quad	475546501309804959              # 0x6997b05fcc0319f
	.quad	-8575712306248138270            # 0x88fcf317f22241e2
	.quad	4908902581746016004             # 0x441fece3bdf81f04
	.quad	-6107954364382784934            # 0xab3c2fddeeaad25a
	.quad	-3087243809672255804            # 0xd527e81cad7626c4
	.quad	-3023256937051093263            # 0xd60b3bd56a5586f1
	.quad	-8470740780517707659            # 0x8a71e223d8d3b075
	.quad	-8807064613298015146            # 0x85c7056562757456
	.quad	-682526969396179382             # 0xf6872d5667844e4a
	.quad	-6397144748195131028            # 0xa738c6bebb12d16c
	.quad	-5464844730172612132            # 0xb428f8ac016561dc
	.quad	-3384744916816525881            # 0xd106f86e69d785c7
	.quad	-2219369894288377261            # 0xe13336d701beba53
	.quad	-9032994600651410532            # 0x82a45b450226b39c
	.quad	-1387106183930235788            # 0xecc0024661173474
	.quad	-6679557232386875260            # 0xa34d721642b06084
	.quad	2877803288514593169             # 0x27f002d7f95d0191
	.quad	-3737760522056206171            # 0xcc20ce9bd35c78a5
	.quad	3597254110643241461             # 0x31ec038df7b441f5
	.quad	-60514634142869810              # 0xff290242c83396ce
	.quad	9108253656731439730             # 0x7e67047175a15272
	.quad	-6955350673980375487            # 0x9f79a169bd203e41
	.quad	1080972517029761927             # 0xf0062c6e984d387
	.quad	-4082502324048081455            # 0xc75809c42c684dd1
	.quad	5962901664714590313             # 0x52c07b78a3e60869
	.quad	-491441886632713915             # 0xf92e0c3537826145
	.quad	-6381430974388925821            # 0xa7709a56ccdf8a83
	.quad	-7224680206786528053            # 0x9bbcc7a142b17ccb
	.quad	-8600080377420466542            # 0x88a66076400bb692
	.quad	-4419164240055772162            # 0xc2abf989935ddbfe
	.quad	7696643601933968438             # 0x6acff893d00ea436
	.quad	-912269281642327298             # 0xf356f7ebf83552fe
	.quad	397432465562684740              # 0x583f6b8c4124d44
	.quad	-7487697328667536418            # 0x98165af37b2153de
	.quad	-4363290727450709941            # 0xc3727a337a8b704b
	.quad	-4747935642407032618            # 0xbe1bf1b059e9a8d6
	.quad	8380944645968776285             # 0x744f18c0592e4c5d
	.quad	-1323233534581402868            # 0xeda2ee1c7064130c
	.quad	1252808770606194548             # 0x1162def06f79df74
	.quad	-7744549986754458649            # 0x9485d4d1c63e8be7
	.quad	-8440366555225904215            # 0x8addcb5645ac2ba9
	.quad	-5069001465015685407            # 0xb9a74a0637ce2ee1
	.quad	7896285879677171347             # 0x6d953e2bd7173693
	.quad	-1724565812842218855            # 0xe8111c87c5c1ba99
	.quad	-3964700705685699528            # 0xc8fa8db6ccdd0438
	.quad	-7995382660667468640            # 0x910ab1d4db9914a0
	.quad	2133748077373825699             # 0x1d9c9892400a22a3
	.quad	-5382542307406947896            # 0xb54d5e4a127f59c8
	.quad	2667185096717282124             # 0x2503beb6d00cab4c
	.quad	-2116491865831296966            # 0xe2a0b5dc971f303a
	.quad	3333981370896602654             # 0x2e44ae64840fd61e
	.quad	-8240336443785642460            # 0x8da471a9de737e24
	.quad	6695424375237764563             # 0x5ceaecfed289e5d3
	.quad	-5688734536304665171            # 0xb10d8e1456105dad
	.quad	8369280469047205704             # 0x7425a83e872c5f48
	.quad	-2499232151953443560            # 0xdd50f1996b947518
	.quad	-3373457468973156582            # 0xd12f124e28f7771a
	.quad	-8479549122611984081            # 0x8a5296ffe33cc92f
	.quad	-9025939945749304720            # 0x82bd6b70d99aaa70
	.quad	-5987750384837592197            # 0xace73cbfdc0bfb7b
	.quad	7164319141522920716             # 0x636cc64d1001550c
	.quad	-2873001962619602342            # 0xd8210befd30efa5a
	.quad	4343712908476262991             # 0x3c47f7e05401aa4f
	.quad	-8713155254278333320            # 0x8714a775e3e95c78
	.quad	7326506586225052274             # 0x65acfaec34810a72
	.quad	-6279758049420528746            # 0xa8d9d1535ce3b396
	.quad	9158133232781315342             # 0x7f1839a741a14d0e
	.quad	-3238011543348273028            # 0xd31045a8341ca07c
	.quad	2224294504121868369             # 0x1ede48111209a051
	.quad	-8941286242233752499            # 0x83ea2b892091e44d
	.quad	-7833187971778608077            # 0x934aed0aab460433
	.quad	-6564921784364802720            # 0xa4e4b66b68b65d60
	.quad	-568112927868484288             # 0xf81da84d56178540
	.quad	-3594466212028615495            # 0xce1de40642e3f4b9
	.quad	3901544858591782543             # 0x36251260ab9d668f
	.quad	-9164070410158966541            # 0x80d2ae83e9ce78f3
	.quad	-4479063491021217766            # 0xc1d72b7c6b42601a
	.quad	-6843401994271320272            # 0xa1075a24e4421730
	.quad	-5598829363776522208            # 0xb24cf65b8612f820
	.quad	-3942566474411762436            # 0xc94930ae1d529cfc
	.quad	-2386850686293264856            # 0xdee033f26797b628
	.quad	-316522074587315140             # 0xfb9b7cd9a4a7443c
	.quad	1628122660560806834             # 0x169840ef017da3b2
	.quad	-7115355324258153819            # 0x9d412e0806e88aa5
	.quad	-8205795374004271537            # 0x8e1f289560ee864f
	.quad	-4282508136895304370            # 0xc491798a08a2ad4e
	.quad	-1033872180650563613            # 0xf1a6f2bab92a27e3
	.quad	-741449152691742558             # 0xf5b5d7ec8acb58a2
	.quad	-5904026244240592420            # 0xae10af696774b1dc
	.quad	-7380934748073420955            # 0x9991a6f3d6bf1765
	.quad	-5995859411864064214            # 0xacca6da1e0a8ef2a
	.quad	-4614482416664388289            # 0xbff610b0cc6edd3f
	.quad	1728547772024695540             # 0x17fd090a58d32af4
	.quad	-1156417002403097458            # 0xeff394dcff8a948e
	.quad	-2451001303396518479            # 0xddfc4b4cef07f5b1
	.quad	-7640289654143017767            # 0x95f83d0a1fb69cd9
	.quad	5385653213018257807             # 0x4abdaf101564f98f
	.quad	-4938676049251384305            # 0xbb764c4ca7a4440f
	.quad	-7102991539009341454            # 0x9d6d1ad41abe37f2
	.quad	-1561659043136842477            # 0xea53df5fd18d5513
	.quad	-8878739423761676818            # 0x84c86189216dc5ee
	.quad	-7893565929601608404            # 0x92746b9be2f8552c
	.quad	3674159897003727797             # 0x32fd3cf5b4e49bb5
	.quad	-5255271393574622601            # 0xb7118682dbb66a77
	.quad	4592699871254659746             # 0x3fbc8c33221dc2a2
	.quad	-1957403223540890347            # 0xe4d5e82392a40515
	.quad	1129188820640936779             # 0xfabaf3feaa5334b
	.quad	-8140906042354138323            # 0x8f05b1163ba6832d
	.quad	3011586022114279439             # 0x29cb4d87f2a7400f
	.quad	-5564446534515285000            # 0xb2c71d5bca9023f8
	.quad	8376168546070237203             # 0x743e20e9ef511013
	.quad	-2343872149716718346            # 0xdf78e4b2bd342cf6
	.quad	-7976533391121755113            # 0x914da9246b255417
	.quad	-8382449121214030822            # 0x8bab8eefb6409c1a
	.quad	1932195658189984911             # 0x1ad089b6c2f7548f
	.quad	-5866375383090150624            # 0xae9672aba3d0c320
	.quad	-6808127464117294670            # 0xa184ac2473b529b2
	.quad	-2721283210435300376            # 0xda3c0f568cc4f3e8
	.quad	-3898473311719230433            # 0xc9e5d72d90a2741f
	.quad	-8618331034163144591            # 0x8865899617fb1871
	.quad	9092669226243950739             # 0x7e2fa67c7a658893
	.quad	-6161227774276542835            # 0xaa7eebfb9df9de8d
	.quad	-2469221522477225288            # 0xddbb901b98feeab8
	.quad	-3089848699418290639            # 0xd51ea6fa85785631
	.quad	6136845133758244198             # 0x552a74227f3ea566
	.quad	-8848684464777513506            # 0x8533285c936b35de
	.quad	-3082000819042179232            # 0xd53a88958f872760
	.quad	-6449169562544503978            # 0xa67ff273b8460356
	.quad	-8464187042230111944            # 0x8a892abaf368f138
	.quad	-3449775934753242068            # 0xd01fef10a657842c
	.quad	3254824252494523782             # 0x2d2b7569b0432d86
	.quad	-9073638986861858149            # 0x8213f56a67f6b29b
	.quad	-7189106879045698444            # 0x9c3b29620e29fc74
	.quad	-6730362715149934782            # 0xa298f2c501f45f42
	.quad	-8986383598807123056            # 0x8349f3ba91b47b90
	.quad	-3801267375510030573            # 0xcb3f2f7642717713
	.quad	2602078556773259892             # 0x241c70a936219a74
	.quad	-139898200960150313             # 0xfe0efb53d30dd4d7
	.quad	-1359087822460813039            # 0xed238cd383aa0111
	.quad	-7004965403241175802            # 0x9ec95d1463e8a506
	.quad	-849429889038008149             # 0xf4363804324a40ab
	.quad	-4144520735624081848            # 0xc67bb4597ce2ce48
	.quad	-5673473379724898090            # 0xb143c6053edcd0d6
	.quad	-568964901102714406             # 0xf81aa16fdc1b81da
	.quad	-2480155706228734709            # 0xdd94b7868e94050b
	.quad	-7273132090830278360            # 0x9b10a4e5e9913128
	.quad	-3855940325606653145            # 0xca7cf2b4191c8327
	.quad	-4479729095110460046            # 0xc1d4ce1f63f57d72
	.quad	-208239388580928527             # 0xfd1c2f611f63a3f1
	.quad	-987975350460687153             # 0xf24a01a73cf2dccf
	.quad	-4871985254153548563            # 0xbc633b39673c8ced
	.quad	-7535013621679011327            # 0x976e41088617ca01
	.quad	-3044990783845967852            # 0xd5be0503e085d814
	.quad	-4807081008671376254            # 0xbd49d14aa79dbc82
	.quad	5417133557047315993             # 0x4b2d8644d8a74e19
	.quad	-1397165242411832414            # 0xec9c459d51852ba2
	.quad	-2451955090545630817            # 0xddf8e7d60ed1219f
	.quad	-7790757304148477115            # 0x93e1ab8252f33b45
	.quad	-3838314940804713212            # 0xcabb90e5c942b504
	.quad	-5126760611758208489            # 0xb8da1662e7b00a17
	.quad	4425478360848884292             # 0x3d6a751f3b936244
	.quad	-1796764746270372707            # 0xe7109bfba19c0c9d
	.quad	920161932633717461              # 0xcc512670a783ad5
	.quad	-8040506994060064798            # 0x906a617d450187e2
	.quad	2880944217109767366             # 0x27fb2b80668b24c6
	.quad	-5438947724147693094            # 0xb484f9dc9641e9da
	.quad	-5622191765467566601            # 0xb1f9f660802dedf7
	.quad	-2186998636757228463            # 0xe1a63853bbd26451
	.quad	6807318348447705460             # 0x5e7873f8a0396974
	.quad	-8284403175614349646            # 0x8d07e33455637eb2
	.quad	-2662955059861265943            # 0xdb0b487b6423e1e9
	.quad	-5743817951090549153            # 0xb049dc016abc5e5f
	.quad	-7940379843253970333            # 0x91ce1a9a3d2cda63
	.quad	-2568086420435798537            # 0xdc5c5301c56b75f7
	.quad	8521269269642088700             # 0x7641a140cc7810fc
	.quad	-8522583040413455942            # 0x89b9b3e11b6329ba
	.quad	-6203421752542164322            # 0xa9e904c87fcb0a9e
	.quad	-6041542782089432023            # 0xac2820d9623bf429
	.quad	6080780864604458309             # 0x546345fa9fbdcd45
	.quad	-2940242459184402125            # 0xd732290fbacaf133
	.quad	-6234081974526590826            # 0xa97c177947ad4096
	.quad	-8755180564631333184            # 0x867f59a9d4bed6c0
	.quad	5327070802775656542             # 0x49ed8eabcccc485e
	.quad	-6332289687361778576            # 0xa81f301449ee8c70
	.quad	6658838503469570677             # 0x5c68f256bfff5a75
	.quad	-3303676090774835316            # 0xd226fc195c6a2f8c
	.quad	8323548129336963346             # 0x73832eec6fff3112
	.quad	-8982326584375353929            # 0x83585d8fd9c25db7
	.quad	-4021154456019173716            # 0xc831fd53c5ff7eac
	.quad	-6616222212041804507            # 0xa42e74f3d032f525
	.quad	-5026443070023967146            # 0xba3e7ca8b77f5e56
	.quad	-3658591746624867729            # 0xcd3a1230c43fb26f
	.quad	2940318199324816876             # 0x28ce1bd2e55f35ec
	.quad	-9204148869281624187            # 0x80444b5e7aa7cf85
	.quad	8755227902219092404             # 0x7980d163cf5b81b4
	.quad	-6893500068174642330            # 0xa0555e361951c366
	.quad	-2891023177508298208            # 0xd7e105bcc3326220
	.quad	-4005189066790915008            # 0xc86ab5c39fa63440
	.quad	-8225464990312760664            # 0x8dd9472bf3fefaa8
	.quad	-394800315061255856             # 0xfa856334878fc150
	.quad	-5670145219463562926            # 0xb14f98f6f0feb952
	.quad	-7164279224554366766            # 0x9c935e00d4b9d8d2
	.quad	7985374283903742932             # 0x6ed1bf9a569f33d4
	.quad	-4343663012265570553            # 0xc3b8358109e84f07
	.quad	758345818024902857              # 0xa862f80ec4700c9
	.quad	-817892746904575288             # 0xf4a642e14c6262c8
	.quad	-3663753745896259333            # 0xcd27bb612758c0fb
	.quad	-7428711994456441411            # 0x98e7e9cccfbd7dbd
	.quad	-9207375118826243939            # 0x8038d51cb897789d
	.quad	-4674203974643163860            # 0xbf21e44003acdd2c
	.quad	-2285846861678029116            # 0xe0470a63e6bd56c4
	.quad	-1231068949876566920            # 0xeeea5d5004981478
	.quad	1754377441329851509             # 0x1858ccfce06cac75
	.quad	-7686947121313936181            # 0x95527a5202df0ccb
	.quad	1096485900831157193             # 0xf37801e0c43ebc9
	.quad	-4996997883215032323            # 0xbaa718e68396cffd
	.quad	-3241078642388441413            # 0xd30560258f54e6bb
	.quad	-1634561335591402499            # 0xe950df20247c83fd
	.quad	5172023733869224042             # 0x47c6b82ef32a206a
	.quad	-7939129862385708418            # 0x91d28b7416cdd27e
	.quad	5538357842881958978             # 0x4cdc331d57fa5442
	.quad	-5312226309554747619            # 0xb6472e511c81471d
	.quad	-2300424733252327085            # 0xe0133fe4adf8e953
	.quad	-2028596868516046619            # 0xe3d8f9e563a198e5
	.quad	6347841120289366951             # 0x58180fddd97723a7
	.quad	-8185402070463610993            # 0x8e679c2f5e44ff8f
	.quad	6273243709394548297             # 0x570f09eaa7ea7649
	.quad	-5620066569652125837            # 0xb201833b35d63f73
	.quad	3229868618315797467             # 0x2cd2cc6551e513db
	.quad	-2413397193637769393            # 0xde81e40a034bcf4f
	.quad	-574350245532641070             # 0xf8077f7ea65e58d2
	.quad	-8425902273664687727            # 0x8b112e86420f6191
	.quad	-358968903457900669             # 0xfb04afaf27faf783
	.quad	-5920691823653471754            # 0xadd57a27d29339f6
	.quad	8774660907532399972             # 0x79c5db9af1f9b564
	.quad	-2789178761139451788            # 0xd94ad8b1c7380874
	.quad	1744954097560724157             # 0x18375281ae7822bd
	.quad	-8660765753353239224            # 0x87cec76f1c830548
	.quad	-8132775725879323210            # 0x8f2293910d0b15b6
	.quad	-6214271173264161126            # 0xa9c2794ae3a3c69a
	.quad	-5554283638921766109            # 0xb2eb3875504ddb23
	.quad	-3156152948152813503            # 0xd433179d9c8cb841
	.quad	6892203506629956076             # 0x5fa60692a46151ec
	.quad	-8890124620236590296            # 0x849feec281d7f328
	.quad	-2609901835997359308            # 0xdbc7c41ba6bcd334
	.quad	-6500969756868349965            # 0xa5c7ea73224deff3
	.quad	1349308723430688769             # 0x12b9b522906c0801
	.quad	-3514526177658049553            # 0xcf39e50feae16bef
	.quad	-2925050114139026943            # 0xd768226b34870a01
	.quad	-9114107888677362827            # 0x81842f29f2cce375
	.quad	-1828156321336891839            # 0xe6a1158300d46641
	.quad	-6780948842419315629            # 0xa1e53af46f801c53
	.quad	6938176635183661009             # 0x60495ae3c1097fd1
	.quad	-3864500034596756632            # 0xca5e89b18b602368
	.quad	4061034775552188357             # 0x385bb19cb14bdfc5
	.quad	-218939024818557886             # 0xfcf62c1dee382c42
	.quad	5076293469440235446             # 0x46729e03dd9ed7b6
	.size	_ZL12pow10_double, 9872

	.type	_ZL11float_table,@object        # 
	.p2align	3, 0x0
_ZL11float_table:
	.quad	-8106986416796705680            # 0x8f7e32ce7bea5c70
	.quad	-1903131822648998118            # 0xe596b7b0c643c71a
	.quad	-5211854272861108818            # 0xb7abc627050305ae
	.quad	-7858832233030797377            # 0x92efd1b8d0cf37bf
	.quad	-1506085128623544834            # 0xeb194f8e1ae525fe
	.quad	-4894216917640746190            # 0xbc143fa4e250eb32
	.quad	-7604722348854507275            # 0x96769950b50d88f5
	.quad	-1099509313941480671            # 0xf0bdc21abb48db21
	.quad	-4568956265895094860            # 0xc097ce7bc90715b4
	.quad	-7344513827457986211            # 0x9a130b963a6c115d
	.quad	-683175679707046969             # 0xf684df56c3e01bc7
	.quad	-4235889358507547898            # 0xc5371912364ce306
	.quad	-7078060301547948642            # 0x9dc5ada82b70b59e
	.quad	-256850038250986857             # 0xfc6f7c4045812297
	.quad	-3894828845342699809            # 0xc9f2c9cd04674edf
	.quad	-6805211891016070170            # 0xa18f07d736b90be6
	.quad	-9133518327554766459            # 0x813f3978f8940985
	.quad	-3545582879861895366            # 0xcecb8f27f4200f3a
	.quad	-6525815118631426616            # 0xa56fa5b99019a5c8
	.quad	-8910000909647051616            # 0x84595161401484a0
	.quad	-3187955011209551616            # 0xd3c21bcecceda100
	.quad	-6239712823709551616            # 0xa968163f0a57b400
	.quad	-8681119073709551616            # 0x878678326eac9000
	.quad	-2821744073709551616            # 0xd8d726b7177a8000
	.quad	-5946744073709551616            # 0xad78ebc5ac620000
	.quad	-8446744073709551616            # 0x8ac7230489e80000
	.quad	-2446744073709551616            # 0xde0b6b3a76400000
	.quad	-5646744073709551616            # 0xb1a2bc2ec5000000
	.quad	-8206744073709551616            # 0x8e1bc9bf04000000
	.quad	-2062744073709551616            # 0xe35fa931a0000000
	.quad	-5339544073709551616            # 0xb5e620f480000000
	.quad	-7960984073709551616            # 0x9184e72a00000000
	.quad	-1669528073709551616            # 0xe8d4a51000000000
	.quad	-5024971273709551616            # 0xba43b74000000000
	.quad	-7709325833709551616            # 0x9502f90000000000
	.quad	-1266874889709551616            # 0xee6b280000000000
	.quad	-4702848726509551616            # 0xbebc200000000000
	.quad	-7451627795949551616            # 0x9896800000000000
	.quad	-854558029293551616             # 0xf424000000000000
	.quad	-4372995238176751616            # 0xc350000000000000
	.quad	-7187745005283311616            # 0x9c40000000000000
	.quad	-432345564227567616             # 0xfa00000000000000
	.quad	-4035225266123964416            # 0xc800000000000000
	.quad	-6917529027641081856            # 0xa000000000000000
	.quad	-9223372036854775808            # 0x8000000000000000
	.quad	-3689348814741910323            # 0xcccccccccccccccd
	.quad	-6640827866535438581            # 0xa3d70a3d70a3d70b
	.quad	-9002011107970261188            # 0x83126e978d4fdf3c
	.quad	-3335171328526686932            # 0xd1b71758e219652c
	.quad	-6357485877563259868            # 0xa7c5ac471b478424
	.quad	-8775337516792518218            # 0x8637bd05af6c69b6
	.quad	-2972493582642298179            # 0xd6bf94d5e57a42bd
	.quad	-6067343680855748867            # 0xabcc77118461cefd
	.quad	-8543223759426509416            # 0x89705f4136b4a598
	.quad	-2601111570856684097            # 0xdbe6fecebdedd5bf
	.quad	-5770238071427257601            # 0xafebff0bcb24aaff
	.quad	-8305539271883716404            # 0x8cbccc096f5088cc
	.quad	-2220816390788215276            # 0xe12e13424bb40e14
	.quad	-5466001927372482544            # 0xb424dc35095cd810
	.quad	-8062150356639896358            # 0x901d7cf73ab0acda
	.quad	-1831394126398103204            # 0xe69594bec44de15c
	.quad	-5154464115860392886            # 0xb877aa3236a4b44a
	.quad	-7812920107430224632            # 0x9392ee8e921d5d08
	.quad	-1432625727662628442            # 0xec1e4a7db69561a6
	.quad	-4835449396872013077            # 0xbce5086492111aeb
	.quad	-7557708332239520785            # 0x971da05074da7bef
	.quad	-1024286887357502286            # 0xf1c90080baf72cb2
	.quad	-4508778324627912152            # 0xc16d9a0095928a28
	.quad	-7296371474444240045            # 0x9abe14cd44753b53
	.quad	-606147914885053102             # 0xf79687aed3eec552
	.quad	-4174267146649952805            # 0xc612062576589ddb
	.quad	-7028762532061872567            # 0x9e74d1b791e07e49
	.quad	-177973607073265138             # 0xfd87b5f28300ca0e
	.quad	-3831727700400522433            # 0xcad2f7f5359a3b3f
	.quad	-6754730975062328270            # 0xa2425ff75e14fc32
	.quad	-9093133594791772939            # 0x81ceb32c4b43fcf5
	.quad	-3480967307441105733            # 0xcfb11ead453994bb
	.long	892611941                       # 0x35342d65
	.long	875834725                       # 0x34342d65
	.long	859057509                       # 0x33342d65
	.long	842280293                       # 0x32342d65
	.long	825503077                       # 0x31342d65
	.long	808725861                       # 0x30342d65
	.long	959655269                       # 0x39332d65
	.long	942878053                       # 0x38332d65
	.long	926100837                       # 0x37332d65
	.long	909323621                       # 0x36332d65
	.long	892546405                       # 0x35332d65
	.long	875769189                       # 0x34332d65
	.long	858991973                       # 0x33332d65
	.long	842214757                       # 0x32332d65
	.long	825437541                       # 0x31332d65
	.long	808660325                       # 0x30332d65
	.long	959589733                       # 0x39322d65
	.long	942812517                       # 0x38322d65
	.long	926035301                       # 0x37322d65
	.long	909258085                       # 0x36322d65
	.long	892480869                       # 0x35322d65
	.long	875703653                       # 0x34322d65
	.long	858926437                       # 0x33322d65
	.long	842149221                       # 0x32322d65
	.long	825372005                       # 0x31322d65
	.long	808594789                       # 0x30322d65
	.long	959524197                       # 0x39312d65
	.long	942746981                       # 0x38312d65
	.long	925969765                       # 0x37312d65
	.long	909192549                       # 0x36312d65
	.long	892415333                       # 0x35312d65
	.long	875638117                       # 0x34312d65
	.long	858860901                       # 0x33312d65
	.long	842083685                       # 0x32312d65
	.long	825306469                       # 0x31312d65
	.long	808529253                       # 0x30312d65
	.long	959458661                       # 0x39302d65
	.long	942681445                       # 0x38302d65
	.long	925904229                       # 0x37302d65
	.long	909127013                       # 0x36302d65
	.long	892349797                       # 0x35302d65
	.long	875572581                       # 0x34302d65
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	925903717                       # 0x37302b65
	.long	942680933                       # 0x38302b65
	.long	959458149                       # 0x39302b65
	.long	808528741                       # 0x30312b65
	.long	825305957                       # 0x31312b65
	.long	842083173                       # 0x32312b65
	.long	858860389                       # 0x33312b65
	.long	875637605                       # 0x34312b65
	.long	892414821                       # 0x35312b65
	.long	909192037                       # 0x36312b65
	.long	925969253                       # 0x37312b65
	.long	942746469                       # 0x38312b65
	.long	959523685                       # 0x39312b65
	.long	808594277                       # 0x30322b65
	.long	825371493                       # 0x31322b65
	.long	842148709                       # 0x32322b65
	.long	858925925                       # 0x33322b65
	.long	875703141                       # 0x34322b65
	.long	892480357                       # 0x35322b65
	.long	909257573                       # 0x36322b65
	.long	926034789                       # 0x37322b65
	.long	942812005                       # 0x38322b65
	.long	959589221                       # 0x39322b65
	.long	808659813                       # 0x30332b65
	.long	825437029                       # 0x31332b65
	.long	842214245                       # 0x32332b65
	.long	858991461                       # 0x33332b65
	.long	875768677                       # 0x34332b65
	.long	892545893                       # 0x35332b65
	.long	909323109                       # 0x36332b65
	.long	926100325                       # 0x37332b65
	.long	942877541                       # 0x38332b65
	.ascii	"\001\002\003\004\005\006\007\b\t\004\001\001"
	.ascii	"\001\002\003\004\005\006\007\b\t\003\001\001"
	.ascii	"\001\002\003\004\005\006\007\b\t\002\001\001"
	.ascii	"\003\003\004\005\006\007\b\t\n\000\001\002"
	.ascii	"\004\004\004\005\006\007\b\t\n\000\002\003"
	.ascii	"\005\005\005\005\006\007\b\t\n\000\003\004"
	.ascii	"\006\006\006\006\006\007\b\t\n\000\004\005"
	.ascii	"\007\007\007\007\007\007\b\t\n\000\005\006"
	.ascii	"\b\b\b\b\b\b\b\t\n\000\006\007"
	.ascii	"\t\t\t\t\t\t\t\t\n\000\007\b"
	.ascii	"\001\003\004\005\006\007\b\t\n\000\001\002"
	.asciz	"\"\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$!\"#$\"#$\"#$\"#$!\""
	.zero	4
	.size	_ZL11float_table, 1344

	.type	rd,@object                      # 
	.bss
	.globl	rd
	.p2align	3, 0x0
rd:
	.zero	5000
	.size	rd, 5000

	.hidden	__dso_handle
	.type	gen,@object                     # 
	.globl	gen
	.p2align	3, 0x0
gen:
	.zero	2504
	.size	gen, 2504

	.type	data,@object                    # 
	.globl	data
	.p2align	3, 0x0
data:
	.quad	0
	.size	data, 8

	.type	.L.str.10,@object               # 
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.10:
	.asciz	"every number cost %lf ns,%lf cycle\n"
	.size	.L.str.10, 36

	.type	.L.str.11,@object               # 
.L.str.11:
	.asciz	"data = %.16le \n buf = %s \n"
	.size	.L.str.11, 27

	.type	.L.str.13,@object               # 
.L.str.13:
	.asciz	"default"
	.size	.L.str.13, 8

	.section	.init_array,"aw",@init_array
	.p2align	3, 0x0
	.quad	_GLOBAL__sub_I_main.cpp
	.ident	"Intel(R) oneAPI DPC++/C++ Compiler 2025.3.1 (2025.3.1.20251023)"
	.section	".note.GNU-stack","",@progbits
