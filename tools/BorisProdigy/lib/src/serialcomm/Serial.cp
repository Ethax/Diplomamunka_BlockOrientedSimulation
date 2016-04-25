#line 1 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
#line 1 "c:/projects/borisprodigy/lib/inc/serialcomm/serial.h"
#line 1 "c:/projects/borisprodigy/lib/inc/common.h"
#line 1 "c:/program files (x86)/mikroelektronika/mikroc pro for pic/include/built_in.h"
#line 1 "c:/program files (x86)/mikroelektronika/mikroc pro for pic/include/stdint.h"




typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int int32_t;


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long int uint32_t;


typedef signed char int_least8_t;
typedef signed int int_least16_t;
typedef signed long int int_least32_t;


typedef unsigned char uint_least8_t;
typedef unsigned int uint_least16_t;
typedef unsigned long int uint_least32_t;



typedef signed char int_fast8_t;
typedef signed int int_fast16_t;
typedef signed long int int_fast32_t;


typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned long int uint_fast32_t;


typedef signed int intptr_t;
typedef unsigned int uintptr_t;


typedef signed long int intmax_t;
typedef unsigned long int uintmax_t;
#line 1 "c:/program files (x86)/mikroelektronika/mikroc pro for pic/include/stdbool.h"



 typedef char _Bool;
#line 1 "c:/program files (x86)/mikroelektronika/mikroc pro for pic/include/stdarg.h"





typedef void * va_list[1];
#line 9 "c:/projects/borisprodigy/lib/inc/serialcomm/serial.h"
enum BaudRate {
 BAUD_1200, BAUD_2400, BAUD_4800, BAUD_9600, BAUD_19200, BAUD_38400,
 BAUD_57600, BAUD_115200
};
#line 21 "c:/projects/borisprodigy/lib/inc/serialcomm/serial.h"
void Serial_initialize(enum BaudRate baudrate);
#line 30 "c:/projects/borisprodigy/lib/inc/serialcomm/serial.h"
void Serial_handleInterrupt();
#line 38 "c:/projects/borisprodigy/lib/inc/serialcomm/serial.h"
void Serial_sendString(char* str);
#line 46 "c:/projects/borisprodigy/lib/inc/serialcomm/serial.h"
void Serial_sendData(uint8_t* data_ptr, uint8_t size);
#line 1 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
#line 1 "c:/projects/borisprodigy/lib/inc/common.h"
#line 14 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
enum RequestType {
 REQUEST_GET_DEVICE_TYPE,
 REQUEST_GET_INPUT,
 REQUEST_SET_OUTPUT
};
#line 23 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
typedef struct request_entry_t {
#line 27 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
 enum RequestType type;
#line 32 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
 uint8_t dataHigh;
#line 37 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
 uint8_t dataLow;
} RequestEntry;
#line 43 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
typedef struct request_queue_t {
#line 47 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
 uint8_t end;
#line 52 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
 uint8_t begin;
#line 57 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
 RequestEntry requests[ 20 ];
} RequestQueue;
#line 64 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
void BorisProdigy_initialize();
#line 74 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
void BorisProdigy_setRequest(enum RequestType type, uint8_t data_h, uint8_t data_l);
#line 80 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
void BorisProdigy_handleRequests();
#line 86 "c:/projects/borisprodigy/lib/inc/core/borisprodigy.h"
void BorisProdigy_run();
#line 7 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
uint8_t receiveBuffer[10] = { 0 };
#line 13 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
uint8_t transmitBuffer[20] = { 0 };
#line 18 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
uint8_t receiveCounter = 0;
#line 25 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
void Serial_initialize(enum BaudRate baudrate) {
#line 28 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
 switch(baudrate) {
 case BAUD_1200:
 UART1_Init(1200); break;
 case BAUD_2400:
 UART1_Init(2400); break;
 case BAUD_4800:
 UART1_Init(4800); break;
 case BAUD_9600:
 UART1_Init(9600); break;
 case BAUD_19200:
 UART1_Init(19200); break;
 case BAUD_38400:
 UART1_Init(38400); break;
 case BAUD_57600:
 UART1_Init(57600); break;
 case BAUD_115200:
 UART1_Init(115200); break;
 }


 RC1IF_bit =  0 ;
 RC1IE_bit =  1 ;
}
#line 56 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
void Serial_handleInterrupt() {

 uint8_t next_byte = UART1_Read();
#line 62 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
 switch(next_byte) {
 case 0x02:
 BorisProdigy_setRequest(REQUEST_GET_DEVICE_TYPE, 0, 0);
 break;
 case 0xb9:
 BorisProdigy_setRequest(REQUEST_GET_INPUT, 0, 0);
 break;
 case 0xba:
 receiveCounter = 2;
 break;
 }
#line 76 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
 if(receiveCounter && next_byte != 0xba) {
 receiveBuffer[2 - receiveCounter] = next_byte;
 if(--receiveCounter == 0)
 BorisProdigy_setRequest(REQUEST_SET_OUTPUT, receiveBuffer[0], receiveBuffer[1]);
 }


 RC1IF_bit =  0 ;
}
#line 89 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
void Serial_sendString(char* str) {
 Serial_sendData(str, strlen(str));
}
#line 96 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
void Serial_sendData(uint8_t* data_ptr, uint8_t size) {
 uint8_t i;
#line 101 "C:/Projects/BorisProdigy/lib/src/serialcomm/Serial.c"
 memcpy(transmitBuffer, data_ptr, size);
 for(i = 0; i < size; i++)
 UART1_Write(transmitBuffer[i]);
}
