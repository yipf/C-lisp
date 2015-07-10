
#include "basic_types.h"
#include "hash_table.h"
#include "core.h"
#include "built-in-functions.h"


 node_t add(node_t a, node_t sum){
     (sum->value).ivalue+=(a->value).ivalue;
     return a;
 }
 
 node_t create_number(int value){
     node_t n;
     n=create_atom("a");
     n->type=INTEGER;
     (n->value).ivalue=value;
     return n;
 }
 
 int main (int argc, char *argv[ ]){
     node_t node;
     int i;
     FILE* stream;
     init_env(101,10);
     init_functions();
     stream=(argc>1)?fopen(argv[1],"r"):stdin;
     node=stream2node(stream?stream:stdin);
     stream=(argc>2)?fopen(argv[2],"w"):stdout;
     node2stream(node,stream?stream:stdout);printf("\n=\n");
     node=eval(node,0);
     node2stream(node,stream?stream:stdout);printf("\n");
     return 0;
 }

/* 
 * int main(){
 * 	int i=0;
 * 	node_t arr,child,dup,sep;
 * 	hash_table_t ht;
 * 	
 * 	ht=create_hash_table(11);
 * 	
 * 	set_hash_table(ht,"a",create_atom("A"));
 *      set_hash_table(ht,"b",create_atom("B"));
 *      set_hash_table(ht,"c",create_atom("C"));
 *      set_hash_table(ht,"d",create_atom("D"));
 *      set_hash_table(ht,"e",create_atom("E"));
 *      set_hash_table(ht,"f",create_atom("F"));
 *      set_hash_table(ht,"g",create_atom("G"));
 *      set_hash_table(ht,"cc",create_atom("G"));
 * 
 * 
 * 	
 * 	set_hash_table(ht,"c",0);
 * 	set_hash_table(ht,"a",0);
 * 	set_hash_table(ht,"a",0);
 * 	
 * 	printf("\n");print_hash_table(ht,sep);
 *  	
 *    printf("\n");print_node(get_hash_table(ht,"cc"),0);
 * 
 *    printf("\n");print_node(get_hash_table(ht,"a"),0);
 * 
 *    printf("\n");print_hash_table(ht,0);
 *   printf("\n");
 *   return 0;
 *   
 * } 
 */
