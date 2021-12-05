/**
 * @file CmdMng.h
 * @brief
 * @version 
 * @date Dec 3, 2021
 * @author lautaro
 */

#ifndef CMDMNG_H_
#define CMDMNG_H_

/*----------------------- INCLUDES -----------------------*/

/*----------------------- DEFINES ------------------------*/

/*----------------------- TYPES --------------------------*/
typedef enum {
    CMDMNG_EVENT_NO_REQUEST           = 0u,
    CMDMNG_EVENT_TRIGGER_BTL          = 1u,
    CMDMNG_EVENT_REQUEST_ERASE        = 2u,
    CMDMNG_EVENT_REQUEST_DOWNLOAD     = 3u,
    CMDMNG_EVENT_MAX_NUMBER           = 4u
} cmdmng_event_t;

/*----------------------- VARIABLES ----------------------*/

/*----------------------- PROTOTYPES ---------------------*/
void CmdMng_Init(void);

void CmdMng_Task(void);

void CmdMng_Read(cmdmng_event_t *event);

#endif /* CMDMNG_H_ */
