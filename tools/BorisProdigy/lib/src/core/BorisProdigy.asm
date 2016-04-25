
_BorisProdigy_initialize:

;BorisProdigy.c,18 :: 		void BorisProdigy_initialize() {
;BorisProdigy.c,20 :: 		ADCON0 = ADCON1 = ADCON2 = 0x00;
	CLRF        ADCON2+0 
	MOVF        ADCON2+0, 0 
	MOVWF       ADCON1+0 
	MOVF        ADCON1+0, 0 
	MOVWF       ADCON0+0 
;BorisProdigy.c,21 :: 		CM1CON0 = CM2CON0 = VREFCON0 = VREFCON1 = VREFCON2 =  0x00;
	CLRF        VREFCON2+0 
	MOVF        VREFCON2+0, 0 
	MOVWF       VREFCON1+0 
	MOVF        VREFCON1+0, 0 
	MOVWF       VREFCON0+0 
	MOVF        VREFCON0+0, 0 
	MOVWF       CM2CON0+0 
	MOVF        CM2CON0+0, 0 
	MOVWF       CM1CON0+0 
;BorisProdigy.c,25 :: 		ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = 0x00;
	CLRF        ANSELE+0 
	MOVF        ANSELE+0, 0 
	MOVWF       ANSELD+0 
	MOVF        ANSELD+0, 0 
	MOVWF       ANSELC+0 
	MOVF        ANSELC+0, 0 
	MOVWF       ANSELB+0 
	MOVF        ANSELB+0, 0 
	MOVWF       ANSELA+0 
;BorisProdigy.c,26 :: 		SLRCON = LATA = LATB = LATC = LATD = LATE = 0x00;
	CLRF        LATE+0 
	MOVF        LATE+0, 0 
	MOVWF       LATD+0 
	MOVF        LATD+0, 0 
	MOVWF       LATC+0 
	MOVF        LATC+0, 0 
	MOVWF       LATB+0 
	MOVF        LATB+0, 0 
	MOVWF       LATA+0 
	MOVF        LATA+0, 0 
	MOVWF       SLRCON+0 
;BorisProdigy.c,27 :: 		TRISA = TRISB = TRISC = TRISD = TRISE = 0xFF;
	MOVLW       255
	MOVWF       TRISE+0 
	MOVF        TRISE+0, 0 
	MOVWF       TRISD+0 
	MOVF        TRISD+0, 0 
	MOVWF       TRISC+0 
	MOVF        TRISC+0, 0 
	MOVWF       TRISB+0 
	MOVF        TRISB+0, 0 
	MOVWF       TRISA+0 
;BorisProdigy.c,31 :: 		TRISB = TRISD = 0x00;
	CLRF        TRISD+0 
	MOVF        TRISD+0, 0 
	MOVWF       TRISB+0 
;BorisProdigy.c,34 :: 		requestQueue.begin = requestQueue.end = 0;
	CLRF        _requestQueue+0 
	MOVF        _requestQueue+0, 0 
	MOVWF       _requestQueue+1 
;BorisProdigy.c,35 :: 		Serial_initialize(BAUD_9600);
	MOVLW       3
	MOVWF       FARG_Serial_initialize_baudrate+0 
	CALL        _Serial_initialize+0, 0
;BorisProdigy.c,36 :: 		}
L_end_BorisProdigy_initialize:
	RETURN      0
; end of _BorisProdigy_initialize

_BorisProdigy_setRequest:

;BorisProdigy.c,42 :: 		void BorisProdigy_setRequest(enum RequestType type, uint8_t data_h, uint8_t data_l) {
;BorisProdigy.c,45 :: 		requestQueue.requests[requestQueue.end].type = type;
	MOVLW       3
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        _requestQueue+0, 0 
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVLW       _requestQueue+2
	ADDWF       R0, 0 
	MOVWF       FSR1 
	MOVLW       hi_addr(_requestQueue+2)
	ADDWFC      R1, 0 
	MOVWF       FSR1H 
	MOVF        FARG_BorisProdigy_setRequest_type+0, 0 
	MOVWF       POSTINC1+0 
;BorisProdigy.c,46 :: 		requestQueue.requests[requestQueue.end].dataHigh = data_h;
	MOVLW       3
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        _requestQueue+0, 0 
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVLW       _requestQueue+2
	ADDWF       R0, 1 
	MOVLW       hi_addr(_requestQueue+2)
	ADDWFC      R1, 1 
	MOVLW       1
	ADDWF       R0, 0 
	MOVWF       FSR1 
	MOVLW       0
	ADDWFC      R1, 0 
	MOVWF       FSR1H 
	MOVF        FARG_BorisProdigy_setRequest_data_h+0, 0 
	MOVWF       POSTINC1+0 
;BorisProdigy.c,47 :: 		requestQueue.requests[requestQueue.end].dataLow = data_l;
	MOVLW       3
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        _requestQueue+0, 0 
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVLW       _requestQueue+2
	ADDWF       R0, 1 
	MOVLW       hi_addr(_requestQueue+2)
	ADDWFC      R1, 1 
	MOVLW       2
	ADDWF       R0, 0 
	MOVWF       FSR1 
	MOVLW       0
	ADDWFC      R1, 0 
	MOVWF       FSR1H 
	MOVF        FARG_BorisProdigy_setRequest_data_l+0, 0 
	MOVWF       POSTINC1+0 
;BorisProdigy.c,50 :: 		requestQueue.end = (requestQueue.end + 1) % REQUEST_QUEUE_SIZE;
	MOVF        _requestQueue+0, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	ADDWFC      R1, 1 
	MOVLW       20
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Div_16x16_S+0, 0
	MOVF        R8, 0 
	MOVWF       R0 
	MOVF        R9, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	MOVWF       _requestQueue+0 
;BorisProdigy.c,55 :: 		if(requestQueue.end == requestQueue.begin)
	MOVF        _requestQueue+0, 0 
	XORWF       _requestQueue+1, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L_BorisProdigy_setRequest0
;BorisProdigy.c,56 :: 		requestQueue.begin = (requestQueue.begin + 1) % REQUEST_QUEUE_SIZE;
	MOVF        _requestQueue+1, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	ADDWFC      R1, 1 
	MOVLW       20
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Div_16x16_S+0, 0
	MOVF        R8, 0 
	MOVWF       R0 
	MOVF        R9, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	MOVWF       _requestQueue+1 
L_BorisProdigy_setRequest0:
;BorisProdigy.c,57 :: 		}
L_end_BorisProdigy_setRequest:
	RETURN      0
