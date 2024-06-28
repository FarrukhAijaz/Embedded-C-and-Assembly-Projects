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
Simulationval	EQU		0x3A980
Boardval		EQU		0x4E200
	

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


__main      PROC 
			LDR			R0,=Boardval
			BL Delay
Loop        B Loop
       
			ENDP
Delay
Again      PROC
		   NOP
		   SUBS			R0,R0,#1
		   BNE          Again
		   BX			LR

		   ENDP
	

;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
            ALIGN
            END
