/* version 16.10.2018 */

#ifndef MODULE_H_INCLUDED
#define MODULE_H_INCLUDED

#include "object.h"
#include "array.h"
#include "definitions.h"
#include "graph.h"
#include "utilities.h"

int medgew(graph*, int);
/*typedef enum states_num {_default, _entry, _for_body, _for_end } states_num;
typedef enum port_attr_num {_load_val, _load_add, _store_val, _store_add } port_attr_num;
*/

char* states[] = { "DEFAULT", "ENTRY", "FOR_BODY", "FOR_END" };
/*char* port_attr[] = { "load_val_", "load_add_", "store_val_", "store_add_" };*/
void mfindforreg(module*);






/** \brief: module domain, create a new module object
 *
 * \param: graph* _g, the graph object that holds the nodes and edges
 * \return: module* _result, the initialized module object
 *
 */
module* mnewModule(graph* _g)
{
    int _inCount = 0, _outCount = 0, _cCount = 0;               /* counters for input, output and constants in the module */
    int _state;                                                 /* indicate what state the node is in */
    int _i, _length;                                            /* running and max run time indexers */
    int _currentMemorySize = 0;                                 /* counts the size of all arrays in the module */
    int _arrSize = 0;                                           /* mark the size of an array in the module */
    int _cnum;                                                  /* temporary cluster number */
    char _firstLetter;                                          /* the first char of any a string */
    char *_name, *_clstrName;                                   /* holds a node name attr. */
    module* _result;                                            /* created module */
    edge* _e;                                                   /* an edge object pointer */
    node* _c;                                                   /* a cluster object pointer */
    node* _node;                                                /* current node, for iteration purposes */


    if( (_result = (module*)malloc(sizeof(module))) == NULL )
    {
		printf("cannot allocate module memory");
		exit(1);
	}
    /* initialize module parameters */
    _result->_numOfConstants = _result->_numOfInputs = _result->_numOfOutputs = 0;
    _result->_graph = _g;
    _result->_arraysList = newmArrayList(MAX_ARRAYS);
    _result->_numOfLoad = 0;
    _result->_numOfStore = 0;
    _result->_defaultOut = -1;
    _result->_load = lnewlnode();
    _result->_store = lnewlnode();
    _result->_return = lnewlnode();



    /* find number of: input, output and constant nodes are there, and also for_end condition */
    _length = _g->_nodes->_size;

    for( _i = 0; _i < _length; _i++ )
    {
        _node = _g->_nodes->_list[_i];
        _name = _node->_name;
        _clstrName = _node->_clstr->_name;
        _node->_state = c2i(_clstrName);
        /* for loop break condition */
        if( strcmp(_name, "condBR") == 0 && strcmp(_node->_clstr->_name, "for_body") == 0 )
            _result->_forendcon = _g->_nodes->_list[_i];
        /* foor loop start dondition */
        if( strcmp(_name, "condBR") == 0 && strcmp(_node->_clstr->_name, "entry") == 0 )
            _result->_forscon = _g->_nodes->_list[_i];

        else if( strcmp(_name, "condBR") == 0 || strcmp(_name, "BR") == 0 )
        {}

        else if( strcmp(_name, "Argument") == 0 )
        {
            _result->_numOfInputs++;
            _inCount++;
        }

        /* add loads and stores lnodes linked lists to module */
        else if( strcmp(_name, "Load") == 0 )
             _result->_numOfLoad = laddtail(_result->_load, _i, _result->_numOfLoad, _node->_state);

        else if( strcmp(_name, "Store") == 0 )
            _result->_numOfStore = laddtail(_result->_store, _i, _result->_numOfStore, _node->_state);

        else if( strcmp(_name, "RT") == 0 )
        {
            _result->_numOfOutputs = laddtail(_result->_return, _i, _result->_numOfOutputs, _node->_state);
            _outCount++;
        }

        else if( strcmp(_name, "Constant") == 0)
        {
            _firstLetter = *_node->_outEdgesName[0];
            /* find a lower case letter, i.e. a constant */
            if( _firstLetter >= 97 && _firstLetter <= 122 )
            {
                if( strstr(_node->_outEdgesName[0], "undef") )
                    memcpy(_g->_nodes->_list[_i]->_outEdgesName[0] + strlen("i32"), "_u", 2);
                else
                    memcpy(_g->_nodes->_list[_i]->_outEdgesName[0] + strlen("i32"), "_", 1);
            }

            /* find a capital letter, i.e. an array */
            else if( _firstLetter >= 65 && _firstLetter <= 90 )
            {
                _arrSize = 20;
                addmArray(_result->_arraysList, newmArray(_g->_nodes->_list[_i]->_outEdgesName[0], "int", _arrSize, _currentMemorySize));
                _currentMemorySize += _arrSize;
            }
            _result->_numOfConstants++;
            _cCount++;
        }
    }

    /* build the input output strings arrays */
    _result->_inputs = (char**)malloc(sizeof(char*) * _inCount);
    _result->_outputs = (char**)malloc(sizeof(char*) * _outCount);
    _result->_constants = (char**)malloc(sizeof(char*) * _cCount);
    _inCount = 0;
    _outCount = 0;
    _cCount = 0;

    for( _i = 0; _i < _length; _i++ )
    {
        _node = _g->_nodes->_list[_i];
        /* add an argument node to the module */
        if( strcmp(_node->_name, "Argument") == 0 )
        {
            _result->_inputs[_inCount] = createString("Module.newModule", strlen(_node->_outEdgesName[0]));
            strcpy(_result->_inputs[_inCount],  _node->_outEdgesName[0]);
            _inCount++;
        }
        /* add return node to the module */
        else if( strcmp(_node->_name, "RT") == 0 )
        {
            if( _node->_indegree != 0 ) _result->_defaultOut = _node->_number;
            _result->_outputs[_outCount] = createString("Module.newModule", strlen(_node->_outEdgesName[0]));
            strcpy(_result->_outputs[_outCount],  _node->_outEdgesName[0]);
            _outCount++;
        }
        /* add a constant node to the module */
        else if( strcmp(_node->_name, "Constant") == 0 )
        {
            _result->_constants[_cCount] = createString("Module.newModule", strlen(_node->_outEdgesName[0]));
            /* integer */
            if( *(_node->_outEdgesName[0]) == 'i' )
            {
                /* named 32 bit, actually 32 bit */
                if( *(_node->_outEdgesName[0] + 1)== '3' )
                    sprintf(_result->_constants[_cCount], "i32_%s", _node->_outEdgesName[0] + strlen("i32 "));
                else
                    sprintf(_result->_constants[_cCount], "i64_%s", _node->_outEdgesName[0] + strlen("i32 "));
            }
            /* other type */
            else
                sprintf(_result->_constants[_cCount], "%s", _node->_outEdgesName[0]);
            _cCount++;
        }
    }

    /* assign cluster class to each cluster */
    for( _i = 0; _i < _g->_clusters->_numOfObjects; _i++ )
    {
        _c = _result->_graph->_clusters->_list[_i];
        _cnum = gfindpc(_result->_graph, _c);
        switch(_cnum)
        {
        case 1:
            strcpy(_c->_name, "entry");
        break;
        case 2:
            strcpy(_c->_name, "for_body");
        break;
        case 3:
            strcpy(_c->_name, "for_end");
        break;
        }
    }

    /* assign width to each edge */
    for( _i = 0; _i < _g->_edges->_size; _i++ )
    {
        _e = _g->_edges->_list[_i];

        if( _e->source == -1 ) continue;
        while( _e != NULL )
        {
            if( _e->target == -1 ) break;
            _e->width = medgew(_g, _e->source);
            _e = _e->_next;
        }
    }
    /*mfindforreg(_result);
    lprintl("loads:", _result->_load);
    lprintl("stores:", _result->_store);
    lprintl("'for' registers:", _result->_forRlist);*/
    return _result;
}

