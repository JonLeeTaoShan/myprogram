/*
*  ksearch.c
*
*  Basic Keyword Search Trie - uses linked lists to build the finite automata
*
*  Keyword-Match: Performs the equivalent of a multi-string strcmp()
*     - use for token testing after parsing the language tokens using lex or the like.
*
*  Keyword-Search: searches the input text for one of multiple keywords,
*  and supports case sensitivite and case insensitive patterns.
*
*
**  Copyright (C) 2001 Marc Norton
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
*
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>

#include "public.h"
#include "rule.h"

#include "sfksearch.h"


static void KTrieFree(KTRIENODE *n);

static unsigned int mtot = 0;

unsigned int KTrieMemUsed(void) { return mtot; }

void KTrieInitMemUsed(void)
{
    mtot = 0;
}

/*
*  Allocate Memory
*/
static void * KTRIE_MALLOC(int n)
{
    void *p;

    if (n < 1)
        return NULL;

    p = calloc(1, n);

    if (p)
        mtot += n;

    return p;
}

/*
*  Free Memory
*/
static void KTRIE_FREE(void *p)
{
    if (p == NULL)
        return;

    free(p);
}

/*
*   Local/Tmp nocase array
*/
static unsigned char Tnocase[65*1024];

/*
** Case Translation Table
*/
static unsigned char xlatcase[256];

/*
*
*/
static void init_xlatcase(void)
{
   int i;
   static int first=1;

   if( !first ) return; /* thread safe */

   for(i=0;i<256;i++)
   {
     xlatcase[ i ] =  (unsigned char)tolower(i);
   }

   first=0;
}

/*
*
*/
static inline void ConvertCaseEx( unsigned char * d, unsigned char *s, int m )
{
     int i;
     for( i=0; i < m; i++ )
     {
       d[i] = xlatcase[ s[i] ];
     }
}


/*
*
*/
KTRIE_STRUCT * KTrieNew()
{
   KTRIE_STRUCT * ts = (KTRIE_STRUCT*) KTRIE_MALLOC( sizeof(KTRIE_STRUCT) );

   if( !ts ) return 0;

   memset(ts, 0, sizeof(KTRIE_STRUCT));

   init_xlatcase();

   ts->memory = sizeof(KTRIE_STRUCT);
   ts->nchars = 0;
   ts->npats  = 0;
   ts->end_states = 0;
   ts->method = 0; /* - old method, 1 = queue */

   return ts;
}

int KTriePatternCount(KTRIE_STRUCT *k)
{
    return k->npats;
}

/*
 * Deletes memory that was used in creating trie
 * and nodes
 */
void KTrieDelete(KTRIE_STRUCT *k)
{
    KTRIEPATTERN *p = NULL;
    KTRIEPATTERN *pnext = NULL;
    int i;

    if (k == NULL)
        return;

    p = k->patrn;

    while (p != NULL)
    {
        pnext = p->next;

        KTRIE_FREE(p->P);
        KTRIE_FREE(p->Pcase);
        KTRIE_FREE(p);

        p = pnext;
    }

    for (i = 0; i < KTRIE_ROOT_NODES; i++)
        KTrieFree(k->root[i]);

    KTRIE_FREE(k);
}

/*
 * Recursively delete all nodes in trie
 */
static void KTrieFree(KTRIENODE *n)
{
    if (n == NULL)
        return;

    KTrieFree(n->child);
    KTrieFree(n->sibling);

    KTRIE_FREE(n);
}

/*
*
*/
static KTRIEPATTERN * KTrieNewPattern(unsigned char * P, int n)
{
   KTRIEPATTERN *p;
   int ret;

   if (n < 1)
       return NULL;

   p = (KTRIEPATTERN*) KTRIE_MALLOC( sizeof(KTRIEPATTERN) );

   if (p == NULL)
       return NULL;

   /* Save as a nocase string */
   p->P = (unsigned char*) KTRIE_MALLOC( n );
   if( !p->P )
   {
       KTRIE_FREE(p);
       return NULL;
   }

   ConvertCaseEx( p->P, P, n );

   /* Save Case specific version */
   p->Pcase = (unsigned char*) KTRIE_MALLOC( n );
   if( !p->Pcase )
   {
       KTRIE_FREE(p->P);
       KTRIE_FREE(p);
       return NULL;
   }

   /*ret = SafeMemcpy(p->Pcase, P, n, p->Pcase, p->Pcase + n);
   if (ret != SAFEMEM_SUCCESS)
   {
       KTRIE_FREE(p->Pcase);
       KTRIE_FREE(p->P);
       KTRIE_FREE(p);
       return NULL;
   }
*/
   memcpy(p->Pcase, P, n);

   p->n    = n;
   p->next = NULL;

   return p;
}

