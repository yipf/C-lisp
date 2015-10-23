#include "core.h"

int main(){
	node_t n;
	init_hash_table(101);
	init_buffer(100);
	init_unique_strings(101);
	n=stream2node(stdin);
	node2stream(n,stdout);
	printf("\n=\n");
	init_functions(); 
	node2stream(eval(n),stdout);
	printf("\n");
	return 0;
}