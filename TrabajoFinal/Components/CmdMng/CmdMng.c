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
#include <string.h>

/*----------------------- DEFINES ------------------------*/
#define CMDMNG_QUEUE_SIZE       64u
#define CMDMNG_BUFFER_SIZE      16u
#define CMDMNG_LF               '\n'
#define CMDMNG_CR               '\r'
#define CMDMNG_EVENT_RAISED     0u
#define CMDMNG_EVENT_LENGTH     4u

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
    int8_t head;
    int8_t tail;
    uint8_t size;
} cmdmng_queue_t;

typedef struct {
    cmdmng_state_t state;
    cmdmng_event_t event;
    cmdmng_queue_t queue;
    uint8_t msg_buffer[CMDMNG_BUFFER_SIZE];
    cmdmng_boolean_t msg_received;
} cmdmng_runtimedata_t;

/*----------------------- VARIABLES ----------------------*/
static char *cmdmng_parser_table[CMDMNG_EVENT_MAX_NUMBER] = {
    "0001",
    "0002",
    "0003",
    "0004"
};

static cmdmng_runtimedata_t cmdmng_runtimedata_default = {
    /* state */         CMDMNG_STATE_0,
    /* event */         CMDMNG_EVENT_NO_REQUEST,
    /* queue */         {{0u}, 0, -1, 0u},
    /* msg_buffer */    {0u},
    /* msg_received */  CMDMNG_BOOLEAN_FALSE
};

static cmdmng_runtimedata_t cmdmng_runtimedata;

static uint8_t cmdmng_rx_buffer[CMDMNG_BUFFER_SIZE];

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
    cmdmng_runtimedata = cmdmng_runtimedata_default;
}

void CmdMng_Task(void)
{
    uartReceive(cmdmng_rx_buffer, CMDMNG_BUFFER_SIZE);

    switch(cmdmng_runtimedata.state)
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
    cmdmng_runtimedata.state = CMDMNG_STATE_IDLE;
}

static void CmdMngIdle(void)
{
    if(CMDMNG_QUEUE_SIZE != cmdmng_runtimedata.queue.size) {
        for(uint8_t i = 0u; i < CMDMNG_BUFFER_SIZE; i++)
        {
            if(CMDMNG_LF == cmdmng_rx_buffer[i]) {
                cmdmng_runtimedata.msg_received =  CMDMNG_BOOLEAN_TRUE;
                break;
            }
            else if(0u == cmdmng_rx_buffer[i]) {
                /* Buffer empty, do nothing */
            }
            else {
                CmdMngQueueWrite(cmdmng_rx_buffer[i]);
            }
        }
    }

    if(CMDMNG_BOOLEAN_TRUE == cmdmng_runtimedata.msg_received) {
        memset(cmdmng_rx_buffer, 0u, CMDMNG_BUFFER_SIZE);
        cmdmng_runtimedata.msg_received = CMDMNG_BOOLEAN_FALSE;
        cmdmng_runtimedata.state = CMDMNG_STATE_PARSE;
    }
}

static void CmdMngParse(void)
{
    if(CMDMNG_BOOLEAN_TRUE == CmdMngParseMsg()) {
        cmdmng_runtimedata.state = CMDMNG_STATE_EVENT;
    }
    else {
        cmdmng_runtimedata.state = CMDMNG_STATE_IDLE;
    }
}

static void CmdMngEvent(void)
{
    cmdmng_boolean_t event_detected = CMDMNG_BOOLEAN_FALSE;
    cmdmng_event_t event;

    for(event = CMDMNG_EVENT_NO_REQUEST; event < CMDMNG_EVENT_MAX_NUMBER; event++) {
        if(CMDMNG_EVENT_RAISED == memcmp(cmdmng_parser_table[event], cmdmng_runtimedata.msg_buffer, CMDMNG_EVENT_LENGTH)) {
            event_detected = CMDMNG_BOOLEAN_TRUE;
            break;
        }
    }

    if(CMDMNG_BOOLEAN_TRUE == event_detected) {
        CmdMngSetEvent(event);
    }

    cmdmng_runtimedata.state = CMDMNG_STATE_IDLE;
}

static void CmdMngQueueWrite(uint8_t data)
{
    if(CMDMNG_QUEUE_SIZE != cmdmng_runtimedata.queue.size) {

        if((CMDMNG_QUEUE_SIZE - 1) == cmdmng_runtimedata.queue.tail) {
            cmdmng_runtimedata.queue.tail = -1;
        }

        cmdmng_runtimedata.queue.buffer[++cmdmng_runtimedata.queue.tail] = data;
        cmdmng_runtimedata.queue.size++;
    }
}

static uint8_t CmdMngQueueRead(void)
{
    uint8_t data = cmdmng_runtimedata.queue.buffer[cmdmng_runtimedata.queue.head];
    cmdmng_runtimedata.queue.buffer[cmdmng_runtimedata.queue.head++] = 0u;

    if(CMDMNG_QUEUE_SIZE == cmdmng_runtimedata.queue.head) {
        cmdmng_runtimedata.queue.head = 0;
    }

    cmdmng_runtimedata.queue.size--;
    return data;
}

static cmdmng_boolean_t CmdMngParseMsg(void)
{
    cmdmng_boolean_t result;

    if(cmdmng_runtimedata.queue.size != 0u) {
        for(uint8_t i = 0u; i < CMDMNG_QUEUE_SIZE; i++)
        {
            uint8_t data = CmdMngQueueRead();

            if((CMDMNG_CR == data) || (i > CMDMNG_BUFFER_SIZE)) {
                break;
            }
            else {
                cmdmng_runtimedata.msg_buffer[i] = data;
            }
        }

        result = CMDMNG_BOOLEAN_TRUE;
    }
    else {
        result = CMDMNG_BOOLEAN_FALSE;
    }

    return result;
}

static void CmdMngSetEvent(cmdmng_event_t event)
{
    cmdmng_runtimedata.event = event;
}

static void CmdMngGetEvent(cmdmng_event_t *event)
{
    *event = cmdmng_runtimedata.event;
}

/* EOF */
