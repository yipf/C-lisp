
#include "core.h"

int main(){
	node_t n,e,top;
	init_unique_strings(101); /* for parse */
	init_hash_table(101);/* for eval */
	init_functions();
	LOCK(0);
	n=stream2node(stdin);
	GC(n);
	MSG(count_nodes(0),"%d\t(first gc)\n")
	node2stream(n,stdout);
	printf("\n=\n");
	e=new_list(mapf(eval_with_gc,n->child));
	node2stream(e,stdout);
	MSG(count_nodes(n),"%d\t(total nodes)\n")

	CLEAR(1);
	MSG(count_nodes(0),"%d\t(after clear nodes)\n")
	return 0;
}