#include "core.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static char BUFFER[100]={'\0'};
static char QUOTE[]="quote";
static size_t count=0;
static char string_stop_flag='\0';


int init_env(size_t num,size_t snum){
	init_environment((num>0)?num:199);
	init_strings((num>0)?num:199);
	return 0;
}

size_t append_char(char ch,char buffer[],size_t count){
	ch=(ch==EOF)?'\0':ch;
	buffer[count]=ch;
	return count+1;
}

node_t gen_atom_node(char buffer[],size_t count,int inside_string,int quote){
	if(count==0){ return 0; }
	node_t node;
	value_t value;
	char *str_end;
	buffer[count]=string_stop_flag;
	node=create_atom(0);
	if(inside_string){
		buffer=global_string(buffer);
		node->type=STRING;
		node->key=buffer;
		 (node->value).svalue=buffer; 
		return quote_node(node,quote);
	}
	/* test if string is an integer number */
	value.ivalue=strtol(buffer,&str_end,10);
	if((*str_end)==string_stop_flag){ 	
		node->type=INTEGER;
		(node->value).ivalue=value.ivalue;
		return quote_node(node,quote);
	}
	/* test if string is a double number */
	value.dvalue=strtod(buffer,&str_end);
	if((*str_end)==string_stop_flag){
		node->type=DOUBLE;
		(node->value).dvalue=value.dvalue;
		return quote_node(node,quote);
	}
#ifdef DEBUG
	SHOW(ENV,"%p")
#endif
	/* otherwise, return a symbol */
	buffer=global_string(buffer);
#ifdef DEBUG
	SHOW(buffer,"%p")
#endif
	node->type=ATOM;
	node->key=buffer;
	(node->value).svalue=buffer;
	return quote_node(node,quote);
}

node_t quote_node(node_t node,int quote){
	node_t child;
	while((quote--)>0){
		child=create_atom(QUOTE);
		append_node(child,node);
		node=create_list(child);
	}
	return node;
}

node_t append_node(node_t prev,node_t next){
	if(next){
		prev->next=next;
		return next;
	}
	return prev;
}

node_t node2list(node_t node){  /* change a node to list. e.g.  a->b->c  --> a=(b c)  */
	node->type=LIST;	
	(node->value).child=node->next; 	
	node->next=0;
	return node;
}

node_t stream2node(FILE* stream){
	node_t first,prev,node;
	char ch;
	int inside_string,quote_count;
	inside_string=0;	/* flag to indicate if current parse is inside a string quoted by "  */
	quote_count=0;
	first=create_atom(0);
	prev=first;
	while((ch=fgetc(stream))!=EOF){ 	/* main loop */
		if(inside_string){ 	/* if a string is parsing outside a " block */
			switch(ch){
				case '\\':
					count=append_char(fgetc(stream),BUFFER,count);
					break;
				case '\"': 	/* exit the string */
					if(count>0){prev=append_node(prev,gen_atom_node(BUFFER,count,inside_string,quote_count)); count=0; quote_count=0;}
					inside_string=0;
					break;
				default: 
					count=append_char(ch,BUFFER,count);
					break;
			}
		}else{
			switch(ch){
				case ';':
					if(count>0){prev=append_node(prev,gen_atom_node(BUFFER,count,inside_string,quote_count)); count=0; quote_count=0;}
					while((ch=fgetc(stream))!='\n');
					break;
				case '`':
					if(count>0){prev=append_node(prev,gen_atom_node(BUFFER,count,inside_string,quote_count)); count=0; quote_count=0;}
					quote_count++;
					break;
				case '\\':
					count=append_char(fgetc(stream),BUFFER,count);
					break;
				case '(':
					if(count>0){prev=append_node(prev,gen_atom_node(BUFFER,count,inside_string,quote_count)); count=0; quote_count=0;}
					prev=append_node(prev,quote_node(stream2node(stream),quote_count));
					count=0; quote_count=0;
					break;
				case ')':
					if(count>0){prev=append_node(prev,gen_atom_node(BUFFER,count,inside_string,quote_count)); count=0; quote_count=0;}
					return node2list(first);
					break;
				case '\"': 	/* enter the string */
					if(count>0){prev=append_node(prev,gen_atom_node(BUFFER,count,inside_string,quote_count)); count=0; quote_count=0;}
					inside_string=1;
					break;
				default: 
					if(isspace(ch)){
						if(count>0){prev=append_node(prev,gen_atom_node(BUFFER,count,inside_string,quote_count)); count=0; quote_count=0;}
					}else{
						count=append_char(ch,BUFFER,count);
					}
					break;
			}
		}
	}
	if(count>0){prev=append_node(prev,gen_atom_node(BUFFER,count,inside_string,quote_count)); count=0; quote_count=0;}
	return node2list(first);
}


int pretty_print_string(char * str,FILE* stream){
	char ch;
	stream=stream?stream:stdout;
	fputc('\"',stream);
	while(ch=*(str++)){
		switch(ch){
			case '\"':
				fprintf(stream,"\\\"");
				break;
			default: 
				fputc(ch,stream);
				break;
		}
	}
	fputc('\"',stream);
	return 0;
}

node_t push_args(node_t args,node_t inputs){
	while(args&&inputs){
		push_value(args->key,inputs);
		args=args->next;
		inputs=inputs->next;
	}
	return args;
}

node_t pop_args(node_t args,node_t end){
	while(args&&(args!=end)){
		pop_value(args->key);
		args=args->next;
	}
	return end;
}

node_t apply_function(node_t func,node_t arg){
	printf("\napply function:");
	SHOW_NODE(func);
	return (func->value).func(arg);
}

node_t apply_lambda(node_t l,node_t inputs){
	node_t child,args,body,last;
	printf("\napply lambda:");
	SHOW_NODE(l);
	child=(l->value).child;
	args=child->next;
	body=args->next;
	args=(args->value).child;
	if((last=push_args(args,inputs))==0){ /*if every args are all initiated */
		child=eval(copy_node(body,0));
	}
	pop_args(args,last); /* restore default ENV */
	return child;
}

node_t eval(node_t node){
	node_t var,args,body,inputs,last;
	value_t value;
	if(node){
		value=node->value;
		switch(node->type){
			case ATOM:
				SHOW(node->key,"%p")
				SHOW_NODE(get_value(node->key))
				return get_value(node->key);
				break;
			case LIST: 
				var=(node->value).child;
				if(var&&var->key==QUOTE){return var->next;};
				while(var){ copy_node_props(eval(var),var);	var=var->next; } /* eval every elements in the list */
				var=(node->value).child;
				if(!var){ break; }
				SHOW_NODE(var)
				switch(var->type){
					case FUNCTION:
						return apply_function(var,var->next);
						break;
					case LAMBDA:
						return apply_lambda(var,var->next);
						break;
					default: 
						break;
				}
				break;
			default:
				break;
		}
	}
	return node;
}