/*
*  Add Pattern info to the list of patterns
*/
int KTrieAddPattern( KTRIE_STRUCT * ts, unsigned char * P, int n,
                      void *idata,int nocase, int id)
{
   KTRIEPATTERN  *pnew;

   if( !ts->patrn )
   {
       pnew = ts->patrn = KTrieNewPattern( P, n );

       if( !pnew ) return -1;
   }
   else
   {
       pnew = KTrieNewPattern(P, n );

       if( !pnew ) return -1;

       pnew->next = ts->patrn; /* insert at head of list */

       ts->patrn = pnew;
   }

   pnew->nocase = nocase;
   pnew->uid    = idata;
   pnew->id     = id;
   pnew->mnext  = NULL;

   ts->npats++;
   ts->memory += sizeof(KTRIEPATTERN) + 2 * n ; /* Case and nocase */

   return 0;
}


/*
*
*/
static KTRIENODE * KTrieCreateNode(KTRIE_STRUCT * ts)
{
   KTRIENODE * t=(KTRIENODE*)KTRIE_MALLOC( sizeof(KTRIENODE) );

   if(!t)
      return 0;

   memset(t,0,sizeof(KTRIENODE));

   ts->memory += sizeof(KTRIENODE);

   return t;
}



/*
*  Insert a Pattern in the Trie
*/
static int KTrieInsert( KTRIE_STRUCT *ts, KTRIEPATTERN * px  )
{
   int            type = 0;
   int            n = px->n;
   unsigned char *P = px->P;
   KTRIENODE     *root;

   /* Make sure we at least have a root character for the tree */
   if( !ts->root[*P] )
   {
      ts->root[*P] = root = KTrieCreateNode(ts);
      if( !root ) return -1;
      root->edge = *P;

   }else{

      root = ts->root[*P];
   }

   /* Walk existing Patterns */
   while( n )
   {
     if( root->edge == *P )
     {
         P++;
         n--;

         if( n && root->child )
         {
            root=root->child;
         }
         else /* cannot continue */
         {
            type = 0; /* Expand the tree via the child */
            break;
         }
     }
     else
     {
         if( root->sibling )
         {
            root=root->sibling;
         }
         else /* cannot continue */
         {
            type = 1; /* Expand the tree via the sibling */
            break;
         }
     }
   }

   /*
   * Add the next char of the Keyword, if any
   */
   if( n )
   {
     if( type == 0 )
     {
      /*
      *  Start with a new child to finish this Keyword
      */
      root->child= KTrieCreateNode( ts );
      if( ! root->child ) return -1;
      root=root->child;
      root->edge  = *P;
      P++;
      n--;
      ts->nchars++;

     }
     else
     {
      /*
      *  Start a new sibling bracnch to finish this Keyword
      */
      root->sibling= KTrieCreateNode( ts );
      if( ! root->sibling ) return -1;
      root=root->sibling;
      root->edge  = *P;
      P++;
      n--;
      ts->nchars++;
     }
   }

   /*
   *    Finish the keyword as child nodes
   */
   while( n )
   {
      root->child = KTrieCreateNode(ts);
      if( ! root->child ) return -1;
      root=root->child;
      root->edge  = *P;
      P++;
      n--;
      ts->nchars++;
   }

   if( root->pkeyword )
   {
      px->mnext = root->pkeyword;  /* insert duplicates at front of list */
      root->pkeyword = px;
      ts->duplicates++;
   }
   else
   {
      root->pkeyword = px;
      ts->end_states++;
   }

   return 0;
}


