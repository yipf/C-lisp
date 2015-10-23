#include "node.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

mem_t push_ptr(mem_t head,void* ptr){
	mem_t cur;
	cur=calloc(1,sizeof(mem_));
	cur->ptr=ptr;
	cur->next=head;
	return cur;
}

static mem_t VALUES=0;

value_t alloc_value(int type,void* ptr){
	value_t value=calloc(1,sizeof(value_));
	value->type=type;
	value->ptr=ptr;
	REGISTER_PTR(VALUES,value)
	return value;
}

static mem_t NODES=0;

node_t alloc_node(value_t car,node_t cdr){
	node_t node=calloc(1,sizeof(node_));
	node->car=car; 	node->cdr=cdr;
	REGISTER_PTR(NODES,node)
	return node;
}

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

char* unique_string(char* string){
	string_node_t head,node;
	head=STRINGS+key2hash(string,NUM);
	node=head->next;
	while(node&&strcmp(string,node->key)){node=node->next;}
	if(node){		return node->key; }
	node=calloc(1,sizeof(string_node_));
	node->key=string; 	node->next=head->next; 	head->next=node;
	return string;
}

node_t new_node(int type,char* string){
	value_t value;
	char* str_end;
	value=alloc_value(type,0);
	if(string){ /* define with a string */
		value->number=strtod(string,&str_end); /* test if `string' is a number */
		if(*str_end){ /* failed */
			value->string=string;		
		}else{ /* success */
			value->type=NUMBER; 
		}
	}
	return alloc_node(value,0);
}

node_t node2stream(node_t node,FILE*stream){
	value_t value;
	if(!node){ return 0;}
	value=node->car;
	if(value){
		switch(value->type){
			case LIST:				fprintf(stream,"( ");	node2stream(value->ptr,stream);			fprintf(stream,") ");		break;
			case NUMBER:	 	fprintf(stream,"%lf ",value->number);																		break;
			case STRING:			fprintf(stream,"\"%s\"",value->string);																		break;
			case SYMBOL:		fprintf(stream,"%s ",value->string);																			break;
			case FUNCTION:		fprintf(stream,"<function:%p> ",value->ptr);															break;
			case LAMBDA:		fprintf(stream,"LAMBDA: "); node2stream(value->ptr,stream);								break;
			default: 			break;
		}
	}else{ fprintf(stream,"NIL "); }
	if(node=node->cdr){node2stream(node,stream);}
	return node;
}

static char *BUFFER=0;
static unsigned LEN=0;

int init_buffer(unsigned n){
	LEN=n>0?n:1;
	BUFFER=calloc(LEN,sizeof(char));
	return 0;
}

char* new_string(char* string,unsigned n){
	char *str;
	str=calloc(n+1,sizeof(char));
	memcpy(str,string,n);
	string=unique_string(str);
	if(string!=str){ free(str); }
	return string;
}

node_t quote_node(node_t node,int quote){
	value_t value;
	while(quote--){
		value=alloc_value(SYMBOL,0);
		value->string=unique_string("quote");
		node=alloc_node(value,node);
		node=alloc_node(alloc_value(LIST,node),0);
 	}
	return node;
}

node_t stream2node(FILE*stream){
	node_t head,cur;
	value_t value;
	char ch;
	unsigned  n;
	int inside_string,quote;
	head=alloc_node(0,0);
	cur=head;
	n=0;	inside_string=0; 	quote=0;
	while((ch=fgetc(stream))!=EOF){
		if(ch=='\\'){ 			BUFFER[n++]=fgetc(stream);			continue;		}
		if(inside_string){
			if(ch=='"'){
				if(n>0){ cur->cdr=quote_node(new_node(STRING,new_string(BUFFER,n)),quote);	cur=cur->cdr;	  n=0;	quote=0;}
				inside_string=0;
			}else{				BUFFER[n++]=ch;			}			
			continue;
		}
		if(ch==')'){ break;} /* return list */
		switch(ch){
			case ';':  /* start a commit line */
				if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,new_string(BUFFER,n)),quote);	cur=cur->cdr;		n=0;	quote=0;}
				while((ch=fgetc(stream))!='\n'&&ch!=EOF);
				break;
			case '"': /* start a string */
				if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,new_string(BUFFER,n)),quote);	cur=cur->cdr;		n=0;	quote=0;}
				inside_string=1;
				break;
			case '(':  /* start a list */
				if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,new_string(BUFFER,n)),quote);	cur=cur->cdr;		n=0;	quote=0;}
				cur->cdr=stream2node(stream);		cur=cur->cdr; 
				break;
			case '`':  /* start a quote */
				if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,new_string(BUFFER,n)),quote);	cur=cur->cdr;		n=0;	quote=0;}
				quote++;
				break;
			default: 
				if(isspace(ch)){ /* if space */
					if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,new_string(BUFFER,n)),quote);	cur=cur->cdr;		n=0;	quote=0;}
				}else{					BUFFER[n++]=ch;				}
				break;
		}
	}
	if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,new_string(BUFFER,n)),quote);	cur=cur->cdr;		n=0;	quote=0;}
	value=alloc_value(LIST,head->cdr); 
	head->car=value; 	head->cdr=0;
	return head;
}
