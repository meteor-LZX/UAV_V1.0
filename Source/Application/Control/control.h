/********************************************************************************
* @author: Meteor
* @date: 2023/11/4
* @version: 1.0
* @description: This is header file of controlling UAV.
********************************************************************************/
#ifndef UAV_V1_0_CONTROL_H
#define UAV_V1_0_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "main.h"


    // Type definition
    // Drone's lock enum
    typedef enum {
        LOCK,
        UNLOCK
    }drone_lock_e;

    // Drone's status
    typedef struct {
        volatile drone_lock_e _status;
    }drone_status_t;


    // External variables



    // External function
    void UAV_UnLock();


#ifdef __cplusplus
}
#endif

#endif //UAV_V1_0_CONTROL_H
