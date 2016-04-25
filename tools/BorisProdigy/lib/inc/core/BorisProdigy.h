#ifndef INC_BORISPRODIGY_H_
#define INC_BORISPRODIGY_H_

#include <common.h>

/**
 * @brief Az kérelmek várakozási sorának mérete.
 */
#define REQUEST_QUEUE_SIZE 20

/**
 * A beérkezett kérelmek típusainak felsorolása.
 */
enum RequestType {
  REQUEST_GET_DEVICE_TYPE,
  REQUEST_GET_INPUT,
  REQUEST_SET_OUTPUT
};

/**
 * @brief A beérkezett kérelem struktúrája.
 */
typedef struct request_entry_t {
  /**
   * A beérkezett kérelem típusa.
   */
  enum RequestType type;
  
  /**
   * A beérkezett kérelem elsõ adatbájtja.
   */
  uint8_t dataHigh;
  
  /**
   * A beérkezett kérelem második adatbájtja.
   */
  uint8_t dataLow;
} RequestEntry;

/**
 * @brief A beérkezett kérelmek várakozási sorának struktúrája.
 */
typedef struct request_queue_t {
  /**
   * A várakozási sor utolsó elemének sorszáma.
   */
  uint8_t end;

  /**
   * A várakozási sor elsõ elemének sorszáma.
   */
  uint8_t begin;

  /**
   * A beérkezett kérelmeket tároló tömb.
   */
  RequestEntry requests[REQUEST_QUEUE_SIZE];
} RequestQueue;

/**
 * @brief Inicializálja a mikrovezérlõ I/O portjait, a perifériáit és az
 * alkalmazást a hozzá kapcsolódó összes modullal együtt.
 */
void BorisProdigy_initialize();

/**
 * @brief Eltárolja a megadott kérelmet a kérelmek várakozási sorának soron
 * következõ rekeszébe.
 *
 * @param type A kérelem típusa.
 * @param data_h A kérelem elsõ adatbájtja.
 * @param data_l A kérelem második adatbájtja.
 */
void BorisProdigy_setRequest(enum RequestType type, uint8_t data_h, uint8_t data_l);

/**
 * @brief Feldolgozza a soron következõ kérelmet a kérelmek várakozási sorából,
 * amennyiben a sor nem üres.
 */
void BorisProdigy_handleRequests();

/**
 * @brief Engedélyezi a globális megszakításokat és végtelen ciklusban
 * végrehajtja a beékezõ kérelmeket.
 */
void BorisProdigy_run();

#endif /* INC_BORISPRODIGY_H_ */