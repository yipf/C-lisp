#include "node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static node_t __STACK__=0;
static node_t __CURRENT__=0;

node_t PUSH(node_t top){ /* PUSH(0) return the top of current __STACK__ */
	if(top){	top->ref=1;	top->top=__STACK__; 	__STACK__=top;	}
	return __STACK__;
}

node_t CLEAR(int release){ /* CLEAR nodes until end, the top of stack is end. CLEAR(0) clear all nodes */
	node_t top;
	while((top=__STACK__)&&top){		__STACK__=__STACK__->top; 	if(release){free(top);}	}
	__STACK__=__STACK__?(__STACK__->top):__STACK__;		
	return release?0:top;
}

node_t LOCK(node_t node){
	return __CURRENT__=node?node:__STACK__;
}

node_t SWEEP(node_t top){
	node_t prev,cur;
	if(!top) return top;
	prev=top; 
	cur=prev->top;
	while(cur&&cur!=__CURRENT__){
		if(cur->ref>1){ 	/* if other referce this node */
			cur->ref--;
			prev=cur;
		}else{ /* if no reference, the free the node */
			prev->top=cur->top;
			free(cur);
		}
		cur=prev->top;
	}
	return top;
}


node_t MARK(node_t top){
	if(top){
		top->ref++;
		if(top->type==LIST){
			top=top->child;
			while(top){  MARK(top);	top=top->cdr;	}
		}
	}
	return top;
}

node_t GC(node_t node){
	MARK(node);
	SWEEP(node);
	//~ LOCK(node);
	return node;
}

index_t count_nodes(node_t end){
	node_t top;
	index_t count;
	top=__STACK__;
	count=0;
	while(top&&(top!=end)){
		count++;  
		printf("\n%d\t%d\t",count,top->ref);		node2stream(top,stdout);	
		top=top->top;
	}
	return count;
}

node_t alloc_node(int type){
	node_t node;
	node=calloc(1,sizeof(node_));
	node->type=type;
	PUSH(node); /* for gc */
	return node;
}

node_t new_node(int type,char* string){
	node_t node;
	char* str_end;
	node=alloc_node(type);
	if(type==STRING){		node->string=string;		return node;	}
	node->number=strtod(string,&str_end); /* test if `string' is a number */
	if(*str_end){ /* failed */
		node->string=string;		
	}else{ /* success */
		node->type=NUMBER; 
	}
	return node;
}

node_t copy_node(node_t src){
	node_t dst=alloc_node(NIL);
	if(src){ memcpy(dst,src,sizeof(node_)-2*sizeof(node_t));	}
	return dst;
}

node_t new_list(node_t child){
	node_t list,last;
	list=alloc_node(LIST);
	list->child=child;
	return list;
}

static char BUFFER[100];

char* buffer2string(index_t n){
	BUFFER[n]='\0';
	return unique_string(BUFFER,1);
}

node_t quote_node(node_t node,index_t quote){
	node_t head;
	while(quote--){
		head=new_node(SYMBOL,unique_string("quote",0));
		head->cdr=node;
		node=new_list(head);
	}
	return node;
}

node_t stream2node(FILE* stream){
	node_t head,cur;
	char ch;
	index_t  n,inside_string,quote;
	head=alloc_node(LIST);
	cur=head;
	n=0;	inside_string=0; 	quote=0;
	while((ch=fgetc(stream))!=EOF){
		if(ch=='\\'){ 			BUFFER[n++]=fgetc(stream);			continue;		}
		if(inside_string){
			if(ch=='"'){
				if(n>0){ cur->cdr=quote_node(new_node(STRING,buffer2string(n)),quote); 	cur=cur->cdr;	  n=0;	quote=0;}
				inside_string=0;
			}else{				BUFFER[n++]=ch;			}			
			continue;
		}
		if(ch==')'){ break;} /* return list */
		switch(ch){
			case ';':  /* head a commit line */
				if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,buffer2string(n)),quote); 	cur=cur->cdr;	  n=0;	quote=0;}
				while((ch=fgetc(stream))!='\n'&&ch!=EOF);
				break;
			case '"': /* head a string */
				if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,buffer2string(n)),quote); 	cur=cur->cdr;	  n=0;	quote=0;}
				inside_string=1;
				break;
			case '(':  /* head a list */
				if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,buffer2string(n)),quote); 	cur=cur->cdr;	  n=0;	quote=0;}
				cur->cdr=quote_node(stream2node(stream),quote);		cur=cur->cdr;  quote=0;
				break;
			case '`':  /* head a quote */
				if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,buffer2string(n)),quote); 	cur=cur->cdr;	  n=0;	quote=0;}
				quote++;
				break;
			default: 
				if(isspace(ch)){ /* if space */
					if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,buffer2string(n)),quote); 	cur=cur->cdr;	  n=0;	quote=0;}
				}else{					BUFFER[n++]=ch;				}
				break;
		}
	}
	if(n>0){ cur->cdr=quote_node(new_node(SYMBOL,buffer2string(n)),quote); 	cur=cur->cdr;	  n=0;	quote=0;}
	head->child=head->cdr; 	head->cdr=0;
	return head;
}

int node2stream(node_t node,FILE* stream){
	node_t last;
	if(!node) return 0;
	switch(node->type){
			case NIL:	 			fprintf(stream,"NIL ");											break;
			case NUMBER:	 	fprintf(stream,"%lf ",node->number);				break;
			case STRING:			fprintf(stream,"\"%s\" ",node->string);				break;
			case SYMBOL:		fprintf(stream,"%s ",node->string);					break;
			case FUNCTION:		fprintf(stream,"<function:%p> ",node->ptr);	break;
			case LAMBDA:		
			case MACRO:		
				fprintf(stream,(node->type==LAMBDA)?"<lambda: ":"<macro:"); 
				node2stream((node=node->child),stream);		
				node2stream((node->cdr),stream);		
				fprintf(stream,"> ");		
				break;
			case LIST:				
				fprintf(stream,"( ");	 
				if(node=node->child){ while(node){ node2stream(node,stream); node=node->cdr; }}
				fprintf(stream,") ");		
				break;
		default: break;
	}
	return 0;
}