; end of _BorisProdigy_setRequest

_BorisProdigy_handleRequests:

;BorisProdigy.c,63 :: 		void BorisProdigy_handleRequests() {
;BorisProdigy.c,65 :: 		uint8_t port_data[2] = { 0 };
	CLRF        BorisProdigy_handleRequests_port_data_L0+0 
	CLRF        BorisProdigy_handleRequests_port_data_L0+1 
;BorisProdigy.c,68 :: 		if(requestQueue.begin == requestQueue.end) return;
	MOVF        _requestQueue+1, 0 
	XORWF       _requestQueue+0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L_BorisProdigy_handleRequests1
	GOTO        L_end_BorisProdigy_handleRequests
L_BorisProdigy_handleRequests1:
;BorisProdigy.c,71 :: 		switch(requestQueue.requests[requestQueue.begin].type) {
	MOVLW       3
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        _requestQueue+1, 0 
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVLW       _requestQueue+2
	ADDWF       R0, 1 
	MOVLW       hi_addr(_requestQueue+2)
	ADDWFC      R1, 1 
	MOVF        R0, 0 
	MOVWF       FLOC__BorisProdigy_handleRequests+0 
	MOVF        R1, 0 
	MOVWF       FLOC__BorisProdigy_handleRequests+1 
	GOTO        L_BorisProdigy_handleRequests2
;BorisProdigy.c,72 :: 		case REQUEST_GET_DEVICE_TYPE:
L_BorisProdigy_handleRequests4:
;BorisProdigy.c,73 :: 		Serial_sendData(serialNumber, 14);
	MOVLW       _serialNumber+0
	MOVWF       FARG_Serial_sendData_data_ptr+0 
	MOVLW       hi_addr(_serialNumber+0)
	MOVWF       FARG_Serial_sendData_data_ptr+1 
	MOVLW       14
	MOVWF       FARG_Serial_sendData_size+0 
	CALL        _Serial_sendData+0, 0
;BorisProdigy.c,74 :: 		break;
	GOTO        L_BorisProdigy_handleRequests3
;BorisProdigy.c,75 :: 		case REQUEST_GET_INPUT:
L_BorisProdigy_handleRequests5:
;BorisProdigy.c,76 :: 		port_data[0] = PORTA;
	MOVF        PORTA+0, 0 
	MOVWF       BorisProdigy_handleRequests_port_data_L0+0 
;BorisProdigy.c,77 :: 		port_data[1] = 0 | (PORTC & 0x0F) | ((PORTE & 0x0F) << 4);
	MOVLW       15
	ANDWF       PORTC+0, 0 
	MOVWF       BorisProdigy_handleRequests_port_data_L0+1 
	MOVLW       15
	ANDWF       PORTE+0, 0 
	MOVWF       R3 
	MOVLW       4
	MOVWF       R2 
	MOVF        R3, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R2, 0 
L__BorisProdigy_handleRequests12:
	BZ          L__BorisProdigy_handleRequests13
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	ADDLW       255
	GOTO        L__BorisProdigy_handleRequests12
L__BorisProdigy_handleRequests13:
	MOVF        R0, 0 
	IORWF       BorisProdigy_handleRequests_port_data_L0+1, 1 
;BorisProdigy.c,78 :: 		Serial_sendData(port_data, 2);
	MOVLW       BorisProdigy_handleRequests_port_data_L0+0
	MOVWF       FARG_Serial_sendData_data_ptr+0 
	MOVLW       hi_addr(BorisProdigy_handleRequests_port_data_L0+0)
	MOVWF       FARG_Serial_sendData_data_ptr+1 
	MOVLW       2
	MOVWF       FARG_Serial_sendData_size+0 
	CALL        _Serial_sendData+0, 0
;BorisProdigy.c,79 :: 		break;
	GOTO        L_BorisProdigy_handleRequests3
;BorisProdigy.c,80 :: 		case REQUEST_SET_OUTPUT:
L_BorisProdigy_handleRequests6:
;BorisProdigy.c,81 :: 		LATB = requestQueue.requests[requestQueue.begin].dataHigh;
	MOVLW       3
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        _requestQueue+1, 0 
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVLW       _requestQueue+2
	ADDWF       R0, 1 
	MOVLW       hi_addr(_requestQueue+2)
	ADDWFC      R1, 1 
	MOVLW       1
	ADDWF       R0, 0 
	MOVWF       FSR0 
	MOVLW       0
	ADDWFC      R1, 0 
	MOVWF       FSR0H 
	MOVF        POSTINC0+0, 0 
	MOVWF       LATB+0 
;BorisProdigy.c,82 :: 		LATD = requestQueue.requests[requestQueue.begin].dataLow;
	MOVLW       3
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        _requestQueue+1, 0 
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Mul_16X16_U+0, 0
	MOVLW       _requestQueue+2
	ADDWF       R0, 1 
	MOVLW       hi_addr(_requestQueue+2)
	ADDWFC      R1, 1 
	MOVLW       2
	ADDWF       R0, 0 
	MOVWF       FSR0 
	MOVLW       0
	ADDWFC      R1, 0 
	MOVWF       FSR0H 
	MOVF        POSTINC0+0, 0 
	MOVWF       LATD+0 
;BorisProdigy.c,83 :: 		break;
	GOTO        L_BorisProdigy_handleRequests3
;BorisProdigy.c,84 :: 		}
L_BorisProdigy_handleRequests2:
	MOVFF       FLOC__BorisProdigy_handleRequests+0, FSR0
	MOVFF       FLOC__BorisProdigy_handleRequests+1, FSR0H
	MOVF        POSTINC0+0, 0 
	XORLW       0
	BTFSC       STATUS+0, 2 
	GOTO        L_BorisProdigy_handleRequests4
	MOVFF       FLOC__BorisProdigy_handleRequests+0, FSR0
	MOVFF       FLOC__BorisProdigy_handleRequests+1, FSR0H
	MOVF        POSTINC0+0, 0 
	XORLW       1
	BTFSC       STATUS+0, 2 
	GOTO        L_BorisProdigy_handleRequests5
	MOVFF       FLOC__BorisProdigy_handleRequests+0, FSR0
	MOVFF       FLOC__BorisProdigy_handleRequests+1, FSR0H
	MOVF        POSTINC0+0, 0 
	XORLW       2
	BTFSC       STATUS+0, 2 
	GOTO        L_BorisProdigy_handleRequests6
L_BorisProdigy_handleRequests3:
;BorisProdigy.c,87 :: 		requestQueue.begin = (requestQueue.begin + 1) % REQUEST_QUEUE_SIZE;
	MOVF        _requestQueue+1, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	ADDWFC      R1, 1 
	MOVLW       20
	MOVWF       R4 
	MOVLW       0
	MOVWF       R5 
	CALL        _Div_16x16_S+0, 0
	MOVF        R8, 0 
	MOVWF       R0 
	MOVF        R9, 0 
	MOVWF       R1 
	MOVF        R0, 0 
	MOVWF       _requestQueue+1 
;BorisProdigy.c,88 :: 		}
L_end_BorisProdigy_handleRequests:
	RETURN      0
