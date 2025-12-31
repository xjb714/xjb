	.file	"main.cpp"
                                        # Start of file scope inline assembly
	.globl	_ZSt21ios_base_library_initv

                                        # End of file scope inline assembly
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
.LBB0_2:
	.cfi_def_cfa_offset 16
.Ltmp2:
	movq	%rax, %rdi
	callq	__clang_call_terminate
.Lfunc_end0:
	.size	_ZNSt13random_deviceD2Ev, .Lfunc_end0-_ZNSt13random_deviceD2Ev
	.cfi_endproc
	.section	.gcc_except_table._ZNSt13random_deviceD2Ev,"aG",@progbits,_ZNSt13random_deviceD2Ev,comdat
	.p2align	2, 0x0
GCC_except_table0:
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
.Lfunc_end1:
	.size	_Z9get_cyclev, .Lfunc_end1-_Z9get_cyclev
	.cfi_endproc
                                        # -- End function
	.globl	_Z5getnsv                       # -- Begin function _Z5getnsv
	.p2align	4
	.type	_Z5getnsv,@function
_Z5getnsv:                              # 
	.cfi_startproc
# %bb.0:
	jmp	_ZNSt6chrono3_V212system_clock3nowEv # TAILCALL
.Lfunc_end2:
	.size	_Z5getnsv, .Lfunc_end2-_Z5getnsv
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
.LBB3_1:                                # =>This Inner Loop Header: Depth=1
	movl	$gen, %edi
	callq	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	andnq	%rbx, %rax, %rcx
	je	.LBB3_1
# %bb.2:
	vmovq	%rax, %xmm0
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	_Z25gen_double_filter_NaN_Infv, .Lfunc_end3-_Z25gen_double_filter_NaN_Infv
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
.LCPI4_0:
	.quad	1                               # 0x1
.LCPI4_1:
	.quad	1073741823                      # 0x3fffffff
.LCPI4_2:
	.quad	9223372035781033984             # 0x7fffffffc0000000
.LCPI4_3:
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
	jb	.LBB4_4
# %bb.1:
	vpbroadcastq	.LCPI4_0(%rip), %zmm0   # zmm0 = [1,1,1,1,1,1,1,1]
	vpbroadcastq	.LCPI4_1(%rip), %zmm1   # zmm1 = [1073741823,1073741823,1073741823,1073741823,1073741823,1073741823,1073741823,1073741823]
	vpbroadcastq	.LCPI4_2(%rip), %zmm2   # zmm2 = [9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984]
	vpbroadcastq	.LCPI4_3(%rip), %zmm3   # zmm3 = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	xorl	%eax, %eax
	.p2align	4
.LBB4_2:                                # =>This Inner Loop Header: Depth=1
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
	ja	.LBB4_6
# %bb.3:                                #   in Loop: Header=BB4_2 Depth=1
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
	jmp	.LBB4_2
.LBB4_6:
	vmovdqu	1224(%rdi), %ymm4
	vpsrlq	$1, 1216(%rdi), %ymm6
	leaq	2464(%rdi), %rax
	movl	$181, %ecx
	vpandq	.LCPI4_2(%rip){1to4}, %ymm6, %ymm6
	vptestmq	.LCPI4_0(%rip){1to4}, %ymm4, %k1
	vpsrlq	$1, %ymm4, %ymm5
	vpandq	.LCPI4_1(%rip){1to4}, %ymm5, %ymm5
	vpternlogq	$86, 2464(%rdi), %ymm5, %ymm6 # ymm6 = mem ^ (ymm6 | ymm5)
	vpxorq	.LCPI4_3(%rip){1to4}, %ymm6, %ymm6 {%k1}
	vmovdqu	%ymm6, 1216(%rdi)
	.p2align	4
.LBB4_7:                                # =>This Inner Loop Header: Depth=1
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
	ja	.LBB4_9
# %bb.8:                                #   in Loop: Header=BB4_7 Depth=1
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
	jmp	.LBB4_7
