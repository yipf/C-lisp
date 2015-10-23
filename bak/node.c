#include "core.h"

#include <stdio.h>      /* printf, scanf, puts */
#include <stdlib.h>      /* calloc  */
#include <string.h>      /* memcpy */
#include<ctype.h>    /* isspace */


static char NAMES[][10]={"NIL","LIST","SYMBOL","INTEGER","NUMBER","STRING","FUNCTION","LAMBDA"};

node_t new_node(value_t value,node_t cdr){
	node_t node;
	node=calloc(1,sizeof(node_));
	node->car=value;
	node->cdr=cdr;
	return node;
}

node_t string2node(char* string){
	return new_node(string2value(string),0);
}

node_t new_list(node_t child){
	value_t value;
	value=new_value(0);
	value->type=LIST;
	value->ptr=child;
	return new_node(value,0);
}

int value2stream(value_* value,FILE*stream){
	if(!value){
		fprintf(stream,"() ");
		return 0;
	}
	switch(value->type){
		case LIST:
			fprintf(stream,"( ");
			node2stream(value->ptr,stream);
			fprintf(stream,") ");
			break;
		case NUMBER:
			fprintf(stream,"%lf ",value->number);
			break;
		case INTEGER:
			fprintf(stream,"%ld ",value->integer);
			break;
		case STRING:
			fprintf(stream,"%s ",value->key);
			break;
		default: 
			fprintf(stream,"%s:%s ",NAMES[value->type],value->key);
			break;
	}
	return 0;
}

node_t node2stream(node_t node,FILE* stream){
	if(node){
		value2stream(node->car,stream);
		node2stream(node->cdr,stream);
	}
	return 0;
}

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
				if(n>0){ cur->cdr=string2node(new_string(BUFFER,n));	cur=cur->cdr;	 (cur->car)->type=STRING; n=0;	}
				inside_string=0;
			}else{				BUFFER[n++]=ch;			}			
			continue;
		}
		if(ch==')'){ break;} /* return list */
		switch(ch){
			case ';':  /* start a commit line */
				if(n>0){ cur->cdr=string2node(new_string(BUFFER,n));	cur=cur->cdr;		n=0;	}
				while((ch=fgetc(stream))!='\n'&&ch!=EOF);
				break;
			case '"': /* start a string */
				if(n>0){ cur->cdr=string2node(new_string(BUFFER,n));	cur=cur->cdr;		n=0;	}
				inside_string=1;
				break;
			case '(':  /* start a list */
				if(n>0){ cur->cdr=string2node(new_string(BUFFER,n));	cur=cur->cdr;		n=0;	}
				cur->cdr=stream2node(stream);				cur=cur->cdr; 
				break;
			default: 
				if(isspace(ch)){ /* if space */
					if(n>0){ cur->cdr=string2node(new_string(BUFFER,n));	cur=cur->cdr;		n=0;	}
				}else{					BUFFER[n++]=ch;				}
				break;
		}
	}
	if(n>0){ cur->cdr=string2node(new_string(BUFFER,n));	cur=cur->cdr;		n=0;	}
	cur=head->cdr;		free(head);
	return new_list(cur);
}


