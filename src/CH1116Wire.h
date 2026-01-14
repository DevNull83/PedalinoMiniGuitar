#pragma once
#include "OLEDDisplay.h"
#include <Wire.h>

class CH1116Wire : public OLEDDisplay {
public:
  CH1116Wire(uint8_t address, int sda, int scl, int rst = -1,
             OLEDDISPLAY_GEOMETRY g = GEOMETRY_128_64)
  : OLEDDisplay(), _address(address), _sda(sda), _scl(scl), _rst(rst) {
    // Imposta geometria 128x64 (buffer, width/height)
    setGeometry(g);
  }

  // Offset colonne (tipicamente 2 per molti 1.54" I²C; prova 2→3→4)
  void setXOffset(uint8_t off) { _xOffset = off & 0x0F; }

  // Connessione I²C al display
  bool connect() override {
    // ESP32/ESP8266 permettono Wire.begin(SDA,SCL); AVR usa pin fissi A4/A5.
    #if defined(ARDUINO_ARCH_ESP32) || defined(ESP32) || defined(ARDUINO_ARCH_ESP8266)
      Wire.begin(_sda, _scl);
      Wire.setClock(400000); // 400 kHz (Fast Mode)
    #else
      (void)_sda; (void)_scl;
      Wire.begin();
    #endif

    if (_rst >= 0) {
      pinMode(_rst, OUTPUT);
      digitalWrite(_rst, LOW);
      delay(10);
      digitalWrite(_rst, HIGH);
      delay(10);
    }
    return true;
  }

  // Invia un comando (I²C control byte = 0x00)
  void sendCommand(uint8_t cmd) override {
    Wire.beginTransmission(_address);
    Wire.write(0x00);      // command
    Wire.write(cmd);
    Wire.endTransmission();
  }

  // Flush del buffer: implementazione CH1116/SH1106 (per pagina)
  void display() override;

protected:
  // Nessun header extra sul buffer per I²C
  int getBufferOffset(void) override { return 0; }

private:
  uint8_t _address;
  int     _sda, _scl, _rst;
  uint8_t _xOffset = 2; // default: 2 colonne (molti moduli 1.54" usano 2)

  inline void sendData(const uint8_t* data, size_t len) {
    // Spezzetta in chunk piccoli per il buffer di Wire (16 bytes è prudente)
    size_t i = 0;
    while (i < len) {
      Wire.beginTransmission(_address);
      Wire.write(0x40);   // data
      size_t chunk = (len - i > 32) ? 32 : (len - i);
      Wire.write(data + i, chunk);
      Wire.endTransmission();
      i += chunk;
    }
  }
};