.LBB4_9:
	movb	$7, %cl
	movabsq	$-5403634167711393303, %rsi     # imm = 0xB5026F5AA96619E9
	kmovd	%ecx, %k1
	movq	$-2147483648, %rcx              # imm = 0x80000000
	vmovdqu64	2472(%rdi), %zmm1 {%k1} {z}
	vmovdqu64	(%rax), %zmm0 {%k1} {z}
	vptestmq	.LCPI4_0(%rip){1to8}, %zmm1, %k2
	vpsrlq	$1, %zmm1, %zmm2
	vpandq	.LCPI4_1(%rip){1to8}, %zmm2, %zmm2
	vpsrlq	$1, %zmm0, %zmm0
	vpbroadcastq	.LCPI4_3(%rip), %zmm1 {%k2} {z} # zmm1 {%k2} {z} = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	vpternlogq	$236, .LCPI4_2(%rip){1to8}, %zmm2, %zmm0 # zmm0 = (zmm0 & m64bcst) | zmm2
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
	jmp	.LBB4_5
.LBB4_4:
	movq	(%rdi,%rcx,8), %rax
	incq	%rcx
.LBB4_5:
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
.Lfunc_end4:
	.size	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv, .Lfunc_end4-_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
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
.Lfunc_end5:
	.size	_Z35gen_double_filter_NaN_Inf_subnormalv, .Lfunc_end5-_Z35gen_double_filter_NaN_Inf_subnormalv
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
.LBB6_1:                                # =>This Inner Loop Header: Depth=1
	movl	$gen, %edi
	callq	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	movl	%eax, %ecx
	notl	%ecx
	testl	$2139095040, %ecx               # imm = 0x7F800000
	je	.LBB6_1
# %bb.2:
	vmovd	%eax, %xmm0
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end6:
	.size	_Z24gen_float_filter_NaN_Infv, .Lfunc_end6-_Z24gen_float_filter_NaN_Infv
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
.Lfunc_end7:
	.size	_Z34gen_float_filter_NaN_Inf_subnormalv, .Lfunc_end7-_Z34gen_float_filter_NaN_Inf_subnormalv
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function _Z9init_datav
.LCPI8_0:
	.quad	1                               # 0x1
.LCPI8_1:
	.quad	1073741823                      # 0x3fffffff
.LCPI8_2:
	.quad	9223372035781033984             # 0x7fffffffc0000000
.LCPI8_3:
	.quad	-5403634167711393303            # 0xb5026f5aa96619e9
	.text
	.globl	_Z9init_datav
	.p2align	4
	.type	_Z9init_datav,@function
_Z9init_datav:                          # 
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	$400000000, %edi                # imm = 0x17D78400
	callq	_Znam
	vpbroadcastq	.LCPI8_0(%rip), %zmm0   # zmm0 = [1,1,1,1,1,1,1,1]
	vpbroadcastq	.LCPI8_1(%rip), %zmm1   # zmm1 = [1073741823,1073741823,1073741823,1073741823,1073741823,1073741823,1073741823,1073741823]
	vpbroadcastq	.LCPI8_2(%rip), %zmm2   # zmm2 = [9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984]
	vpbroadcastq	.LCPI8_3(%rip), %zmm3   # zmm3 = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	vpbroadcastq	.LCPI8_1(%rip), %ymm4   # ymm4 = [1073741823,1073741823,1073741823,1073741823]
	vpbroadcastq	.LCPI8_2(%rip), %ymm5   # ymm5 = [9223372035781033984,9223372035781033984,9223372035781033984,9223372035781033984]
	vpbroadcastq	.LCPI8_0(%rip), %ymm6   # ymm6 = [1,1,1,1]
	vpbroadcastq	.LCPI8_3(%rip), %ymm7   # ymm7 = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	movq	gen+2496(%rip), %r10
	movb	$7, %dl
	movq	$-2147483648, %rsi              # imm = 0x80000000
	movabsq	$-5403634167711393303, %rdi     # imm = 0xB5026F5AA96619E9
	movabsq	$22906492245, %r8               # imm = 0x555555555
	movabsq	$8202884508482404352, %r9       # imm = 0x71D67FFFEDA60000
	xorl	%ecx, %ecx
	movq	%rax, data(%rip)
	jmp	.LBB8_1
	.p2align	4
