
#include "product.h"
#include <hiredis.h>

//redisContext *g_rCtx;
int redis_op_init(void **ctx)
{
	 struct timeval timeout = { 1, 500000 }; // 1.5 seconds   
	 redisContext *g_rCtx;
	/* g_rCtx = redisConnectWithTimeout("127.0.0.1", 6379, timeout);   
	 if (g_rCtx == NULL || g_rCtx->err) 
	 {
	 	  redisFree(g_rCtx); 
		  return -1;
	 }
	*/
	 g_rCtx = redisConnectUnixWithTimeout("/tmp/redis.sock",timeout);
	 if(NULL == g_rCtx)
 	{
 		 redisFree(g_rCtx); 
 		return -1;
 	}
    
    *ctx = (void *)g_rCtx;
 	
	return 0;
}

int redis_op_get_code(void *ctx,char *str)
{
      redisContext *rCtx = (redisContext *)ctx;
	  redisReply *reply;
	  reply = redisCommand(rCtx,"GET NETBAR_WACODE");
	  if(NULL != reply && reply->type == REDIS_REPLY_STRING)
	  {
	  	strcpy(str,reply->str);
	  }
	  freeReplyObject(reply);
	  return 0;
}


int redis_op_get_devinfo(void *ctx,char *mac,u_int *onlinetime,u_short*sport,u_short*dport )
{
     redisContext *rCtx = (redisContext *)ctx;
	 redisReply *reply;
	 char *p = NULL;

	  reply = redisCommand(rCtx,"hmget %s onlinetime srcport endport",mac);
	  if(NULL != reply && REDIS_REPLY_ARRAY == reply->type)
	  {
	  	if(NULL != reply->element[0]->str)
  		{
  			 *onlinetime = strtoul(reply->element[0]->str,&p,10);
  		}
	    if(NULL != reply->element[1]->str)
    	{
    		*sport = strtoul(reply->element[1]->str,&p,10);
    	}
        if(NULL != reply->element[2]->str)
    	{
    		*dport =strtoul(reply->element[2]->str,&p,10);
    	}
	  }
	freeReplyObject(reply);
	  return 0;
}

int redis_op_get_devsn(void *ctx,char *devsn)
{
      redisContext *rCtx = (redisContext *)ctx;
	  redisReply *reply;
	  reply = redisCommand(rCtx,"GET COLLECTION_EQUIPMENT_ID");
	  if(NULL != reply && reply->type == REDIS_REPLY_STRING)
	  {
	  	strcpy(devsn,reply->str);
	  }
	  freeReplyObject(reply);
	  return 0;
}

int redis_op_get_dev_lon_lat(void *ctx,char *longitude,char *latitude)
{
      redisContext *rCtx = (redisContext *)ctx;
	  redisReply *reply;
	  reply = redisCommand(rCtx,"GET LONGITUDE");
	  if(NULL != reply && reply->type == REDIS_REPLY_STRING)
	  {
	  	strcpy(longitude,reply->str);
	  }
	  freeReplyObject(reply);

	  reply = redisCommand(rCtx,"GET LATITUDE");
	  if(NULL != reply && reply->type == REDIS_REPLY_STRING)
	  {
	  	strcpy(latitude,reply->str);
	  }
	  freeReplyObject(reply);
	  
	  return 0;
}
void redis_op_close(void *ctx)
{
    redisContext *rCtx = (redisContext *)ctx;
	redisFree(rCtx);
}


