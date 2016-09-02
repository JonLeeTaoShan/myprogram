
/*
cap.h
*/

#ifndef __CAP_H__
#define __CAP_H__

/* pcap�ļ�ͷ�ṹ */
typedef struct _tagPCAPHEAD
{
	unsigned int uilogo;///�ļ���ʶ 
	char	unknow2[0x14];
}PCAPHEAD;

/* pcap�ļ���ÿ������ͷ */
typedef struct _tagPCAPHD 
{
	//�ػ�������ݰ���ʱ��seconds
	unsigned int uisec;
	//�ػ�������ݰ���ʱ��microseconds
	unsigned int uimsec;

	//�������ݰ��ĳ���
	unsigned int  length;
	//�������ݰ��ĳ���
	unsigned int  length2;
}PCAPHD ;


/* ETHͷ���� */
typedef struct ETH_HEADER 
{
	unsigned char  ucDMac[6];
	unsigned char  ucSMac[6];
	unsigned short usEthType;
}ETHDR;

/* ETHͷVLAN���� */
typedef struct ETH_VLAN
{
	unsigned short usVlanTag;
	unsigned short usVlan;
}VLANHDR;

/*  Wireless Header (IEEE 802.11) */
typedef struct
{
	unsigned short 	frame_control;
	unsigned short 	duration_id;
	unsigned char  	addr1[6];
	unsigned char  	addr2[6];
	unsigned char  	addr3[6];
	unsigned short 	seq_control;
	unsigned char  	addr4[6];
} WIFI_HEADER;

/*DHCP protocol*/

typedef struct _tagDHCPHDR
{
	unsigned char op;//报文类型,1表示请求报文,2表示回应报文
	unsigned char htype;//硬件地址类型
	unsigned char hlen;//硬件地址长度
	unsigned char hops;//跳数
	unsigned int xid;//事务ID
	unsigned short secs;//地址续借使用的秒数
	unsigned short flags;//标志字段
	unsigned int ciaddr;//客户端的IP地址
	unsigned int yiaddr;//"你自己的"或客户端的IP地址。
	unsigned int nexaddr;//表明DHCP协议流程的下一个阶段要使用的服务器的IP地址。
	unsigned int giaddr;//DHCP中继器的IP地址。
	unsigned char chaddr[16];//客户端硬件地址
	unsigned char sname[64];//可选的服务器主机名
	unsigned char file[128];//启动文件名,是一个可选的空结尾的字符串
	unsigned int cookie;
}DHCPHDR;
//unsigned char *options;//可选参数域,格式为"代码+长度+数据"

/* 5.4 BIT��IPͷ���� */
typedef struct IP_ADDRESS
{
	unsigned char byte1;
	unsigned char byte2;
	unsigned char byte3;
	unsigned char byte4;
}IpAddress;

/* 6.IPv4 ͷ�Ķ��� */
typedef struct IP_HEADER
{
#if defined(MIPSA1)
	unsigned char ver:4, ihl:4;
#else
	unsigned char ihl:4, ver:4;	/* LITTLE_ENDIAN */
#endif
	unsigned char tos;         // TOS����
	unsigned short tlen;       // �ܳ���
	unsigned short identification; // Identification
	unsigned short flags_fo;     // Flags (3 bits) + Fragment offset (13 bits)
	unsigned char ttl;         // ������
	unsigned char proto;       // �����Э����Ϣ
	unsigned short crc;         // У���
	unsigned int uiSIP;     // ԴIP
	unsigned int uiDIP;     // Ŀ��IP
	//unsigned int   op_pad;       // Option + Padding
}IPHDR;

 /*  ipv6 address  */
typedef struct 
{
	union 
	{
		unsigned char   addr8[16];
		unsigned short	addr16[8];
		unsigned int 	addr32[4];
	} ADDR;                    /* 128-bit IP6 address */
} IPV6_ADDR;

/*IPV6 header*/
typedef struct _tagIPV6
{
	unsigned int ver:4;//version
	unsigned int tra_cla:8;//traffic class
	unsigned int f_lable:20;//flow label
	unsigned short plen;//payload length;
	unsigned char ip_nxt;///* ip_nexthdr 		*/
	unsigned char h_limit;//hop limit
	IPV6_ADDR srcaddr;//srcIP
	IPV6_ADDR dstaddr;//destIP
}IPV6HDR;

/*UDP header*/
typedef struct UDP_HEADER
{
	unsigned short sport; //source port
	unsigned short dport; // dest  port
	unsigned short usLength; //UDP length
	unsigned short usChecksum; //checksum
}UDPHDR;

/* TCP header*/
typedef struct _TCPHeader
{
	unsigned short sPort;			// Source Port
	unsigned short dPort;	        // Destination Port
	unsigned int sequenceNumber;	// Number of Sequence
	unsigned int acknowledgeNumber;	// Number of aknowledge
#if defined(MIPSA1)
	unsigned char	tlen:4;			/* tcp head len				*/
	unsigned char 	reserv_1:4; 	/* reserved					*/
#else
	unsigned char 	reserv_1:4; 	/* reserved					*/
	unsigned char	tlen:4;			/* tcp head len				*/
#endif
	unsigned char flags:6;			/* flags					*/
	unsigned char reserv_2:2;		/* reserved					*/
//	unsigned char dataoffset;		// Pointer to data
//	unsigned char flags;			// Flags
	unsigned short windows;			// Size of window
	unsigned short checksum;		// Total checksum
	unsigned short urgentPointer;	// Urgent pointer
} TCPHDR;

/* Ethernet packet type */
#define ETH_IP_PACKET 0x0800
#define ETH_PPPOE_PACKET 0x8864
#define ETH_PPPOE_PACKET_IPv4 0x0021
#define ETH_VLAN_PACKET 0x8100
#define ETH_IPV6_PACKET 0x86DD


/* Ip Packet Type */
#define TCP_PACKET 0x06
#define UDP_PACKET 0x11

/* PAKET HEADER LENGTH */
#define ETH_HEAD_LENGTH  0x0E     /* Etherner header:     14 bytes */
#define IP_HEAD_LENGTH   0x14     /* IP header:           20 bytes */
//#define TCP_HEAD_LENGTH  0x14     /* TCP header:          20 bytes */
#define UDP_HEAD_LENGTH  0x08     /* UDP header:           8 bytes */
#define PPOE_HEAD_LENGTH 0x08     /* PPPOE header:         8 bytes */


#define pkt_swapu16(n)  ((((n) & 0x00ff) << 8) | (((n) & 0xff00) >> 8))
#define pkt_swapu32(n)   ((((n) & 0xff000000) >> 24) | (((n) & 0x000000ff) << 24) | (((n) & 0x00ff0000) >> 8) | (((n) & 0x0000ff00) << 8))


#endif


