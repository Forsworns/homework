
;-----------------------------------------------------------
;
;              Build this with the "Source" menu using
;                     "Build All" option
;
;-----------------------------------------------------------
;
;                           实验二示例程序 

;-----------------------------------------------------------
;                                                          |
;                                                          |
; 功能：控制7段数码管的显示                                |
; 编写：《嵌入式系统原理与实验》课程组                     |
;-----------------------------------------------------------
		DOSSEG
		.MODEL	SMALL		; 设定8086汇编程序使用Small model
		.8086				; 设定采用8086汇编指令集
;-----------------------------------------------------------
;	符号定义                                               |
;-----------------------------------------------------------
;
; 8255芯片端口地址 （Port number）分配:
PortA	EQU		90H			; Port A's port number in I/O space
PortB	EQU 	92H			; Port B's port number in I/O space
PortC	EQU 	94H			; Port C's port number in I/O space
CtrlPT	EQU 	96H			; 8255 Control Register's port number in I/O space
;
Patch_Protues	EQU		IN AL, 0	;	Simulation Patch for Proteus, please ignore this line


;-----------------------------------------------------------
;	定义数据段                                             |
;-----------------------------------------------------------
		.data					; 定义数据段;

DelayShort	dw	400   			; 短延时参量	
DelayLong	dw	40000			; 长延时参量

; 显示数字
DISCHAR DB 01,02,03,04

; SEGTAB是显示字符0-F，其中有部分数据的段码有错误，请自行修正
SEGTAB  	DB 3FH	; 7-Segment Tube, 共阴极类型的7段数码管示意图
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
		DB 71H	;这个写错了


;-----------------------------------------------------------
;	定义代码段                                             |
;-----------------------------------------------------------
		.code						; Code segment definition
		.startup					; 定义汇编程序执行入口点
;------------------------------------------------------------------------
		Patch_Protues				; Simulation Patch for Proteus,
									; Please ignore the above code line.
;------------------------------------------------------------------------


; Init 8255 in Mode 0
; PortA Output, PortB Output, PortC Input
;
		MOV AL,10001001B
		OUT CtrlPT,AL	;
;
; 把数字1、2、3、4显示在数码管上
;

L1:	        
		IN AL,PortC
		MOV DL,AL;暂存
		
		;控制灯，数字的1号，3号位置
		AND AL,0F0H;按位与先把低位置零
		OR AL,0AH;把低位修改成想要的1010
		OUT PortA,AL;输出到低位控制的位置和高位控制的灯上
	        
		;控制1、3号位上显示的具体数字——高位
		MOV AL,DL;恢复AL
		AND AX,	00F0H;将AL内的低位置零，AH整体置零，此时为相对SEFTAB偏移量
		MOV CL,4
		SHR AX,CL;把AL高位的数移位移到低位作为偏移量
		ADD AX,OFFSET SEGTAB;SEGTAB初始偏移地址与偏移量相加得新的偏移地址（不用SEG SEGTAB求出的段地址，段地址*16+偏移地址为物理地址
		MOV BX,AX;赋给BX
		MOV AL,DS:[BX];取DATA SEGMENT中的数据
		OUT PortB,AL

		CALL DELAY
		
		;控制灯，数字的2号，4号位置
		MOV AL,DL
		AND AL,0F0H;按位与先把低位置零
		OR AL,05H;把低位修改成想要的0101
		OUT PortA,AL;输出到低位控制的位置和高位控制的灯上
		
		;控制2、4号位上显示的具体数字——低位
		MOV AL,DL;恢复AL
		AND AX,	000FH;将AL内的高位置零，AH整体置零，此时为相对SEFTAB偏移量
		ADD AX,OFFSET SEGTAB;SEGTAB初始偏移地址与偏移量相加得新的偏移地址（不用SEG SEGTAB求出的段地址，段地址*16+偏移地址为物理地址
		MOV BX,AX;赋给BX
		MOV AL,DS:[BX];取DATA SEGMENT中的数据
		OUT PortB,AL

		CALL DELAY
		JMP L1
RET

;--------------------------------------------
;                                           |
; Delay system running for a while          |
; CX : contains time para.                  |
;                                           |
;--------------------------------------------

DELAY1 	PROC
    	PUSH CX
    	MOV CX,DelayLong	;
D0: 	LOOP D0
    	POP CX
    	RET
DELAY1 	ENDP


;--------------------------------------------
;                                           |
; Delay system running for a while          |
;                                           |
;--------------------------------------------

DELAY 	PROC
    	PUSH CX
    	MOV CX,DelayShort
D1: 	LOOP D1
    	POP CX
    	RET
DELAY 	ENDP


;-----------------------------------------------------------
;	定义堆栈段                                             |
;-----------------------------------------------------------
		.stack 100h				; 定义256字节容量的堆栈


		END						;指示汇编程序结束编译
