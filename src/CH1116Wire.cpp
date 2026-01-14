#include "CH1116Wire.h"

void CH1116Wire::display() {
  // CH1116/SH1106: scrittura per pagina (8 pagine da 8 righe),
  // con indirizzo di pagina e colonna di partenza (xOffset) prima dei 128 byte.
  const uint8_t pages = height() / 8; // 64 -> 8 pagine
  const uint16_t w    = width();      // 128

  for (uint8_t p = 0; p < pages; p++) {
    // Seleziona pagina (0xB0..0xB7)
    sendCommand(0xB0 + p);

    // Imposta colonna di partenza (low e high nibble)
    // Tipico offset per 1.54" I²C con CH1116/SH1106: 2 colonne
    sendCommand(0x00 + (_xOffset & 0x0F));           // Low column start
    sendCommand(0x10 + ((_xOffset >> 4) & 0x0F));    // High column (in genere 0)

    // Scrivi esattamente 128 byte dal buffer per questa pagina
    const uint8_t* pagePtr = buffer + (p * w);
    sendData(pagePtr, w);
  }
}