/** \brief: module domain, free module object
 *
 * \param: module* _m, pointer to a module object
 * \return: int, 0 if module object was success, any other integer otherwise
 *
 */
int mfreeModule(module* _m)
{
    int _i;
    int _result = 0;

    for(_i = 0; _i < _m->_numOfInputs; _i++)
        free(_m->_inputs[_i]);

    for(_i = 0; _i < _m->_numOfOutputs; _i++)
        free(_m->_outputs[_i]);

    for(_i = 0; _i < _m->_numOfConstants; _i++)
        free(_m->_constants[_i]);


    if (gfreeGraph(_m->_graph) ) _result = 1;
    freemArrayList(_m->_arraysList);
    lfreelnode(_m->_return);
    lfreelnode(_m->_load);
    lfreelnode(_m->_store);
    return _result;
}


/** \brief: module domain, assign width to an edge specified by the node number that the edge goes out of,
 *          according to the name of the source node
 * \param: graph* _g, a pointer to the graph object
 * \param: int _sourceNodeNumber, the number of the node that edge goes out of
 * \return: int, the edge width
 *
 */

int medgew(graph* _g, int _sourceNodeNumber)
{
    int _result;
    node* _node = _g->_nodes->_list[_sourceNodeNumber];

    if( _node->_number == -1 ) _result = -1;
    else if( strcmp(_node->_name, "LESS-THAN") == 0 )
        _result = 1;
    else if( strcmp(_node->_name, "Constant") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "Argument") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "condBR") == 0 )
        _result = 1;
    else if( strcmp(_node->_name, "BR") == 0 )
        _result = 1;
    else if( strcmp(_node->_name, "Clocked Register") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "GEP") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "Load") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "MULTIPLY") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "ADD") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "TRUNC") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "ASSIGNMENT") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "RT") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "ICMPEQ") == 0 )
        _result = 1;
    else if( strcmp(_node->_name, "Load") == 0 )
        _result = 32;
    else if( strcmp(_node->_name, "Store") == 0 )
        _result = 32;

    return _result;
}



/** \brief: writes the input/output port declaration to the output .v file.
 *          Called by the writeTitle(..) routine. Assumption: module has input AND output ports
 * \param: const char* _fileName, the name of the output file
 * \param: const int _count, how many ports to declare
 * \return: void
 *
 */
