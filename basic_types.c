#include "basic_types.h"

char TYPE_STRINGS[20][20]={"LIST","FUNCTION","LAMBDA","MACRO","ATOM","ARG","INTEGER","DOUBLE","STRING","STREAM"};

node_t create_atom(char* key){
	node_t n;
	n=ALLOC(node_,1);
	n->key=key;
	n->type=ATOM;
	(n->value).svalue=key;
	n->next=0;
	return n;
}

node_t create_list(node_t child){
	node_t n;
	n=ALLOC(node_,1);
	n->key=TYPE_STRINGS[LIST];
	n->type=LIST;
	(n->value).child=child;
	n->next=0;
	return n;
}

node_t copy_node_props(node_t src,node_t dst){
	if(!src||src==dst){ return dst; }
	if(!dst){ dst=ALLOC(node_,1); }
	if(dst==src) return src;
	dst->key=src->key;
	dst->type=src->type;
	memcpy(&(dst->value),&(src->value),sizeof(value_t));
	return dst;
}

node_t map_children(meta_func func,node_t child,node_t extra){
	while(child){
		copy_node_props(child,func(child,extra));
		child=child->next;
	}
	return child;
}

node_t copy_node(node_t src,node_t dst){
	node_t child,dchild;
	dst=copy_node_props(src,dst);
	if(src->type!=LIST){ return dst; }
	child=(src->value).child;
	if(child){
		dchild=copy_node(child,0);
		(dst->value).child=dchild;
		while(child=child->next){ dchild=(dchild->next=copy_node(child,0));	}
	}
	return dst;
}

node_t destroy_node(node_t n){
	node_t child;
	if(n->type==LIST){
		child=(n->value).child;
		while(child){ child->next=destroy_node(child->next); }
	}
	free(n);
	return 0;
}

node_t node2stream_(node_t node,node_t extra){
	char* sep;
	value_t value;
	if(!node){fprintf(ostream,"NIL"); return 0;}
	sep=(extra)?((extra->value).svalue):""; /* init the value of `sep' */
	value=node->value;
#ifdef DEBUG
	fprintf(ostream,"%s:",TYPE_STRINGS[node->type]);
#endif
	switch(node->type){
		case LIST:
		case LAMBDA:
		case MACRO:
			fprintf(ostream,"(%s",sep);
			map_children(node2stream_,value.child,extra);
			fprintf(ostream,")%s",sep);
			break;
		case INTEGER:
			fprintf(ostream,"%d%s",value.ivalue,sep);
			break;
		case DOUBLE:
			fprintf(ostream,"%f%s",value.dvalue,sep);
			break;
		case STRING:
			pretty_print_string(value.svalue,ostream);fprintf(ostream,"%s",sep);
			/*fprintf(ostream,"\"%s\"%s",value.svalue,sep);*/
			break;
		case ATOM:
			fprintf(ostream,"%s%s",node->key,sep);
			break;
		default: 
			fprintf(ostream,"[%s|%s]%s",node->key,TYPE_STRINGS[node->type],sep);
			break;
	}
	return node;
}

node_t node2stream(node_t node,FILE* stream){
	ostream=stream?stream:stdout;
	return node2stream_(node,create_atom(" "));
}

node_t make_stack(node_t bottom){
	return create_list(bottom);
}

node_t push_stack(node_t stack,node_t element){
	node_t top;
	top=(stack->value).child;
	if(element){
		(stack->value).child=element;
		element->next=top;
	}
	return stack;
}

node_t pop_stack(node_t stack){
	node_t top;
	top=(stack->value).child;
	if(top){
		(stack->value).child=top->next;
	}
	return top;
}

node_t get_top(node_t stack){
	return (stack->value).child;
}
