/**
 * @file CmdMng.c
 * @brief
 * @version 
 * @date Dec 3, 2021
 * @author lautaro
 */


/*----------------------- INCLUDES -----------------------*/
#include "CmdMng.h"
#include "API_uart.h"
#include <stdint.h>

/*----------------------- DEFINES ------------------------*/
#define CMDMNG_QUEUE_SIZE       64u
#define CMDMNG_BUFFER_SIZE      16u
#define CMDMNG_EOL              '\n'
#define CMDMNG_EVENT_RAISED     0u

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

typedef struct {
    uint8_t buffer[CMDMNG_QUEUE_SIZE];
    int8_t head = 0;
    int8_t tail = -1;
    uint8_t size = 0u;
} cmdmng_queue_t;

/*----------------------- VARIABLES ----------------------*/
static cmdmng_state_t cmdmng_state;
static cmdmng_event_t cmdmng_event;
static cmdmng_queue_t cmdmng_queue;
static uint8_t cmdmng_msg_buffer[CMDMNG_BUFFER_SIZE];
static uint8_t cmdmng_parser_table[CMDMNG_EVENT_MAX_NUMBER] = {
    "0001",
    "0002",
    "0003",
    "0004"
};

/*----------------------- PROTOTYPES ---------------------*/
static cmdmng_boolean_t CmdMngParseMsg(void);
static void CmdMngQueueWrite(uint8_t data);
static uint8_t CmdMngQueueRead(void);
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
    uint8_t data;

    uartReceive(&data, 1u);

    if(cmdmng_queue.size != CMDMNG_QUEUE_SIZE) {
        CmdMngQueueWrite(data);
    }

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

static void CmdMngState0(void)
{

}

static void CmdMngIdle(void)
{

}

static void CmdMngParse(void)
{
    if(CMDMNG_BOOLEAN_TRUE == CmdMngParseMsg()) {
        cmdmng_state = CMDMNG_STATE_EVENT;
    }
}

static void CmdMngEvent(void)
{
    cmdmng_boolean_t event_detected = CMDMNG_BOOLEAN_FALSE;
    cmdmng_event_t event;

    for(event = CMDMNG_EVENT_NO_REQUEST; event < CMDMNG_EVENT_MAX_NUMBER; event++) {
        if(CMDMNG_EVENT_RAISED == memcmp(cmdmng_msg_buffer, cmdmng_parser_table[i], sizeof(cmdmng_parser_table[i]))) {
            event_detected = CMDMNG_BOOLEAN_TRUE;
            break;
        }
    }

    if(CMDMNG_BOOLEAN_TRUE == CmdMngParseMsg()) {
        cmdmng_event = event;
    }

    cmdmng_state = CMDMNG_STATE_IDLE;
}

static void CmdMngQueueWrite(uint8_t data)
{
    if(CMDMNG_QUEUE_SIZE != cmdmng_queue.size) {

        if((CMDMNG_QUEUE_SIZE - 1) == cmdmng_queue.tail) {
            cmdmng_queue.tail = -1;
        }

        cmdmng_queue.buffer[++cmdmng_queue.tail] = data;
        cmdmng_queue.size++;
    }
}

static uint8_t CmdMngQueueRead(void)
{
    uint8_t data = cmdmng_queue.buffer[cmdmng_queue.head++];

    if(CMDMNG_QUEUE_SIZE == cmdmng_queue.head) {
        cmdmng_queue.head = 0;
    }

    cmdmng_queue.size--;
    return data;
}

static cmdmng_boolean_t CmdMngParseMsg(void)
{
    cmdmng_boolean_t result;

    if(cmdmng_queue.size != 0u) {
        for(uint8_t i = 0u; i < CMDMNG_QUEUE_SIZE; i++)
        {
            uint8_t data = CmdMngQueueRead();

            if((CMDMNG_EOL == data) || (i > CMDMNG_BUFFER_SIZE)) {
                break;
            }
            else {
                cmdmng_msg_buffer[i++] = data;
            }
        }

        result = CMDMNG_BOOLEAN_TRUE;
    }
    else {
        result = CMDMNG_BOOLEAN_FALSE;
    }
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
