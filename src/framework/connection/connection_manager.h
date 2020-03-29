
class connection_manager {
public:
    connection_manager();
    ~connection_manager();

    int get_connection_state(int state);
    int create_socket();
    int get_current_socket();
    int connect(const char* ipadd);    
    int send_message(const char* message);
    int recv_message(char* recieve);
    int close_current_socket();
    const char* get_error_code();
private:
    const char* m_ip;
    int m_port;
    int m_socket;
    const char* error;
};