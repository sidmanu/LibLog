#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "log_intf.h"

log_entry_t* 
log_entry_init()
{
	log_entry_t *ent;
	ent = (log_entry_t *) malloc (sizeof(log_entry_t));
	if (!ent) {
		return NULL;
	}
	return ent;
}

int 
log_add(log_t *log, int level, char *str)
{
	int ret_val = SUCCESS;
	struct timeval t;

	if (log->num_entries >= LOG_MAX_ENTRIES)
		return LOG_FULL;

	log_entry_t *log_ent = NULL;
	log_ent = log_entry_init();
	if (!log_ent)
		goto cleanup;
	ret_val = gettimeofday(&t, NULL);
	
	if (ret_val != 0) {
		printf("Error in getting time of the day\n");
		ret_val = errno;
		goto cleanup;
	}

	log_ent->level = level;
	strncpy(log_ent->log_str, str, LOG_LEN_ENTRY);
	memcpy(&log_ent->time, &t, sizeof(struct timeval));
	memcpy(&log->entries[log->num_entries], 
			log_ent,
			sizeof(log_entry_t));
		
	log->num_entries++;

cleanup:
	free(log_ent);
	return ret_val;

}

void
snprintf_level(char *buf, int level)
{
	switch (level) {
	case LOG_CRITICAL:
		snprintf(buf, 20,  "CRITICAL");
		break;
	case LOG_ERROR:
		snprintf(buf, 20, "ERROR");
		break;
	case LOG_WARNING:
		snprintf(buf, 20, "WARNING");
		break;
	case LOG_DEBUG:
		snprintf(buf, 20, "DEBUG");
		break;
	case LOG_INFO:
		snprintf(buf, 20, "INFO");
		break;
	case LOG_VERBOSE:
		snprintf(buf, 20, "VERBOSE");
		break;
	default:
		snprintf(buf, 20, "UNKNOWN");
	} 
}

void 
snprintf_log_entry(char *str, log_t *log,
	int entry_num)
{
	assert(str);
	assert(log);
	assert((entry_num >= 0) && (entry_num < log->num_entries));
	
	int i = entry_num;

	time_t 	now_time;
	struct tm *now_tm;
	char tmbuf[64], buf[64], level_buf[20];

	now_time = (log->entries[i]).time.tv_sec;
	now_tm = localtime(&now_time);
	strftime(tmbuf, (sizeof tmbuf),
		"%Y-%m-%d %H:%M:%S",
		now_tm);
	/* to get microseconds */
	snprintf(buf, sizeof buf, "%s.%06d >>",
		tmbuf, log->entries[i].time.tv_usec);
	snprintf_level((char *)&level_buf, 
		(log->entries[i]).level);
	
	snprintf(str, LOG_LEN_ENTRY, 
		"%s: %s %s\n", level_buf,
                 buf, log->entries[i].log_str);

}

void 
log_print_all(log_t *log, int level)
{
	int 	i = 0;

	assert(log);
	char str[LOG_LEN_ENTRY];

	for (i = 0; i < LOG_MAX_ENTRIES; i++) {
		if ((log->entries[i]).level != level)
			continue;
		snprintf_log_entry((char*) str, log, i); 
		printf("%s", str);
	}

}


int 
log_init(log_t **pp_log, int log_id, char *name)
{
	assert(pp_log);

	*pp_log = (log_t *) malloc (sizeof(log_t));
	if (!(*pp_log)) {
		printf("Error in malloc!\n");
		return LOG_MALLOC_ERR;
	} 

	memset(*pp_log, 0, sizeof(log_t));

	(*pp_log)->log_id = log_id;
	strncpy((*pp_log)->log_name, name, 20);
	
	return SUCCESS;
}

int 
log_dump_to_file(log_t *log, int max_level, char *file_name)
{
	FILE * file;
	char str[LOG_LEN_ENTRY];
	int i;

	file = fopen(file_name, "a");
	
	if (!file) {
		log_add(log, LOG_ERROR, "Unable to open file"
			" to dump logs\n"); 
		return LOG_INVALID_ARG;	
	}
	
	fprintf(file, "BEGIN--LOG::%s:%d\n\n\n\n", log->log_name,
		log->log_id);
	for (i = 0; i < log->num_entries; i++) {
		if ((log->entries[i]).level > max_level) 
			continue;
		snprintf_log_entry((char *)str, log, i);
		if (strlen(str) != fprintf(file, "%s", str))
			return LOG_FILE_OVERFLOW;;
	}

	fprintf(file, "\n\n\n\nEND--LOG::%s:%d", log->log_name,
		log->log_id);
	fclose(file);
}
