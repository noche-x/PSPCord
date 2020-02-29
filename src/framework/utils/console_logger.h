#include <string>

class console_logger
{
private:
    /* data */
public:
    void init();

    void log(std::string message, ...);
    // void info(std::string message, ...);
    // void exec(std::string message, ...);
    // void warning(std::string message, ...);
    // void error(std::string message, ...);
};
