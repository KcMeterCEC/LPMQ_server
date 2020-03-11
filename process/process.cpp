#include "process.h"
#include "../debug.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

static int mem_sort(const void *a, const void *b)
{
    struct TaskOverview *p = (struct TaskOverview *)a;
    struct TaskOverview *q = (struct TaskOverview *)b;

    if(p->mem.rss > q->mem.rss) return -1;
    return p->mem.rss != q->mem.rss;
}

Process::Process()
{
    page_size_in_kb = getpagesize() >> 10;

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

    uint32_t maximum_task = (maximum_len - sizeof(struct TaskListHeader)) / sizeof(struct TaskOverview);
    if(head->number > maximum_task)
    {
        head->number = maximum_task;
    }
    struct TaskOverview *task_list = (struct TaskOverview *)(buf + sizeof(struct TaskListHeader));
    struct dirent **namelist;
    int32_t entry_number;
    uint32_t actually_task_num = 0;

    if((entry_number = scandir(path_proc, &namelist, NULL, alphasort)) > 0)
    {
        while(entry_number--)
        {
            actually_task_num += task_analyze(*namelist[entry_number], task_list[actually_task_num], status);
            if(actually_task_num >= maximum_task)
            {
                // LOG_ERR("Need more memory to save task informaton!");
                actually_task_num -= 1;
            }

            free(namelist[entry_number]);
        }
        free(namelist);

        //now,we need sort
        std::qsort(task_list, actually_task_num, sizeof(struct TaskOverview), mem_sort);

        if(actually_task_num > head->number)
        {
            actually_task_num = head->number;
        }
    }
    else
    {
        perror("can't open proc:");
        status = -LPMQ_NO_FILE;
    }

    len = actually_task_num * sizeof(struct TaskOverview) + sizeof(struct TaskListHeader);

    return len;
}
bool Process::task_analyze(const struct dirent &entry, struct TaskOverview &task, int16_t &status)
{
    char dir_name[30];
    bool ret = false;
    long pid;
    if((pid = std::strtol(entry.d_name, NULL, 10)) && (!errno))
    {
        if(std::snprintf(dir_name, sizeof(dir_name), "%s/%s", path_proc, entry.d_name) < 0)
            goto quick_exit;

        task.id.pid = pid;

        //get uid and gid for this process
        struct stat st;
        if(stat(dir_name, &st))
            goto quick_exit;
        task.id.uid = st.st_uid;
        task.id.gid = st.st_gid;

        if(std::snprintf(dir_name, sizeof(dir_name), "%s/%s/%s", path_proc, entry.d_name, "stat") < 0)
            goto quick_exit;
        
        if(!file_read(dir_name, (uint8_t *)file_buf, sizeof(file_buf), status))
            goto quick_exit;
        //get the name of process
        char *cp = std::strchr(file_buf, ')');
        *cp = '\0';
        char *name = std::strchr(file_buf, '(');
        std::strncpy(task.name, name + 1, sizeof(task.name));

        //read stat of process
        int n = sscanf(cp+2,
				"%c %d "                    /* state, ppid */
				"%d %d %d %d "              /* pgid, sid, tty, tpgid */
				"%*s %*s %*s %*s %*s "      /* flags, min_flt, cmin_flt, maj_flt, cmaj_flt */
				"%lu %lu "                  /* utime, stime */
				"%ld %ld %ld "              /* cutime, cstime, priority */
				"%ld "                      /* nice */
				"%ld %*s "                  /* num_threads, it_real_value */
				"%llu "                     /* start_time */
				"%lu "                      /* vsize */
				"%ld "                      /* rss */
				"%*s %*s %*s %*s %*s %*s "  /*rss_rlim, start_code, end_code, start_stack, kstk_esp, kstk_eip */
				"%*s %*s %*s %*s "          /*signal, blocked, sigignore, sigcatch */
				"%*s %*s %*s %*s "          /*wchan, nswap, cnswap, exit_signal */
				"%d %u %u "                 /*cpu last seen on,rt_priority, policy*/
                "%llu %lu %ld "             /*blkio_ticks,guest_time,cguest_time*/
                "%lu %lu %lu %lu "          /*start_data, end_data, start_brk, arg_start*/
                "%lu %lu %lu %d "           /*arg_end, env_start, env_end, exit_code*/
				,
				&task.stat.state, &task.id.ppid,
                &task.id.pgid, &task.id.sid, &task.stat.tty_nr, &task.id.tpgid,
                &task.time.utime, &task.time.stime,
                &task.time.cutime, &task.time.cstime, &task.policy.priority,
                &task.policy.nice, &task.policy.threads,
                &task.time.starttime,
                &task.mem.vsize,
                &task.mem.rss,
                &task.policy.processor, &task.policy.rt_priority, &task.policy.policy,
                &task.stat.delayacct_blkio_ticks, &task.time.guest_time, &task.time.cguest_time,
                &task.mem.start_data, &task.mem.end_data, &task.mem.start_brk, &task.mem.arg_start,
                &task.mem.arg_end, &task.mem.env_start, &task.mem.env_end, &task.stat.exit_code);
        // vsize is in bytes and i want kb
        task.mem.vsize >>= 10;
        // vsize is in bytes but rss is in *PAGES*! Yes, i can't believe that.
        task.mem.rss *= page_size_in_kb;
        //get the ticke of process
        task.time.ticket = task.time.utime + task.time.stime;

        ret = true;
    }

quick_exit:
    return ret;
}