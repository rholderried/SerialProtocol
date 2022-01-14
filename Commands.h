#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdint.h>
#include <stdbool.h>
#include "Variables.h"

typedef enum 
{
    eCOMMAND_TYPE_NONE      =   -1,
    eCOMMAND_TYPE_GETVAR    =   0,
    eCOMMAND_TYPE_SETVAR    =   1,
    eCOMMAND_TYPE_COMMAND   =   2
}COMMAND_TYPE;

typedef bool(*WRITEEEPROM_CB)(int16_t);
typedef bool(*READEEPROM_CB)(int16_t);

typedef struct
{
    int16_t         i16_num;
    float           f_val;
    COMMAND_TYPE    e_cmdType;
}COMMAND;

#define COMMAND_DEFAULT         {0, 0.0, eCOMMAND_TYPE_NONE}

typedef struct
{
    bool            b_valid;
    int16_t         i16_num;
    float           f_val;
    COMMAND_TYPE    e_cmdType;
}RESPONSE;

#define RESPONSE_DEFAULT         {false, 0, 0.0, eCOMMAND_TYPE_NONE}

class SerialCommands
{
    public:

    WRITEEEPROM_CB  writeEEPROM = nullptr;
    READEEPROM_CB   readEEPROM = nullptr;

    uint8_t ui8_varStructLength;
    VAR     *p_varStruct;
    
    SerialCommands(void);
    
    RESPONSE    executeCmd (COMMAND cmd);
    bool        readValFromVarStruct(int16_t i16_varNum, float *pf_val);
    bool        writeValToVarStruct(int16_t i16_varNum, float f_val);
};

// extern VAR varStruct[];
// extern uint8_t ui8_byteLength[];

#endif //_COMMANDS_H_