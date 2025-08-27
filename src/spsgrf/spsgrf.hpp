 // =================
  //  File Name: spsgrf.h
  //  Author: Jack Holdsworth
  //  ID: 20023933
  //  Create Time: 2024-01-26 13:12:26
  //  Modified time: 2024-04-16 15:32:12
  //  Description: spsgrf setup and function commands
 // =================
 
#pragma once
#include <Arduino.h>
#include <SPI.h>

// command codes for spsgrf radio registers. provided by SPIRIT1 DK
#define COMMAND_TX                                  ((uint8_t)(0x60)) /*!< Start to transmit; valid only from READY */
#define COMMAND_RX                                  ((uint8_t)(0x61)) /*!< Start to receive; valid only from READY */
#define COMMAND_READY                               ((uint8_t)(0x62)) /*!< Go to READY; valid only from STANDBY or SLEEP or LOCK */
#define COMMAND_STANDBY                             ((uint8_t)(0x63)) /*!< Go to STANDBY; valid only from READY */
#define COMMAND_SLEEP                               ((uint8_t)(0x64)) /*!< Go to SLEEP; valid only from READY */
#define COMMAND_LOCKRX                              ((uint8_t)(0x65)) /*!< Go to LOCK state by using the RX configuration of the synth; valid only from READY */
#define COMMAND_LOCKTX                              ((uint8_t)(0x66)) /*!< Go to LOCK state by using the TX configuration of the synth; valid only from READY */
#define COMMAND_SABORT                              ((uint8_t)(0x67)) /*!< Force exit form TX or RX states and go to READY state; valid only from TX or RX */
#define COMMAND_SRES                                ((uint8_t)(0x70)) /*!< Reset of all digital part, except SPI registers */
#define COMMAND_FLUSHRXFIFO                         ((uint8_t)(0x71)) /*!< Clean the RX FIFO; valid from all states */
#define COMMAND_FLUSHTXFIFO                         ((uint8_t)(0x72)) /*!< Clean the TX FIFO; valid from all states */

// arduino pins to spsgrf pins
#define SPSGRF_SCK     36   
#define SPSGRF_MISO    37
#define SPSGRF_MOSI    35
#define SPSGRF_CS      8 
#define SPSGRF_SDN     14
#define SPSGRF_GPIO0   15

class Spirit {
private:
    uint8_t rxBuffer [12];
    char rssiChars [12];
    uint8_t rawRssi = 0;
    float signalStrength = 0;
    float number0 = 0;
    float number1 = 1;
    char number0Chars [12];
    char number1Chars [12];
    
public:
    Spirit();
    void SpiritSetup();
    uint8_t* SpiritGet();
    void SpiritSpiCommandStrobes(uint8_t cmd_code);
    void SpiritSpiReadRegisters(uint8_t address, uint8_t n_regs, uint8_t* buffer);
    void SpiritSpiWriteRegisters(uint8_t address, uint8_t n_regs, uint8_t* buffer);
    uint8_t spiritLastRSSI();
    uint8_t SpiritRead(uint8_t n_regs, uint8_t* buffer);
    void SpiritPutInRX();
    void SpiritSend(uint8_t n_regs, uint8_t* buffer);
    void SpiritVcoCalibration();
    void SpiritBaseConfiguration();
};