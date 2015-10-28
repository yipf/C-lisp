
#include "str.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static string_node_t STRINGS;
static index_t NUM;

index_t key2hash(char* key,index_t num){
	index_t hash,len;
	hash=0;
	len=strlen(key);
	while(len>0){
		hash+=key[len-1];
		while(hash>=num) {hash-=num;}
		len>>=1;
	}
	return hash;
}

int init_unique_strings(index_t num){
	NUM=num?num:11;
	STRINGS=calloc(num,sizeof(string_node_));
	return 0;
}

char* copy_string(char* src, index_t n){
	char *dst;
	dst=calloc(n+1,sizeof(char));
	return memcpy(dst,src,n);
}

char* unique_string(char* string,int copy){
	string_node_t head,node;
	head=STRINGS+key2hash(string,NUM);
	node=head->next;
	while(node&&strcmp(string,node->key)){node=node->next;}
	if(node){		return node->key; }
	node=calloc(1,sizeof(string_node_));
	if(copy){ 	string=copy_string(string,strlen(string)); 	}
	node->key=string; 	node->next=head->next; 	head->next=node;
	return string;
}

int print_all_strings(void){
	string_node_t head;
	char* key;
	index_t i;
	for(i=0;i<NUM;i++){
		head=STRINGS+i;
		while(head=head->next){ 	key=head->key;	printf("\n%p\t->\t%s",key,key);	}
	}
	return 0;
}
