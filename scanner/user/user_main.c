#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "driver/uart.h"

static void request_scan(void);
static void scan_done_cb(void* arg, STATUS status);
static void init_done_cb(void);



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
    request_scan();
}


static void request_scan()
{
    struct scan_config sc;
    sc.ssid = NULL;
    sc.bssid = NULL;
    sc.channel = 0;
    sc.show_hidden = 1;

    wifi_station_scan(&sc, scan_done_cb);
}


static void init_done_cb()
{
    uart1_sendStr_no_wait("System init done, scanning for available SSID\n\r");
    request_scan();
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
}