/*
*
*/
static void Build_Bad_Character_Shifts( KTRIE_STRUCT * kt )
{
    int           i,k;
    KTRIEPATTERN *plist;

    /* Calc the min pattern size */
    kt->bcSize = 32000;

    for( plist=kt->patrn; plist!=NULL; plist=plist->next )
    {
      if( plist->n < kt->bcSize )
      {
          kt->bcSize = plist->n; /* smallest pattern size */
      }
    }

    /*
    *  Initialze the Bad Character shift table.
    */
    for (i = 0; i < KTRIE_ROOT_NODES; i++)
    {
      kt->bcShift[i] = (unsigned short)kt->bcSize;
    }

    /*
    *  Finish the Bad character shift table
    */
    for( plist=kt->patrn; plist!=NULL; plist=plist->next )
    {
       int shift, cindex;

       for( k=0; k<kt->bcSize; k++ )
       {
          shift = kt->bcSize - 1 - k;

          cindex = plist->P[ k ];

          if( shift < kt->bcShift[ cindex ] )
          {
              kt->bcShift[ cindex ] = (unsigned short)shift;
          }
       }
    }
}

static int KTrieBuildMatchStateNode(KTRIENODE *root)
{
    int cnt = 0;
    KTRIEPATTERN *p;

    if (!root)
        return 0;

    /* each and every prefix match at this root*/
    if (root->pkeyword)
    {
        for (p = root->pkeyword; p; p = p->mnext)
        {
            cnt++;
        }

    }

    /* for child of this root */
    if (root->child)
    {
        cnt += KTrieBuildMatchStateNode(root->child);
    }

    /* 1st sibling of this root -- other siblings will be processed from
     * within the processing for root->sibling. */
    if (root->sibling)
    {
        cnt += KTrieBuildMatchStateNode(root->sibling);
    }

    return cnt;
}

static int KTrieBuildMatchStateTrees( KTRIE_STRUCT * ts)
{
    int i, cnt = 0;
    KTRIENODE     * root;

    /* Find the states that have a MatchList */
    for (i = 0; i < KTRIE_ROOT_NODES; i++)
    {
        root = ts->root[i];
        /* each and every prefix match at this root*/
        if (root)
        {
            cnt += KTrieBuildMatchStateNode(root);
        }
    }

    return cnt;
}


/*
*  Build the Keyword TRIE
*
*/
static inline int _KTrieCompile(KTRIE_STRUCT * ts)
{
  KTRIEPATTERN * p;
  /*
  static int  tmem=0;
  */

  /*
  *    Build the Keyword TRIE
  */
  for( p=ts->patrn; p; p=p->next )
  {
       if( KTrieInsert( ts, p ) )
       return -1;
  }

  /*
  *    Build A Setwise Bad Character Shift Table
  */
  Build_Bad_Character_Shifts( ts );

  /*
  tmem += ts->memory;
  printf(" Compile stats: %d patterns, %d chars, %d duplicate patterns, %d bytes, %d total-bytes\n",ts->npats,ts->nchars,ts->duplicates,ts->memory,tmem);
  */

  return 0;
}

int KTrieCompile(KTRIE_STRUCT * ts)
{
    int rval;

    if ((rval = _KTrieCompile(ts)))
        return rval;


    return 0;
}



void sfksearch_print_qinfo(void)
{

}
static
inline
void
_init_queue( SFK_PMQ * b)
{
    b->inq=0;
    b->inq_flush=0;
}

/* uniquely insert into q */
static
inline
int
_add_queue(SFK_PMQ * b, void * p  )
{
    int i;

    for(i=(int)(b->inq)-1;i>=0;i--)
        if( p == b->q[i] )
            return 0;

    if( b->inq < SFK_MAX_INQ )
    {
        b->q[ b->inq++ ] = p;
    }

    if( b->inq == SFK_MAX_INQ )
    {
        return 1;
    }
    return 0;
}

static
inline
unsigned
_process_queue( SFK_PMQ * q,
           int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
           void *data,int *id)
{
    KTRIEPATTERN  * pk;
    unsigned int    i;

    for( i=0; i<q->inq; i++ )
    {
        pk = q->q[i];
        if (pk)
        {
            if (match (pk,data,id) > 0)
            {
                q->inq=0;
                return 1;
            }
        }
    }
    q->inq=0;
    return 0;
}