.LBB8_12:                               #   in Loop: Header=BB8_1 Depth=1
	kmovd	%edx, %k1
	vmovdqu64	gen+2472(%rip), %zmm9 {%k1} {z}
	vmovdqu64	gen+2464(%rip), %zmm8 {%k1} {z}
	vpsrlq	$1, %zmm9, %zmm10
	vptestmq	%zmm0, %zmm9, %k2
	vpsrlq	$1, %zmm8, %zmm8
	vpandq	%zmm1, %zmm10, %zmm10
	vpbroadcastq	.LCPI8_3(%rip), %zmm9 {%k2} {z} # zmm9 {%k2} {z} = [13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313,13043109905998158313]
	vpternlogq	$236, %zmm2, %zmm10, %zmm8 # zmm8 = (zmm8 & zmm2) | zmm10
	vpternlogq	$150, gen+1216(%rip), %zmm8, %zmm9 # zmm9 = zmm9 ^ zmm8 ^ mem
	vmovdqu64	%zmm9, gen+2464(%rip) {%k1}
	movq	gen+2488(%rip), %r10
	movq	gen(%rip), %r11
	movl	%r11d, %ebx
	andq	%rsi, %r10
	andl	$2147483646, %ebx               # imm = 0x7FFFFFFE
	orq	%r10, %rbx
	movl	%r11d, %r10d
	andl	$1, %r10d
	shrq	%rbx
	xorq	gen+1240(%rip), %rbx
	negq	%r10
	andq	%rdi, %r10
	xorq	%rbx, %r10
	movq	%r10, gen+2488(%rip)
	movl	$1, %r10d
.LBB8_6:                                #   in Loop: Header=BB8_1 Depth=1
	movq	%r11, %rbx
	shrq	$29, %rbx
	andq	%r8, %rbx
	xorq	%r11, %rbx
	movq	%rbx, %r11
	shlq	$17, %r11
	andq	%r9, %r11
	xorq	%rbx, %r11
	movl	%r11d, %ebx
	notl	%ebx
	testl	$2139095040, %ebx               # imm = 0x7F800000
	jne	.LBB8_7
.LBB8_1:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB8_3 Depth 2
                                        #     Child Loop BB8_10 Depth 2
	cmpq	$312, %r10                      # imm = 0x138
	jb	.LBB8_5
# %bb.2:                                #   in Loop: Header=BB8_1 Depth=1
	movl	$180, %r10d
	.p2align	4
.LBB8_3:                                #   Parent Loop BB8_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	leaq	gen-1432(,%r10,8), %r11
	leaq	-164(%r10), %rbx
	vmovdqu64	(%r11), %zmm8
	vpsrlq	$1, -8(%r11), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, 1240(%r11), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, -8(%r11)
	vmovdqu64	64(%r11), %zmm8
	vpsrlq	$1, 56(%r11), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, 1304(%r11), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, 56(%r11)
	vmovdqu64	128(%r11), %zmm8
	vpsrlq	$1, 120(%r11), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, 1368(%r11), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, 120(%r11)
	cmpq	$143, %rbx
	ja	.LBB8_9
# %bb.4:                                #   in Loop: Header=BB8_3 Depth=2
	vmovdqu64	gen-1240(,%r10,8), %zmm8
	vpsrlq	$1, gen-1248(,%r10,8), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, gen(,%r10,8), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, gen-1248(,%r10,8)
	addq	$32, %r10
	jmp	.LBB8_3
	.p2align	4