void mwriteIODeclaration(const char* _fileName, module* _m)
{
    int _i = 0, _size = 0;
    lnode* _ls;
    FILE* _file;

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    /* print clock */
    fprintf(_file, "%5sinput %s,\ninput %s,\n", "", CLK, RESET);

    /* print input ports */
    _size = _m->_numOfInputs;
    for (_i = 0; _i < _size; _i++ )
        fprintf(_file, "%5sinput [%s-1:0]  %s,\n", "", INT_32, _m->_inputs[_i]);

    /* load values ports */
    _ls = _m->_load;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sinput [%s-1:0] load_val_%d,\n", "", INT_32, _ls->_value);
        _ls = _ls->_next;
    }
    /* store conditions output ports */
    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5soutput reg[%s-1:0] %s_%d,\n", "", INT_10, STORE, _ls->_value);
        _ls = _ls->_next;
    }
    /* load conditions output ports */
    _ls = _m->_load;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "output reg[%s-1:0] %s_%d,\n", INT_10, LOAD, _ls->_value);
        _ls = _ls->_next;
    }
    /* store values ports */
    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "output [%s-1:0] store_val_%d,\n", INT_32, _ls->_value);
        _ls = _ls->_next;
    }
    /* load addresses ports */
    _ls = _m->_load;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "output [%s-1:0] load_add_%d,\n", INT_10, _ls->_value);
        _ls = _ls->_next;
    }
    /* store addresses ports */
    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "output [%s-1:0] store_add_%d,\n", INT_32, _ls->_value);
        _ls = _ls->_next;
    }
    /* print output ports */
    _size = _m->_numOfOutputs;
    for (_i = 0; _i < _size; _i++ )
    {
        if(_i == _size - 1 || _size == 1 )
            fprintf(_file, "output [%s-1:0] out_%d\n", INT_32, _i);
        else
            fprintf(_file, "output [%s-1:0] out_%d,\n", INT_32, _i);
    }

    fclose(_file);
}


/** \brief: module domain, prints the modules title AND constants declarations to the output file
 *
 * \param: const char* _fileName, the output file name
 * \param: module* _m, a pointer to the struct holding the modules info
 * \return: void
 *
 */
void mwriteTitle(const char* _fileName, module* _m)
{

    int _i;                                                     /* running index */
    int _size;
    char _name[20];                                             /* char array design to hold the file name without .v extension */
    FILE* _file;                                                /* pointer to the file to write to */

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    /*copy just the name of the file without the .v suffix, and print it to the file
	  as the title of the module */
    memcpy(_name, &_fileName[0], strlen(_fileName) - 2);
	_name[strlen(_fileName) - 2] = '\0';
    fprintf(_file, "module %s(\n", _name);

    /* print clock and reset inputs */
    fprintf(_file, "%5sinput %s,\n%5sinput %s,\n", "", CLK, "", RESET);

    /* print input ports */
    _size = _m->_numOfInputs;
    for (_i = 0; _i < _size; _i++ )
        fprintf(_file, "%5sinput [%s-1:0]  %s,\n", "", INT_32, _m->_inputs[_i]);

    /* print output ports */
    _size = _m->_numOfOutputs;
    for (_i = 0; _i < _size; _i++ )
    {
        if(_i == _size - 1 || _size == 1 )
            fprintf(_file, "%5soutput [%s-1:0] out_%d\n", "", INT_32, _i);
        else
            fprintf(_file, "%5soutput [%s-1:0] out_%d,\n", "", INT_32, _i);
    }

    /*if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);*/
    fprintf(_file, ");\n\n");

    fprintf(_file, "\n");
    fclose(_file);
}

/** \brief: write local parameters for the state machine and constants to the file( after writing title )
 *
 * \param
 * \param
 * \return
 *
 */
 void mwriteConstants(const char* _fileName, module* _m)
 {
     int _i;
     int _numOfConstants;
     FILE* _file;

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);
    /*fprintf(_file, ");\n\n");*/
    _numOfConstants = _m->_numOfConstants;

    /* print the states of the fsm */
    fprintf(_file, "%5slocalparam [%s - 1:0] %s = 2'b00;\n", "", INT_2, states[_default] );
    fprintf(_file, "%5slocalparam [%s - 1:0] %s = 2'b01;\n", "", INT_2, states[_entry] );
    fprintf(_file, "%5slocalparam [%s - 1:0] %s = 2'b10;\n", "", INT_2, states[_for_body] );
    fprintf(_file, "%5slocalparam [%s - 1:0] %s = 2'b11;\n", "", INT_2, states[_for_end] );
    fprintf(_file, "\n");

    /* print constants  */
    for (_i = 0; _i < _numOfConstants; _i++ )
    {
        /* real constant: variables and pointers */
        if( *(_m->_constants[_i]) >= 97 && *(_m->_constants[_i]) <= 122 )
        {
            if(strstr(_m->_constants[_i], "undef"))
                fprintf(_file, "%5slocalparam [%s - 1:0] %s = 0;\n", "", INT_32, _m->_constants[_i] );
            else
                fprintf(_file, "%5slocalparam [%s - 1:0] %s = %s;\n", "", INT_32, _m->_constants[_i], _m->_constants[_i] + strlen("i32 ") );
        }
        /* arrays */
        else if( *(_m->_constants[_i]) >= 65 && *(_m->_constants[_i]) <= 90 )
           fprintf(_file, "%5slocalparam [%s - 1:0] %s = %d;\n", "", INT_32, _m->_constants[_i],  afindabn(_m->_arraysList, (_m->_constants[_i])));
    }



    fprintf(_file, "\n");
    fclose(_file);
 }



/** \brief: assign the approipriate operantio to the assignment
 *
 * \param: node* _operand, the operation to be exe. by the node
 * \return: char*, what operation in verilog to exe.
 *
 */
