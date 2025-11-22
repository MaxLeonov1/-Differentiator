#include <time.h>
#include <unistd.h>

#include "logger.h"

/*=====================================================================================*/

const char* GetGlobalLogDir() {
    static char log_dir[MAX_STR_LEN_] = "";
    
    if (log_dir[0] == '\0') {
        char proj_path[MAX_STR_LEN_] = "";
        time_t log_time_sec = time(NULL);
        struct tm* log_time = localtime(&log_time_sec);
        
        getcwd(proj_path, sizeof(proj_path));
        sprintf(proj_path, "%s%clogs", proj_path, PATH_SEP);
        _MKDIR(proj_path);

        sprintf(log_dir,
                "%s%clog_%d.%d.%d_%d-%d-%d",
                proj_path,
                PATH_SEP,
                log_time->tm_mday,
                log_time->tm_mon,
                1900 + log_time->tm_year,
                log_time->tm_hour,
                log_time->tm_min,
                log_time->tm_sec);
        _MKDIR(log_dir);
    }
    
    return log_dir;
}

