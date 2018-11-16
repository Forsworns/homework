;--------------------------------------------------------------------------
;
;              Build this with the "Source" menu using
;                     "Build All" option
;
;--------------------------------------------------------------------------
;
;                           ʵ��������ͨ�ÿ��
;
;--------------------------------------------------------------------------
; ���ܣ� ������					                            				   |
; ��д����Ƕ��ʽϵͳԭ����ʵ�顷�γ���                   				   |
; �汾��3.5
; �޶���B
;--------------------------------------------------------------------------


		DOSSEG
		.MODEL	SMALL		; �趨8086������ʹ��Small model
		.8086				; �趨����8086���ָ�

;-----------------------------------------------------------
;	�����ջ��                                             |
;-----------------------------------------------------------
	.stack 100h				; ����256�ֽ������Ķ�ջ

;-------------------------------------------------------------------------
;	���Ŷ���                                                              |
;-------------------------------------------------------------------------
;
;
; 8253оƬ�˿ڵ�ַ ��Port Address):
L8253T0			EQU	0100H			; Timer0's port number in I/O space
L8253T1			EQU 	0102H			; Timer1's port number in I/O space
L8253T2			EQU 	0104H			; Timer2's port number in I/O space
L8253CS			EQU 	0106H			; 8253 Control Register's port number in I/O space
;
; 8255оƬ�˿ڵ�ַ ��Port Address):
L8255PA			EQU	0121H			; Port A's port number in I/O space
L8255PB			EQU 	0123H			; Port B's port number in I/O space
L8255PC			EQU 	0125H			; Port C's port number in I/O space
L8255CS			EQU 	0127H			; 8255 Control Register's port number in I/O space
;
;  �ж�ʸ���Ŷ���
IRQNum			EQU	41			; �ж�ʸ����,Ҫ����ѧ�ż���õ�����´˶��塣

Patch_Protues	EQU	IN AL, 0	;	Simulation Patch for Proteus, please ignore this line

;=======================================================================
; �궨��
;=======================================================================

; �޲�Proteus�����BUG���μ�������е�ʹ��˵��
    WaitForHWInt MACRO INTNum		; INTNum is the HW INT number
		MOV AL, INTNum   			;
		OUT 0,AL					;
		STI
		CLI
    ENDM



;-----------------------------------------------------------
;	�������ݶ�                                             |
;-----------------------------------------------------------
		.data					; �������ݶ�;

DelayShort	dw	40				; ����ʱ����	
DelayLong	dw	4000			; ����ʱ����


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
;	��������                                             |
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
		;CALL INT_INIT			; Procedure INT_INIT is used to set up the IVT
		
		MOV CL,200;
Transfer_Again: CALL PC0TOPC6
		LOOP Transfer_Again
		
		MOV CL,200;
Display_Again:
		CALL DISPLAY8255				; Procedure DISPLAY8255 is used to contrl 7-segment tubes
		LOOP Display_Again


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
		JMP	Display_Again

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
		MOV AL,00110111B; counter0 in mode3
		OUT DX,AL
		MOV DX,L8253T0
		MOV AL,00; write low bit of counter
		OUT DX,AL
		MOV AL,10
		OUT DX,AL
; Set the mode and the initial count for Timer0


; Set the mode and the initial count for Timer1


; Set the mode and the initial count for Timer2


		RET
INIT8253 ENDP



PC0TOPC6 PROC
	       ;ԭ����(AH6 and c)or c���Ϊcֵ��ע������λ���ܶ�
	       MOV DX,L8255PC
	       IN AL,DX
	       MOV AH,AL;�ݴ�
	       AND AL,00000001B;�뵽C0λֵ
	       MOV CL,6
	       SHL AL,CL;��C6  0c000000
	       OR AL,10111111B; 1c111111
	       AND AH,AL
	       AND AL,01000000B; 0c000000
	       OR AH,AL
	       MOV AL,AH
	       OUT DX,AL
	       RET
PC0TOPC6 ENDP
;--------------------------------------------
;                                           |
; DISPLAY  STUDENTS ID				 		|
;                                           |
;--------------------------------------------

DISPLAY8255 PROC
; Put your code here
		MOV AL,0EH
		MOV DX,L8255PA
		OUT DX,AL
		MOV AL,SEGTAB
		MOV DX,L8255PB
		OUT DX,AL
		CALL DELAY
		MOV AL,0DH
		MOV DX,L8255PA
		OUT DX,AL
		MOV AL,SEGTAB+2
		MOV DX,L8255PB
		OUT DX,AL
		CALL DELAY
		MOV AL,0BH
		MOV DX,L8255PA
		OUT DX,AL
		MOV AL,SEGTAB+3
		MOV DX,L8255PB
		OUT DX,AL	
		CALL DELAY
		MOV AL,07H
		MOV DX,L8255PA
		OUT DX,AL
		MOV AL,SEGTAB+3
		MOV DX,L8255PB
		OUT DX,AL
		CALL DELAY
		RET
DISPLAY8255 ENDP


;--------------------------------------------------------------
;                                                             |                                                            |
; Function��DELAY FUNCTION                                    | 
; Input��None												  |
; Output: None                                                |
;--------------------------------------------------------------

DELAY 	PROC
    	PUSH CX
    	MOV CX, DelayShort
D1: 	LOOP D1
    	POP CX
    	RET
DELAY 	ENDP

;-------------------------------------------------------------
;                                                             |                                                            |
; Function��INTERRUPT Vector Table INIT						  |
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


						; Do not to forget to return back from a procedure		
INT_INIT	ENDP

		
;--------------------------------------------------------------
;                                                             |                                                            |
; FUNCTION: INTERRUPT SERVICE  Routine ��ISR��				  | 
; Input::                                                     |
; Output:                                                     |
;                                                             |
;--------------------------------------------------------------	
		
MYIRQ 	PROC FAR				; The code is not complete and you should finalize the procedure
; Put your code here

						; Do not forget to return back from a ISR
MYIRQ 	ENDP

	END					