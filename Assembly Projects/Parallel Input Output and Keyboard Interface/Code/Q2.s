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
	
GPIO_PORTB_DATA 	EQU 	0x400053FC
PB_INP 				EQU 	0x4000503C
PB_OUT 				EQU 	0x400053C0
GPIO_PORTB_DIR_R 	EQU 	0x40005400
GPIO_PORTB_AFSEL_R 	EQU 	0x40005420
GPIO_PORTB_DEN_R 	EQU 	0x4000551C
GPIO_PORTB_PDR 		EQU 	0x40005514
SYSCTL_RCGCGPIO_R 	EQU 	0x400FE608
LEDONE       		EQU		0xE0
LEDTWO				EQU		0xD0
LEDTHREE			EQU		0xB0
LEDFOUR				EQU		0x70
DELAYFORTY          EQU     0x28
	

;***************************************************************
; Directives - This Data Section is part of the code
; It is in the read only section  so values cannot be changed.
;***************************************************************
;LABEL      DIRECTIVE   VALUE       COMMENT
            AREA        sdata, DATA, READONLY
            THUMB
CTR1        DCB         0x0B
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
			LDR 		R1,=GPIO_PORTB_PDR
			MOV 		R0, #0x00 ;pull down on pins 0-3 of PORT B
			STR 		R0, [R1]
			; enable digital port
			LDR 		R1,=GPIO_PORTB_DEN_R ; R1 = &GPIO_PORTB_DEN_R
			LDR 		R0, [R1]
			ORR 		R0, R0, #0xFF ; enable digital I/O on PB
			STR 		R0, [R1]
			BX 			LR  ;branch back to main 


__main      PROC 
	        
			
			
			BL 			PortB_Init ; initialize Port B
			
			LDR 		R1,=PB_OUT ; R1 = 0x400053C0 
            LDR         R2,=0xFF
			STR 		R2, [R1] ; display number of switch
			; to turn off all LEDs in the beginning
loop0 		MOV         R6,#0     ;set for debounce
loop1		LDR 		R1,=PB_INP ; R1 = 0x4000503C
			;polling for a key press
loop_c 		
			LDR 		R0, [R1] ; R0 = [R1] (read pb0-pb3)
			AND 		R2,R0,#0x0F ; check if any switch is pressed
			ADD         R6,R6,#1 ;add to r6 to differentiate between number of reads
			CMP         R2,#0x0F 
			BEQ         loop_c ; if no button is pressed then take reading again
			BL			Delay  ;delay 100ms for debounce
			CMP    		R6,#1   ;check if first cycle
			BNE         debounce ;if not then move to debounce for comparison
			MOV         R7,R2 ; save this reading for comparison later
			BEQ         loop1 ; if first cycle, take another reading without resetting r6
debounce	CMP         R7,R2 ;compare previous reading 
			BNE         loop0 ;take another reading now if they didnt match
			
			;CBZ 		R2,loop_c;find the pressed key
			

display 	LDR 		R1,=PB_OUT ; R1 = 0x400053C0
            ;LDR         R2,=LEDTHREE ;for debugg purpose
			LSL         R2,#4; left shift by four to adjst according to pin masking 
			STR 		R2, [R1] ; display number of switch
			;delay by four seconds
			LDR			R9,=DELAYFORTY  ;start delay of 40 times 100ms which is 4 seconds
forty		BL			Delay
			SUB			R9,R9,#1
			CMP         R9,#0
			BNE         forty
			B 			loop0
       
			ENDP
Delay		PROC
           LDR			R10,=Boardval
Again      
		   NOP
		   SUBS			R10,R10,#1
		   BNE          Again
		   BX			LR

		   ENDP
	

;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
            ALIGN
            END
