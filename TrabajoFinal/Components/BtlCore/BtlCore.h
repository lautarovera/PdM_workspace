/**
 * @file BtlCore.h
 * @brief
 * @version 
 * @date Dec 3, 2021
 * @author lautaro
 */

#ifndef BTLCORE_H_
#define BTLCORE_H_

/*----------------------- INCLUDES -----------------------*/

/*----------------------- DEFINES ------------------------*/

/*----------------------- TYPES --------------------------*/
typedef enum {
    BTLCORE_BOOLEAN_FALSE               = 0u,
    BTLCORE_BOOLEAN_TRUE                = 1u
}btlcore_boolean_t;

/*----------------------- VARIABLES ----------------------*/

/*----------------------- PROTOTYPES ---------------------*/
void BtlCore_Init(void);

void BtlCore_Task(void);

btlcore_boolean_t BtlCore_FaultRaised(void);

btlcore_boolean_t BtlCore_ResetRequested(void);

#endif /* BTLCORE_H_ */
