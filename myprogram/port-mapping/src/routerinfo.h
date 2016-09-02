

/*
routerinfo.h
*/
#ifndef __ROUTERINFO_H_
#define __ROUTERINFO_H_

int get_wan_name(char *wanname);
int get_wan_IP( char * device, u_int *ip);
int getInterfaceMac(char * device, u_char * mac );
int getInterfaceIp(char *device,u_int *ip);

#endif



