#include "log_intf.h"


int main()
{
	log_t *mylog;
	int i;
	char buf[30];

	log_init(&mylog, 20, "mainlog");

	log_add(mylog, LOG_INFO, "Program Started");
	
	for (i = 0; i < 5; i++) {
		snprintf(buf, 20, "Counting %d", i+1);
		printf("%s\n", buf);
		log_add(mylog, LOG_ERROR, buf);
		sleep(1);
	}

	log_add(mylog, LOG_INFO, "Done");
	
	log_print_all(mylog, LOG_ERROR);
	printf("INFO\n");
	
	log_print_all(mylog, LOG_INFO);
	log_dump_to_file(mylog, LOG_ERROR, "log_dump.log");

	printf("LOG file dumped to log_dump.log in local directory\n");
	return 0;
}

