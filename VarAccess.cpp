#include <stdint.h>
#include <stdbool.h>
#include "Variables.h"

const uint8_t ui8_byteLength[7] = {1,1,2,2,4,4,4};

VarAccess::VarAccess() {}

bool VarAccess::initVarstruct()
{
    uint16_t currentEEVarAddress = ADDRESS_OFFET;

    for (uint8_t i = 0; i < ui8_varStructLength; i++)
    {
        if (p_varStruct[i].vartype = eVARTYPE_EEPROM)
        {
            p_varStruct[i].runtime.ui8_byteLength = ui8_byteLength[p_varStruct->datatype];
            p_varStruct[i].runtime.ui16_eeAddress = currentEEVarAddress;
            currentEEVarAddress += EEPROMADDRESSTYPE * p_varStruct[i].runtime.ui8_byteLength;
        }
    }
}

//=============================================================================
bool VarAccess::readValFromVarStruct(int16_t i16_varNum, float *pf_val)
{
    int16_t i16_varIdx = i16_varNum - 1;
    bool b_success;

    if (i16_varNum > 0 && i16_varNum <= ui8_varStructLength)
    {

        switch (p_varStruct[i16_varIdx].datatype)
        {
            case eDTYPE_UINT8:
                *pf_val = static_cast<float>(*reinterpret_cast<uint8_t*>(p_varStruct[i16_varIdx].val));
                break;
            case eDTYPE_INT8:
                *pf_val = static_cast<float>(*reinterpret_cast<int8_t*>(p_varStruct[i16_varIdx].val));
                break;
            case eDTYPE_UINT16:
                *pf_val = static_cast<float>(*reinterpret_cast<uint16_t*>(p_varStruct[i16_varIdx].val));
                break;
            case eDTYPE_INT16:
                *pf_val = static_cast<float>(*reinterpret_cast<int16_t*>(p_varStruct[i16_varIdx].val));
                break;
            case eDTYPE_UINT32:
                *pf_val = static_cast<float>(*reinterpret_cast<uint32_t*>(p_varStruct[i16_varIdx].val));
                break;
            case eDTYPE_INT32:
                *pf_val = static_cast<float>(*reinterpret_cast<int32_t*>(p_varStruct[i16_varIdx].val));
                break;
            case eDTYPE_F32:
                *pf_val = *reinterpret_cast<float*>(p_varStruct[i16_varIdx].val);
                break;
        }

        b_success = true;
    }

    return b_success;
}

//=============================================================================
bool VarAccess::writeValToVarStruct(int16_t i16_varNum, float f_val)
{
    int16_t i16_varIdx = i16_varNum - 1;
    bool b_success;

    if (i16_varNum > 0 && i16_varNum <= ui8_varStructLength)
    {

        switch (p_varStruct[i16_varIdx].datatype)
        {
            case eDTYPE_UINT8:
                *reinterpret_cast<uint8_t*>(p_varStruct[i16_varIdx].val) = static_cast<uint8_t>(f_val);
                break;
            case eDTYPE_INT8:
                *reinterpret_cast<int8_t*>(p_varStruct[i16_varIdx].val) = static_cast<int8_t>(f_val);
                break;
            case eDTYPE_UINT16:
                *reinterpret_cast<uint16_t*>(p_varStruct[i16_varIdx].val) = static_cast<uint16_t>(f_val);
                break;
            case eDTYPE_INT16:
                *reinterpret_cast<int16_t*>(p_varStruct[i16_varIdx].val) = static_cast<int16_t>(f_val);
                break;
            case eDTYPE_UINT32:
               *reinterpret_cast<uint32_t*>(p_varStruct[i16_varIdx].val) = static_cast<uint32_t>(f_val);
               break;
            case eDTYPE_INT32:
                *reinterpret_cast<int32_t*>(p_varStruct[i16_varIdx].val) = static_cast<int32_t>(f_val);
                break;
            case eDTYPE_F32:
                *reinterpret_cast<float*>(p_varStruct[i16_varIdx].val) = f_val;
                break;
        }

        b_success = true;
    }

    return b_success;
}