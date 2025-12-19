	.text
	.file	"convert-neon.cpp"
                                        # Start of file scope inline assembly
	.globl	_ZSt21ios_base_library_initv

                                        # End of file scope inline assembly
	.globl	_Z4nanov                        # -- Begin function _Z4nanov
	.p2align	4, 0x90
	.type	_Z4nanov,@function
_Z4nanov:                               # @_Z4nanov
	.cfi_startproc
# %bb.0:
	jmp	_ZNSt6chrono3_V212steady_clock3nowEv@PLT # TAILCALL
.Lfunc_end0:
	.size	_Z4nanov, .Lfunc_end0-_Z4nanov
	.cfi_endproc
                                        # -- End function
	.globl	_Z20encode_ten_thousandsmm      # -- Begin function _Z20encode_ten_thousandsmm
	.p2align	4, 0x90
	.type	_Z20encode_ten_thousandsmm,@function
_Z20encode_ten_thousandsmm:             # @_Z20encode_ten_thousandsmm
	.cfi_startproc
# %bb.0:
	shlq	$32, %rsi
	movabsq	$545460846719, %rcx             # imm = 0x7F0000007F
	movabsq	$281474976710556, %rdx          # imm = 0xFFFFFFFFFF9C
	orq	%rdi, %rsi
	imulq	$10486, %rsi, %rax              # imm = 0x28F6
	shrq	$20, %rax
	andq	%rax, %rcx
	imulq	%rcx, %rdx
	addq	%rsi, %rdx
	shlq	$16, %rdx
	addq	%rcx, %rdx
	movabsq	$4222189076152335, %rcx         # imm = 0xF000F000F000F
	imulq	$103, %rdx, %rax
	shrq	$10, %rax
	andq	%rax, %rcx
	movabsq	$72057594037927926, %rax        # imm = 0xFFFFFFFFFFFFF6
	imulq	%rcx, %rax
	addq	%rdx, %rax
	shlq	$8, %rax
	addq	%rcx, %rax
	retq
.Lfunc_end1:
	.size	_Z20encode_ten_thousandsmm, .Lfunc_end1-_Z20encode_ten_thousandsmm
	.cfi_endproc
                                        # -- End function
	.globl	_Z21encode_ten_thousands2mm     # -- Begin function _Z21encode_ten_thousands2mm
	.p2align	4, 0x90
	.type	_Z21encode_ten_thousands2mm,@function
_Z21encode_ten_thousands2mm:            # @_Z21encode_ten_thousands2mm
	.cfi_startproc
# %bb.0:
	shlq	$32, %rsi
	movabsq	$545460846719, %rcx             # imm = 0x7F0000007F
	movabsq	$281474976710556, %rdx          # imm = 0xFFFFFFFFFF9C
	orq	%rdi, %rsi
	imulq	$5243, %rsi, %rax               # imm = 0x147B
	shrq	$19, %rax
	andq	%rax, %rcx
	imulq	%rcx, %rdx
	addq	%rsi, %rdx
	shlq	$16, %rdx
	addq	%rcx, %rdx
	movabsq	$4222189076152335, %rcx         # imm = 0xF000F000F000F
	imulq	$103, %rdx, %rax
	shrq	$10, %rax
	andq	%rax, %rcx
	movabsq	$72057594037927926, %rax        # imm = 0xFFFFFFFFFFFFF6
	imulq	%rcx, %rax
	addq	%rdx, %rax
	shlq	$8, %rax
	addq	%rcx, %rax
	retq
.Lfunc_end2:
	.size	_Z21encode_ten_thousands2mm, .Lfunc_end2-_Z21encode_ten_thousands2mm
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _Z16to_string_khuongmPc
.LCPI3_0:
	.quad	10486                           # 0x28f6
.LCPI3_1:
	.quad	545460846719                    # 0x7f0000007f
.LCPI3_2:
	.quad	281474976710556                 # 0xffffffffff9c
.LCPI3_3:
	.quad	103                             # 0x67
.LCPI3_4:
	.quad	4222189076152335                # 0xf000f000f000f
.LCPI3_5:
	.quad	72057594037927926               # 0xfffffffffffff6
.LCPI3_6:
	.quad	3472328296227680304             # 0x3030303030303030
	.text
	.globl	_Z16to_string_khuongmPc
	.p2align	4, 0x90
	.type	_Z16to_string_khuongmPc,@function
_Z16to_string_khuongmPc:                # @_Z16to_string_khuongmPc
	.cfi_startproc
