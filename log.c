/*
writer: jeckypei@163.com
provide a log API
*/

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>



#include "log.h"

int g_console_output = 0;
int g_log_level = LOG_L_INFO;
char * g_log_level_str;
char *g_log_file_name;
char *g_log_file_name_last;

pthread_rwlock_t  g_log_rwlock = PTHREAD_RWLOCK_INITIALIZER;

unsigned long g_log_size_limit = DEFAULT_LOG_SIZE ;
int g_log_fd = -1;
//FILE *g_log_file;
//int g_log_enable_console_output;

char * logLevel [] = {
	(char *)"emery",
	(char *)"alert",
	(char *)"crit",
	(char *)"err",
	(char *)"warning",
	(char *)"notice",
	(char *)"info",
	(char *)"debug",
	(char *)NULL
};
extern int log_open();
int log_init(int enable_console_output)
{
	int i;
	if(g_log_level_str){
		for (i = 0; logLevel[i] != NULL; i++) {
			if( strcmp(g_log_level_str, logLevel[i]) == 0){
				g_log_level = i;
				break;
			}
		} 
	}
	printf("init log output level: %s\n", logLevel[g_log_level]);
	if (enable_console_output)
		g_console_output = ENABLE;
	printf("init log console output : %d\n", g_console_output);
	if(g_log_file_name) {
		g_log_file_name_last = malloc(strlen(g_log_file_name) + 32);
		sprintf(g_log_file_name_last, "%s.last", g_log_file_name);
		log_open();
		printf("open log file  : %s \n", g_log_file_name);
	}
	return 0;
}

int log_open()
{
	if(g_log_file_name) {
		g_log_fd = open(g_log_file_name, O_APPEND |O_RDWR |O_CREAT );
		if(g_log_fd < 0 ) {
			printf("Error: can not open log file: %s, error:%s \n", g_log_file_name, strerror(errno));
		}
			
	}	
	return 0;
}

int log_close()
{
	if ( g_log_fd < 0)
		return 0;
	printf("close log file  : %s \n", g_log_file_name);
	close(g_log_fd);
	g_log_fd = -1;
	return 0;
}



int get_time(char * time_str)
{
	time_t now;
	struct tm *tm_now;
 
	time(&now);
	tm_now = localtime(&now);
 
	sprintf(time_str,"%d/%d/%d %d:%d:%d\n", tm_now->tm_year, tm_now->tm_mon, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	return 0;
}

int check_log_size()
{
	struct stat stat;
	int  ret ;
	if(g_log_fd < 0)
		log_open();
	if(g_log_fd < 0)
		return -1;
	ret = fstat(g_log_fd, &stat);
	if (ret < 0 ) {
		printf("Error: can not get file size of %s, error: %s \n", g_log_file_name, strerror(errno));
		//close(fd);
		return -1;
	}
	if (stat.st_size + 512 < (g_log_size_limit /2)) {
		return 0;
	}

	pthread_rwlock_wrlock(&g_log_rwlock);
	log_close();
	remove(g_log_file_name_last);
	rename(g_log_file_name, g_log_file_name_last);
	log_open();
	 
	pthread_rwlock_unlock(&g_log_rwlock);
	return 0;
}
void mlog(char * module, int log_level, const char* format, ...) 
{  
	va_list valist;  
	int len;
	char time_str[64];
	if (log_level <= g_log_level  || ((g_log_fd < 0) && (g_console_output == DISABLE)) )
		return ;
  
	char buf[LOG_MSG_MAX_LEN] = {0};
	if (NULL == format)  
		   return;	
	get_time(time_str);
	snprintf(buf, sizeof(buf), "%s %s", time_str, module);
	if (module)
		snprintf(buf, sizeof(buf), "%s [%s]: ",buf, logLevel[log_level]);

	len = strlen(buf);


	va_start(valist, format);  
	vsnprintf(buf + len, LOG_MSG_MAX_LEN - len,	format, valist);  
	va_end(valist);  
	pthread_rwlock_rdlock (&g_log_rwlock);

	if (g_log_fd > 0)
		write(g_log_fd, buf, strlen(buf));
	pthread_rwlock_unlock (&g_log_rwlock);
	
	if (g_console_output)
		printf(buf);
	check_log_size();
} 

