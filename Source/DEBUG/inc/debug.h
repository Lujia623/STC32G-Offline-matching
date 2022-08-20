#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "BSP_STC32G_UART.h"

#if defined(DEVICE_UART1) && !defined(DEBUG_ENABLE)
#define DEBUG_ENABLE
#endif

#ifdef DEBUG_ENABLE

#define _DBG_LOG_HDR(lvl_name, color_n)                    \
    xprintf("\033[0m\033["#color_n "m["lvl_name"] ")
#define _DBG_LOG_X_END                                     \
    xprintf("\033[0m\r\n")

#define dbg_log_line(lvl, color_n, fmt, ...)                \
    do                                                      \
    {                                                       \
        _DBG_LOG_HDR(lvl, color_n);                         \
        xprintf(fmt, ##__VA_ARGS__);                     \
        _DBG_LOG_X_END;                                     \
    }                                                       \
    while (0)

#define LOG_D(fmt, ...)      dbg_log_line("DEBUG", 0, fmt, ##__VA_ARGS__)
#define LOG_W(fmt, ...)      dbg_log_line("WARNING", 33, fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...)      dbg_log_line("ERROR", 31, fmt, ##__VA_ARGS__)
#endif

#endif /*__DEBUG_H_*/
