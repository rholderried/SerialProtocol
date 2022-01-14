#ifndef _SERIALPROTOCOL_H_
#define _SERIALPROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>
#include "Commands.h"

#define TRANSMIT_BUFFER_LENGTH  64
// #define NUMBER_OF_CONTROL_BYTES 2
#define TXRX_BUFFER_LENGTH      TRANSMIT_BUFFER_LENGTH

#define STX 0x02
#define ETX 0x03

#define GETVAR_IDENTIFIER   '?'
#define SETVAR_IDENTIFIER   '!'
#define COMMAND_IDENTIFIER  ':'

#define DEBUG_FUNCTIONS

typedef bool(*TX_CB)(uint8_t*, uint8_t);
typedef void(*DBG_FCN_CB)(void);

typedef enum
{
    ePROTOCOL_IDLE,
    ePROTOCOL_RECEIVING,
    ePROTOCOL_EVALUATING,
    ePROTOCOL_SENDING,
    ePROTOCOL_DEBUG
}PROTOCOL_STATE;

typedef enum
{
    eDEBUG_ACTIVATION_NONE,
    eDEBUG_ACTIVATION_S1,
    eDEBUG_ACTIVATION_S2,
    eDEBUG_ACTIVATION_FINAL
}DEBUG_ACTIVATION_STATE;

#define PROTOCOL_STATE_DEFAULT ePROTOCOL_IDLE

class Buffer
{
    
    private:

    uint8_t     *pui8_bufPtr;
    int16_t     i16_bufIdx;
    uint8_t     ui8_bufLen;
    uint8_t     ui8_bufSpace;
    bool        b_ovfl;

    public:

    Buffer(uint8_t bufLen, uint8_t *buf);
    void putElem(uint8_t data);
    uint8_t readBuf (uint8_t **pui8_target);
    void flushBuf (void);
    bool getNextFreeBufSpace(uint8_t **pui8_target);
    bool increaseBufIdx(uint8_t ui8_idx);
    int16_t getActualIdx(void);
};

class SerialProtocol
{
    public:

    struct {
        PROTOCOL_STATE  e_state;
        bool            b_error;
        bool            b_sent;

        #ifdef DEBUG_FUNCTIONS
        DEBUG_ACTIVATION_STATE e_dbgActState;
        DBG_FCN_CB  debugFcnArray[10] = {nullptr};
        #endif
    } control;

    uint8_t txRxBuffer[TXRX_BUFFER_LENGTH] = {0};


    Buffer rxBuffer = Buffer(TXRX_BUFFER_LENGTH, this->txRxBuffer);
    Buffer txBuffer = Buffer(TXRX_BUFFER_LENGTH, this->txRxBuffer);

    TX_CB       txCallback = nullptr;
    

    //Serial_ serialDevice;

    // Public methods
    

    SerialProtocol();
    void setupCallbacks(TX_CB transmit_cb, READEEPROM_CB readEEPROM_cb, WRITEEEPROM_CB writeEEPROM_cb);
    void statemachine   (void);
    void sendBuffer     (void);
    void receive        (uint8_t data);

    private:
    SerialCommands cmdModule = SerialCommands();
    COMMAND commandParser(uint8_t *pui8_buf, uint8_t ui8_stringSize);
    uint8_t responseBuilder(uint8_t *pui8_buf, RESPONSE response);

};

#endif //_SERIALPROTOCOL_H_