char* moperator(const node* _operand, const module* _m)
{
    int _opNum;                                 /* the node number */
    static char _result[MAX_OPERAND_SIZE];

    /*if( (_result = (char*)malloc(MAX_OPERAND_SIZE * sizeof(char))) == NULL )
        exit(1);*/

    sprintf(_result, "%s", EMPTY_STRING);
    _opNum = _operand->_number;
    /*printf("\nname:%s\n", _operand->_name);*/

    /*less than*/
    if(strcmp(_operand->_name, "LESS-THAN") == 0)
        sprintf(_result, "w_%d_%d <= w_%d_%d", _operand->_inEdges[0], _opNum, _operand->_inEdges[1], _opNum);

    /* subtract */
    else if(strcmp(_operand->_name, "SUBTRACT") == 0)
        sprintf(_result, "w_%d_%d - w_%d_%d", _operand->_inEdges[0], _opNum, _operand->_inEdges[1], _opNum);

    /* multiply */
    else if( strcmp(_operand->_name, "MULTIPLY") == 0 )
        sprintf(_result, "w_%d_%d * w_%d_%d", _operand->_inEdges[0], _opNum, _operand->_inEdges[1], _opNum);

    /* add */
    else if( strcmp(_operand->_name, "ADD") == 0 )
        sprintf(_result, "w_%d_%d + w_%d_%d", _operand->_inEdges[0], _opNum, _operand->_inEdges[1], _opNum);

    /*constant*/
    else if ( strcmp(_operand->_name, "Constant") == 0)
    {
        /* lower case operand name means a real constant */
        if( *_operand->_outEdgesName[0] >= 97 && *_operand->_outEdgesName[0] <= 122 )
            sprintf(_result, "%s", _operand->_outEdgesName[0]);

        /* upper case operand name means an array */
        if( *_operand->_outEdgesName[0] >= 65 && *_operand->_outEdgesName[0] <= 90 )
            sprintf(_result, "%s", _operand->_outEdgesName[0]);
    }

    else if( strcmp(_operand->_name, "Argument") == 0 )
        sprintf(_result, "%s", _operand->_outEdgesName[0]);

    else if( strcmp(_operand->_name, "condBR") == 0 )
        sprintf(_result, "%s", _operand->_outEdgesName[0]);

    else if( strcmp(_operand->_name, "BR") == 0 )
        sprintf(_result, "%s", _operand->_outEdgesName[0]);

    else if( strcmp(_operand->_name, "Clocked Register") == 0 )
        sprintf(_result, "%s", _operand->_outEdgesName[0]);

    else if( strcmp(_operand->_name, "GEP") == 0 )
        sprintf(_result, "w_%d_%d + w_%d_%d + w_%d_%d", _operand->_inEdges[0], _opNum, _operand->_inEdges[1], _opNum, _operand->_inEdges[2], _opNum);

    else if( strcmp(_operand->_name, "TRUNC") == 0 )
        sprintf(_result, "w_%d_%d[%s-1:0]", _operand->_inEdges[0], _opNum, INT_32);

    else if( strcmp(_operand->_name, "ASSIGNMENT") == 0 )
        sprintf(_result, "w_%d_%d", _operand->_inEdges[0], _opNum);

    else if( strcmp(_operand->_name, "RT") == 0 )
        sprintf(_result, "%s", _operand->_outEdgesName[0]);

    else if( strcmp(_operand->_name, "ICMPEQ") == 0 )
        sprintf(_result, "(w_%d_%d == w_%d_%d) ?  1 : 0", _operand->_inEdges[0], _opNum, _operand->_inEdges[1], _opNum);

    else if( strcmp(_operand->_name, "CONTROL_REGISTER") == 0 )
        sprintf(_result, "%s", _operand->_name);

    else if( strstr(_operand->_name, "SELECT") != NULL )
        sprintf(_result, "(w_%d_%d ) ?  w_%d_%d : w_%d_%d", _operand->_inEdges[0], _opNum, _operand->_inEdges[1], _opNum, _operand->_inEdges[2], _opNum);

    else if( strcmp(_operand->_name, "Load") == 0 )
        sprintf(_result, "load_val_%d", _opNum);

    else if( strcmp(_operand->_name, "Store") == 0 )
        sprintf(_result, "load_val_%d", _opNum);

    return _result;
}

/** \brief: module domain, declare wires and register of the module
 *
 * \param: char* _fileName, name of the output file
 * \param: module* _module, pointer
 * \return: void
 *
 */