static
inline
int KTriePrefixMatchQ( KTRIE_STRUCT  * kt,
        unsigned char * T,
        int n,
        int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
        void * data,int *id)
{
    KTRIENODE     * root;
    //KTRIEPATTERN  * pk;
    //int index ;

    root   = kt->root[ xlatcase[*T] ];

    if( !root )
        return 0;

    while( n )
    {
        if( root->edge == xlatcase[*T] )
        {
            T++;
            n--;

            if( root->pkeyword )
            {
                if( _add_queue( &kt->q, root->pkeyword ) )
                {
                    if( _process_queue( &kt->q,match,data,id) )
                    {
                        return 1;
                    }
                }
            }

            if( n && root->child )
            {
                root = root->child;
            }
            else /* cannot continue -- match is over */
            {
                break;
            }
        }
        else
        {
            if( root->sibling )
            {
                root = root->sibling;
            }
            else /* cannot continue */
            {
                break;
            }
        }
    }

    return 0;
}

/*
*   Search - Algorithm
*
*   This routine will log any substring of T that matches a keyword,
*   and processes all prefix matches. This is used for generic
*   pattern searching with a set of keywords and a body of text.
*
*
*
*   kt- Trie Structure
*   T - nocase text
*   Tc- case specific text
*   n - text length
*
*   returns:
*   # pattern matches
*/
static inline int KTriePrefixMatch( KTRIE_STRUCT  * kt,
       unsigned char * T,
       unsigned char * Tc,
       unsigned char * bT,
       int n,
       int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
       void * data,int *id )
{
   KTRIENODE     * root   = kt->root[ *T ];
   int             nfound = 0;
   KTRIEPATTERN  * pk;
   int index ;

   /* Check if any keywords start with this character */
   if( !root ) return 0;

   while( n )
   {
        if( root->edge == *T )
        {
            T++;
            n--;

            pk = root->pkeyword;
            if (pk)
            {
                index = (int)(T - bT - pk->n );
                nfound++;
                if (match (pk, data, id) > 0)
                {
                    return nfound;
                }
            }

            if( n && root->child )
            {
                root = root->child;
            }
            else /* cannot continue -- match is over */
            {
                break;
            }
        }
        else
        {
            if( root->sibling )
            {
                root = root->sibling;
            }
            else /* cannot continue */
            {
                break;
            }
        }
   }

   return nfound;
}

static
inline
int KTrieSearchQ( KTRIE_STRUCT * ks, unsigned char * T, int n,
                  int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
                  void * data ,int *id)
{
    _init_queue(&ks->q);
    while( n > 0 )
    {
        if( KTriePrefixMatchQ( ks, T++, n--, match, data ,id) )
            return 0;
    }
    _process_queue(&ks->q,match,data,id);

    return 0;
}

static
inline
int KTrieSearchQBC( KTRIE_STRUCT * ks, unsigned char * T, int n,
                    int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
                    void * data ,int *id)
{
    int             tshift;
    unsigned char  *Tend;
    short          *bcShift = (short*)ks->bcShift;
    int             bcSize  = ks->bcSize;

    _init_queue(&ks->q);

    Tend = T + n - bcSize;

    bcSize--;

    for( ;T <= Tend; n--, T++ )
    {
        while( (tshift = bcShift[ T[bcSize] ]) > 0 )
        {
            T  += tshift;
            if( T > Tend )
                return 0;
        }

        if( KTriePrefixMatchQ( ks, T, n, match, data ,id) )
            return 0;
    }

    _process_queue(&ks->q,match,data,id);

    return 0;
}


/*
*
*/
static
inline
int KTrieSearchNoBC( KTRIE_STRUCT * ks, unsigned char * Tx, int n,
                     int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
                     void * data ,int *id)
{
   int            nfound = 0;
   unsigned char *T, *bT;

   ConvertCaseEx( Tnocase, Tx, n );

   T  = Tnocase;
   bT = T;

   for( ; n>0 ; n--, T++, Tx++ )
   {
      nfound += KTriePrefixMatch( ks, T, Tx, bT, n, match, data ,id);
   }

   return nfound;
}

