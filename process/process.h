#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <cstdint>

class Process
{
public:    
    Process() = default;

    ~Process();

    std::uint32_t exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status);

    const char *cpuinfo_path_get(void);
private:    
    const char *path_cpuinfo = "/proc/cpuinfo";
};
#endif