void mwiredec(const char* _fileName, module* _m)
{
    int _index;
    int _nlength;
    char* _name;
    char* _cluster;
    edge* _edge;
    FILE* _file;
    lnode* _ls;

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);
    _nlength = _m->_graph->_nodes->_size;

    /* declare all wires, for each edge, a corresponding wire
    --------------------------------------------------------*/
    fprintf(_file, "\n%5s//wire declaration:\n", "");
    for(_index = 0; _index < _nlength; _index++)
    {
        _edge = _m->_graph->_edges->_list[_index];
        _name = _m->_graph->_nodes->_list[_index]->_name;
        /* don't declare empty edge, negative width edge or load\store nodes */
        if( _edge->source == -1 ) continue;
        else if( strcmp(_m->_graph->_nodes->_list[_index]->_name, "Store") == 0 ) continue;
        else if( _edge->width == -1 ) continue;


        _cluster = _m->_graph->_nodes->_list[_index]->_clstr->_name;
        while( _edge != NULL )
        {
            if( strcmp(_cluster, "for_body") != 0 && strstr(_name, "BR") ) continue;
            fprintf(_file, "%5swire[`int_%d - 1:0] w_%d_%d;\n", "", _edge->width, _index, _edge->target);
            _edge = _edge->_next;
        }
    }

    /* declare the multiplexer module output wires */
    /*---------------------------------------------*/
    fprintf(_file, "\n%5s//memory input wire declaration:\n", "");
    for(_index = 1; _index <= NUM_OF_MEM_READS; _index++)
        fprintf(_file, "%5swire[%s - 1:0] %s_%d;\n", "", INT_32, MEM_LOAD_IN_WIRE, _index);
    for(_index = 1; _index <= NUM_OF_MEM_WRITES; _index++)
    {
        fprintf(_file, "%5swire[%s - 1:0] %s_%d;\n", "", INT_32, MEM_STORE_ADD_IN_WIRE, _index);
        fprintf(_file, "%5swire[%s - 1:0] %s_%d;\n", "", INT_32, MEM_STORE_VAL_IN_WIRE, _index);
    }

    /* declare store conditions wires */
    /*---------------------------------------------*/
    fprintf(_file, "\n%5s//store conditions wire declaration:\n", "");
    for(_index = 1; _index <= NUM_OF_MEM_WRITES; _index++)
        fprintf(_file, "%5swire[%s - 1:0] %s_%d;\n", "", INT_1, WIRE_STORE, _index);

    /* declare all registers
    -----------------------*/
    fprintf(_file, "\n%5s//registers declaration\n", "");
    for(_index = 0; _index < _nlength; _index++)
    {
        _name = _m->_graph->_nodes->_list[_index]->_name;

        if( strstr(_name, "Register") != NULL )
            fprintf(_file, "%5sreg[%s- 1:0] %s_%d;\n", "", INT_32, REG, _m->_graph->_nodes->_list[_index]->_number);
    }

    /* store values ports */
    fprintf(_file, "\n%5s//declare local parameters for memory module\n", "");
    _ls = _m->_load;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sreg [%s-1:0] %s%d;\n", "", INT_32, port_attr[_load_val], _ls->_value);
        _ls = _ls->_next;
    }

    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sreg [%s-1:0] %s%d = 0;\n", "", INT_32, port_attr[_store_val], _ls->_value);
        _ls = _ls->_next;
    }
    /* load addresses ports */
    _ls = _m->_load;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sreg [%s-1:0] %s%d = 0;\n", "", INT_10, port_attr[_load_add], _ls->_value);
        _ls = _ls->_next;
    }
    /* store addresses ports */
    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sreg [%s-1:0] %s%d = 0;\n", "", INT_32, port_attr[_store_add], _ls->_value);
        _ls = _ls->_next;
    }
    /* declare store registers */
    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sreg %s%d;\n", "", "store_", _ls->_value);
        _ls = _ls->_next;
    }

    fprintf(_file, "%5sreg[%s - 1:0] CONTROL_REGISTER;\n", "", INT_32);
    fprintf(_file, "%5sreg[%s:0] %s, %s;\n", "", INT_1, STATE, NEXT);

    fclose(_file);
}

/** \brief: module domain, wire combinatorial part of the module. for each edge, get the opertor according to the
            source node of the edge by calling operand method, and assgin the operator to all target nodes
            of that source node, i.e. all the nodes that can be reached from that node.
 *
 * \param: char* _fileName, name of the output file
 * \param: module* _module, pointer
 * \return: void
 *
 */
 void mwirecom(const char* _fileName, module* _m)
 {
     int _index;                                                /* iterating indexes */
     int _maxc = 0;                                             /* holds the largest constant found */
     int _nlength;                                              /* number of nodes if the graph */
     int _ocount = 0;                                           /* number of outputs in the module */
     char* _name;                                               /* holds the temp node name */
     char* _operator;                                           /* holds the string that represent the node operator */
     edge* _edge;                                               /* temp edge object for iteration */
     FILE* _file;                                               /* file poinetr to write to */
     /*lnode* _ls;*/
     node* _node;                                               /* iterating node */

     if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    _nlength = _m->_graph->_nodes->_size;

    /* wire all edges according to their in comming node operator */
    fprintf(_file, "\n\n%5s//wire combinatorial part\n", "");
    for(_index = 0; _index < _nlength; _index++)
    {
        _edge = _m->_graph->_edges->_list[_index];
        if( _edge->source == -1 ) continue;
        while( _edge != NULL )
        {
            _name = _m->_graph->_nodes->_list[_index]->_name;

            /* not a load or store node, wire load seperatly */
            if( /*strcmp(_name, LOAD) == 0 ||*/ strcmp(_name, STORE) == 0 ) break;

            /* wire register output */
            if( strcmp(_name, "Clocked_Register") == 0 )
                fprintf(_file, "%5sassign w_%d_%d = reg_%d;\n", "", _index, _edge->target, _index);

            /* skip output wires */
            else if( strcmp(_name, "RT") != 0)
            {
                /* assign the same operator to all edges the goes out of node in the index position */
                _operator = moperator(_m->_graph->_nodes->_list[_index], _m);
                if( strlen(_operator) > 0 )
                    fprintf(_file, "%5sassign w_%d_%d = %s;\n", "", _index, _edge->target, _operator);
                _operator = NULL;
            }
            else
                fprintf(_file, "%5sassign w_%d_%d = %s_%d;\n", "", _index, _edge->target, REG, _index);

            _edge = _edge->_next;
        }
    }
    /*
    _ls = _m->_load;
    while( _ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sassign w_%d_%d = %s%d;\n", "", _ls->_value, _m->_graph->_nodes->_list[_ls->_value]->_outlst->_value, port_attr[_load_val], _ls->_value);
        _ls = _ls->_next;
    }
    _ls = _m->_load;
    while( _ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sassign load_add_%d = Load_%d & w_%d_%d;\n", "", _ls->_value,_ls->_value, _m->_graph->_nodes->_list[_ls->_value]->_inEdges[0], _ls->_value);
        _ls = _ls->_next;
    }
    */

    /* output wire(s): */
    if( _m->_defaultOut == -1 )
    {
        for(_index = 0; _index < _m->_numOfConstants; _index++)
            if( *(_m->_constants[_index]) == 'i' )
                if( atoi(_m->_constants[_index] + strlen("i32 ")) > _maxc )
                    _maxc = atoi(_m->_constants[_index] + strlen("i32 "));

        fprintf(_file, "%5sassign out_0 = i64_%d;\n", "", _maxc);
    }

    else
    {
        for(_index = 0; _index < _m->_graph->_nodes->_size; _index++)
        {
            _node = _m->_graph->_nodes->_list[_index];
            _name = _node->_name;
            if( strcmp(_name, "RT") == 0 )
            {
                fprintf(_file, "%5sassign out_%d = (w_%d_%d) ? w_%d_%d : 32'b0;\n", "", _ocount, _m->_forscon->_inEdges[0], _m->_forscon->_number, _node->_inEdges[_ocount], _index);
                _ocount++;
            }

        }
    }
    fclose(_file);
 }

