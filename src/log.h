#pragma once
#include <stdio.h>
#include "types.h"

enum _T_logtype {
    E_LOG_INFO,
    E_LOG_ERR,
    E_LOG_WARN,
};

typedef enum _T_logtype T_logtype;

#define ANSI_RED "\e[31m"
#define ANSI_ORANGE "\e[33m"
#define ANSI_GREEN "\e[32m"
#define ANSI_BLUE "\e[34m"
#define ANSI_DEFAULT "\e[0m"

inline static T_string E_GetColourANSI(T_logtype type) {
    switch(type){
        case E_LOG_INFO:
            return (T_string)ANSI_BLUE;
        case E_LOG_ERR:
            return (T_string)ANSI_RED;
        case E_LOG_WARN:
            return (T_string)ANSI_ORANGE;
        default:
            return (T_string)ANSI_DEFAULT;
    }
}

inline static T_string E_GetLogLabel(T_logtype type) {
    switch(type) {
        case E_LOG_INFO:
            return (T_string)"[INFO] ";
        case E_LOG_ERR:
            return (T_string)"[ERROR]";
        case E_LOG_WARN:
            return (T_string)"[WARN] ";
        default:
            return (T_string)"[LOG]  ";
    }
}

inline static void E_Log(T_logtype type, T_string message) {
    T_string colour = E_GetColourANSI(type);
    T_string logLabel = E_GetLogLabel(type);

    printf("%s%s %s%s\n", colour, logLabel, message, ANSI_DEFAULT);
}