/*
*
*/
static
inline
int KTrieSearchBC( KTRIE_STRUCT * ks, unsigned char * Tx, int n,
                   int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
                   void * data ,int *id)
{
   int             tshift;
   unsigned char  *Tend;
   unsigned char  *T, *bT;
   int             nfound  = 0;
   short          *bcShift = (short*)ks->bcShift;
   int             bcSize  = ks->bcSize;

   ConvertCaseEx( Tnocase, Tx, n );

   T  = Tnocase;
   bT = T;

   Tend = T + n - bcSize;

   bcSize--;

   for( ;T <= Tend; n--, T++, Tx++ )
   {
       while( (tshift = bcShift[ *( T + bcSize ) ]) > 0 )
       {
          T  += tshift;
          Tx += tshift;
          if( T > Tend ) return nfound;
       }

       nfound += KTriePrefixMatch( ks, T, Tx, bT, n, match, data ,id);
   }

   return nfound;
}

/*
*
*/
int KTrieSearch( KTRIE_STRUCT * ks, unsigned char * T, int n,
           int(*match)(KTRIEPATTERN *ktriepattern,void *data,int *id),
           void * data,int *id )
{
    if( ks->method == KTRIEMETHOD_QUEUE )
    {
      //if( ks->bcSize < 3)
       return KTrieSearchQ( ks, T, n, match, data ,id);
      //else
      // return KTrieSearchQBC( ks, T, n, match, data );
    }
    else
    {
        if( ks->bcSize < 3)
            return KTrieSearchNoBC( ks, T, n, match, data ,id);
        else
            return KTrieSearchBC( ks, T, n, match, data ,id);
    }
}


int match(KTRIEPATTERN *ktriepattern,void *data,int *id)
{
    PROTOCOL_PARSE *pparse = (PROTOCOL_PARSE *)data;
    if(ktriepattern)
    {
        sprintf(pparse->proto_name,"%s",ktriepattern->uid);
        *id = ktriepattern->id;
    }
    return 0;
}

/*
*
*    TEST DRIVER FOR KEYWORD TRIE
*
*
*/

#ifdef KTRIE_MAIN

typedef enum _en_PROTO_ID
{
  en_PROTO_ID_HTTP       = 0,
  en_PROTO_ID_WEIXIN,
  en_PROTO_ID_BAIDUSEARCH,
  en_PROTO_ID_BAIDUAPP,
  en_PROTO_ID_BAIDUTIEBA,
  en_PROTO_ID_QQ,
  en_PROTO_ID_QQGAME,
  en_PROTO_ID_TAOBAO,
  en_PROTO_ID_MEITUANTUANGOU,
  en_PROTO_ID_JD,
  en_PROTO_ID_DIANPING,
  en_PROTO_ID_BDNUOMI,
  en_PROTO_ID_MEITUANWAIMAI,
  en_PROTO_ID_WEIBO,
  en_PROTO_ID_58TONGCHENG,
  en_PROTO_ID_DIDIDACHE,
  en_PROTO_ID_360SAFE,
  en_PROTO_ID_CTRIP,
//  en_PROTO_ID_TIANYASHEQU,
//  en_PROTO_ID_MAOPU,
//  en_PROTO_ID_WANGYI,
  en_PROTO_ID_163WEBMAIL,
  en_PROTO_ID_QQNEWS,
  en_PROTO_ID_QQZONE,
  en_PROTO_ID_TECENTVIDEO,
  en_PROTO_ID_ZHIFUBAO,
  en_PROTO_ID_ZHILIAN,
  en_PROTO_ID_LEIPIN,
  en_PROTO_ID_KUGOU,
  en_PROTO_ID_QQMAIL,
  en_PROTO_ID_12306,
  en_PROTO_ID_WEIPIN,
  en_PROTO_ID_YY,
  en_PROTO_ID_QUNAR,
  en_PROTO_ID_VDIAN,
  en_PROTO_ID_DAZHIHUI,
  en_PROTO_ID_XUNLEI,
  en_PROTO_ID_360YUNPAN,
  en_PROTO_ID_JINRITOUTIAO,
  en_PROTO_ID_BAIDUMAP,
  en_PROTO_ID_GAODEMAP,
  en_PROTO_ID_DIDAPINCHE,
  en_PROTO_ID_SUNINGYIGOU,
  en_PROTO_ID_AIQIYI,
  en_PROTO_ID_YOUKU,
  en_PROTO_ID_QQMUSIC,
  en_PROTO_ID_LETV,
  en_PROTO_ID_MGOTV,
  en_PROTO_ID_BAOFENG,
  en_PROTO_ID_SOHUVIDEO,
  en_PROTO_ID_TUDOU,
  en_PROTO_ID_BDVIDEO,
  en_PROTO_ID_KUWO,
  en_PROTO_ID_FTP,
  en_PROTO_ID_IMAP,
  en_PROTO_ID_POP,
  en_PROTO_ID_SMTP,
  en_PROTO_ID_TELNET,
  en_PROTO_ID_MILIAO,
  en_PROTO_ID_NULL,
  en_PROTO_ID_MAX
}PROTO_ID;

