#include "StateMachineWifiCam.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "wifi.h"

const char* STATE_TAG = "STATE MACHINE";
enum InitStateGlobal stateMachineState = STATE_UNINITIALIZED;

enum StateMachineResult initializeStateMachine(){
    esp_err_t err;

    if(stateMachineState != STATE_UNINITIALIZED &&
     stateMachineState != STATE_INITIALZATION_FAIL){
        ESP_LOGW(STATE_TAG , "State Machine is already initialized or initializing");
        return STATE_FAIL;
     }
    ESP_LOGI(STATE_TAG , "Initializing State Machine");
    stateMachineState = STATE_INITIALZING;

    // starting default event looper
    ESP_LOGI(STATE_TAG , "Creating default event Looper");
    err = esp_event_loop_create_default();
    if(err != ESP_OK){
        if(err == ESP_ERR_INVALID_STATE){
            ESP_LOGW(STATE_TAG , "Invalid state default looper already created");
            
        }else{
            ESP_LOGE(STATE_TAG , "Failed to initialize default looper. code : %d" , err);
            stateMachineState = STATE_INITIALZATION_FAIL;
            return STATE_FAIL;
        }
    }

    ESP_LOGI(STATE_TAG , "Initializing NVS");
    err = nvs_flash_init();
    if(err != ESP_OK){
        ESP_LOGE(STATE_TAG , "Failed to initialize NVS. code: %d" , err);
        stateMachineState = STATE_INITIALZATION_FAIL;
        return STATE_FAIL;
    }

    //Initialize Wifi
    initializeWifiWithSTAMode();
    //Initialize Netif

    //Initialize Communication
    return STATE_OK;
}