/** \brief: module domain, wire the memory module
 *
 * \param: char* _fileName, name of the output file
 * \param: module* _module, pointer
 * \return: void
 *
 */
void mwiremem(const char* _fileName, module* _m)
{
    int _index;                                     /* running index */
    int _loadCount = 0;                             /* count the number of Load nodes */
    /*int _nodeNum;                                    holds a node number attr. */
    /*int _loads[20];                                  holds the number of load nodes */
    lnode* _l;                                      /* linked list of all Load nodes */
    FILE* _file;

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    fprintf(_file, "\n%5s%s qr(\n", "", QRAM );

    for(_index = 1; _index <= MAX_NUM_STORE; _index++)
        fprintf(_file, "%5s%s_%d,\n", "", MEM_STORE_VAL_IN_WIRE, _index);

    for(_index = 1; _index <= MAX_NUM_LOADS; _index++)
        fprintf(_file, "%5s%s_%d,\n", "", MEM_LOAD_IN_WIRE, _index);

    for(_index = 1; _index <= MAX_NUM_STORE; _index++)
        fprintf(_file, "%5s%s_%d,\n", "", WIRE_STORE, _index);

    fprintf(_file, "%5s%s,\n", "", CLOCK);
    /* add memory module if the array list of the module is not empty */
    if(_m->_arraysList->_count != 0 )
    {
        _l = _m->_load;

        /* store condition: store registers */
        while( _l != NULL )
        {
            if( _l->_value != -1 )
            {
                fprintf(_file, "%5s%s_%d,\n", "", LOAD_VALUE, _l->_value);
                _loadCount++;
            }
            _l = _l->_next;
        }

        /* add clock wire to memory by default */
        fprintf(_file, "%5s);\n", "" );
    }

    fclose(_file);
}

/** \brief: module domain, wire the first state of the FSM, the entry state
 *                                                              -----
 * \param: char* _fname, name of the output file
 * \param: module* _module, pointer
 * \return: void
 *
 */
void mwireseqe(const char* _fname, module* _m)
{
    int _index;
    lnode *_l, *_s;
    FILE* _file;

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);

    /*  'entry'  */
    fprintf(_file, "%5s//entry\n", "");
    fprintf(_file, "%5s%s: begin\n", "", states[entry]);
    _l = _m->_load;
    _s = _m->_store;
    while( _l != NULL )
    {
        /* find a load node in the entry cluster */
        _index = _l->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_l->_value]->_clstr->_name, "entry") == 0 )
            {
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_load_add], _index, _m->_graph->_nodes->_list[_index]->_inEdges[0], _index);
                printf("%10s%s_%d <= {%s'b%d};\n", "", LOAD, _index, INT_10, 1);
            }
            else
            {
                fprintf(_file, "%10s%s%d <= {%s'b%d};\n", "", port_attr[_load_add], _index, INT_10, 1);
                printf("%10s%s%d <= {%s'b%d};\n", "", port_attr[_load_add], _index, INT_10, 1);
            }

        }

        /* find a store node in the entry cluster */
        _index = _s->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_s->_value]->_clstr->_name, "entry") == 0 )
            {
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_store_add], _index, _m->_graph->_nodes->_list[_index]->_inEdges[0], _index);
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_store_val], _index, _m->_graph->_nodes->_list[_index]->_inEdges[1], _index);
            }
            /*
            else
            {
                fprintf(_file, "%10s%s%d <= {%s'b%d};\n", "", port_attr[_store_add], _index, INT_10, 1);
                fprintf(_file, "%10s%s%d <= {%s'b%d};\n", "", port_attr[_store_val], _index, INT_10, 1);
            }
            */
        }
        _l = _l->_next;
        _s = _s->_next;
    }
    /*
    for(_index = 0; _index < _m->_graph->_nodes->_size; _index++)
        if( strcmp(_m->_graph->_nodes->_list[_index]->_name, "Clocked_Register") == 0 )
            fprintf(_file, "%10s%s_%d <= {%s'b%d};\n", "", REG, _index, INT_32, 0);
    */
    fprintf(_file, "%10s%s <= %s;\n", "", NEXT, states[_for_body]);
    fprintf(_file, "%5send// end entry\n\n", "");
    fclose(_file);
}

/** \brief: module domain-wire sequesnce-default part the default state of the FSM, i.e. reset
 *                                  -------
 * \param: char* _fname, name of the output file
 * \param: module* _module, pointer
 * \return: void
 *
 */
