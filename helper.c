#include "helper.h"

unsigned int key2hash(char* key,unsigned int num){
	unsigned int hash,len;
	if(num<1) return 0;
	hash=0;
	len=strlen(key);
	while(len>0){
		hash+=key[len-1];
		while(hash>=num) hash-=num;
		len>>=1;
	}
	return hash;
}

char * copy_string(char* src, unsigned int len){
	char* dst;
	dst=ALLOC(char,len+1);
	memcpy(dst,src,len);
	dst[len]='\0';
	return dst;
}