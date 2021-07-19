#include"mm.h"

#define NALLOC 1024

union block_header{
    struct{
        union block_header *next_block;
        size_t size;
    }s;
    long x;
};

typedef union block_header Header;

static Header base;
static Header *freep=NULL;

static Header *morecore(size_t nu){
    char *cp,*sbrk(int);
    Header *up;
    if(nu<NALLOC) nu=NALLOC;
    cp=sbrk(nu*sizeof(Header));
    if(cp==(char*)-1) return NULL;
    up=(Header *) cp;
    up->s.size=nu;
    myfree((void *)(up+1));           // put up into free list for alloc to use
    return freep;
}

void *mymalloc(size_t size){
    Header *p,*prev;
    Header *morecore(size_t);
    size_t nunits;

    nunits=(size+sizeof(Header)-1)/sizeof(Header)+1;  // sbrk return nunits*sizeof(Header) bytes storages
    if((prev=freep)==NULL){
        base.s.next_block=freep=prev=&base;
        base.s.size=0;
    }
    for(p=prev->s.next_block;;prev=p,p=p->s.next_block){
        if(p->s.size >= nunits){
            if(p->s.size == nunits) prev->s.next_block=p->s.next_block;     // removed p from free list
            else{
                p->s.size-=nunits;
                p += p->s.size;
                p->s.size=nunits;
            }
            freep=prev;
            return (void *)(p+1);
        }
        if(p==freep){                       // 找一輪free list沒找到
            if((p=morecore(nunits))==NULL){
                return NULL;
            }
        }
    }
}

void myfree(void *ptr){
    Header *p,*bp;

    bp=(Header *)ptr-1;
    for(p=freep;!(bp>p && bp< p->s.next_block);p=p->s.next_block){
        if(p>=p->s.next_block &&(bp>p || bp<p->s.next_block)){
            break;
        }
    }
    if(bp+bp->s.size==p->s.next_block){
        bp->s.size+=p->s.next_block->s.size;
        bp->s.next_block=p->s.next_block->s.next_block;
    }
    else{
        bp->s.next_block=p->s.next_block;
    }
    if(p+p->s.size==bp){
        p->s.size+=bp->s.size;
        p->s.next_block=bp->s.next_block;
    }
    else{
        p->s.next_block=bp;
    }
    freep=p;
}

void *myrealloc(void *ptr,size_t size){
    Header *bp;
    bp=(Header *)ptr-1;

    size_t nunits;
    nunits=(size+sizeof(Header)-1)/sizeof(Header)+1;

    if(ptr==NULL) return mymalloc(size);
    
    if(bp->s.size>=nunits){     // size is enough
        return ptr;
    }
    else{
        void *np;
        np=mymalloc(size);
        if(np==NULL) return NULL;
        memcpy(np,ptr,(bp->s.size-1)*sizeof(Header));
        myfree(ptr);
        return np;
    }
}

void *mycalloc(size_t nmemb,size_t size){
    size_t tol=nmemb*size;
    void *np=mymalloc(tol);
    if(np==NULL) return NULL;
    memset(np,0,tol);
    return np;
}