#include "environment.h"

static node_t* ENV=0;
static unsigned int ENV_COUNT=0;

unsigned int init_environment(unsigned int count){
	unsigned int i;
	count=count?count:101;
	ENV_COUNT=count;
	ENV=ALLOC(node_t,count);
	for(i=0;i<count;i++){		ENV[i]=create_atom(0); 	}
	return count;
}

static node_t get_prev(char* key){
	node_t prev,next;
	prev=ENV[key2hash(key,ENV_COUNT)];
	while(next=prev->next){
		if(key==next->key) return prev;
		prev=next;
	}
	return prev;
}

node_t get_value(char* key){
	node_t stack;
	stack=get_prev(key)->next;
	return stack?get_top(stack):0;
}

node_t set_value(char* key, node_t value){
	node_t prev,stack,top;
	prev=get_prev(key);
	stack=prev->next;
	if(stack){ /* if found, replace the top element with value */
		top=get_top(stack);
		copy_node_props(value,top);
	}else{/* else, add the value and return  */
		top=copy_node_props(value,0);
		stack=make_stack(top);
		stack->key=key;
		stack->next=prev->next;
		prev->next=stack;
	}
	return top;
}

node_t  push_value(char* key, node_t value){
	node_t prev,stack,top;
	prev=get_prev(key);
	stack=prev->next;
	if(stack){ /* if found, push the value to the corresponding element  */
		top=copy_node_props(value,0);
		push_stack(stack,top);
	}else{/* else, generate a new stack and add it to prev  */
		top=copy_node_props(value,0);
		stack=make_stack(top);
		stack->key=key;
		stack->next=prev->next;
		prev->next=stack;
	}
	return top;
}

node_t pop_value(char* key){
	node_t prev,stack,top;
	stack=get_prev(key)->next;
	if(stack){ /* if found, popup the top value and return it; */
		top=pop_stack(stack);
		if(!get_top(stack)){/* if the stack is empty, then remove it. */
			prev->next=stack->next;
			free(stack);
		}
		return top;
	}
	return 0;
}