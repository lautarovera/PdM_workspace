/**
 * @file BtlCore.c
 * @brief
 * @version 
 * @date Dec 3, 2021
 * @author lautaro
 */

/*----------------------- INCLUDES -----------------------*/
#include "BtlCore.h"

/*----------------------- DEFINES ------------------------*/

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
    btlcore_target_t boot_target;
    btlcore_boolean_t fault;
} btlcore_runtimedata_t;

/*----------------------- VARIABLES ----------------------*/
static btlcore_runtimedata_t btlcore_runtimedata_default = {
    /* state */         BTLCORE_STATE_0,
    /* boot_target */   BTLCORE_TARGET_DEFAULT,
    /* fault */         BTLCORE_BOOLEAN_FALSE };

static btlcore_runtimedata_t btlcore_runtimedata;

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
}

void BtlCore_Task(void)
{
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

static void BtlCoreState0(void)
{
    if(BTLCORE_TARGET_BTL == btlcore_runtimedata.boot_target) {
        btlcore_runtimedata.state = BTLCORE_STATE_IDLE;
    }
}

static void BtlCoreIdle(void)
{
    if(BTLCORE_TARGET_BTL == btlcore_runtimedata.boot_target) {
        btlcore_runtimedata.state = BTLCORE_STATE_IDLE;
    }
}

static btlcore_boolean_t BtlCoreErase(void)
{

}

static btlcore_boolean_t BtlCoreDownload(void)
{

}

static btlcore_boolean_t BtlCoreUpdate(void)
{

}

static btlcore_boolean_t BtlCoreSelfCheck(void)
{

}

static void BtlCoreReset(void)
{

}

static void BtlCoreError(void)
{

}

/* EOF */
