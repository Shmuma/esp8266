# esp8266

Various small esp8266 projects

## Access point configuration

All projects are missing default access point SSID and password
configuration. They supposed to be put in user/user_config.h include file with
two macro definitions:

    #define SSID "YourIoTSSID"
    #define SSID_PASSWORD "YourPreciousPassword"

## Scanner

Simple AP scanner -- shows available APs on UART1 (921600, GPIO2).
