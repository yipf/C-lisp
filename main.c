
#include "core.h"

int main(){
	node_t n;
	init_unique_strings(101); /* for parse */
	init_hash_table(101);/* for eval */
	init_functions();
	
	n=stream2node(stdin);
	node2stream(n,stdout);
	MSG(count_nodes(0),"%d\t(total nodes)")
	printf("\n=\n");
	node2stream(eval(n),stdout);

	MSG(count_nodes(0),"%d\t(total nodes)")
	POP(0,1);
	MSG(count_nodes(0),"%d\t(after eval nodes)\n")
	return 0;
}