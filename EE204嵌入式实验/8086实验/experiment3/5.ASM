
;--------------------------------------------------------------------------
;
;              Build this with the "Source" menu using
;                     "Build All" option
;
;--------------------------------------------------------------------------
;
;                           实验三程序通用框架
;
;--------------------------------------------------------------------------
; 功能： 程序框架					                            				   |
; 编写：《嵌入式系统原理与实验》课程组                   				   |
; 版本：3.5
; 修订：B
;--------------------------------------------------------------------------


		DOSSEG
		.MODEL	SMALL		; 设定8086汇编程序使用Small model
		.8086				; 设定采用8086汇编指令集

;-----------------------------------------------------------
;	定义堆栈段                                             |
;-----------------------------------------------------------
	.stack 100h				; 定义256字节容量的堆栈

;-------------------------------------------------------------------------
;	符号定义                                                              |
;-------------------------------------------------------------------------
;
;
; 8253芯片端口地址 （Port Address):
L8253T0			EQU	0100H			; Timer0's port number in I/O space
L8253T1			EQU 	0102H			; Timer1's port number in I/O space
L8253T2			EQU 	0104H			; Timer2's port number in I/O space
L8253CS			EQU 	0106H			; 8253 Control Register's port number in I/O space
;
; 8255芯片端口地址 （Port Address):
L8255PA			EQU	0121H			; Port A's port number in I/O space
L8255PB			EQU 	0123H			; Port B's port number in I/O space
L8255PC			EQU 	0125H			; Port C's port number in I/O space
L8255CS			EQU 	0127H			; 8255 Control Register's port number in I/O space
;
;  中断矢量号定义
IRQNum			EQU	41			; 中断矢量号,要根据学号计算得到后更新此定义。

Patch_Protues	EQU	IN AL, 0	;	Simulation Patch for Proteus, please ignore this line

;=======================================================================
; 宏定义
;=======================================================================

; 修补Proteus仿真的BUG，参见程序段中的使用说明
    WaitForHWInt MACRO INTNum		; INTNum is the HW INT number
		MOV AL, INTNum   			;
		OUT 0,AL					;
		STI
		CLI
    ENDM



;-----------------------------------------------------------
;	定义数据段                                             |
;-----------------------------------------------------------
		.data					; 定义数据段;

DelayShort	dw	40				; 短延时参量	
DelayLong	dw	4000			; 长延时参量


; SEGTAB is the code for displaying "0-F", and some of the codes may not be correct. Find and correct the errors.
	SEGTAB  DB 3FH	; 7-Segment Tube
		DB 06H	;
		DB 5BH	;            a a a
		DB 4FH	;         f         b
		DB 66H	;         f         b
		DB 6DH	;         f         b
		DB 7DH	;            g g g 
		DB 07H	;         e         c
		DB 7FH	;         e         c
		DB 6FH	;         e         c
        	DB 77H	;            d d d     h h h
		DB 7CH	; ----------------------------------
		DB 39H	;       b7 b6 b5 b4 b3 b2 b1 b0
		DB 5EH	;       DP  g  f  e  d  c  b  a
		DB 79H	;
		DB 71H	;

;-----------------------------------------------------------
;	定义代码段                                             |
;-----------------------------------------------------------
		.code						; Code segment definition
		.startup					; Entrance of this program
;------------------------------------------------------------------------
		Patch_Protues					; Simulation Patch for Proteus,
								; Please ignore the above code line.
;------------------------------------------------------------------------

START:								; Modify the following codes accordingly
								; 
		CLI						; Disable interrupts
		MOV AX, @DATA					;
		MOV DS, AX					; Initialize DS

		CALL INIT8255					; Initialize 8255 
		CALL INIT8253					; Initialize 8253

		MOV  BL, IRQNum					; BL is used as a parameter to call the procedure INT_INIT
		CALL INT_INIT			; Procedure INT_INIT is used to set up the IVT
;===================================================================================
; Attention:
; The following codes is a Patching for Proteus 8086 Hardware Simulation Bug.
; Use these codes in the case you want the 8086 to halt and waiting for HW INT only! 
; You can treat it as if it doesn't exist. 
;
; If you need to use HW INT, please uncomment it, or
; Don't modify it, leave it here just as it is.
;		WaitForHWInt IRQNum				
;====================================================================================

		HLT						; 
