
_Serial_initialize:

;Serial.c,25 :: 		void Serial_initialize(enum BaudRate baudrate) {
;Serial.c,28 :: 		switch(baudrate) {
	GOTO        L_Serial_initialize0
;Serial.c,29 :: 		case BAUD_1200:
L_Serial_initialize2:
;Serial.c,30 :: 		UART1_Init(1200); break;
	BSF         BAUDCON+0, 3, 0
	MOVLW       6
	MOVWF       SPBRGH+0 
	MOVLW       130
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
	GOTO        L_Serial_initialize1
;Serial.c,31 :: 		case BAUD_2400:
L_Serial_initialize3:
;Serial.c,32 :: 		UART1_Init(2400); break;
	BSF         BAUDCON+0, 3, 0
	MOVLW       3
	MOVWF       SPBRGH+0 
	MOVLW       64
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
	GOTO        L_Serial_initialize1
;Serial.c,33 :: 		case BAUD_4800:
L_Serial_initialize4:
;Serial.c,34 :: 		UART1_Init(4800); break;
	BSF         BAUDCON+0, 3, 0
	MOVLW       1
	MOVWF       SPBRGH+0 
	MOVLW       160
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
	GOTO        L_Serial_initialize1
;Serial.c,35 :: 		case BAUD_9600:
L_Serial_initialize5:
;Serial.c,36 :: 		UART1_Init(9600); break;
	BSF         BAUDCON+0, 3, 0
	CLRF        SPBRGH+0 
	MOVLW       207
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
	GOTO        L_Serial_initialize1
;Serial.c,37 :: 		case BAUD_19200:
L_Serial_initialize6:
;Serial.c,38 :: 		UART1_Init(19200); break;
	BSF         BAUDCON+0, 3, 0
	CLRF        SPBRGH+0 
	MOVLW       103
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
	GOTO        L_Serial_initialize1
;Serial.c,39 :: 		case BAUD_38400:
L_Serial_initialize7:
;Serial.c,40 :: 		UART1_Init(38400); break;
	BSF         BAUDCON+0, 3, 0
	CLRF        SPBRGH+0 
	MOVLW       51
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
	GOTO        L_Serial_initialize1
;Serial.c,41 :: 		case BAUD_57600:
L_Serial_initialize8:
;Serial.c,42 :: 		UART1_Init(57600); break;
	BSF         BAUDCON+0, 3, 0
	CLRF        SPBRGH+0 
	MOVLW       34
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
	GOTO        L_Serial_initialize1
;Serial.c,43 :: 		case BAUD_115200:
L_Serial_initialize9:
;Serial.c,44 :: 		UART1_Init(115200); break;
	BSF         BAUDCON+0, 3, 0
	CLRF        SPBRGH+0 
	MOVLW       16
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
	GOTO        L_Serial_initialize1
;Serial.c,45 :: 		}
L_Serial_initialize0:
	MOVF        FARG_Serial_initialize_baudrate+0, 0 
	XORLW       0
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_initialize2
	MOVF        FARG_Serial_initialize_baudrate+0, 0 
	XORLW       1
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_initialize3
	MOVF        FARG_Serial_initialize_baudrate+0, 0 
	XORLW       2
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_initialize4
	MOVF        FARG_Serial_initialize_baudrate+0, 0 
	XORLW       3
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_initialize5
	MOVF        FARG_Serial_initialize_baudrate+0, 0 
	XORLW       4
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_initialize6
	MOVF        FARG_Serial_initialize_baudrate+0, 0 
	XORLW       5
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_initialize7
	MOVF        FARG_Serial_initialize_baudrate+0, 0 
	XORLW       6
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_initialize8
	MOVF        FARG_Serial_initialize_baudrate+0, 0 
	XORLW       7
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_initialize9
L_Serial_initialize1:
;Serial.c,48 :: 		RC1IF_bit = false;
	BCF         RC1IF_bit+0, BitPos(RC1IF_bit+0) 
;Serial.c,49 :: 		RC1IE_bit = true;
	BSF         RC1IE_bit+0, BitPos(RC1IE_bit+0) 
;Serial.c,50 :: 		}
L_end_Serial_initialize:
	RETURN      0
