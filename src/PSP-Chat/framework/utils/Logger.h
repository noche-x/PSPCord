#include <iostream>
#include <fstream>
#include <iomanip>

namespace Logger
{
    std::ofstream m_out;
    
    void Init(const char* filename) {
        if (std::ifstream(filename).good())
            std::remove(filename);
            
        m_out.open(filename, std::ofstream::out | std::ofstream::app );
    }

    void Log(const char* msg) {
        m_out << msg << std::endl;
        m_out.flush();
    }

    void Log(int msg) {
        m_out << msg << std::endl;
        m_out.flush();
    }

    void Close() {
        m_out.close();
    }
} // namespace Logger
