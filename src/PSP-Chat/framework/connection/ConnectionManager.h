#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <string.h>
#include <pspnet.h>
#include <psputility.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <psphttp.h>
#include <pspsdk.h>
#include <pspwlan.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h> 
#include <iostream>
#include <string>

class ConnectionManager {
public:
    ConnectionManager();
    ~ConnectionManager();

    int GetConnectionState(int state);
    int CreateSocket();
    int GetCurrentSocket();
    int CloseCurrentSocket();
    int Connect(const char* ipadd);    
    int SendMessage(const char* message);
    const char* RecvMessage();
    const char* GetErrorCode();
private:
    const char* m_ip;
    int m_port;
    int m_socket;
    const char* error;
};