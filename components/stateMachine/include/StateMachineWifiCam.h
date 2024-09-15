#ifndef STATE_MACHINE_WIFI_CAMERA
    #define STATE_MACHINE_WIFI_CAMERA
    #include "esp_event.h"

    enum InitStateGlobal{
        STATE_UNINITIALIZED,
        STATE_INITIALZING,
        STATE_INITIALIZED,
        STATE_INITIALZATION_FAIL
    };

    enum StateMachineResult{
        STATE_OK,
        STATE_FAIL
    };

    extern enum InitStateGlobal stateMachineState;

    enum StateMachineResult initializeStateMachine();
    
#endif
