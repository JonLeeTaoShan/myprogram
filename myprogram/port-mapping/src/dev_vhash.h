//
// Created by 韩清华 on 16/2/29.
//

#ifndef PORT_MIRRORING_DEV_VHASH_H
#define PORT_MIRRORING_DEV_VHASH_H

#define DEV_VHASH_NUM_HASHSIZE		0x200	// hashtable SIZE
#define DEV_VHASH(num)			((num) & (DEV_VHASH_NUM_HASHSIZE - 1))	// hash


typedef struct _tagDEV_VHASH_ITEM
{
    char mac[16];//MAC xxxxxxxxxxxx
    char devinfo[64];//
}DEV_VHASH_ITEM;


void DEV_VHash_init();
unsigned int DEV_VHash_Add(DEV_VHASH_ITEM *devItem,DEV_VHASH_ITEM **ppstVhashItem);
unsigned int DEV_VHash_Update(DEV_VHASH_ITEM *devItem,DEV_VHASH_ITEM **ppstVhashItem);
unsigned int DEV_VHash_Del(char *mac);
unsigned int DEV_VHash_FindStruct(char *mac,DEV_VHASH_ITEM **ppstVhashItem);
void DEV_VHash_EXIT();
void DEV_VHash_Lock();
void DEV_VHash_unLock();

void DEV_VHash_Clean();

#endif //PORT_MIRRORING_DEV_VHASH_H
