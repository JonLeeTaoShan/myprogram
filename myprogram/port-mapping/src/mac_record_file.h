
/*mac_record_file.h*/

#ifndef _MAC_RECORD_FILE_H_
#define _MAC_RECORD_FILE_H_

#include "mac_vhash.h"


#define FILE_RECORD_PATH "/etc/"
#define FILE_RECORD_NAME "recordmac.dat"


int file_record_init();
int file_record_update(MAC_ITEM *pstMItem);


#endif