void mwireseqd(const char* _fname, module* _m)
{
    int _index;
    lnode *_l, *_s;
    FILE* _file;

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);

    /*  'default'  */
    fprintf(_file, "\n%5s//default\n", "");
    fprintf(_file, "%5s%s: begin\n", "", states[_default]);
    _l = _m->_load;
    _s = _m->_store;
    while( _l != NULL )
    {
        /* zero all in the default state */
        _index = _l->_value;
        if( _index != -1 )
            fprintf(_file, "%10s%s%d <= %d;\n", "", port_attr[_load_add], _index, 0);

        _index = _s->_value;
        if( _index != -1 )
        {
            fprintf(_file, "%10s%s%d <= %d;\n", "", port_attr[_store_add], _index, 0);
            fprintf(_file, "%10s%s%d <= %d;\n", "", port_attr[_store_val], _index, 0);
        }
        _l = _l->_next;
        _s = _s->_next;
    }
    for(_index = 0; _index < _m->_graph->_nodes->_size; _index++)
        if( strcmp(_m->_graph->_nodes->_list[_index]->_name, "Clocked_Register") == 0 )
            fprintf(_file, "%10s%s_%d <= %d;\n", "", REG, _index, 0);

    fprintf(_file, "%10s%s <= %s;\n", "", NEXT, states[_entry]);
    fprintf(_file, "%5send//end default\n\n", "");
    fclose(_file);
}

/** \brief: module domain, wire second state of the sequential: for_body state
 *                                                              --------
 * \param: char* _fname, name of the output file
 * \param: module* _module, pointer
 * \return: void
 *
 */
void mwireseqf(const char* _fname, module* _m)
{
    int _index;
    int _nlength;
    char* _name;
    char* _cluster;
    node* _node;

    FILE* _file;

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);
    _nlength = _m->_graph->_nodes->_size;
    /* 'for_body' cluster */
    fprintf(_file, "%5s//for body\n", "");
    fprintf(_file, "%5s%s: begin\n", "", states[_for_body]);
    fprintf(_file, "%5sif(w_%d_%d) %s <= %s;\n", "", _m->_forendcon->_inlst->_value, _m->_forendcon->_number, NEXT, states[_for_end]);
    fprintf(_file, "%5selse begin\n", "");
    for(_index = 0; _index < _nlength; _index++)
    {
        _node = _m->_graph->_nodes->_list[_index];
        if( *(_name = _node->_name) == '\0' ) continue;
        _cluster = _node->_clstr->_name;

        if(  strstr(_name, "Register") != '\0' )
            fprintf(_file, "%10s%s_%d <= w_%d_%d;\n", "", REG, _index, _node->_inEdges[0], _index);
        /* if the node has a for_body cluster class then assign its store/load wires */
        else if( strcmp(_cluster, "for_body") == 0 )
        {
            if( strcmp(_name, LOAD) == 0 )
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_load_add], _index, _node->_inEdges[0], _index);
            else if (strcmp(_name, STORE) == 0 )
            {
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_store_add], _index, _node->_inEdges[0], _index);
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_store_val], _index, _node->_inEdges[1], _index);
            }
        }
        /*
        else
        {
            if( strcmp(_name, LOAD) == 0 )
                fprintf(_file, "%10s%s%d <= {%s'b%d};\n", "", port_attr[_load_add], _index, INT_10, 0);
            else if (strcmp(_name, STORE) == 0 )
            {
                fprintf(_file, "%10s%s%d <= {%s'b%d};\n", "", port_attr[_store_add], _index, INT_10, 0);
                fprintf(_file, "%10s%s%d <= {%s'b%d};\n", "", port_attr[_store_val], _index, INT_32, 0);
            }
        }
        */
    }
    fprintf(_file, "%10send     //end else\n", "");
    /*fprintf(_file, "%5s%s <= %s;\n", "", STATE, states[_for_end]);*/
    fprintf(_file, "%5send     //end for_body case\n\n", "");
    fclose(_file);
}

/** \brief: module domain, wire the final state of the FSM: the for_end part
 *                                                              -------
 * \param: char* _fname, name of the output file
 * \param: module* _module, pointer
 * \return: void
 *
 */
void mwireseqfe(const char* _fname, module* _m)
{
    int _index;
    lnode *_l, *_s;
    FILE* _file;

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);

    /* for_end */
    fprintf(_file, "%5s//for end\n", "");
    fprintf(_file, "%5s%s: begin\n", "", states[_for_end]);
    _l = _m->_load;
    _s = _m->_store;
    while( _l != NULL )
    {
        /* find a load node in the for end cluster */
        _index = _l->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_l->_value]->_clstr->_name, "for_end") == 0 )
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_load_add], _index, _m->_graph->_nodes->_list[_s->_value]->_inEdges[0], _index);
            /*
            else
                fprintf(_file, "%10s%s%d <= {%s'b%d};\n", "", port_attr[_load_add], _index, INT_10, 0);
            */
        }

        /* find a store node in the for end cluster */
        _index = _s->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_s->_value]->_clstr->_name, "for_end") == 0 )
            {
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_store_add], _index, _m->_graph->_nodes->_list[_s->_value]->_inEdges[0], _index);
                fprintf(_file, "%10s%s%d <= w_%d_%d;\n", "", port_attr[_store_val], _index, _m->_graph->_nodes->_list[_s->_value]->_inEdges[1], _index);
            }
            /*
            else
                fprintf(_file, "%10s%s%d <= {%s'b%d};\n", "", port_attr[_load_add], _index, INT_10, 0);
            */
        }
        _l = _l->_next;
        _s = _s->_next;
    }
    /*
    for(_index = 0; _index < _m->_graph->_nodes->_size; _index++)
        if( strcmp(_m->_graph->_nodes->_list[_index]->_name, "Clocked_Register") == 0 )
            fprintf(_file, "%10s%s_%d <= {%s'b%d};\n", "", REG, _index, INT_32, 0);
    */
    fprintf(_file, "%10s%s <= %s;\n", "", NEXT, states[_for_end]);
    fprintf(_file, "%5send//end for_end\n\n", "");            /* end case 3*/
    fprintf(_file, "%5sendcase\n", "");        /* end switch-case */

    fclose(_file);
}

