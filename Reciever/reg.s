.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.section .text
.globl reg0
.align
.func reg0

reg0:
		MOV r1, r0		// Put value of r0 in r1
						//Not necessary but just extra cause I am.
.endfunc
.end
