#include <stdio.h>
#include "zlog.h"

int main(int argc, char** argv)
{
	int rc;
	zlog_category_t *c;
	rc = zlog_init("log.conf");
	if(rc) {
		printf("init failed\n");
		return -1;
	}
	
	c = zlog_get_category("my_cat");
	if(!c) {
		printf("get category fail\n");
		zlog_fini();
		return -2;
	}
	
	zlog_debug(c, "i am debug one");
	zlog_error(c, "i am error one");
	while(1){	
		zlog_info(c, "hello, zlog");
	}
	zlog_fini();
	return 0; 
}
