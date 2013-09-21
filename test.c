#include "log_intf.h"


int main()
{
	log_t *mylog;
	log_hello();
	log_init(&mylog, 20, "mainlog");
	log_add(mylog, LOG_ERROR, "phalana");
	log_add(mylog, LOG_ERROR, "dhimkaaa");
	
	log_print_all(mylog, LOG_ERROR);

	return 0;
}

