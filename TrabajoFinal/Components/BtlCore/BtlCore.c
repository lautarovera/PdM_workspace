/**
 * @file BtlCore.c
 * @brief
 * @version 
 * @date Dec 3, 2021
 * @author lautaro
 */

/*----------------------- INCLUDES -----------------------*/
#include "CmdMng.h"
#include "BtlCore.h"
#include "API_delay.h"
#include "API_uart.h"

/*----------------------- DEFINES ------------------------*/
#define BTLCORE_DELAY_ERASE             500u
#define BTLCORE_DELAY_DOWNLOAD          5000u
#define BTLCORE_DELAY_UPDATE            2000u

/*----------------------- TYPES --------------------------*/
typedef enum {
    BTLCORE_STATE_0                     = 0u,
    BTLCORE_STATE_IDLE                  = 1u,
    BTLCORE_STATE_ERASE                 = 2u,
    BTLCORE_STATE_DOWNLOAD              = 3u,
    BTLCORE_STATE_UPDATE                = 4u,
    BTLCORE_STATE_SELFCHECK             = 5u,
    BTLCORE_STATE_RESET                 = 6u,
    BTLCORE_STATE_ERROR                 = 255u
} btlcore_state_t;

typedef enum {
    BTLCORE_TARGET_DEFAULT              = 0u,
    BTLCORE_TARGET_BTL                  = 1u,
    BTLCORE_TARGET_APP                  = 2u
} btlcore_target_t;

typedef struct {
    btlcore_state_t state;
    btlcore_boolean_t fault;
    cmdmng_event_t event;
    btlcore_boolean_t reset_flag;
} btlcore_runtimedata_t;

/*----------------------- VARIABLES ----------------------*/
static btlcore_target_t btlcore_boot_target __attribute__((section(".noinit")));

static btlcore_runtimedata_t btlcore_runtimedata_default = {
    /* state */         BTLCORE_STATE_0,
    /* fault */         BTLCORE_BOOLEAN_FALSE,
    /* event */         CMDMNG_EVENT_NO_REQUEST,
    /* reset flag*/     BTLCORE_BOOLEAN_FALSE
};

static btlcore_runtimedata_t btlcore_runtimedata;

static delay_t erase_delay;

static delay_t download_delay;

static delay_t update_delay;

/*----------------------- PROTOTYPES ---------------------*/
static void BtlCoreState0(void);
static void BtlCoreIdle(void);
static void BtlCoreErase(void);
static void BtlCoreDownload(void);
static void BtlCoreUpdate(void);
static void BtlCoreSelfCheck(void);
static void BtlCoreReset(void);
static void BtlCoreError(void);

/*----------------------- ROUTINES -----------------------*/
void BtlCore_Init(void)
{
    btlcore_runtimedata = btlcore_runtimedata_default;

    /* Initialize API delay for LED1, LED2 and LED3 */
    delayInit(&erase_delay, BTLCORE_DELAY_ERASE);
    delayInit(&download_delay, BTLCORE_DELAY_DOWNLOAD);
    delayInit(&update_delay, BTLCORE_DELAY_UPDATE);
}

void BtlCore_Task(void)
{
    CmdMng_Read(&btlcore_runtimedata.event);

    switch(btlcore_runtimedata.state)
    {
        case BTLCORE_STATE_0:
            BtlCoreState0();
            break;
        case BTLCORE_STATE_IDLE:
            BtlCoreIdle();
            break;
        case BTLCORE_STATE_ERASE:
            BtlCoreErase();
            break;
        case BTLCORE_STATE_DOWNLOAD:
            BtlCoreDownload();
            break;
        case BTLCORE_STATE_UPDATE:
            BtlCoreUpdate();
            break;
        case BTLCORE_STATE_SELFCHECK:
            BtlCoreSelfCheck();
            break;
        case BTLCORE_STATE_RESET:
            BtlCoreReset();
            break;
        case BTLCORE_STATE_ERROR:
        default:
            BtlCoreError();
            break;
    }
}

btlcore_boolean_t BtlCore_FaultRaised(void)
{
    return btlcore_runtimedata.fault;
}

btlcore_boolean_t BtlCore_ResetRequested(void)
{
    return btlcore_runtimedata.reset_flag;
}

static void BtlCoreState0(void)
{
    if(CMDMNG_EVENT_TRIGGER_BTL == btlcore_runtimedata.event) {
        uartSendString("JUMP TO BOOTLOADER\r\n", sizeof("JUMP TO BOOTLOADER\r\n"));
        btlcore_boot_target = BTLCORE_TARGET_BTL;
        btlcore_runtimedata.reset_flag = BTLCORE_BOOLEAN_TRUE;
    }

    if(BTLCORE_TARGET_BTL == btlcore_boot_target) {
        btlcore_runtimedata.state = BTLCORE_STATE_IDLE;
    }
}

static void BtlCoreIdle(void)
{
    if(CMDMNG_EVENT_REQUEST_ERASE == btlcore_runtimedata.event) {
        btlcore_runtimedata.state = BTLCORE_STATE_ERASE;
    }
}

static void BtlCoreErase(void)
{
    if(delayRead(&erase_delay))
    {
        if(CMDMNG_EVENT_REQUEST_DOWNLOAD == btlcore_runtimedata.event) {
            uartSendString("ERASE OK\r\n", sizeof("ERASE OK\r\n"));
            btlcore_runtimedata.state = BTLCORE_STATE_DOWNLOAD;
        }
//        else {
//            btlcore_runtimedata.state = BTLCORE_STATE_ERROR;
//        }
    }
}

static void BtlCoreDownload(void)
{
    if(delayRead(&download_delay))
    {
        uartSendString("DOWNLOAD OK\r\n", sizeof("DOWNLOAD OK\r\n"));
        btlcore_runtimedata.state = BTLCORE_STATE_UPDATE;
    }
//    else {
//        btlcore_runtimedata.state = BTLCORE_STATE_ERROR;
//    }
}

static void BtlCoreUpdate(void)
{
    if(delayRead(&update_delay))
    {
        uartSendString("UPDATE OK\r\n", sizeof("UPDATE OK\r\n"));
        btlcore_runtimedata.state = BTLCORE_STATE_SELFCHECK;
    }
}

static void BtlCoreSelfCheck(void)
{
    uartSendString("SELF CHECK OK\r\n", sizeof("SELF CHECK OK\r\n"));
    btlcore_runtimedata.state = BTLCORE_STATE_RESET;
}

static void BtlCoreReset(void)
{
    uartSendString("JUMP TO APP\r\n", sizeof("JUMP TO APP\r\n"));
    btlcore_boot_target = BTLCORE_TARGET_APP;
    btlcore_runtimedata.reset_flag = BTLCORE_BOOLEAN_TRUE;
}

static void BtlCoreError(void)
{
    btlcore_runtimedata.fault = BTLCORE_BOOLEAN_TRUE;
}

/* EOF */