typedef struct _tagHostPattern
{
  int id;
  char proto_name[16];
  char host[32];
}URLPATTERN;



URLPATTERN g_urlpattern[] = {
    {en_PROTO_ID_NULL,"","xiaoyun.com"},
    {en_PROTO_ID_NULL,"","wireless.mapbar.com"},
    {en_PROTO_ID_NULL,"","apple.com"},
    {en_PROTO_ID_NULL,"","gclick.cn"},
    {en_PROTO_ID_NULL,"","pro.cn"},
    {en_PROTO_ID_NULL,"","bdimg.com"},
    {en_PROTO_ID_BAIDUAPP,"baiduapp","m.baidu.com"},
    {en_PROTO_ID_BAIDUTIEBA,"baidutieba","c.tieba.baidu.com"},
    {en_PROTO_ID_TAOBAO,"taobao","m.taobao.com"},
    {en_PROTO_ID_MEITUANTUANGOU,"meituantuangou","api.meituan.com"},
    {en_PROTO_ID_MEITUANWAIMAI,"meituanwaimai","waimaiapi.meituan.com"},
    {en_PROTO_ID_JD,"jd","jd.com"},
    {en_PROTO_ID_JD,"jd","m.360buy.com"},
    {en_PROTO_ID_DIANPING,"dianping","api.dianping.com"},
    {en_PROTO_ID_BDNUOMI,"bdnuomi","app.nuomi.com"},
    {en_PROTO_ID_WEIBO,"weibo","wbapp.mobile.sina.cn"},
    {en_PROTO_ID_58TONGCHENG,"58tongcheng","app.58.com"},
    {en_PROTO_ID_DIDIDACHE,"dididache","diditaxi.com.cn"},
    {en_PROTO_ID_DIDIDACHE,"dididache","udache.com"},
    {en_PROTO_ID_360SAFE,"360safe","shouji.360.cn"},
    {en_PROTO_ID_CTRIP,"ctrip","m.ctrip.com"},
    {en_PROTO_ID_QQNEWS,"qqnews","r.inews.qq.com"},
    {en_PROTO_ID_NULL,"","uu.qq.com"},
    {en_PROTO_ID_NULL,"","pingma.qq.com"},
    {en_PROTO_ID_TECENTVIDEO,"tecentvideo","video.qq.com"},
    {en_PROTO_ID_NULL,"","omgmta1.qq.com"},
    {en_PROTO_ID_ZHIFUBAO,"zhifubao","mobilecns.alipay.com"},
    {en_PROTO_ID_ZHIFUBAO,"zhifubao","mdap.alipay.com"},
    {en_PROTO_ID_KUGOU,"kugou","kugou.com"},
    {en_PROTO_ID_QQMAIL,"qqmail","mail.qq.com"},
    {en_PROTO_ID_12306,"12306","domob.cn"},
    {en_PROTO_ID_WEIPIN,"weipin","appvipshop.com"},
    {en_PROTO_ID_YY,"YY","3g.yy.com"},
    {en_PROTO_ID_NULL,"","image.yy.com"},
    {en_PROTO_ID_QUNAR,"qunar","mapi.travel.qunar.com"},
    {en_PROTO_ID_VDIAN,"vdian","125.39.222.147:80"},
    {en_PROTO_ID_XUNLEI,"xunlei","m.xunlei.com"},
    {en_PROTO_ID_360YUNPAN,"360yunpan","yunpan.360.cn"},
    {en_PROTO_ID_JINRITOUTIAO,"jinritoutiao","snssdk.com"},
    {en_PROTO_ID_BAIDUMAP,"baidumap","client.map.baidu.com"},
    {en_PROTO_ID_GAODEMAP,"gaodemap","webfiles.amap.com"},
    {en_PROTO_ID_SUNINGYIGOU,"suningyigou","suning.com"},
    {en_PROTO_ID_AIQIYI,"aiqiyi","iqiyi.com"},
    {en_PROTO_ID_YOUKU,"youku","3g.youku.com"},
    {en_PROTO_ID_YOUKU,"youku","mobile.youku.com"},
    {en_PROTO_ID_YOUKU,"youku","m.youku.com"},
    {en_PROTO_ID_QQMUSIC,"qqmusic","music.qq.com"},
    {en_PROTO_ID_LETV,"letv","letv.com"},
    {en_PROTO_ID_MGOTV,"mgotv","mobile.api.hunantv.com"},
    {en_PROTO_ID_BAOFENG,"baofeng","baofeng.com"},
    {en_PROTO_ID_BAOFENG,"baofeng","baofeng.net"},
    {en_PROTO_ID_SOHUVIDEO,"sohuvideo","tv.sohu.com"},
    {en_PROTO_ID_TUDOU,"tudou","3g.tudou.com"},
    {en_PROTO_ID_BDVIDEO,"bdvideo","video.baidu.com"},
    {en_PROTO_ID_KUWO,"kuwo","kuwo.cn"},
    {en_PROTO_ID_MAX,"baidu","baidu.com"}
};


