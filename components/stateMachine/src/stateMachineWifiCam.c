#include "StateMachineWifiCam.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "netif.h"
#include "freertos/task.h"

#define WIFI_TASK_MEMORY 8192
#define NETIF_TASK_MEMEORY 4096

const char* STATE_TAG = "STATE MACHINE";
enum InitStateGlobal stateMachineState = STATE_UNINITIALIZED;
StateConnectionState stateConnectionState = STATE_DISCONNECTED;

enum WifiInitState wifiInitState = WIFI_UNINITIALIZED;
enum WifiScanState wifiScanState = IDLE;
enum WifiConnectionState wifiConnectionState = WIFI_DISCONNETED;
int32_t wifiFlags = 0;

NetifInitState netifInitState = NETIF_UNINITIALIZED;
NetifConnectionState netifConnectionState = NETIF_HASNOIP;

TaskHandle_t wifiTaskHandle = NULL;
TaskHandle_t netifTaskHandle = NULL;
void executeStateMachineState();
enum StateMachineResult initializeStateMachine();
void stateMachineMain(void* params){
    while(1){
        executeStateMachineState();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void executeStateMachineState(){
    if(stateMachineState == STATE_UNINITIALIZED){
        ESP_LOGW(STATE_TAG , "Initializing");
        initializeStateMachine();
    }

    if(wifiInitState == WIFI_UNINITIALIZED){
        if(stateMachineState == STATE_INITIALIZED){
            xTaskCreate(wifiTaskMain,
            "WIFI_TASK",
            WIFI_TASK_MEMORY,
            NULL,
            5,
            &wifiTaskHandle
            );
        }
    }

    if(netifInitState == NETIF_UNINITIALIZED){
        if(stateMachineState == STATE_INITIALIZED){
            xTaskCreate(netifTaskMain,
            "NETIF_TASK",
            NETIF_TASK_MEMEORY,
            NULL,
            5,
            &netifTaskHandle
            );
        }
    }

    if(stateMachineState == STATE_INITIALIZED && netifInitState == NETIF_INITIALIZED && wifiInitState == WIFI_INITIALIZED){
        if(wifiConnectionState == WIFI_DISCONNETED){
            wifiFlags |= WIFI_CONNECT_READY;
        }
    }

    if(stateMachineState == STATE_INITIALIZED && netifConnectionState == NETIF_HASIP && wifiConnectionState == WIFI_CONNECTED){
        stateMachineState = STATE_CONNECTED;
        ESP_LOGI(STATE_TAG , "State is now connected");
    }
}

enum StateMachineResult initializeStateMachine(){
    esp_err_t err;

    if(stateMachineState != STATE_UNINITIALIZED &&
     stateMachineState != STATE_INITIALZATION_FAIL){
        ESP_LOGW(STATE_TAG , "State Machine is already initialized or initializing");
        return STATE_OK;
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
    stateMachineState = STATE_INITIALIZED;
    return STATE_OK;
}