.LBB8_9:                                #   in Loop: Header=BB8_1 Depth=1
	vmovdqu	gen+1224(%rip), %ymm8
	vpsrlq	$1, gen+1216(%rip), %ymm10
	movl	$181, %r10d
	vpsrlq	$1, %ymm8, %ymm9
	vpand	%ymm5, %ymm10, %ymm10
	vptestmq	%ymm6, %ymm8, %k1
	vpand	%ymm4, %ymm9, %ymm9
	vpternlogq	$86, gen+2464(%rip), %ymm9, %ymm10 # ymm10 = mem ^ (ymm10 | ymm9)
	vpxorq	%ymm7, %ymm10, %ymm10 {%k1}
	vmovdqu	%ymm10, gen+1216(%rip)
	.p2align	4
.LBB8_10:                               #   Parent Loop BB8_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	leaq	gen-192(,%r10,8), %r11
	leaq	-165(%r10), %rbx
	vmovdqu64	(%r11), %zmm8
	vpsrlq	$1, -8(%r11), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, -1256(%r11), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, -8(%r11)
	vmovdqu64	64(%r11), %zmm8
	vpsrlq	$1, 56(%r11), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, -1192(%r11), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, 56(%r11)
	vmovdqu64	128(%r11), %zmm8
	vpsrlq	$1, 120(%r11), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, -1128(%r11), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, 120(%r11)
	cmpq	$143, %rbx
	ja	.LBB8_12
# %bb.11:                               #   in Loop: Header=BB8_10 Depth=2
	vmovdqu64	gen(,%r10,8), %zmm8
	vpsrlq	$1, gen-8(,%r10,8), %zmm10
	vpsrlq	$1, %zmm8, %zmm9
	vpandq	%zmm2, %zmm10, %zmm10
	vptestmq	%zmm0, %zmm8, %k1
	vpandq	%zmm1, %zmm9, %zmm9
	vpternlogq	$86, gen-1256(,%r10,8), %zmm9, %zmm10 # zmm10 = mem ^ (zmm10 | zmm9)
	vpxorq	%zmm3, %zmm10, %zmm10 {%k1}
	vmovdqu64	%zmm10, gen-8(,%r10,8)
	addq	$32, %r10
	jmp	.LBB8_10
	.p2align	4
.LBB8_5:                                #   in Loop: Header=BB8_1 Depth=1
	movq	gen(,%r10,8), %r11
	incq	%r10
	jmp	.LBB8_6
.LBB8_7:                                #   in Loop: Header=BB8_1 Depth=1
	movl	%r11d, %ebx
	andl	$134201152, %ebx                # imm = 0x7FFBF40
	shlq	$37, %rbx
	xorq	%r11, %rbx
	shrq	$43, %rbx
	xorl	%r11d, %ebx
	movl	%ebx, (%rax,%rcx,4)
	incq	%rcx
	cmpq	$100000000, %rcx                # imm = 0x5F5E100
	jne	.LBB8_1
# %bb.8:
	movq	%r10, gen+2496(%rip)
	popq	%rbx
	.cfi_def_cfa_offset 8
	vzeroupper
	retq
.Lfunc_end8:
	.size	_Z9init_datav, .Lfunc_end8-_Z9init_datav
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          # -- Begin function main
.LCPI9_0:
	.long	27370496                        # 0x1a1a400
	.long	0                               # 0x0
	.long	0                               # 0x0
	.long	0                               # 0x0
.LCPI9_3:
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
.LCPI9_6:
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
.LCPI9_1:
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
.LCPI9_2:
	.quad	160                             # 0xa0
