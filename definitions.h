/* version 16.10.2018 */

#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED


/* files names */
#define DRAM "dram"
#define QRAM "qram"
#define STORE1 "store_1"
#define STORE2 "store_2"

#define NUM_OF_MEM_READS 4
#define NUM_OF_MEM_WRITES 2
#define MAX_NUM_LOADS 4

/* wires definitions */
#define WIRE_STORE "w_store"
#define LOAD_VALUE "load_val"
#define MEM_LOAD_IN_WIRE "w_load_mem_in"
#define MEM_STORE_ADD_IN_WIRE "w_store_add_mem_in"
#define MEM_STORE_VAL_IN_WIRE "w_store_val_mem_in"
#define CLOCK "clk"

#define EMPTY_STRING ""
#define MEMORY "memory"
#define DUALRAM "dualRAM"
#define INT_10 "`int_10"
#define WIDTH "`int_32"
#define INT_64 "`int_64"
#define LINES "`int_1024"

#define MAX_LEN 20
#define MODULE_LENGTH 7
#define MAX_HEADER_LENGTH 500

#define _LABEL_STR "label= "
#define LINE_LENGTH 256

#define REG "reg"
#define LOAD "Load"
#define STORE "Store"
#define STATE "state"
#define NEXT "next"
#define RESET "reset"
#define CLK "clk"
#define INT_SIZE "`int_size"
#define INT_0 "`int_0"
#define INT_1 "`int_1"
#define INT_2 "`int_2"
#define INT_10 "`int_10"
#define INT_32 "`int_32"
#define INT_64 "`int_64"
#define WIRE_SIZE wire_size
#define MAX_OPERAND_SIZE 256
#define MAX_NUM_LOADS 4
#define MAX_NUM_STORE 2
#define MAX_ARRAYS 5

#define MAX_NUM_LOADS 4
#define MAX_NUM_STORE 2
#define _MUX mux

#define MAX_PORT_NUM 5
#define MAX_CHAR_PER_PORT 50
#define MAX_CHAR_PER_CLUSTER 50
#define MAX_CHAR_PER_PORT_ADDRESS 12



char* port_attr[] = { "load_val_", "load_add_", "store_val_", "store_add_" };
typedef enum states_num {_default, _entry, _for_body, _for_end } states_num;
typedef enum port_attr_num {_load_val, _load_add, _store_val, _store_add } port_attr_num;
typedef enum cluserorder { entry = 1, for_body = 2, for_end = 3 } clusterorder;

#endif
