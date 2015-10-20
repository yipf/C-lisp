#include "value.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static value_t HEAD=0;

value_t push_value(value_t head,value_t value){
	value->next=head;
	return value;
}

value_t pop_value(value_t head,int pop){
	head=(head&&pop)?(head->next):head;
	return head;
}

value_t free_values(value_t head){
	if(head){
		head->next=free_values(head->next);
		free(head->key);
		free(head);
	}
	return head;
}

value_t new_value(char* key){
	value_t value;
	value=calloc(1,sizeof(value_));
	value->key=key;
	if(HEAD){	value->next=HEAD;	}
	HEAD=value;
	return value;
}

int init_env(hash_size_t num){
	NUM=num?num:11;
	HASH=calloc(num,sizeof(hash_node_));
	return 0;
}

hash_node_t create_hash_node(char*key){
	hash_node_t node;
	node=calloc(1,sizeof(hash_node_));
	node->key=key;
	return node;
}

hash_size_t key2hash(char* key,hash_size_t num){
	hash_size_t hash,len;
	hash=0;
	len=strlen(key);
	while(len>0){
		hash+=key[len-1];
		while(hash>=num) hash-=num;
		len>>=1;
	}
	return hash;
}

hash_node_t get_node(hash_node_t node,char* key){
	while(node && strcmp(node->key,key)){	node=node->next; 	}
	return node;
}

value_t hash_push_value(char* key,value_t value){
	hash_node_t head,node;
	head=HASH+key2hash(key,NUM);
	node=get_node(head->next,key);
	if(!node){
		node=create_hash_node(key);
		node->next=head->next;
		head->next=node;
	}
	node->top=push_value(node->top,value);
	return value;
}

value_t hash_pop_value(char* key, int pop){
	hash_node_t node;
	value_t value;
	node=get_node((HASH+key2hash(key,NUM))->next,key);
	value=0;
	if(node){
		value=pop_value(node->top,pop);
		node->top=value;
	}
	return value;
}

value_t string2value(char* string){
	value_t value;
	char* str_end;
	if(string){
		value=hash_pop_value(string,0);
		if(value){ return value;	}
	}
	value=new_value(string);
	if(string){
		hash_push_value(string,value); /* regiester to hash table for search */
	/* fiil field via string */
		value->integer=strtol(string,&str_end,10);
		if(!(*str_end)){	value->type=INTEGER; return value; }
		value->number=strtod(string,&str_end);
		if(!(*str_end)){ 	value->type=NUMBER; return value; }
		value->type=ATOM;
	}
	return value;
}