.LCPI9_4:
	.quad	0x3e45798ee2308c3a              #  1.0E-8
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2, 0x0
.LCPI9_5:
	.long	27370496                        # 0x1a1a400
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
	subq	$120, %rsp
	.cfi_def_cfa_offset 176
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	vstmxcsr	48(%rsp)
	movl	$gen, %edi
	orl	$32832, 48(%rsp)                # imm = 0x8040
	vldmxcsr	48(%rsp)
	callq	_ZNSt23mersenne_twister_engineImLm64ELm312ELm156ELm31ELm13043109905998158313ELm29ELm6148914691236517205ELm17ELm8202884508482404352ELm37ELm18444473444759240704ELm43ELm6364136223846793005EEclEv
	movq	%rax, %rbx
	callq	_ZNSt6chrono3_V212system_clock3nowEv
	vmovd	.LCPI9_5(%rip), %xmm0           # xmm0 = [27370496,0,0,0]
	vmovdqu64	.LCPI9_1(%rip), %zmm1   # zmm1 = [45035996,450359963,4503599628,45035996274,450359962738,4503599627371,45035996273705,450359962737050]
                                        # AlignMOV convert to UnAlignMOV 
	vpbroadcastq	.LCPI9_2(%rip), %zmm2   # zmm2 = [160,160,160,160,160,160,160,160]
	vmovq	.LCPI9_6(%rip), %xmm3           # xmm3 = [7,15,23,31,39,47,55,63,0,0,0,0,0,0,0,0]
	leaq	(%rbx,%rbx), %rdi
	movl	$100000000, %r13d               # imm = 0x5F5E100
	movq	%rax, 32(%rsp)                  # 8-byte Spill
	#APP
	rdtsc
	#NO_APP
	movq	%rbx, %r8
	movl	%r8d, %ebp
	movq	%rax, 16(%rsp)                  # 8-byte Spill
	movq	%rdx, 24(%rsp)                  # 8-byte Spill
	.p2align	4
.LBB9_1:                                # =>This Inner Loop Header: Depth=1
	movl	%edi, %eax
	shrl	$31, %eax
	movl	%edi, %ecx
	shrl	$23, %ecx
	movb	$45, 48(%rsp)
	leaq	48(%rsp,%rax), %rax
	movq	%rax, 8(%rsp)                   # 8-byte Spill
	movl	%edi, %eax
	andl	$8388607, %eax                  # imm = 0x7FFFFF
	testb	%cl, %cl
	je	.LBB9_4
# %bb.2:                                #   in Loop: Header=BB9_1 Depth=1
	movzbl	%cl, %ebx
	cmpl	$255, %ebx
	je	.LBB9_3
# %bb.7:                                #   in Loop: Header=BB9_1 Depth=1
	addq	$-150, %rbx
	orq	$8388608, %rax                  # imm = 0x800000
	imulq	$1233, %rbx, %r12               # imm = 0x4D1
	testl	$8388607, %edi                  # imm = 0x7FFFFF
	je	.LBB9_9
.LBB9_8:                                #   in Loop: Header=BB9_1 Depth=1
	xorl	%r10d, %r10d
	xorl	%esi, %esi
