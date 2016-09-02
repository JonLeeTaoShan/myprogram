//
//  udpclient.c
//  port-mirroring
//
//  Created by 韩清华 on 15/9/21.
//  Copyright (c) 2015年 韩清华. All rights reserved.
//

#include "product.h"
#include "udpclient.h"
#include "spinlock.h"

typedef struct _tagNET_CLIENT_SOCKET
{
    unsigned int uiConnState;	/* 连接状态*/
    int isockfd;
    unsigned int remoteip;
    unsigned short remoteport;
    struct sockaddr_in servaddr;
    SPINLOCK_S spinlock;
}NET_CLIENT_SOCKET;

enum CONN_STATE
{
    CONN_STATE_INIT = 0,
    CONN_STATE_SUCCESS
};

NET_CLIENT_SOCKET g_CliSock;

int udp_client_create_socket(NET_CLIENT_SOCKET *sockcfg)
{
    /* 套接字描述符 */
    int sockfd;
    int iret = 0;
    unsigned int addr;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sockfd < 0)
    {
        /* 如果socket()调用出现错误则显示错误信息并退出 */
        printf("socket error\n");
        return -1;
    }
    
    struct timeval tv_out;
    tv_out.tv_sec = 1;
    tv_out.tv_usec = 0;
    
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv_out, sizeof(tv_out));//设置连接超时
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv_out, sizeof(tv_out));//设置连接超时
    /* 主机字节顺序 */
    memset(&sockcfg->servaddr,0,sizeof(sockcfg->servaddr));
    sockcfg->servaddr.sin_family = AF_INET;
    /* 网络字节顺序，短整型 */
    sockcfg->servaddr.sin_port = htons(sockcfg->remoteport);
    //their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    
    addr = sockcfg->remoteip;
    memcpy(&sockcfg->servaddr.sin_addr,&addr, sizeof(addr));
    
    sockcfg->isockfd = sockfd;
    
    return 0;
}


int udp_client_init(unsigned int ip,unsigned short port)
{
    NET_CLIENT_SOCKET *pstSock = &g_CliSock;
    
    memset(pstSock,0,sizeof(NET_CLIENT_SOCKET));
    
    InitSpinLock(&pstSock->spinlock);
    
    pstSock->remoteip = ip;
    pstSock->remoteport = port;
    if (udp_client_create_socket(pstSock) < 0)
    {
        return -1;
    }
    pstSock->uiConnState = CONN_STATE_SUCCESS;
    
    
    return 0;
}

int udp_client_close()
{
    NET_CLIENT_SOCKET *pstSock = &g_CliSock;
    close(pstSock->isockfd);
    pstSock->isockfd = -1;
    return 0;
}

int udp_client_sent_recv_buff(char *sbuf,int slen,char *rbuf,int *rlen)
{
    int iret =0;
    int recv_len = 0;
    int addrlen = 0;
    NET_CLIENT_SOCKET *pstSock = &g_CliSock;
    
    SpinLock_Lock(&pstSock->spinlock);
    
    if (CONN_STATE_INIT ==  pstSock->uiConnState)
    {
        if (udp_client_create_socket(pstSock) < 0)
        {
            SpinLock_unLock(&pstSock->spinlock);
            return -1;
        }
        pstSock->uiConnState = CONN_STATE_SUCCESS;
    }
    
    addrlen = sizeof(pstSock->servaddr);
    
    iret = sendto(pstSock->isockfd, (const char *)sbuf, slen, 0, (struct sockaddr *)&pstSock->servaddr, addrlen);
    
    //  printf("sendto %d\n", iret);
    
    if (iret < 0)
    {
        printf("%s\n","send error\n" );
        pstSock->uiConnState = CONN_STATE_INIT;
        udp_client_close();
        SpinLock_unLock(&pstSock->spinlock);
        return -1;
    }
    else if (iret >= 0 && NULL != rbuf)
    {
        recv_len = recvfrom(pstSock->isockfd,rbuf,*rlen,0,(struct sockaddr *)&pstSock->servaddr,&addrlen);
        if (recv_len < 0)
        {
            pstSock->uiConnState = CONN_STATE_INIT;
            udp_client_close();
            SpinLock_unLock(&pstSock->spinlock);
            return -1;
        }
        else
        {
            *rlen = recv_len;
        }
    }
    SpinLock_unLock(&pstSock->spinlock);
    return 0;
}