;=====================================================================================


;--------------------------------------------
;                                           |
; INIT 8255 					            |
;                                           |
;--------------------------------------------
INIT8255 PROC

		; Init 8255 in Mode 0,	L8255PA OPUT, L8255PB OPUT, L8255PCU OPUT, L8255PCL IPUT
		;PUSH CX,AX,BX,DX
		MOV DX,L8255CS
		MOV AL,10000001B
		OUT DX,AL
		RET
INIT8255 ENDP

;--------------------------------------------
;                                           |
; INIT 8253 					            |
;                                           |
;--------------------------------------------
INIT8253 PROC
		MOV DX,L8253CS
		MOV AL,00110110B; counter0 in mode3
		OUT DX,AL
		MOV DX,L8253T0
		MOV AL,50H; write low bit of counter
		OUT DX,AL
		MOV AL,0C3H
		OUT DX,AL
; Set the mode and the initial count for Timer0
	       MOV DX,L8253CS
		MOV AL,01110111B; counter1 in mode3
		OUT DX,AL
		MOV DX,L8253T1
		MOV AL,20; write low bit of counter
		OUT DX,AL
		MOV AL,00
		OUT DX,AL

; Set the mode and the initial count for Timer1

		MOV DX,L8253CS
		MOV AL,10110000B; counter2 in mode0;timer2 用mode1有问题一直闪，timer2的gate改接PC7，ISR里注释交换
		OUT DX,AL
		MOV DX,L8253T2
		MOV AL,50H; write low bit of counter
		OUT DX,AL
		MOV AL,0C3H
		OUT DX,AL
; Set the mode and the initial count for Timer2


		RET
INIT8253 ENDP


;--------------------------------------------------------------
;                                                             |                                                            |
; Function：DELAY FUNCTION                                    | 
; Input：None												  |
; Output: None                                                |
;--------------------------------------------------------------

DELAY 	PROC
    	PUSH CX
    	MOV CX, DelayLong
D1: 	LOOP D1
    	POP CX
    	RET
DELAY 	ENDP

;-------------------------------------------------------------
;                                                             |                                                            |
; Function：INTERRUPT Vector Table INIT						  |
; Input: BL = Interrupt number								  |
; Output: None			                                	  |
;                                                             |
;-------------------------------------------------------------	
INT_INIT	PROC FAR			; The code is not complete and you should finalize the procedure
		CLI				; Disable interrupt
		MOV AX, 0
		MOV ES, AX			; To set up the interrupt vector table
; Put your code here
; Hint: you can use the directives such as SEGMENT,OFFSET to get the segment value and the offset of a label
		MOV DX,OFFSET MYIRQ
		MOV AL,4
		IMUL BL
		MOV BH,0
		MOV BL,AL;乘积在AL中
		MOV ES:[BX],DX
		MOV DX,SEG MYIRQ
		ADD BL,2
		MOV ES:[BX],DX
		STI
		RET				; Do not to forget to return back from a procedure		
INT_INIT	ENDP

		
;--------------------------------------------------------------
;                                                             |                                                            |
; FUNCTION: INTERRUPT SERVICE  Routine （ISR）				  | 
; Input::                                                     |
; Output:                                                     |
;                                                             |
;--------------------------------------------------------------	
		
MYIRQ 	PROC FAR				; The code is not complete and you should finalize the procedure
; Put your code here
	       PUSH AX
	       PUSH DX
	       CLI;
	       ;MOV DX,L8255PC;timer2用mode1的方案但是不可行
	       ;CALL DELAY
	       ;MOV AL,01111111B
	       ;OUT DX,AL
	       ;MOV AL,10000000B;PC7调整重新计数，重置gate以使得timer2重新计数
	       ;OUT DX,AL
	       ;MOV AL,01111111B
	       ;OUT DX,AL
	       
	       MOV DX,L8253CS
	       MOV AL,10110000B; counter2 in mode0;timer2 用mode1有问题一直闪，timer2的gate改接PC7，ISR里注释交换
	       OUT DX,AL
	       MOV DX,L8253T2
	       MOV AL,50H; write low bit of counter
	       OUT DX,AL
	       MOV AL,0C3H
	       OUT DX,AL
	       
	       POP DX
	       POP AX
	       STI
	       IRET	     ; Do not forget to return back from a ISR
MYIRQ 	ENDP

	END					
