#include "process.h"
#include "../debug.h"

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

Process::Process()
{
    page_size = getpagesize();

    LOG_INFO("task overview size: " << sizeof(struct TaskOverview));
}
Process::~Process()
{

}
uint32_t Process::exec(uint8_t *buf, uint32_t maximum_len, int16_t &status)
{
    uint32_t len = 0;
    switch(buf[0])
    {
        case CPU_STAT:
        {
            len = exec_cpustat(buf + 1, maximum_len - 1, status);
        }break;
        case TASK_LIST:
        {
            len = exec_tasklist(buf + 1, maximum_len - 1, status);
        }break;
        default:
        {
            LOG_ERR("unknown PS command!");
        }break;
    }

    return len ? len + 1 : 0;
}
const char * Process::cpuinfo_path_get(void)
{
    return path_cpuinfo;
}
uint32_t Process::exec_cpustat(uint8_t *buf, uint32_t maximum_len, int16_t &status)
{
    return file_read(path_cpustat, buf, maximum_len, status);
}
uint32_t Process::exec_tasklist(uint8_t *buf, uint32_t maximum_len, int16_t &status)
{
    uint32_t len = 0;
    struct TaskListHeader *head = (struct TaskListHeader *)(buf);

    LOG_INFO("focus: " << head->focus << "number " << head->number);

    uint32_t list_size = head->number * sizeof(struct TaskOverview);
    if(list_size > maximum_len)
    {
        head->number = maximum_len / sizeof(struct TaskOverview);
    }
    struct TaskOverview *task_list = (struct TaskOverview *)(buf);
    struct dirent **namelist;
    int32_t entry_number;
    uint32_t actually_task_num = 0;

    if((entry_number = scandir(path_proc, &namelist, NULL, alphasort)) > 0)
    {
        while(entry_number--)
        {
            LOG_INFO("entry:" << namelist[entry_number]->d_name);

            free(namelist[entry_number]);
        }
        free(namelist);
    }
    else
    {
        perror("can't open proc:");
        status = -LPMQ_NO_FILE;
    }

    len = actually_task_num * sizeof(struct TaskOverview);

    return len;
}