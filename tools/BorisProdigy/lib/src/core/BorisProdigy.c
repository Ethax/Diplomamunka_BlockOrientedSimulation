#include <core/BorisProdigy.h>
#include <serialcomm/Serial.h>

/*
 * A kérelmek várakozási sorának deklarálása.
 */
volatile RequestQueue requestQueue;

/*
 * Az eszköz sorozatszámának deklarálása és inicializálása.
 */
uint8_t serialNumber[] = "SO3715-1H.110"; //{ 'S', 'O', '3', '7', '1', '5', '-', '1', 'H', '.', '1', '1', '0', '\0' };

/*
 * Inicializálja a mikrovezérlõ I/O portjait, a perifériáit és az  alkalmazást
 * a hozzá kapcsolódó összes modullal együtt.
 */
void BorisProdigy_initialize() {
  /* Az A/D és D/A átalakítók, illetve a komparátorok kikapcsolása. */
  ADCON0 = ADCON1 = ADCON2 = 0x00;
  CM1CON0 = CM2CON0 = VREFCON0 = VREFCON1 = VREFCON2 =  0x00;
  
  /* Minden I/O port standard jelváltozási sebességû digitális bemenetre
  konfigurálása, és a hozzájuk tartozó tárolók törlése. */
  ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = 0x00;
  SLRCON = LATA = LATB = LATC = LATD = LATE = 0x00;
  TRISA = TRISB = TRISC = TRISD = TRISE = 0xFF;

  /* A B és C portok digitális kimenetre konfigurálása a LED-ek
  mûködtetéséhez. */
  TRISB = TRISD = 0x00;

  /* Az alkalmazás moduljainak inicializálása. */
  requestQueue.begin = requestQueue.end = 0;
  Serial_initialize(BAUD_9600);
}

/*
 * Eltárolja a megadott kérelmet a kérelmek várakozási sorának soron következõ
 * rekeszébe.
 */
void BorisProdigy_setRequest(enum RequestType type, uint8_t data_h, uint8_t data_l) {
  /* A várakozási sor következõ elemének felülírása a megadott kérelem
  paraméterekkel. */
  requestQueue.requests[requestQueue.end].type = type;
  requestQueue.requests[requestQueue.end].dataHigh = data_h;
  requestQueue.requests[requestQueue.end].dataLow = data_l;
  
  /* A sor utolsó elemére mutató számláló növelése. */
  requestQueue.end = (requestQueue.end + 1) % REQUEST_QUEUE_SIZE;

  /* A várakozási sorban levõ következõ kérelem kijelölése, amennyiben a
  várakozási sorban túlcsordulás keletkezett. (A legrégebbi kérelem el fog
  veszni.) */
  if(requestQueue.end == requestQueue.begin)
    requestQueue.begin = (requestQueue.begin + 1) % REQUEST_QUEUE_SIZE;
}

/*
 * Feldolgozza a soron következõ kérelmet a kérelmek várakozási sorából,
 * amennyiben a sor nem üres.
 */
void BorisProdigy_handleRequests() {
  /* A bemenet beolvasása után annak a jelenlegi állapotait tároló tömb. */
  uint8_t port_data[2] = { 0 };
  
  /* Azonnali visszatérés a függvénybõl, ha a várakozási sor üres. */
  if(requestQueue.begin == requestQueue.end) return;

  /* A soron következõ kérelem feldolgozása. */
  switch(requestQueue.requests[requestQueue.begin].type) {
  case REQUEST_GET_DEVICE_TYPE:
    Serial_sendData(serialNumber, 14);
    break;
  case REQUEST_GET_INPUT:
    port_data[0] = PORTA;
    port_data[1] = 0 | (PORTC & 0x0F) | ((PORTE & 0x0F) << 4);
    Serial_sendData(port_data, 2);
    break;
  case REQUEST_SET_OUTPUT:
    LATB = requestQueue.requests[requestQueue.begin].dataHigh;
    LATD = requestQueue.requests[requestQueue.begin].dataLow;
    break;
  }

  /* A várakozási sorban levõ következõ kérelem kijelölése. */
  requestQueue.begin = (requestQueue.begin + 1) % REQUEST_QUEUE_SIZE;
}

/*
 * Engedélyezi a globális megszakításokat és végtelen ciklusban végrehajtja a
 * beékezõ kérelmeket.
 */
void BorisProdigy_run() {
  /* A megszakítások prioritáskezelésének tiltása, majd a periferiális és
  globális megszakítások engedélyezése. */
  IPEN_bit = false;
  PEIE_bit = GIE_bit = true;

  /* A beérkezett kérelmek folyamatos feldolgozása és a megfelelõ mûveletek
  végrehajtása. */
  requestQueue.begin = requestQueue.end = 0;
  while(true)
    BorisProdigy_handleRequests();
}