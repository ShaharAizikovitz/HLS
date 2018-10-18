/* version 16.10.2018 */

#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "definitions.h"
#include "log.h"
#include "lnklst.h"
#include "utilities.h"


char* DICTIONARY[] = { "LESS-THAN", "Constant", "Argument", "condBR", "BR", "Clocked Register", "GEP", "Load", "MULTIPLY",
                    "ADD", "TRUNC", "ASSIGNMENT", "RT", "ICMPEQ" };




/** \brief: object domain, create a new edge struct
 *
 * \return: a pointer to the new struct
 *
*/
edge* onewEdge()
{
    edge* _result;

    if((_result = (edge*)malloc(sizeof(edge))) == NULL)
        exit(1);
    else
    {
        _result->source = -1;
        _result->target = -1;
        _result->width = 1;
        _result->_next = NULL;
    }
    return _result;
}


/** \brief: object domain, create a new edge list of size _size
 *
 * \param: int _size the number of elements to be in the list
 * \return: edge** _result, created list
 *
 */
edgeList* onewEdgeList(int _size)
{
    int _index;
    edgeList* _result;

    if ((_result = (edgeList*)malloc(sizeof(edgeList))) == NULL)
        exit(1);
    if ((_result->_list = (edge**)malloc(sizeof(edge) * _size)) == NULL)
        exit(1);
    else
    {
        for(_index = 0; _index < _size; _index++)
            _result->_list[_index] = onewEdge();

    _result->_size = _size;
    _result->_numOfCurrentElem = 0;
    }
    return _result;
}


/** \brief: object domain, add a given edge to a known edge list and adjust the number of elements
 *
 * \param: edgeList* _list, a pointer to the
 * \param: edge* _edge, the edge to be inserted
 * \return: int _size, the number of total elements in the list
 *
 */
 int oadde(edgeList* _list, int _esource, int _etarget)
 {
    edge* _head;
    edge* _new = onewEdge();

    _head = _list->_list[_esource];
    _new->source = _esource;
    _new->target = _etarget;


    if( _head->source == -1 )
            _list->_list[_esource] = _new;
    else
    {
        _new->_next = _head;
        _list->_list[_esource] = _new;
    }

    return ++_list->_numOfCurrentElem;
 }

 /** \brief: object domain,
  *
  * \param
  * \param
  * \return
  *
  */
int ofindTargetNode(edgeList* _list, int _source)
 {
    int _result = 0;
    int _i;
    for(_i = 0; _i < _list->_size; _i++)
    {
        if( _list->_list[_i]->source == _source )
        {
            _result = _list->_list[_i]->target;
            break;
        }
    }


    return _result;
 }


 /** \brief: object domain, freeing a given edge list of size _size
  *
  * \param: edgeList _list, a pointer to the desired list to be free
  * \return: void
  *
  */
 int ofreeEdgeList(edgeList* _list)
 {
     int _lsize = _list->_size;
     int _i;
	 int _result = 0;

     for( _i = 0; _i < _lsize; _i++)
     {
         free(_list->_list[_i]);
		 _list->_list[_i] = NULL;
		 if( _list->_list[_i] != NULL ) { _result = 1; printf("failure to free edge: %d\n", _i); }
     }

	 return _result;
 }



void ofprinte(char* _fname, edgeList* _list)
{
    int _i;
    edge* _temp;
    FILE* _file;

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);

    fprintf(_file, "edge list:\n");
    printf("edge list:\n");
    for(_i = 0; _i < _list->_size; _i++)
    {
        _temp = _list->_list[_i];
        if( _temp->source == -1 ) continue;

        fprintf(_file, "%d->", _temp->source);
        printf("%d->", _temp->source);
        while(_temp != NULL )
        {
            fprintf(_file, "%d->", _temp->target);
            printf("%d->", _temp->target);
            _temp = _temp->_next;
        }
        fprintf(_file, "|\n");
        printf("|\n");
    }
    fclose(_file);
}


node* onewNode()
{
    int _i;
    node* _result;
    _result = (node*)malloc(sizeof(node));
    _result->_name = (char*)malloc(sizeof(char) * MAX_CHAR_PER_PORT);
    _result->_inlst = lnewlnode();
    _result->_outlst = lnewlnode();

    for(_i = 0; _i < MAX_PORT_NUM; _i++)
    {
        sprintf(_result->_inEdgesName[_i], "%s", EMPTY_STRING);
        sprintf(_result->_outEdgesName[_i], "%s", EMPTY_STRING);
        _result->_inEdges[_i] = -1;
    }
    sprintf(_result->_name, "%s", EMPTY_STRING);
    _result->_number = -1;
    _result->_freeEdge = 0;
    _result->_indegree = _result->_outdegree = 0;

    return _result;
}


