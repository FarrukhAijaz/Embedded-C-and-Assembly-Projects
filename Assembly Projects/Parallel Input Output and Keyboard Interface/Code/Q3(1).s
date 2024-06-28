;*************************************************************** 
; Program_Directives.s  
; Copies the table from one location
; to another memory location.           
; Directives and Addressing modes are   
; explained with this program.   
;***************************************************************    
;*************************************************************** 
; EQU Directives
; These directives do not allocate memory
;***************************************************************
;LABEL      DIRECTIVE   VALUE       COMMENT
Simulationval		EQU		0x3A980
Boardval			EQU		0x4E200
Boardhalf			EQU     0x13880
	
GPIO_PORTB_DATA 	EQU 	0x400053FC
PB_INP 				EQU 	0x4000503C
PB_OUT 				EQU 	0x400053C0
GPIO_PORTB_DIR_R 	EQU 	0x40005400
GPIO_PORTB_AFSEL_R 	EQU 	0x40005420
GPIO_PORTB_DEN_R 	EQU 	0x4000551C
GPIO_PORTB_PDR 		EQU 	0x40005514
GPIO_PORTB_PUR_R    EQU     0x40005510
SYSCTL_RCGCGPIO_R 	EQU 	0x400FE608


;***************************************************************
; Directives - This Data Section is part of the code
; It is in the read only section  so values cannot be changed.
;***************************************************************
;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        sdata, DATA, READONLY
            THUMB
CTR1        DCB         0x0B
MSGKEY1     DCB         "KEY1"
            DCB         0x0D
            DCB         0x04
MSGKEY2     DCB         "KEY2"
            DCB         0x0D
            DCB         0x04
MSGKEY3     DCB         "KEY3"
            DCB         0x0D
            DCB         0x04
MSGKEY4     DCB         "KEY4"
            DCB         0x0D
            DCB         0x04
MSGKEY5     DCB         "KEY5"
            DCB         0x0D
            DCB         0x04
MSGKEY6     DCB         "KEY6"
            DCB         0x0D
            DCB         0x04
MSGKEY7     DCB         "KEY7"
            DCB         0x0D
            DCB         0x04
MSGKEY8     DCB         "KEY8"
            DCB         0x0D
            DCB         0x04
MSGKEY9     DCB         "KEY9"
            DCB         0x0D
            DCB         0x04
MSGKEY10    DCB         "KEY10"
            DCB         0x0D
            DCB         0x04
MSGKEY11    DCB         "KEY11"
            DCB         0x0D
            DCB         0x04
MSGKEY12    DCB         "KEY12"
            DCB         0x0D
            DCB         0x04
MSGKEY13    DCB         "KEY13"
            DCB         0x0D
            DCB         0x04
MSGKEY14    DCB         "KEY14"
            DCB         0x0D
            DCB         0x04
MSGKEY15    DCB         "KEY15"
            DCB         0x0D
            DCB         0x04
MSGKEY16    DCB         "KEY16"
            DCB         0x0D
            DCB         0x04
*****************************
; Program section                         
;***************************************************************
;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        main, READONLY, CODE
            THUMB
            EXTERN      OutStr      ; Reference external subroutine 			
			EXTERN		InChar      ; Reference the external inchar subroutine
            EXPORT      __main      ; Make available

PortB_Init
			; activate clock
			LDR 		R1,= SYSCTL_RCGCGPIO_R ; R1 = &SYSCTL_RCGCGPIO_R
			LDR 		R0, [R1]
			ORR 		R0, R0, #0x2F ;turn on clock for all 5 ports
			STR 		R0, [R1] ; LSB corresponds to PortA
			NOP
			NOP

			; set direction register
			LDR 		R1,=GPIO_PORTB_DIR_R ; R1 = &GPIO_PORTB_DIR_R
			LDR 		R0, [R1]
			ORR 		R0, R0, #0xF0 ; make PB7-4 output
			BIC 		R0, R0, #0x0F ; make PB3-0 input
			STR 		R0, [R1]
			; regular port function
			LDR 		R1,=GPIO_PORTB_AFSEL_R ; R1 = &GPIO_PORTB_AFSEL_R
			LDR 		R0, [R1]
			BIC 		R0, R0, #0xFF ; disable alternate function	
			STR 		R0, [R1]
			; pull-down resistors on switch pins
			LDR 		R1,=GPIO_PORTB_PUR_R
			MOV 		R0, #0x0F ;pull UP on pins 0-3 of PORT B
			STR 		R0, [R1]
			; enable digital port
			LDR 		R1,=GPIO_PORTB_DEN_R ; R1 = &GPIO_PORTB_DEN_R
			LDR 		R0, [R1]
			ORR 		R0, R0, #0xFF ; enable digital I/O on PB
			STR 		R0, [R1]
			BX 			LR  ;branch back to main 