.LBB9_10:                               #   in Loop: Header=BB9_1 Depth=1
	sarq	$12, %r12
	movl	$31, %edx
	vmovdqa64	%zmm0, %zmm5
	subq	%r12, %rdx
	movq	%r12, %rcx
	notq	%rcx
	movl	%edx, %r14d
	andl	$-16, %r14d
	imulq	$1701, %rcx, %rcx               # imm = 0x6A5
	movl	%edx, %r15d
	andl	$15, %r15d
	movq	%rdx, %r9
	shrq	$4, %r9
	imull	$1701, %r14d, %r14d             # imm = 0x6A5
	shrq	$9, %rcx
	addl	$-54432, %r14d                  # imm = 0xFFFF2B60
	leal	(%rcx,%rbx), %r11d
	shrl	$9, %r14d
	addl	%r15d, %r14d
	subl	%r14d, %ecx
	movl	%edx, %r14d
	andl	$12, %r14d
	andl	$3, %edx
	movl	_ZZN8xjb_compL10xjb32_compEfPcE21pow10_base_table_pow5+40(%r14), %r14d
	movzbl	_ZZN8xjb_compL10xjb32_compEfPcE21pow10_base_table_pow5+56(%rdx), %edx
	imulq	%r14, %rdx
	movb	$28, %r14b
	mulxq	_ZZN8xjb_compL10xjb32_compEfPcE21pow10_base_table_pow5(,%r9,8), %r9, %rdx
	shrdq	%cl, %rdx, %r9
	shrxq	%rcx, %rdx, %rdx
	testb	$64, %cl
	movl	%eax, %ecx
	notl	%ecx
	cmoveq	%r9, %rdx
	leal	37(%r11), %r9d
	subb	%r11b, %r14b
	andl	$1, %ecx
	xorl	%r15d, %r15d
	shlxq	%r9, %rax, %rax
	mulxq	%rax, %r9, %r9
	movb	$36, %al
	shrxq	%r14, %rdx, %rdx
	addq	%rcx, %rdx
	shrxq	%r10, %rdx, %rcx
	movl	$8, %r10d
	bzhiq	%rax, %r9, %rax
	leaq	(%rax,%rdx), %r11
	shrq	$36, %r11
	cmpq	%rax, %rcx
	movq	8(%rsp), %rcx                   # 8-byte Reload
	seta	%r15b
	shrq	$36, %r9
	xorl	%r14d, %r14d
	addq	%r11, %r9
	vpbroadcastq	%r9, %zmm4
	cmpq	$1000000, %r9                   # imm = 0xF4240
	vpmadd52luq	%zmm1, %zmm4, %zmm5
	setb	%r14b
	cmpq	$10000000, %r9                  # imm = 0x989680
	adcq	$0, %r14
	movl	$808464432, (%rcx)              # imm = 0x30303030
	leal	(,%r14,8), %edx
	subq	%r14, %r10
	movabsq	$423878912376825, %r14          # imm = 0x18183FFFFFFF9
	vpmullq	%zmm2, %zmm5, %zmm4
	vpermb	%zmm4, %zmm3, %zmm4
	vmovq	%xmm4, %rcx
	shrxq	%rdx, %rcx, %rdx
	lzcntq	%rdx, %rcx
	shrl	$3, %ecx
	xorq	$7, %rcx
	orq	%r11, %r15
	leaq	(%rax,%rax,4), %r11
	cmoveq	%r10, %rcx
	shrq	$32, %rax
	addq	%r14, %rax
	addq	%r11, %rax
	shrq	$35, %rax
	testb	%sil, %sil
	jne	.LBB9_11
.LBB9_12:                               #   in Loop: Header=BB9_1 Depth=1
	addq	%r10, %r12
	movl	$1, %r11d
	movl	$1, %r15d
	subq	%r12, %r11
	leaq	1(%r12), %rsi
	cmpq	$7, %r12
	cmovbq	%rsi, %r15
	movq	%rsi, 40(%rsp)                  # 8-byte Spill
	cmpq	$-3, %r12
	movl	$0, %esi
	cmovbq	%rsi, %r11
	movq	%r15, %r14
	adcq	$0, %r14
	cmpq	$6, %r12
	ja	.LBB9_14
# %bb.13:                               #   in Loop: Header=BB9_1 Depth=1
	leaq	3(%r12), %rbx
	addq	$2, %rcx
	cmpq	%rcx, %rbx
	cmovaq	%rbx, %rcx
	jmp	.LBB9_15
	.p2align	4
.LBB9_14:                               #   in Loop: Header=BB9_1 Depth=1
	cmpq	$-3, %r12
	movq	%r13, %rsi
	movq	%r8, %r13
	movl	%ebp, %r8d
	setb	%bl
	testq	%rcx, %rcx
	setne	%bpl
	andb	%bl, %bpl
	movzbl	%bpl, %ebx
	movl	%r8d, %ebp
	movq	%r13, %r8
	movq	%rsi, %r13
	leaq	1(%rcx,%rbx), %rcx
.LBB9_15:                               #   in Loop: Header=BB9_1 Depth=1
	movabsq	$3472328296227680304, %rsi      # imm = 0x3030303030303030
	orq	%rsi, %rdx
	movq	8(%rsp), %rsi                   # 8-byte Reload
	leaq	(%rsi,%r11), %rbx
	movq	%rdx, (%rsi,%r11)
	movq	%rax, (%r10,%rbx)
	movq	(%r15,%rbx), %rax
	movq	%rax, (%r14,%rbx)
	movb	$46, (%rsi,%r15)
	cmpl	$99999, %r9d                    # imm = 0x1869F
	jbe	.LBB9_16
