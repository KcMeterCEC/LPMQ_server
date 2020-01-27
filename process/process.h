#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../worker/basic.h"

typedef enum
{
    CPU_STAT,
}ps_cmd;

class Process : public Basic
{
public:    
    Process() = default;

    ~Process();

    std::uint32_t exec(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status);

    const char *cpuinfo_path_get(void);
private:    
    const char *path_cpuinfo = "/proc/cpuinfo";
    const char *path_cpustat = "/proc/stat";

    std::uint32_t exec_cpustat(std::uint8_t *buf, std::uint32_t maximum_len, std::int16_t &status);
};
#endif

