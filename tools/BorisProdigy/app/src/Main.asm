
_interrupt:

;Main.c,11 :: 		void interrupt() {
;Main.c,14 :: 		if(RC1IF_bit && RC1IE_bit)
	BTFSS       RC1IF_bit+0, BitPos(RC1IF_bit+0) 
	GOTO        L_interrupt2
	BTFSS       RC1IE_bit+0, BitPos(RC1IE_bit+0) 
	GOTO        L_interrupt2
L__interrupt3:
;Main.c,15 :: 		Serial_handleInterrupt();
	CALL        _Serial_handleInterrupt+0, 0
L_interrupt2:
;Main.c,16 :: 		}
L_end_interrupt:
L__interrupt5:
	RETFIE      1
; end of _interrupt

_main:

;Main.c,21 :: 		void main() {
;Main.c,23 :: 		BorisProdigy_initialize();
	CALL        _BorisProdigy_initialize+0, 0
;Main.c,24 :: 		BorisProdigy_run();
	CALL        _BorisProdigy_run+0, 0
;Main.c,25 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
