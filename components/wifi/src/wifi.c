#include "wifi.h"

const char* WIFI_TAG = "WIFI Task";
enum WifiInitState wifiInitState = WIFI_UNINITIALIZED;
enum WifiScanState wifiScanState = IDLE;
enum WifiConnectionState wifiConnectionState = DISCONNETED;

void event_handler_wifi_t(void* args , esp_event_base_t eventBase, int32_t event_id, void* eventData){
    ESP_LOGI(WIFI_TAG , "Recieved an event => base : %s ; id : %ld" , eventBase , event_id);
    if(event_id == (int32_t)WIFI_EVENT_STA_START){
        ESP_LOGI(WIFI_TAG , "Wifi started from handler");
    }
}
enum WifiResult initializeWifiWithSTAMode(){
    esp_err_t err;
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_LOGI(WIFI_TAG , "Wifi Initialzing in STA Mode");
    wifiInitState = WIFI_INITIALZING;

    ESP_LOGI(WIFI_TAG , "Registering wifi to default looper");
    err = esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &event_handler_wifi_t,
        NULL,
        NULL
    );
    if(err != ESP_OK){
        ESP_LOGE(WIFI_TAG , "Unable to register Wifi to default looper : %d" , err);
        wifiInitState = WIFI_INITIALZATION_FAIL;
        return WIFI_FAIL;
    }

    err = esp_wifi_init(&wifi_init_config);
    if(err != ESP_OK){
        ESP_LOGE(WIFI_TAG , "Unable to init ESP Wifi . code : %d" , err);
        wifiInitState = WIFI_INITIALZATION_FAIL;
        return WIFI_FAIL;
    }

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if(err != ESP_OK){
        ESP_LOGE(WIFI_TAG , "Unable to set Wifi to station mode. code : %d" , err);
        wifiInitState = WIFI_INITIALZATION_FAIL;
        return WIFI_FAIL;
    }
    ESP_LOGI(WIFI_TAG , "Wifi Initialzing done in STA mode");
    ESP_LOGI(WIFI_TAG , "Starting Wifi");
    err = esp_wifi_start();
    if(err != ESP_OK){
        ESP_LOGE(WIFI_TAG , "Unable to start WIFI. code : %d" , err);
        wifiInitState = WIFI_INITIALZATION_FAIL;
        return WIFI_FAIL;
    }
    wifiInitState = WIFI_INITIALIZED;
    return WIFI_SUCCESS;
    
}
enum WifiResult startDefaultWifiScan();
enum WifiResult connectToWifi(wifi_config_t config);