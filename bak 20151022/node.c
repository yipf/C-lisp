#include "node.h"

#include <stdio.h>  	/* io */
#include <stdlib.h>  	/* calloc */
#include <string.h>  	/* calloc */

struct node_;

typedef struct value_{
	union{
	};
	int type;
	struct value_ 
};

typedef struct node_{
	
};












/* basic */

static node_t HEAD=0;
static unsigned int ALL=0;

node_t alloc_node(char* key){
	node_t node=calloc(1,sizeof(node_));
	node->key=key;
	node->next=HEAD; 	ALL++;
	return HEAD=node;
}

int print_all(void){
	SHOW(ALL,"%d nodes are allocated.")
}

node_t clone_node(node_t src,node_t dst){
	if
	dst=dst?alloc_node(src->key);
	
	node_t dst=calloc(1,sizeof(node_));
	return HEAD=dst;
}

/* hash_table */

static node_t HASH;
static hash_size_t NUM;

node_t init_hash_table(hash_size_t num){
	NUM=num?num:11;
	HASH=calloc(num,sizeof(node_));
	return HASH;
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

node_t get_node(node_t node,char* key){
	while(node && strcmp(node->key,key)){	node=node->cdr; 	}
	return node;
}

node_t hash_push_value(char* key,node_t value){
	node_t head,target,nvalue;
	head=HASH+key2hash(key,NUM);
	target=get_node(head->cdr,key);
	if(!target){
		target=alloc_node(key);
		target->cdr=head->cdr;
		head->cdr=target;
	}
	nvalue=alloc_node(key);
	nvalue->child=value;
	node->child=push_value(node->child,nvalue);
	return value;
}

node_t hash_pop_value(char* key, int pop){
	node_t target,value;
	target=get_node((HASH+key2hash(key,NUM))->cdr,key);
	value=0;
	if(target&&(value=target->child)&&pop){		target->child=value->cdr; 	}
	return value?(value->child):0;
}

/* node */

static char LIST[]="LIST";
static char FUNCTION[]="FUNCTION"
static char STRING[]="STRING";
static char NUMBER[]="NUMBER";
static char LAMBDA[]="LAMBDA";

node_t string2node(char* string,int isstring){
	node_t node;
	char* str_end;
	node=hash_pop_value(string,0);
	if(!node){
		node=alloc_node(string);
		node->number=strtod(string,&str_end);		if(!(*str_end)){ 	node->key=NUMBER; return node; } /* test if it is a number */
		if(isstring){node->string=string;}
	}
	return node;
}

/* IO for nodes */

node_t node2stream(node_t node,FILE*stream){
	node_t child;
	char* key;
	switch(node->key){
		case LIST:
			fprintf(stream,"( ",key);	
			child=node->child; 	while(child){	node2stream(child,stream); 	child=child->cdr;	}
			fprintf(stream,") ",key);
			break;
		case NUMBER:	 	fprintf(stream,"%lf ",node->number);		break;
		case STRING:			fprintf(stream,"%s ",node->string);			break;
		case FUNCTION:		fprintf(stream,"<%s:%p> ",key,node->ptr);			break;
		case LAMBDA:		
			fprintf(stream,"%s: ",key);
			child=node->child; 	while(child){	node2stream(child,stream); 	child=child->cdr;	}
		break;
		default: 
			fprintf(stream,"SYMBOL:%s ",key);
			break;
	}
	return node;
}

static char *BUFFER;
static index_t LEN;

int init_buffer(index_t n){
	LEN=n>0?n:1;
	BUFFER=calloc(LEN,sizeof(char));
	return 0;
}

char* new_string(char* string,index_t n){
	char *str;
	str=calloc(n+1,sizeof(char));
	memcpy(str,string,n);
	return str;
}

node_t stream2node(FILE* stream){
	node_t head,cur;
	char ch;
	index_t n;
	int inside_string,quote;
	cur=head=new_node(0,0);
	n=0;	inside_string=0;
	while((ch=fgetc(stream))!=EOF){
		if(ch=='\\'){ 			BUFFER[n++]=fgetc(stream);			continue;		}
		if(inside_string){
			if(ch=='"'){
				if(n>0){ cur->cdr=string2node(new_string(BUFFER,n),1);	cur=cur->cdr;	 (cur->car)->type=STRING; n=0;	}
				inside_string=0;
			}else{				BUFFER[n++]=ch;			}			
			continue;
		}
		if(ch==')'){ break;} /* return list */
		switch(ch){
			case ';':  /* start a commit line */
				if(n>0){ cur->cdr=string2node(new_string(BUFFER,n),0);	cur=cur->cdr;		n=0;	}
				while((ch=fgetc(stream))!='\n'&&ch!=EOF);
				break;
			case '"': /* start a string */
				if(n>0){ cur->cdr=string2node(new_string(BUFFER,n),0);	cur=cur->cdr;		n=0;	}
				inside_string=1;
				break;
			case '(':  /* start a list */
				if(n>0){ cur->cdr=string2node(new_string(BUFFER,n),0);	cur=cur->cdr;		n=0;	}
				cur->cdr=stream2node(stream);				cur=cur->cdr; 
				break;
			default: 
				if(isspace(ch)){ /* if space */
					if(n>0){ cur->cdr=string2node(new_string(BUFFER,n),0);	cur=cur->cdr;		n=0;	}
				}else{					BUFFER[n++]=ch;				}
				break;
		}
	}
	if(n>0){ cur->cdr=string2node(new_string(BUFFER,n),0);	cur=cur->cdr;		n=0;	}
	cur=head->cdr;		free(head);
	return new_list(cur);
}
