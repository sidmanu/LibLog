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
log_print_all(log_t *log, int level)
{
	int 	i = 0;
	time_t 	now_time;
	struct tm *now_tm;
	char tmbuf[64], buf[64];

	assert(log);

	for (i = 0; i < LOG_MAX_ENTRIES; i++) {
		if ((log->entries[i]).level != level)
			continue;

		printf("\n");
		//snprintf(buf, "%s", 2, 20);
#if 0
		now_time = (log->entries[i]).time.tv_sec;
		now_tm = localtime(&now_time);
		stftime(tmbuf, (sizeof tmbuf),
			"%Y-%m-%d %H:%M:%S",
			now_tm);
		snprintf(buf, sizeof buf, "%s.%06d >>",
			tmbuf, log->entries[i].time.tv_usec);
#endif
	  	printf("%d: %s: %s\n", level,
			buf, log->entries[i].log_str);		

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

void 
log_hello()
{

	printf("Hello\n");

}