; end of _BorisProdigy_handleRequests

_BorisProdigy_run:

;BorisProdigy.c,94 :: 		void BorisProdigy_run() {
;BorisProdigy.c,97 :: 		IPEN_bit = false;
	BCF         IPEN_bit+0, BitPos(IPEN_bit+0) 
;BorisProdigy.c,98 :: 		PEIE_bit = GIE_bit = true;
	BSF         GIE_bit+0, BitPos(GIE_bit+0) 
	BTFSC       GIE_bit+0, BitPos(GIE_bit+0) 
	GOTO        L__BorisProdigy_run15
	BCF         PEIE_bit+0, BitPos(PEIE_bit+0) 
	GOTO        L__BorisProdigy_run16
L__BorisProdigy_run15:
	BSF         PEIE_bit+0, BitPos(PEIE_bit+0) 
L__BorisProdigy_run16:
;BorisProdigy.c,102 :: 		requestQueue.begin = requestQueue.end = 0;
	CLRF        _requestQueue+0 
	MOVF        _requestQueue+0, 0 
	MOVWF       _requestQueue+1 
;BorisProdigy.c,103 :: 		while(true)
L_BorisProdigy_run7:
;BorisProdigy.c,104 :: 		BorisProdigy_handleRequests();
	CALL        _BorisProdigy_handleRequests+0, 0
	GOTO        L_BorisProdigy_run7
;BorisProdigy.c,105 :: 		}
L_end_BorisProdigy_run:
	RETURN      0
; end of _BorisProdigy_run