# %bb.0:
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	movabsq	$2535301200456458803, %r8       # imm = 0x232F33025BD42233
	mulxq	%rax, %rax, %rax
	mulxq	%r8, %r8, %r8
	shrq	$26, %rax
	shrq	$37, %r8
	imulq	$100000000, %rax, %rcx          # imm = 0x5F5E100
	movq	%rax, %rdx
	subq	%rcx, %rdi
	movabsq	$1844674407370956, %rcx         # imm = 0x68DB8BAC710CC
	mulxq	%rcx, %rcx, %rcx
	imulq	$10000, %rcx, %rcx              # imm = 0x2710
	subq	%rcx, %rax
	imulq	$109951163, %rdi, %rcx          # imm = 0x68DB8BB
	vmovq	%rax, %xmm0
	shrq	$40, %rcx
	imull	$10000, %ecx, %edx              # imm = 0x2710
	vmovq	%rcx, %xmm2
	subl	%edx, %edi
	vmovd	%edi, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vmovd	%r8d, %xmm1
	vpsllq	$32, %xmm0, %xmm0
	vpunpcklqdq	%xmm2, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm2[0]
	vpor	%xmm1, %xmm0, %xmm0
	vpmullq	.LCPI3_0(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$20, %xmm1, %xmm1
	vpandq	.LCPI3_1(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI3_2(%rip){1to2}, %xmm1, %xmm2
	vpaddq	%xmm0, %xmm2, %xmm0
	vpsllq	$16, %xmm0, %xmm0
	vpaddq	%xmm1, %xmm0, %xmm0
	vpmullq	.LCPI3_3(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$10, %xmm1, %xmm1
	vpandq	.LCPI3_4(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI3_5(%rip){1to2}, %xmm1, %xmm2
	vpaddq	%xmm0, %xmm2, %xmm0
	vpsllq	$8, %xmm0, %xmm0
	vpaddq	%xmm1, %xmm0, %xmm0
	vpaddq	.LCPI3_6(%rip){1to2}, %xmm0, %xmm0
	vmovdqu	%xmm0, (%rsi)
	retq
.Lfunc_end3:
	.size	_Z16to_string_khuongmPc, .Lfunc_end3-_Z16to_string_khuongmPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          # -- Begin function _Z14to_string_treemPc
.LCPI4_0:
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
.LCPI4_1:
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2, 0x0
.LCPI4_2:
	.zero	4,48
	.text
	.globl	_Z14to_string_treemPc
	.p2align	4, 0x90
	.type	_Z14to_string_treemPc,@function
_Z14to_string_treemPc:                  # @_Z14to_string_treemPc
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
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	movq	%rdx, %r10
	mulxq	%rax, %rax, %rax
	shrq	$26, %rax
	imulq	$100000000, %rax, %rcx          # imm = 0x5F5E100
	subq	%rcx, %rdi
	movabsq	$2535301200456458803, %rcx      # imm = 0x232F33025BD42233
	mulxq	%rcx, %r9, %r9
	movabsq	$1844674407370956, %rcx         # imm = 0x68DB8BAC710CC
	movq	%rax, %rdx
	imulq	$109951163, %rdi, %r8           # imm = 0x68DB8BB
	movl	%edi, %r11d
	imulq	$140737489, %rdi, %rbx          # imm = 0x8637BD1
	mulxq	%rcx, %rcx, %rcx
	movq	%rdi, -8(%rsp)                  # 8-byte Spill
	movq	%r10, %rdx
	movq	%r10, %rdi
	shrq	$40, %r8
	shrq	$37, %r9
	shrq	$47, %rbx
	imull	$10000, %ecx, %ecx              # imm = 0x2710
	movq	%rbx, %r10
	subl	%ecx, %eax
	imull	$10000, %r8d, %ecx              # imm = 0x2710
	imull	$5243, %eax, %r14d              # imm = 0x147B
	movl	%eax, %r15d
	imull	$8389, %eax, %eax               # imm = 0x20C5
	subl	%ecx, %r11d
	movabsq	$811296384146066817, %rcx       # imm = 0xB424DC35095CD81
	mulxq	%rcx, %r12, %r12
	imulq	$42949673, %r9, %rcx            # imm = 0x28F5C29
	shrl	$19, %r14d
	imull	$5243, %r11d, %ebx              # imm = 0x147B
	movl	%r11d, %ebp
	movl	%r11d, -20(%rsp)                # 4-byte Spill
	movq	%r10, %r11
	imull	$205, %r11d, %r10d
	shrl	$23, %eax
	shrq	$32, %rcx
	shrl	$19, %ebx
	shrq	$42, %r12
	movzwl	%r10w, %r10d
	imull	$100, %ecx, %ecx
	shrl	$11, %r10d
	addl	%r10d, %r10d
	leal	(%r10,%r10,4), %r10d
	subl	%ecx, %r9d
	imull	$100, %r14d, %ecx
	subb	%r10b, %r11b
	movq	%r11, -16(%rsp)                 # 8-byte Spill
	subl	%ecx, %r15d
	imull	$5243, %r8d, %ecx               # imm = 0x147B
	shrl	$19, %ecx
	imull	$100, %ecx, %ecx
	subl	%ecx, %r8d
	imull	$100, %ebx, %ecx
	subl	%ecx, %ebp
	imulq	$429496730, %r12, %rcx          # imm = 0x1999999A
	shrq	$32, %rcx
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %ecx
	subl	%ecx, %r12d
	imull	$103, %r9d, %ecx
	movzwl	%cx, %r13d
	shrl	$10, %r13d
	leal	(%r13,%r13), %ecx
	leal	(%rcx,%rcx,4), %ecx
	subb	%cl, %r9b
	imull	$205, %r14d, %ecx
	movzwl	%cx, %ecx
	shrl	$11, %ecx
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %ecx
	subb	%cl, %r14b
	imull	$103, %r15d, %ecx
	movzwl	%cx, %ecx
	shrl	$10, %ecx
	leal	(%rcx,%rcx), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r15b
	imull	$103, %r8d, %edx
	movzwl	%dx, %r10d
	shrl	$10, %r10d
	leal	(%r10,%r10), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r8b
	imull	$205, %ebx, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %bl
	imull	$103, %ebp, %edx
	movzwl	%dx, %r11d
	shrl	$10, %r11d
	leal	(%r11,%r11), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %bpl
	movq	%rdi, %rdx
	shrq	$15, %rdx
	movabsq	$633825300114115, %rdi          # imm = 0x24075F3DCEAC3
	mulxq	%rdi, %rdx, %rdx
	shrl	$20, %edx
	vmovd	%edx, %xmm0
	vpinsrb	$1, %r12d, %xmm0, %xmm0
	vpinsrb	$2, %r13d, %xmm0, %xmm0
	vpinsrb	$3, %r9d, %xmm0, %xmm0
	vpinsrb	$4, %eax, %xmm0, %xmm0
	imulq	$112589991, -8(%rsp), %rax      # 8-byte Folded Reload
                                        # imm = 0x6B5FCA7
	vpinsrb	$5, %r14d, %xmm0, %xmm0
	vpinsrb	$6, %ecx, %xmm0, %xmm0
	vpinsrb	$7, %r15d, %xmm0, %xmm0
	shrq	$50, %rax
	vpinsrb	$8, %eax, %xmm0, %xmm0
	vpinsrb	$9, -16(%rsp), %xmm0, %xmm0     # 4-byte Folded Reload
	imull	$8389, -20(%rsp), %eax          # 4-byte Folded Reload
                                        # imm = 0x20C5
	shrl	$23, %eax
	vpinsrb	$10, %r10d, %xmm0, %xmm0
	vpinsrb	$11, %r8d, %xmm0, %xmm0
	vpinsrb	$12, %eax, %xmm0, %xmm0
	movw	$4113, %ax                      # imm = 0x1011
	vpinsrb	$13, %ebx, %xmm0, %xmm1
	kmovd	%eax, %k1
	vpinsrb	$14, %r11d, %xmm1, %xmm1
	vpinsrb	$15, %ebp, %xmm1, %xmm1
	vpord	.LCPI4_2(%rip){1to4}, %xmm1, %xmm1
	vpaddb	.LCPI4_1(%rip), %xmm0, %xmm1 {%k1}
	vmovdqu	%xmm1, (%rsi)
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
.Lfunc_end4:
	.size	_Z14to_string_treemPc, .Lfunc_end4-_Z14to_string_treemPc
	.cfi_endproc
                                        # -- End function
	.globl	_Z17write_tenthousandmPc        # -- Begin function _Z17write_tenthousandmPc
	.p2align	4, 0x90
	.type	_Z17write_tenthousandmPc,@function
_Z17write_tenthousandmPc:               # @_Z17write_tenthousandmPc
	.cfi_startproc
# %bb.0:
	imulq	$4295380, %rdi, %rax            # imm = 0x418AD4
	movl	%eax, %ecx
	shrq	$32, %rax
	addq	%rcx, %rcx
	addb	$48, %al
	movb	%al, (%rsi)
	leaq	(%rcx,%rcx,4), %rax
	movl	%eax, %ecx
	addq	%rcx, %rcx
	shrq	$32, %rax
	leaq	(%rcx,%rcx,4), %rcx
	orb	$48, %al
	movb	%al, 1(%rsi)
	movl	%ecx, %eax
	addq	%rax, %rax
	shrq	$32, %rcx
	leaq	(%rax,%rax,4), %rax
	orb	$48, %cl
	movb	%cl, 2(%rsi)
	shrq	$32, %rax
	orb	$48, %al
	movb	%al, 3(%rsi)
	retq
.Lfunc_end5:
	.size	_Z17write_tenthousandmPc, .Lfunc_end5-_Z17write_tenthousandmPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          # -- Begin function _Z18to_string_tree_strmPc
.LCPI6_0:
	.quad	10                              # 0xa
	.quad	4295380                         # 0x418ad4
.LCPI6_2:
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
.LCPI6_3:
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
.LCPI6_1:
	.quad	0                               # 0x0
	.quad	1                               # 0x1
	.quad	2                               # 0x2
	.quad	8                               # 0x8
	.quad	9                               # 0x9
	.zero	8
	.zero	8
	.zero	8
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2, 0x0
.LCPI6_4:
	.zero	4,48
	.text
	.globl	_Z18to_string_tree_strmPc
	.p2align	4, 0x90
	.type	_Z18to_string_tree_strmPc,@function
_Z18to_string_tree_strmPc:              # @_Z18to_string_tree_strmPc
	.cfi_startproc
# %bb.0:
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	movabsq	$2535301200456458803, %rcx      # imm = 0x232F33025BD42233
	movabsq	$1844674407370956, %r8          # imm = 0x68DB8BAC710CC
	vmovdqa	.LCPI6_0(%rip), %xmm2           # xmm2 = [10,4295380]
	vmovdqa64	.LCPI6_1(%rip), %zmm6   # zmm6 = [0,1,2,8,9,u,u,u]
	mulxq	%rax, %rax, %rax
	mulxq	%rcx, %rcx, %rcx
	shrq	$26, %rax
	shrq	$37, %rcx
	movq	%rax, %rdx
	mulxq	%r8, %rdx, %rdx
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	imulq	$4295380, %rcx, %rcx            # imm = 0x418AD4
	subq	%r8, %rdi
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	imulq	$109951163, %rdi, %r10          # imm = 0x68DB8BB
	subq	%rdx, %rax
	movl	%ecx, %edx
	addq	%rdx, %rdx
	shrq	$40, %r10
	leaq	(%rdx,%rdx,4), %rdx
	vmovq	%rax, %xmm0
	imull	$10000, %r10d, %r9d             # imm = 0x2710
	imulq	$4295380, %r10, %r10            # imm = 0x418AD4
	movl	%edx, %r8d
	addq	%r8, %r8
	vmovq	%rdx, %xmm5
	leaq	(%r8,%r8,4), %r8
	movl	%r10d, %r11d
	addq	%r11, %r11
	subl	%r9d, %edi
	leaq	(%r11,%r11,4), %r11
	vmovq	%r8, %xmm1
	vmovd	%edi, %xmm3
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	movl	%r11d, %ebx
	addq	%rbx, %rbx
	vpmuludq	%xmm2, %xmm0, %xmm0
	leaq	(%rbx,%rbx,4), %rbx
	vmovq	%rbx, %xmm4
	vpextrq	$1, %xmm0, %rax
	vpunpcklqdq	%xmm3, %xmm4, %xmm3     # xmm3 = xmm4[0],xmm3[0]
	movl	%eax, %eax
	addq	%rax, %rax
	vpmuludq	%xmm2, %xmm3, %xmm2
	vmovq	%rcx, %xmm3
	leaq	(%rax,%rax,4), %rax
	movb	$32, %cl
	vpunpcklqdq	%xmm5, %xmm3, %xmm3     # xmm3 = xmm3[0],xmm5[0]
	vmovq	%r11, %xmm5
	kmovd	%ecx, %k1
	vinserti128	$1, %xmm1, %ymm3, %ymm1
	movl	%eax, %r8d
	addq	%r8, %r8
	vpextrq	$1, %xmm2, %rdi
	vpermt2q	%zmm0, %zmm6, %zmm1
	vmovq	%r10, %xmm0
	leaq	(%r8,%r8,4), %r8
	movl	%edi, %edi
	addq	%rdi, %rdi
	vpunpcklqdq	%xmm5, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm5[0]
	vinserti128	$1, %xmm4, %ymm0, %ymm0
	leaq	(%rdi,%rdi,4), %rdi
	movl	%r8d, %r9d
	addq	%r9, %r9
	vpermt2q	%zmm2, %zmm6, %zmm0
	leaq	(%r9,%r9,4), %r9
	movl	%edi, %ebx
	addq	%rbx, %rbx
	leaq	(%rbx,%rbx,4), %rbx
	vpbroadcastq	%rax, %zmm1 {%k1}
	movb	$64, %al
	kmovd	%eax, %k2
	movb	$-128, %al
	movl	%ebx, %r14d
	addq	%r14, %r14
	kmovd	%eax, %k3
	vpbroadcastq	%r8, %zmm1 {%k2}
	movw	$4369, %ax                      # imm = 0x1111
	vpbroadcastq	%rdi, %zmm0 {%k1}
	leaq	(%r14,%r14,4), %r14
	vpbroadcastq	%r9, %zmm1 {%k3}
	kmovd	%eax, %k1
	vpbroadcastq	%rbx, %zmm0 {%k2}
	vpsrlq	$32, %zmm1, %zmm1
	vpbroadcastq	%r14, %zmm0 {%k3}
	vpmovqb	%zmm1, %xmm1
	vpsrlq	$32, %zmm0, %zmm0
	vpmovqb	%zmm0, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpord	.LCPI6_4(%rip){1to4}, %xmm0, %xmm1
	vpaddb	.LCPI6_3(%rip), %xmm0, %xmm1 {%k1}
	vmovdqu	%xmm1, (%rsi)
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	vzeroupper
	retq
.Lfunc_end6:
	.size	_Z18to_string_tree_strmPc, .Lfunc_end6-_Z18to_string_tree_strmPc
	.cfi_endproc
                                        # -- End function
	.globl	_Z20to_string_tree_tablemPc     # -- Begin function _Z20to_string_tree_tablemPc
	.p2align	4, 0x90
	.type	_Z20to_string_tree_tablemPc,@function
_Z20to_string_tree_tablemPc:            # @_Z20to_string_tree_tablemPc
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	movq	%rdi, %r9
	movabsq	$811296384146066817, %r11       # imm = 0xB424DC35095CD81
	mulxq	%rax, %rax, %rax
	shrq	$26, %rax
	imulq	$100000000, %rax, %rcx          # imm = 0x5F5E100
	subq	%rcx, %r9
	movabsq	$2535301200456458803, %rcx      # imm = 0x232F33025BD42233
	mulxq	%rcx, %r8, %r8
	movabsq	$1844674407370956, %rcx         # imm = 0x68DB8BAC710CC
	movq	%rax, %rdx
	imulq	$140737489, %r9, %r10           # imm = 0x8637BD1
	mulxq	%rcx, %rcx, %rcx
	shrq	$37, %r8
	shrq	$47, %r10
	imull	$10000, %ecx, %ecx              # imm = 0x2710
	subl	%ecx, %eax
	imulq	$109951163, %r9, %rcx           # imm = 0x68DB8BB
                                        # kill: def $r9d killed $r9d killed $r9
	shrq	$40, %rcx
	imull	$10000, %ecx, %edx              # imm = 0x2710
	subl	%edx, %r9d
	movq	%rdi, %rdx
	imulq	$42949673, %r8, %rdi            # imm = 0x28F5C29
	mulxq	%r11, %rdx, %rdx
	shrq	$32, %rdi
	shrq	$42, %rdx
	imull	$100, %edi, %edi
	subl	%edi, %r8d
	imull	$5243, %eax, %edi               # imm = 0x147B
	shrl	$19, %edi
	imull	$100, %edi, %r11d
	subl	%r11d, %eax
	imull	$5243, %ecx, %r11d              # imm = 0x147B
	movzwl	%ax, %eax
	shrl	$19, %r11d
	imull	$100, %r11d, %r11d
	subl	%r11d, %ecx
	imull	$5243, %r9d, %r11d              # imm = 0x147B
	shrl	$19, %r11d
	imull	$100, %r11d, %ebx
	subl	%ebx, %r9d
	leaq	_ZZ20to_string_tree_tablemPcE5table(%rip), %rbx
	movzwl	(%rbx,%rdx,2), %edx
	movw	%dx, (%rsi)
	movzwl	(%rbx,%r8,2), %edx
	movw	%dx, 2(%rsi)
	movzwl	(%rbx,%rdi,2), %edx
	movw	%dx, 4(%rsi)
	movzwl	(%rbx,%rax,2), %eax
	movw	%ax, 6(%rsi)
	movzwl	(%rbx,%r10,2), %eax
	movw	%ax, 8(%rsi)
	movzwl	%cx, %eax
	movzwl	(%rbx,%rax,2), %eax
	movw	%ax, 10(%rsi)
	movzwl	(%rbx,%r11,2), %eax
	movw	%ax, 12(%rsi)
	movzwl	%r9w, %eax
	movzwl	(%rbx,%rax,2), %eax
	movw	%ax, 14(%rsi)
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end7:
	.size	_Z20to_string_tree_tablemPc, .Lfunc_end7-_Z20to_string_tree_tablemPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _Z14to_string_sse2mPc
.LCPI8_0:
	.quad	3518437209                      # 0xd1b71759
.LCPI8_1:
	.quad	10000                           # 0x2710
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0
.LCPI8_2:
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
.LCPI8_3:
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
.LCPI8_4:
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
.LCPI8_5:
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
.LCPI8_6:
	.zero	16,48
	.text
	.globl	_Z14to_string_sse2mPc
	.p2align	4, 0x90
	.type	_Z14to_string_sse2mPc,@function
_Z14to_string_sse2mPc:                  # @_Z14to_string_sse2mPc
	.cfi_startproc
# %bb.0:
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	mulxq	%rax, %rax, %rax
	shrq	$26, %rax
	imulq	$100000000, %rax, %rcx          # imm = 0x5F5E100
	vmovd	%eax, %xmm0
	subq	%rcx, %rdi
	vmovq	%rdi, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vpmuludq	.LCPI8_0(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	.LCPI8_1(%rip){1to2}, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	.LCPI8_2(%rip), %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	.LCPI8_3(%rip), %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	.LCPI8_4(%rip), %xmm0, %xmm1
	vpmullw	.LCPI8_5(%rip), %xmm1, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	.LCPI8_6(%rip), %xmm0, %xmm0
	vmovdqu	%xmm0, (%rsi)
	retq
.Lfunc_end8:
	.size	_Z14to_string_sse2mPc, .Lfunc_end8-_Z14to_string_sse2mPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _Z22to_string_sse2__pshufbmPc
.LCPI9_0:
	.quad	3518437209                      # 0xd1b71759
.LCPI9_1:
	.quad	10000                           # 0x2710
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0
.LCPI9_2:
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
.LCPI9_3:
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
.LCPI9_4:
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
.LCPI9_5:
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
.LCPI9_6:
	.byte	0                               # 0x0
	.byte	8                               # 0x8
	.byte	4                               # 0x4
	.byte	12                              # 0xc
	.byte	1                               # 0x1
	.byte	9                               # 0x9
	.byte	5                               # 0x5
	.byte	13                              # 0xd
	.byte	2                               # 0x2
	.byte	10                              # 0xa
	.byte	6                               # 0x6
	.byte	14                              # 0xe
	.byte	3                               # 0x3
	.byte	11                              # 0xb
	.byte	7                               # 0x7
	.byte	15                              # 0xf
.LCPI9_7:
	.zero	16,48
	.text
	.globl	_Z22to_string_sse2__pshufbmPc
	.p2align	4, 0x90
	.type	_Z22to_string_sse2__pshufbmPc,@function
_Z22to_string_sse2__pshufbmPc:          # @_Z22to_string_sse2__pshufbmPc
	.cfi_startproc
# %bb.0:
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	mulxq	%rax, %rax, %rax
	shrq	$26, %rax
	imulq	$100000000, %rax, %rcx          # imm = 0x5F5E100
	vmovd	%eax, %xmm0
	subq	%rcx, %rdi
	vmovq	%rdi, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vpmuludq	.LCPI9_0(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	.LCPI9_1(%rip){1to2}, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	.LCPI9_2(%rip), %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	.LCPI9_3(%rip), %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	.LCPI9_4(%rip), %xmm0, %xmm1
	vpmullw	.LCPI9_5(%rip), %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	.LCPI9_6(%rip), %xmm0, %xmm0    # xmm0 = xmm0[0,8,4,12,1,9,5,13,2,10,6,14,3,11,7,15]
	vpaddb	.LCPI9_7(%rip), %xmm0, %xmm0
	vmovdqu	%xmm0, (%rsi)
	retq
.Lfunc_end9:
	.size	_Z22to_string_sse2__pshufbmPc, .Lfunc_end9-_Z22to_string_sse2__pshufbmPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _Z14to_string_avx2mPc
.LCPI10_0:
	.quad	3518437209                      # 0xd1b71759
.LCPI10_1:
	.quad	10000                           # 0x2710
	.text
	.globl	_Z14to_string_avx2mPc
	.p2align	4, 0x90
	.type	_Z14to_string_avx2mPc,@function
_Z14to_string_avx2mPc:                  # @_Z14to_string_avx2mPc
	.cfi_startproc
# %bb.0:
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	mulxq	%rax, %rax, %rax
	shrq	$26, %rax
	imulq	$100000000, %rax, %rcx          # imm = 0x5F5E100
	vmovd	%eax, %xmm0
	leaq	_ZZ23to_string_tree_bigtablemPcE8bigtable(%rip), %rax
	subq	%rcx, %rdi
	vmovq	%rdi, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vpmuludq	.LCPI10_0(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	.LCPI10_1(%rip){1to2}, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpxor	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpcmpeqd	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm1, (%rax,%xmm0,4), %xmm2
	vmovdqu	%xmm2, (%rsi)
	retq
.Lfunc_end10:
	.size	_Z14to_string_avx2mPc, .Lfunc_end10-_Z14to_string_avx2mPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          # -- Begin function _Z20to_string_avx512ifmamPc
.LCPI11_0:
	.long	27370496                        # 0x1a1a400
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
.LCPI11_1:
	.quad	4048780183313844224             # 0x3830282018100800
	.quad	8678551244950751296             # 0x7870686058504840
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
.LCPI11_2:
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
.LCPI11_3:
	.quad	10                              # 0xa
.LCPI11_4:
	.quad	48                              # 0x30
	.text
	.globl	_Z20to_string_avx512ifmamPc
	.p2align	4, 0x90
	.type	_Z20to_string_avx512ifmamPc,@function
_Z20to_string_avx512ifmamPc:            # @_Z20to_string_avx512ifmamPc
	.cfi_startproc
# %bb.0:
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	vmovdqa64	.LCPI11_2(%rip), %zmm4  # zmm4 = [45035996,450359963,4503599628,45035996274,450359962738,4503599627371,45035996273705,450359962737050]
	vpbroadcastq	.LCPI11_4(%rip), %zmm6  # zmm6 = [48,48,48,48,48,48,48,48]
	vmovdqa	.LCPI11_1(%rip), %xmm3          # xmm3 = [4048780183313844224,8678551244950751296]
	mulxq	%rax, %rax, %rax
	vpbroadcastq	%rax, %zmm0
	shrq	$26, %rax
	vpsrlq	$26, %zmm0, %zmm0
	vinserti128	$0, .LCPI11_0(%rip), %ymm0, %ymm2
	imulq	$100000000, %rax, %rax          # imm = 0x5F5E100
	subq	%rax, %rdi
	vpbroadcastq	%rdi, %zmm1
	vmovdqa64	%zmm2, %zmm5
	vpmadd52luq	%zmm4, %zmm0, %zmm5
	vpbroadcastq	.LCPI11_3(%rip), %zmm0  # zmm0 = [10,10,10,10,10,10,10,10]
	vpmadd52luq	%zmm4, %zmm1, %zmm2
	vmovdqa64	%zmm6, %zmm4
	vpmadd52huq	%zmm5, %zmm0, %zmm4
	vpmadd52huq	%zmm2, %zmm0, %zmm6
	vpermt2b	%zmm6, %zmm3, %zmm4
	vmovdqu	%xmm4, (%rsi)
	vzeroupper
	retq
.Lfunc_end11:
	.size	_Z20to_string_avx512ifmamPc, .Lfunc_end11-_Z20to_string_avx512ifmamPc
	.cfi_endproc
                                        # -- End function
	.globl	_Z23to_string_tree_bigtablemPc  # -- Begin function _Z23to_string_tree_bigtablemPc
	.p2align	4, 0x90
	.type	_Z23to_string_tree_bigtablemPc,@function
_Z23to_string_tree_bigtablemPc:         # @_Z23to_string_tree_bigtablemPc
	.cfi_startproc
# %bb.0:
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	movq	%rdx, %rcx
	movabsq	$1844674407370956, %r8          # imm = 0x68DB8BAC710CC
	leaq	_ZZ23to_string_tree_bigtablemPcE8bigtable(%rip), %r9
	mulxq	%rax, %rax, %rax
	shrq	$26, %rax
	imulq	$100000000, %rax, %rdi          # imm = 0x5F5E100
	subq	%rdi, %rcx
	movabsq	$2535301200456458803, %rdi      # imm = 0x232F33025BD42233
	mulxq	%rdi, %rdi, %rdi
	movq	%rax, %rdx
	mulxq	%r8, %rdx, %rdx
	shrq	$37, %rdi
	imull	$10000, %edx, %edx              # imm = 0x2710
	movl	(%r9,%rdi,4), %edi
	subl	%edx, %eax
	imulq	$109951163, %rcx, %rdx          # imm = 0x68DB8BB
	movl	%edi, (%rsi)
	shrq	$40, %rdx
	movl	(%r9,%rax,4), %eax
	imull	$10000, %edx, %r8d              # imm = 0x2710
	subl	%r8d, %ecx
	movl	%eax, 4(%rsi)
	movl	(%r9,%rdx,4), %eax
	movl	%eax, 8(%rsi)
	movl	(%r9,%rcx,4), %eax
	movl	%eax, 12(%rsi)
	retq
.Lfunc_end12:
	.size	_Z23to_string_tree_bigtablemPc, .Lfunc_end12-_Z23to_string_tree_bigtablemPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _Z16to_string_scalarmPc
.LCPI13_0:
	.quad	-42949672959999                 # 0xffffd8f000000001
.LCPI13_1:
	.quad	5243                            # 0x147b
.LCPI13_2:
	.quad	545460846719                    # 0x7f0000007f
.LCPI13_3:
	.quad	-6553599                        # 0xffffffffff9c0001
.LCPI13_4:
	.quad	103                             # 0x67
.LCPI13_5:
	.quad	4222189076152335                # 0xf000f000f000f
.LCPI13_6:
	.quad	-2559                           # 0xfffffffffffff601
.LCPI13_7:
	.quad	3472328296227680304             # 0x3030303030303030
	.text
	.globl	_Z16to_string_scalarmPc
	.p2align	4, 0x90
	.type	_Z16to_string_scalarmPc,@function
_Z16to_string_scalarmPc:                # @_Z16to_string_scalarmPc
	.cfi_startproc
# %bb.0:
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	mulxq	%rax, %rax, %rax
	shrq	$26, %rax
	imulq	$-100000000, %rax, %rcx         # imm = 0xFA0A1F00
	imulq	$109951163, %rax, %rdx          # imm = 0x68DB8BB
	vmovq	%rax, %xmm0
	addq	%rdi, %rcx
	imulq	$109951163, %rcx, %rdi          # imm = 0x68DB8BB
	vmovq	%rcx, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vmovq	%rdx, %xmm1
	vpsllq	$32, %xmm0, %xmm0
	vmovq	%rdi, %xmm2
	vpunpcklqdq	%xmm2, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm2[0]
	vpsrlq	$40, %xmm1, %xmm1
	vpmullq	.LCPI13_0(%rip){1to2}, %xmm1, %xmm1
	vpaddq	%xmm0, %xmm1, %xmm0
	vpsllq	$16, %xmm0, %xmm1
	vpmullq	.LCPI13_1(%rip){1to2}, %xmm0, %xmm0
	vpsrlq	$19, %xmm0, %xmm0
	vpandq	.LCPI13_2(%rip){1to2}, %xmm0, %xmm0
	vpmullq	.LCPI13_3(%rip){1to2}, %xmm0, %xmm0
	vpaddq	%xmm1, %xmm0, %xmm0
	vpsllq	$8, %xmm0, %xmm1
	vpmullq	.LCPI13_4(%rip){1to2}, %xmm0, %xmm0
	vpsrlq	$10, %xmm0, %xmm0
	vpandq	.LCPI13_5(%rip){1to2}, %xmm0, %xmm0
	vpmullq	.LCPI13_6(%rip){1to2}, %xmm0, %xmm0
	vpaddq	%xmm0, %xmm1, %xmm0
	vpaddq	.LCPI13_7(%rip){1to2}, %xmm0, %xmm0
	vmovdqu	%xmm0, (%rsi)
	retq
.Lfunc_end13:
	.size	_Z16to_string_scalarmPc, .Lfunc_end13-_Z16to_string_scalarmPc
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _Z19to_string_scalar_v2mPc
.LCPI14_0:
	.quad	4294957296                      # 0xffffd8f0
.LCPI14_1:
	.quad	5243                            # 0x147b
.LCPI14_2:
	.quad	545460846719                    # 0x7f0000007f
.LCPI14_3:
	.quad	65436                           # 0xff9c
.LCPI14_4:
	.quad	103                             # 0x67
.LCPI14_5:
	.quad	4222189076152335                # 0xf000f000f000f
.LCPI14_6:
	.quad	246                             # 0xf6
.LCPI14_7:
	.quad	3472328296227680304             # 0x3030303030303030
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0
.LCPI14_8:
	.byte	7                               # 0x7
	.byte	6                               # 0x6
	.byte	5                               # 0x5
	.byte	4                               # 0x4
	.byte	3                               # 0x3
	.byte	2                               # 0x2
	.byte	1                               # 0x1
	.byte	0                               # 0x0
	.byte	15                              # 0xf
	.byte	14                              # 0xe
	.byte	13                              # 0xd
	.byte	12                              # 0xc
	.byte	11                              # 0xb
	.byte	10                              # 0xa
	.byte	9                               # 0x9
	.byte	8                               # 0x8
	.text
	.globl	_Z19to_string_scalar_v2mPc
	.p2align	4, 0x90
	.type	_Z19to_string_scalar_v2mPc,@function
_Z19to_string_scalar_v2mPc:             # @_Z19to_string_scalar_v2mPc
	.cfi_startproc
# %bb.0:
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rdi, %rdx
	mulxq	%rax, %rax, %rax
	shrq	$26, %rax
	imulq	$-100000000, %rax, %rcx         # imm = 0xFA0A1F00
	imulq	$109951163, %rax, %rdx          # imm = 0x68DB8BB
	addq	%rdi, %rcx
	vmovq	%rdx, %xmm0
	imulq	$109951163, %rcx, %rdi          # imm = 0x68DB8BB
	vmovq	%rcx, %xmm2
	vmovq	%rdi, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vmovq	%rax, %xmm1
	vpsrlq	$40, %xmm0, %xmm0
	vpmuludq	.LCPI14_0(%rip){1to2}, %xmm0, %xmm0
	vpunpcklqdq	%xmm2, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm2[0]
	vpaddq	%xmm1, %xmm0, %xmm0
	vpmullq	.LCPI14_1(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$19, %xmm1, %xmm1
	vpandq	.LCPI14_2(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI14_3(%rip){1to2}, %xmm1, %xmm1
	vpaddq	%xmm0, %xmm1, %xmm0
	vpmullq	.LCPI14_4(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$10, %xmm1, %xmm1
	vpandq	.LCPI14_5(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI14_6(%rip){1to2}, %xmm1, %xmm1
	vpaddq	%xmm1, %xmm0, %xmm0
	vpaddq	.LCPI14_7(%rip){1to2}, %xmm0, %xmm0
	vpshufb	.LCPI14_8(%rip), %xmm0, %xmm0   # xmm0 = xmm0[7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8]
	vmovdqu	%xmm0, (%rsi)
	retq
.Lfunc_end14:
	.size	_Z19to_string_scalar_v2mPc, .Lfunc_end14-_Z19to_string_scalar_v2mPc
	.cfi_endproc
                                        # -- End function
	.globl	_Z23to_string_8digit_scalarmPc  # -- Begin function _Z23to_string_8digit_scalarmPc
	.p2align	4, 0x90
	.type	_Z23to_string_8digit_scalarmPc,@function
_Z23to_string_8digit_scalarmPc:         # @_Z23to_string_8digit_scalarmPc
	.cfi_startproc
# %bb.0:
	imulq	$109951163, %rdi, %rax          # imm = 0x68DB8BB
	movl	$4294957296, %ecx               # imm = 0xFFFFD8F0
	movabsq	$545460846719, %rdx             # imm = 0x7F0000007F
	shrq	$40, %rax
	imulq	%rax, %rcx
	addq	%rdi, %rcx
	imulq	$5243, %rcx, %rax               # imm = 0x147B
	shrq	$19, %rax
	andq	%rax, %rdx
	imulq	$65436, %rdx, %rax              # imm = 0xFF9C
	movabsq	$4222189076152335, %rdx         # imm = 0xF000F000F000F
	addq	%rcx, %rax
	imulq	$103, %rax, %rcx
	shrq	$10, %rcx
	andq	%rcx, %rdx
	imulq	$246, %rdx, %rcx
	movabsq	$3472328296227680304, %rdx      # imm = 0x3030303030303030
	addq	%rax, %rdx
	addq	%rcx, %rdx
	movbeq	%rdx, (%rsi)
	retq
.Lfunc_end15:
	.size	_Z23to_string_8digit_scalarmPc, .Lfunc_end15-_Z23to_string_8digit_scalarmPc
	.cfi_endproc
                                        # -- End function
	.globl	_Z7to_bcd8m                     # -- Begin function _Z7to_bcd8m
	.p2align	4, 0x90
	.type	_Z7to_bcd8m,@function
_Z7to_bcd8m:                            # @_Z7to_bcd8m
	.cfi_startproc
# %bb.0:
	imulq	$109951163, %rdi, %rax          # imm = 0x68DB8BB
	movl	$4294957296, %ecx               # imm = 0xFFFFD8F0
	movabsq	$545460846719, %rdx             # imm = 0x7F0000007F
	shrq	$40, %rax
	imulq	%rax, %rcx
	addq	%rdi, %rcx
	imulq	$5243, %rcx, %rax               # imm = 0x147B
	shrq	$19, %rax
	andq	%rax, %rdx
	imulq	$65436, %rdx, %rdx              # imm = 0xFF9C
	addq	%rcx, %rdx
	movabsq	$4222189076152335, %rcx         # imm = 0xF000F000F000F
	imulq	$103, %rdx, %rax
	shrq	$10, %rax
	andq	%rax, %rcx
	movabsq	$3472328296227680304, %rax      # imm = 0x3030303030303030
	imulq	$246, %rcx, %rcx
	addq	%rdx, %rax
	addq	%rcx, %rax
	bswapq	%rax
	retq
.Lfunc_end16:
	.size	_Z7to_bcd8m, .Lfunc_end16-_Z7to_bcd8m
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          # -- Begin function main
.LCPI17_0:
	.long	27370496                        # 0x1a1a400
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
.LCPI17_1:
	.quad	4048780183313844224             # 0x3830282018100800
	.quad	8678551244950751296             # 0x7870686058504840
.LCPI17_21:
	.byte	7                               # 0x7
	.byte	6                               # 0x6
	.byte	5                               # 0x5
	.byte	4                               # 0x4
	.byte	3                               # 0x3
	.byte	2                               # 0x2
	.byte	1                               # 0x1
	.byte	0                               # 0x0
	.byte	15                              # 0xf
	.byte	14                              # 0xe
	.byte	13                              # 0xd
	.byte	12                              # 0xc
	.byte	11                              # 0xb
	.byte	10                              # 0xa
	.byte	9                               # 0x9
	.byte	8                               # 0x8
.LCPI17_22:
	.zero	16,48
.LCPI17_23:
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
.LCPI17_24:
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
.LCPI17_25:
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
.LCPI17_26:
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
.LCPI17_27:
	.quad	10                              # 0xa
	.quad	4295380                         # 0x418ad4
.LCPI17_29:
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.zero	1
	.byte	48                              # 0x30
	.byte	48                              # 0x30
	.byte	48                              # 0x30
.LCPI17_30:
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
	.byte	48                              # 0x30
	.zero	1
	.zero	1
	.zero	1
.LCPI17_33:
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
	.short	5243                            # 0x147b
.LCPI17_34:
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
	.short	65436                           # 0xff9c
.LCPI17_35:
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
	.short	6554                            # 0x199a
.LCPI17_36:
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
	.short	246                             # 0xf6
.LCPI17_37:
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
	.short	65526                           # 0xfff6
.LCPI17_38:
	.byte	0                               # 0x0
	.byte	8                               # 0x8
	.byte	4                               # 0x4
	.byte	12                              # 0xc
	.byte	1                               # 0x1
	.byte	9                               # 0x9
	.byte	5                               # 0x5
	.byte	13                              # 0xd
	.byte	2                               # 0x2
	.byte	10                              # 0xa
	.byte	6                               # 0x6
	.byte	14                              # 0xe
	.byte	3                               # 0x3
	.byte	11                              # 0xb
	.byte	7                               # 0x7
	.byte	15                              # 0xf
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0
.LCPI17_2:
	.quad	10486                           # 0x28f6
.LCPI17_3:
	.quad	545460846719                    # 0x7f0000007f
.LCPI17_4:
	.quad	281474976710556                 # 0xffffffffff9c
.LCPI17_5:
	.quad	103                             # 0x67
.LCPI17_6:
	.quad	4222189076152335                # 0xf000f000f000f
.LCPI17_7:
	.quad	72057594037927926               # 0xfffffffffffff6
.LCPI17_8:
	.quad	3472328296227680304             # 0x3030303030303030
.LCPI17_9:
	.quad	-100000000                      # 0xfffffffffa0a1f00
.LCPI17_10:
	.quad	109951163                       # 0x68db8bb
.LCPI17_11:
	.quad	-42949672959999                 # 0xffffd8f000000001
.LCPI17_12:
	.quad	5243                            # 0x147b
.LCPI17_13:
	.quad	-6553599                        # 0xffffffffff9c0001
.LCPI17_14:
	.quad	-2559                           # 0xfffffffffffff601
.LCPI17_17:
	.quad	4294957296                      # 0xffffd8f0
.LCPI17_18:
	.quad	65436                           # 0xff9c
.LCPI17_19:
	.quad	246                             # 0xf6
.LCPI17_31:
	.quad	3518437209                      # 0xd1b71759
.LCPI17_32:
	.quad	10000                           # 0x2710
.LCPI17_40:
	.quad	10                              # 0xa
.LCPI17_41:
	.quad	48                              # 0x30
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
.LCPI17_15:
	.quad	0                               # 0x0
	.quad	8                               # 0x8
	.quad	1                               # 0x1
	.quad	9                               # 0x9
	.quad	2                               # 0x2
	.quad	10                              # 0xa
	.quad	3                               # 0x3
	.quad	11                              # 0xb
.LCPI17_16:
	.quad	4                               # 0x4
	.quad	12                              # 0xc
	.quad	5                               # 0x5
	.quad	13                              # 0xd
	.quad	6                               # 0x6
	.quad	14                              # 0xe
	.quad	7                               # 0x7
	.quad	15                              # 0xf
.LCPI17_20:
	.byte	7                               # 0x7
	.byte	6                               # 0x6
	.byte	5                               # 0x5
	.byte	4                               # 0x4
	.byte	3                               # 0x3
	.byte	2                               # 0x2
	.byte	1                               # 0x1
	.byte	0                               # 0x0
	.byte	15                              # 0xf
	.byte	14                              # 0xe
	.byte	13                              # 0xd
	.byte	12                              # 0xc
	.byte	11                              # 0xb
	.byte	10                              # 0xa
	.byte	9                               # 0x9
	.byte	8                               # 0x8
	.byte	7                               # 0x7
	.byte	6                               # 0x6
	.byte	5                               # 0x5
	.byte	4                               # 0x4
	.byte	3                               # 0x3
	.byte	2                               # 0x2
	.byte	1                               # 0x1
	.byte	0                               # 0x0
	.byte	15                              # 0xf
	.byte	14                              # 0xe
	.byte	13                              # 0xd
	.byte	12                              # 0xc
	.byte	11                              # 0xb
	.byte	10                              # 0xa
	.byte	9                               # 0x9
	.byte	8                               # 0x8
	.byte	7                               # 0x7
	.byte	6                               # 0x6
	.byte	5                               # 0x5
	.byte	4                               # 0x4
	.byte	3                               # 0x3
	.byte	2                               # 0x2
	.byte	1                               # 0x1
	.byte	0                               # 0x0
	.byte	15                              # 0xf
	.byte	14                              # 0xe
	.byte	13                              # 0xd
	.byte	12                              # 0xc
	.byte	11                              # 0xb
	.byte	10                              # 0xa
	.byte	9                               # 0x9
	.byte	8                               # 0x8
	.byte	7                               # 0x7
	.byte	6                               # 0x6
	.byte	5                               # 0x5
	.byte	4                               # 0x4
	.byte	3                               # 0x3
	.byte	2                               # 0x2
	.byte	1                               # 0x1
	.byte	0                               # 0x0
	.byte	15                              # 0xf
	.byte	14                              # 0xe
	.byte	13                              # 0xd
	.byte	12                              # 0xc
	.byte	11                              # 0xb
	.byte	10                              # 0xa
	.byte	9                               # 0x9
	.byte	8                               # 0x8
.LCPI17_28:
	.quad	0                               # 0x0
	.quad	1                               # 0x1
	.quad	2                               # 0x2
	.quad	8                               # 0x8
	.quad	9                               # 0x9
	.zero	8
	.zero	8
	.zero	8
.LCPI17_39:
	.quad	45035996                        # 0x2af31dc
	.quad	450359963                       # 0x1ad7f29b
	.quad	4503599628                      # 0x10c6f7a0c
	.quad	45035996274                     # 0xa7c5ac472
	.quad	450359962738                    # 0x68db8bac72
	.quad	4503599627371                   # 0x4189374bc6b
	.quad	45035996273705                  # 0x28f5c28f5c29
	.quad	450359962737050                 # 0x199999999999a
.LCPI17_43:
	.byte	48                              # 0x30
	.zero	1
.LCPI17_44:
	.short	5243                            # 0x147b
.LCPI17_45:
	.short	65436                           # 0xff9c
.LCPI17_46:
	.short	6554                            # 0x199a
.LCPI17_47:
	.short	246                             # 0xf6
.LCPI17_48:
	.short	65526                           # 0xfff6
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2, 0x0
.LCPI17_42:
	.zero	4,48
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, DW.ref.__gxx_personality_v0
	.cfi_lsda 27, .Lexception0
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
	subq	$440, %rsp                      # imm = 0x1B8
	.cfi_def_cfa_offset 496
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movl	$50000, %ebx                    # imm = 0xC350
	movabsq	$1152921504606846975, %r13      # imm = 0xFFFFFFFFFFFFFFF
	xorl	%eax, %eax
	movq	%rax, (%rsp)                    # 8-byte Spill
	xorl	%r12d, %r12d
	xorl	%r14d, %r14d
	jmp	.LBB17_1
	.p2align	4, 0x90
.LBB17_2:                               #   in Loop: Header=BB17_1 Depth=1
	movq	%r14, (%rcx)
	leaq	8(%rcx), %r14
	decq	%rbx
	je	.LBB17_3
.LBB17_1:                               # =>This Inner Loop Header: Depth=1
	callq	rand@PLT
	movq	%r14, %rcx
	movslq	%eax, %r14
	cmpq	%r12, %rcx
	jne	.LBB17_2
# %bb.9:                                #   in Loop: Header=BB17_1 Depth=1
	subq	(%rsp), %r12                    # 8-byte Folded Reload
	movabsq	$9223372036854775800, %rax      # imm = 0x7FFFFFFFFFFFFFF8
	cmpq	%rax, %r12
	je	.LBB17_10
# %bb.12:                               #   in Loop: Header=BB17_1 Depth=1
	movq	%r12, %rax
	sarq	$3, %rax
	cmpq	$1, %rax
	movq	%rax, %rcx
	adcq	$0, %rcx
	leaq	(%rcx,%rax), %rbp
	cmpq	%r13, %rbp
	jb	.LBB17_14
# %bb.13:                               #   in Loop: Header=BB17_1 Depth=1
	movq	%r13, %rbp
.LBB17_14:                              #   in Loop: Header=BB17_1 Depth=1
	addq	%rax, %rcx
	cmovbq	%r13, %rbp
	leaq	(,%rbp,8), %rdi
.Ltmp0:
	callq	_Znwm@PLT
.Ltmp1:
# %bb.15:                               #   in Loop: Header=BB17_1 Depth=1
	movq	%r14, (%rax,%r12)
	movq	(%rsp), %r14                    # 8-byte Reload
	movq	%rax, %r15
	testq	%r12, %r12
	jle	.LBB17_17
# %bb.16:                               #   in Loop: Header=BB17_1 Depth=1
	movq	%r15, %rdi
	movq	%r14, %rsi
	movq	%r12, %rdx
	callq	memcpy@PLT
.LBB17_17:                              #   in Loop: Header=BB17_1 Depth=1
	testq	%r14, %r14
	je	.LBB17_19
# %bb.18:                               #   in Loop: Header=BB17_1 Depth=1
	movq	%r14, %rdi
	callq	_ZdlPv@PLT
.LBB17_19:                              #   in Loop: Header=BB17_1 Depth=1
	addq	%r15, %r12
	movq	%r15, (%rsp)                    # 8-byte Spill
	movq	%r12, %rcx
	leaq	(%r15,%rbp,8), %r12
	leaq	8(%rcx), %r14
	decq	%rbx
	jne	.LBB17_1
.LBB17_3:
	movq	(%rsp), %r13                    # 8-byte Reload
	movq	%r14, %r15
	movq	%rcx, 56(%rsp)                  # 8-byte Spill
	movq	%r14, 48(%rsp)                  # 8-byte Spill
	subq	%r13, %r15
	leaq	(%r15,%r15), %rdi
.Ltmp3:
	callq	_Znam@PLT
.Ltmp4:
# %bb.4:
	movq	56(%rsp), %rbx                  # 8-byte Reload
	movq	%rax, %rbp
	addq	$16, %rax
	leaq	8(%r13), %rdx
	vinserti128	$0, .LCPI17_0(%rip), %ymm0, %ymm0
	vpbroadcastq	.LCPI17_40(%rip), %zmm4 # zmm4 = [10,10,10,10,10,10,10,10]
	vpbroadcastq	.LCPI17_41(%rip), %zmm3 # zmm3 = [48,48,48,48,48,48,48,48]
	vpbroadcastq	.LCPI17_31(%rip), %xmm2 # xmm2 = [3518437209,3518437209]
	vpbroadcastq	.LCPI17_32(%rip), %xmm1 # xmm1 = [10000,10000]
	movq	%r13, (%rsp)                    # 8-byte Spill
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	movq	%r15, 32(%rsp)                  # 8-byte Spill
	movq	%rbp, 8(%rsp)                   # 8-byte Spill
	movq	%rax, 136(%rsp)                 # 8-byte Spill
	movq	%rdx, 128(%rsp)                 # 8-byte Spill
	movq	%rbx, %rsi
	subq	%r13, %rsi
	movq	%rsi, %rax
	andq	$-8, %rax
	movq	%rsi, %rdx
	movq	%rsi, 40(%rsp)                  # 8-byte Spill
	shrq	$3, %rsi
	leaq	16(%rbp,%rax,2), %rcx
	leaq	8(%r13,%rax), %rax
	incq	%rsi
	movl	%esi, %edx
	andl	$7, %edx
	movq	%rsi, 88(%rsp)                  # 8-byte Spill
	vmovdqu64	%zmm0, 368(%rsp)        # 64-byte Spill
	vmovdqu64	%zmm4, 304(%rsp)        # 64-byte Spill
	vmovdqu64	%zmm3, 240(%rsp)        # 64-byte Spill
	vmovdqa	%xmm2, 176(%rsp)                # 16-byte Spill
	vmovdqa	%xmm1, 160(%rsp)                # 16-byte Spill
	cmpq	%rax, %rbp
	movq	%rdx, 96(%rsp)                  # 8-byte Spill
	setb	%al
	cmpq	%rcx, %r13
	setb	%cl
	xorl	%r12d, %r12d
	andb	%al, %cl
	movq	%rsi, %rax
	andq	$-8, %rax
	movb	%cl, 31(%rsp)                   # 1-byte Spill
	movq	%rax, %rcx
	movq	%rax, 216(%rsp)                 # 8-byte Spill
	leaq	(%r13,%rax,8), %rax
	shlq	$4, %rcx
	movabsq	$633825300114115, %r13          # imm = 0x24075F3DCEAC3
	addq	%rbp, %rcx
	movq	%rax, 200(%rsp)                 # 8-byte Spill
	movq	%rsi, %rax
                                        # kill: def $esi killed $esi killed $rsi def $rsi
	andl	$3, %esi
	movq	%rcx, 208(%rsp)                 # 8-byte Spill
	movq	%rsi, 144(%rsp)                 # 8-byte Spill
	.p2align	4, 0x90
.LBB17_5:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB17_7 Depth 2
                                        #       Child Loop BB17_26 Depth 3
                                        #       Child Loop BB17_29 Depth 3
                                        #     Child Loop BB17_44 Depth 2
                                        #       Child Loop BB17_48 Depth 3
                                        #       Child Loop BB17_51 Depth 3
                                        #       Child Loop BB17_54 Depth 3
                                        #     Child Loop BB17_67 Depth 2
                                        #       Child Loop BB17_71 Depth 3
                                        #       Child Loop BB17_75 Depth 3
                                        #       Child Loop BB17_78 Depth 3
                                        #     Child Loop BB17_91 Depth 2
                                        #       Child Loop BB17_97 Depth 3
                                        #     Child Loop BB17_110 Depth 2
                                        #       Child Loop BB17_116 Depth 3
                                        #     Child Loop BB17_129 Depth 2
                                        #       Child Loop BB17_135 Depth 3
                                        #     Child Loop BB17_148 Depth 2
                                        #       Child Loop BB17_152 Depth 3
                                        #       Child Loop BB17_155 Depth 3
                                        #     Child Loop BB17_168 Depth 2
                                        #       Child Loop BB17_172 Depth 3
                                        #       Child Loop BB17_175 Depth 3
                                        #     Child Loop BB17_188 Depth 2
                                        #       Child Loop BB17_192 Depth 3
                                        #       Child Loop BB17_195 Depth 3
                                        #     Child Loop BB17_208 Depth 2
                                        #       Child Loop BB17_212 Depth 3
                                        #       Child Loop BB17_215 Depth 3
                                        #     Child Loop BB17_228 Depth 2
                                        #       Child Loop BB17_232 Depth 3
                                        #       Child Loop BB17_235 Depth 3
                                        #     Child Loop BB17_248 Depth 2
                                        #       Child Loop BB17_252 Depth 3
                                        #       Child Loop BB17_255 Depth 3
                                        #     Child Loop BB17_268 Depth 2
                                        #       Child Loop BB17_272 Depth 3
                                        #       Child Loop BB17_275 Depth 3
.Ltmp6:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$11, %edx
	leaq	.L.str(%rip), %rsi
	vzeroupper
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp7:
# %bb.6:                                #   in Loop: Header=BB17_5 Depth=1
	movq	%r12, 224(%rsp)                 # 8-byte Spill
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	(%rsp), %rbp                    # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	xorl	%r15d, %r15d
	movq	%rax, 16(%rsp)                  # 8-byte Spill
	jmp	.LBB17_7
	.p2align	4, 0x90
.LBB17_8:                               #   in Loop: Header=BB17_7 Depth=2
	xorl	%eax, %eax
.LBB17_31:                              #   in Loop: Header=BB17_7 Depth=2
	sarq	$3, %rax
	addq	%rax, %r15
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	16(%rsp), %rax                  # 8-byte Folded Reload
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_32
.LBB17_7:                               #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_26 Depth 3
                                        #       Child Loop BB17_29 Depth 3
	cmpq	48(%rsp), %rbp                  # 8-byte Folded Reload
	je	.LBB17_8
# %bb.24:                               #   in Loop: Header=BB17_7 Depth=2
	movq	8(%rsp), %rcx                   # 8-byte Reload
	cmpq	$0, 96(%rsp)                    # 8-byte Folded Reload
	movabsq	$2535301200456458803, %r11      # imm = 0x232F33025BD42233
	movq	%rbp, %rsi
	je	.LBB17_27
# %bb.25:                               #   in Loop: Header=BB17_7 Depth=2
	movq	96(%rsp), %rdi                  # 8-byte Reload
	movq	8(%rsp), %rcx                   # 8-byte Reload
	movq	%rbp, %rsi
	.p2align	4, 0x90
.LBB17_26:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_7 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rsi), %rdx
	addq	$8, %rsi
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%r11, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$109951163, %r9, %r10           # imm = 0x68DB8BB
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$37, %r8
	shrq	$40, %r10
	vmovq	%r10, %xmm2
	subq	%rdx, %rax
	imull	$10000, %r10d, %edx             # imm = 0x2710
	vmovq	%rax, %xmm0
	subl	%edx, %r9d
	vmovd	%r9d, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vmovd	%r8d, %xmm1
	vpsllq	$32, %xmm0, %xmm0
	vpunpcklqdq	%xmm2, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm2[0]
	vpor	%xmm1, %xmm0, %xmm0
	vpmullq	.LCPI17_2(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$20, %xmm1, %xmm1
	vpandq	.LCPI17_3(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI17_4(%rip){1to2}, %xmm1, %xmm2
	vpaddq	%xmm0, %xmm2, %xmm0
	vpsllq	$16, %xmm0, %xmm0
	vpaddq	%xmm1, %xmm0, %xmm0
	vpmullq	.LCPI17_5(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$10, %xmm1, %xmm1
	vpandq	.LCPI17_6(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI17_7(%rip){1to2}, %xmm1, %xmm2
	vpaddq	%xmm0, %xmm2, %xmm0
	vpsllq	$8, %xmm0, %xmm0
	vpaddq	%xmm1, %xmm0, %xmm0
	vpaddq	.LCPI17_8(%rip){1to2}, %xmm0, %xmm0
	vmovdqu	%xmm0, (%rcx)
	addq	$16, %rcx
	decq	%rdi
	jne	.LBB17_26
.LBB17_27:                              #   in Loop: Header=BB17_7 Depth=2
	movq	32(%rsp), %rax                  # 8-byte Reload
	cmpq	$56, 40(%rsp)                   # 8-byte Folded Reload
	jb	.LBB17_31
# %bb.28:                               #   in Loop: Header=BB17_7 Depth=2
	movabsq	$2535301200456458803, %rbp      # imm = 0x232F33025BD42233
	xorl	%edi, %edi
	.p2align	4, 0x90
.LBB17_29:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_7 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rsi,%rdi), %rdx
	vpbroadcastq	.LCPI17_5(%rip), %xmm13 # xmm13 = [103,103]
	vpbroadcastq	.LCPI17_6(%rip), %xmm12 # xmm12 = [4222189076152335,4222189076152335]
	vpbroadcastq	.LCPI17_7(%rip), %xmm11 # xmm11 = [72057594037927926,72057594037927926]
	vpbroadcastq	.LCPI17_8(%rip), %xmm10 # xmm10 = [3472328296227680304,3472328296227680304]
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%rbp, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$37, %r8
	vmovd	%r8d, %xmm2
	subq	%rdx, %rax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	vmovq	%rax, %xmm1
	shrq	$40, %rdx
	imull	$10000, %edx, %r10d             # imm = 0x2710
	subl	%r10d, %r9d
	vmovd	%r9d, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vmovq	%rdx, %xmm1
	vpsllq	$32, %xmm0, %xmm0
	vpunpcklqdq	%xmm1, %xmm2, %xmm1     # xmm1 = xmm2[0],xmm1[0]
	vpor	%xmm1, %xmm0, %xmm3
	vpbroadcastq	.LCPI17_2(%rip), %xmm0  # xmm0 = [10486,10486]
	vpmullq	%xmm0, %xmm3, %xmm1
	vpsrlq	$20, %xmm1, %xmm2
	vpbroadcastq	.LCPI17_3(%rip), %xmm1  # xmm1 = [545460846719,545460846719]
	vpand	%xmm1, %xmm2, %xmm4
	vpbroadcastq	.LCPI17_4(%rip), %xmm2  # xmm2 = [281474976710556,281474976710556]
	vpmullq	%xmm2, %xmm4, %xmm5
	vpaddq	%xmm3, %xmm5, %xmm3
	vpsllq	$16, %xmm3, %xmm3
	vpaddq	%xmm4, %xmm3, %xmm6
	vpmullq	%xmm13, %xmm6, %xmm4
	vpsrlq	$10, %xmm4, %xmm5
	vpand	%xmm5, %xmm12, %xmm7
	vpmullq	%xmm11, %xmm7, %xmm8
	vpaddq	%xmm7, %xmm10, %xmm7
	vpaddq	%xmm6, %xmm8, %xmm6
	vpsllq	$8, %xmm6, %xmm8
	vpaddq	%xmm7, %xmm8, %xmm7
	vmovdqu	%xmm7, (%rcx,%rdi,2)
	movq	8(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%rbp, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$37, %r8
	vmovd	%r8d, %xmm9
	subq	%rdx, %rax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	vmovq	%rax, %xmm8
	shrq	$40, %rdx
	imull	$10000, %edx, %r10d             # imm = 0x2710
	subl	%r10d, %r9d
	vmovd	%r9d, %xmm7
	vpunpcklqdq	%xmm7, %xmm8, %xmm7     # xmm7 = xmm8[0],xmm7[0]
	vmovq	%rdx, %xmm8
	vpsllq	$32, %xmm7, %xmm7
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vpor	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm0, %xmm7, %xmm8
	vpsrlq	$20, %xmm8, %xmm8
	vpand	%xmm1, %xmm8, %xmm8
	vpmullq	%xmm2, %xmm8, %xmm9
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$16, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm13, %xmm7, %xmm8
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm12, %xmm8, %xmm8
	vpmullq	%xmm11, %xmm8, %xmm9
	vpaddq	%xmm10, %xmm8, %xmm8
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$8, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vmovdqu	%xmm7, 16(%rcx,%rdi,2)
	movq	16(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	mulxq	%rbp, %r10, %r10
	movq	%rdx, %r9
	shrq	$26, %rax
	shrq	$37, %r10
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	vmovd	%r10d, %xmm9
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	subq	%r8, %r9
	subq	%rdx, %rax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	vmovq	%rax, %xmm8
	shrq	$40, %rdx
	imull	$10000, %edx, %r8d              # imm = 0x2710
	subl	%r8d, %r9d
	vmovd	%r9d, %xmm7
	vpunpcklqdq	%xmm7, %xmm8, %xmm7     # xmm7 = xmm8[0],xmm7[0]
	vmovq	%rdx, %xmm8
	vpsllq	$32, %xmm7, %xmm7
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vpor	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm0, %xmm7, %xmm8
	vpsrlq	$20, %xmm8, %xmm8
	vpand	%xmm1, %xmm8, %xmm8
	vpmullq	%xmm2, %xmm8, %xmm9
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$16, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm13, %xmm7, %xmm8
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm12, %xmm8, %xmm8
	vpmullq	%xmm11, %xmm8, %xmm9
	vpaddq	%xmm10, %xmm8, %xmm8
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$8, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vmovdqu	%xmm7, 32(%rcx,%rdi,2)
	movq	24(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%rbp, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$37, %r8
	vmovd	%r8d, %xmm9
	subq	%rdx, %rax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	vmovq	%rax, %xmm8
	shrq	$40, %rdx
	imull	$10000, %edx, %r10d             # imm = 0x2710
	subl	%r10d, %r9d
	vmovd	%r9d, %xmm7
	vpunpcklqdq	%xmm7, %xmm8, %xmm7     # xmm7 = xmm8[0],xmm7[0]
	vmovq	%rdx, %xmm8
	vpsllq	$32, %xmm7, %xmm7
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vpor	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm0, %xmm7, %xmm8
	vpsrlq	$20, %xmm8, %xmm8
	vpand	%xmm1, %xmm8, %xmm8
	vpmullq	%xmm2, %xmm8, %xmm9
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$16, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm13, %xmm7, %xmm8
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm12, %xmm8, %xmm8
	vpmullq	%xmm11, %xmm8, %xmm9
	vpaddq	%xmm10, %xmm8, %xmm8
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$8, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vmovdqu	%xmm7, 48(%rcx,%rdi,2)
	movq	32(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	mulxq	%rbp, %r10, %r10
	movq	%rdx, %r9
	shrq	$26, %rax
	shrq	$37, %r10
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	vmovd	%r10d, %xmm9
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	subq	%r8, %r9
	subq	%rdx, %rax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	vmovq	%rax, %xmm8
	shrq	$40, %rdx
	imull	$10000, %edx, %r8d              # imm = 0x2710
	subl	%r8d, %r9d
	vmovd	%r9d, %xmm7
	vpunpcklqdq	%xmm7, %xmm8, %xmm7     # xmm7 = xmm8[0],xmm7[0]
	vmovq	%rdx, %xmm8
	vpsllq	$32, %xmm7, %xmm7
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vpor	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm0, %xmm7, %xmm8
	vpsrlq	$20, %xmm8, %xmm8
	vpand	%xmm1, %xmm8, %xmm8
	vpmullq	%xmm2, %xmm8, %xmm9
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$16, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm13, %xmm7, %xmm8
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm12, %xmm8, %xmm8
	vpmullq	%xmm11, %xmm8, %xmm9
	vpaddq	%xmm10, %xmm8, %xmm8
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$8, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vmovdqu	%xmm7, 64(%rcx,%rdi,2)
	movq	40(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%rbp, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$37, %r8
	vmovd	%r8d, %xmm9
	subq	%rdx, %rax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	vmovq	%rax, %xmm8
	shrq	$40, %rdx
	imull	$10000, %edx, %r10d             # imm = 0x2710
	subl	%r10d, %r9d
	vmovd	%r9d, %xmm7
	vpunpcklqdq	%xmm7, %xmm8, %xmm7     # xmm7 = xmm8[0],xmm7[0]
	vmovq	%rdx, %xmm8
	vpsllq	$32, %xmm7, %xmm7
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vpor	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm0, %xmm7, %xmm8
	vpsrlq	$20, %xmm8, %xmm8
	vpand	%xmm1, %xmm8, %xmm8
	vpmullq	%xmm2, %xmm8, %xmm9
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$16, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm13, %xmm7, %xmm8
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm12, %xmm8, %xmm8
	vpmullq	%xmm11, %xmm8, %xmm9
	vpaddq	%xmm10, %xmm8, %xmm8
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$8, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vmovdqu	%xmm7, 80(%rcx,%rdi,2)
	movq	48(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	mulxq	%rbp, %r10, %r10
	movq	%rdx, %r9
	shrq	$26, %rax
	shrq	$37, %r10
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	vmovd	%r10d, %xmm9
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	subq	%r8, %r9
	subq	%rdx, %rax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	vmovq	%rax, %xmm8
	shrq	$40, %rdx
	imull	$10000, %edx, %r8d              # imm = 0x2710
	subl	%r8d, %r9d
	leaq	56(%rsi,%rdi), %r8
	vmovd	%r9d, %xmm7
	vpunpcklqdq	%xmm7, %xmm8, %xmm7     # xmm7 = xmm8[0],xmm7[0]
	vmovq	%rdx, %xmm8
	vpsllq	$32, %xmm7, %xmm7
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vpor	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm0, %xmm7, %xmm8
	vpsrlq	$20, %xmm8, %xmm8
	vpand	%xmm1, %xmm8, %xmm8
	vpmullq	%xmm2, %xmm8, %xmm9
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$16, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm13, %xmm7, %xmm8
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm12, %xmm8, %xmm8
	vpmullq	%xmm11, %xmm8, %xmm9
	vpaddq	%xmm10, %xmm8, %xmm8
	vpaddq	%xmm7, %xmm9, %xmm7
	vpsllq	$8, %xmm7, %xmm7
	vpaddq	%xmm7, %xmm8, %xmm7
	vmovdqu	%xmm7, 96(%rcx,%rdi,2)
	movq	56(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r10
	shrq	$26, %rax
	imulq	$100000000, %rax, %r9           # imm = 0x5F5E100
	subq	%r9, %r10
	mulxq	%rbp, %r9, %r9
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$37, %r9
	vmovd	%r9d, %xmm9
	subq	%rdx, %rax
	imulq	$109951163, %r10, %rdx          # imm = 0x68DB8BB
	vmovq	%rax, %xmm8
	shrq	$40, %rdx
	imull	$10000, %edx, %r11d             # imm = 0x2710
	subl	%r11d, %r10d
	vmovd	%r10d, %xmm7
	vpunpcklqdq	%xmm7, %xmm8, %xmm7     # xmm7 = xmm8[0],xmm7[0]
	vmovq	%rdx, %xmm8
	vpsllq	$32, %xmm7, %xmm7
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vpor	%xmm7, %xmm8, %xmm7
	vpmullq	%xmm0, %xmm7, %xmm0
	vpsrlq	$20, %xmm0, %xmm0
	vpand	%xmm1, %xmm0, %xmm0
	vpmullq	%xmm2, %xmm0, %xmm1
	vpaddq	%xmm7, %xmm1, %xmm1
	vpsllq	$16, %xmm1, %xmm1
	vpaddq	%xmm0, %xmm1, %xmm0
	vpmullq	%xmm13, %xmm0, %xmm1
	vpsrlq	$10, %xmm1, %xmm1
	vpand	%xmm1, %xmm12, %xmm1
	vpmullq	%xmm11, %xmm1, %xmm2
	vpaddq	%xmm1, %xmm10, %xmm1
	vpaddq	%xmm0, %xmm2, %xmm0
	vpsllq	$8, %xmm0, %xmm0
	vpaddq	%xmm1, %xmm0, %xmm0
	vmovdqu	%xmm0, 112(%rcx,%rdi,2)
	addq	$64, %rdi
	cmpq	%rbx, %r8
	jne	.LBB17_29
# %bb.30:                               #   in Loop: Header=BB17_7 Depth=2
	movq	32(%rsp), %rax                  # 8-byte Reload
	movq	(%rsp), %rbp                    # 8-byte Reload
	jmp	.LBB17_31
	.p2align	4, 0x90
.LBB17_32:                              #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm10, %xmm0
	vcvtusi2sd	%r15, %xmm10, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp8:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp9:
# %bb.33:                               #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.36:                               #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_38
# %bb.37:                               #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_40
	.p2align	4, 0x90
.LBB17_38:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp10:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp11:
# %bb.39:                               #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp12:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp13:
.LBB17_40:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp14:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp15:
# %bb.41:                               #   in Loop: Header=BB17_5 Depth=1
.Ltmp16:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp17:
# %bb.42:                               #   in Loop: Header=BB17_5 Depth=1
.Ltmp18:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$8, %edx
	leaq	.L.str.1(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp19:
# %bb.43:                               #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	movq	%rax, %rbp
	xorl	%ebx, %ebx
	jmp	.LBB17_44
	.p2align	4, 0x90
.LBB17_45:                              #   in Loop: Header=BB17_44 Depth=2
	xorl	%edx, %edx
.LBB17_56:                              #   in Loop: Header=BB17_44 Depth=2
	sarq	$3, %rdx
	addq	%rdx, %rbx
	vzeroupper
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	%rbp, %rax
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_57
.LBB17_44:                              #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_48 Depth 3
                                        #       Child Loop BB17_51 Depth 3
                                        #       Child Loop BB17_54 Depth 3
	movq	(%rsp), %rcx                    # 8-byte Reload
	cmpq	48(%rsp), %rcx                  # 8-byte Folded Reload
	je	.LBB17_45
# %bb.46:                               #   in Loop: Header=BB17_44 Depth=2
	cmpq	$56, 40(%rsp)                   # 8-byte Folded Reload
	setb	%al
	orb	31(%rsp), %al                   # 1-byte Folded Reload
	movq	8(%rsp), %rax                   # 8-byte Reload
	jne	.LBB17_50
# %bb.47:                               #   in Loop: Header=BB17_44 Depth=2
	vmovdqa64	.LCPI17_15(%rip), %zmm7 # zmm7 = [0,8,1,9,2,10,3,11]
	vmovdqa64	.LCPI17_16(%rip), %zmm8 # zmm8 = [4,12,5,13,6,14,7,15]
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	216(%rsp), %rsi                 # 8-byte Reload
	movq	(%rsp), %rdi                    # 8-byte Reload
	xorl	%ecx, %ecx
	.p2align	4, 0x90
.LBB17_48:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_44 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	%r12, %rdx
	mulxq	56(%rdi,%rcx,8), %rdx, %rdx
	vpbroadcastq	.LCPI17_10(%rip), %zmm6 # zmm6 = [109951163,109951163,109951163,109951163,109951163,109951163,109951163,109951163]
	vpbroadcastq	.LCPI17_11(%rip), %zmm4 # zmm4 = [18446701124036591617,18446701124036591617,18446701124036591617,18446701124036591617,18446701124036591617,18446701124036591617,18446701124036591617,18446701124036591617]
	vpbroadcastq	.LCPI17_3(%rip), %zmm9  # zmm9 = [545460846719,545460846719,545460846719,545460846719,545460846719,545460846719,545460846719,545460846719]
	vpbroadcastq	.LCPI17_14(%rip), %zmm10 # zmm10 = [18446744073709549057,18446744073709549057,18446744073709549057,18446744073709549057,18446744073709549057,18446744073709549057,18446744073709549057,18446744073709549057]
	vmovq	%rdx, %xmm0
	movq	%r12, %rdx
	mulxq	48(%rdi,%rcx,8), %rdx, %rdx
	vmovq	%rdx, %xmm1
	movq	%r12, %rdx
	mulxq	40(%rdi,%rcx,8), %rdx, %rdx
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vmovq	%rdx, %xmm1
	movq	%r12, %rdx
	mulxq	32(%rdi,%rcx,8), %rdx, %rdx
	vmovq	%rdx, %xmm2
	movq	%r12, %rdx
	mulxq	24(%rdi,%rcx,8), %rdx, %rdx
	vpunpcklqdq	%xmm1, %xmm2, %xmm1     # xmm1 = xmm2[0],xmm1[0]
	vinserti128	$1, %xmm0, %ymm1, %ymm0
	vmovq	%rdx, %xmm1
	movq	%r12, %rdx
	mulxq	16(%rdi,%rcx,8), %rdx, %rdx
	vmovq	%rdx, %xmm2
	movq	%r12, %rdx
	mulxq	8(%rdi,%rcx,8), %rdx, %rdx
	vpunpcklqdq	%xmm1, %xmm2, %xmm1     # xmm1 = xmm2[0],xmm1[0]
	vmovq	%rdx, %xmm2
	movq	%r12, %rdx
	mulxq	(%rdi,%rcx,8), %rdx, %rdx
	vmovq	%rdx, %xmm3
	vpunpcklqdq	%xmm2, %xmm3, %xmm2     # xmm2 = xmm3[0],xmm2[0]
	vpbroadcastq	.LCPI17_12(%rip), %zmm3 # zmm3 = [5243,5243,5243,5243,5243,5243,5243,5243]
	vinserti128	$1, %xmm1, %ymm2, %ymm1
	vinserti64x4	$1, %ymm0, %zmm1, %zmm0
	vpsrlq	$26, %zmm0, %zmm0
	vpmullq	.LCPI17_9(%rip){1to8}, %zmm0, %zmm1
	vpsllq	$32, %zmm0, %zmm2
	vpmullq	%zmm6, %zmm0, %zmm0
	vpaddq	(%rdi,%rcx,8), %zmm1, %zmm1
	addq	$8, %rcx
	vpsrlq	$40, %zmm0, %zmm0
	vpmullq	%zmm4, %zmm0, %zmm0
	vpaddq	%zmm2, %zmm0, %zmm0
	vpsllq	$16, %zmm0, %zmm2
	vpmullq	%zmm3, %zmm0, %zmm0
	vpsllq	$32, %zmm1, %zmm5
	vpmullq	%zmm6, %zmm1, %zmm1
	vpsrlq	$19, %zmm0, %zmm0
	vpandq	%zmm9, %zmm0, %zmm0
	vpsrlq	$40, %zmm1, %zmm1
	vpmullq	%zmm4, %zmm1, %zmm1
	vpbroadcastq	.LCPI17_6(%rip), %zmm4  # zmm4 = [4222189076152335,4222189076152335,4222189076152335,4222189076152335,4222189076152335,4222189076152335,4222189076152335,4222189076152335]
	vpaddq	%zmm5, %zmm1, %zmm1
	vpbroadcastq	.LCPI17_13(%rip), %zmm5 # zmm5 = [18446744073702998017,18446744073702998017,18446744073702998017,18446744073702998017,18446744073702998017,18446744073702998017,18446744073702998017,18446744073702998017]
	vpsllq	$16, %zmm1, %zmm6
	vpmullq	%zmm3, %zmm1, %zmm1
	vpbroadcastq	.LCPI17_5(%rip), %zmm3  # zmm3 = [103,103,103,103,103,103,103,103]
	vpsrlq	$19, %zmm1, %zmm1
	vpandq	%zmm9, %zmm1, %zmm1
	vpmullq	%zmm5, %zmm0, %zmm0
	vpmullq	%zmm5, %zmm1, %zmm1
	vpaddq	%zmm2, %zmm0, %zmm0
	vpaddq	%zmm6, %zmm1, %zmm1
	vpbroadcastq	.LCPI17_8(%rip), %zmm6  # zmm6 = [3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304]
	vpsllq	$8, %zmm0, %zmm2
	vpmullq	%zmm3, %zmm0, %zmm0
	vpsllq	$8, %zmm1, %zmm9
	vpmullq	%zmm3, %zmm1, %zmm1
	vpsrlq	$10, %zmm0, %zmm0
	vpsrlq	$10, %zmm1, %zmm1
	vpandq	%zmm4, %zmm0, %zmm0
	vpandq	%zmm4, %zmm1, %zmm1
	vpmullq	%zmm10, %zmm0, %zmm0
	vpmullq	%zmm10, %zmm1, %zmm1
	vpaddq	%zmm6, %zmm2, %zmm2
	vpaddq	%zmm0, %zmm2, %zmm0
	vpaddq	%zmm6, %zmm9, %zmm2
	vpaddq	%zmm1, %zmm2, %zmm1
	vmovdqa64	%zmm0, %zmm2
	vpermt2q	%zmm1, %zmm7, %zmm2
	vpermt2q	%zmm1, %zmm8, %zmm0
	vmovdqu64	%zmm0, 64(%rax)
	vmovdqu64	%zmm2, (%rax)
	subq	$-128, %rax
	cmpq	%rcx, %rsi
	jne	.LBB17_48
# %bb.49:                               #   in Loop: Header=BB17_44 Depth=2
	movq	208(%rsp), %rax                 # 8-byte Reload
	movq	200(%rsp), %rcx                 # 8-byte Reload
	movq	%r15, %rdx
	cmpq	%rsi, 88(%rsp)                  # 8-byte Folded Reload
	je	.LBB17_56
.LBB17_50:                              #   in Loop: Header=BB17_44 Depth=2
	movq	56(%rsp), %rsi                  # 8-byte Reload
	subq	%rcx, %rsi
	movl	%esi, %edi
	shrl	$3, %edi
	incl	%edi
	andl	$7, %edi
	je	.LBB17_52
	.p2align	4, 0x90
.LBB17_51:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_44 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx), %rdx
	addq	$8, %rcx
	mulxq	%r12, %r8, %r8
	shrq	$26, %r8
	imulq	$-100000000, %r8, %r9           # imm = 0xFA0A1F00
	vmovq	%r8, %xmm0
	addq	%rdx, %r9
	imulq	$109951163, %r8, %rdx           # imm = 0x68DB8BB
	imulq	$109951163, %r9, %r10           # imm = 0x68DB8BB
	vmovq	%r9, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vmovq	%rdx, %xmm1
	vpsllq	$32, %xmm0, %xmm0
	vmovq	%r10, %xmm2
	vpunpcklqdq	%xmm2, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm2[0]
	vpsrlq	$40, %xmm1, %xmm1
	vpmullq	.LCPI17_11(%rip){1to2}, %xmm1, %xmm1
	vpaddq	%xmm0, %xmm1, %xmm0
	vpmullq	.LCPI17_12(%rip){1to2}, %xmm0, %xmm1
	vpsllq	$16, %xmm0, %xmm0
	vpsrlq	$19, %xmm1, %xmm1
	vpandq	.LCPI17_3(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI17_13(%rip){1to2}, %xmm1, %xmm1
	vpaddq	%xmm0, %xmm1, %xmm0
	vpmullq	.LCPI17_5(%rip){1to2}, %xmm0, %xmm1
	vpsllq	$8, %xmm0, %xmm0
	vpsrlq	$10, %xmm1, %xmm1
	vpandq	.LCPI17_6(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI17_14(%rip){1to2}, %xmm1, %xmm1
	vpaddq	%xmm1, %xmm0, %xmm0
	vpaddq	.LCPI17_8(%rip){1to2}, %xmm0, %xmm0
	vmovdqu	%xmm0, (%rax)
	addq	$16, %rax
	decq	%rdi
	jne	.LBB17_51
.LBB17_52:                              #   in Loop: Header=BB17_44 Depth=2
	movq	%r15, %rdx
	cmpq	$56, %rsi
	jb	.LBB17_56
# %bb.53:                               #   in Loop: Header=BB17_44 Depth=2
	movq	56(%rsp), %r11                  # 8-byte Reload
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB17_54:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_44 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx,%rsi), %rdx
	vpbroadcastq	.LCPI17_3(%rip), %xmm16 # xmm16 = [545460846719,545460846719]
	vpbroadcastq	.LCPI17_13(%rip), %xmm15 # xmm15 = [18446744073702998017,18446744073702998017]
	vpbroadcastq	.LCPI17_5(%rip), %xmm14 # xmm14 = [103,103]
	vpbroadcastq	.LCPI17_6(%rip), %xmm13 # xmm13 = [4222189076152335,4222189076152335]
	vpbroadcastq	.LCPI17_14(%rip), %xmm12 # xmm12 = [18446744073709549057,18446744073709549057]
	vpbroadcastq	.LCPI17_8(%rip), %xmm11 # xmm11 = [3472328296227680304,3472328296227680304]
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm1
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%r8, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpsllq	$32, %xmm0, %xmm1
	vmovq	%rdx, %xmm2
	vmovq	%r9, %xmm0
	vpunpcklqdq	%xmm0, %xmm2, %xmm0     # xmm0 = xmm2[0],xmm0[0]
	vpsrlq	$40, %xmm0, %xmm2
	vpbroadcastq	.LCPI17_11(%rip), %xmm0 # xmm0 = [18446701124036591617,18446701124036591617]
	vpmullq	%xmm0, %xmm2, %xmm2
	vpaddq	%xmm1, %xmm2, %xmm2
	vpbroadcastq	.LCPI17_12(%rip), %xmm1 # xmm1 = [5243,5243]
	vpsllq	$16, %xmm2, %xmm4
	vpmullq	%xmm1, %xmm2, %xmm2
	vpsrlq	$19, %xmm2, %xmm3
	vpandq	%xmm16, %xmm3, %xmm5
	vpmullq	%xmm15, %xmm5, %xmm5
	vpaddq	%xmm4, %xmm5, %xmm5
	vpsllq	$8, %xmm5, %xmm8
	vpmullq	%xmm14, %xmm5, %xmm5
	vpaddq	%xmm11, %xmm8, %xmm8
	vpsrlq	$10, %xmm5, %xmm6
	vpand	%xmm6, %xmm13, %xmm7
	vpmullq	%xmm12, %xmm7, %xmm9
	vpaddq	%xmm9, %xmm8, %xmm8
	vmovdqu	%xmm8, (%rax,%rsi,2)
	movq	8(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm9
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%r8, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%rdx, %xmm10
	vpsllq	$32, %xmm8, %xmm8
	vmovq	%r9, %xmm9
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpsrlq	$40, %xmm9, %xmm9
	vpmullq	%xmm0, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpsllq	$16, %xmm8, %xmm9
	vpmullq	%xmm1, %xmm8, %xmm8
	vpsrlq	$19, %xmm8, %xmm8
	vpandq	%xmm16, %xmm8, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpsllq	$8, %xmm8, %xmm9
	vpmullq	%xmm14, %xmm8, %xmm8
	vpaddq	%xmm11, %xmm9, %xmm9
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm13, %xmm8, %xmm8
	vpmullq	%xmm12, %xmm8, %xmm8
	vpaddq	%xmm8, %xmm9, %xmm8
	vmovdqu	%xmm8, 16(%rax,%rsi,2)
	movq	16(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm9
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%r8, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%rdx, %xmm10
	vpsllq	$32, %xmm8, %xmm8
	vmovq	%r9, %xmm9
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpsrlq	$40, %xmm9, %xmm9
	vpmullq	%xmm0, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpsllq	$16, %xmm8, %xmm9
	vpmullq	%xmm1, %xmm8, %xmm8
	vpsrlq	$19, %xmm8, %xmm8
	vpandq	%xmm16, %xmm8, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpsllq	$8, %xmm8, %xmm9
	vpmullq	%xmm14, %xmm8, %xmm8
	vpaddq	%xmm11, %xmm9, %xmm9
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm13, %xmm8, %xmm8
	vpmullq	%xmm12, %xmm8, %xmm8
	vpaddq	%xmm8, %xmm9, %xmm8
	vmovdqu	%xmm8, 32(%rax,%rsi,2)
	movq	24(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm9
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%r8, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%rdx, %xmm10
	vpsllq	$32, %xmm8, %xmm8
	vmovq	%r9, %xmm9
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpsrlq	$40, %xmm9, %xmm9
	vpmullq	%xmm0, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpsllq	$16, %xmm8, %xmm9
	vpmullq	%xmm1, %xmm8, %xmm8
	vpsrlq	$19, %xmm8, %xmm8
	vpandq	%xmm16, %xmm8, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpsllq	$8, %xmm8, %xmm9
	vpmullq	%xmm14, %xmm8, %xmm8
	vpaddq	%xmm11, %xmm9, %xmm9
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm13, %xmm8, %xmm8
	vpmullq	%xmm12, %xmm8, %xmm8
	vpaddq	%xmm8, %xmm9, %xmm8
	vmovdqu	%xmm8, 48(%rax,%rsi,2)
	movq	32(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm9
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%r8, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%rdx, %xmm10
	vpsllq	$32, %xmm8, %xmm8
	vmovq	%r9, %xmm9
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpsrlq	$40, %xmm9, %xmm9
	vpmullq	%xmm0, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpsllq	$16, %xmm8, %xmm9
	vpmullq	%xmm1, %xmm8, %xmm8
	vpsrlq	$19, %xmm8, %xmm8
	vpandq	%xmm16, %xmm8, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpsllq	$8, %xmm8, %xmm9
	vpmullq	%xmm14, %xmm8, %xmm8
	vpaddq	%xmm11, %xmm9, %xmm9
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm13, %xmm8, %xmm8
	vpmullq	%xmm12, %xmm8, %xmm8
	vpaddq	%xmm8, %xmm9, %xmm8
	vmovdqu	%xmm8, 64(%rax,%rsi,2)
	movq	40(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm9
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%r8, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%rdx, %xmm10
	vpsllq	$32, %xmm8, %xmm8
	vmovq	%r9, %xmm9
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpsrlq	$40, %xmm9, %xmm9
	vpmullq	%xmm0, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpsllq	$16, %xmm8, %xmm9
	vpmullq	%xmm1, %xmm8, %xmm8
	vpsrlq	$19, %xmm8, %xmm8
	vpandq	%xmm16, %xmm8, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpsllq	$8, %xmm8, %xmm9
	vpmullq	%xmm14, %xmm8, %xmm8
	vpaddq	%xmm11, %xmm9, %xmm9
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm13, %xmm8, %xmm8
	vpmullq	%xmm12, %xmm8, %xmm8
	vpaddq	%xmm8, %xmm9, %xmm8
	vmovdqu	%xmm8, 80(%rax,%rsi,2)
	movq	48(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm9
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	leaq	56(%rcx,%rsi), %rdi
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%r8, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%rdx, %xmm10
	vpsllq	$32, %xmm8, %xmm8
	vmovq	%r9, %xmm9
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpsrlq	$40, %xmm9, %xmm9
	vpmullq	%xmm0, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpsllq	$16, %xmm8, %xmm9
	vpmullq	%xmm1, %xmm8, %xmm8
	vpsrlq	$19, %xmm8, %xmm8
	vpandq	%xmm16, %xmm8, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpsllq	$8, %xmm8, %xmm9
	vpmullq	%xmm14, %xmm8, %xmm8
	vpaddq	%xmm11, %xmm9, %xmm9
	vpsrlq	$10, %xmm8, %xmm8
	vpand	%xmm13, %xmm8, %xmm8
	vpmullq	%xmm12, %xmm8, %xmm8
	vpaddq	%xmm8, %xmm9, %xmm8
	vmovdqu	%xmm8, 96(%rax,%rsi,2)
	movq	56(%rcx,%rsi), %rdx
	mulxq	%r12, %r8, %r8
	shrq	$26, %r8
	imulq	$-100000000, %r8, %r9           # imm = 0xFA0A1F00
	vmovq	%r8, %xmm9
	addq	%rdx, %r9
	imulq	$109951163, %r8, %rdx           # imm = 0x68DB8BB
	imulq	$109951163, %r9, %r10           # imm = 0x68DB8BB
	vmovq	%r9, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%rdx, %xmm10
	vpsllq	$32, %xmm8, %xmm8
	vmovq	%r10, %xmm9
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpsrlq	$40, %xmm9, %xmm9
	vpmullq	%xmm0, %xmm9, %xmm0
	vpaddq	%xmm0, %xmm8, %xmm0
	vpsllq	$16, %xmm0, %xmm8
	vpmullq	%xmm1, %xmm0, %xmm0
	vpsrlq	$19, %xmm0, %xmm0
	vpandq	%xmm16, %xmm0, %xmm0
	vpmullq	%xmm15, %xmm0, %xmm0
	vpaddq	%xmm0, %xmm8, %xmm0
	vpsllq	$8, %xmm0, %xmm1
	vpmullq	%xmm14, %xmm0, %xmm0
	vpaddq	%xmm1, %xmm11, %xmm1
	vpsrlq	$10, %xmm0, %xmm0
	vpand	%xmm0, %xmm13, %xmm0
	vpmullq	%xmm12, %xmm0, %xmm0
	vpaddq	%xmm0, %xmm1, %xmm0
	vmovdqu	%xmm0, 112(%rax,%rsi,2)
	addq	$64, %rsi
	cmpq	%r11, %rdi
	jne	.LBB17_54
# %bb.55:                               #   in Loop: Header=BB17_44 Depth=2
	movq	%r15, %rdx
	jmp	.LBB17_56
	.p2align	4, 0x90
.LBB17_57:                              #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm11, %xmm0
	vcvtusi2sd	%rbx, %xmm11, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp20:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp21:
# %bb.58:                               #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.59:                               #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_61
# %bb.60:                               #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_63
	.p2align	4, 0x90
.LBB17_61:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp22:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp23:
# %bb.62:                               #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp24:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp25:
.LBB17_63:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp26:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp27:
# %bb.64:                               #   in Loop: Header=BB17_5 Depth=1
.Ltmp28:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp29:
# %bb.65:                               #   in Loop: Header=BB17_5 Depth=1
.Ltmp30:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$11, %edx
	leaq	.L.str.2(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp31:
# %bb.66:                               #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	movq	%rax, %rbp
	xorl	%ebx, %ebx
	jmp	.LBB17_67
	.p2align	4, 0x90
.LBB17_68:                              #   in Loop: Header=BB17_67 Depth=2
	xorl	%edx, %edx
.LBB17_80:                              #   in Loop: Header=BB17_67 Depth=2
	sarq	$3, %rdx
	addq	%rdx, %rbx
	vzeroupper
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	%rbp, %rax
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_81
.LBB17_67:                              #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_71 Depth 3
                                        #       Child Loop BB17_75 Depth 3
                                        #       Child Loop BB17_78 Depth 3
	movq	(%rsp), %rcx                    # 8-byte Reload
	cmpq	48(%rsp), %rcx                  # 8-byte Folded Reload
	je	.LBB17_68
# %bb.69:                               #   in Loop: Header=BB17_67 Depth=2
	cmpq	$56, 40(%rsp)                   # 8-byte Folded Reload
	setb	%al
	orb	31(%rsp), %al                   # 1-byte Folded Reload
	movq	8(%rsp), %rax                   # 8-byte Reload
	jne	.LBB17_73
# %bb.70:                               #   in Loop: Header=BB17_67 Depth=2
	vbroadcasti32x4	.LCPI17_21(%rip), %zmm7 # zmm7 = [7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8]
                                        # zmm7 = mem[0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3]
	vmovdqa64	.LCPI17_15(%rip), %zmm8 # zmm8 = [0,8,1,9,2,10,3,11]
	vmovdqa64	.LCPI17_16(%rip), %zmm9 # zmm9 = [4,12,5,13,6,14,7,15]
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	216(%rsp), %rsi                 # 8-byte Reload
	movq	(%rsp), %rdi                    # 8-byte Reload
	xorl	%ecx, %ecx
	.p2align	4, 0x90
.LBB17_71:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_67 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	%r12, %rdx
	mulxq	56(%rdi,%rcx,8), %rdx, %rdx
	vpbroadcastq	.LCPI17_17(%rip), %zmm4 # zmm4 = [4294957296,4294957296,4294957296,4294957296,4294957296,4294957296,4294957296,4294957296]
	vpbroadcastq	.LCPI17_12(%rip), %zmm6 # zmm6 = [5243,5243,5243,5243,5243,5243,5243,5243]
	vpbroadcastq	.LCPI17_18(%rip), %zmm5 # zmm5 = [65436,65436,65436,65436,65436,65436,65436,65436]
	vpbroadcastq	.LCPI17_5(%rip), %zmm10 # zmm10 = [103,103,103,103,103,103,103,103]
	vmovq	%rdx, %xmm0
	movq	%r12, %rdx
	mulxq	48(%rdi,%rcx,8), %rdx, %rdx
	vmovq	%rdx, %xmm1
	movq	%r12, %rdx
	mulxq	40(%rdi,%rcx,8), %rdx, %rdx
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vmovq	%rdx, %xmm1
	movq	%r12, %rdx
	mulxq	32(%rdi,%rcx,8), %rdx, %rdx
	vmovq	%rdx, %xmm2
	movq	%r12, %rdx
	mulxq	24(%rdi,%rcx,8), %rdx, %rdx
	vpunpcklqdq	%xmm1, %xmm2, %xmm1     # xmm1 = xmm2[0],xmm1[0]
	vinserti128	$1, %xmm0, %ymm1, %ymm0
	vmovq	%rdx, %xmm2
	movq	%r12, %rdx
	mulxq	16(%rdi,%rcx,8), %rdx, %rdx
	vmovq	%rdx, %xmm1
	movq	%r12, %rdx
	mulxq	8(%rdi,%rcx,8), %rdx, %rdx
	vpunpcklqdq	%xmm2, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm2[0]
	vmovq	%rdx, %xmm2
	movq	%r12, %rdx
	mulxq	(%rdi,%rcx,8), %rdx, %rdx
	vmovq	%rdx, %xmm3
	vpunpcklqdq	%xmm2, %xmm3, %xmm2     # xmm2 = xmm3[0],xmm2[0]
	vinserti128	$1, %xmm1, %ymm2, %ymm1
	vpbroadcastq	.LCPI17_10(%rip), %zmm2 # zmm2 = [109951163,109951163,109951163,109951163,109951163,109951163,109951163,109951163]
	vinserti64x4	$1, %ymm0, %zmm1, %zmm0
	vpsrlq	$26, %zmm0, %zmm0
	vpmullq	.LCPI17_9(%rip){1to8}, %zmm0, %zmm1
	vpaddq	(%rdi,%rcx,8), %zmm1, %zmm1
	addq	$8, %rcx
	vpmullq	%zmm2, %zmm0, %zmm3
	vpsrlq	$40, %zmm3, %zmm3
	vpmuludq	%zmm4, %zmm3, %zmm3
	vpmullq	%zmm2, %zmm1, %zmm2
	vpaddq	%zmm0, %zmm3, %zmm0
	vpsrlq	$40, %zmm2, %zmm2
	vpmullq	%zmm6, %zmm0, %zmm3
	vpmuludq	%zmm4, %zmm2, %zmm2
	vpbroadcastq	.LCPI17_3(%rip), %zmm4  # zmm4 = [545460846719,545460846719,545460846719,545460846719,545460846719,545460846719,545460846719,545460846719]
	vpsrlq	$19, %zmm3, %zmm3
	vpaddq	%zmm1, %zmm2, %zmm1
	vpmullq	%zmm6, %zmm1, %zmm2
	vpbroadcastq	.LCPI17_8(%rip), %zmm6  # zmm6 = [3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304,3472328296227680304]
	vpsrlq	$19, %zmm2, %zmm2
	vpandq	%zmm4, %zmm3, %zmm3
	vpandq	%zmm4, %zmm2, %zmm2
	vpbroadcastq	.LCPI17_6(%rip), %zmm4  # zmm4 = [4222189076152335,4222189076152335,4222189076152335,4222189076152335,4222189076152335,4222189076152335,4222189076152335,4222189076152335]
	vpmullq	%zmm5, %zmm3, %zmm3
	vpmullq	%zmm5, %zmm2, %zmm2
	vpbroadcastq	.LCPI17_19(%rip), %zmm5 # zmm5 = [246,246,246,246,246,246,246,246]
	vpaddq	%zmm0, %zmm3, %zmm0
	vpaddq	%zmm1, %zmm2, %zmm1
	vpmullq	%zmm10, %zmm0, %zmm3
	vpmullq	%zmm10, %zmm1, %zmm2
	vpaddq	%zmm6, %zmm0, %zmm0
	vpaddq	%zmm6, %zmm1, %zmm1
	vpsrlq	$10, %zmm3, %zmm3
	vpsrlq	$10, %zmm2, %zmm2
	vpandq	%zmm4, %zmm3, %zmm3
	vpandq	%zmm4, %zmm2, %zmm2
	vpmullq	%zmm5, %zmm3, %zmm3
	vpmullq	%zmm5, %zmm2, %zmm2
	vpaddq	%zmm3, %zmm0, %zmm0
	vpaddq	%zmm2, %zmm1, %zmm1
	vpshufb	%zmm7, %zmm0, %zmm0
	vpshufb	%zmm7, %zmm1, %zmm1
	vmovdqa64	%zmm0, %zmm2
	vpermt2q	%zmm1, %zmm8, %zmm2
	vpermt2q	%zmm1, %zmm9, %zmm0
	vmovdqu64	%zmm0, 64(%rax)
	vmovdqu64	%zmm2, (%rax)
	subq	$-128, %rax
	cmpq	%rcx, %rsi
	jne	.LBB17_71
# %bb.72:                               #   in Loop: Header=BB17_67 Depth=2
	movq	208(%rsp), %rax                 # 8-byte Reload
	movq	200(%rsp), %rcx                 # 8-byte Reload
	movq	%r15, %rdx
	cmpq	%rsi, 88(%rsp)                  # 8-byte Folded Reload
	je	.LBB17_80
.LBB17_73:                              #   in Loop: Header=BB17_67 Depth=2
	movq	56(%rsp), %rsi                  # 8-byte Reload
	subq	%rcx, %rsi
	movl	%esi, %edi
	shrl	$3, %edi
	incl	%edi
	andl	$7, %edi
	je	.LBB17_76
# %bb.74:                               #   in Loop: Header=BB17_67 Depth=2
	vmovdqa	.LCPI17_21(%rip), %xmm3         # xmm3 = [7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8]
	.p2align	4, 0x90
.LBB17_75:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_67 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx), %rdx
	addq	$8, %rcx
	mulxq	%r12, %r8, %r8
	shrq	$26, %r8
	imulq	$-100000000, %r8, %r9           # imm = 0xFA0A1F00
	addq	%rdx, %r9
	imulq	$109951163, %r8, %rdx           # imm = 0x68DB8BB
	imulq	$109951163, %r9, %r10           # imm = 0x68DB8BB
	vmovq	%r9, %xmm2
	vmovq	%rdx, %xmm0
	vmovq	%r10, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vmovq	%r8, %xmm1
	vpsrlq	$40, %xmm0, %xmm0
	vpmuludq	.LCPI17_17(%rip){1to2}, %xmm0, %xmm0
	vpunpcklqdq	%xmm2, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm2[0]
	vpaddq	%xmm1, %xmm0, %xmm0
	vpmullq	.LCPI17_12(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$19, %xmm1, %xmm1
	vpandq	.LCPI17_3(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI17_18(%rip){1to2}, %xmm1, %xmm1
	vpaddq	%xmm0, %xmm1, %xmm0
	vpmullq	.LCPI17_5(%rip){1to2}, %xmm0, %xmm1
	vpsrlq	$10, %xmm1, %xmm1
	vpandq	.LCPI17_6(%rip){1to2}, %xmm1, %xmm1
	vpmullq	.LCPI17_19(%rip){1to2}, %xmm1, %xmm1
	vpaddq	%xmm1, %xmm0, %xmm0
	vpaddq	.LCPI17_8(%rip){1to2}, %xmm0, %xmm0
	vpshufb	%xmm3, %xmm0, %xmm0
	vmovdqu	%xmm0, (%rax)
	addq	$16, %rax
	decq	%rdi
	jne	.LBB17_75
.LBB17_76:                              #   in Loop: Header=BB17_67 Depth=2
	movq	%r15, %rdx
	cmpq	$56, %rsi
	jb	.LBB17_80
# %bb.77:                               #   in Loop: Header=BB17_67 Depth=2
	vmovdqa	.LCPI17_21(%rip), %xmm11        # xmm11 = [7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8]
	movq	56(%rsp), %r11                  # 8-byte Reload
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB17_78:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_67 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx,%rsi), %rdx
	vpbroadcastq	.LCPI17_3(%rip), %xmm17 # xmm17 = [545460846719,545460846719]
	vpbroadcastq	.LCPI17_18(%rip), %xmm16 # xmm16 = [65436,65436]
	vpbroadcastq	.LCPI17_5(%rip), %xmm15 # xmm15 = [103,103]
	vpbroadcastq	.LCPI17_6(%rip), %xmm14 # xmm14 = [4222189076152335,4222189076152335]
	vpbroadcastq	.LCPI17_19(%rip), %xmm13 # xmm13 = [246,246]
	vpbroadcastq	.LCPI17_8(%rip), %xmm12 # xmm12 = [3472328296227680304,3472328296227680304]
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm3
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%r8, %xmm2
	vpunpcklqdq	%xmm2, %xmm3, %xmm2     # xmm2 = xmm3[0],xmm2[0]
	vmovq	%rdx, %xmm1
	vmovq	%r9, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpsrlq	$40, %xmm0, %xmm1
	vpbroadcastq	.LCPI17_17(%rip), %xmm0 # xmm0 = [4294957296,4294957296]
	vpmuludq	%xmm0, %xmm1, %xmm1
	vpaddq	%xmm2, %xmm1, %xmm4
	vpbroadcastq	.LCPI17_12(%rip), %xmm1 # xmm1 = [5243,5243]
	vpmullq	%xmm1, %xmm4, %xmm2
	vpsrlq	$19, %xmm2, %xmm3
	vpandq	%xmm17, %xmm3, %xmm5
	vpmullq	%xmm16, %xmm5, %xmm5
	vpaddq	%xmm4, %xmm5, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm5
	vpaddq	%xmm12, %xmm8, %xmm8
	vpsrlq	$10, %xmm5, %xmm6
	vpand	%xmm6, %xmm14, %xmm7
	vpmullq	%xmm13, %xmm7, %xmm9
	vpaddq	%xmm9, %xmm8, %xmm8
	vpshufb	%xmm11, %xmm8, %xmm8
	vmovdqu	%xmm8, (%rax,%rsi,2)
	movq	8(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm10
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%rdx, %xmm9
	vmovq	%r9, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%r8, %xmm9
	vpsrlq	$40, %xmm8, %xmm8
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpmuludq	%xmm0, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpmullq	%xmm1, %xmm8, %xmm9
	vpsrlq	$19, %xmm9, %xmm9
	vpandq	%xmm17, %xmm9, %xmm9
	vpmullq	%xmm16, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm9
	vpaddq	%xmm12, %xmm8, %xmm8
	vpsrlq	$10, %xmm9, %xmm9
	vpand	%xmm14, %xmm9, %xmm9
	vpmullq	%xmm13, %xmm9, %xmm9
	vpaddq	%xmm9, %xmm8, %xmm8
	vpshufb	%xmm11, %xmm8, %xmm8
	vmovdqu	%xmm8, 16(%rax,%rsi,2)
	movq	16(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm10
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%rdx, %xmm9
	vmovq	%r9, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%r8, %xmm9
	vpsrlq	$40, %xmm8, %xmm8
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpmuludq	%xmm0, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpmullq	%xmm1, %xmm8, %xmm9
	vpsrlq	$19, %xmm9, %xmm9
	vpandq	%xmm17, %xmm9, %xmm9
	vpmullq	%xmm16, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm9
	vpaddq	%xmm12, %xmm8, %xmm8
	vpsrlq	$10, %xmm9, %xmm9
	vpand	%xmm14, %xmm9, %xmm9
	vpmullq	%xmm13, %xmm9, %xmm9
	vpaddq	%xmm9, %xmm8, %xmm8
	vpshufb	%xmm11, %xmm8, %xmm8
	vmovdqu	%xmm8, 32(%rax,%rsi,2)
	movq	24(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm10
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%rdx, %xmm9
	vmovq	%r9, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%r8, %xmm9
	vpsrlq	$40, %xmm8, %xmm8
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpmuludq	%xmm0, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpmullq	%xmm1, %xmm8, %xmm9
	vpsrlq	$19, %xmm9, %xmm9
	vpandq	%xmm17, %xmm9, %xmm9
	vpmullq	%xmm16, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm9
	vpaddq	%xmm12, %xmm8, %xmm8
	vpsrlq	$10, %xmm9, %xmm9
	vpand	%xmm14, %xmm9, %xmm9
	vpmullq	%xmm13, %xmm9, %xmm9
	vpaddq	%xmm9, %xmm8, %xmm8
	vpshufb	%xmm11, %xmm8, %xmm8
	vmovdqu	%xmm8, 48(%rax,%rsi,2)
	movq	32(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm10
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%rdx, %xmm9
	vmovq	%r9, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%r8, %xmm9
	vpsrlq	$40, %xmm8, %xmm8
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpmuludq	%xmm0, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpmullq	%xmm1, %xmm8, %xmm9
	vpsrlq	$19, %xmm9, %xmm9
	vpandq	%xmm17, %xmm9, %xmm9
	vpmullq	%xmm16, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm9
	vpaddq	%xmm12, %xmm8, %xmm8
	vpsrlq	$10, %xmm9, %xmm9
	vpand	%xmm14, %xmm9, %xmm9
	vpmullq	%xmm13, %xmm9, %xmm9
	vpaddq	%xmm9, %xmm8, %xmm8
	vpshufb	%xmm11, %xmm8, %xmm8
	vmovdqu	%xmm8, 64(%rax,%rsi,2)
	movq	40(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm10
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%rdx, %xmm9
	vmovq	%r9, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%r8, %xmm9
	vpsrlq	$40, %xmm8, %xmm8
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpmuludq	%xmm0, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpmullq	%xmm1, %xmm8, %xmm9
	vpsrlq	$19, %xmm9, %xmm9
	vpandq	%xmm17, %xmm9, %xmm9
	vpmullq	%xmm16, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm9
	vpaddq	%xmm12, %xmm8, %xmm8
	vpsrlq	$10, %xmm9, %xmm9
	vpand	%xmm14, %xmm9, %xmm9
	vpmullq	%xmm13, %xmm9, %xmm9
	vpaddq	%xmm9, %xmm8, %xmm8
	vpshufb	%xmm11, %xmm8, %xmm8
	vmovdqu	%xmm8, 80(%rax,%rsi,2)
	movq	48(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$-100000000, %rdi, %r8          # imm = 0xFA0A1F00
	vmovq	%rdi, %xmm10
	addq	%rdx, %r8
	imulq	$109951163, %rdi, %rdx          # imm = 0x68DB8BB
	leaq	56(%rcx,%rsi), %rdi
	imulq	$109951163, %r8, %r9            # imm = 0x68DB8BB
	vmovq	%rdx, %xmm9
	vmovq	%r9, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%r8, %xmm9
	vpsrlq	$40, %xmm8, %xmm8
	vpunpcklqdq	%xmm9, %xmm10, %xmm9    # xmm9 = xmm10[0],xmm9[0]
	vpmuludq	%xmm0, %xmm8, %xmm8
	vpaddq	%xmm9, %xmm8, %xmm8
	vpmullq	%xmm1, %xmm8, %xmm9
	vpsrlq	$19, %xmm9, %xmm9
	vpandq	%xmm17, %xmm9, %xmm9
	vpmullq	%xmm16, %xmm9, %xmm9
	vpaddq	%xmm8, %xmm9, %xmm8
	vpmullq	%xmm15, %xmm8, %xmm9
	vpaddq	%xmm12, %xmm8, %xmm8
	vpsrlq	$10, %xmm9, %xmm9
	vpand	%xmm14, %xmm9, %xmm9
	vpmullq	%xmm13, %xmm9, %xmm9
	vpaddq	%xmm9, %xmm8, %xmm8
	vpshufb	%xmm11, %xmm8, %xmm8
	vmovdqu	%xmm8, 96(%rax,%rsi,2)
	movq	56(%rcx,%rsi), %rdx
	mulxq	%r12, %r8, %r8
	shrq	$26, %r8
	imulq	$-100000000, %r8, %r9           # imm = 0xFA0A1F00
	addq	%rdx, %r9
	imulq	$109951163, %r8, %rdx           # imm = 0x68DB8BB
	imulq	$109951163, %r9, %r10           # imm = 0x68DB8BB
	vmovq	%rdx, %xmm9
	vmovq	%r10, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vmovq	%r8, %xmm9
	vpsrlq	$40, %xmm8, %xmm8
	vpmuludq	%xmm0, %xmm8, %xmm0
	vmovq	%r9, %xmm8
	vpunpcklqdq	%xmm8, %xmm9, %xmm8     # xmm8 = xmm9[0],xmm8[0]
	vpaddq	%xmm0, %xmm8, %xmm0
	vpmullq	%xmm1, %xmm0, %xmm1
	vpsrlq	$19, %xmm1, %xmm1
	vpandq	%xmm17, %xmm1, %xmm1
	vpmullq	%xmm16, %xmm1, %xmm1
	vpaddq	%xmm0, %xmm1, %xmm0
	vpmullq	%xmm15, %xmm0, %xmm1
	vpaddq	%xmm0, %xmm12, %xmm0
	vpsrlq	$10, %xmm1, %xmm1
	vpand	%xmm1, %xmm14, %xmm1
	vpmullq	%xmm13, %xmm1, %xmm1
	vpaddq	%xmm1, %xmm0, %xmm0
	vpshufb	%xmm11, %xmm0, %xmm0
	vmovdqu	%xmm0, 112(%rax,%rsi,2)
	addq	$64, %rsi
	cmpq	%r11, %rdi
	jne	.LBB17_78
# %bb.79:                               #   in Loop: Header=BB17_67 Depth=2
	movq	%r15, %rdx
	jmp	.LBB17_80
	.p2align	4, 0x90
.LBB17_81:                              #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm11, %xmm0
	vcvtusi2sd	%rbx, %xmm11, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp32:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp33:
	movq	8(%rsp), %rbp                   # 8-byte Reload
# %bb.82:                               #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.83:                               #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_85
# %bb.84:                               #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_87
	.p2align	4, 0x90
.LBB17_85:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp34:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp35:
# %bb.86:                               #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp36:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp37:
.LBB17_87:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp38:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp39:
# %bb.88:                               #   in Loop: Header=BB17_5 Depth=1
.Ltmp40:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp41:
# %bb.89:                               #   in Loop: Header=BB17_5 Depth=1
.Ltmp42:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$11, %edx
	leaq	.L.str.3(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp43:
# %bb.90:                               #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	xorl	%ebx, %ebx
	movq	%rax, 232(%rsp)                 # 8-byte Spill
	jmp	.LBB17_91
	.p2align	4, 0x90
.LBB17_92:                              #   in Loop: Header=BB17_91 Depth=2
	xorl	%eax, %eax
.LBB17_99:                              #   in Loop: Header=BB17_91 Depth=2
	sarq	$3, %rax
	addq	%rax, %rbx
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	232(%rsp), %rax                 # 8-byte Folded Reload
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_100
.LBB17_91:                              #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_97 Depth 3
	movq	(%rsp), %rax                    # 8-byte Reload
	cmpq	48(%rsp), %rax                  # 8-byte Folded Reload
	je	.LBB17_92
# %bb.93:                               #   in Loop: Header=BB17_91 Depth=2
	testb	$8, 40(%rsp)                    # 1-byte Folded Reload
	movq	%rbp, %rcx
	movq	%rax, %rdx
	movq	%rbx, 112(%rsp)                 # 8-byte Spill
	jne	.LBB17_95
# %bb.94:                               #   in Loop: Header=BB17_91 Depth=2
	movq	(%rsp), %rax                    # 8-byte Reload
	movabsq	$1844674407370955162, %rdi      # imm = 0x199999999999999A
	movabsq	$811296384146066817, %r15       # imm = 0xB424DC35095CD81
	movq	(%rax), %rsi
	movabsq	$-3689348814741910323, %rax     # imm = 0xCCCCCCCCCCCCCCCD
	movq	%rsi, %rdx
	mulxq	%rax, %rdx, %rdx
	movl	%esi, %ecx
	shrq	$3, %rdx
	leal	(%rdx,%rdx), %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %ecx
	movl	%ecx, 16(%rsp)                  # 4-byte Spill
	mulxq	%rdi, %rax, %rax
	movabsq	$2361183241434822607, %rcx      # imm = 0x20C49BA5E353F7CF
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %edx
	movabsq	$2951479051793528259, %rax      # imm = 0x28F5C28F5C28F5C3
	movq	%rdx, 64(%rsp)                  # 8-byte Spill
	movq	%rsi, %rdx
	shrq	$2, %rdx
	mulxq	%rax, %rdx, %rdx
	shrq	$2, %rdx
	mulxq	%rdi, %rax, %rax
	movq	%rdx, %r8
	movq	%rsi, %rdx
	shrq	$3, %rdx
	mulxq	%rcx, %rbx, %rbx
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	shrq	$4, %rbx
	movq	%rbx, %rdx
	mulxq	%rdi, %rcx, %rcx
	movq	%rsi, %rdx
	subl	%eax, %r8d
	movabsq	$3777893186295716171, %rax      # imm = 0x346DC5D63886594B
	movq	%r8, 80(%rsp)                   # 8-byte Spill
	mulxq	%rax, %r8, %r8
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %ecx
	shrq	$11, %r8
	movq	%r8, %rdx
	mulxq	%rdi, %rax, %rax
	movq	%rsi, %rdx
	subl	%ecx, %ebx
	shrq	$5, %rdx
	movabsq	$755578637259143235, %rcx       # imm = 0xA7C5AC471B47843
	mulxq	%rcx, %r9, %r9
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	shrq	$7, %r9
	movq	%r9, %rdx
	mulxq	%rdi, %rcx, %rcx
	movq	%rsi, %rdx
	subl	%eax, %r8d
	movabsq	$4835703278458516699, %rax      # imm = 0x431BDE82D7B634DB
	mulxq	%rax, %r10, %r10
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %ecx
	shrq	$18, %r10
	movq	%r10, %rdx
	mulxq	%rdi, %rax, %rax
	movq	%rsi, %rdx
	subl	%ecx, %r9d
	movabsq	$-6067343680855748867, %rcx     # imm = 0xABCC77118461CEFD
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %r10d
	movabsq	$-2972493582642298179, %rax     # imm = 0xD6BF94D5E57A42BD
	mulxq	%rax, %r11, %r11
	shrq	$23, %r11
	movq	%r11, %rdx
	mulxq	%rdi, %rax, %rax
	movq	%rsi, %rdx
	mulxq	%rcx, %r12, %r12
	shrq	$26, %r12
	addl	%eax, %eax
	movq	%r12, %rdx
	mulxq	%rdi, %rcx, %rcx
	leal	(%rax,%rax,4), %eax
	movq	%rsi, %rdx
	shrq	$9, %rdx
	subl	%eax, %r11d
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %eax
	subl	%eax, %r12d
	movabsq	$19342813113834067, %rax        # imm = 0x44B82FA09B5A53
	mulxq	%rax, %rcx, %rcx
	shrq	$11, %rcx
	movq	%rcx, %rdx
	mulxq	%rdi, %rax, %rax
	movq	%rsi, %rdx
	movabsq	$-5770238071427257601, %rdi     # imm = 0xAFEBFF0BCB24AAFF
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %ecx
	movabsq	$-2601111570856684097, %rax     # imm = 0xDBE6FECEBDEDD5BF
	mulxq	%rax, %r14, %r14
	mulxq	%rdi, %r13, %r13
	shrq	$33, %r14
	shrq	$36, %r13
	imulq	$1717986919, %r14, %rax         # imm = 0x66666667
	imulq	$429496730, %r13, %rdx          # imm = 0x1999999A
	shrq	$34, %rax
	shrq	$32, %rdx
	addl	%eax, %eax
	addl	%edx, %edx
	leal	(%rax,%rax,4), %eax
	leal	(%rdx,%rdx,4), %edx
	subl	%eax, %r14d
	movabsq	$2535301200456458803, %rax      # imm = 0x232F33025BD42233
	subl	%edx, %r13d
	movq	%rsi, %rdx
	mulxq	%rax, %rbp, %rbp
	shrq	$37, %rbp
	imulq	$429496730, %rbp, %rax          # imm = 0x1999999A
	shrq	$32, %rax
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %ebp
	movabsq	$4056481920730334085, %rax      # imm = 0x384B84D092ED0385
	mulxq	%rax, %rax, %rax
	shrq	$41, %rax
	imulq	$429496730, %rax, %rdx          # imm = 0x1999999A
	shrq	$32, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edi
	movq	%rsi, %rdx
	mulxq	%r15, %r15, %r15
	shrq	$15, %rsi
	subl	%edi, %eax
	shrq	$42, %r15
	imulq	$429496730, %r15, %rdx          # imm = 0x1999999A
	shrq	$32, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %r15d
	movq	%rsi, %rdx
	movabsq	$633825300114115, %rsi          # imm = 0x24075F3DCEAC3
	mulxq	%rsi, %rdx, %rdx
	shrq	$20, %rdx
	imull	$26215, %edx, %esi              # imm = 0x6667
	shrl	$18, %esi
	addl	%esi, %esi
	leal	(%rsi,%rsi,4), %esi
	subl	%esi, %edx
	vmovd	%edx, %xmm0
	movq	128(%rsp), %rdx                 # 8-byte Reload
	vpinsrb	$1, %r15d, %xmm0, %xmm0
	movq	32(%rsp), %r15                  # 8-byte Reload
	vpinsrb	$2, %eax, %xmm0, %xmm0
	vpinsrb	$3, %ebp, %xmm0, %xmm0
	movq	8(%rsp), %rbp                   # 8-byte Reload
	vpinsrb	$4, %r13d, %xmm0, %xmm0
	movabsq	$633825300114115, %r13          # imm = 0x24075F3DCEAC3
	vpinsrb	$5, %r14d, %xmm0, %xmm0
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	vpinsrb	$6, %ecx, %xmm0, %xmm0
	movq	136(%rsp), %rcx                 # 8-byte Reload
	vpinsrb	$7, %r12d, %xmm0, %xmm0
	vpinsrb	$8, %r11d, %xmm0, %xmm0
	vpinsrb	$9, %r10d, %xmm0, %xmm0
	vpinsrb	$10, %r9d, %xmm0, %xmm0
	vpinsrb	$11, %r8d, %xmm0, %xmm0
	vpinsrb	$12, %ebx, %xmm0, %xmm0
	vpinsrb	$13, 80(%rsp), %xmm0, %xmm0     # 4-byte Folded Reload
	movq	112(%rsp), %rbx                 # 8-byte Reload
	vpinsrb	$14, 64(%rsp), %xmm0, %xmm0     # 4-byte Folded Reload
	vpinsrb	$15, 16(%rsp), %xmm0, %xmm0     # 4-byte Folded Reload
	vpord	.LCPI17_42(%rip){1to4}, %xmm0, %xmm0
	vmovdqu	%xmm0, (%rbp)
.LBB17_95:                              #   in Loop: Header=BB17_91 Depth=2
	cmpq	$8, 40(%rsp)                    # 8-byte Folded Reload
	movq	%r15, %rax
	jb	.LBB17_99
# %bb.96:                               #   in Loop: Header=BB17_91 Depth=2
	vpbroadcastb	.LCPI17_43(%rip), %xmm1 # xmm1 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	xorl	%esi, %esi
	movq	%rcx, 64(%rsp)                  # 8-byte Spill
	movq	%rdx, 80(%rsp)                  # 8-byte Spill
	.p2align	4, 0x90
.LBB17_97:                              #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_91 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	%rsi, 16(%rsp)                  # 8-byte Spill
	movabsq	$1844674407370955162, %r8       # imm = 0x199999999999999A
	movq	16(%rsp), %rax                  # 8-byte Reload
	movq	%r8, %r10
	movq	(%rdx,%rax), %rsi
	movabsq	$-3689348814741910323, %rax     # imm = 0xCCCCCCCCCCCCCCCD
	movq	%rsi, %rdx
	mulxq	%rax, %rdx, %rdx
	movl	%esi, %ecx
	shrq	$3, %rdx
	leal	(%rdx,%rdx), %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %ecx
	movl	%ecx, 72(%rsp)                  # 4-byte Spill
	mulxq	%r8, %rax, %rax
	movabsq	$2361183241434822607, %rcx      # imm = 0x20C49BA5E353F7CF
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %edx
	movabsq	$2951479051793528259, %rax      # imm = 0x28F5C28F5C28F5C3
	movq	%rdx, 104(%rsp)                 # 8-byte Spill
	movq	%rsi, %rdx
	shrq	$2, %rdx
	mulxq	%rax, %rdx, %rdx
	shrq	$2, %rdx
	mulxq	%r8, %rax, %rax
	movq	%rdx, %rdi
	movq	%rsi, %rdx
	shrq	$3, %rdx
	mulxq	%rcx, %rdx, %rdx
	addl	%eax, %eax
	shrq	$4, %rdx
	leal	(%rax,%rax,4), %eax
	mulxq	%r8, %rcx, %rcx
	subl	%eax, %edi
	movabsq	$3777893186295716171, %rax      # imm = 0x346DC5D63886594B
	movq	%rdi, 152(%rsp)                 # 8-byte Spill
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %ecx
	subl	%ecx, %edx
	movabsq	$755578637259143235, %rcx       # imm = 0xA7C5AC471B47843
	movq	%rdx, 120(%rsp)                 # 8-byte Spill
	movq	%rsi, %rdx
	mulxq	%rax, %r8, %r8
	shrq	$11, %r8
	movq	%r8, %rdx
	mulxq	%r10, %rax, %rax
	movq	%rsi, %rdx
	shrq	$5, %rdx
	mulxq	%rcx, %r9, %r9
	movq	%r10, %rcx
	movq	%rcx, %rdi
	addl	%eax, %eax
	shrq	$7, %r9
	leal	(%rax,%rax,4), %eax
	movq	%r9, %rdx
	mulxq	%r10, %rdx, %rdx
	subl	%eax, %r8d
	movabsq	$4835703278458516699, %rax      # imm = 0x431BDE82D7B634DB
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %r9d
	movq	%rsi, %rdx
	mulxq	%rax, %r10, %r10
	shrq	$18, %r10
	movq	%r10, %rdx
	mulxq	%rcx, %rax, %rax
	movq	%rsi, %rdx
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %r10d
	movabsq	$-2972493582642298179, %rax     # imm = 0xD6BF94D5E57A42BD
	mulxq	%rax, %r11, %r11
	shrq	$23, %r11
	movq	%r11, %rdx
	mulxq	%rcx, %rax, %rax
	movabsq	$-6067343680855748867, %rcx     # imm = 0xABCC77118461CEFD
	movq	%rsi, %rdx
	mulxq	%rcx, %r12, %r12
	movabsq	$-5770238071427257601, %rcx     # imm = 0xAFEBFF0BCB24AAFF
	addl	%eax, %eax
	shrq	$26, %r12
	leal	(%rax,%rax,4), %eax
	movq	%r12, %rdx
	mulxq	%rdi, %rdx, %rdx
	subl	%eax, %r11d
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %eax
	movq	%rsi, %rdx
	shrq	$9, %rdx
	subl	%eax, %r12d
	movabsq	$19342813113834067, %rax        # imm = 0x44B82FA09B5A53
	mulxq	%rax, %r15, %r15
	shrq	$11, %r15
	movq	%r15, %rdx
	mulxq	%rdi, %rax, %rax
	movq	%rsi, %rdx
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %r15d
	movabsq	$-2601111570856684097, %rax     # imm = 0xDBE6FECEBDEDD5BF
	mulxq	%rax, %r13, %r13
	mulxq	%rcx, %rbx, %rbx
	movabsq	$633825300114115, %rcx          # imm = 0x24075F3DCEAC3
	shrq	$33, %r13
	shrq	$36, %rbx
	imulq	$1717986919, %r13, %rax         # imm = 0x66666667
	imulq	$429496730, %rbx, %rdx          # imm = 0x1999999A
	shrq	$34, %rax
	shrq	$32, %rdx
	addl	%eax, %eax
	addl	%edx, %edx
	leal	(%rax,%rax,4), %eax
	leal	(%rdx,%rdx,4), %edx
	subl	%eax, %r13d
	movabsq	$2535301200456458803, %rax      # imm = 0x232F33025BD42233
	subl	%edx, %ebx
	movq	%rsi, %rdx
	shrq	$15, %rsi
	mulxq	%rax, %r14, %r14
	shrq	$37, %r14
	imulq	$429496730, %r14, %rax          # imm = 0x1999999A
	shrq	$32, %rax
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %r14d
	movabsq	$4056481920730334085, %rax      # imm = 0x384B84D092ED0385
	mulxq	%rax, %rbp, %rbp
	shrq	$41, %rbp
	imulq	$429496730, %rbp, %rax          # imm = 0x1999999A
	shrq	$32, %rax
	addl	%eax, %eax
	leal	(%rax,%rax,4), %edi
	movabsq	$811296384146066817, %rax       # imm = 0xB424DC35095CD81
	mulxq	%rax, %rax, %rax
	subl	%edi, %ebp
	shrq	$42, %rax
	imulq	$429496730, %rax, %rdx          # imm = 0x1999999A
	shrq	$32, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %eax
	movq	%rsi, %rdx
	mulxq	%rcx, %rcx, %rcx
	shrq	$20, %rcx
	imull	$26215, %ecx, %edx              # imm = 0x6667
	shrl	$18, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %ecx
	vmovd	%ecx, %xmm0
	movq	16(%rsp), %rcx                  # 8-byte Reload
	vpinsrb	$1, %eax, %xmm0, %xmm0
	movq	64(%rsp), %rax                  # 8-byte Reload
	vpinsrb	$2, %ebp, %xmm0, %xmm0
	vpinsrb	$3, %r14d, %xmm0, %xmm0
	vpinsrb	$4, %ebx, %xmm0, %xmm0
	movabsq	$1844674407370955162, %rbx      # imm = 0x199999999999999A
	vpinsrb	$5, %r13d, %xmm0, %xmm0
	vpinsrb	$6, %r15d, %xmm0, %xmm0
	vpinsrb	$7, %r12d, %xmm0, %xmm0
	vpinsrb	$8, %r11d, %xmm0, %xmm0
	vpinsrb	$9, %r10d, %xmm0, %xmm0
	vpinsrb	$10, %r9d, %xmm0, %xmm0
	vpinsrb	$11, %r8d, %xmm0, %xmm0
	vpinsrb	$12, 120(%rsp), %xmm0, %xmm0    # 4-byte Folded Reload
	vpinsrb	$13, 152(%rsp), %xmm0, %xmm0    # 4-byte Folded Reload
	vpinsrb	$14, 104(%rsp), %xmm0, %xmm0    # 4-byte Folded Reload
	vpinsrb	$15, 72(%rsp), %xmm0, %xmm0     # 4-byte Folded Reload
	vpor	%xmm1, %xmm0, %xmm0
	vmovdqu	%xmm0, (%rax,%rcx,2)
	movq	80(%rsp), %rax                  # 8-byte Reload
	movq	16(%rsp), %rcx                  # 8-byte Reload
	movq	8(%rax,%rcx), %rsi
	movabsq	$-3689348814741910323, %rax     # imm = 0xCCCCCCCCCCCCCCCD
	movq	%rsi, %rdx
	mulxq	%rax, %r14, %r14
	movl	%esi, %ecx
	shrq	$3, %r14
	leal	(%r14,%r14), %eax
	movq	%r14, %rdx
	leal	(%rax,%rax,4), %eax
	subl	%eax, %ecx
	movl	%ecx, 72(%rsp)                  # 4-byte Spill
	mulxq	%rbx, %rax, %rax
	movq	%rsi, %rdx
	shrq	$2, %rdx
	movabsq	$2951479051793528259, %rcx      # imm = 0x28F5C28F5C28F5C3
	mulxq	%rcx, %rdx, %rdx
	addl	%eax, %eax
	shrq	$2, %rdx
	leal	(%rax,%rax,4), %eax
	mulxq	%rbx, %rcx, %rcx
	movq	%rdx, %rdi
	movq	%rsi, %rdx
	shrq	$3, %rdx
	subl	%eax, %r14d
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %eax
	movabsq	$3777893186295716171, %rcx      # imm = 0x346DC5D63886594B
	subl	%eax, %edi
	movabsq	$2361183241434822607, %rax      # imm = 0x20C49BA5E353F7CF
	movq	%rdi, 104(%rsp)                 # 8-byte Spill
	mulxq	%rax, %rdi, %rdi
	shrq	$4, %rdi
	movq	%rdi, %rdx
	mulxq	%rbx, %rax, %rax
	movq	%rsi, %rdx
	mulxq	%rcx, %r8, %r8
	shrq	$11, %r8
	addl	%eax, %eax
	movq	%r8, %rdx
	mulxq	%rbx, %rcx, %rcx
	leal	(%rax,%rax,4), %eax
	movq	%rsi, %rdx
	shrq	$5, %rdx
	subl	%eax, %edi
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %eax
	movabsq	$4835703278458516699, %rcx      # imm = 0x431BDE82D7B634DB
	subl	%eax, %r8d
	movabsq	$755578637259143235, %rax       # imm = 0xA7C5AC471B47843
	mulxq	%rax, %r9, %r9
	shrq	$7, %r9
	movq	%r9, %rdx
	mulxq	%rbx, %rax, %rax
	movq	%rsi, %rdx
	mulxq	%rcx, %r10, %r10
	shrq	$18, %r10
	addl	%eax, %eax
	movq	%r10, %rdx
	mulxq	%rbx, %rcx, %rcx
	leal	(%rax,%rax,4), %eax
	movq	%rsi, %rdx
	subl	%eax, %r9d
	addl	%ecx, %ecx
	leal	(%rcx,%rcx,4), %eax
	movq	%rbx, %rcx
	subl	%eax, %r10d
	movabsq	$-2972493582642298179, %rax     # imm = 0xD6BF94D5E57A42BD
	mulxq	%rax, %r11, %r11
	shrq	$23, %r11
	movq	%r11, %rdx
	mulxq	%rbx, %rax, %rax
	movabsq	$-6067343680855748867, %rbx     # imm = 0xABCC77118461CEFD
	movq	%rsi, %rdx
	mulxq	%rbx, %r12, %r12
	movq	%rcx, %rbx
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	shrq	$26, %r12
	movq	%r12, %rdx
	subl	%eax, %r11d
	mulxq	%rcx, %rax, %rax
	movq	%rsi, %rdx
	shrq	$9, %rdx
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %r12d
	movabsq	$19342813113834067, %rax        # imm = 0x44B82FA09B5A53
	mulxq	%rax, %rcx, %rcx
	shrq	$11, %rcx
	movq	%rcx, %rdx
	mulxq	%rbx, %rax, %rax
	movabsq	$-2601111570856684097, %rbx     # imm = 0xDBE6FECEBDEDD5BF
	movq	%rsi, %rdx
	mulxq	%rbx, %r13, %r13
	addl	%eax, %eax
	shrq	$33, %r13
	leal	(%rax,%rax,4), %eax
	imulq	$1717986919, %r13, %rdx         # imm = 0x66666667
	subl	%eax, %ecx
	movabsq	$-5770238071427257601, %rax     # imm = 0xAFEBFF0BCB24AAFF
	shrq	$34, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %r13d
	movq	%rsi, %rdx
	mulxq	%rax, %rbx, %rbx
	shrq	$36, %rbx
	imulq	$429496730, %rbx, %rax          # imm = 0x1999999A
	shrq	$32, %rax
	addl	%eax, %eax
	leal	(%rax,%rax,4), %eax
	subl	%eax, %ebx
	movabsq	$2535301200456458803, %rax      # imm = 0x232F33025BD42233
	mulxq	%rax, %rbp, %rbp
	shrq	$37, %rbp
	imulq	$429496730, %rbp, %rax          # imm = 0x1999999A
	shrq	$32, %rax
	addl	%eax, %eax
	leal	(%rax,%rax,4), %r15d
	movabsq	$4056481920730334085, %rax      # imm = 0x384B84D092ED0385
	mulxq	%rax, %rax, %rax
	subl	%r15d, %ebp
	movabsq	$811296384146066817, %r15       # imm = 0xB424DC35095CD81
	shrq	$41, %rax
	imulq	$429496730, %rax, %rdx          # imm = 0x1999999A
	shrq	$32, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %eax
	movq	%rsi, %rdx
	mulxq	%r15, %r15, %r15
	shrq	$15, %rsi
	shrq	$42, %r15
	imulq	$429496730, %r15, %rdx          # imm = 0x1999999A
	shrq	$32, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %r15d
	movq	%rsi, %rdx
	movabsq	$633825300114115, %rsi          # imm = 0x24075F3DCEAC3
	mulxq	%rsi, %rdx, %rdx
	shrq	$20, %rdx
	imull	$26215, %edx, %esi              # imm = 0x6667
	shrl	$18, %esi
	addl	%esi, %esi
	leal	(%rsi,%rsi,4), %esi
	subl	%esi, %edx
	movq	16(%rsp), %rsi                  # 8-byte Reload
	vmovd	%edx, %xmm0
	movq	80(%rsp), %rdx                  # 8-byte Reload
	vpinsrb	$1, %r15d, %xmm0, %xmm0
	vpinsrb	$2, %eax, %xmm0, %xmm0
	vpinsrb	$3, %ebp, %xmm0, %xmm0
	vpinsrb	$4, %ebx, %xmm0, %xmm0
	vpinsrb	$5, %r13d, %xmm0, %xmm0
	leaq	8(%rdx,%rsi), %rax
	vpinsrb	$6, %ecx, %xmm0, %xmm0
	movq	64(%rsp), %rcx                  # 8-byte Reload
	vpinsrb	$7, %r12d, %xmm0, %xmm0
	vpinsrb	$8, %r11d, %xmm0, %xmm0
	vpinsrb	$9, %r10d, %xmm0, %xmm0
	vpinsrb	$10, %r9d, %xmm0, %xmm0
	vpinsrb	$11, %r8d, %xmm0, %xmm0
	vpinsrb	$12, %edi, %xmm0, %xmm0
	vpinsrb	$13, 104(%rsp), %xmm0, %xmm0    # 4-byte Folded Reload
	vpinsrb	$14, %r14d, %xmm0, %xmm0
	vpinsrb	$15, 72(%rsp), %xmm0, %xmm0     # 4-byte Folded Reload
	vpor	%xmm1, %xmm0, %xmm0
	vmovdqu	%xmm0, 16(%rcx,%rsi,2)
	addq	$16, %rsi
	cmpq	56(%rsp), %rax                  # 8-byte Folded Reload
	jne	.LBB17_97
# %bb.98:                               #   in Loop: Header=BB17_91 Depth=2
	movq	32(%rsp), %r15                  # 8-byte Reload
	movq	8(%rsp), %rbp                   # 8-byte Reload
	movq	112(%rsp), %rbx                 # 8-byte Reload
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	movabsq	$633825300114115, %r13          # imm = 0x24075F3DCEAC3
	movq	%r15, %rax
	jmp	.LBB17_99
	.p2align	4, 0x90
.LBB17_100:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm7, %xmm0
	vcvtusi2sd	%rbx, %xmm7, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp44:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp45:
# %bb.101:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.102:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_104
# %bb.103:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_106
	.p2align	4, 0x90
.LBB17_104:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp46:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp47:
# %bb.105:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp48:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp49:
.LBB17_106:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp50:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp51:
# %bb.107:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp52:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp53:
# %bb.108:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp54:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$7, %edx
	leaq	.L.str.4(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp55:
# %bb.109:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movabsq	$1000000000000000, %r15         # imm = 0x38D7EA4C68000
	xorl	%r12d, %r12d
	movq	%rax, 80(%rsp)                  # 8-byte Spill
	jmp	.LBB17_110
	.p2align	4, 0x90
.LBB17_111:                             #   in Loop: Header=BB17_110 Depth=2
	xorl	%eax, %eax
.LBB17_118:                             #   in Loop: Header=BB17_110 Depth=2
	sarq	$3, %rax
	addq	%rax, %r12
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	80(%rsp), %rax                  # 8-byte Folded Reload
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_119
.LBB17_110:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_116 Depth 3
	movq	(%rsp), %rsi                    # 8-byte Reload
	cmpq	48(%rsp), %rsi                  # 8-byte Folded Reload
	je	.LBB17_111
# %bb.112:                              #   in Loop: Header=BB17_110 Depth=2
	testb	$8, 40(%rsp)                    # 1-byte Folded Reload
	movq	%rbp, %rcx
	jne	.LBB17_114
# %bb.113:                              #   in Loop: Header=BB17_110 Depth=2
	movq	(%rsp), %rax                    # 8-byte Reload
	movabsq	$18446744074, %r14              # imm = 0x44B82FA0A
	movq	(%rax), %rax
	movq	%rax, %rdx
	shrq	$15, %rdx
	mulxq	%r13, %rcx, %rcx
	shrq	$20, %rcx
	vmovd	%ecx, %xmm0
	imulq	%r15, %rcx
	subq	%rcx, %rax
	movabsq	$198070406285661, %rcx          # imm = 0xB424DC35095D
	movq	%rax, %rdx
	mulxq	%rcx, %rcx, %rcx
	movabsq	$100000000000000, %rdx          # imm = 0x5AF3107A4000
	shrq	$30, %rcx
	vpinsrb	$1, %ecx, %xmm0, %xmm0
	imulq	%rdx, %rcx
	subq	%rcx, %rax
	movabsq	$247588007857077, %rcx          # imm = 0xE12E13424BB5
	movq	%rax, %rdx
	mulxq	%rcx, %rcx, %rcx
	movabsq	$10000000000000, %rdx           # imm = 0x9184E72A000
	shrl	$27, %ecx
	vpinsrb	$2, %ecx, %xmm0, %xmm0
	imulq	%rdx, %rcx
	subq	%rcx, %rax
	movabsq	$19342813113835, %rcx           # imm = 0x119799812DEB
	movq	%rax, %rdx
	mulxq	%rcx, %rcx, %rcx
	movabsq	$1000000000000, %rdx            # imm = 0xE8D4A51000
	shrl	$20, %ecx
	vpinsrb	$3, %ecx, %xmm0, %xmm0
	imulq	%rdx, %rcx
	subq	%rcx, %rax
	movabsq	$1511157274519, %rcx            # imm = 0x15FD7FE1797
	movq	%rax, %rdx
	mulxq	%rcx, %rcx, %rcx
	movabsq	$100000000000, %rdx             # imm = 0x174876E800
	shrl	$13, %ecx
	vpinsrb	$4, %ecx, %xmm0, %xmm0
	imulq	%rdx, %rcx
	subq	%rcx, %rax
	movabsq	$14757395259, %rcx              # imm = 0x36F9BFB3B
	movq	%rax, %rdx
	mulxq	%rcx, %rcx, %rcx
	movabsq	$10000000000, %rdx              # imm = 0x2540BE400
	shrl	$3, %ecx
	vpinsrb	$5, %ecx, %xmm0, %xmm0
	imulq	%rdx, %rcx
	subq	%rcx, %rax
	movq	%rax, %rdx
	mulxq	%r14, %rcx, %rcx
	imulq	$1000000000, %rcx, %rdx         # imm = 0x3B9ACA00
	vpinsrb	$6, %ecx, %xmm0, %xmm0
	movq	136(%rsp), %rcx                 # 8-byte Reload
	subq	%rdx, %rax
	imulq	$720575941, %rax, %rdx          # imm = 0x2AF31DC5
	shrq	$56, %rdx
	imull	$100000000, %edx, %esi          # imm = 0x5F5E100
	vpinsrb	$7, %edx, %xmm0, %xmm0
	subl	%esi, %eax
	imulq	$112589991, %rax, %rsi          # imm = 0x6B5FCA7
	shrq	$50, %rsi
	imull	$10000000, %esi, %edi           # imm = 0x989680
	vpinsrb	$8, %esi, %xmm0, %xmm0
	movq	128(%rsp), %rsi                 # 8-byte Reload
	subl	%edi, %eax
	imulq	$17592187, %rax, %rdi           # imm = 0x10C6F7B
	shrq	$44, %rdi
	imull	$1000000, %edi, %r8d            # imm = 0xF4240
	vpinsrb	$9, %edi, %xmm0, %xmm0
	subl	%r8d, %eax
	imulq	$687195, %rax, %r8              # imm = 0xA7C5B
	shrq	$36, %r8
	imull	$100000, %r8d, %r9d             # imm = 0x186A0
	vpinsrb	$10, %r8d, %xmm0, %xmm0
	subl	%r9d, %eax
	imulq	$429497, %rax, %r9              # imm = 0x68DB9
	shrq	$32, %r9
	imull	$10000, %r9d, %r10d             # imm = 0x2710
	vpinsrb	$11, %r9d, %xmm0, %xmm0
	subl	%r10d, %eax
	imull	$8389, %eax, %r10d              # imm = 0x20C5
	shrl	$23, %r10d
	imull	$1000, %r10d, %r11d             # imm = 0x3E8
	vpinsrb	$12, %r10d, %xmm0, %xmm0
	subl	%r11d, %eax
	movzwl	%ax, %r11d
	imull	$656, %r11d, %r11d              # imm = 0x290
	shrl	$16, %r11d
	imull	$100, %r11d, %ebx
	vpinsrb	$13, %r11d, %xmm0, %xmm1
	subl	%ebx, %eax
	imull	$103, %eax, %ebx
	movzwl	%bx, %ebx
	shrl	$10, %ebx
	leal	(%rbx,%rbx), %r14d
	vpinsrb	$14, %ebx, %xmm1, %xmm1
	leal	(%r14,%r14,4), %r14d
	subb	%r14b, %al
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	vpinsrb	$15, %eax, %xmm1, %xmm1
	movw	$4681, %ax                      # imm = 0x1249
	vpord	.LCPI17_42(%rip){1to4}, %xmm1, %xmm1
	kmovd	%eax, %k1
	vpaddb	.LCPI17_24(%rip), %xmm0, %xmm1 {%k1}
	vmovdqu	%xmm1, (%rbp)
.LBB17_114:                             #   in Loop: Header=BB17_110 Depth=2
	movq	32(%rsp), %rax                  # 8-byte Reload
	cmpq	$8, 40(%rsp)                    # 8-byte Folded Reload
	jb	.LBB17_118
# %bb.115:                              #   in Loop: Header=BB17_110 Depth=2
	vpbroadcastb	.LCPI17_43(%rip), %xmm2 # xmm2 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	vpbroadcastb	.LCPI17_43(%rip), %xmm3 # xmm3 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	xorl	%edi, %edi
	movq	%r12, 72(%rsp)                  # 8-byte Spill
	movq	%rcx, 64(%rsp)                  # 8-byte Spill
	movq	%rsi, 16(%rsp)                  # 8-byte Spill
	.p2align	4, 0x90
.LBB17_116:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_110 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rsi,%rdi), %rax
	movabsq	$633825300114115, %rsi          # imm = 0x24075F3DCEAC3
	movabsq	$1000000000000000, %r15         # imm = 0x38D7EA4C68000
	movabsq	$198070406285661, %rcx          # imm = 0xB424DC35095D
	movq	%rax, %rdx
	shrq	$15, %rdx
	mulxq	%rsi, %rdx, %rdx
	shrq	$20, %rdx
	vmovd	%edx, %xmm0
	imulq	%r15, %rdx
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rcx, %rdx, %rdx
	movabsq	$100000000000000, %rcx          # imm = 0x5AF3107A4000
	shrq	$30, %rdx
	vpinsrb	$1, %edx, %xmm0, %xmm0
	imulq	%rcx, %rdx
	movabsq	$247588007857077, %rcx          # imm = 0xE12E13424BB5
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rcx, %rdx, %rdx
	movabsq	$10000000000000, %rcx           # imm = 0x9184E72A000
	shrl	$27, %edx
	vpinsrb	$2, %edx, %xmm0, %xmm0
	imulq	%rcx, %rdx
	movabsq	$19342813113835, %rcx           # imm = 0x119799812DEB
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rcx, %rdx, %rdx
	movabsq	$1000000000000, %rcx            # imm = 0xE8D4A51000
	shrl	$20, %edx
	vpinsrb	$3, %edx, %xmm0, %xmm0
	imulq	%rcx, %rdx
	movabsq	$1511157274519, %rcx            # imm = 0x15FD7FE1797
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rcx, %rdx, %rdx
	movabsq	$100000000000, %rcx             # imm = 0x174876E800
	shrl	$13, %edx
	vpinsrb	$4, %edx, %xmm0, %xmm0
	imulq	%rcx, %rdx
	movabsq	$14757395259, %rcx              # imm = 0x36F9BFB3B
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rcx, %rdx, %rdx
	movabsq	$10000000000, %rcx              # imm = 0x2540BE400
	shrl	$3, %edx
	vpinsrb	$5, %edx, %xmm0, %xmm0
	imulq	%rcx, %rdx
	movabsq	$18446744074, %rcx              # imm = 0x44B82FA0A
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rcx, %rdx, %rdx
	movq	64(%rsp), %rcx                  # 8-byte Reload
	imulq	$1000000000, %rdx, %r8          # imm = 0x3B9ACA00
	vpinsrb	$6, %edx, %xmm0, %xmm0
	subq	%r8, %rax
	imulq	$720575941, %rax, %r8           # imm = 0x2AF31DC5
	shrq	$56, %r8
	imull	$100000000, %r8d, %r9d          # imm = 0x5F5E100
	vpinsrb	$7, %r8d, %xmm0, %xmm0
	subl	%r9d, %eax
	imulq	$112589991, %rax, %r9           # imm = 0x6B5FCA7
	shrq	$50, %r9
	imull	$10000000, %r9d, %r10d          # imm = 0x989680
	vpinsrb	$8, %r9d, %xmm0, %xmm0
	subl	%r10d, %eax
	imulq	$17592187, %rax, %r10           # imm = 0x10C6F7B
	shrq	$44, %r10
	imull	$1000000, %r10d, %r11d          # imm = 0xF4240
	vpinsrb	$9, %r10d, %xmm0, %xmm0
	subl	%r11d, %eax
	imulq	$687195, %rax, %r11             # imm = 0xA7C5B
	shrq	$36, %r11
	imull	$100000, %r11d, %ebx            # imm = 0x186A0
	vpinsrb	$10, %r11d, %xmm0, %xmm0
	subl	%ebx, %eax
	imulq	$429497, %rax, %rbx             # imm = 0x68DB9
	shrq	$32, %rbx
	imull	$10000, %ebx, %r14d             # imm = 0x2710
	vpinsrb	$11, %ebx, %xmm0, %xmm0
	subl	%r14d, %eax
	imull	$8389, %eax, %r14d              # imm = 0x20C5
	shrl	$23, %r14d
	imull	$1000, %r14d, %r12d             # imm = 0x3E8
	vpinsrb	$12, %r14d, %xmm0, %xmm0
	subl	%r12d, %eax
	movzwl	%ax, %r12d
	imull	$656, %r12d, %r12d              # imm = 0x290
	shrl	$16, %r12d
	imull	$100, %r12d, %r13d
	vpinsrb	$13, %r12d, %xmm0, %xmm1
	subl	%r13d, %eax
	imull	$103, %eax, %r13d
	movzwl	%r13w, %r13d
	shrl	$10, %r13d
	leal	(%r13,%r13), %ebp
	vpinsrb	$14, %r13d, %xmm1, %xmm1
	leal	(%rbp,%rbp,4), %ebp
	subb	%bpl, %al
	vpinsrb	$15, %eax, %xmm1, %xmm1
	movw	$4681, %ax                      # imm = 0x1249
	kmovd	%eax, %k1
	movq	16(%rsp), %rax                  # 8-byte Reload
	vpor	%xmm3, %xmm1, %xmm1
	vpaddb	%xmm2, %xmm0, %xmm1 {%k1}
	vmovdqu	%xmm1, (%rcx,%rdi,2)
	movq	8(%rax,%rdi), %rax
	movq	%rax, %rdx
	shrq	$15, %rdx
	mulxq	%rsi, %rdx, %rdx
	movabsq	$198070406285661, %rsi          # imm = 0xB424DC35095D
	shrq	$20, %rdx
	vmovd	%edx, %xmm0
	imulq	%r15, %rdx
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rsi, %rdx, %rdx
	movabsq	$100000000000000, %rsi          # imm = 0x5AF3107A4000
	shrq	$30, %rdx
	vpinsrb	$1, %edx, %xmm0, %xmm0
	imulq	%rsi, %rdx
	movabsq	$247588007857077, %rsi          # imm = 0xE12E13424BB5
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rsi, %rdx, %rdx
	movabsq	$10000000000000, %rsi           # imm = 0x9184E72A000
	shrl	$27, %edx
	vpinsrb	$2, %edx, %xmm0, %xmm0
	imulq	%rsi, %rdx
	movabsq	$19342813113835, %rsi           # imm = 0x119799812DEB
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rsi, %rdx, %rdx
	movabsq	$1000000000000, %rsi            # imm = 0xE8D4A51000
	shrl	$20, %edx
	vpinsrb	$3, %edx, %xmm0, %xmm0
	imulq	%rsi, %rdx
	movabsq	$1511157274519, %rsi            # imm = 0x15FD7FE1797
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rsi, %rdx, %rdx
	movabsq	$100000000000, %rsi             # imm = 0x174876E800
	shrl	$13, %edx
	vpinsrb	$4, %edx, %xmm0, %xmm0
	imulq	%rsi, %rdx
	movabsq	$14757395259, %rsi              # imm = 0x36F9BFB3B
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rsi, %rdx, %rdx
	movabsq	$10000000000, %rsi              # imm = 0x2540BE400
	shrl	$3, %edx
	vpinsrb	$5, %edx, %xmm0, %xmm0
	imulq	%rsi, %rdx
	movabsq	$18446744074, %rsi              # imm = 0x44B82FA0A
	subq	%rdx, %rax
	movq	%rax, %rdx
	mulxq	%rsi, %rdx, %rdx
	movq	16(%rsp), %rsi                  # 8-byte Reload
	imulq	$1000000000, %rdx, %r8          # imm = 0x3B9ACA00
	vpinsrb	$6, %edx, %xmm0, %xmm0
	subq	%r8, %rax
	imulq	$720575941, %rax, %r8           # imm = 0x2AF31DC5
	shrq	$56, %r8
	imull	$100000000, %r8d, %r9d          # imm = 0x5F5E100
	vpinsrb	$7, %r8d, %xmm0, %xmm0
	subl	%r9d, %eax
	imulq	$112589991, %rax, %r9           # imm = 0x6B5FCA7
	shrq	$50, %r9
	imull	$10000000, %r9d, %r10d          # imm = 0x989680
	vpinsrb	$8, %r9d, %xmm0, %xmm0
	subl	%r10d, %eax
	imulq	$17592187, %rax, %r10           # imm = 0x10C6F7B
	shrq	$44, %r10
	imull	$1000000, %r10d, %r11d          # imm = 0xF4240
	vpinsrb	$9, %r10d, %xmm0, %xmm0
	subl	%r11d, %eax
	imulq	$687195, %rax, %r11             # imm = 0xA7C5B
	shrq	$36, %r11
	imull	$100000, %r11d, %ebx            # imm = 0x186A0
	vpinsrb	$10, %r11d, %xmm0, %xmm0
	subl	%ebx, %eax
	imulq	$429497, %rax, %rbx             # imm = 0x68DB9
	shrq	$32, %rbx
	imull	$10000, %ebx, %ebp              # imm = 0x2710
	vpinsrb	$11, %ebx, %xmm0, %xmm0
	subl	%ebp, %eax
	imull	$8389, %eax, %ebp               # imm = 0x20C5
	shrl	$23, %ebp
	imull	$1000, %ebp, %r14d              # imm = 0x3E8
	vpinsrb	$12, %ebp, %xmm0, %xmm0
	subl	%r14d, %eax
	movzwl	%ax, %r14d
	imull	$656, %r14d, %r14d              # imm = 0x290
	shrl	$16, %r14d
	imull	$100, %r14d, %r12d
	vpinsrb	$13, %r14d, %xmm0, %xmm1
	subl	%r12d, %eax
	imull	$103, %eax, %r12d
	movzwl	%r12w, %r12d
	shrl	$10, %r12d
	leal	(%r12,%r12), %r13d
	vpinsrb	$14, %r12d, %xmm1, %xmm1
	leal	(%r13,%r13,4), %r13d
	subb	%r13b, %al
	vpinsrb	$15, %eax, %xmm1, %xmm1
	leaq	8(%rsi,%rdi), %rax
	vpor	%xmm3, %xmm1, %xmm1
	vpaddb	%xmm2, %xmm0, %xmm1 {%k1}
	vmovdqu	%xmm1, 16(%rcx,%rdi,2)
	addq	$16, %rdi
	cmpq	56(%rsp), %rax                  # 8-byte Folded Reload
	jne	.LBB17_116
# %bb.117:                              #   in Loop: Header=BB17_110 Depth=2
	movq	32(%rsp), %rax                  # 8-byte Reload
	movq	8(%rsp), %rbp                   # 8-byte Reload
	movq	72(%rsp), %r12                  # 8-byte Reload
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	movabsq	$633825300114115, %r13          # imm = 0x24075F3DCEAC3
	jmp	.LBB17_118
	.p2align	4, 0x90
.LBB17_119:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm4, %xmm0
	vcvtusi2sd	%r12, %xmm4, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp56:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp57:
# %bb.120:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.121:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_123
# %bb.122:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_125
	.p2align	4, 0x90
.LBB17_123:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp58:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp59:
# %bb.124:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp60:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp61:
.LBB17_125:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp62:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp63:
# %bb.126:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp64:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp65:
# %bb.127:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp66:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$7, %edx
	leaq	.L.str.5(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp67:
# %bb.128:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	xorl	%ebx, %ebx
	movq	%rax, 112(%rsp)                 # 8-byte Spill
	jmp	.LBB17_129
	.p2align	4, 0x90
.LBB17_130:                             #   in Loop: Header=BB17_129 Depth=2
	xorl	%eax, %eax
.LBB17_137:                             #   in Loop: Header=BB17_129 Depth=2
	sarq	$3, %rax
	addq	%rax, %rbx
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	112(%rsp), %rax                 # 8-byte Folded Reload
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_138
.LBB17_129:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_135 Depth 3
	movq	(%rsp), %rax                    # 8-byte Reload
	cmpq	48(%rsp), %rax                  # 8-byte Folded Reload
	je	.LBB17_130
# %bb.131:                              #   in Loop: Header=BB17_129 Depth=2
	testb	$8, 40(%rsp)                    # 1-byte Folded Reload
	movq	%rbp, %rcx
	movq	%rax, %rdx
	movq	%rbx, 120(%rsp)                 # 8-byte Spill
	jne	.LBB17_133
# %bb.132:                              #   in Loop: Header=BB17_129 Depth=2
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	(%rax), %rcx
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rcx, %rdx
	mulxq	%rax, %rax, %rax
	movq	%rcx, %rsi
	shrq	$26, %rax
	imulq	$100000000, %rax, %rdx          # imm = 0x5F5E100
	subq	%rdx, %rsi
	movq	%rcx, %rdx
	movq	%rsi, %rdi
	movabsq	$2535301200456458803, %rsi      # imm = 0x232F33025BD42233
	mulxq	%rsi, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	movq	%rdi, %rsi
	movq	%rdi, 16(%rsp)                  # 8-byte Spill
	imulq	$109951163, %rdi, %rdi          # imm = 0x68DB8BB
	imulq	$140737489, %rsi, %r10          # imm = 0x8637BD1
	movl	%esi, %r9d
	movabsq	$811296384146066817, %rsi       # imm = 0xB424DC35095CD81
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$40, %rdi
	shrq	$37, %r8
	shrq	$47, %r10
	subl	%edx, %eax
	imull	$10000, %edi, %edx              # imm = 0x2710
	imull	$5243, %eax, %ebp               # imm = 0x147B
	movl	%eax, %r14d
	imull	$8389, %eax, %eax               # imm = 0x20C5
	subl	%edx, %r9d
	movq	%rcx, %rdx
	mulxq	%rsi, %r15, %r15
	imulq	$42949673, %r8, %rdx            # imm = 0x28F5C29
	shrq	$15, %rcx
	shrl	$19, %ebp
	imull	$5243, %r9d, %r11d              # imm = 0x147B
	movl	%r9d, %ebx
	movl	%r9d, 64(%rsp)                  # 4-byte Spill
	shrl	$23, %eax
	shrq	$32, %rdx
	shrq	$42, %r15
	imull	$100, %edx, %edx
	shrl	$19, %r11d
	subl	%edx, %r8d
	imull	$100, %ebp, %edx
	subl	%edx, %r14d
	imull	$5243, %edi, %edx               # imm = 0x147B
	shrl	$19, %edx
	imull	$100, %edx, %edx
	subl	%edx, %edi
	imull	$100, %r11d, %edx
	subl	%edx, %ebx
	imulq	$429496730, %r15, %rdx          # imm = 0x1999999A
	shrq	$32, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %r15d
	imull	$103, %r8d, %edx
	movzwl	%dx, %r12d
	shrl	$10, %r12d
	leal	(%r12,%r12), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r8b
	imull	$205, %ebp, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %bpl
	imull	$103, %r14d, %edx
	movzwl	%dx, %r13d
	shrl	$10, %r13d
	leal	(%r13,%r13), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r14b
	imull	$205, %r10d, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r10b
	imull	$103, %edi, %edx
	movzwl	%dx, %r9d
	shrl	$10, %r9d
	leal	(%r9,%r9), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %dil
	imull	$205, %r11d, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r11b
	imull	$103, %ebx, %edx
	movzwl	%dx, %esi
	shrl	$10, %esi
	leal	(%rsi,%rsi), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %bl
	movq	%rcx, %rdx
	movabsq	$633825300114115, %rcx          # imm = 0x24075F3DCEAC3
	mulxq	%rcx, %rcx, %rcx
	movq	128(%rsp), %rdx                 # 8-byte Reload
	shrl	$20, %ecx
	vmovd	%ecx, %xmm0
	movq	136(%rsp), %rcx                 # 8-byte Reload
	vpinsrb	$1, %r15d, %xmm0, %xmm0
	movq	32(%rsp), %r15                  # 8-byte Reload
	vpinsrb	$2, %r12d, %xmm0, %xmm0
	vpinsrb	$3, %r8d, %xmm0, %xmm0
	vpinsrb	$4, %eax, %xmm0, %xmm0
	imulq	$112589991, 16(%rsp), %rax      # 8-byte Folded Reload
                                        # imm = 0x6B5FCA7
	vpinsrb	$5, %ebp, %xmm0, %xmm0
	movq	8(%rsp), %rbp                   # 8-byte Reload
	vpinsrb	$6, %r13d, %xmm0, %xmm0
	vpinsrb	$7, %r14d, %xmm0, %xmm0
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	shrq	$50, %rax
	vpinsrb	$8, %eax, %xmm0, %xmm0
	imull	$8389, 64(%rsp), %eax           # 4-byte Folded Reload
                                        # imm = 0x20C5
	vpinsrb	$9, %r10d, %xmm0, %xmm0
	vpinsrb	$10, %r9d, %xmm0, %xmm0
	vpinsrb	$11, %edi, %xmm0, %xmm0
	shrl	$23, %eax
	vpinsrb	$12, %eax, %xmm0, %xmm0
	movw	$4113, %ax                      # imm = 0x1011
	vpinsrb	$13, %r11d, %xmm0, %xmm1
	kmovd	%eax, %k1
	vpinsrb	$14, %esi, %xmm1, %xmm1
	vpinsrb	$15, %ebx, %xmm1, %xmm1
	vpord	.LCPI17_42(%rip){1to4}, %xmm1, %xmm1
	movq	120(%rsp), %rbx                 # 8-byte Reload
	vpaddb	.LCPI17_26(%rip), %xmm0, %xmm1 {%k1}
	vmovdqu	%xmm1, (%rbp)
.LBB17_133:                             #   in Loop: Header=BB17_129 Depth=2
	cmpq	$8, 40(%rsp)                    # 8-byte Folded Reload
	movq	%r15, %rax
	jb	.LBB17_137
# %bb.134:                              #   in Loop: Header=BB17_129 Depth=2
	vpbroadcastb	.LCPI17_43(%rip), %xmm2 # xmm2 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	vpbroadcastb	.LCPI17_43(%rip), %xmm3 # xmm3 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	xorl	%esi, %esi
	movq	%rcx, 64(%rsp)                  # 8-byte Spill
	movq	%rdx, 80(%rsp)                  # 8-byte Spill
	.p2align	4, 0x90
.LBB17_135:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_129 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rdx,%rsi), %rcx
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rsi, 16(%rsp)                  # 8-byte Spill
	movabsq	$2535301200456458803, %rsi      # imm = 0x232F33025BD42233
	movq	%rcx, %rdx
	mulxq	%rax, %r8, %r8
	movq	%rcx, %rax
	shrq	$26, %r8
	imulq	$100000000, %r8, %rdx           # imm = 0x5F5E100
	subq	%rdx, %rax
	movq	%rcx, %rdx
	mulxq	%rsi, %r10, %r10
	movq	%r8, %rdx
	mulxq	%r14, %rdx, %rdx
	movabsq	$811296384146066817, %rsi       # imm = 0xB424DC35095CD81
	imulq	$109951163, %rax, %r9           # imm = 0x68DB8BB
	movq	%rax, 72(%rsp)                  # 8-byte Spill
	movl	%eax, %ebp
	imulq	$140737489, %rax, %r14          # imm = 0x8637BD1
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r10
	shrq	$40, %r9
	shrq	$47, %r14
	subl	%edx, %r8d
	imull	$10000, %r9d, %edx              # imm = 0x2710
	imull	$5243, %r8d, %r12d              # imm = 0x147B
	movl	%r8d, %r15d
	subl	%edx, %ebp
	movq	%rcx, %rdx
	mulxq	%rsi, %r13, %r13
	imulq	$42949673, %r10, %rdx           # imm = 0x28F5C29
	shrq	$15, %rcx
	shrl	$19, %r12d
	imull	$5243, %ebp, %esi               # imm = 0x147B
	movl	%ebp, 104(%rsp)                 # 4-byte Spill
	shrq	$32, %rdx
	shrq	$42, %r13
	imull	$100, %edx, %edx
	shrl	$19, %esi
	subl	%edx, %r10d
	imull	$100, %r12d, %edx
	subl	%edx, %r15d
	imull	$5243, %r9d, %edx               # imm = 0x147B
	shrl	$19, %edx
	imull	$100, %edx, %edx
	subl	%edx, %r9d
	imull	$100, %esi, %edx
	subl	%edx, %ebp
	imulq	$429496730, %r13, %rdx          # imm = 0x1999999A
	shrq	$32, %rdx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subl	%edx, %r13d
	imull	$103, %r10d, %edx
	movzwl	%dx, %eax
	shrl	$10, %eax
	leal	(%rax,%rax), %edx
	movq	%rax, 152(%rsp)                 # 8-byte Spill
	imull	$8389, %r8d, %eax               # imm = 0x20C5
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r10b
	imull	$205, %r12d, %edx
	shrl	$23, %eax
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r12b
	imull	$103, %r15d, %edx
	movzwl	%dx, %ebx
	shrl	$10, %ebx
	leal	(%rbx,%rbx), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r15b
	imull	$205, %r14d, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r14b
	imull	$103, %r9d, %edx
	movzwl	%dx, %r11d
	shrl	$10, %r11d
	leal	(%r11,%r11), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r9b
	imull	$205, %esi, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %sil
	imull	$103, %ebp, %edx
	movzwl	%dx, %edi
	shrl	$10, %edi
	leal	(%rdi,%rdi), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %bpl
	movq	%rcx, %rdx
	movabsq	$633825300114115, %rcx          # imm = 0x24075F3DCEAC3
	mulxq	%rcx, %rcx, %rcx
	imull	$8389, 104(%rsp), %edx          # 4-byte Folded Reload
                                        # imm = 0x20C5
	shrl	$20, %ecx
	vmovd	%ecx, %xmm0
	imulq	$112589991, 72(%rsp), %rcx      # 8-byte Folded Reload
                                        # imm = 0x6B5FCA7
	vpinsrb	$1, %r13d, %xmm0, %xmm0
	vpinsrb	$2, 152(%rsp), %xmm0, %xmm0     # 4-byte Folded Reload
	shrl	$23, %edx
	shrq	$50, %rcx
	vpinsrb	$3, %r10d, %xmm0, %xmm0
	vpinsrb	$4, %eax, %xmm0, %xmm0
	movw	$4113, %ax                      # imm = 0x1011
	vpinsrb	$5, %r12d, %xmm0, %xmm0
	kmovd	%eax, %k1
	movq	64(%rsp), %rax                  # 8-byte Reload
	vpinsrb	$6, %ebx, %xmm0, %xmm0
	vpinsrb	$7, %r15d, %xmm0, %xmm0
	vpinsrb	$8, %ecx, %xmm0, %xmm0
	movq	16(%rsp), %rcx                  # 8-byte Reload
	vpinsrb	$9, %r14d, %xmm0, %xmm0
	vpinsrb	$10, %r11d, %xmm0, %xmm0
	vpinsrb	$11, %r9d, %xmm0, %xmm0
	vpinsrb	$12, %edx, %xmm0, %xmm0
	vpinsrb	$13, %esi, %xmm0, %xmm1
	vpinsrb	$14, %edi, %xmm1, %xmm1
	vpinsrb	$15, %ebp, %xmm1, %xmm1
	vpor	%xmm3, %xmm1, %xmm1
	vpaddb	%xmm2, %xmm0, %xmm1 {%k1}
	vmovdqu	%xmm1, (%rax,%rcx,2)
	movq	80(%rsp), %rax                  # 8-byte Reload
	movq	16(%rsp), %rcx                  # 8-byte Reload
	movq	8(%rax,%rcx), %rcx
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movq	%rcx, %rdx
	mulxq	%rax, %rax, %rax
	movq	%rcx, %rsi
	shrq	$26, %rax
	imulq	$100000000, %rax, %rdx          # imm = 0x5F5E100
	subq	%rdx, %rsi
	movq	%rcx, %rdx
	movq	%rsi, %rdi
	movabsq	$2535301200456458803, %rsi      # imm = 0x232F33025BD42233
	imulq	$109951163, %rdi, %r10          # imm = 0x68DB8BB
	imulq	$140737489, %rdi, %r14          # imm = 0x8637BD1
	movl	%edi, %r8d
	mulxq	%rsi, %r11, %r11
	movabsq	$1844674407370956, %rsi         # imm = 0x68DB8BAC710CC
	movq	%rax, %rdx
	mulxq	%rsi, %rdx, %rdx
	movq	%rdi, 72(%rsp)                  # 8-byte Spill
	movabsq	$811296384146066817, %rdi       # imm = 0xB424DC35095CD81
	shrq	$40, %r10
	shrq	$47, %r14
	shrq	$37, %r11
	imull	$10000, %r10d, %esi             # imm = 0x2710
	imull	$10000, %edx, %edx              # imm = 0x2710
	subl	%esi, %r8d
	subl	%edx, %eax
	movq	%rcx, %rdx
	mulxq	%rdi, %r15, %r15
	imulq	$42949673, %r11, %rdx           # imm = 0x28F5C29
	imull	$5243, %r8d, %esi               # imm = 0x147B
	movl	%r8d, %edi
	movl	%r8d, 104(%rsp)                 # 4-byte Spill
	shrq	$15, %rcx
	imull	$5243, %eax, %ebp               # imm = 0x147B
	movl	%eax, %r12d
	imull	$8389, %eax, %eax               # imm = 0x20C5
	shrq	$32, %rdx
	shrl	$19, %esi
	shrq	$42, %r15
	imull	$100, %edx, %edx
	shrl	$19, %ebp
	shrl	$23, %eax
	subl	%edx, %r11d
	imull	$100, %ebp, %edx
	subl	%edx, %r12d
	imull	$5243, %r10d, %edx              # imm = 0x147B
	shrl	$19, %edx
	imull	$100, %edx, %edx
	subl	%edx, %r10d
	imull	$100, %esi, %edx
	imull	$103, %r10d, %r9d
	subl	%edx, %edi
	imulq	$429496730, %r15, %rdx          # imm = 0x1999999A
	movzwl	%r9w, %ebx
	shrl	$10, %ebx
	shrq	$32, %rdx
	leal	(%rbx,%rbx), %r9d
	addl	%edx, %edx
	leal	(%r9,%r9,4), %r9d
	leal	(%rdx,%rdx,4), %edx
	subb	%r9b, %r10b
	subl	%edx, %r15d
	imull	$103, %r11d, %edx
	movzwl	%dx, %r13d
	shrl	$10, %r13d
	leal	(%r13,%r13), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r11b
	imull	$205, %ebp, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %bpl
	imull	$103, %r12d, %edx
	movzwl	%dx, %r8d
	shrl	$10, %r8d
	leal	(%r8,%r8), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r12b
	imull	$205, %r14d, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %r14b
	imull	$205, %esi, %edx
	movzwl	%dx, %edx
	shrl	$11, %edx
	addl	%edx, %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %sil
	imull	$103, %edi, %edx
	movzwl	%dx, %r9d
	shrl	$10, %r9d
	leal	(%r9,%r9), %edx
	leal	(%rdx,%rdx,4), %edx
	subb	%dl, %dil
	movq	%rcx, %rdx
	movabsq	$633825300114115, %rcx          # imm = 0x24075F3DCEAC3
	mulxq	%rcx, %rcx, %rcx
	movq	80(%rsp), %rdx                  # 8-byte Reload
	shrl	$20, %ecx
	vmovd	%ecx, %xmm0
	movq	64(%rsp), %rcx                  # 8-byte Reload
	vpinsrb	$1, %r15d, %xmm0, %xmm0
	vpinsrb	$2, %r13d, %xmm0, %xmm0
	vpinsrb	$3, %r11d, %xmm0, %xmm0
	vpinsrb	$4, %eax, %xmm0, %xmm0
	imulq	$112589991, 72(%rsp), %rax      # 8-byte Folded Reload
                                        # imm = 0x6B5FCA7
	vpinsrb	$5, %ebp, %xmm0, %xmm0
	vpinsrb	$6, %r8d, %xmm0, %xmm0
	imull	$8389, 104(%rsp), %r8d          # 4-byte Folded Reload
                                        # imm = 0x20C5
	vpinsrb	$7, %r12d, %xmm0, %xmm0
	shrq	$50, %rax
	vpinsrb	$8, %eax, %xmm0, %xmm0
	vpinsrb	$9, %r14d, %xmm0, %xmm0
	shrl	$23, %r8d
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	vpinsrb	$10, %ebx, %xmm0, %xmm0
	vpinsrb	$11, %r10d, %xmm0, %xmm0
	vpinsrb	$12, %r8d, %xmm0, %xmm0
	vpinsrb	$13, %esi, %xmm0, %xmm1
	movq	16(%rsp), %rsi                  # 8-byte Reload
	vpinsrb	$14, %r9d, %xmm1, %xmm1
	vpinsrb	$15, %edi, %xmm1, %xmm1
	vpor	%xmm3, %xmm1, %xmm1
	vpaddb	%xmm2, %xmm0, %xmm1 {%k1}
	leaq	8(%rdx,%rsi), %rax
	vmovdqu	%xmm1, 16(%rcx,%rsi,2)
	addq	$16, %rsi
	cmpq	56(%rsp), %rax                  # 8-byte Folded Reload
	jne	.LBB17_135
# %bb.136:                              #   in Loop: Header=BB17_129 Depth=2
	movq	32(%rsp), %r15                  # 8-byte Reload
	movq	8(%rsp), %rbp                   # 8-byte Reload
	movq	120(%rsp), %rbx                 # 8-byte Reload
	movq	%r15, %rax
	jmp	.LBB17_137
	.p2align	4, 0x90
.LBB17_138:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm2, %xmm0
	vcvtusi2sd	%rbx, %xmm2, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp68:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp69:
# %bb.139:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.140:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_142
# %bb.141:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_144
	.p2align	4, 0x90
.LBB17_142:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp70:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp71:
# %bb.143:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp72:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp73:
.LBB17_144:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp74:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp75:
# %bb.145:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp76:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp77:
# %bb.146:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp78:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$8, %edx
	leaq	.L.str.6(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp79:
# %bb.147:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	(%rsp), %rbx                    # 8-byte Reload
	movq	48(%rsp), %rbp                  # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	xorl	%r15d, %r15d
	movq	%rax, 64(%rsp)                  # 8-byte Spill
	jmp	.LBB17_148
	.p2align	4, 0x90
.LBB17_149:                             #   in Loop: Header=BB17_148 Depth=2
	xorl	%eax, %eax
.LBB17_157:                             #   in Loop: Header=BB17_148 Depth=2
	sarq	$3, %rax
	addq	%rax, %r15
	vzeroupper
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	64(%rsp), %rax                  # 8-byte Folded Reload
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_158
.LBB17_148:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_152 Depth 3
                                        #       Child Loop BB17_155 Depth 3
	cmpq	%rbp, %rbx
	je	.LBB17_149
# %bb.150:                              #   in Loop: Header=BB17_148 Depth=2
	vmovdqa	.LCPI17_27(%rip), %xmm6         # xmm6 = [10,4295380]
	vmovdqa64	.LCPI17_28(%rip), %zmm7 # zmm7 = [0,1,2,8,9,u,u,u]
	vpbroadcastb	.LCPI17_43(%rip), %xmm8 # xmm8 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	vpbroadcastb	.LCPI17_43(%rip), %xmm9 # xmm9 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	movq	8(%rsp), %rcx                   # 8-byte Reload
	cmpq	$0, 144(%rsp)                   # 8-byte Folded Reload
	movq	%r15, 16(%rsp)                  # 8-byte Spill
	movq	%rbx, %rsi
	movabsq	$2535301200456458803, %r13      # imm = 0x232F33025BD42233
	movb	$64, %bpl
	movb	$-128, %r15b
	movw	$4369, %bx                      # imm = 0x1111
	je	.LBB17_153
# %bb.151:                              #   in Loop: Header=BB17_148 Depth=2
	movq	144(%rsp), %rdi                 # 8-byte Reload
	movq	8(%rsp), %rcx                   # 8-byte Reload
	movq	(%rsp), %rsi                    # 8-byte Reload
	.p2align	4, 0x90
.LBB17_152:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_148 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rsi), %rdx
	kmovd	%ebp, %k2
	kmovd	%r15d, %k3
	addq	$8, %rsi
	mulxq	%r12, %rax, %rax
	mulxq	%r13, %r11, %r11
	movq	%rdx, %r9
	shrq	$26, %rax
	shrq	$37, %r11
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	subq	%r8, %r9
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	imulq	$109951163, %r9, %r10           # imm = 0x68DB8BB
	subq	%rdx, %rax
	shrq	$40, %r10
	vmovq	%rax, %xmm1
	imull	$10000, %r10d, %edx             # imm = 0x2710
	subl	%edx, %r9d
	imulq	$4295380, %r11, %rdx            # imm = 0x418AD4
	vmovd	%r9d, %xmm2
	movl	%edx, %r8d
	addq	%r8, %r8
	vmovq	%rdx, %xmm4
	leaq	(%r8,%r8,4), %r8
	movl	%r8d, %eax
	addq	%rax, %rax
	vmovq	%r8, %xmm5
	movb	$32, %r8b
	leaq	(%rax,%rax,4), %rax
	vpunpcklqdq	%xmm5, %xmm4, %xmm4     # xmm4 = xmm4[0],xmm5[0]
	kmovd	%r8d, %k1
	vmovq	%rax, %xmm0
	imulq	$4295380, %r10, %rax            # imm = 0x418AD4
	vpunpcklqdq	%xmm1, %xmm0, %xmm1     # xmm1 = xmm0[0],xmm1[0]
	vinserti128	$1, %xmm0, %ymm4, %ymm0
	vpmuludq	%xmm6, %xmm1, %xmm1
	movl	%eax, %r10d
	addq	%r10, %r10
	leaq	(%r10,%r10,4), %r10
	vpextrq	$1, %xmm1, %rdx
	vpermt2q	%zmm1, %zmm7, %zmm0
	vmovq	%rax, %xmm1
	movl	%r10d, %r9d
	addq	%r9, %r9
	vmovq	%r10, %xmm4
	movl	%edx, %edx
	addq	%rdx, %rdx
	leaq	(%r9,%r9,4), %r9
	vpunpcklqdq	%xmm4, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm4[0]
	leaq	(%rdx,%rdx,4), %rdx
	vmovq	%r9, %xmm3
	vpunpcklqdq	%xmm2, %xmm3, %xmm2     # xmm2 = xmm3[0],xmm2[0]
	vinserti128	$1, %xmm3, %ymm1, %ymm1
	vpbroadcastq	%rdx, %zmm0 {%k1}
	movl	%edx, %edx
	addq	%rdx, %rdx
	vpmuludq	%xmm6, %xmm2, %xmm2
	leaq	(%rdx,%rdx,4), %rdx
	vpbroadcastq	%rdx, %zmm0 {%k2}
	movl	%edx, %edx
	addq	%rdx, %rdx
	vpermt2q	%zmm2, %zmm7, %zmm1
	vpextrq	$1, %xmm2, %rax
	leaq	(%rdx,%rdx,4), %rdx
	movl	%eax, %eax
	addq	%rax, %rax
	leaq	(%rax,%rax,4), %rax
	vpbroadcastq	%rdx, %zmm0 {%k3}
	vpsrlq	$32, %zmm0, %zmm0
	vpmovqb	%zmm0, %xmm0
	vpbroadcastq	%rax, %zmm1 {%k1}
	movl	%eax, %eax
	addq	%rax, %rax
	kmovd	%ebx, %k1
	leaq	(%rax,%rax,4), %rax
	vpbroadcastq	%rax, %zmm1 {%k2}
	movl	%eax, %eax
	addq	%rax, %rax
	leaq	(%rax,%rax,4), %rax
	vpbroadcastq	%rax, %zmm1 {%k3}
	vpsrlq	$32, %zmm1, %zmm1
	vpmovqb	%zmm1, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vpor	%xmm0, %xmm8, %xmm1
	vpaddb	%xmm9, %xmm0, %xmm1 {%k1}
	vmovdqu	%xmm1, (%rcx)
	addq	$16, %rcx
	decq	%rdi
	jne	.LBB17_152
.LBB17_153:                             #   in Loop: Header=BB17_148 Depth=2
	movq	32(%rsp), %rax                  # 8-byte Reload
	cmpq	$24, 40(%rsp)                   # 8-byte Folded Reload
	movq	(%rsp), %rbx                    # 8-byte Reload
	movq	48(%rsp), %rbp                  # 8-byte Reload
	movq	16(%rsp), %r15                  # 8-byte Reload
	jb	.LBB17_157
# %bb.154:                              #   in Loop: Header=BB17_148 Depth=2
	vmovdqa	.LCPI17_27(%rip), %xmm6         # xmm6 = [10,4295380]
	vmovdqa64	.LCPI17_28(%rip), %zmm7 # zmm7 = [0,1,2,8,9,u,u,u]
	vpbroadcastb	.LCPI17_43(%rip), %xmm8 # xmm8 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	vpbroadcastb	.LCPI17_43(%rip), %xmm9 # xmm9 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	movabsq	$2535301200456458803, %rbx      # imm = 0x232F33025BD42233
	movb	$64, %bpl
	movb	$-128, %r15b
	movw	$4369, %r13w                    # imm = 0x1111
	xorl	%edi, %edi
	.p2align	4, 0x90
.LBB17_155:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_148 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rsi,%rdi), %rdx
	kmovd	%ebp, %k2
	kmovd	%r15d, %k1
	kmovd	%r13d, %k4
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%rbx, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$109951163, %r9, %rbx           # imm = 0x68DB8BB
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$37, %r8
	shrq	$40, %rbx
	imull	$10000, %ebx, %r10d             # imm = 0x2710
	imulq	$4295380, %rbx, %rbx            # imm = 0x418AD4
	subq	%rdx, %rax
	imulq	$4295380, %r8, %rdx             # imm = 0x418AD4
	vmovq	%rax, %xmm0
	movl	%ebx, %r14d
	addq	%r14, %r14
	subl	%r10d, %r9d
	movl	%edx, %r8d
	addq	%r8, %r8
	leaq	(%r14,%r14,4), %r14
	vmovd	%r9d, %xmm2
	vmovq	%rdx, %xmm4
	leaq	(%r8,%r8,4), %r8
	movl	%r14d, %r12d
	addq	%r12, %r12
	movl	%r8d, %r11d
	addq	%r11, %r11
	leaq	(%r12,%r12,4), %r12
	vmovq	%r8, %xmm5
	movb	$32, %r8b
	leaq	(%r11,%r11,4), %r11
	vpunpcklqdq	%xmm5, %xmm4, %xmm4     # xmm4 = xmm4[0],xmm5[0]
	kmovd	%r8d, %k3
	vmovq	%r12, %xmm3
	vmovq	%r11, %xmm1
	vpunpcklqdq	%xmm2, %xmm3, %xmm2     # xmm2 = xmm3[0],xmm2[0]
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm6, %xmm2, %xmm2
	vinserti128	$1, %xmm1, %ymm4, %ymm1
	vmovq	%r14, %xmm4
	vpmuludq	%xmm6, %xmm0, %xmm0
	vpextrq	$1, %xmm2, %r9
	vpextrq	$1, %xmm0, %rax
	vpermt2q	%zmm0, %zmm7, %zmm1
	vmovq	%rbx, %xmm0
	movl	%r9d, %r9d
	addq	%r9, %r9
	movl	%eax, %eax
	addq	%rax, %rax
	vpunpcklqdq	%xmm4, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm4[0]
	leaq	(%r9,%r9,4), %r9
	leaq	(%rax,%rax,4), %rax
	vinserti128	$1, %xmm3, %ymm0, %ymm0
	vpermt2q	%zmm2, %zmm7, %zmm0
	movl	%r9d, %r12d
	addq	%r12, %r12
	movl	%eax, %r10d
	addq	%r10, %r10
	leaq	(%r12,%r12,4), %r12
	leaq	(%r10,%r10,4), %r10
	vpbroadcastq	%rax, %zmm1 {%k3}
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movl	%r12d, %edx
	addq	%rdx, %rdx
	movl	%r10d, %r11d
	addq	%r11, %r11
	leaq	(%rdx,%rdx,4), %rdx
	vpbroadcastq	%r10, %zmm1 {%k2}
	movabsq	$1844674407370956, %r10         # imm = 0x68DB8BAC710CC
	vpbroadcastq	%r9, %zmm0 {%k3}
	leaq	(%r11,%r11,4), %r11
	vpbroadcastq	%r12, %zmm0 {%k2}
	vpbroadcastq	%r11, %zmm1 {%k1}
	vpbroadcastq	%rdx, %zmm0 {%k1}
	vpsrlq	$32, %zmm0, %zmm0
	vpsrlq	$32, %zmm1, %zmm1
	vpmovqb	%zmm0, %xmm0
	vpmovqb	%zmm1, %xmm1
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpor	%xmm0, %xmm9, %xmm1
	vpaddb	%xmm8, %xmm0, %xmm1 {%k4}
	vmovdqu	%xmm1, (%rcx,%rdi,2)
	movq	8(%rsi,%rdi), %rdx
	mulxq	%rax, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	movabsq	$2535301200456458803, %r8       # imm = 0x232F33025BD42233
	mulxq	%r8, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r10, %rdx, %rdx
	imulq	$109951163, %r9, %rbx           # imm = 0x68DB8BB
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$40, %rbx
	shrq	$37, %r8
	imull	$10000, %ebx, %r11d             # imm = 0x2710
	imulq	$4295380, %rbx, %rbx            # imm = 0x418AD4
	subq	%rdx, %rax
	imulq	$4295380, %r8, %rdx             # imm = 0x418AD4
	movl	%ebx, %r14d
	addq	%r14, %r14
	subl	%r11d, %r9d
	vmovq	%rax, %xmm0
	leaq	(%r14,%r14,4), %r14
	vmovd	%r9d, %xmm2
	movl	%edx, %r8d
	addq	%r8, %r8
	vmovq	%rdx, %xmm4
	leaq	(%r8,%r8,4), %r8
	movl	%r14d, %r12d
	addq	%r12, %r12
	leaq	(%r12,%r12,4), %r12
	movl	%r8d, %r10d
	addq	%r10, %r10
	vmovq	%r8, %xmm5
	leaq	(%r10,%r10,4), %r10
	vmovq	%r12, %xmm3
	vpunpcklqdq	%xmm5, %xmm4, %xmm4     # xmm4 = xmm4[0],xmm5[0]
	vpunpcklqdq	%xmm2, %xmm3, %xmm2     # xmm2 = xmm3[0],xmm2[0]
	vmovq	%r10, %xmm1
	vpmuludq	%xmm6, %xmm2, %xmm2
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vinserti128	$1, %xmm1, %ymm4, %ymm1
	vmovq	%r14, %xmm4
	vpmuludq	%xmm6, %xmm0, %xmm0
	vpextrq	$1, %xmm2, %r9
	movl	%r9d, %r9d
	addq	%r9, %r9
	vpextrq	$1, %xmm0, %rax
	vpermt2q	%zmm0, %zmm7, %zmm1
	vmovq	%rbx, %xmm0
	leaq	(%r9,%r9,4), %r9
	movl	%eax, %eax
	addq	%rax, %rax
	vpunpcklqdq	%xmm4, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm4[0]
	leaq	(%rax,%rax,4), %rax
	vinserti128	$1, %xmm3, %ymm0, %ymm0
	movl	%r9d, %r12d
	addq	%r12, %r12
	vpermt2q	%zmm2, %zmm7, %zmm0
	leaq	(%r12,%r12,4), %r12
	movl	%eax, %r10d
	addq	%r10, %r10
	leaq	(%r10,%r10,4), %r10
	movl	%r12d, %edx
	addq	%rdx, %rdx
	vpbroadcastq	%rax, %zmm1 {%k3}
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	leaq	(%rdx,%rdx,4), %rdx
	movl	%r10d, %r11d
	addq	%r11, %r11
	vpbroadcastq	%r10, %zmm1 {%k2}
	movabsq	$2535301200456458803, %r10      # imm = 0x232F33025BD42233
	vpbroadcastq	%r9, %zmm0 {%k3}
	leaq	(%r11,%r11,4), %r11
	vpbroadcastq	%r12, %zmm0 {%k2}
	vpbroadcastq	%r11, %zmm1 {%k1}
	vpbroadcastq	%rdx, %zmm0 {%k1}
	vpsrlq	$32, %zmm0, %zmm0
	vpsrlq	$32, %zmm1, %zmm1
	vpmovqb	%zmm0, %xmm0
	vpmovqb	%zmm1, %xmm1
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpor	%xmm0, %xmm9, %xmm1
	vpaddb	%xmm8, %xmm0, %xmm1 {%k4}
	vmovdqu	%xmm1, 16(%rcx,%rdi,2)
	movq	16(%rsi,%rdi), %rdx
	mulxq	%rax, %rax, %rax
	movq	%rdx, %r9
	mulxq	%r10, %r10, %r10
	shrq	$26, %rax
	shrq	$37, %r10
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	movq	%rax, %rdx
	subq	%r8, %r9
	movabsq	$1844674407370956, %r8          # imm = 0x68DB8BAC710CC
	mulxq	%r8, %rdx, %rdx
	imulq	$109951163, %r9, %rbx           # imm = 0x68DB8BB
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$40, %rbx
	imull	$10000, %ebx, %r12d             # imm = 0x2710
	imulq	$4295380, %rbx, %rbx            # imm = 0x418AD4
	subq	%rdx, %rax
	imulq	$4295380, %r10, %rdx            # imm = 0x418AD4
	movl	%ebx, %r14d
	addq	%r14, %r14
	subl	%r12d, %r9d
	vmovq	%rax, %xmm0
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	leaq	(%r14,%r14,4), %r14
	vmovd	%r9d, %xmm2
	movl	%edx, %r8d
	addq	%r8, %r8
	vmovq	%rdx, %xmm4
	leaq	(%r8,%r8,4), %r8
	movl	%r14d, %r9d
	addq	%r9, %r9
	leaq	(%r9,%r9,4), %r9
	movl	%r8d, %r10d
	addq	%r10, %r10
	vmovq	%r8, %xmm5
	leaq	(%r10,%r10,4), %r10
	vmovq	%r9, %xmm3
	vpunpcklqdq	%xmm5, %xmm4, %xmm4     # xmm4 = xmm4[0],xmm5[0]
	vpunpcklqdq	%xmm2, %xmm3, %xmm2     # xmm2 = xmm3[0],xmm2[0]
	vmovq	%r10, %xmm1
	vpmuludq	%xmm6, %xmm2, %xmm2
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vinserti128	$1, %xmm1, %ymm4, %ymm1
	vmovq	%r14, %xmm4
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	vpmuludq	%xmm6, %xmm0, %xmm0
	vpextrq	$1, %xmm2, %rdx
	movl	%edx, %edx
	addq	%rdx, %rdx
	vpextrq	$1, %xmm0, %rax
	vpermt2q	%zmm0, %zmm7, %zmm1
	vmovq	%rbx, %xmm0
	leaq	(%rdx,%rdx,4), %rdx
	movabsq	$2535301200456458803, %rbx      # imm = 0x232F33025BD42233
	movl	%eax, %eax
	addq	%rax, %rax
	vpunpcklqdq	%xmm4, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm4[0]
	leaq	(%rax,%rax,4), %rax
	vinserti128	$1, %xmm3, %ymm0, %ymm0
	movl	%edx, %r8d
	addq	%r8, %r8
	vpermt2q	%zmm2, %zmm7, %zmm0
	leaq	(%r8,%r8,4), %r8
	movl	%eax, %r10d
	addq	%r10, %r10
	leaq	(%r10,%r10,4), %r10
	vpbroadcastq	%rax, %zmm1 {%k3}
	movl	%r8d, %eax
	addq	%rax, %rax
	leaq	(%rax,%rax,4), %rax
	movl	%r10d, %r11d
	addq	%r11, %r11
	vpbroadcastq	%r10, %zmm1 {%k2}
	vpbroadcastq	%rdx, %zmm0 {%k3}
	leaq	(%r11,%r11,4), %r11
	vpbroadcastq	%r8, %zmm0 {%k2}
	vpbroadcastq	%r11, %zmm1 {%k1}
	vpbroadcastq	%rax, %zmm0 {%k1}
	vpsrlq	$32, %zmm0, %zmm0
	vpsrlq	$32, %zmm1, %zmm1
	vpmovqb	%zmm0, %xmm0
	vpmovqb	%zmm1, %xmm1
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpor	%xmm0, %xmm9, %xmm1
	vpaddb	%xmm8, %xmm0, %xmm1 {%k4}
	vmovdqu	%xmm1, 32(%rcx,%rdi,2)
	movq	24(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%rbx, %r8, %r8
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$109951163, %r9, %r10           # imm = 0x68DB8BB
	imulq	$10000, %rdx, %rdx              # imm = 0x2710
	shrq	$37, %r8
	shrq	$40, %r10
	imull	$10000, %r10d, %r11d            # imm = 0x2710
	subq	%rdx, %rax
	imulq	$4295380, %r8, %rdx             # imm = 0x418AD4
	vmovq	%rax, %xmm1
	subl	%r11d, %r9d
	movl	%edx, %r8d
	addq	%r8, %r8
	vmovd	%r9d, %xmm2
	vmovq	%rdx, %xmm4
	leaq	(%r8,%r8,4), %r8
	movl	%r8d, %eax
	addq	%rax, %rax
	vmovq	%r8, %xmm5
	leaq	(%rax,%rax,4), %rax
	vpunpcklqdq	%xmm5, %xmm4, %xmm4     # xmm4 = xmm4[0],xmm5[0]
	vmovq	%rax, %xmm0
	imulq	$4295380, %r10, %rax            # imm = 0x418AD4
	vpunpcklqdq	%xmm1, %xmm0, %xmm1     # xmm1 = xmm0[0],xmm1[0]
	vinserti128	$1, %xmm0, %ymm4, %ymm0
	vpmuludq	%xmm6, %xmm1, %xmm1
	movl	%eax, %r10d
	addq	%r10, %r10
	leaq	(%r10,%r10,4), %r10
	vpextrq	$1, %xmm1, %rdx
	vpermt2q	%zmm1, %zmm7, %zmm0
	vmovq	%rax, %xmm1
	movl	%r10d, %r9d
	addq	%r9, %r9
	vmovq	%r10, %xmm4
	movl	%edx, %edx
	addq	%rdx, %rdx
	leaq	(%r9,%r9,4), %r9
	vpunpcklqdq	%xmm4, %xmm1, %xmm1     # xmm1 = xmm1[0],xmm4[0]
	leaq	(%rdx,%rdx,4), %rdx
	vmovq	%r9, %xmm3
	vpunpcklqdq	%xmm2, %xmm3, %xmm2     # xmm2 = xmm3[0],xmm2[0]
	vinserti128	$1, %xmm3, %ymm1, %ymm1
	vpbroadcastq	%rdx, %zmm0 {%k3}
	movl	%edx, %edx
	addq	%rdx, %rdx
	vpmuludq	%xmm6, %xmm2, %xmm2
	leaq	(%rdx,%rdx,4), %rdx
	vpbroadcastq	%rdx, %zmm0 {%k2}
	movl	%edx, %edx
	addq	%rdx, %rdx
	vpermt2q	%zmm2, %zmm7, %zmm1
	vpextrq	$1, %xmm2, %rax
	leaq	(%rdx,%rdx,4), %rdx
	movl	%eax, %eax
	addq	%rax, %rax
	leaq	(%rax,%rax,4), %rax
	vpbroadcastq	%rdx, %zmm0 {%k1}
	vpsrlq	$32, %zmm0, %zmm0
	vpmovqb	%zmm0, %xmm0
	vpbroadcastq	%rax, %zmm1 {%k3}
	movl	%eax, %eax
	addq	%rax, %rax
	leaq	(%rax,%rax,4), %rax
	vpbroadcastq	%rax, %zmm1 {%k2}
	movl	%eax, %eax
	addq	%rax, %rax
	leaq	(%rax,%rax,4), %rax
	vpbroadcastq	%rax, %zmm1 {%k1}
	leaq	24(%rsi,%rdi), %rax
	vpsrlq	$32, %zmm1, %zmm1
	vpmovqb	%zmm1, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vpor	%xmm0, %xmm9, %xmm1
	vpaddb	%xmm8, %xmm0, %xmm1 {%k4}
	vmovdqu	%xmm1, 48(%rcx,%rdi,2)
	addq	$32, %rdi
	cmpq	56(%rsp), %rax                  # 8-byte Folded Reload
	jne	.LBB17_155
# %bb.156:                              #   in Loop: Header=BB17_148 Depth=2
	movq	32(%rsp), %rax                  # 8-byte Reload
	movq	48(%rsp), %rbp                  # 8-byte Reload
	movq	(%rsp), %rbx                    # 8-byte Reload
	movq	16(%rsp), %r15                  # 8-byte Reload
	jmp	.LBB17_157
	.p2align	4, 0x90
.LBB17_158:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm10, %xmm0
	vcvtusi2sd	%r15, %xmm10, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp80:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp81:
# %bb.159:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.160:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_162
# %bb.161:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_164
	.p2align	4, 0x90
.LBB17_162:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp82:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp83:
# %bb.163:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp84:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp85:
.LBB17_164:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp86:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp87:
# %bb.165:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp88:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp89:
# %bb.166:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp90:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$7, %edx
	leaq	.L.str.7(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp91:
# %bb.167:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	(%rsp), %rbx                    # 8-byte Reload
	xorl	%r15d, %r15d
	movq	%rax, 64(%rsp)                  # 8-byte Spill
	jmp	.LBB17_168
	.p2align	4, 0x90
.LBB17_169:                             #   in Loop: Header=BB17_168 Depth=2
	xorl	%eax, %eax
.LBB17_177:                             #   in Loop: Header=BB17_168 Depth=2
	sarq	$3, %rax
	addq	%rax, %r15
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	64(%rsp), %rax                  # 8-byte Folded Reload
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_178
.LBB17_168:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_172 Depth 3
                                        #       Child Loop BB17_175 Depth 3
	cmpq	%rbp, %rbx
	je	.LBB17_169
# %bb.170:                              #   in Loop: Header=BB17_168 Depth=2
	movq	8(%rsp), %rsi                   # 8-byte Reload
	testb	$3, 88(%rsp)                    # 1-byte Folded Reload
	movabsq	$-6067343680855748867, %rax     # imm = 0xABCC77118461CEFD
	movabsq	$2535301200456458803, %r10      # imm = 0x232F33025BD42233
	leaq	_ZZ20to_string_tree_tablemPcE5table(%rip), %rbp
	movabsq	$811296384146066817, %rdi       # imm = 0xB424DC35095CD81
	movq	%rbx, %r8
	movq	%r15, 16(%rsp)                  # 8-byte Spill
	je	.LBB17_173
# %bb.171:                              #   in Loop: Header=BB17_168 Depth=2
	movq	144(%rsp), %r13                 # 8-byte Reload
	movq	8(%rsp), %rsi                   # 8-byte Reload
	movq	(%rsp), %r8                     # 8-byte Reload
	.p2align	4, 0x90
.LBB17_172:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_168 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%r8), %rcx
	movq	%rax, %r15
	addq	$8, %r8
	movq	%rcx, %rdx
	mulxq	%rax, %rax, %rax
	movq	%rcx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %rdx          # imm = 0x5F5E100
	subq	%rdx, %r9
	movq	%rcx, %rdx
	mulxq	%r10, %rbx, %rbx
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	imulq	$109951163, %r9, %r11           # imm = 0x68DB8BB
	movl	%r9d, %r10d
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %rbx
	shrq	$40, %r11
	subl	%edx, %eax
	imull	$10000, %r11d, %edx             # imm = 0x2710
	imull	$5243, %eax, %r12d              # imm = 0x147B
	subl	%edx, %r10d
	imulq	$42949673, %rbx, %rdx           # imm = 0x28F5C29
	shrl	$19, %r12d
	imull	$5243, %r10d, %r14d             # imm = 0x147B
	shrq	$32, %rdx
	imull	$100, %edx, %edx
	shrl	$19, %r14d
	subl	%edx, %ebx
	imull	$100, %r12d, %edx
	subl	%edx, %eax
	imull	$5243, %r11d, %edx              # imm = 0x147B
	movzwl	%ax, %eax
	shrl	$19, %edx
	imull	$100, %edx, %edx
	subl	%edx, %r11d
	imull	$100, %r14d, %edx
	subl	%edx, %r10d
	movq	%rcx, %rdx
	mulxq	%rdi, %rcx, %rcx
	movzwl	(%rbp,%rax,2), %eax
	shrq	$42, %rcx
	movzwl	(%rbp,%rcx,2), %ecx
	movw	%cx, (%rsi)
	movzwl	(%rbp,%rbx,2), %ecx
	movw	%cx, 2(%rsi)
	movzwl	(%rbp,%r12,2), %ecx
	movw	%cx, 4(%rsi)
	movw	%ax, 6(%rsi)
	imulq	$140737489, %r9, %rax           # imm = 0x8637BD1
	movzwl	(%rbp,%r14,2), %ecx
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	shrq	$47, %rax
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 8(%rsi)
	movzwl	%r11w, %eax
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 10(%rsi)
	movzwl	%r10w, %eax
	movw	%cx, 12(%rsi)
	movabsq	$2535301200456458803, %r10      # imm = 0x232F33025BD42233
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 14(%rsi)
	addq	$16, %rsi
	decq	%r13
	movq	%r15, %rax
	jne	.LBB17_172
.LBB17_173:                             #   in Loop: Header=BB17_168 Depth=2
	movq	32(%rsp), %rax                  # 8-byte Reload
	cmpq	$24, 40(%rsp)                   # 8-byte Folded Reload
	movq	48(%rsp), %rbp                  # 8-byte Reload
	movq	(%rsp), %rbx                    # 8-byte Reload
	movq	16(%rsp), %r15                  # 8-byte Reload
	jb	.LBB17_177
# %bb.174:                              #   in Loop: Header=BB17_168 Depth=2
	leaq	_ZZ20to_string_tree_tablemPcE5table(%rip), %rbp
	movq	%r8, %rdi
	xorl	%r8d, %r8d
	.p2align	4, 0x90
.LBB17_175:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_168 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rdi,%r8), %rcx
	movabsq	$-6067343680855748867, %r15     # imm = 0xABCC77118461CEFD
	movabsq	$2535301200456458803, %r13      # imm = 0x232F33025BD42233
	movq	%rcx, %rdx
	mulxq	%r15, %rax, %rax
	mulxq	%r13, %r10, %r10
	movq	%rcx, %r11
	shrq	$26, %rax
	shrq	$37, %r10
	imulq	$100000000, %rax, %r9           # imm = 0x5F5E100
	movq	%rax, %rdx
	mulxq	%r14, %rdx, %rdx
	subq	%r9, %r11
	imull	$10000, %edx, %edx              # imm = 0x2710
	imulq	$109951163, %r11, %r9           # imm = 0x68DB8BB
	imulq	$140737489, %r11, %rbx          # imm = 0x8637BD1
                                        # kill: def $r11d killed $r11d killed $r11
	subl	%edx, %eax
	shrq	$40, %r9
	shrq	$47, %rbx
	imull	$10000, %r9d, %edx              # imm = 0x2710
	subl	%edx, %r11d
	movq	%rcx, %rdx
	movabsq	$811296384146066817, %rcx       # imm = 0xB424DC35095CD81
	mulxq	%rcx, %rcx, %rcx
	imulq	$42949673, %r10, %rdx           # imm = 0x28F5C29
	shrq	$32, %rdx
	shrq	$42, %rcx
	imull	$100, %edx, %edx
	movzwl	(%rbp,%rcx,2), %ecx
	subl	%edx, %r10d
	imull	$5243, %eax, %edx               # imm = 0x147B
	movw	%cx, (%rsi,%r8,2)
	movzwl	(%rbp,%r10,2), %ecx
	shrl	$19, %edx
	imull	$100, %edx, %r14d
	subl	%r14d, %eax
	movw	%cx, 2(%rsi,%r8,2)
	movzwl	(%rbp,%rdx,2), %ecx
	imull	$5243, %r9d, %r14d              # imm = 0x147B
	movzwl	%ax, %eax
	movzwl	(%rbp,%rax,2), %eax
	shrl	$19, %r14d
	imull	$100, %r14d, %r14d
	movw	%cx, 4(%rsi,%r8,2)
	movw	%ax, 6(%rsi,%r8,2)
	movzwl	(%rbp,%rbx,2), %eax
	subl	%r14d, %r9d
	imull	$5243, %r11d, %r14d             # imm = 0x147B
	shrl	$19, %r14d
	imull	$100, %r14d, %r12d
	movw	%ax, 8(%rsi,%r8,2)
	movzwl	%r9w, %eax
	movabsq	$1844674407370956, %r9          # imm = 0x68DB8BAC710CC
	movzwl	(%rbp,%rax,2), %eax
	subl	%r12d, %r11d
	movw	%ax, 10(%rsi,%r8,2)
	movzwl	(%rbp,%r14,2), %eax
	movw	%ax, 12(%rsi,%r8,2)
	movzwl	%r11w, %eax
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 14(%rsi,%r8,2)
	movq	8(%rdi,%r8), %rcx
	movq	%rcx, %rdx
	mulxq	%r15, %rax, %rax
	movq	%rcx, %r11
	shrq	$26, %rax
	imulq	$100000000, %rax, %rdx          # imm = 0x5F5E100
	subq	%rdx, %r11
	movq	%rcx, %rdx
	mulxq	%r13, %r10, %r10
	movq	%rax, %rdx
	mulxq	%r9, %rdx, %rdx
	imulq	$109951163, %r11, %r9           # imm = 0x68DB8BB
	imulq	$140737489, %r11, %rbx          # imm = 0x8637BD1
                                        # kill: def $r11d killed $r11d killed $r11
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r10
	shrq	$40, %r9
	shrq	$47, %rbx
	imull	$10000, %r9d, %r14d             # imm = 0x2710
	subl	%edx, %eax
	movq	%rcx, %rdx
	movabsq	$811296384146066817, %rcx       # imm = 0xB424DC35095CD81
	mulxq	%rcx, %rcx, %rcx
	imulq	$42949673, %r10, %rdx           # imm = 0x28F5C29
	subl	%r14d, %r11d
	shrq	$32, %rdx
	shrq	$42, %rcx
	imull	$100, %edx, %edx
	movzwl	(%rbp,%rcx,2), %ecx
	subl	%edx, %r10d
	imull	$5243, %eax, %edx               # imm = 0x147B
	movw	%cx, 16(%rsi,%r8,2)
	movzwl	(%rbp,%r10,2), %ecx
	shrl	$19, %edx
	imull	$100, %edx, %r14d
	subl	%r14d, %eax
	movw	%cx, 18(%rsi,%r8,2)
	movzwl	(%rbp,%rdx,2), %ecx
	imull	$5243, %r9d, %r14d              # imm = 0x147B
	movzwl	%ax, %eax
	movzwl	(%rbp,%rax,2), %eax
	shrl	$19, %r14d
	imull	$100, %r14d, %r14d
	movw	%cx, 20(%rsi,%r8,2)
	movw	%ax, 22(%rsi,%r8,2)
	movzwl	(%rbp,%rbx,2), %eax
	subl	%r14d, %r9d
	imull	$5243, %r11d, %r14d             # imm = 0x147B
	shrl	$19, %r14d
	imull	$100, %r14d, %r12d
	movw	%ax, 24(%rsi,%r8,2)
	movzwl	%r9w, %eax
	movzwl	(%rbp,%rax,2), %eax
	subl	%r12d, %r11d
	movw	%ax, 26(%rsi,%r8,2)
	movzwl	(%rbp,%r14,2), %eax
	movw	%ax, 28(%rsi,%r8,2)
	movzwl	%r11w, %eax
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 30(%rsi,%r8,2)
	movq	16(%rdi,%r8), %rcx
	movq	%rcx, %rdx
	mulxq	%r15, %rax, %rax
	movq	%rcx, %r11
	mulxq	%r13, %r10, %r10
	shrq	$26, %rax
	shrq	$37, %r10
	imulq	$100000000, %rax, %r9           # imm = 0x5F5E100
	movq	%rax, %rdx
	subq	%r9, %r11
	movabsq	$1844674407370956, %r9          # imm = 0x68DB8BAC710CC
	mulxq	%r9, %rdx, %rdx
	imulq	$109951163, %r11, %r9           # imm = 0x68DB8BB
	imulq	$140737489, %r11, %rbx          # imm = 0x8637BD1
                                        # kill: def $r11d killed $r11d killed $r11
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$40, %r9
	shrq	$47, %rbx
	subl	%edx, %eax
	imull	$10000, %r9d, %edx              # imm = 0x2710
	subl	%edx, %r11d
	movq	%rcx, %rdx
	movabsq	$811296384146066817, %rcx       # imm = 0xB424DC35095CD81
	mulxq	%rcx, %rcx, %rcx
	imulq	$42949673, %r10, %rdx           # imm = 0x28F5C29
	shrq	$32, %rdx
	shrq	$42, %rcx
	imull	$100, %edx, %edx
	movzwl	(%rbp,%rcx,2), %ecx
	subl	%edx, %r10d
	imull	$5243, %eax, %edx               # imm = 0x147B
	movw	%cx, 32(%rsi,%r8,2)
	movzwl	(%rbp,%r10,2), %ecx
	movabsq	$1844674407370956, %r10         # imm = 0x68DB8BAC710CC
	shrl	$19, %edx
	imull	$100, %edx, %r14d
	subl	%r14d, %eax
	movw	%cx, 34(%rsi,%r8,2)
	movzwl	(%rbp,%rdx,2), %ecx
	imull	$5243, %r9d, %r14d              # imm = 0x147B
	movzwl	%ax, %eax
	movzwl	(%rbp,%rax,2), %eax
	shrl	$19, %r14d
	imull	$100, %r14d, %r14d
	movw	%cx, 36(%rsi,%r8,2)
	movw	%ax, 38(%rsi,%r8,2)
	movzwl	(%rbp,%rbx,2), %eax
	subl	%r14d, %r9d
	imull	$5243, %r11d, %r14d             # imm = 0x147B
	shrl	$19, %r14d
	imull	$100, %r14d, %r12d
	movw	%ax, 40(%rsi,%r8,2)
	movzwl	%r9w, %eax
	movzwl	(%rbp,%rax,2), %eax
	subl	%r12d, %r11d
	movw	%ax, 42(%rsi,%r8,2)
	movzwl	(%rbp,%r14,2), %eax
	movw	%ax, 44(%rsi,%r8,2)
	movzwl	%r11w, %eax
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 46(%rsi,%r8,2)
	movq	24(%rdi,%r8), %rcx
	movq	%rcx, %rdx
	mulxq	%r15, %rax, %rax
	movq	%rcx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %rdx          # imm = 0x5F5E100
	subq	%rdx, %r9
	movq	%rcx, %rdx
	mulxq	%r13, %rbx, %rbx
	movq	%rax, %rdx
	mulxq	%r10, %rdx, %rdx
	imulq	$109951163, %r9, %r11           # imm = 0x68DB8BB
	movl	%r9d, %r10d
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %rbx
	shrq	$40, %r11
	subl	%edx, %eax
	imull	$10000, %r11d, %edx             # imm = 0x2710
	imull	$5243, %eax, %r12d              # imm = 0x147B
	subl	%edx, %r10d
	imulq	$42949673, %rbx, %rdx           # imm = 0x28F5C29
	shrl	$19, %r12d
	imull	$5243, %r10d, %r14d             # imm = 0x147B
	shrq	$32, %rdx
	imull	$100, %edx, %edx
	shrl	$19, %r14d
	imull	$100, %r14d, %r15d
	subl	%edx, %ebx
	imull	$100, %r12d, %edx
	subl	%r15d, %r10d
	subl	%edx, %eax
	imull	$5243, %r11d, %edx              # imm = 0x147B
	movzwl	%ax, %eax
	shrl	$19, %edx
	imull	$100, %edx, %edx
	subl	%edx, %r11d
	movq	%rcx, %rdx
	movabsq	$811296384146066817, %rcx       # imm = 0xB424DC35095CD81
	mulxq	%rcx, %rcx, %rcx
	movzwl	(%rbp,%rax,2), %eax
	shrq	$42, %rcx
	movzwl	(%rbp,%rcx,2), %ecx
	movw	%cx, 48(%rsi,%r8,2)
	movzwl	(%rbp,%rbx,2), %ecx
	movw	%cx, 50(%rsi,%r8,2)
	movzwl	(%rbp,%r12,2), %ecx
	movw	%cx, 52(%rsi,%r8,2)
	movw	%ax, 54(%rsi,%r8,2)
	imulq	$140737489, %r9, %rax           # imm = 0x8637BD1
	movzwl	(%rbp,%r14,2), %ecx
	movabsq	$1844674407370956, %r14         # imm = 0x68DB8BAC710CC
	shrq	$47, %rax
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 56(%rsi,%r8,2)
	movzwl	%r11w, %eax
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 58(%rsi,%r8,2)
	movzwl	%r10w, %eax
	movw	%cx, 60(%rsi,%r8,2)
	movzwl	(%rbp,%rax,2), %eax
	movw	%ax, 62(%rsi,%r8,2)
	leaq	24(%rdi,%r8), %rax
	addq	$32, %r8
	cmpq	56(%rsp), %rax                  # 8-byte Folded Reload
	jne	.LBB17_175
# %bb.176:                              #   in Loop: Header=BB17_168 Depth=2
	movq	32(%rsp), %rax                  # 8-byte Reload
	movq	48(%rsp), %rbp                  # 8-byte Reload
	movq	(%rsp), %rbx                    # 8-byte Reload
	movq	16(%rsp), %r15                  # 8-byte Reload
	jmp	.LBB17_177
	.p2align	4, 0x90
.LBB17_178:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm10, %xmm0
	vcvtusi2sd	%r15, %xmm10, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp92:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp93:
# %bb.179:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.180:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_182
# %bb.181:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_184
	.p2align	4, 0x90
.LBB17_182:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp94:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp95:
# %bb.183:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp96:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp97:
.LBB17_184:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp98:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp99:
# %bb.185:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp100:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp101:
# %bb.186:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp102:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$7, %edx
	leaq	.L.str.8(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp103:
# %bb.187:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	movq	(%rsp), %rbp                    # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	xorl	%ebx, %ebx
	movq	%rax, 16(%rsp)                  # 8-byte Spill
	jmp	.LBB17_188
	.p2align	4, 0x90
.LBB17_189:                             #   in Loop: Header=BB17_188 Depth=2
	xorl	%eax, %eax
.LBB17_197:                             #   in Loop: Header=BB17_188 Depth=2
	sarq	$3, %rax
	addq	%rax, %rbx
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	16(%rsp), %rax                  # 8-byte Folded Reload
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_198
.LBB17_188:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_192 Depth 3
                                        #       Child Loop BB17_195 Depth 3
	cmpq	48(%rsp), %rbp                  # 8-byte Folded Reload
	je	.LBB17_189
# %bb.190:                              #   in Loop: Header=BB17_188 Depth=2
	movq	8(%rsp), %rcx                   # 8-byte Reload
	testb	$7, 88(%rsp)                    # 1-byte Folded Reload
	movq	%rbx, %r13
	movq	%r14, %rbx
	leaq	_ZZ23to_string_tree_bigtablemPcE8bigtable(%rip), %r11
	movabsq	$2535301200456458803, %r14      # imm = 0x232F33025BD42233
	movq	%rbp, %rsi
	je	.LBB17_193
# %bb.191:                              #   in Loop: Header=BB17_188 Depth=2
	movq	96(%rsp), %rdi                  # 8-byte Reload
	movq	8(%rsp), %rcx                   # 8-byte Reload
	movq	%rbp, %rsi
	.p2align	4, 0x90
.LBB17_192:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_188 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rsi), %rdx
	addq	$8, %rsi
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%r14, %r8, %r8
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r8
	movl	(%r11,%r8,4), %r8d
	subl	%edx, %eax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	movl	(%r11,%rax,4), %eax
	shrq	$40, %rdx
	movl	%r8d, (%rcx)
	imull	$10000, %edx, %r10d             # imm = 0x2710
	movl	(%r11,%rdx,4), %r8d
	movl	%eax, 4(%rcx)
	subl	%r10d, %r9d
	movl	(%r11,%r9,4), %edx
	movl	%r8d, 8(%rcx)
	movl	%edx, 12(%rcx)
	addq	$16, %rcx
	decq	%rdi
	jne	.LBB17_192
.LBB17_193:                             #   in Loop: Header=BB17_188 Depth=2
	cmpq	$56, 40(%rsp)                   # 8-byte Folded Reload
	movq	%rbx, %r14
	movq	%r15, %rax
	movq	%r13, %rbx
	jb	.LBB17_197
# %bb.194:                              #   in Loop: Header=BB17_188 Depth=2
	movq	56(%rsp), %r15                  # 8-byte Reload
	movq	%r14, %rbx
	leaq	_ZZ23to_string_tree_bigtablemPcE8bigtable(%rip), %rbp
	movabsq	$2535301200456458803, %r14      # imm = 0x232F33025BD42233
	xorl	%edi, %edi
	.p2align	4, 0x90
.LBB17_195:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_188 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%r14, %r8, %r8
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r8
	movl	(%rbp,%r8,4), %r8d
	subl	%edx, %eax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	movl	(%rbp,%rax,4), %eax
	shrq	$40, %rdx
	movl	%r8d, (%rcx,%rdi,2)
	imull	$10000, %edx, %r10d             # imm = 0x2710
	movl	%eax, 4(%rcx,%rdi,2)
	movl	(%rbp,%rdx,4), %eax
	subl	%r10d, %r9d
	movl	%eax, 8(%rcx,%rdi,2)
	movl	(%rbp,%r9,4), %eax
	movl	%eax, 12(%rcx,%rdi,2)
	movq	8(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%r14, %r8, %r8
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r8
	movl	(%rbp,%r8,4), %r8d
	subl	%edx, %eax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	movl	(%rbp,%rax,4), %eax
	shrq	$40, %rdx
	movl	%r8d, 16(%rcx,%rdi,2)
	imull	$10000, %edx, %r10d             # imm = 0x2710
	movl	%eax, 20(%rcx,%rdi,2)
	movl	(%rbp,%rdx,4), %eax
	subl	%r10d, %r9d
	movl	%eax, 24(%rcx,%rdi,2)
	movl	(%rbp,%r9,4), %eax
	movl	%eax, 28(%rcx,%rdi,2)
	movq	16(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	mulxq	%r14, %r10, %r10
	movq	%rdx, %r9
	shrq	$26, %rax
	shrq	$37, %r10
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	imull	$10000, %edx, %edx              # imm = 0x2710
	subq	%r8, %r9
	subl	%edx, %eax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	movl	(%rbp,%rax,4), %eax
	shrq	$40, %rdx
	imull	$10000, %edx, %r8d              # imm = 0x2710
	subl	%r8d, %r9d
	movl	(%rbp,%r10,4), %r8d
	movl	%r8d, 32(%rcx,%rdi,2)
	movl	%eax, 36(%rcx,%rdi,2)
	movl	(%rbp,%rdx,4), %eax
	movl	%eax, 40(%rcx,%rdi,2)
	movl	(%rbp,%r9,4), %eax
	movl	%eax, 44(%rcx,%rdi,2)
	movq	24(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%r14, %r8, %r8
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r8
	movl	(%rbp,%r8,4), %r8d
	subl	%edx, %eax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	movl	(%rbp,%rax,4), %eax
	shrq	$40, %rdx
	movl	%r8d, 48(%rcx,%rdi,2)
	imull	$10000, %edx, %r10d             # imm = 0x2710
	movl	%eax, 52(%rcx,%rdi,2)
	movl	(%rbp,%rdx,4), %eax
	subl	%r10d, %r9d
	movl	%eax, 56(%rcx,%rdi,2)
	movl	(%rbp,%r9,4), %eax
	movl	%eax, 60(%rcx,%rdi,2)
	movq	32(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%r14, %r8, %r8
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r8
	movl	(%rbp,%r8,4), %r8d
	subl	%edx, %eax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	movl	(%rbp,%rax,4), %eax
	shrq	$40, %rdx
	movl	%r8d, 64(%rcx,%rdi,2)
	imull	$10000, %edx, %r10d             # imm = 0x2710
	movl	%eax, 68(%rcx,%rdi,2)
	movl	(%rbp,%rdx,4), %eax
	subl	%r10d, %r9d
	movl	%eax, 72(%rcx,%rdi,2)
	movl	(%rbp,%r9,4), %eax
	movl	%eax, 76(%rcx,%rdi,2)
	movq	40(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%r14, %r8, %r8
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r8
	movl	(%rbp,%r8,4), %r8d
	subl	%edx, %eax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	movl	(%rbp,%rax,4), %eax
	shrq	$40, %rdx
	movl	%r8d, 80(%rcx,%rdi,2)
	imull	$10000, %edx, %r10d             # imm = 0x2710
	movl	%eax, 84(%rcx,%rdi,2)
	movl	(%rbp,%rdx,4), %eax
	subl	%r10d, %r9d
	movl	%eax, 88(%rcx,%rdi,2)
	movl	(%rbp,%r9,4), %eax
	movl	%eax, 92(%rcx,%rdi,2)
	movq	48(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r9
	shrq	$26, %rax
	imulq	$100000000, %rax, %r8           # imm = 0x5F5E100
	subq	%r8, %r9
	mulxq	%r14, %r8, %r8
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r8
	movl	(%rbp,%r8,4), %r8d
	subl	%edx, %eax
	imulq	$109951163, %r9, %rdx           # imm = 0x68DB8BB
	movl	(%rbp,%rax,4), %eax
	shrq	$40, %rdx
	movl	%r8d, 96(%rcx,%rdi,2)
	leaq	56(%rsi,%rdi), %r8
	imull	$10000, %edx, %r10d             # imm = 0x2710
	movl	%eax, 100(%rcx,%rdi,2)
	movl	(%rbp,%rdx,4), %eax
	subl	%r10d, %r9d
	movl	%eax, 104(%rcx,%rdi,2)
	movl	(%rbp,%r9,4), %eax
	movl	%eax, 108(%rcx,%rdi,2)
	movq	56(%rsi,%rdi), %rdx
	mulxq	%r12, %rax, %rax
	movq	%rdx, %r10
	shrq	$26, %rax
	imulq	$100000000, %rax, %r9           # imm = 0x5F5E100
	subq	%r9, %r10
	mulxq	%r14, %r9, %r9
	movq	%rax, %rdx
	mulxq	%rbx, %rdx, %rdx
	imull	$10000, %edx, %edx              # imm = 0x2710
	shrq	$37, %r9
	movl	(%rbp,%r9,4), %r9d
	subl	%edx, %eax
	imulq	$109951163, %r10, %rdx          # imm = 0x68DB8BB
	movl	(%rbp,%rax,4), %eax
	shrq	$40, %rdx
	movl	%r9d, 112(%rcx,%rdi,2)
	imull	$10000, %edx, %r11d             # imm = 0x2710
	movl	(%rbp,%rdx,4), %r9d
	movl	%eax, 116(%rcx,%rdi,2)
	subl	%r11d, %r10d
	movl	(%rbp,%r10,4), %edx
	movl	%r9d, 120(%rcx,%rdi,2)
	movl	%edx, 124(%rcx,%rdi,2)
	addq	$64, %rdi
	cmpq	%r15, %r8
	jne	.LBB17_195
# %bb.196:                              #   in Loop: Header=BB17_188 Depth=2
	movq	32(%rsp), %r15                  # 8-byte Reload
	movq	(%rsp), %rbp                    # 8-byte Reload
	movq	%rbx, %r14
	movq	%r13, %rbx
	movq	%r15, %rax
	jmp	.LBB17_197
	.p2align	4, 0x90
.LBB17_198:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm2, %xmm0
	vcvtusi2sd	%rbx, %xmm2, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp104:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp105:
	movabsq	$633825300114115, %r13          # imm = 0x24075F3DCEAC3
# %bb.199:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.200:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_202
# %bb.201:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_204
	.p2align	4, 0x90
.LBB17_202:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp106:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp107:
# %bb.203:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp108:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp109:
.LBB17_204:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp110:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp111:
# %bb.205:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp112:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp113:
# %bb.206:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp114:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$7, %edx
	leaq	.L.str.9(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp115:
# %bb.207:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	movq	%rax, %rbp
	xorl	%ebx, %ebx
	jmp	.LBB17_208
	.p2align	4, 0x90
.LBB17_209:                             #   in Loop: Header=BB17_208 Depth=2
	xorl	%edx, %edx
.LBB17_217:                             #   in Loop: Header=BB17_208 Depth=2
	sarq	$3, %rdx
	addq	%rdx, %rbx
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	%rbp, %rax
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_218
.LBB17_208:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_212 Depth 3
                                        #       Child Loop BB17_215 Depth 3
	movq	(%rsp), %rcx                    # 8-byte Reload
	cmpq	48(%rsp), %rcx                  # 8-byte Folded Reload
	je	.LBB17_209
# %bb.210:                              #   in Loop: Header=BB17_208 Depth=2
	vmovdqa	176(%rsp), %xmm3                # 16-byte Reload
	vmovdqa	160(%rsp), %xmm4                # 16-byte Reload
	vpbroadcastw	.LCPI17_44(%rip), %xmm5 # xmm5 = [5243,5243,5243,5243,5243,5243,5243,5243]
	vpbroadcastw	.LCPI17_45(%rip), %xmm6 # xmm6 = [65436,65436,65436,65436,65436,65436,65436,65436]
	vpbroadcastw	.LCPI17_46(%rip), %xmm7 # xmm7 = [6554,6554,6554,6554,6554,6554,6554,6554]
	vpbroadcastb	.LCPI17_43(%rip), %xmm8 # xmm8 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	vpbroadcastw	.LCPI17_47(%rip), %xmm9 # xmm9 = [246,246,246,246,246,246,246,246]
	movq	8(%rsp), %rax                   # 8-byte Reload
	testb	$7, 88(%rsp)                    # 1-byte Folded Reload
	je	.LBB17_213
# %bb.211:                              #   in Loop: Header=BB17_208 Depth=2
	movq	96(%rsp), %rsi                  # 8-byte Reload
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	(%rsp), %rcx                    # 8-byte Reload
	.p2align	4, 0x90
.LBB17_212:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_208 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx), %rdx
	addq	$8, %rcx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm0
	subq	%r8, %rdx
	vmovq	%rdx, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, (%rax)
	addq	$16, %rax
	decq	%rsi
	jne	.LBB17_212
.LBB17_213:                             #   in Loop: Header=BB17_208 Depth=2
	cmpq	$56, 40(%rsp)                   # 8-byte Folded Reload
	movq	%r15, %rdx
	jb	.LBB17_217
# %bb.214:                              #   in Loop: Header=BB17_208 Depth=2
	vmovdqa	176(%rsp), %xmm3                # 16-byte Reload
	vmovdqa	160(%rsp), %xmm4                # 16-byte Reload
	vpbroadcastw	.LCPI17_44(%rip), %xmm5 # xmm5 = [5243,5243,5243,5243,5243,5243,5243,5243]
	vpbroadcastw	.LCPI17_45(%rip), %xmm6 # xmm6 = [65436,65436,65436,65436,65436,65436,65436,65436]
	vpbroadcastw	.LCPI17_46(%rip), %xmm7 # xmm7 = [6554,6554,6554,6554,6554,6554,6554,6554]
	vpbroadcastb	.LCPI17_43(%rip), %xmm8 # xmm8 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	vpbroadcastw	.LCPI17_47(%rip), %xmm9 # xmm9 = [246,246,246,246,246,246,246,246]
	movq	56(%rsp), %r10                  # 8-byte Reload
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB17_215:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_208 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, (%rax,%rsi,2)
	movq	8(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, 16(%rax,%rsi,2)
	movq	16(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, 32(%rax,%rsi,2)
	movq	24(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, 48(%rax,%rsi,2)
	movq	32(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, 64(%rax,%rsi,2)
	movq	40(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, 80(%rax,%rsi,2)
	movq	48(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	leaq	56(%rcx,%rsi), %rdi
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, 96(%rax,%rsi,2)
	movq	56(%rcx,%rsi), %rdx
	mulxq	%r12, %r8, %r8
	shrq	$26, %r8
	imulq	$100000000, %r8, %r9            # imm = 0x5F5E100
	vmovd	%r8d, %xmm1
	subq	%r9, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpslld	$16, %xmm0, %xmm0
	vpor	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm9, %xmm2
	vpaddw	%xmm2, %xmm0, %xmm0
	vpsllw	$8, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm8, %xmm0
	vmovdqu	%xmm0, 112(%rax,%rsi,2)
	addq	$64, %rsi
	cmpq	%r10, %rdi
	jne	.LBB17_215
# %bb.216:                              #   in Loop: Header=BB17_208 Depth=2
	movq	%r15, %rdx
	jmp	.LBB17_217
	.p2align	4, 0x90
.LBB17_218:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm10, %xmm0
	vcvtusi2sd	%rbx, %xmm10, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp116:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp117:
# %bb.219:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.220:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_222
# %bb.221:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_224
	.p2align	4, 0x90
.LBB17_222:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp118:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp119:
# %bb.223:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp120:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp121:
.LBB17_224:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp122:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp123:
# %bb.225:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp124:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp125:
# %bb.226:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp126:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$8, %edx
	leaq	.L.str.10(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp127:
# %bb.227:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	movq	%rax, %rbp
	xorl	%ebx, %ebx
	jmp	.LBB17_228
	.p2align	4, 0x90
.LBB17_229:                             #   in Loop: Header=BB17_228 Depth=2
	xorl	%edx, %edx
.LBB17_237:                             #   in Loop: Header=BB17_228 Depth=2
	sarq	$3, %rdx
	addq	%rdx, %rbx
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	%rbp, %rax
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_238
.LBB17_228:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_232 Depth 3
                                        #       Child Loop BB17_235 Depth 3
	movq	(%rsp), %rcx                    # 8-byte Reload
	cmpq	48(%rsp), %rcx                  # 8-byte Folded Reload
	je	.LBB17_229
# %bb.230:                              #   in Loop: Header=BB17_228 Depth=2
	vmovdqa	176(%rsp), %xmm3                # 16-byte Reload
	vmovdqa	160(%rsp), %xmm4                # 16-byte Reload
	vpbroadcastw	.LCPI17_44(%rip), %xmm5 # xmm5 = [5243,5243,5243,5243,5243,5243,5243,5243]
	vpbroadcastw	.LCPI17_45(%rip), %xmm6 # xmm6 = [65436,65436,65436,65436,65436,65436,65436,65436]
	vpbroadcastw	.LCPI17_46(%rip), %xmm7 # xmm7 = [6554,6554,6554,6554,6554,6554,6554,6554]
	vpbroadcastw	.LCPI17_48(%rip), %xmm8 # xmm8 = [65526,65526,65526,65526,65526,65526,65526,65526]
	vmovdqa	.LCPI17_38(%rip), %xmm9         # xmm9 = [0,8,4,12,1,9,5,13,2,10,6,14,3,11,7,15]
	vpbroadcastb	.LCPI17_43(%rip), %xmm10 # xmm10 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	movq	8(%rsp), %rax                   # 8-byte Reload
	testb	$7, 88(%rsp)                    # 1-byte Folded Reload
	je	.LBB17_233
# %bb.231:                              #   in Loop: Header=BB17_228 Depth=2
	movq	96(%rsp), %rsi                  # 8-byte Reload
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	(%rsp), %rcx                    # 8-byte Reload
	.p2align	4, 0x90
.LBB17_232:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_228 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx), %rdx
	addq	$8, %rcx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm0
	subq	%r8, %rdx
	vmovq	%rdx, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm9, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm10, %xmm0
	vmovdqu	%xmm0, (%rax)
	addq	$16, %rax
	decq	%rsi
	jne	.LBB17_232
.LBB17_233:                             #   in Loop: Header=BB17_228 Depth=2
	cmpq	$56, 40(%rsp)                   # 8-byte Folded Reload
	movq	%r15, %rdx
	jb	.LBB17_237
# %bb.234:                              #   in Loop: Header=BB17_228 Depth=2
	vmovdqa	176(%rsp), %xmm3                # 16-byte Reload
	vmovdqa	160(%rsp), %xmm4                # 16-byte Reload
	vpbroadcastw	.LCPI17_44(%rip), %xmm5 # xmm5 = [5243,5243,5243,5243,5243,5243,5243,5243]
	vpbroadcastw	.LCPI17_45(%rip), %xmm6 # xmm6 = [65436,65436,65436,65436,65436,65436,65436,65436]
	vpbroadcastw	.LCPI17_46(%rip), %xmm7 # xmm7 = [6554,6554,6554,6554,6554,6554,6554,6554]
	vpbroadcastw	.LCPI17_48(%rip), %xmm8 # xmm8 = [65526,65526,65526,65526,65526,65526,65526,65526]
	vpbroadcastb	.LCPI17_43(%rip), %xmm9 # xmm9 = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48]
	vmovdqa	.LCPI17_38(%rip), %xmm10        # xmm10 = [0,8,4,12,1,9,5,13,2,10,6,14,3,11,7,15]
	movq	56(%rsp), %r10                  # 8-byte Reload
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB17_235:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_228 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm10, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm9, %xmm0
	vmovdqu	%xmm0, (%rax,%rsi,2)
	movq	8(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm10, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm9, %xmm0
	vmovdqu	%xmm0, 16(%rax,%rsi,2)
	movq	16(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm10, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm9, %xmm0
	vmovdqu	%xmm0, 32(%rax,%rsi,2)
	movq	24(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm10, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm9, %xmm0
	vmovdqu	%xmm0, 48(%rax,%rsi,2)
	movq	32(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm10, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm9, %xmm0
	vmovdqu	%xmm0, 64(%rax,%rsi,2)
	movq	40(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm10, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm9, %xmm0
	vmovdqu	%xmm0, 80(%rax,%rsi,2)
	movq	48(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	leaq	56(%rcx,%rsi), %rdi
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm10, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm9, %xmm0
	vmovdqu	%xmm0, 96(%rax,%rsi,2)
	movq	56(%rcx,%rsi), %rdx
	mulxq	%r12, %r8, %r8
	shrq	$26, %r8
	imulq	$100000000, %r8, %r9            # imm = 0x5F5E100
	vmovd	%r8d, %xmm1
	subq	%r9, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpmulhuw	%xmm5, %xmm0, %xmm1
	vpsrlw	$3, %xmm1, %xmm1
	vpmullw	%xmm6, %xmm1, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackusdw	%xmm0, %xmm1, %xmm0
	vpmulhuw	%xmm7, %xmm0, %xmm1
	vpmullw	%xmm1, %xmm8, %xmm2
	vpaddw	%xmm0, %xmm2, %xmm0
	vpackuswb	%xmm0, %xmm1, %xmm0
	vpshufb	%xmm10, %xmm0, %xmm0
	vpaddb	%xmm0, %xmm9, %xmm0
	vmovdqu	%xmm0, 112(%rax,%rsi,2)
	addq	$64, %rsi
	cmpq	%r10, %rdi
	jne	.LBB17_235
# %bb.236:                              #   in Loop: Header=BB17_228 Depth=2
	movq	%r15, %rdx
	jmp	.LBB17_237
	.p2align	4, 0x90
.LBB17_238:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm11, %xmm0
	vcvtusi2sd	%rbx, %xmm11, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp128:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp129:
# %bb.239:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.240:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_242
# %bb.241:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_244
	.p2align	4, 0x90
.LBB17_242:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp130:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp131:
# %bb.243:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp132:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp133:
.LBB17_244:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp134:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp135:
# %bb.245:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp136:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp137:
# %bb.246:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp138:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$8, %edx
	leaq	.L.str.11(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp139:
# %bb.247:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	movq	%rax, %rbp
	xorl	%ebx, %ebx
	jmp	.LBB17_248
	.p2align	4, 0x90
.LBB17_249:                             #   in Loop: Header=BB17_248 Depth=2
	xorl	%edx, %edx
.LBB17_257:                             #   in Loop: Header=BB17_248 Depth=2
	sarq	$3, %rdx
	addq	%rdx, %rbx
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	%rbp, %rax
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_258
.LBB17_248:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_252 Depth 3
                                        #       Child Loop BB17_255 Depth 3
	movq	(%rsp), %rcx                    # 8-byte Reload
	cmpq	48(%rsp), %rcx                  # 8-byte Folded Reload
	je	.LBB17_249
# %bb.250:                              #   in Loop: Header=BB17_248 Depth=2
	vmovdqa	176(%rsp), %xmm3                # 16-byte Reload
	vmovdqa	160(%rsp), %xmm4                # 16-byte Reload
	movq	8(%rsp), %rax                   # 8-byte Reload
	testb	$7, 88(%rsp)                    # 1-byte Folded Reload
	leaq	_ZZ23to_string_tree_bigtablemPcE8bigtable(%rip), %r9
	je	.LBB17_253
# %bb.251:                              #   in Loop: Header=BB17_248 Depth=2
	movq	96(%rsp), %rsi                  # 8-byte Reload
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	(%rsp), %rcx                    # 8-byte Reload
	.p2align	4, 0x90
.LBB17_252:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_248 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx), %rdx
	addq	$8, %rcx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm0
	subq	%r8, %rdx
	vmovq	%rdx, %xmm1
	vpunpcklqdq	%xmm1, %xmm0, %xmm0     # xmm0 = xmm0[0],xmm1[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r9,%xmm0,4), %xmm1
	vmovdqu	%xmm1, (%rax)
	addq	$16, %rax
	decq	%rsi
	jne	.LBB17_252
.LBB17_253:                             #   in Loop: Header=BB17_248 Depth=2
	cmpq	$56, 40(%rsp)                   # 8-byte Folded Reload
	movq	%r15, %rdx
	jb	.LBB17_257
# %bb.254:                              #   in Loop: Header=BB17_248 Depth=2
	vmovdqa	176(%rsp), %xmm3                # 16-byte Reload
	vmovdqa	160(%rsp), %xmm4                # 16-byte Reload
	movq	56(%rsp), %r10                  # 8-byte Reload
	leaq	_ZZ23to_string_tree_bigtablemPcE8bigtable(%rip), %r11
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB17_255:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_248 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r11,%xmm0,4), %xmm1
	vmovdqu	%xmm1, (%rax,%rsi,2)
	movq	8(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r11,%xmm0,4), %xmm1
	vmovdqu	%xmm1, 16(%rax,%rsi,2)
	movq	16(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r11,%xmm0,4), %xmm1
	vmovdqu	%xmm1, 32(%rax,%rsi,2)
	movq	24(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r11,%xmm0,4), %xmm1
	vmovdqu	%xmm1, 48(%rax,%rsi,2)
	movq	32(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r11,%xmm0,4), %xmm1
	vmovdqu	%xmm1, 64(%rax,%rsi,2)
	movq	40(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r11,%xmm0,4), %xmm1
	vmovdqu	%xmm1, 80(%rax,%rsi,2)
	movq	48(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %r8           # imm = 0x5F5E100
	vmovd	%edi, %xmm1
	leaq	56(%rcx,%rsi), %rdi
	subq	%r8, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r11,%xmm0,4), %xmm1
	vmovdqu	%xmm1, 96(%rax,%rsi,2)
	movq	56(%rcx,%rsi), %rdx
	mulxq	%r12, %r8, %r8
	shrq	$26, %r8
	imulq	$100000000, %r8, %r9            # imm = 0x5F5E100
	vmovd	%r8d, %xmm1
	subq	%r9, %rdx
	vmovq	%rdx, %xmm0
	vpunpcklqdq	%xmm0, %xmm1, %xmm0     # xmm0 = xmm1[0],xmm0[0]
	vpmuludq	%xmm3, %xmm0, %xmm1
	vpsrlq	$45, %xmm1, %xmm1
	vpmuldq	%xmm4, %xmm1, %xmm2
	vpsubd	%xmm2, %xmm0, %xmm0
	vpcmpeqd	%xmm2, %xmm2, %xmm2
	vpsllq	$32, %xmm0, %xmm0
	vpor	%xmm1, %xmm0, %xmm0
	vpxor	%xmm1, %xmm1, %xmm1
	vpgatherdd	%xmm2, (%r11,%xmm0,4), %xmm1
	vmovdqu	%xmm1, 112(%rax,%rsi,2)
	addq	$64, %rsi
	cmpq	%r10, %rdi
	jne	.LBB17_255
# %bb.256:                              #   in Loop: Header=BB17_248 Depth=2
	movq	%r15, %rdx
	jmp	.LBB17_257
	.p2align	4, 0x90
.LBB17_258:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm11, %xmm0
	vcvtusi2sd	%rbx, %xmm11, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp140:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp141:
# %bb.259:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.260:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_262
# %bb.261:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_264
	.p2align	4, 0x90
.LBB17_262:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp142:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp143:
# %bb.263:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp144:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp145:
.LBB17_264:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp146:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp147:
# %bb.265:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp148:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp149:
# %bb.266:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp150:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movl	$14, %edx
	leaq	.L.str.12(%rip), %rsi
	callq	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
.Ltmp151:
# %bb.267:                              #   in Loop: Header=BB17_5 Depth=1
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	movq	32(%rsp), %r15                  # 8-byte Reload
	movabsq	$-6067343680855748867, %r12     # imm = 0xABCC77118461CEFD
	movq	%rax, %rbp
	xorl	%ebx, %ebx
	jmp	.LBB17_268
	.p2align	4, 0x90
.LBB17_269:                             #   in Loop: Header=BB17_268 Depth=2
	xorl	%edx, %edx
.LBB17_277:                             #   in Loop: Header=BB17_268 Depth=2
	sarq	$3, %rdx
	addq	%rdx, %rbx
	vzeroupper
	callq	_ZNSt6chrono3_V212steady_clock3nowEv@PLT
	subq	%rbp, %rax
	cmpq	$50000000, %rax                 # imm = 0x2FAF080
	jae	.LBB17_278
.LBB17_268:                             #   Parent Loop BB17_5 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB17_272 Depth 3
                                        #       Child Loop BB17_275 Depth 3
	movq	(%rsp), %rcx                    # 8-byte Reload
	cmpq	48(%rsp), %rcx                  # 8-byte Folded Reload
	je	.LBB17_269
# %bb.270:                              #   in Loop: Header=BB17_268 Depth=2
	vmovdqu64	368(%rsp), %zmm3        # 64-byte Reload
	vmovdqa	.LCPI17_1(%rip), %xmm4          # xmm4 = [4048780183313844224,8678551244950751296]
	vmovdqa64	.LCPI17_39(%rip), %zmm5 # zmm5 = [45035996,450359963,4503599628,45035996274,450359962738,4503599627371,45035996273705,450359962737050]
	vmovdqu64	304(%rsp), %zmm6        # 64-byte Reload
	vmovdqu64	240(%rsp), %zmm7        # 64-byte Reload
	movq	8(%rsp), %rax                   # 8-byte Reload
	testb	$7, 88(%rsp)                    # 1-byte Folded Reload
	je	.LBB17_273
# %bb.271:                              #   in Loop: Header=BB17_268 Depth=2
	movq	96(%rsp), %rsi                  # 8-byte Reload
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	(%rsp), %rcx                    # 8-byte Reload
	.p2align	4, 0x90
.LBB17_272:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_268 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx), %rdx
	vmovdqa64	%zmm3, %zmm2
	addq	$8, %rcx
	mulxq	%r12, %rdi, %rdi
	vpbroadcastq	%rdi, %zmm0
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %rdi          # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%rdi, %rdx
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqu	%xmm1, (%rax)
	addq	$16, %rax
	decq	%rsi
	jne	.LBB17_272
.LBB17_273:                             #   in Loop: Header=BB17_268 Depth=2
	cmpq	$56, 40(%rsp)                   # 8-byte Folded Reload
	movq	%r15, %rdx
	jb	.LBB17_277
# %bb.274:                              #   in Loop: Header=BB17_268 Depth=2
	vmovdqu64	368(%rsp), %zmm3        # 64-byte Reload
	vmovdqa	.LCPI17_1(%rip), %xmm4          # xmm4 = [4048780183313844224,8678551244950751296]
	vmovdqa64	.LCPI17_39(%rip), %zmm5 # zmm5 = [45035996,450359963,4503599628,45035996274,450359962738,4503599627371,45035996273705,450359962737050]
	vmovdqu64	304(%rsp), %zmm6        # 64-byte Reload
	vmovdqu64	240(%rsp), %zmm7        # 64-byte Reload
	movq	56(%rsp), %r9                   # 8-byte Reload
	xorl	%esi, %esi
	.p2align	4, 0x90
.LBB17_275:                             #   Parent Loop BB17_5 Depth=1
                                        #     Parent Loop BB17_268 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movq	(%rcx,%rsi), %rdx
	vmovdqa64	%zmm3, %zmm2
	mulxq	%r12, %rdi, %rdi
	vpbroadcastq	%rdi, %zmm0
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %rdi          # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%rdi, %rdx
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqa64	%zmm3, %zmm2
	vmovdqu	%xmm1, (%rax,%rsi,2)
	movq	8(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	vpbroadcastq	%rdi, %zmm0
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %rdi          # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%rdi, %rdx
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqa64	%zmm3, %zmm2
	vmovdqu	%xmm1, 16(%rax,%rsi,2)
	movq	16(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	vpbroadcastq	%rdi, %zmm0
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %rdi          # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%rdi, %rdx
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqa64	%zmm3, %zmm2
	vmovdqu	%xmm1, 32(%rax,%rsi,2)
	movq	24(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	vpbroadcastq	%rdi, %zmm0
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %rdi          # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%rdi, %rdx
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqa64	%zmm3, %zmm2
	vmovdqu	%xmm1, 48(%rax,%rsi,2)
	movq	32(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	vpbroadcastq	%rdi, %zmm0
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %rdi          # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%rdi, %rdx
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqa64	%zmm3, %zmm2
	vmovdqu	%xmm1, 64(%rax,%rsi,2)
	movq	40(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	vpbroadcastq	%rdi, %zmm0
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %rdi          # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%rdi, %rdx
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqa64	%zmm3, %zmm2
	vmovdqu	%xmm1, 80(%rax,%rsi,2)
	movq	48(%rcx,%rsi), %rdx
	mulxq	%r12, %rdi, %rdi
	vpbroadcastq	%rdi, %zmm0
	shrq	$26, %rdi
	imulq	$100000000, %rdi, %rdi          # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%rdi, %rdx
	leaq	56(%rcx,%rsi), %rdi
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqa64	%zmm3, %zmm2
	vmovdqu	%xmm1, 96(%rax,%rsi,2)
	movq	56(%rcx,%rsi), %rdx
	mulxq	%r12, %r8, %r8
	vpbroadcastq	%r8, %zmm0
	shrq	$26, %r8
	imulq	$100000000, %r8, %r8            # imm = 0x5F5E100
	vpsrlq	$26, %zmm0, %zmm0
	vpmadd52luq	%zmm5, %zmm0, %zmm2
	vmovdqa64	%zmm3, %zmm0
	subq	%r8, %rdx
	vpbroadcastq	%rdx, %zmm1
	vpmadd52luq	%zmm5, %zmm1, %zmm0
	vmovdqa64	%zmm7, %zmm1
	vpmadd52huq	%zmm2, %zmm6, %zmm1
	vmovdqa64	%zmm7, %zmm2
	vpmadd52huq	%zmm0, %zmm6, %zmm2
	vpermt2b	%zmm2, %zmm4, %zmm1
	vmovdqu	%xmm1, 112(%rax,%rsi,2)
	addq	$64, %rsi
	cmpq	%r9, %rdi
	jne	.LBB17_275
# %bb.276:                              #   in Loop: Header=BB17_268 Depth=2
	movq	%r15, %rdx
	jmp	.LBB17_277
	.p2align	4, 0x90
.LBB17_278:                             #   in Loop: Header=BB17_5 Depth=1
	vcvtusi2sd	%rax, %xmm11, %xmm0
	vcvtusi2sd	%rbx, %xmm11, %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
.Ltmp152:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	callq	_ZNSo9_M_insertIdEERSoT_@PLT
.Ltmp153:
	movq	56(%rsp), %rbx                  # 8-byte Reload
	movq	8(%rsp), %rbp                   # 8-byte Reload
# %bb.279:                              #   in Loop: Header=BB17_5 Depth=1
	movq	%rax, %r15
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r15,%rax), %r12
	testq	%r12, %r12
	je	.LBB17_34
# %bb.280:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r12)
	je	.LBB17_282
# %bb.281:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r12), %eax
	jmp	.LBB17_284
	.p2align	4, 0x90
.LBB17_282:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp154:
	movq	%r12, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp155:
# %bb.283:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r12), %rax
.Ltmp156:
	movl	$10, %esi
	movq	%r12, %rdi
	callq	*48(%rax)
.Ltmp157:
.LBB17_284:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp158:
	movsbl	%al, %esi
	movq	%r15, %rdi
	callq	_ZNSo3putEc@PLT
.Ltmp159:
# %bb.285:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp160:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp161:
# %bb.286:                              #   in Loop: Header=BB17_5 Depth=1
	movq	_ZSt4cout@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rax
	movq	-24(%rax), %rax
	movq	240(%rcx,%rax), %r15
	testq	%r15, %r15
	je	.LBB17_34
# %bb.287:                              #   in Loop: Header=BB17_5 Depth=1
	cmpb	$0, 56(%r15)
	je	.LBB17_289
# %bb.288:                              #   in Loop: Header=BB17_5 Depth=1
	movzbl	67(%r15), %eax
	jmp	.LBB17_291
	.p2align	4, 0x90
.LBB17_289:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp162:
	movq	%r15, %rdi
	callq	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
.Ltmp163:
# %bb.290:                              #   in Loop: Header=BB17_5 Depth=1
	movq	(%r15), %rax
.Ltmp164:
	movl	$10, %esi
	movq	%r15, %rdi
	callq	*48(%rax)
.Ltmp165:
.LBB17_291:                             #   in Loop: Header=BB17_5 Depth=1
.Ltmp166:
	movq	_ZSt4cout@GOTPCREL(%rip), %rdi
	movsbl	%al, %esi
	callq	_ZNSo3putEc@PLT
.Ltmp167:
	movq	224(%rsp), %r12                 # 8-byte Reload
# %bb.292:                              #   in Loop: Header=BB17_5 Depth=1
.Ltmp168:
	movq	%rax, %rdi
	callq	_ZNSo5flushEv@PLT
.Ltmp169:
# %bb.293:                              #   in Loop: Header=BB17_5 Depth=1
	incq	%r12
	cmpq	$3, %r12
	jne	.LBB17_5
# %bb.294:
	movq	%rbp, %rdi
	callq	_ZdaPv@PLT
	movq	(%rsp), %rdi                    # 8-byte Reload
	testq	%rdi, %rdi
	je	.LBB17_296
# %bb.295:
	callq	_ZdlPv@PLT
.LBB17_296:
	xorl	%eax, %eax
	addq	$440, %rsp                      # imm = 0x1B8
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
.LBB17_34:
	.cfi_def_cfa_offset 496
.Ltmp171:
	callq	_ZSt16__throw_bad_castv@PLT
.Ltmp172:
# %bb.35:
.LBB17_10:
.Ltmp174:
	leaq	.L.str.13(%rip), %rdi
	callq	_ZSt20__throw_length_errorPKc@PLT
.Ltmp175:
# %bb.11:
.LBB17_299:
.Ltmp5:
	movq	%rax, %r14
	movq	%r13, %rdi
	jmp	.LBB17_300
.LBB17_21:
.Ltmp2:
	jmp	.LBB17_22
.LBB17_23:
.Ltmp176:
	jmp	.LBB17_22
.LBB17_298:
.Ltmp173:
	jmp	.LBB17_22
.LBB17_297:
.Ltmp170:
.LBB17_22:
	movq	(%rsp), %rdi                    # 8-byte Reload
	movq	%rax, %r14
.LBB17_300:
	testq	%rdi, %rdi
	je	.LBB17_302
# %bb.301:
	callq	_ZdlPv@PLT
.LBB17_302:
	movq	%r14, %rdi
	callq	_Unwind_Resume@PLT
.Lfunc_end17:
	.size	main, .Lfunc_end17-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2, 0x0
GCC_except_table17:
.Lexception0:
	.byte	255                             # @LPStart Encoding = omit
	.byte	255                             # @TType Encoding = omit
	.byte	1                               # Call site Encoding = uleb128
	.uleb128 .Lcst_end0-.Lcst_begin0
.Lcst_begin0:
	.uleb128 .Ltmp0-.Lfunc_begin0           # >> Call Site 1 <<
	.uleb128 .Ltmp1-.Ltmp0                  #   Call between .Ltmp0 and .Ltmp1
	.uleb128 .Ltmp2-.Lfunc_begin0           #     jumps to .Ltmp2
	.byte	0                               #   On action: cleanup
	.uleb128 .Ltmp1-.Lfunc_begin0           # >> Call Site 2 <<
	.uleb128 .Ltmp3-.Ltmp1                  #   Call between .Ltmp1 and .Ltmp3
	.byte	0                               #     has no landing pad
	.byte	0                               #   On action: cleanup
	.uleb128 .Ltmp3-.Lfunc_begin0           # >> Call Site 3 <<
	.uleb128 .Ltmp4-.Ltmp3                  #   Call between .Ltmp3 and .Ltmp4
	.uleb128 .Ltmp5-.Lfunc_begin0           #     jumps to .Ltmp5
	.byte	0                               #   On action: cleanup
	.uleb128 .Ltmp6-.Lfunc_begin0           # >> Call Site 4 <<
	.uleb128 .Ltmp169-.Ltmp6                #   Call between .Ltmp6 and .Ltmp169
	.uleb128 .Ltmp170-.Lfunc_begin0         #     jumps to .Ltmp170
	.byte	0                               #   On action: cleanup
	.uleb128 .Ltmp171-.Lfunc_begin0         # >> Call Site 5 <<
	.uleb128 .Ltmp172-.Ltmp171              #   Call between .Ltmp171 and .Ltmp172
	.uleb128 .Ltmp173-.Lfunc_begin0         #     jumps to .Ltmp173
	.byte	0                               #   On action: cleanup
	.uleb128 .Ltmp174-.Lfunc_begin0         # >> Call Site 6 <<
	.uleb128 .Ltmp175-.Ltmp174              #   Call between .Ltmp174 and .Ltmp175
	.uleb128 .Ltmp176-.Lfunc_begin0         #     jumps to .Ltmp176
	.byte	0                               #   On action: cleanup
	.uleb128 .Ltmp175-.Lfunc_begin0         # >> Call Site 7 <<
	.uleb128 .Lfunc_end17-.Ltmp175          #   Call between .Ltmp175 and .Lfunc_end17
	.byte	0                               #     has no landing pad
	.byte	0                               #   On action: cleanup
.Lcst_end0:
	.p2align	2, 0x0
                                        # -- End function
	.type	_ZZ20to_string_tree_tablemPcE5table,@object # @_ZZ20to_string_tree_tablemPcE5table
	.section	.rodata.str1.16,"aMS",@progbits,1
	.p2align	4, 0x0
_ZZ20to_string_tree_tablemPcE5table:
	.asciz	"00010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899"
	.size	_ZZ20to_string_tree_tablemPcE5table, 201

	.type	_ZZ23to_string_tree_bigtablemPcE8bigtable,@object # @_ZZ23to_string_tree_bigtablemPcE8bigtable
	.section	.rodata,"a",@progbits
	.p2align	4, 0x0
_ZZ23to_string_tree_bigtablemPcE8bigtable:
	.ascii	"0000000100020003000400050006000700080009001000110012001300140015001600170018001900200021002200230024002500260027002800290030003100320033003400350036003700380039004000410042004300440045004600470048004900500051005200530054005500560057005800590060006100620063006400650066006700680069007000710072007300740075007600770078007900800081008200830084008500860087008800890090009100920093009400950096009700980099010001010102010301040105010601070108010901100111011201130114011501160117011801190120012101220123012401250126012701280129013001310132013301340135013601370138013901400141014201430144014501460147014801490150015101520153015401550156015701580159016001610162016301640165016601670168016901700171017201730174017501760177017801790180018101820183018401850186018701880189019001910192019301940195019601970198019902000201020202030204020502060207020802090210021102120213021402150216021702180219022002210222022302240225022602270228022902300231023202330234023502360237023802390240024102420243024402450246024702480249025002510252025302540255025602570258025902600261026202630264026502660267026802690270027102720273027402750276027702780279028002810282028302840285028602870288028902900291029202930294029502960297029802990300030103020303030403050306030703080309031003110312031303140315031603170318031903200321032203230324032503260327032803290330033103320333033403350336033703380339034003410342034303440345034603470348034903500351035203530354035503560357035803590360036103620363036403650366036703680369037003710372037303740375037603770378037903800381038203830384038503860387038803890390039103920393039403950396039703980399040004010402040304040405040604070408040904100411041204130414041504160417041804190420042104220423042404250426042704280429043004310432043304340435043604370438043904400441044204430444044504460447044804490450045104520453045404550456045704580459046004610462046304640465046604670468046904700471047204730474047504760477047804790480048104820483048404850486048704880489049004910492049304940495049604970498049905000501050205030504050505060507050805090510051105120513051405150516051705180519052005210522052305240525052605270528052905300531053205330534053505360537053805390540054105420543054405450546054705480549055005510552055305540555055605570558055905600561056205630564056505660567056805690570057105720573057405750576057705780579058005810582058305840585058605870588058905900591059205930594059505960597059805990600060106020603060406050606060706080609061006110612061306140615061606170618061906200621062206230624062506260627062806290630063106320633063406350636063706380639064006410642064306440645064606470648064906500651065206530654065506560657065806590660066106620663066406650666066706680669067006710672067306740675067606770678067906800681068206830684068506860687068806890690069106920693069406950696069706980699070007010702070307040705070607070708070907100711071207130714071507160717071807190720072107220723072407250726072707280729073007310732073307340735073607370738073907400741074207430744074507460747074807490750075107520753075407550756075707580759076007610762076307640765076607670768076907700771077207730774077507760777077807790780078107820783078407850786078707880789079007910792079307940795079607970798079908000801080208030804080508060807080808090810081108120813081408150816081708180819082008210822082308240825082608270828082908300831083208330834083508360837083808390840084108420843084408450846084708480849085008510852085308540855085608570858085908600861086208630864086508660867086808690870087108720873087408750876087708780879088008810882088308840885088608870888088908900891089208930894089508960897089808990900090109020903090409050906090709080909091009110912091309140915091609170918091909200921092209230924092509260927092809290930093109320933093409350936093709380939094009410942094309440945094609470948094909500951095209530954095509560957095809590960096109620963096409650966096709680969097009710972097309740975097609770978097909800981098209830984098509860987098809890990099109920993099409950996099709980999100010011002100310041005100610071008100910101011101210131014101510161017101810191020102110221023102410251026102710281029103010311032103310341035103610371038103910401041104210431044104510461047104810491050105110521053105410551056105710581059106010611062106310641065106610671068106910701071107210731074107510761077107810791080108110821083108410851086108710881089109010911092109310941095109610971098109911001101110211031104110511061107110811091110111111121113111411151116111711181119112011211122112311241125112611271128112911301131113211331134113511361137113811391140114111421143114411451146114711481149115011511152115311541155115611571158115911601161116211631164116511661167116811691170117111721173117411751176117711781179118011811182118311841185118611871188118911901191119211931194119511961197119811991200120112021203120412051206120712081209121012111212121312141215121612171218121912201221122212231224122512261227122812291230123112321233123412351236123712381239124012411242124312441245124612471248124912501251125212531254125512561257125812591260126112621263126412651266126712681269127012711272127312741275127612771278127912801281128212831284128512861287128812891290129112921293129412951296129712981299130013011302130313041305130613071308130913101311131213131314131513161317131813191320132113221323132413251326132713281329133013311332133313341335133613371338133913401341134213431344134513461347134813491350135113521353135413551356135713581359136013611362136313641365136613671368136913701371137213731374137513761377137813791380138113821383138413851386138713881389139013911392139313941395139613971398139914001401140214031404140514061407140814091410141114121413141414151416141714181419142014211422142314241425142614271428142914301431143214331434143514361437143814391440144114421443144414451446144714481449145014511452145314541455145614571458145914601461146214631464146514661467146814691470147114721473147414751476147714781479148014811482148314841485148614871488148914901491149214931494149514961497149814991500150115021503150415051506150715081509151015111512151315141515151615171518151915201521152215231524152515261527152815291530153115321533153415351536153715381539154015411542154315441545154615471548154915501551155215531554155515561557155815591560156115621563156415651566156715681569157015711572157315741575157615771578157915801581158215831584158515861587158815891590159115921593159415951596159715981599160016011602160316041605160616071608160916101611161216131614161516161617161816191620162116221623162416251626162716281629163016311632163316341635163616371638163916401641164216431644164516461647164816491650165116521653165416551656165716581659166016611662166316641665166616671668166916701671167216731674167516761677167816791680168116821683168416851686168716881689169016911692169316941695169616971698169917001701170217031704170517061707170817091710171117121713171417151716171717181719172017211722172317241725172617271728172917301731173217331734173517361737173817391740174117421743174417451746174717481749175017511752175317541755175617571758175917601761176217631764176517661767176817691770177117721773177417751776177717781779178017811782178317841785178617871788178917901791179217931794179517961797179817991800180118021803180418051806180718081809181018111812181318141815181618171818181918201821182218231824182518261827182818291830183118321833183418351836183718381839184018411842184318441845184618471848184918501851185218531854185518561857185818591860186118621863186418651866186718681869187018711872187318741875187618771878187918801881188218831884188518861887188818891890189118921893189418951896189718981899190019011902190319041905190619071908190919101911191219131914191519161917191819191920192119221923192419251926192719281929193019311932193319341935193619371938193919401941194219431944194519461947194819491950195119521953195419551956195719581959196019611962196319641965196619671968196919701971197219731974197519761977197819791980198119821983198419851986198719881989199019911992199319941995199619971998199920002001200220032004200520062007200820092010201120122013201420152016201720182019202020212022202320242025202620272028202920302031203220332034203520362037203820392040204120422043204420452046204720482049205020512052205320542055205620572058205920602061206220632064206520662067206820692070207120722073207420752076207720782079208020812082208320842085208620872088208920902091209220932094209520962097209820992100210121022103210421052106210721082109211021112112211321142115211621172118211921202121212221232124212521262127212821292130213121322133213421352136213721382139214021412142214321442145214621472148214921502151215221532154215521562157215821592160216121622163216421652166216721682169217021712172217321742175217621772178217921802181218221832184218521862187218821892190219121922193219421952196219721982199220022012202220322042205220622072208220922102211221222132214221522162217221822192220222122222223222422252226222722282229223022312232223322342235223622372238223922402241224222432244224522462247224822492250225122522253225422552256225722582259226022612262226322642265226622672268226922702271227222732274227522762277227822792280228122822283228422852286228722882289229022912292229322942295229622972298229923002301230223032304230523062307230823092310231123122313231423152316231723182319232023212322232323242325232623272328232923302331233223332334233523362337233823392340234123422343234423452346234723482349235023512352235323542355235623572358235923602361236223632364236523662367236823692370237123722373237423752376237723782379238023812382238323842385238623872388238923902391239223932394239523962397239823992400240124022403240424052406240724082409241024112412241324142415241624172418241924202421242224232424242524262427242824292430243124322433243424352436243724382439244024412442244324442445244624472448244924502451245224532454245524562457245824592460246124622463246424652466246724682469247024712472247324742475247624772478247924802481248224832484248524862487248824892490249124922493249424952496249724982499250025012502250325042505250625072508250925102511251225132514251525162517251825192520252125222523252425252526252725282529253025312532253325342535253625372538253925402541254225432544254525462547254825492550255125522553255425552556255725582559256025612562256325642565256625672568256925702571257225732574257525762577257825792580258125822583258425852586258725882589259025912592259325942595259625972598259926002601260226032604260526062607260826092610261126122613261426152616261726182619262026212622262326242625262626272628262926302631263226332634263526362637263826392640264126422643264426452646264726482649265026512652265326542655265626572658265926602661266226632664266526662667266826692670267126722673267426752676267726782679268026812682268326842685268626872688268926902691269226932694269526962697269826992700270127022703270427052706270727082709271027112712271327142715271627172718271927202721272227232724272527262727272827292730273127322733273427352736273727382739274027412742274327442745274627472748274927502751275227532754275527562757275827592760276127622763276427652766276727682769277027712772277327742775277627772778277927802781278227832784278527862787278827892790279127922793279427952796279727982799280028012802280328042805280628072808280928102811281228132814281528162817281828192820282128222823282428252826282728282829283028312832283328342835283628372838283928402841284228432844284528462847284828492850285128522853285428552856285728582859286028612862286328642865286628672868286928702871287228732874287528762877287828792880288128822883288428852886288728882889289028912892289328942895289628972898289929002901290229032904290529062907290829092910291129122913291429152916291729182919292029212922292329242925292629272928292929302931293229332934293529362937293829392940294129422943294429452946294729482949295029512952295329542955295629572958295929602961296229632964296529662967296829692970297129722973297429752976297729782979298029812982298329842985298629872988298929902991299229932994299529962997299829993000300130023003300430053006300730083009301030113012301330143015301630173018301930203021302230233024302530263027302830293030303130323033303430353036303730383039304030413042304330443045304630473048304930503051305230533054305530563057305830593060306130623063306430653066306730683069307030713072307330743075307630773078307930803081308230833084308530863087308830893090309130923093309430953096309730983099310031013102310331043105310631073108310931103111311231133114311531163117311831193120312131223123312431253126312731283129313031313132313331343135313631373138313931403141314231433144314531463147314831493150315131523153315431553156315731583159316031613162316331643165316631673168316931703171317231733174317531763177317831793180318131823183318431853186318731883189319031913192319331943195319631973198319932003201320232033204320532063207320832093210321132123213321432153216321732183219322032213222322332243225322632273228322932303231323232333234323532363237323832393240324132423243324432453246324732483249325032513252325332543255325632573258325932603261326232633264326532663267326832693270327132723273327432753276327732783279328032813282328332843285328632873288328932903291329232933294329532963297329832993300330133023303330433053306330733083309331033113312331333143315331633173318331933203321332233233324332533263327332833293330333133323333333433353336333733383339334033413342334333443345334633473348334933503351335233533354335533563357335833593360336133623363336433653366336733683369337033713372337333743375337633773378337933803381338233833384338533863387338833893390339133923393339433953396339733983399340034013402340334043405340634073408340934103411341234133414341534163417341834193420342134223423342434253426342734283429343034313432343334343435343634373438343934403441344234433444344534463447344834493450345134523453345434553456345734583459346034613462346334643465346634673468346934703471347234733474347534763477347834793480348134823483348434853486348734883489349034913492349334943495349634973498349935003501350235033504350535063507350835093510351135123513351435153516351735183519352035213522352335243525352635273528352935303531353235333534353535363537353835393540354135423543354435453546354735483549355035513552355335543555355635573558355935603561356235633564356535663567356835693570357135723573357435753576357735783579358035813582358335843585358635873588358935903591359235933594359535963597359835993600360136023603360436053606360736083609361036113612361336143615361636173618361936203621362236233624362536263627362836293630363136323633363436353636363736383639364036413642364336443645364636473648364936503651365236533654365536563657365836593660366136623663366436653666366736683669367036713672367336743675367636773678367936803681368236833684368536863687368836893690369136923693369436953696369736983699370037013702370337043705370637073708370937103711371237133714371537163717371837193720372137223723372437253726372737283729373037313732373337343735373637373738373937403741374237433744374537463747374837493750375137523753375437553756375737583759376037613762376337643765376637673768376937703771377237733774377537763777377837793780378137823783378437853786378737883789379037913792379337943795379637973798379938003801380238033804380538063807380838093810381138123813381438153816381738183819382038213822382338243825382638273828382938303831383238333834383538363837383838393840384138423843384438453846384738483849385038513852385338543855385638573858385938603861386238633864386538663867386838693870387138723873387438753876387738783879388038813882388338843885388638873888388938903891389238933894389538963897389838993900390139023903390439053906390739083909391039113912391339143915391639173918391939203921392239233924392539263927392839293930393139323933393439353936393739383939394039413942394339443945394639473948394939503951395239533954395539563957395839593960396139623963396439653966396739683969397039713972397339743975397639773978397939803981398239833984398539863987398839893990399139923993399439953996399739983999400040014002400340044005400640074008400940104011401240134014401540164017401840194020402140224023402440254026402740284029403040314032403340344035403640374038403940404041404240434044404540464047404840494050405140524053405440554056405740584059406040614062406340644065406640674068406940704071407240734074407540764077407840794080408140824083408440854086408740884089409040914092409340944095409640974098409941004101410241034104410541064107410841094110411141124113411441154116411741184119412041214122412341244125412641274128412941304131413241334134413541364137413841394140414141424143414441454146414741484149415041514152415341544155415641574158415941604161416241634164416541664167416841694170417141724173417441754176417741784179418041814182418341844185418641874188418941904191419241934194419541964197419841994200420142024203420442054206420742084209421042114212421342144215421642174218421942204221422242234224422542264227422842294230423142324233423442354236423742384239424042414242424342444245424642474248424942504251425242534254425542564257425842594260426142624263426442654266426742684269427042714272427342744275427642774278427942804281428242834284428542864287428842894290429142924293429442954296429742984299430043014302430343044305430643074308430943104311431243134314431543164317431843194320432143224323432443254326432743284329433043314332433343344335433643374338433943404341434243434344434543464347434843494350435143524353435443554356435743584359436043614362436343644365436643674368436943704371437243734374437543764377437843794380438143824383438443854386438743884389439043914392439343944395439643974398439944004401440244034404440544064407440844094410441144124413441444154416441744184419442044214422442344244425442644274428442944304431443244334434443544364437443844394440444144424443444444454446444744484449445044514452445344544455445644574458445944604461446244634464446544664467446844694470447144724473447444754476447744784479448044814482448344844485448644874488448944904491449244934494449544964497449844994500450145024503450445054506450745084509451045114512451345144515451645174518451945204521452245234524452545264527452845294530453145324533453445354536453745384539454045414542454345444545454645474548454945504551455245534554455545564557455845594560456145624563456445654566456745684569457045714572457345744575457645774578457945804581458245834584458545864587458845894590459145924593459445954596459745984599460046014602460346044605460646074608460946104611461246134614461546164617461846194620462146224623462446254626462746284629463046314632463346344635463646374638463946404641464246434644464546464647464846494650465146524653465446554656465746584659466046614662466346644665466646674668466946704671467246734674467546764677467846794680468146824683468446854686468746884689469046914692469346944695469646974698469947004701470247034704470547064707470847094710471147124713471447154716471747184719472047214722472347244725472647274728472947304731473247334734473547364737473847394740474147424743474447454746474747484749475047514752475347544755475647574758475947604761476247634764476547664767476847694770477147724773477447754776477747784779478047814782478347844785478647874788478947904791479247934794479547964797479847994800480148024803480448054806480748084809481048114812481348144815481648174818481948204821482248234824482548264827482848294830483148324833483448354836483748384839484048414842484348444845484648474848484948504851485248534854485548564857485848594860486148624863486448654866486748684869487048714872487348744875487648774878487948804881488248834884488548864887488848894890489148924893489448954896489748984899490049014902490349044905490649074908490949104911491249134914491549164917491849194920492149224923492449254926492749284929493049314932493349344935493649374938493949404941494249434944494549464947494849494950495149524953495449554956495749584959496049614962496349644965496649674968496949704971497249734974497549764977497849794980498149824983498449854986498749884989499049914992499349944995499649974998499950005001500250035004500550065007500850095010501150125013501450155016501750185019502050215022502350245025502650275028502950305031503250335034503550365037503850395040504150425043504450455046504750485049505050515052505350545055505650575058505950605061506250635064506550665067506850695070507150725073507450755076507750785079508050815082508350845085508650875088508950905091509250935094509550965097509850995100510151025103510451055106510751085109511051115112511351145115511651175118511951205121512251235124512551265127512851295130513151325133513451355136513751385139514051415142514351445145514651475148514951505151515251535154515551565157515851595160516151625163516451655166516751685169517051715172517351745175517651775178517951805181518251835184518551865187518851895190519151925193519451955196519751985199520052015202520352045205520652075208520952105211521252135214521552165217521852195220522152225223522452255226522752285229523052315232523352345235523652375238523952405241524252435244524552465247524852495250525152525253525452555256525752585259526052615262526352645265526652675268526952705271527252735274527552765277527852795280528152825283528452855286528752885289529052915292529352945295529652975298529953005301530253035304530553065307530853095310531153125313531453155316531753185319532053215322532353245325532653275328532953305331533253335334533553365337533853395340534153425343534453455346534753485349535053515352535353545355535653575358535953605361536253635364536553665367536853695370537153725373537453755376537753785379538053815382538353845385538653875388538953905391539253935394539553965397539853995400540154025403540454055406540754085409541054115412541354145415541654175418541954205421542254235424542554265427542854295430543154325433543454355436543754385439544054415442544354445445544654475448544954505451545254535454545554565457545854595460546154625463546454655466546754685469547054715472547354745475547654775478547954805481548254835484548554865487548854895490549154925493549454955496549754985499550055015502550355045505550655075508550955105511551255135514551555165517551855195520552155225523552455255526552755285529553055315532553355345535553655375538553955405541554255435544554555465547554855495550555155525553555455555556555755585559556055615562556355645565556655675568556955705571557255735574557555765577557855795580558155825583558455855586558755885589559055915592559355945595559655975598559956005601560256035604560556065607560856095610561156125613561456155616561756185619562056215622562356245625562656275628562956305631563256335634563556365637563856395640564156425643564456455646564756485649565056515652565356545655565656575658565956605661566256635664566556665667566856695670567156725673567456755676567756785679568056815682568356845685568656875688568956905691569256935694569556965697569856995700570157025703570457055706570757085709571057115712571357145715571657175718571957205721572257235724572557265727572857295730573157325733573457355736573757385739574057415742574357445745574657475748574957505751575257535754575557565757575857595760576157625763576457655766576757685769577057715772577357745775577657775778577957805781578257835784578557865787578857895790579157925793579457955796579757985799580058015802580358045805580658075808580958105811581258135814581558165817581858195820582158225823582458255826582758285829583058315832583358345835583658375838583958405841584258435844584558465847584858495850585158525853585458555856585758585859586058615862586358645865586658675868586958705871587258735874587558765877587858795880588158825883588458855886588758885889589058915892589358945895589658975898589959005901590259035904590559065907590859095910591159125913591459155916591759185919592059215922592359245925592659275928592959305931593259335934593559365937593859395940594159425943594459455946594759485949595059515952595359545955595659575958595959605961596259635964596559665967596859695970597159725973597459755976597759785979598059815982598359845985598659875988598959905991599259935994599559965997599859996000600160026003600460056006600760086009601060116012601360146015601660176018601960206021602260236024602560266027602860296030603160326033603460356036603760386039604060416042604360446045604660476048604960506051605260536054605560566057605860596060606160626063606460656066606760686069607060716072607360746075607660776078607960806081608260836084608560866087608860896090609160926093609460956096609760986099610061016102610361046105610661076108610961106111611261136114611561166117611861196120612161226123612461256126612761286129613061316132613361346135613661376138613961406141614261436144614561466147614861496150615161526153615461556156615761586159616061616162616361646165616661676168616961706171617261736174617561766177617861796180618161826183618461856186618761886189619061916192619361946195619661976198619962006201620262036204620562066207620862096210621162126213621462156216621762186219622062216222622362246225622662276228622962306231623262336234623562366237623862396240624162426243624462456246624762486249625062516252625362546255625662576258625962606261626262636264626562666267626862696270627162726273627462756276627762786279628062816282628362846285628662876288628962906291629262936294629562966297629862996300630163026303630463056306630763086309631063116312631363146315631663176318631963206321632263236324632563266327632863296330633163326333633463356336633763386339634063416342634363446345634663476348634963506351635263536354635563566357635863596360636163626363636463656366636763686369637063716372637363746375637663776378637963806381638263836384638563866387638863896390639163926393639463956396639763986399640064016402640364046405640664076408640964106411641264136414641564166417641864196420642164226423642464256426642764286429643064316432643364346435643664376438643964406441644264436444644564466447644864496450645164526453645464556456645764586459646064616462646364646465646664676468646964706471647264736474647564766477647864796480648164826483648464856486648764886489649064916492649364946495649664976498649965006501650265036504650565066507650865096510651165126513651465156516651765186519652065216522652365246525652665276528652965306531653265336534653565366537653865396540654165426543654465456546654765486549655065516552655365546555655665576558655965606561656265636564656565666567656865696570657165726573657465756576657765786579658065816582658365846585658665876588658965906591659265936594659565966597659865996600660166026603660466056606660766086609661066116612661366146615661666176618661966206621662266236624662566266627662866296630663166326633663466356636663766386639664066416642664366446645664666476648664966506651665266536654665566566657665866596660666166626663666466656666666766686669667066716672667366746675667666776678667966806681668266836684668566866687668866896690669166926693669466956696669766986699670067016702670367046705670667076708670967106711671267136714671567166717671867196720672167226723672467256726672767286729673067316732673367346735673667376738673967406741674267436744674567466747674867496750675167526753675467556756675767586759676067616762676367646765676667676768676967706771677267736774677567766777677867796780678167826783678467856786678767886789679067916792679367946795679667976798679968006801680268036804680568066807680868096810681168126813681468156816681768186819682068216822682368246825682668276828682968306831683268336834683568366837683868396840684168426843684468456846684768486849685068516852685368546855685668576858685968606861686268636864686568666867686868696870687168726873687468756876687768786879688068816882688368846885688668876888688968906891689268936894689568966897689868996900690169026903690469056906690769086909691069116912691369146915691669176918691969206921692269236924692569266927692869296930693169326933693469356936693769386939694069416942694369446945694669476948694969506951695269536954695569566957695869596960696169626963696469656966696769686969697069716972697369746975697669776978697969806981698269836984698569866987698869896990699169926993699469956996699769986999700070017002700370047005700670077008700970107011701270137014701570167017701870197020702170227023702470257026702770287029703070317032703370347035703670377038703970407041704270437044704570467047704870497050705170527053705470557056705770587059706070617062706370647065706670677068706970707071707270737074707570767077707870797080708170827083708470857086708770887089709070917092709370947095709670977098709971007101710271037104710571067107710871097110711171127113711471157116711771187119712071217122712371247125712671277128712971307131713271337134713571367137713871397140714171427143714471457146714771487149715071517152715371547155715671577158715971607161716271637164716571667167716871697170717171727173717471757176717771787179718071817182718371847185718671877188718971907191719271937194719571967197719871997200720172027203720472057206720772087209721072117212721372147215721672177218721972207221722272237224722572267227722872297230723172327233723472357236723772387239724072417242724372447245724672477248724972507251725272537254725572567257725872597260726172627263726472657266726772687269727072717272727372747275727672777278727972807281728272837284728572867287728872897290729172927293729472957296729772987299730073017302730373047305730673077308730973107311731273137314731573167317731873197320732173227323732473257326732773287329733073317332733373347335733673377338733973407341734273437344734573467347734873497350735173527353735473557356735773587359736073617362736373647365736673677368736973707371737273737374737573767377737873797380738173827383738473857386738773887389739073917392739373947395739673977398739974007401740274037404740574067407740874097410741174127413741474157416741774187419742074217422742374247425742674277428742974307431743274337434743574367437743874397440744174427443744474457446744774487449745074517452745374547455745674577458745974607461746274637464746574667467746874697470747174727473747474757476747774787479748074817482748374847485748674877488748974907491749274937494749574967497749874997500750175027503750475057506750775087509751075117512751375147515751675177518751975207521752275237524752575267527752875297530753175327533753475357536753775387539754075417542754375447545754675477548754975507551755275537554755575567557755875597560756175627563756475657566756775687569757075717572757375747575757675777578757975807581758275837584758575867587758875897590759175927593759475957596759775987599760076017602760376047605760676077608760976107611761276137614761576167617761876197620762176227623762476257626762776287629763076317632763376347635763676377638763976407641764276437644764576467647764876497650765176527653765476557656765776587659766076617662766376647665766676677668766976707671767276737674767576767677767876797680768176827683768476857686768776887689769076917692769376947695769676977698769977007701770277037704770577067707770877097710771177127713771477157716771777187719772077217722772377247725772677277728772977307731773277337734773577367737773877397740774177427743774477457746774777487749775077517752775377547755775677577758775977607761776277637764776577667767776877697770777177727773777477757776777777787779778077817782778377847785778677877788778977907791779277937794779577967797779877997800780178027803780478057806780778087809781078117812781378147815781678177818781978207821782278237824782578267827782878297830783178327833783478357836783778387839784078417842784378447845784678477848784978507851785278537854785578567857785878597860786178627863786478657866786778687869787078717872787378747875787678777878787978807881788278837884788578867887788878897890789178927893789478957896789778987899790079017902790379047905790679077908790979107911791279137914791579167917791879197920792179227923792479257926792779287929793079317932793379347935793679377938793979407941794279437944794579467947794879497950795179527953795479557956795779587959796079617962796379647965796679677968796979707971797279737974797579767977797879797980798179827983798479857986798779887989799079917992799379947995799679977998799980008001800280038004800580068007800880098010801180128013801480158016801780188019802080218022802380248025802680278028802980308031803280338034803580368037803880398040804180428043804480458046804780488049805080518052805380548055805680578058805980608061806280638064806580668067806880698070807180728073807480758076807780788079808080818082808380848085808680878088808980908091809280938094809580968097809880998100810181028103810481058106810781088109811081118112811381148115811681178118811981208121812281238124812581268127812881298130813181328133813481358136813781388139814081418142814381448145814681478148814981508151815281538154815581568157815881598160816181628163816481658166816781688169817081718172817381748175817681778178817981808181818281838184818581868187818881898190819181928193819481958196819781988199820082018202820382048205820682078208820982108211821282138214821582168217821882198220822182228223822482258226822782288229823082318232823382348235823682378238823982408241824282438244824582468247824882498250825182528253825482558256825782588259826082618262826382648265826682678268826982708271827282738274827582768277827882798280828182828283828482858286828782888289829082918292829382948295829682978298829983008301830283038304830583068307830883098310831183128313831483158316831783188319832083218322832383248325832683278328832983308331833283338334833583368337833883398340834183428343834483458346834783488349835083518352835383548355835683578358835983608361836283638364836583668367836883698370837183728373837483758376837783788379838083818382838383848385838683878388838983908391839283938394839583968397839883998400840184028403840484058406840784088409841084118412841384148415841684178418841984208421842284238424842584268427842884298430843184328433843484358436843784388439844084418442844384448445844684478448844984508451845284538454845584568457845884598460846184628463846484658466846784688469847084718472847384748475847684778478847984808481848284838484848584868487848884898490849184928493849484958496849784988499850085018502850385048505850685078508850985108511851285138514851585168517851885198520852185228523852485258526852785288529853085318532853385348535853685378538853985408541854285438544854585468547854885498550855185528553855485558556855785588559856085618562856385648565856685678568856985708571857285738574857585768577857885798580858185828583858485858586858785888589859085918592859385948595859685978598859986008601860286038604860586068607860886098610861186128613861486158616861786188619862086218622862386248625862686278628862986308631863286338634863586368637863886398640864186428643864486458646864786488649865086518652865386548655865686578658865986608661866286638664866586668667866886698670867186728673867486758676867786788679868086818682868386848685868686878688868986908691869286938694869586968697869886998700870187028703870487058706870787088709871087118712871387148715871687178718871987208721872287238724872587268727872887298730873187328733873487358736873787388739874087418742874387448745874687478748874987508751875287538754875587568757875887598760876187628763876487658766876787688769877087718772877387748775877687778778877987808781878287838784878587868787878887898790879187928793879487958796879787988799880088018802880388048805880688078808880988108811881288138814881588168817881888198820882188228823882488258826882788288829883088318832883388348835883688378838883988408841884288438844884588468847884888498850885188528853885488558856885788588859886088618862886388648865886688678868886988708871887288738874887588768877887888798880888188828883888488858886888788888889889088918892889388948895889688978898889989008901890289038904890589068907890889098910891189128913891489158916891789188919892089218922892389248925892689278928892989308931893289338934893589368937893889398940894189428943894489458946894789488949895089518952895389548955895689578958895989608961896289638964896589668967896889698970897189728973897489758976897789788979898089818982898389848985898689878988898989908991899289938994899589968997899889999000900190029003900490059006900790089009901090119012901390149015901690179018901990209021902290239024902590269027902890299030903190329033903490359036903790389039904090419042904390449045904690479048904990509051905290539054905590569057905890599060906190629063906490659066906790689069907090719072907390749075907690779078907990809081908290839084908590869087908890899090909190929093909490959096909790989099910091019102910391049105910691079108910991109111911291139114911591169117911891199120912191229123912491259126912791289129913091319132913391349135913691379138913991409141914291439144914591469147914891499150915191529153915491559156915791589159916091619162916391649165916691679168916991709171917291739174917591769177917891799180918191829183918491859186918791889189919091919192919391949195919691979198919992009201920292039204920592069207920892099210921192129213921492159216921792189219922092219222922392249225922692279228922992309231923292339234923592369237923892399240924192429243924492459246924792489249925092519252925392549255925692579258925992609261926292639264926592669267926892699270927192729273927492759276927792789279928092819282928392849285928692879288928992909291929292939294929592969297929892999300930193029303930493059306930793089309931093119312931393149315931693179318931993209321932293239324932593269327932893299330933193329333933493359336933793389339934093419342934393449345934693479348934993509351935293539354935593569357935893599360936193629363936493659366936793689369937093719372937393749375937693779378937993809381938293839384938593869387938893899390939193929393939493959396939793989399940094019402940394049405940694079408940994109411941294139414941594169417941894199420942194229423942494259426942794289429943094319432943394349435943694379438943994409441944294439444944594469447944894499450945194529453945494559456945794589459946094619462946394649465946694679468946994709471947294739474947594769477947894799480948194829483948494859486948794889489949094919492949394949495949694979498949995009501950295039504950595069507950895099510951195129513951495159516951795189519952095219522952395249525952695279528952995309531953295339534953595369537953895399540954195429543954495459546954795489549955095519552955395549555955695579558955995609561956295639564956595669567956895699570957195729573957495759576957795789579958095819582958395849585958695879588958995909591959295939594959595969597959895999600960196029603960496059606960796089609961096119612961396149615961696179618961996209621962296239624962596269627962896299630963196329633963496359636963796389639964096419642964396449645964696479648964996509651965296539654965596569657965896599660966196629663966496659666966796689669967096719672967396749675967696779678967996809681968296839684968596869687968896899690969196929693969496959696969796989699970097019702970397049705970697079708970997109711971297139714971597169717971897199720972197229723972497259726972797289729973097319732973397349735973697379738973997409741974297439744974597469747974897499750975197529753975497559756975797589759976097619762976397649765976697679768976997709771977297739774977597769777977897799780978197829783978497859786978797889789979097919792979397949795979697979798979998009801980298039804980598069807980898099810981198129813981498159816981798189819982098219822982398249825982698279828982998309831983298339834983598369837983898399840984198429843984498459846984798489849985098519852985398549855985698579858985998609861986298639864986598669867986898699870987198729873987498759876987798789879988098819882988398849885988698879888988998909891989298939894989598969897989898999900990199029903990499059906990799089909991099119912991399149915991699179918991999209921992299239924992599269927992899299930993199329933993499359936993799389939994099419942994399449945994699479948994999509951995299539954995599569957995899599960996199629963996499659966996799689969997099719972997399749975997699779978997999809981998299839984998599869987998899899990999199929993999499959996999799989999"
	.size	_ZZ23to_string_tree_bigtablemPcE8bigtable, 40000

	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"khuong     "
	.size	.L.str, 12

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"scalar  "
	.size	.L.str.1, 9

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"scalar_v2  "
	.size	.L.str.2, 12

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"backlinear "
	.size	.L.str.3, 12

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"linear "
	.size	.L.str.4, 8

	.type	.L.str.5,@object                # @.str.5
.L.str.5:
	.asciz	"tree   "
	.size	.L.str.5, 8

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"treetst "
	.size	.L.str.6, 9

	.type	.L.str.7,@object                # @.str.7
.L.str.7:
	.asciz	"treest "
	.size	.L.str.7, 8

	.type	.L.str.8,@object                # @.str.8
.L.str.8:
	.asciz	"treebt "
	.size	.L.str.8, 8

	.type	.L.str.9,@object                # @.str.9
.L.str.9:
	.asciz	"sse2   "
	.size	.L.str.9, 8

	.type	.L.str.10,@object               # @.str.10
.L.str.10:
	.asciz	"sse2(2) "
	.size	.L.str.10, 9

	.type	.L.str.11,@object               # @.str.11
.L.str.11:
	.asciz	"avx2    "
	.size	.L.str.11, 9

	.type	.L.str.12,@object               # @.str.12
.L.str.12:
	.asciz	"avx512ifma    "
	.size	.L.str.12, 15

	.type	.L.str.13,@object               # @.str.13
.L.str.13:
	.asciz	"vector::_M_realloc_append"
	.size	.L.str.13, 26

	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.p2align	3, 0x0
	.type	DW.ref.__gxx_personality_v0,@object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"Ubuntu clang version 18.1.3 (1ubuntu1)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __gxx_personality_v0
	.addrsig_sym _Unwind_Resume
	.addrsig_sym _ZZ23to_string_tree_bigtablemPcE8bigtable
	.addrsig_sym _ZSt4cout
