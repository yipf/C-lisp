/* value */

#define SHOW(var,format) printf("\n%s:%d:\t%s\t:: ",__FILE__,__LINE__,#var);printf(format,var);

static char LIST[];
static char FUNCTION[];
static char STRING[];
static char NUMBER[];

typedef struct node_{
	union{
		double number;
		struct node_ *child;
		char* string;
		void* ptr;
	};
	char* key;
	struct node_ *cdr;
	struct node_ *next; /* for gc*/
};

static hash_node_t HEAD;







typedef char* hash_key_t;
typedef void* hash_value_t;
typedef unsigned int hash_size_t;

typedef struct hash_node_{
	hash_key_t key;
	hash_value_t value;
	struct hash_node_ *cdr;
	struct hash_node_ *next; /* for gc */
}hash_node_,*hash_node_t;

typedef struct hash_table_{
	hash_size_t hash;
	hash_node_t 
};

static hash_node_t HEAD;



hash_node_t create_hash_table(hash_size_t num);












enum{NIL=0,LIST=1,SYMBOL,INTEGER,NUMBER,STRING,FUNCTION,LAMBDA};

typedef struct value_{
	/* value part */
	union {
		double number;
		long int integer;
		void* ptr;
	};
	int type;
	char* key;
	struct value_ * next; /* for gc */
}value_,*value_t;

static value_t HEAD;

static value_t push_value(value_t head,value_t value);
static value_t pop_value(value_t head,int pop);
static value_t free_values(value_t head);

value_t new_value(char* key);

/* hash table */

typedef struct hash_node_{
	char* key;
	value_t top;
	struct hash_node_ * next;
}hash_node_,*hash_node_t;

typedef unsigned int hash_size_t;

static hash_size_t NUM;
static hash_node_* HASH;

int init_env(hash_size_t num);

static hash_node_t create_hash_node(char*key);
static hash_size_t key2hash(char* key,hash_size_t num);
static hash_node_t get_node(hash_node_t node,char* key);

value_t hash_push_value(char* key,value_t value);
value_t hash_pop_value(char* key, int pop);

value_t string2value(char* key);