; end of _Serial_initialize

_Serial_handleInterrupt:

;Serial.c,56 :: 		void Serial_handleInterrupt() {
;Serial.c,58 :: 		uint8_t next_byte = UART1_Read();
	CALL        _UART1_Read+0, 0
	MOVF        R0, 0 
	MOVWF       Serial_handleInterrupt_next_byte_L0+0 
;Serial.c,62 :: 		switch(next_byte) {
	GOTO        L_Serial_handleInterrupt10
;Serial.c,63 :: 		case 0x02: /* Get Device Type Request */
L_Serial_handleInterrupt12:
;Serial.c,64 :: 		BorisProdigy_setRequest(REQUEST_GET_DEVICE_TYPE, 0, 0);
	CLRF        FARG_BorisProdigy_setRequest_type+0 
	CLRF        FARG_BorisProdigy_setRequest_data_h+0 
	CLRF        FARG_BorisProdigy_setRequest_data_l+0 
	CALL        _BorisProdigy_setRequest+0, 0
;Serial.c,65 :: 		break;
	GOTO        L_Serial_handleInterrupt11
;Serial.c,66 :: 		case 0xb9: /* Get Input Request */
L_Serial_handleInterrupt13:
;Serial.c,67 :: 		BorisProdigy_setRequest(REQUEST_GET_INPUT, 0, 0);
	MOVLW       1
	MOVWF       FARG_BorisProdigy_setRequest_type+0 
	CLRF        FARG_BorisProdigy_setRequest_data_h+0 
	CLRF        FARG_BorisProdigy_setRequest_data_l+0 
	CALL        _BorisProdigy_setRequest+0, 0
;Serial.c,68 :: 		break;
	GOTO        L_Serial_handleInterrupt11
;Serial.c,69 :: 		case 0xba: /* Set Output Request */
L_Serial_handleInterrupt14:
;Serial.c,70 :: 		receiveCounter = 2;
	MOVLW       2
	MOVWF       _receiveCounter+0 
;Serial.c,71 :: 		break;
	GOTO        L_Serial_handleInterrupt11
;Serial.c,72 :: 		}
L_Serial_handleInterrupt10:
	MOVF        Serial_handleInterrupt_next_byte_L0+0, 0 
	XORLW       2
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_handleInterrupt12
	MOVF        Serial_handleInterrupt_next_byte_L0+0, 0 
	XORLW       185
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_handleInterrupt13
	MOVF        Serial_handleInterrupt_next_byte_L0+0, 0 
	XORLW       186
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_handleInterrupt14
L_Serial_handleInterrupt11:
;Serial.c,76 :: 		if(receiveCounter && next_byte != 0xba) {
	MOVF        _receiveCounter+0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_handleInterrupt17
	MOVF        Serial_handleInterrupt_next_byte_L0+0, 0 
	XORLW       186
	BTFSC       STATUS+0, 2 
	GOTO        L_Serial_handleInterrupt17
L__Serial_handleInterrupt22:
;Serial.c,77 :: 		receiveBuffer[2 - receiveCounter] = next_byte;
	MOVF        _receiveCounter+0, 0 
	SUBLW       2
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	SUBWFB      R1, 1 
	MOVLW       _receiveBuffer+0
	ADDWF       R0, 0 
	MOVWF       FSR1 
	MOVLW       hi_addr(_receiveBuffer+0)
	ADDWFC      R1, 0 
	MOVWF       FSR1H 
	MOVF        Serial_handleInterrupt_next_byte_L0+0, 0 
	MOVWF       POSTINC1+0 
;Serial.c,78 :: 		if(--receiveCounter == 0)
	DECF        _receiveCounter+0, 1 
	MOVF        _receiveCounter+0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_Serial_handleInterrupt18
;Serial.c,79 :: 		BorisProdigy_setRequest(REQUEST_SET_OUTPUT, receiveBuffer[0], receiveBuffer[1]);
	MOVLW       2
	MOVWF       FARG_BorisProdigy_setRequest_type+0 
	MOVF        _receiveBuffer+0, 0 
	MOVWF       FARG_BorisProdigy_setRequest_data_h+0 
	MOVF        _receiveBuffer+1, 0 
	MOVWF       FARG_BorisProdigy_setRequest_data_l+0 
	CALL        _BorisProdigy_setRequest+0, 0
L_Serial_handleInterrupt18:
;Serial.c,80 :: 		}
L_Serial_handleInterrupt17:
;Serial.c,83 :: 		RC1IF_bit = false;
	BCF         RC1IF_bit+0, BitPos(RC1IF_bit+0) 
;Serial.c,84 :: 		}
L_end_Serial_handleInterrupt:
	RETURN      0