/** \brief: module domain, wire the multiplexer module of the memory module
 *
 * \param: const char *_fname, the name of the file to written to
 * \param: module *_m, a pointer to the module object
 * \return: void
 *
 */
void mwiremx(const char *_fname, module *_m)
{
    int _lcount = 0, _index = 0;
    FILE* _file;
    lnode *_ls;

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);

    fprintf(_file, "\n%5s//wire the memory multiplexer\n", "");
    fprintf(_file, "%5smux mx(\n", "");
    /* store values ports */
    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sstore_val_%d,\n", "", _ls->_value);
        _ls = _ls->_next;
        _lcount++;
    }

    /* store addresses ports */
    _ls = _m->_store;
    _lcount = 0;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sstore_add_%d,\n", "", _ls->_value);
        _ls = _ls->_next;
        _lcount++;
    }

    /* load addresses output ports, at most MAX_NUM_LOADS */
    _ls = _m->_load;
    _lcount = 0;
    while(_ls != NULL && _lcount < MAX_NUM_LOADS )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sload_add_%d,\n", "", _ls->_value);
        _ls = _ls->_next;
        _lcount++;
    }

    /* wire the output of the multiplexer */
    for(_index = 1; _index <= NUM_OF_MEM_READS; _index++)
        fprintf(_file, "%5s%s_%d,\n", "", MEM_LOAD_IN_WIRE, _index);
    for(_index = 1; _index <= NUM_OF_MEM_WRITES; _index++)
        fprintf(_file, "%5s%s_%d,\n", "", MEM_STORE_VAL_IN_WIRE, _index);

    for(_index = 1; _index <= NUM_OF_MEM_WRITES; _index++)
        fprintf(_file, "%5s%s_%d,\n", "", WIRE_STORE, _index);

    fprintf(_file, "%5s%s,\n", "", STATE);
    fprintf(_file, "%5s%s\n", "", CLOCK);

    fprintf(_file, "%5s);\n", "");
    fclose(_file);
}


/** \brief: module domain, wire sequential part of the module
 *
 * \param: char* _fname, name of the output file
 * \param: module* _module, pointer
 * \return: void
 *
 */
void mwireseq(const char* _fname, module* _m)
{
    /*int _i;*/
    /*int _maxc;*/
    /*int _ocount;*/
    char* _bin;
    /*char* _name;*/
    /*lnode* _ls;*/
    FILE* _file;

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);

    if( (_bin = (char*)malloc(sizeof(char) * 32 )) == NULL )
        exit(1);

    fprintf(_file, "\n%5s//wire sequential part\n", "");
    fprintf(_file, "%5s//control state\n", "");
    fprintf(_file, "%5salways @(posedge %s ) begin \n", "", CLK/*, RESET*/);
    fprintf(_file, "%10sif(%s) %s <= %s;\n", "", RESET, STATE, states[_default] );
    fprintf(_file, "%10selse %s <= %s;\n", "", STATE, NEXT );


    /* FSM : implement finite state machine */
    fprintf(_file, "%5scase (%s)\n", "", STATE);
    fclose(_file);

    /* defaul state */
    mwireseqd(_fname, _m);

    /*  'entry'  state */
    mwireseqe(_fname, _m);

    /* 'for_body' cluster state */
    mwireseqf(_fname, _m);

    /* for_end state */
    mwireseqfe(_fname, _m);

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);
    fprintf(_file, "\n%5s//end ssequensial part\n%5send\n", "", "" );
    fclose(_file);
}

/** \brief: wires all the input-output ports:_m->_nodes->_list[_nodeNum]->_name
 *
 * \param: const char* _fileName, the name of the output file
 * \param: ObjectList _list, a struct that holds all the elements scanned in the input file
 * \return: void
 *
 */
void mwire(const char* _fname, module* _m)
{
    FILE* _file;


    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);

    mwiredec(_fname, _m);          /* declare all wires and registers */

    mwirecom(_fname, _m);          /* wire combinatorial part */

    /* wire memory module */
    mwiremem(_fname, _m);          /* wire memory */
    mwiremx(_fname, _m);



    mwireseq(_fname, _m);          /* wire sequential part */

    fclose(_file);

}



/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
FILE* mwriteModule(const char* _output, module* _m)
{
    FILE* _result;

    if( (_result = fopen(_output, "w")) == NULL )
        exit(1);
    fclose(_result);

    fwriteHeader(_output, "creator");

    mwriteTitle(_output, _m);

    mwriteConstants(_output, _m);

    mwire(_output, _m);


    if( (_result = fopen(_output, "a")) == NULL )
        exit(1);
    fprintf(_result, "\nendmodule");
    fclose(_result);

    return _result;
}

#endif