int trie_nmatches = 0;

int match( unsigned id, int index, void * data )
{
   trie_nmatches++;
 //  data = data;
   printf("id=%d found at index=%d\n",id,index);
   return 0;
}

/*
*
*/
int main( int argc, char ** argv )
{
    int i;
    KTRIE_STRUCT * ts;
    int nocase=1;  // don't care about case

    ts = KTrieNew();
/*
    if( argc < 3 )
    {
        printf("%s text pat1 pat2 ... patn [-c(ase-sensitive)\n",argv[0]);
        printf("search for keywords-default, or match keywords\n");
        exit(0);
    }
*/
    // for(i=1;i<argc;i++)
    // {
    //    if( strcmp(argv[i],"-c")==0 ) nocase=0; /* ignore case */
    // }

  int num = sizeof(g_urlpattern)/sizeof(URLPATTERN);
  printf("%d\n",num);


    printf("New TRIE created\n");
/*
    for(i=2;i<argc;i++)
    {
       if( argv[i][0]=='-' )
           continue;

       KTrieAddPattern( ts, (unsigned char *)argv[i], strlen(argv[i]), nocase, i );
    }
*/
unsigned char text[] = {"suggestion.baidu.com/su?wd=&json=1&p=3&sid=1426_7477_18280_18534_17943_18545_15827_11858_18099_18016&req=2&bs=%E6%98%93%E8%B4%AD%E7%BD%91&csor=0&cb=jQuery110204447600981220603_1451039615469&_=1451039615470"};

  for (i = 0; i < num; ++i)
  {
   
    KTrieAddPattern( ts, (unsigned char *)g_urlpattern[i].host,strlen(g_urlpattern[i].host),(void *)g_urlpattern[i].proto_name, nocase, g_urlpattern[i].id);
  }

     // KTrieAddPattern( ts, (unsigned char *)"baidu.com", strlen("baidu.com"), (void *)"baidu",nocase, 0);
     //  KTrieAddPattern( ts, (unsigned char *)"du.com", strlen("du.com"), (void *)"du",nocase, 1 );
    printf("Patterns added \n");

    KTrieCompile( ts );

    printf("Patterns compiled \n");
    printf("--> %d characters, %d patterns, %d bytes allocated\n",ts->nchars,ts->npats,ts->memory);

    printf("Searching...\n");

    KTrieSearch( ts, (unsigned char*)text, strlen(text), match, 0 );

    printf("%d matches found\n",trie_nmatches);

    printf("normal pgm finish.\n");

    return 0;
}

#endif

