/**
 * @file CmdMng.c
 * @brief
 * @version 
 * @date Dec 3, 2021
 * @author lautaro
 */


/*----------------------- INCLUDES -----------------------*/
#include "CmdMng.h"

/*----------------------- DEFINES ------------------------*/

/*----------------------- TYPES --------------------------*/
typedef enum {
    CMDMNG_STATE_0              = 0u,
    CMDMNG_STATE_IDLE           = 1u,
    CMDMNG_STATE_PARSE          = 2u,
    CMDMNG_STATE_EVENT          = 3u,
} cmdmng_state_t;

typedef enum {
    CMDMNG_BOOLEAN_FALSE        = 0u,
    CMDMNG_BOOLEAN_TRUE         = 1u
} cmdmng_boolean_t;

/*----------------------- VARIABLES ----------------------*/
static cmdmng_state_t cmdmng_state;
static cmdmng_event_t cmdmng_event;

/*----------------------- PROTOTYPES ---------------------*/
static cmdmng_boolean_t CmdMngParseMsg(void);
static void CmdMngSetEvent(cmdmng_event_t event);
static void CmdMngGetEvent(cmdmng_event_t *event);
static void CmdMngState0(void);
static void CmdMngIdle(void);
static void CmdMngParse(void);
static void CmdMngEvent(void);

/*----------------------- ROUTINES -----------------------*/
void CmdMng_Init(void)
{
    cmdmng_state = CMDMNG_STATE_0;
    cmdmng_event = CMDMNG_NO_REQUEST;
}

void CmdMng_Task(void)
{
    switch(cmdmng_state)
    {
        case CMDMNG_STATE_0:
            CmdMngState0();
            break;
        case CMDMNG_STATE_IDLE:
            CmdMngIdle();
            break;
        case CMDMNG_STATE_PARSE:
            CmdMngParse();
            break;
        case CMDMNG_STATE_EVENT:
            CmdMngEvent();
        default:
            break;
    }
}

void CmdMng_Read(cmdmng_event_t *event)
{
    CmdMngGetEvent(event);
}

static cmdmng_boolean_t CmdMngParseMsg(void)
{

}

static void CmdMngSetEvent(cmdmng_event_t event)
{
    cmdmng_event = event;
}

static void CmdMngGetEvent(cmdmng_event_t *event)
{
    *event = cmdmng_event;
}

/* EOF */