.LBB9_17:                               #   in Loop: Header=BB9_1 Depth=1
	movq	%r12, %rax
	negq	%rax
	movq	%r12, %rdx
	cmovsq	%r12, %rax
	shrq	$63, %rdx
	addq	$3, %r12
	imulq	$103, %rax, %rsi
	shll	$9, %edx
	shlq	$24, %rax
	orq	%rdx, %rax
	shrq	$10, %rsi
	imulq	$-167706624, %rsi, %rdx         # imm = 0xF6010000
	cmpq	$10, %r12
	leaq	808463205(%rdx,%rax), %rax
	movl	$0, %edx
	cmovaeq	%rax, %rdx
	movq	%rdx, (%rbx,%rcx)
.LBB9_18:                               #   in Loop: Header=BB9_1 Depth=1
	addq	%r8, %rdi
	addl	%r8d, %ebp
	decq	%r13
	jne	.LBB9_1
	jmp	.LBB9_19
.LBB9_4:                                #   in Loop: Header=BB9_1 Depth=1
	testl	$8388607, %edi                  # imm = 0x7FFFFF
	je	.LBB9_6
# %bb.5:                                #   in Loop: Header=BB9_1 Depth=1
	movq	$-183717, %r12                  # imm = 0xFFFD325B
	movq	$-149, %rbx
	jmp	.LBB9_8
.LBB9_3:                                #   in Loop: Header=BB9_1 Depth=1
	movq	8(%rsp), %rdx                   # 8-byte Reload
	testl	$8388607, %edi                  # imm = 0x7FFFFF
	movl	$7233902, %eax                  # imm = 0x6E616E
	movl	$6712937, %ecx                  # imm = 0x666E69
	cmovel	%ecx, %eax
	movl	%eax, (%rdx)
	jmp	.LBB9_18
.LBB9_11:                               #   in Loop: Header=BB9_1 Depth=1
	cmpq	$-119, %rbx
	sete	%sil
	cmpq	$64, %rbx
	sete	%r11b
	orb	%sil, %r11b
	cmpq	$67, %rbx
	sete	%sil
	orb	%r11b, %sil
	movzbl	%sil, %esi
	addq	%rsi, %rax
	jmp	.LBB9_12
.LBB9_16:                               #   in Loop: Header=BB9_1 Depth=1
	movq	2(%rbx), %rax
	movabsq	$1085102592571150095, %rdx      # imm = 0xF0F0F0F0F0F0F0F
	movq	40(%rsp), %r12                  # 8-byte Reload
	andq	%rdx, %rax
	tzcntq	%rax, %rax
	shrl	$3, %eax
	movzbl	2(%rbx,%rax), %r9d
	leaq	2(%rax), %rdx
	subq	%rdx, %rcx
	subq	%rdx, %r12
	movb	%r9b, (%rbx)
	movq	3(%rbx,%rax), %rax
	movq	%rax, 2(%rbx)
	xorl	%eax, %eax
	cmpq	$1, %rcx
	setne	%al
	addq	%rax, %rcx
	jmp	.LBB9_17
.LBB9_9:                                #   in Loop: Header=BB9_1 Depth=1
	addq	$-512, %r12                     # imm = 0xFE00
	movb	$1, %sil
	movl	$1, %r10d
	jmp	.LBB9_10
.LBB9_6:                                #   in Loop: Header=BB9_1 Depth=1
	movq	8(%rsp), %rax                   # 8-byte Reload
	movl	$3157552, (%rax)                # imm = 0x302E30
	jmp	.LBB9_18
