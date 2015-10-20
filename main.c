#include "core.h"

int main(void){
	init_env(101);
	node_t n1,n2,n3;
	n1=string2node("3.14");
	n2=string2node("a");

	n3=string2node("b");  
	printf("\n");	node2stream(n1,stdout);
	printf("\n");	node2stream(n2,stdout);

	n2=make_pair(n2,n3);
	printf("\n");	node2stream(make_pair(n1,n2),stdout);
	n2=new_list(make_pair(n1,n2));
	printf("\n");	node2stream(n2,stdout);
	n3=string2node("a");
	n1=new_list(make_pair(n1,n3));
	printf("\n n1");	node2stream(n1,stdout);
	printf("\nCAR ");	node2stream(CAR(n2),stdout);
	printf("\nCDR ");	node2stream(CDR(CDR(n1)),stdout);
	return 0;
}