; end of _Serial_handleInterrupt

_Serial_sendString:

;Serial.c,89 :: 		void Serial_sendString(char* str) {
;Serial.c,90 :: 		Serial_sendData(str, strlen(str));
	MOVF        FARG_Serial_sendString_str+0, 0 
	MOVWF       FARG_strlen_s+0 
	MOVF        FARG_Serial_sendString_str+1, 0 
	MOVWF       FARG_strlen_s+1 
	CALL        _strlen+0, 0
	MOVF        R0, 0 
	MOVWF       FARG_Serial_sendData_size+0 
	MOVF        FARG_Serial_sendString_str+0, 0 
	MOVWF       FARG_Serial_sendData_data_ptr+0 
	MOVF        FARG_Serial_sendString_str+1, 0 
	MOVWF       FARG_Serial_sendData_data_ptr+1 
	CALL        _Serial_sendData+0, 0
;Serial.c,91 :: 		}
L_end_Serial_sendString:
	RETURN      0
; end of _Serial_sendString

_Serial_sendData:

;Serial.c,96 :: 		void Serial_sendData(uint8_t* data_ptr, uint8_t size) {
;Serial.c,101 :: 		memcpy(transmitBuffer, data_ptr, size);
	MOVLW       _transmitBuffer+0
	MOVWF       FARG_memcpy_d1+0 
	MOVLW       hi_addr(_transmitBuffer+0)
	MOVWF       FARG_memcpy_d1+1 
	MOVF        FARG_Serial_sendData_data_ptr+0, 0 
	MOVWF       FARG_memcpy_s1+0 
	MOVF        FARG_Serial_sendData_data_ptr+1, 0 
	MOVWF       FARG_memcpy_s1+1 
	MOVF        FARG_Serial_sendData_size+0, 0 
	MOVWF       FARG_memcpy_n+0 
	MOVLW       0
	MOVWF       FARG_memcpy_n+1 
	CALL        _memcpy+0, 0
;Serial.c,102 :: 		for(i = 0; i < size; i++)
	CLRF        Serial_sendData_i_L0+0 
L_Serial_sendData19:
	MOVF        FARG_Serial_sendData_size+0, 0 
	SUBWF       Serial_sendData_i_L0+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_Serial_sendData20
;Serial.c,103 :: 		UART1_Write(transmitBuffer[i]);
	MOVLW       _transmitBuffer+0
	MOVWF       FSR0 
	MOVLW       hi_addr(_transmitBuffer+0)
	MOVWF       FSR0H 
	MOVF        Serial_sendData_i_L0+0, 0 
	ADDWF       FSR0, 1 
	BTFSC       STATUS+0, 0 
	INCF        FSR0H, 1 
	MOVF        POSTINC0+0, 0 
	MOVWF       FARG_UART1_Write_data_+0 
	CALL        _UART1_Write+0, 0
;Serial.c,102 :: 		for(i = 0; i < size; i++)
	INCF        Serial_sendData_i_L0+0, 1 
;Serial.c,103 :: 		UART1_Write(transmitBuffer[i]);
	GOTO        L_Serial_sendData19
L_Serial_sendData20:
;Serial.c,104 :: 		}
L_end_Serial_sendData:
	RETURN      0
; end of _Serial_sendData
