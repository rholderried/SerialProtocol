#include <stdint.h>
#include "Commands.h"
#include "Variables.h"

extern VAR varStruct[];
extern uint8_t ui8_byteLength[];

SerialCommands::SerialCommands(void){}

RESPONSE SerialCommands::executeCmd(COMMAND cmd)
{
    RESPONSE rsp = RESPONSE_DEFAULT;

    switch (cmd.e_cmdType)
    {

        case eCOMMAND_TYPE_GETVAR:
            {
                float f_val = 0.0;

                // If there is no readEEPROM callback or this is no EEPROM var, simply skip this step
                if (readEEPROM != nullptr && varStruct[cmd.i16_num - 1].vartype == eVARTYPE_EEPROM)
                    // If conditions are met, EEPROM read must be successful.
                    if (!readEEPROM(cmd.i16_num))
                        goto terminate;

                if (!readValFromVarStruct(cmd.i16_num, &f_val))
                    goto terminate;
                
                rsp.i16_num     = cmd.i16_num;
                rsp.f_val       = f_val;
                rsp.e_cmdType   = cmd.e_cmdType;
                rsp.b_valid     = true;
                
            }
            break;

        case eCOMMAND_TYPE_SETVAR:
            {
                float f_formerVal = 0.0;
                bool writeSuccessful = false;
                bool readSuccessful = readValFromVarStruct(cmd.i16_num, &f_formerVal);

                if (!readSuccessful)
                    goto terminate;

                // Read back actual value and write new one (write will only happen if read was successful)
                if (!writeValToVarStruct(cmd.i16_num, cmd.f_val))
                    goto terminate;

                // If the varStruct write operation was successful, trigger an EEPROM write (if callback present and variable is of type eVARTYPE_EEPROM)
                if (writeEEPROM != nullptr && varStruct[cmd.i16_num - 1].vartype == eVARTYPE_EEPROM)
                {
                    // If conditions are met, write must be successful.
                    if (!writeEEPROM(cmd.i16_num))
                    {
                        // If the EEPROM write was not successful, write back the old value to the var struct to keep it in sync with the EEPROM.
                        writeValToVarStruct(cmd.i16_num, f_formerVal);
                        goto terminate;
                    }
                }

                // If everything happens to be allright, create the response
                rsp.i16_num     = cmd.i16_num;
                rsp.f_val       = cmd.f_val;
                rsp.e_cmdType   = cmd.e_cmdType;
                rsp.b_valid     = true;
            }
        // TODO: Settle up the command interface
        default:
            break;
    }

    terminate: return rsp;
}

bool SerialCommands::readValFromVarStruct(int16_t i16_varNum, float *pf_val)
{
    int16_t i16_varIdx = i16_varNum - 1;
    bool b_success;

    if (i16_varNum > 0 && i16_varNum <= VARSTRUCT_LENGTH)
    {

        switch (varStruct[i16_varIdx].datatype)
        {
            case eDTYPE_UINT8:
                *pf_val = static_cast<float>(*reinterpret_cast<uint8_t*>(varStruct[i16_varIdx].val));
                break;
            case eDTYPE_INT8:
                *pf_val = static_cast<float>(*reinterpret_cast<int8_t*>(varStruct[i16_varIdx].val));
                break;
            case eDTYPE_UINT16:
                *pf_val = static_cast<float>(*reinterpret_cast<uint16_t*>(varStruct[i16_varIdx].val));
                break;
            case eDTYPE_INT16:
                *pf_val = static_cast<float>(*reinterpret_cast<int16_t*>(varStruct[i16_varIdx].val));
                break;
            case eDTYPE_UINT32:
                *pf_val = static_cast<float>(*reinterpret_cast<uint32_t*>(varStruct[i16_varIdx].val));
                break;
            case eDTYPE_INT32:
                *pf_val = static_cast<float>(*reinterpret_cast<int32_t*>(varStruct[i16_varIdx].val));
                break;
            case eDTYPE_F32:
                *pf_val = *reinterpret_cast<float*>(varStruct[i16_varIdx].val);
                break;
        }

        b_success = true;
    }

    return b_success;
}

bool SerialCommands::writeValToVarStruct(int16_t i16_varNum, float f_val)
{
    int16_t i16_varIdx = i16_varNum - 1;
    bool b_success;

    if (i16_varNum > 0 && i16_varNum <= VARSTRUCT_LENGTH)
    {

        switch (varStruct[i16_varIdx].datatype)
        {
            case eDTYPE_UINT8:
                *reinterpret_cast<uint8_t*>(varStruct[i16_varIdx].val) = static_cast<uint8_t>(f_val);
                break;
            case eDTYPE_INT8:
                *reinterpret_cast<int8_t*>(varStruct[i16_varIdx].val) = static_cast<int8_t>(f_val);
                break;
            case eDTYPE_UINT16:
                *reinterpret_cast<uint16_t*>(varStruct[i16_varIdx].val) = static_cast<uint16_t>(f_val);
                break;
            case eDTYPE_INT16:
                *reinterpret_cast<int16_t*>(varStruct[i16_varIdx].val) = static_cast<int16_t>(f_val);
                break;
            case eDTYPE_UINT32:
               *reinterpret_cast<uint32_t*>(varStruct[i16_varIdx].val) = static_cast<uint32_t>(f_val);
               break;
            case eDTYPE_INT32:
                *reinterpret_cast<int32_t*>(varStruct[i16_varIdx].val) = static_cast<int32_t>(f_val);
                break;
            case eDTYPE_F32:
                *reinterpret_cast<float*>(varStruct[i16_varIdx].val) = f_val;
                break;
        }

        b_success = true;
    }

    return b_success;
}