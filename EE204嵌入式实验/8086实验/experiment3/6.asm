
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

		CALL INIT8255
		CALL INIT8253
		MOV  BL, IRQNum					; BL is used as a parameter to call the procedure INT_INIT
		CALL INT_INIT			; Procedure INT_INIT is used to set up the IVT
		MOV AX,0
LOOP_DIV:	;很烦的是除法和IO都会选用AX和DX
		MOV AX,CX;将数移进来
		MOV DX,0000H;被除数32位中的高16位
		MOV BX,1000H;除数
		DIV BX
		;AX;商-1110
		;DX;余数
		PUSH DX;暂存
		PUSH AX
		MOV AL,00001110B;控制1号数字管
		MOV DX,L8255PA
		OUT DX,AL
		POP AX
		ADD AX,OFFSET SEGTAB
		MOV BX,AX;赋给BX
		MOV AL,DS:[BX];取DATA SEGMENT中的数据
		MOV DX,L8255PB
		OUT DX,AL
		POP DX
		
		CALL DELAY
		
		
		MOV AX,DX;将余数继续作为被除数
		MOV DX,0000H;被除数32位中的高16位
		MOV BX,100H;除数
		DIV BX
		;AX;商-1101
		;DX;余数
		PUSH DX
		PUSH AX
		MOV AL,00001101B;控制2号数字管
		MOV DX,L8255PA
		OUT DX,AL
		POP AX
		ADD AX,OFFSET SEGTAB
		MOV BX,AX;赋给BX
		MOV AL,DS:[BX];取DATA SEGMENT中的数据
		MOV DX,L8255PB
		OUT DX,AL
		POP DX
		
		CALL DELAY
		
		MOV AX,DX
		MOV BL,10H
		DIV BL
		;AL;商-1011
		;AH;余数-0111
		PUSH AX
		MOV AL,00001011B;控制3号数字管
		MOV DX,L8255PA
		OUT DX,AL
		POP AX
		PUSH AX
		MOV AH,0
		ADD AX,OFFSET SEGTAB
		MOV BX,AX;赋给BX
		MOV AL,DS:[BX];取DATA SEGMENT中的数据
		MOV DX,L8255PB
		OUT DX,AL
		POP AX
		
		CALL DELAY
		
		MOV AL,00000111B;控制4号数字管
		MOV DX,L8255PA
		OUT DX,AL
		MOV AL,AH
		MOV AH,0
		ADD AX,OFFSET SEGTAB
		MOV BX,AX;赋给BX
		MOV AL,DS:[BX];取DATA SEGMENT中的数据
		MOV DX,L8255PB
		OUT DX,AL
		
		  
		CALL DELAY
		JMP LOOP_DIV
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

INIT8253 PROC
		MOV DX,L8253CS
		MOV AL,00110111B; counter0 in mode3
		OUT DX,AL
		MOV DX,L8253T0
		MOV AL,00; write low bit of counter
		OUT DX,AL
		MOV AL,01
		OUT DX,AL
; Set the mode and the initial count for Timer0
	       MOV DX,L8253CS
		MOV AL,01110111B; counter1 in mode3
		OUT DX,AL
		MOV DX,L8253T1
		MOV AL,00; write low bit of counter
		OUT DX,AL
		MOV AL,10
		OUT DX,AL

; Set the mode and the initial count for Timer1

; Set the mode and the initial count for Timer2


		RET
INIT8253 ENDP



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

DELAY 	PROC
    	PUSH CX
    	MOV CX, DelayShort
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
	       CLI
	       INC CX
	       STI
END_ISR:       IRET					; Do not forget to return back from a ISR
MYIRQ 	ENDP
	       

	END					
