#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include <common.h>

/**
 * A beállítható adatátviteli sebességek felsorolása.
 */
enum BaudRate {
  BAUD_1200, BAUD_2400, BAUD_4800, BAUD_9600, BAUD_19200, BAUD_38400,
  BAUD_57600, BAUD_115200
};

/**
 * @breif Inicializálja a soros kommunikációs portot és beállíja az adatátviteli
 * sebességet a megadott értéknek megfelelõen, majd engedélyezi a megszakítást
 * az adatbájtok érkezésére.
 *
 * @param baudrate A beállítani kívánt adatátviteli sebesség.
 */
void Serial_initialize(enum BaudRate baudrate);

/**
 * @brief Kezeli a soros portra érkezett adatbájttal járó megszakítást és törli
 * a megszakítás jelzõbitjét.
 *
 * Annak érdekében, hogy a megszakítás le legyen kezelve annak bekövetkezésekor,
 * ennek a függvénynek megszakításokat kiszolgáló rutinban kell lennie.
 */
void Serial_handleInterrupt();

/**
 * @brief Kiküldi az átadott karakterláncot a soros kommunikációs porton
 * keresztül.
 *
 * @param str A kiküldésre szánt karakterlánc elsõ elemére mutató pointer.
 */
void Serial_sendString(char* str);

/**
 * @brief Kiküldi az átadott adattömböt a soros kommunikációs porton keresztül.
 *
 * @param str A kiküldésre szánt adattömb elsõ elemére mutató pointer.
 * @param size Az adattömb mérete.
 */
void Serial_sendData(uint8_t* data_ptr, uint8_t size);

#endif /* INC_SERIAL_H_ */