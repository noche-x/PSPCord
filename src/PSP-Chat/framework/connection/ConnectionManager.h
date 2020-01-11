#include <pspsdk.h>

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
#include <pspwlan.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include <string>

enum ConnectionLibErrors {
    NONE = 0,
    INIT_NETWORK_MANAGER,
    INIT_INET,
    INIT_ACCESS_POINT_CONTROL,    
    INIT_DNS_RESOLVER,    
    INIT_CONNECT_PRIMARY_INTERNET,
    CONNECTION_STATE_ERROR,

};

class ConnectionManager {
public:
    ConnectionManager(const char* address, int port);
    ~ConnectionManager();

    int GetConnectionState();
    int CreateSocket();
    int GetCurrentSocket();
    int CloseCurrentSocket();
    int Connect();    
    ConnectionLibErrors GetErrorCode();
private:
    const char* m_ip;
    int m_port;
    int m_socket;
    std::string_view error;
};