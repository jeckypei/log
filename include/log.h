#ifndef __LOG_H__
#define __LOG_H__
/*
writer: jeckypei@163.com
provide a log API
*/
#include <stdio.h>
#include <string.h>
#include <errno.h> 
#include <stdarg.h>


#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE  0
#endif
#ifndef ENABLE
#define ENABLE  1
#endif
#ifndef DISABLE
#define DISABLE  0
#endif

#define DEFAULT_LOG_SIZE 2000000

#define LOG_MSG_MAX_LEN  8192
enum LogLevelE{
	LOG_L_EMERG = 0,
	LOG_L_ALERT,
	LOG_L_CRIT,
	LOG_L_ERR,
	LOG_L_WARNING,
	LOG_L_NOTICE,
	LOG_L_INFO,
	LOG_L_DEBUG,
	LOG_L_NO_LEVEL,
	
};
//extern int g_log_level;


int log_init(int enable_console_output, char *loglevel, char *logfile, unsigned logsize);
int log_close();
void mlog(char * module, int log_level, const char* format, ...) ;
#define slog(module,log_level,format, ...)     do { mlog(module, log_level,format, ##__VA_ARGS__);} while(0) 

#define LOG_EMERG(format, ...)     do { mlog("", LOG_L_EMERG,format, ##__VA_ARGS__);} while(0) 
#define LOG_ALERT(format, ...)     do { mlog("", LOG_L_ALERT,format, ##__VA_ARGS__);} while(0) 
#define LOG_CRIT(format, ...)     do { mlog("", LOG_L_CRIT,format, ##__VA_ARGS__);} while(0) 
#define LOG_ERR(format, ...)     do { mlog("", LOG_L_ERR,format, ##__VA_ARGS__);} while(0) 
#define LOG_WARNING(format, ...)     do { mlog("", LOG_L_WARNING,format, ##__VA_ARGS__);} while(0) 
#define LOG_NOTICE(format, ...)     do { mlog("", LOG_L_NOTICE,format, ##__VA_ARGS__);} while(0) 
#define LOG_INFO(format, ...)     do { mlog("", LOG_L_INFO,format, ##__VA_ARGS__);} while(0) 
#define LOG_DEBUG(format, ...)     do { mlog("", LOG_L_DEBUG,format, ##__VA_ARGS__);} while(0) 

//extern char * g_log_level_str;
//extern char *g_log_file_name;
//extern unsigned long g_log_size_limit;




#endif
