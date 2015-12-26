#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "driver/uart.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void loop(os_event_t *events);


//Main code function
static void ICACHE_FLASH_ATTR
loop(os_event_t *events)
{
    //    os_printf("Hello\n\r");
    uart1_sendStr_no_wait("Hello!\n\r");
    os_delay_us(10000000);
    system_os_post(user_procTaskPrio, 0, 0 );
}


static void scan_done_cb (void* arg, STATUS status)
{
    uart1_sendStr_no_wait("Scan done:\n\r");

    struct bss_info *bssInfo = (struct bss_info*)arg;
    bssInfo = STAILQ_NEXT(bssInfo, next);

    static char buf[200];

    while (bssInfo != NULL) {
        bzero(buf, sizeof(buf));
        os_sprintf(buf, "%-32s %02X:%02X:%02X:%02X:%02X:%02X, ch %2d, auth %d, hid %d, rssi %d\n\r", 
                   bssInfo->ssid, 
                   bssInfo->bssid[0], bssInfo->bssid[1], bssInfo->bssid[2],
                   bssInfo->bssid[3], bssInfo->bssid[4], bssInfo->bssid[5],
                   bssInfo->channel,
                   bssInfo->authmode, bssInfo->is_hidden,
                   bssInfo->rssi);
        uart1_sendStr_no_wait(buf);
        os_delay_us(10000);
        bssInfo = STAILQ_NEXT(bssInfo, next);
    }

    uart1_sendStr_no_wait("\n\r");
    wifi_station_scan(NULL, scan_done_cb);
}


static void init_done_cb() 
{
    uart1_sendStr_no_wait("System init done, scanning for available SSID\n\r");

    struct scan_config sc;
    sc.ssid = NULL;
    sc.bssid = NULL;
    sc.channel = 0;
    sc.show_hidden = 1;

    wifi_station_scan(&sc, scan_done_cb);
}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
    // Set operation mode, no association yet
    wifi_set_opmode(STATION_MODE);

    // Disable stupid auto-connect
    wifi_station_set_auto_connect(0);

    // register callback on system fully setup
    system_init_done_cb(init_done_cb);

    // setup uart driver
    uart_init(BIT_RATE_115200, BIT_RATE_115200);

    /* char ssid[32] = SSID; */
    /* char password[64] = SSID_PASSWORD; */
    /* struct station_config stationConf; */


    //Set ap settings
    /* os_memcpy(&stationConf.ssid, ssid, 32); */
    /* os_memcpy(&stationConf.password, password, 64); */
    /* wifi_station_set_config(&stationConf); */

    // Start os task
    //    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    //    system_os_post(user_procTaskPrio, 0, 0 );
}
