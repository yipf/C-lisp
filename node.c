#include "node.h"

char TYPE_STRINGS[20][20]={"LIST","FUNCTION","LAMBDA","MACRO","ATOM","ARG","INTEGER","DOUBLE","STRING","STREAM"};


node_t create_node(char* key,int type, value_t value, node_t next){
	node_t n;
	n=ALLOC(node_,1);
	n->key=key;
	n->type=type;
	memcpy(&(n->value),&(value),sizeof(value_t));
	n->next=next;
	return n;
}

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

node_t node2stream_(node_t node){
	value_t value;
	if(!node){fprintf(ostream,"NIL"); return 0;}
	value=node->value;
#ifdef DEBUG
	fprintf(ostream,"%s:",TYPE_STRINGS[node->type]);
#endif
	switch(node->type){
		case LIST:
		case LAMBDA:
		case MACRO:
			fprintf(ostream,"( ");
			node=value.child;
			while(node){
				node2stream_(node);
				node=node->next;
			}
			fprintf(ostream,") ");
			break;
		case INTEGER:
			fprintf(ostream,"%d ",value.ivalue);
			break;
		case DOUBLE:
			fprintf(ostream,"%f ",value.dvalue);
			break;
		case STRING:
			pretty_print_string(value.svalue,ostream);fprintf(ostream," ");
			/*fprintf(ostream,"\"%s\"%s",value.svalue,sep);*/
			break;
		case ATOM:
			fprintf(ostream,"%s ",node->key );
			break;
		default: 
			fprintf(ostream,"%s[%s] ",node->key,TYPE_STRINGS[node->type]);
			break;
	}
	return node;
}

node_t node2stream(node_t node,FILE* stream){
	ostream=stream?stream:stdout;
	return node2stream_(node);
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
