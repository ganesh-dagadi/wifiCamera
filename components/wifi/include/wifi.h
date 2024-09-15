#ifndef WIFI
    #define WIFI
    #include "esp_wifi.h"
    #include "esp_event.h"
    #include "esp_log.h"
    #include "esp_err.h"
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
        DISCONNETED,
        CONNECTING,
        RECONNECTING,
        CONNECTED,
        CONNECTION_FAIL
    };

    enum WifiResult{
        WIFI_SUCCESS,
        WIFI_FAIL
    };

    extern enum WifiInitState wifiInitState;
    extern enum WifiScanState wifiScanState;
    extern enum WifiConnectionState wifiConnectionState;

    // wifi_init_config_t wifi_init_config;

    enum WifiResult initializeWifiWithSTAMode();
    enum WifiResult startDefaultWifiScan();
    enum WifiResult connectToWifi(wifi_config_t config);


#endif