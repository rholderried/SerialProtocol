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
 * defines
 *****************************************************************************/
#define EEPROM_BYTE_ADRESSABLE      1
#define EEPROM_WORD_ADRESSABLE      2
#define EEPROM_LONG_ADRESSABLE      4

#define EEPROM_ADDRESSTYPE_DEFAULT    EEPROM_BYTE_ADRESSABLE

#ifndef EEPROM_ADDRESSTYPE
#define EEPROM_ADDRESSTYPE EEPROM_ADDRESSTYPE_DEFAULT
#endif

#define ADDRESS_OFFSET_DEFAULT  0
#ifndef ADDRESS_OFFET
#define ADDRESS_OFFET ADDRESS_OFFSET_DEFAULT
#endif

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

    struct
    {
        uint16_t    ui16_eeAddress;
        uint8_t     ui8_byteLength;
    }runtime;
}VAR;

/** \brief EEPROM write user callback.*/
typedef bool(*WRITEEEPROM_CB)(uint32_t ui32_val, uint16_t ui16_address);
/** \brief EEPROM read user callback.*/
typedef bool(*READEEPROM_CB)(uint32_t *ui32_val, uint16_t ui16_address);

class VarAccess
{
    public:

    uint8_t ui8_varStructLength;    /*!< Remembers the length of the variable structure.*/
    VAR     *p_varStruct;           /*!< Remembers the address of the variable structure.*/

    VarAccess();
    bool initVarstruct();

    WRITEEEPROM_CB  writeEEPROM_cb = nullptr;  /*!< Gets called in case of a EEPROM variable has been writen by command.*/
    READEEPROM_CB   readEEPROM_cb  = nullptr;   /*!< Gets called in case of a EEPROM variable has been read by command.*/
    
    /** \brief Performs a variable read operation through the variable structure.
     *
     * @param i16_varNum    Variable number (deduced from ID number) to access.
     * @param *pf_val       Address to the variable to which the value gets written.
     * @returns Success indicator.
     */
    bool readValFromVarStruct(int16_t i16_varNum, float *pf_val);

    /** \brief Performs a variable write operation through the variable structure.
     *
     * @param i16_varNum    Variable number (deduced from ID number) to access.
     * @param f_val         Value to write.
     * @returns Success indicator.
     */
    bool writeValToVarStruct(int16_t i16_varNum, float f_val);

    bool readEEPROMValueIntoVarStruct(int16_t i16_varNum);
    bool writeEEPROMwithValueFromVarStruct(int16_t i16_varNum);
};

#endif //_VARIABLES_H_