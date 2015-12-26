# esp8266

Various small esp8266 projects

## Access point configuration

All projects are missing default access point SSID and password
configuration. They supposed to be put in user/user_config.h include file with
two macro definitions:

    #define SSID "YourIoTSSID"
    #define SSID_PASSWORD "YourPreciousPassword"

## Test project

Simple playground used to experiment with API. By default connects with your
ssid, and shows various information in UART1 (115200).o
