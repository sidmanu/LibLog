#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#define LOG_LEN_ENTRY 512 
#define LOG_MAX_ENTRIES 1000

enum err {
	SUCCESS = 0,
	LOG_MALLOC_ERR,
	LOG_INVALID_ARG,
	LOG_FULL
}; 

typedef enum log_level {
	LOG_CRITICAL =0,
	LOG_ERROR,
	LOG_WARNING,
	LOG_DEBUG,
	LOG_INFO,
	LOG_VERBOSE
} log_level_t;


typedef struct log_entry_ {
	struct timeval time;
	log_level_t level;
	char 	log_str[LOG_LEN_ENTRY];
} log_entry_t;


typedef struct log_ {
	int 	log_id;
	char 	log_name[20];
	int 	num_entries;
	long 	buf_len;	
	struct log_entry_ entries[LOG_MAX_ENTRIES]; 
} log_t;
/* Public API */

int log_init(log_t **log, int log_id, char *name);

int log_add(log_t *log, int level, char *str);

void log_print_all(log_t *log, int level);

void log_hello();
	