__main      PROC 
	        
			BL 			PortB_Init ; initialize Port B
			
			
	 
loopf		LDR 		R1,=PB_OUT ; R1 = 0x400053C0 
            LDR         R2,=0x70
			STR 		R2, [R1] ; display number of switch
			;only L1 is off
			;check input
			;output code
			BL          Input
			CMP         R2,#0x07
			BNE         next0
			LDR         R0,=MSGKEY16
			BL          OutStr
next0		CMP         R2,#0x0b
			BNE         next1
			LDR         R0,=MSGKEY15
			BL          OutStr
next1		CMP         R2,#0x0d
			BNE         next2
			LDR         R0,=MSGKEY14
			BL          OutStr			
next2		CMP         R2,#0x0e
			BNE         next3
			LDR         R0,=MSGKEY13
			BL          OutStr	
next3
			
			
			LDR 		R1,=PB_OUT ; R1 = 0x400053C0 
            LDR         R2,=0xB0
			STR 		R2, [R1] ; display number of switch
			;only L2 is off
			BL			Input
			;output code
			CMP         R2,#0x07
			BNE         next4
			LDR         R0,=MSGKEY12
			BL          OutStr
next4		CMP         R2,#0x0b
			BNE         next5
			LDR         R0,=MSGKEY11
			BL          OutStr
next5		CMP         R2,#0x0d
			BNE         next6
			LDR         R0,=MSGKEY10
			BL          OutStr			
next6		CMP         R2,#0x0e
			BNE         next7
			LDR         R0,=MSGKEY9
			BL          OutStr	
next7			



			

			LDR 		R1,=PB_OUT ; R1 = 0x400053C0 
            LDR         R2,=0xD0
			STR 		R2, [R1] ; display number of switch
			;only L3 is off
			BL			Input
			;output code
			CMP         R2,#0x07
			BNE         next8
			LDR         R0,=MSGKEY8
			BL          OutStr
next8		CMP         R2,#0x0b
			BNE         next9
			LDR         R0,=MSGKEY7
			BL          OutStr
next9		CMP         R2,#0x0d
			BNE         next10
			LDR         R0,=MSGKEY6
			BL          OutStr			
next10		CMP         R2,#0x0e
			BNE         next11
			LDR         R0,=MSGKEY5
			BL          OutStr	
next11

			

			LDR 		R1,=PB_OUT ; R1 = 0x400053C0 
            LDR         R2,=0xE0
			STR 		R2, [R1] ; display number of switch
			;only L4 is off
			BL			Input
						;output code
			CMP         R2,#0x07
			BNE         next12
			LDR         R0,=MSGKEY4
			BL          OutStr
next12		CMP         R2,#0x0b
			BNE         next13
			LDR         R0,=MSGKEY3
			BL          OutStr
next13		CMP         R2,#0x0d
			BNE         next14
			LDR         R0,=MSGKEY2
			BL          OutStr			
next14		CMP         R2,#0x0e
			BNE         next15
			LDR         R0,=MSGKEY1
			BL          OutStr	
next15
			B  			loopf

			



Delay		PROC
           
		   LDR			R10,=Boardhalf
Again      
		   NOP
		   SUBS			R10,R10,#1
		   BNE          Again
		   BX			LR
		   ENDP
			   
Input     	PROC
	        PUSH		{LR}
loop0 		MOV         R6,#0     ;set for debounce
loop1		LDR 		R1,=PB_INP ; R1 = 0x4000503C
			;polling for a key press
loop_c 		
			LDR 		R0, [R1] ; R0 = [R1] (read pb0-pb3)
			AND 		R2,R0,#0x0F ; check if any switch is pressed
			ADD         R6,R6,#1 ;add to r6 to differentiate between number of reads
			BL			Delay  ;delay 100ms for debounce
			CMP    		R6,#1   ;check if first cycle
			BNE         debounce ;if not then move to debounce for comparison
			MOV         R7,R2 ; save this reading for comparison later
			BEQ         loop1 ; if first cycle, take another reading without resetting r6
debounce	CMP         R7,R2 ;compare previous reading 
			BNE         loop0 ;take another reading now if they didnt match
release     LDR         R1,=PB_INP
			LDR 		R8, [R1] ; R0 = [R1] (read pb0-pb3)
			AND 		R8,R8,#0x0F ; check if any switch is pressed
			CMP         R8,#0x0F
			BNE         release
			POP			{LR}
			BX  		LR
			ENDP
			
	

;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
            ALIGN
            END
