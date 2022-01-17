/**************************************************************************//**
 * \file Variables.h
 * \author Roman Holderried
 *
 * \brief Declarations for the variable structure.
 * 
 * Variale definitions take place in an external, user defined source file.
 * The variable structure must be based on the types defined by this header.
 *
 * <b> History </b>
 * 	- 2022-01-14 - File creation
 *****************************************************************************/
#ifndef _VARIABLES_H_
#define _VARIABLES_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "stdint.h"

/******************************************************************************
 * Type definitions
 *****************************************************************************/

/** \brief Reflects the storage type of the linked variable.*/
typedef enum
{
    eVARTYPE_NONE,      /*!< Unknown storage type. Shouldn't be used.*/
    eVARTYPE_EEPROM,    /*!< EEPROM variable. Command execution will call EEPROM write/read functions on setVar/getVar.*/
    eVARTYPE_RAM        /*!< RAM variable. Just the linked variable will be accessed, no EEPROM read/write.*/
}TYPE;

/** \brief Reflects the data type of the linked variable.*/
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

/** \brief Variable struct member declaration.*/
typedef struct
{
    void    *val;
    TYPE    vartype;
    DTYPE   datatype;
}VAR;

#endif //_VARIABLES_H_