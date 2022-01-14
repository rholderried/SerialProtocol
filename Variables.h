#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "stdint.h"

#define VARSTRUCT_LENGTH    7

typedef enum
{
    eVARTYPE_NONE,
    eVARTYPE_EEPROM,
    eVARTYPE_RAM
}TYPE;

typedef enum
{
    eDTYPE_UINT8  = 0,
    eDTYPE_INT8   = 1,
    eDTYPE_UINT16 = 2,
    eDTYPE_INT16  = 3,
    eDTYPE_UINT32 = 4,
    eDTYPE_INT32  = 5,
    eDTYPE_F32    = 6
}DTYPE;

typedef struct
{
    void    *val;
    TYPE    vartype;
    DTYPE   datatype;
}VAR;

#endif //_VARIABLES_H_