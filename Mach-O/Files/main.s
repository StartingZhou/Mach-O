	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	callq	_objc_autoreleasePoolPush
	leaq	L__unnamed_cfstring_(%rip), %rsi
	movq	%rsi, %rdi
	movq	%rax, -32(%rbp)         ## 8-byte Spill
	movb	$0, %al
	callq	_NSLog
	movq	L_OBJC_CLASSLIST_REFERENCES_$_(%rip), %rsi
	movq	L_OBJC_SELECTOR_REFERENCES_(%rip), %rdi
	movq	%rdi, -40(%rbp)         ## 8-byte Spill
	movq	%rsi, %rdi
	movq	-40(%rbp), %rsi         ## 8-byte Reload
	callq	*_objc_msgSend@GOTPCREL(%rip)
	movq	L_OBJC_SELECTOR_REFERENCES_.2(%rip), %rsi
	movq	%rax, %rdi
	callq	*_objc_msgSend@GOTPCREL(%rip)
	movl	$10, %ecx
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	L_OBJC_SELECTOR_REFERENCES_.4(%rip), %rsi
	movq	%rax, %rdi
	movl	%ecx, %edx
	callq	*_objc_msgSend@GOTPCREL(%rip)
	movl	$1, -4(%rbp)
	movq	-32(%rbp), %rdi         ## 8-byte Reload
	movl	%eax, -44(%rbp)         ## 4-byte Spill
	callq	_objc_autoreleasePoolPop
	movl	-4(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Hello World!"

	.section	__DATA,__cfstring
	.p2align	3               ## @_unnamed_cfstring_
L__unnamed_cfstring_:
	.quad	___CFConstantStringClassReference
	.long	1992                    ## 0x7c8
	.space	4
	.quad	L_.str
	.quad	12                      ## 0xc

	.section	__DATA,__objc_classrefs,regular,no_dead_strip
	.p2align	3               ## @"OBJC_CLASSLIST_REFERENCES_$_"
L_OBJC_CLASSLIST_REFERENCES_$_:
	.quad	_OBJC_CLASS_$_Adding

	.section	__TEXT,__objc_methname,cstring_literals
L_OBJC_METH_VAR_NAME_:                  ## @OBJC_METH_VAR_NAME_
	.asciz	"alloc"

	.section	__DATA,__objc_selrefs,literal_pointers,no_dead_strip
	.p2align	3               ## @OBJC_SELECTOR_REFERENCES_
L_OBJC_SELECTOR_REFERENCES_:
	.quad	L_OBJC_METH_VAR_NAME_

	.section	__TEXT,__objc_methname,cstring_literals
L_OBJC_METH_VAR_NAME_.1:                ## @OBJC_METH_VAR_NAME_.1
	.asciz	"init"

	.section	__DATA,__objc_selrefs,literal_pointers,no_dead_strip
	.p2align	3               ## @OBJC_SELECTOR_REFERENCES_.2
L_OBJC_SELECTOR_REFERENCES_.2:
	.quad	L_OBJC_METH_VAR_NAME_.1

	.section	__TEXT,__objc_methname,cstring_literals
L_OBJC_METH_VAR_NAME_.3:                ## @OBJC_METH_VAR_NAME_.3
	.asciz	"addX:Y:"

	.section	__DATA,__objc_selrefs,literal_pointers,no_dead_strip
	.p2align	3               ## @OBJC_SELECTOR_REFERENCES_.4
L_OBJC_SELECTOR_REFERENCES_.4:
	.quad	L_OBJC_METH_VAR_NAME_.3

	.section	__DATA,__objc_imageinfo,regular,no_dead_strip
L_OBJC_IMAGE_INFO:
	.long	0
	.long	64


.subsections_via_symbols
