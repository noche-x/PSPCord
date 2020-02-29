#include "console_logger.h"

void console_logger::init()
{
    pspDebugScreenInit();
}

void console_logger::log(std::string message, ...)
{
    message.insert(0, "[+] ");
    message.append("/n");

    va_list argList;
    char cbuffer[512];
    va_start(argList, message);
    vsnprintf(cbuffer, 512, message, argList);
    va_end(argList);

    pspDebugScreenPrintf(cbuffer);
}

void console_logger::info(std::string message, ...)
{
    message.insert(0, "[i] ");
    message.append("/n");

    va_list argList;
    char cbuffer[512];
    va_start(argList, message);
    vsnprintf(cbuffer, 512, message, argList);
    va_end(argList);

    pspDebugScreenPrintf(cbuffer);
}

void console_logger::exec(std::string message, ...)
{
    message.insert(0, "[*] ");
    message.append("/n");

    va_list argList;
    char cbuffer[512];
    va_start(argList, message);
    vsnprintf(cbuffer, 512, message, argList);
    va_end(argList);

    pspDebugScreenPrintf(cbuffer);
}

void console_logger::warning(std::string message, ...)
{
    message.insert(0, "[!] ");
    message.append("/n");

    va_list argList;
    char cbuffer[512];
    va_start(argList, message);
    vsnprintf(cbuffer, 512, message, argList);
    va_end(argList);

    pspDebugScreenPrintf(cbuffer);
}

void console_logger::error(std::string message, ...)
{
    message.insert(0, "[!!] ");
    message.append("/n");

    va_list argList;
    char cbuffer[512];
    va_start(argList, message);
    vsnprintf(cbuffer, 512, message, argList);
    va_end(argList);

    pspDebugScreenPrintf(cbuffer);
}