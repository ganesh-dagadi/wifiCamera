#ifndef WIFI
    #define WIFI
    #include "esp_wifi.h"
    #include "esp_event.h"
    #include "esp_log.h"
    #include "esp_err.h"

    #define WIFI_CONNECT_READY 0x00000001
    #define WIFI_CONNECT_NOT_READY 0xFFFFFFFE 
    enum WifiInitState{
        WIFI_UNINITIALIZED,
        WIFI_INITIALZING,
        WIFI_INITIALIZED,
        WIFI_INITIALZATION_FAIL
    };

    enum WifiScanState{
        IDLE,
        SCANNING,
        SCAN_COMPLETED
    };

    enum WifiConnectionState{
        WIFI_DISCONNETED,
        WIFI_CONNECTING,
        WIFI_RECONNECTING,
        WIFI_CONNECTED,
        WIFI_CONNECTION_FAIL
    };

    enum WifiResult{
        WIFI_SUCCESS,
        WIFI_FAIL
    };

    typedef enum WifiEventId{
        WIFI_BEGIN_CONNECTION
    } WifiEventId;

    extern enum WifiInitState wifiInitState;
    extern enum WifiScanState wifiScanState;
    extern enum WifiConnectionState wifiConnectionState;
/*
    bit 0 : wifiReadToConnect
*/
    extern int32_t wifiFlags;
    
    void wifiTaskMain(void* params);



#endif