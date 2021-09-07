#ifndef _CONFIG_H_
#define _CONFIG_H_

// ======== Global Constants =========

#define MAX_PACKET_SIZE 1024
#define IP_ADDR_SIZE 256
#define DOMAIN_KEY_SIZE 40
#define API_KEY_SIZE 40
#define DEVICE_ID_SIZE 40
#define DEVICE_MODEL_SIZE 40
#define FW_VERSION_SIZE 40
#define SSID_SIZE 128
#define PASSWD_SUZE 80

// ====================================

#define DEBUG_PRINT(PRINTER, ...) PRINTER->print(__VA_ARGS__)
#define DEBUG_PRINTLN(PRINTER, ...) PRINTER->println(__VA_ARGS__)
#define DEBUG_PRINTF(PRINTER, format, ...) PRINTER->printf(format, __VA_ARGS__)

#define XSTR(x) #x
#define STR(x) XSTR(x)

#endif //_CONFIG_H_