.LBB9_19:
	movq	24(%rsp), %rcx                  # 8-byte Reload
	shlq	$32, %rcx
	orq	16(%rsp), %rcx                  # 8-byte Folded Reload
	#APP
	rdtsc
	#NO_APP
	movq	%rdx, %r14
	shlq	$32, %r14
	orq	%rax, %r14
	subq	%rcx, %r14
	vzeroupper
	callq	_ZNSt6chrono3_V212system_clock3nowEv
	subq	32(%rsp), %rax                  # 8-byte Folded Reload
	vmovsd	.LCPI9_4(%rip), %xmm1           # xmm1 = [1.0E-8,0.0E+0]
	movl	$.L.str, %edi
	vcvtsi2sd	%rax, %xmm15, %xmm0
	movb	$2, %al
	vmulsd	%xmm1, %xmm0, %xmm0
	vcvtusi2sd	%r14, %xmm15, %xmm2
	vmulsd	%xmm1, %xmm2, %xmm1
	callq	printf
	vmovd	%ebp, %xmm0
	leaq	48(%rsp), %rsi
	movl	$.L.str.2, %edi
	movb	$1, %al
	vcvtss2sd	%xmm0, %xmm0, %xmm0
	callq	printf
	xorl	%eax, %eax
	addq	$120, %rsp
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
.Lfunc_end9:
	.size	main, .Lfunc_end9-main
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
.Lfunc_end10:
	.size	__clang_call_terminate, .Lfunc_end10-__clang_call_terminate
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
	je	.LBB11_3
# %bb.2:
	movq	24(%rsp), %rsi
	incq	%rsi
	callq	_ZdlPvm
.LBB11_3:
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
.LBB11_4:                               # =>This Inner Loop Header: Depth=1
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
	jne	.LBB11_4
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
.LBB11_6:
	.cfi_def_cfa_offset 64
.Ltmp5:
	movq	8(%rsp), %rdi
	movq	%rax, %rbx
	cmpq	%r14, %rdi
	je	.LBB11_8
# %bb.7:
	movq	24(%rsp), %rsi
	incq	%rsi
	callq	_ZdlPvm
.LBB11_8:
	movq	%rbx, %rdi
	callq	_Unwind_Resume@PLT
.Lfunc_end11:
	.size	_GLOBAL__sub_I_main.cpp, .Lfunc_end11-_GLOBAL__sub_I_main.cpp
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2, 0x0
GCC_except_table11:
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
	.uleb128 .Lfunc_end11-.Ltmp4            #   Call between .Ltmp4 and .Lfunc_end11
	.byte	0                               #     has no landing pad
	.byte	0                               #   On action: cleanup
.Lcst_end1:
	.p2align	2, 0x0
                                        # -- End function
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

	.type	.L.str,@object                  # 
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"every number cost %lf ns,%lf cycle\n"
	.size	.L.str, 36

	.type	.L.str.2,@object                # 
.L.str.2:
	.asciz	"data = %.8e \n buf = %s \n"
	.size	.L.str.2, 25

	.type	.L.str.3,@object                # 
.L.str.3:
	.asciz	"default"
	.size	.L.str.3, 8

	.type	_ZZN8xjb_compL10xjb32_compEfPcE21pow10_base_table_pow5,@object # 
	.section	.rodata,"a",@progbits
	.p2align	4, 0x0
_ZZN8xjb_compL10xjb32_compEfPcE21pow10_base_table_pow5:
	.quad	-3480967307441105733            # 0xcfb11ead453994bb
	.quad	-1831394126398103204            # 0xe69594bec44de15c
	.quad	-9223372036854775808            # 0x8000000000000000
	.quad	-8206744073709551616            # 0x8e1bc9bf04000000
	.quad	-7078060301547948642            # 0x9dc5ada82b70b59e
	.quad	2684354560001                   # 0x27100000001
	.quad	1048576000000390625             # 0xe8d4a510005f5e1
	.quad	2098791681                      # 0x7d190501
	.size	_ZZN8xjb_compL10xjb32_compEfPcE21pow10_base_table_pow5, 64

	.section	.init_array,"aw",@init_array
	.p2align	3, 0x0
	.quad	_GLOBAL__sub_I_main.cpp
	.ident	"Intel(R) oneAPI DPC++/C++ Compiler 2025.3.1 (2025.3.1.20251023)"
	.section	".note.GNU-stack","",@progbits
