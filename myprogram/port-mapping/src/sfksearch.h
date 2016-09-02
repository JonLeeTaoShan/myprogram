/*
*   ksearch.h
*
*   Trie based multi-pattern matcher
*
*
*  Copyright (C) 2001 Marc Norton
** Copyright (C) 2014-2015 Cisco and/or its affiliates. All rights reserved.
** Copyright (C) 2003-2013 Sourcefire, Inc.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef KTRIE_H
#define KTRIE_H

#define ALPHABET_SIZE 256


#define KTRIEMETHOD_STD 0
#define KTRIEMETHOD_QUEUE 1

/*
*
*/
typedef struct _ktriepattern {

  struct  _ktriepattern * next;  /* global list of all patterns */
  struct  _ktriepattern * mnext;  /* matching list of duplicate keywords */

  unsigned char * P;    /* no case */
  unsigned char * Pcase; /* case sensitive */
  int             n;
  int             nocase;
  void          * uid;
  int             id;
} KTRIEPATTERN;


/*
*
*/
typedef struct _ktrienode {

  int     edge; /* character */

  struct  _ktrienode * sibling;
  struct  _ktrienode * child;

  KTRIEPATTERN *pkeyword;

} KTRIENODE;


#define KTRIE_ROOT_NODES     256

#define SFK_MAX_INQ 32
typedef struct
{
    unsigned inq;
    unsigned inq_flush;
    void * q[SFK_MAX_INQ];
} SFK_PMQ;

/*
*
*/
typedef struct {

  KTRIEPATTERN * patrn; /* List of patterns, built as they are added */


  KTRIENODE    * root[KTRIE_ROOT_NODES];  /* KTrie nodes */

  int            memory;
  int            nchars;
  int            npats;
  int            duplicates;
  int            method;
  int            end_states; /* should equal npats - duplicates */

  int            bcSize;
  unsigned short bcShift[KTRIE_ROOT_NODES];
  SFK_PMQ        q;

} KTRIE_STRUCT;



KTRIE_STRUCT * KTrieNew();
int            KTrieAddPattern( KTRIE_STRUCT *ts, unsigned char * P, int n,  void *idata,int nocase,int id );
int            KTrieCompile(KTRIE_STRUCT * ts);

int            KTrieSearch( KTRIE_STRUCT * ts, unsigned char * T,  int n,
                            int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
                            void *data,int *id);

int match(KTRIEPATTERN *ktriepattern,void *data,int *id);

unsigned int   KTrieMemUsed(void);
void KTrieInitMemUsed(void);
void           KTrieDelete(KTRIE_STRUCT *k);
int            KTriePatternCount(KTRIE_STRUCT *k);

void sfksearch_print_qinfo(void);

#endif
