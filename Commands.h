/**************************************************************************//**
 * \file Commands.h
 * \author Roman Holderried
 *
 * \brief Command evaluation and variable structure access.
 *
 * <b> History </b>
 * 	- 2022-01-13 - File creation
 *****************************************************************************/
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "Variables.h"
#include "CommandStucture.h"

/******************************************************************************
 * Type definitions
 *****************************************************************************/

/** \brief Command type enumeration.*/
typedef enum 
{
    eCOMMAND_TYPE_NONE      =   -1,
    eCOMMAND_TYPE_GETVAR    =   0,
    eCOMMAND_TYPE_SETVAR    =   1,
    eCOMMAND_TYPE_COMMAND   =   2
}COMMAND_TYPE;

/** \brief EEPROM write user callback.*/
typedef bool(*WRITEEEPROM_CB)(VAR*);
/** \brief EEPROM read user callback.*/
typedef bool(*READEEPROM_CB)(VAR*);


/** \brief Command structure declaration.*/
typedef struct
{
    int16_t         i16_num;    /*!< ID Number.*/
    float           f_val;      /*!< Command Value.*/
    COMMAND_TYPE    e_cmdType;  /*!< Command Type.*/
}COMMAND;

#define COMMAND_DEFAULT         {0, 0.0, eCOMMAND_TYPE_NONE}

/** \brief Response structure declaration.*/
typedef struct
{
    bool            b_valid;    /*!< Flags if response is valid and can be sent.*/
    int16_t         i16_num;    /*!< ID Number. (Reflects Command ID number).*/
    float           f_val;      /*!< Response value.*/
    COMMAND_TYPE    e_cmdType;  /*!< Response type inherited from Command type.*/
}RESPONSE;

#define RESPONSE_DEFAULT         {false, 0, 0.0, eCOMMAND_TYPE_NONE}

/******************************************************************************
 * Class declarations
 *****************************************************************************/
class SerialCommands
{
    public:

    WRITEEEPROM_CB  writeEEPROM = nullptr;  /*!< Gets called in case of a EEPROM variable has been writen by command.*/
    READEEPROM_CB   readEEPROM = nullptr;   /*!< Gets called in case of a EEPROM variable has been read by command.*/

    uint8_t ui8_cmdCBStructLength;  /*!< Remembers the length of the command callback structure.*/
    COMMAND_CB *p_cmdCBStruct;      /*!< Command callback structure.*/

    VarAccess varAccess = VarAccess();
    
    /** \brief SerialCommands c'tor*/
    SerialCommands(void);
    
    /** \brief Executes the incoming command.
     *
     * @param cmd               Holds the command information from the parsed command.
     * @returns Response structure.
     */
    RESPONSE    executeCmd (COMMAND cmd); 
};

#endif //_COMMANDS_H_