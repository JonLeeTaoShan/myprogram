
/*
	redis_op.h
	
*/

#ifndef __REDIS_OP_H_
#define __REDIS_OP_H_

int redis_op_init(void **ctx);
int redis_op_get_code(void *ctx,char *str);
int redis_op_get_devinfo(void *ctx,char *mac,u_int *onlinetime,u_short*sport,u_short *dport );
int redis_op_get_devsn(void *ctx,char *devsn);
int redis_op_get_dev_lon_lat(void *ctx,char *longitude,char *latitude);
void redis_op_close(void *ctx);


#endif


