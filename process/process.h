#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../worker/basic.h"

typedef enum
{
    CPU_STAT,
    TASK_LIST,
}ps_cmd;

typedef enum
{
    FOCUS_CPU,
    FOCUS_MEM,
    FOCUS_IO,
}task_list_focus;

struct TaskListHeader
{
    uint16_t focus;
    uint16_t number;
}__attribute__((__packed__));

struct TaskId
{
    uint32_t uid;
    uint32_t gid;
    int32_t  pid;
    int32_t  ppid;
    int32_t  pgid;
    int32_t  sid;
    int32_t  tpgid;
}__attribute__((__packed__));
struct TaskFault
{
    uint32_t minflt;
    uint32_t cminflt;
    uint32_t majflt;
    uint32_t cmajflt;
}__attribute__((__packed__));
struct TaskTime
{ 
    uint32_t utime;
    uint32_t stime;
    int32_t  cutime;
    int32_t  cstime;
    uint64_t starttime;
    uint32_t guest_time;
    int32_t  cguest_time;  
    int64_t  ticket;
}__attribute__((__packed__));
struct TaskMem
{
    uint32_t vsize;
    int32_t  rss;
    uint32_t start_data;
    uint32_t end_data;
    uint32_t start_brk;
    uint32_t arg_start;
    uint32_t arg_end;
    uint32_t env_start;
    uint32_t env_end;
}__attribute__((__packed__));
struct TaskPolicy
{
    int32_t  processor;
    uint32_t rt_priority;
    uint32_t policy;
    int32_t  priority;
    int32_t  nice;
    int32_t  threads;
}__attribute__((__packed__));
struct TaskStat
{
    char state;
    int32_t  tty_nr;
    uint32_t flags;
    int32_t  exit_code;
    uint32_t wchan;
    uint32_t exit_signal;   
    uint64_t delayacct_blkio_ticks; 
}__attribute__((__packed__));
struct TaskOverview
{
    char                name[50];
    struct TaskId       id;
    struct TaskFault    fault;
    struct TaskTime     time;
    struct TaskMem      mem;
    struct TaskPolicy   policy;
    struct TaskStat     stat;
}__attribute__((__packed__));

class Process : public Basic
{
public:    
    Process();

    ~Process();

    uint32_t exec(uint8_t *buf, uint32_t maximum_len, int16_t &status);

    const char *cpuinfo_path_get(void);
private:    
    const char *path_proc = "/proc";
    const char *path_cpuinfo = "/proc/cpuinfo";
    const char *path_cpustat = "/proc/stat";
    char    file_buf[4096] = "\0";
    uint32_t page_size_in_kb = 4;

    bool task_analyze(const struct dirent &entry, struct TaskOverview &task, int16_t &status);    
    uint32_t exec_cpustat(uint8_t *buf, uint32_t maximum_len, int16_t &status) const;
    uint32_t exec_tasklist(uint8_t *buf, uint32_t maximum_len, int16_t &status);
};
#endif

