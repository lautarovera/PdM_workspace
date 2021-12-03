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
    CMDMNG_TRIGGER_BTL          = 0u,
    CMDMNG_REQUEST_ERASE        = 1u,
    CMDMNG_REQUEST_DOWNLOAD     = 2u,
    CMDMNG_NO_REQUEST           = 255u
} cmdmng_event_t;

/*----------------------- VARIABLES ----------------------*/

/*----------------------- PROTOTYPES ---------------------*/
void CmdMng_Init(void);

void CmdMng_Task(void);

void CmdMng_Read(cmdmng_event_t *event);

#endif /* CMDMNG_H_ */
