#include "SerialProtocol.h"
#include <stdint.h>
#include "Commands.h"
#include <string.h>
#include <stdlib.h>
#include "Helpers.h"




/** \brief Buffer constructor
 *
 * @param size  Size of the externally provided buffer
 * @param *buf  Pointer to the first byte of the buffer
 */
Buffer::Buffer(uint8_t ui8_size, uint8_t *pui8_buf)
{
    pui8_bufPtr     = pui8_buf;
    i16_bufIdx      = -1;
    ui8_bufLen      = ui8_size;
    ui8_bufSpace    = ui8_size;
    b_ovfl          = false;
}

/** \brief Puts one byte into the buffer
 *
 * @param data Data byte
 */
void Buffer::putElem(uint8_t ui8_data)
{
    // Put the data into the buffer only when it is not going to be overflowed
    if (ui8_bufSpace > 0)
    {
        ui8_bufSpace--;
        i16_bufIdx++;
        pui8_bufPtr[i16_bufIdx] = ui8_data;
    }
    else
        b_ovfl = true;
}

/** \brief Buffer read operation
 *
 * This routine receives the address of a pointer variable, which gets moved
 * to the start of the buffer.
 * 
 * @param   **targetPtr Pointer address.
 * @returns Size of the stored data in bytes.
 */
uint8_t Buffer::readBuf(uint8_t **pui8_target)
{
    uint8_t size = i16_bufIdx + 1;

    *pui8_target = pui8_bufPtr;

    //flushBuf();

    return size;
}

void Buffer::flushBuf (void)
{
    i16_bufIdx      = -1;
    ui8_bufSpace    = ui8_bufLen;
    b_ovfl          = false;
} 

bool Buffer::getNextFreeBufSpace(uint8_t **pui8_target)
{
    bool success = false;

    if (ui8_bufSpace > 0)
    {
        *pui8_target = &pui8_bufPtr[i16_bufIdx + 1];
        success = true;
    }

    return success;
}

bool Buffer::increaseBufIdx(uint8_t ui8_size)
{
    bool success = false;

    if ((i16_bufIdx + ui8_size) < ui8_bufLen)
    {
        i16_bufIdx      += ui8_size;
        ui8_bufSpace    -= ui8_size;
        success         = true;
    }

    return success;

}

int16_t Buffer::getActualIdx(void)
{
    return i16_bufIdx;
}

/** \brief SerialProtocol c'tor
 *
 * @param transmitCallback  Callback for transmitting data
 */
SerialProtocol::SerialProtocol()
{
    control.e_state = PROTOCOL_STATE_DEFAULT;
    control.b_error = false;
    control.b_sent = false;
    #ifdef DEBUG_FUNCTIONS
    control.e_dbgActState = eDEBUG_ACTIVATION_NONE;
    #endif
}

void SerialProtocol::setupCallbacks(TX_CB transmit_cb, READEEPROM_CB readEEPROM_cb, WRITEEEPROM_CB writeEEPROM_cb)
{
    txCallback              = transmit_cb;
    cmdModule.readEEPROM    = readEEPROM_cb;
    cmdModule.writeEEPROM   = writeEEPROM_cb;
}

/*******************************************************************************************
 * SerialProtocol member definitions
*******************************************************************************************/
// SerialProtocol::cmdModule = SerialCommands();

/** \brief Receive callback routine
 *
 * Every incoming data bytes shall be passed to this function, which
 * will adjust the protocol state machine accordingly.
 * 
 * @param data  Incoming data byte
 */
void SerialProtocol::receive(uint8_t ui8_data)
{
    
    if (ui8_data == STX)
    {
        if (control.e_state == ePROTOCOL_IDLE)
        {
            control.e_state = ePROTOCOL_RECEIVING;
            rxBuffer.putElem(ui8_data);
        }
        else
        // TODO: Error handling
        {
            control.e_state = ePROTOCOL_IDLE;
        }
    }
    else if (ui8_data == ETX)
    {
        
        if (control.e_state == ePROTOCOL_RECEIVING)
        {
            control.e_state = ePROTOCOL_EVALUATING;
            rxBuffer.putElem(ui8_data);
        }
        else
        // TODO: Error handling
        {
            control.e_state = ePROTOCOL_IDLE;
        }
    }
    else if (control.e_state == ePROTOCOL_RECEIVING)
    {
        rxBuffer.putElem(ui8_data);
    }
    #ifdef DEBUG_FUNCTIONS
    if (control.e_state == ePROTOCOL_IDLE)
    {
        switch(control.e_dbgActState)
        {
            case eDEBUG_ACTIVATION_NONE:
                control.e_dbgActState = ui8_data == 'D' ? eDEBUG_ACTIVATION_S1 : eDEBUG_ACTIVATION_NONE;
                break;
            case eDEBUG_ACTIVATION_S1:
                control.e_dbgActState = ui8_data == 'b' ? eDEBUG_ACTIVATION_S2 : eDEBUG_ACTIVATION_NONE;
                break;
            case eDEBUG_ACTIVATION_S2:
                control.e_dbgActState = ui8_data == 'g' ? eDEBUG_ACTIVATION_FINAL : eDEBUG_ACTIVATION_NONE;
                break;
            case eDEBUG_ACTIVATION_FINAL:
                {
                    int8_t ui8_parsed = -1;
                    ui8_parsed = ui8_data - '0';

                    // If the number is in range, execute callback
                    if (ui8_parsed >= 0 && ui8_parsed < 10)
                    {
                        if(control.debugFcnArray[ui8_parsed] != nullptr)
                            control.debugFcnArray[ui8_parsed]();
                    }
                    control.e_dbgActState = eDEBUG_ACTIVATION_NONE;
                }
                break;
                
        }
    }
    else
        control.e_dbgActState = eDEBUG_ACTIVATION_NONE;
    #endif
}


