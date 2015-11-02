
#include "core.h"

int main(){
	node_t n;
	init_unique_strings(101); /* for parse */
	init_hash_table(101);/* for eval */
	init_functions();
	
	LOCK(0);
	
	n=stream2node(stdin);
	MSG(count_nodes(0),"%d\t(first gc)\n")
	node2stream(n,stdout);
	printf("\n=\n");
	n=eval_with_gc(n);
	node2stream(n,stdout);
	MSG(count_nodes(0),"%d\t(total nodes)\n")
	CLEAR(1);
	MSG(count_nodes(0),"%d\t(after clear nodes)\n")
	return 0;
}