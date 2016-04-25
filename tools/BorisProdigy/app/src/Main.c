#include <common.h>
#include <core/BorisProdigy.h>
#include <serialcomm/Serial.h>

/**
 * @brief A magas prioritású megszakításokat kiszolgáló rutin.
 *
 * Amennyiben a prioritáskezelés nincs engedélyezve, minden megszakítás ide fut
 * be.
 */
void interrupt() {
  /* Az UART fogadó pufferébe érkezett adat hatására bekövetkezett megszakítás
  kezelése. */
  if(RC1IF_bit && RC1IE_bit)
    Serial_handleInterrupt();
}

/**
 * @brief A program futásának kezdõpontja.
 */
void main() {
  /* Az alkalmazás inicializálása és futtatása. */
  BorisProdigy_initialize();
  BorisProdigy_run();
}