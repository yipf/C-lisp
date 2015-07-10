#include "hash_table.h"

hash_table_t create_hash_table(unsigned int num){
	hash_table_t ht;
	node_t* data;
	unsigned int i;
	num=(num>0)?num:101;
	ht=ALLOC(hash_table_,1);
	data=ALLOC(node_t,num);
	for(i=0;i<num;++i){ 	data[i]=create_node(0); 	}
	ht->data=data;
	ht->num=num;
	return ht;
}

unsigned int key2hash(hash_key_t key,hash_table_t ht){
	unsigned int hash,num,index;
	char ch;
	index=strlen(key);
	num=ht->num;
	hash=0;
	while(index>0){
		hash+=key[index-1];
		while(hash>=num) hash-=num;
		index>>=1;
	}
	return hash;
}

static node_t get_prev(hash_table_t ht,hash_key_t key){
	node_t prev,next;
	prev=(ht->data)[key2hash(key,ht)];
	while(next=prev->next){
		if(strcmp(key,next->key)==0) return prev;
		prev=next;
	}
	return prev;
}

node_t get_hash_table(hash_table_t ht,hash_key_t key){
	node_t value;
	value=get_prev(ht,key)->next;
	return value?get_top(value):value;
}

int set_hash_table(hash_table_t ht,hash_key_t key,node_t value){
	node_t prev,next;
	prev=get_prev(ht,key);
	value=copy_node_props(value,0);
	if(next=prev->next){ /* if the key has been registered */
		if(value){ /* if value!=0 then set the current node to value */
			push_stack(next,value);
		}else{ /* if value==0 then delete the  current value */
			pop_stack(next);
			if(get_top(next)==0){prev->next=next->next;}
		}
	}else{ /* if the key does not exist then add it to the hash table*/
		if(value){
			next=make_stack(value);
			next->key=key;
			prev->next=next;
		}
	}
	return 0;
}

int print_hash_table(hash_table_t ht,node_t sep){
	unsigned int num,i;
	node_t* data,target;
	num=ht->num;	data=ht->data;
	for(i=0;i<num;++i){
		target=data[i]->next;
		while(target){
			printf("\n[%s: ",target->key); 	node2stream((target->value).child,stdout); 	printf("]");
			target=target->next;
		}
	}
	return 0;
}