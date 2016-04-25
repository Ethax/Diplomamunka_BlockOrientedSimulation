#include <serialcomm/Serial.h>
#include <core/BorisProdigy.h>

/*
 * A beérkezõ adatokat tároló fogadó puffer deklarálása és inicializálása.
 */
uint8_t receiveBuffer[10] = { 0 };

/*
 * Az elküldésre szánt adatokat tároló kimenõ puffer deklarálása és
 * inicializálása.
 */
uint8_t transmitBuffer[20] = { 0 };

/*
 * A beérkezõ adatbájtokat számláló változó deklarálása és inicializálása.
 */
uint8_t receiveCounter = 0;

/*
 * Inicializálja a soros kommunikációs portot és beállíja az adatátviteli
 * sebességet a megadott értéknek megfelelõen, majd engedélyezi a megszakítást
 * az adatbájtok érkezésére.
 */
void Serial_initialize(enum BaudRate baudrate) {
  /* A soros kommunikációs port inicializálása és a megadott adatátviteli
  sebesség beállítása. */
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

  /* A soros portra érkezett adatbájtokkal járó megszakítások engedélyezése. */
  RC1IF_bit = false;
  RC1IE_bit = true;
}

/*
 * Kezeli a soros portra érkezett adatbájttal járó megszakítást és törli a
 * megszakítás jelzõbitjét.
 */
void Serial_handleInterrupt() {
  /* A fogadott bájt kiolvasása a soros port pufferébõl. */
  uint8_t next_byte = UART1_Read();

  /* A fogadott bájtnak megfelelõ kérelem belehelyezése a kérelmek várakozási
  sorába. */
  switch(next_byte) {
  case 0x02: /* Get Device Type Request */
    BorisProdigy_setRequest(REQUEST_GET_DEVICE_TYPE, 0, 0);
    break;
  case 0xb9: /* Get Input Request */
    BorisProdigy_setRequest(REQUEST_GET_INPUT, 0, 0);
    break;
  case 0xba: /* Set Output Request */
    receiveCounter = 2;
    break;
  }
  
  /* A kimenetet beállító új paraméterek fogadása, amennyiben egy korábbi
  utasítás az új paraméterek érkezését jelezte. */
  if(receiveCounter && next_byte != 0xba) {
    receiveBuffer[2 - receiveCounter] = next_byte;
    if(--receiveCounter == 0)
      BorisProdigy_setRequest(REQUEST_SET_OUTPUT, receiveBuffer[0], receiveBuffer[1]);
  }

  /* A lekezelt megszakítás jelzöbitjének törlése. */
  RC1IF_bit = false;
}

/*
 * Kiküldi az átadott karakterláncot a soros kommunikációs porton keresztül.
 */
void Serial_sendString(char* str) {
  Serial_sendData(str, strlen(str));
}

/*
 * Kiküldi az átadott adattömböt a soros kommunikációs porton keresztül.
 */
void Serial_sendData(uint8_t* data_ptr, uint8_t size) {
  uint8_t i;
  
  /* A mutatott memóriaterület átmásolása a kimenõ pufferbe és a kimenõ puffer 
  tartalmának kiküldése a soros kommunikációs porton keresztül. */
  memcpy(transmitBuffer, data_ptr, size);
  for(i = 0; i < size; i++)
    UART1_Write(transmitBuffer[i]);
}