//
//  udpclient.h
//  port-mirroring
//
//  Created by 韩清华 on 15/9/21.
//  Copyright (c) 2015年 韩清华. All rights reserved.
//

#ifndef port_mirroring_udpclient_h
#define port_mirroring_udpclient_h

int udp_client_init(unsigned int ip,unsigned short port);

int udp_client_sent_recv_buff(char *sbuf,int slen,char *rbuf,int *rlen);


#endif