void SerialProtocol::statemachine(void)
{
    
    switch(control.e_state)
    {
        case ePROTOCOL_IDLE:
            break;
        case ePROTOCOL_RECEIVING:
            break;
        case ePROTOCOL_EVALUATING:
            {
                uint8_t *   pui8_buf;
                COMMAND     cmd;
                RESPONSE    rsp; 
                uint8_t     ui8_msgSize = rxBuffer.readBuf(&pui8_buf);

                // Parse the command (skip STX and don't care for ETX)
                cmd = commandParser(++pui8_buf, ui8_msgSize-2);

                rsp = cmdModule.executeCmd(cmd);

                // TODO: Error behaviour if there is no valid response
                // Currently, the arduino won't send any response
                if (rsp.b_valid)
                {
                    txBuffer.flushBuf();
                    
                    // For performance reasons, directly write the TX/RX buffer
                    txBuffer.putElem(STX);
                    txBuffer.getNextFreeBufSpace(&pui8_buf);
                    txBuffer.increaseBufIdx(responseBuilder(pui8_buf, rsp));
                    txBuffer.putElem(ETX);
                    // TODO: Error handling -> Message too long 

                    control.e_state = ePROTOCOL_SENDING;

                }
                else
                    control.e_state = ePROTOCOL_IDLE;
                
                rxBuffer.flushBuf();
            }
    
            break;

        case ePROTOCOL_SENDING:
            {
                uint8_t *   pui8_buf;
                uint8_t     ui8_size;

                if (!control.b_sent && txCallback != nullptr)
                {
                    ui8_size = txBuffer.readBuf(&pui8_buf);
                    control.b_sent = txCallback(pui8_buf, ui8_size);
                }
                else
                {
                    control.b_sent  = false;
                    control.e_state = ePROTOCOL_IDLE;
                }
                
            }
            break;
        
        default:
            break;
    }
}


COMMAND SerialProtocol::commandParser(uint8_t* pui8_buf, uint8_t ui8_stringSize)
{
    uint8_t i = 0;
    // uint8_t cmdIdx  = 0;
    COMMAND cmd     = COMMAND_DEFAULT;

    for (; i < ui8_stringSize; i++)
    {

        if (pui8_buf[i] == GETVAR_IDENTIFIER)
        {
            cmd.e_cmdType = eCOMMAND_TYPE_GETVAR;
            break;
        }
        else if (pui8_buf[i] == SETVAR_IDENTIFIER)
        {
            cmd.e_cmdType = eCOMMAND_TYPE_SETVAR;
            break;
        }
        else if (pui8_buf[i] == COMMAND_IDENTIFIER)
        {
            cmd.e_cmdType = eCOMMAND_TYPE_COMMAND;
            break;
        }
        
        // TODO: Error handling when no command identifier has been received
    }

    // No valid command identifier found (TODO: Error handling)
    if (cmd.e_cmdType == eCOMMAND_TYPE_NONE)
        goto terminate;
    
    /*******************************************************************************************
     * Variable number conversion
    *******************************************************************************************/
    // Loop breaks when i reflects the buffer position of the command identifier
    // Variable number conversion
    {
        // One additional character necessary for string termination
        char p_numStr[i+1] = {0};

        // copy the number string into new array
        memcpy(p_numStr,pui8_buf,i);
        // Properly terminate string to use the atoi buildin
        p_numStr[i] = '\0';
        // Convert
        cmd.i16_num = static_cast<int16_t>(atoi(p_numStr));
    }

    /*******************************************************************************************
     * Variable value conversion
    *******************************************************************************************/
   // Only if a parameter has been passed
   if (ui8_stringSize > i + 1)
   {
        uint8_t ui8_strLen = ui8_stringSize - i;
        char p_valStr[ui8_strLen] = {0};

        // copy the number string into new array
        memcpy(p_valStr, &pui8_buf[i+1], ui8_strLen - 1);
        // Properly terminate string to use the atoi buildin
        p_valStr[ui8_strLen - 1] = '\0';
        // Convert
        cmd.f_val = atof(p_valStr);
    }

    terminate: return cmd;
}

uint8_t SerialProtocol::responseBuilder(uint8_t *pui8_buf, RESPONSE response)
{
    uint8_t ui8_size    = 0;
    uint8_t cmdIdArr[3] = {'?', '!', ':'};

    // Convert variable number to ASCII
    ui8_size = ftoa(pui8_buf, static_cast<float>(response.i16_num), 0, true);

    // Increase Buffer index
    pui8_buf += ui8_size;
    *pui8_buf++ = cmdIdArr[response.e_cmdType];
    ui8_size++;

    // Write the data value into the buffer
    ui8_size += ftoa(pui8_buf, response.f_val, 5 ,true);

    return ui8_size;
}