/** \brief: object domain, free a node pointed by node
 *
 * \param: node* _node, a pointer to the node wished free
 * \return: int, 0 on success 1 other wise
 *
 */
int ofreeNode(node* _node)
{
	int _result = 0;
    free(_node->_name);
    lfreelnode(_node->_inlst);
    lfreelnode(_node->_outlst);
    free(_node);
	_node = NULL;
	if( _node != NULL ) { _result = 1; printf("failure to free node\n"); }
	return _result;
}

/** \brief: object domain, print an object with its outfoing edges to console
 *
 * \param: node* _o, the node object  pointer
 * \return: void
 *
 */
void oprint(node* _o)
{
    printf("number of node: %d\n", _o->_number);
    printf("cluster: %s\n", _o->_clstr->_name);
    printf("name: %s\n", _o->_name);
    printf("inputs\n");
    lprintl("", _o->_inlst);
    printf("\noutputs\n");
    lprintl("", _o->_outlst);

    printf("\n\n");
}

/** \brief: object domain, print node object into file
 *
 * \param: char* _fname, the file to print to
 * \param: node* _node, the node to be printed
 * \return: void
 *
 */
void ofprint(char* _fname, node* _o)
{
    lnode *_node;
    FILE* _file;

    if((_file = fopen(_fname, "a")) == NULL )
        exit(1);

    fprintf(_file, "\nnumber of node: %d\n", _o->_number);
    fprintf(_file, "cluster: %s\n", _o->_clstr->_name);
    fprintf(_file, "name: %s\n", _o->_name);

    /* print incomming edges */
    fprintf(_file, "inputs\n");
    _node = _o->_inlst;
    while(_node != NULL )
    {
        fprintf(_file, "%d->", _node->_value);
        _node = _node->_next;
    }
    fprintf(_file, "|\n");

    /* print outgoing edges */
    fprintf(_file, "outputs\n");
    _node = _o->_outlst;
    while(_node != NULL )
    {
        fprintf(_file, "%d->", _node->_value);
        _node = _node->_next;
    }
    fprintf(_file, "|\n");


    fprintf(_file, "\n\n");
    fclose(_file);
}



/*-----------------------------------------------------------------------------------------------
                                           node List
------------------------------------------------------------------------------------------------*/

/*a struct that holds ALL of the objects found in the input file, include how many in total, and the size*/
struct nodeList{
    /*members*/
    int _size;
    int _numOfObjects;
    node** _list;

    /*methods*/
    nodeList*(*newNodeList)(int);
    int(*addNode)(nodeList, node*);
};

/** \brief: object domain, create a new node list and instanciate it
 *
 * \param: int _size, how many objects in the new list
 * \return: an node list pointer
 *
 */
nodeList* onewNodeList(int _size)
{
    int i;
    nodeList* _result = (nodeList*)malloc(sizeof(nodeList));
    _result->_list = (node**)malloc(sizeof(node)*_size);
    _result->_numOfObjects = 0;
    _result->_size = _size;

    for(i = 0; i < _size; i++)
        (_result->_list)[i] = onewNode();

    writeToLog("node list initialized..", "Object.initObject");
    return _result;
}

/** \brief: object domain, add a given node to as node list, and advances the number of objects in the list
 *
 * \param: nodeList* _objList, a pointer to a specific list
 * \param: node* _obj, a pointer to be inserted
 * \return: int, the number of elements in the list
 *
 */
int oaddNode(nodeList* _nodeList, node* _node)
{
    /*make the list at the _objList->_numOfObjects place pointed by the new obkect _obj
    and increment by 1 */
    int _nodeNum = _node->_number;
    _nodeList->_list[_nodeNum] = _node;
    _nodeList->_numOfObjects++;
    return _nodeList->_numOfObjects;
}



/** \brief: object domain, freeing all the assigned fields of an node list
 *
 * \param: objectList* _list, a pointer to the desierd node list
 * \return: void
 *
 */
int ofreeNodeList(nodeList* _list)
{
    int _size = (_list->_numOfObjects);
    int _i;
	int _result = 0;

    writeToLog("node list deallocating..", "Object.freeObjectList");
    for(_i = 0; _i < _size; _i++)
    {
        _result = ofreeNode(_list->_list[_i]);
    }
	if( _result ) printf("failure to free node list\n");
	return _result;
}


/** \brief: object domain finds object by name from node array
 *
 * \param: nodeList* _list, the node list that holds the array
 * \param: const chsr* _name, the name saught
 * \return: node*, a pointer to the found node, or NULL if does not exist in the list
 *
 */
node* ofindbn(nodeList* _list, const char* _name)
{
    int _i;
    node* _result = NULL;

    for(_i = 0; _i < _list->_size; _i++)
        if( strcmp(_list->_list[_i]->_name, _name) == 0 )
        {
            _result = _list->_list[_i];
            break;
        }

    return _result;
}

#endif
