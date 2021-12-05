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

/*----------------------- DEFINES ------------------------*/
#define BTLCORE_DELAY_ERASE             500u
#define BTLCORE_DELAY_DOWNLOAD          5000u
#define BTLCORE_DELAY_UPDATE            2000u

/*----------------------- TYPES --------------------------*/
typedef enum {
    BTLCORE_STATE_0             = 0u,
    BTLCORE_STATE_IDLE          = 1u,
    BTLCORE_STATE_ERASE         = 2u,
    BTLCORE_STATE_DOWNLOAD      = 3u,
    BTLCORE_STATE_UPDATE        = 4u,
    BTLCORE_STATE_SELFCHECK     = 5u,
    BTLCORE_STATE_RESET         = 6u,
    BTLCORE_STATE_ERROR         = 255u
} btlcore_state_t;

typedef enum {
    BTLCORE_TARGET_APP          = 0u,
    BTLCORE_TARGET_BTL          = 1u,
    BTLCORE_TARGET_DEFAULT      = 255u
} btlcore_target_t;

typedef enum {
    BTLCORE_BOOLEAN_FALSE       = 0u,
    BTLCORE_BOOLEAN_TRUE        = 1u
}btlcore_boolean_t;

typedef struct {
    btlcore_state_t state;
    btlcore_target_t boot_target __attribute__((section(".noinit"),zero_init));
    btlcore_boolean_t fault;
    cmdmng_event_t event;
} btlcore_runtimedata_t;

/*----------------------- VARIABLES ----------------------*/
static btlcore_runtimedata_t btlcore_runtimedata_default = {
    /* state */         BTLCORE_STATE_0,
    /* boot_target */   BTLCORE_TARGET_DEFAULT,
    /* fault */         BTLCORE_BOOLEAN_FALSE,
    /* event */         CMDMNG_NO_REQUEST
};
static btlcore_runtimedata_t btlcore_runtimedata;
static delay_t erase_delay;
static delay_t download_delay;
static delay_t update_delay;

/*----------------------- PROTOTYPES ---------------------*/
static void BtlCoreState0(void);
static void BtlCoreIdle(void);
static btlcore_boolean_t BtlCoreErase(void);
static btlcore_boolean_t BtlCoreDownload(void);
static btlcore_boolean_t BtlCoreUpdate(void);
static btlcore_boolean_t BtlCoreSelfCheck(void);
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
    CmdMng_Read(btlcore_runtimedata.event);

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

void BtlCore_FaultRaised(void)
{
    return btlcore_runtimedata.fault;
}

static void BtlCoreState0(void)
{
    if(CMDMNG_EVENT_TRIGGER_BTL == btlcore_runtimedata.event) {
        btlcore_runtimedata.boot_target = BTLCORE_TARGET_BTL;
        NVIC_SystemReset();
    }

    if(BTLCORE_TARGET_BTL == btlcore_runtimedata.boot_target) {
        btlcore_runtimedata.state = BTLCORE_STATE_IDLE;
    }
}

static void BtlCoreIdle(void)
{
    if(CMDMNG_EVENT_REQUEST_ERASE == btlcore_runtimedata.event) {
        btlcore_runtimedata.state = BTLCORE_STATE_ERASE;
    }
}

static btlcore_boolean_t BtlCoreErase(void)
{
    if(delayRead(&erase_delay))
    {
        if(CMDMNG_EVENT_REQUEST_DOWNLOAD == btlcore_runtimedata.event) {
            btlcore_runtimedata.state = BTLCORE_STATE_DOWNLOAD;
        }
        else {
            btlcore_runtimedata.state = BTLCORE_STATE_ERROR;
        }
    }
}

static btlcore_boolean_t BtlCoreDownload(void)
{
    if(delayRead(&download_delay))
    {
        btlcore_runtimedata.state = BTLCORE_STATE_UPDATE;
    }
    else {
        btlcore_runtimedata.state = BTLCORE_STATE_ERROR;
    }
}

static btlcore_boolean_t BtlCoreUpdate(void)
{
    if(delayRead(&update_delay))
    {
        btlcore_runtimedata.state = BTLCORE_STATE_SELFCHECK;
    }
}

static btlcore_boolean_t BtlCoreSelfCheck(void)
{
    btlcore_runtimedata.state = BTLCORE_STATE_RESET;
}

static void BtlCoreReset(void)
{
    btlcore_runtimedata.boot_target = BTLCORE_TARGET_APP;
    NVIC_SystemReset();
}

static void BtlCoreError(void)
{
    btlcore_runtimedata.fault = BTLCORE_BOOLEAN_TRUE;
}

/* EOF */
