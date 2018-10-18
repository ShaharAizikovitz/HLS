/* version 16.10.2018 */

#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED

#include "definitions.h"
typedef struct module module;
typedef struct edgeList edgeList;
typedef struct input input;
typedef struct graph graph;
typedef struct nodeNum nodeNum;
typedef struct node node;
typedef struct nodeList nodeList;
typedef struct edge edge;
typedef struct lnode lnode;
typedef struct mArray mArray;
typedef struct mArrayList mArrayList;

/*---------------------------------------------------------------------------------------------
                                         lnode
----------------------------------------------------------------------------------------------*/
struct lnode{
    int _value, _state;
    lnode* _next;
    lnode* _prev;
};

/*---------------------------------------------------------------------------------------------
                                         marray
----------------------------------------------------------------------------------------------*/
struct mArray{
    int _size;
    int _address;
    char* _type;
    char* _name;
};

/*---------------------------------------------------------------------------------------------
                                         mArrayList
----------------------------------------------------------------------------------------------*/
struct mArrayList{
    int _count;
    int _totalSpace;
    mArray** _list;
};

/*---------------------------------------------------------------------------------------------
                                         node
----------------------------------------------------------------------------------------------*/
/* node that holds the "shape"s input-output ports, their addresses, name and serial number*/
struct node{
    /*members*/
    char _inEdgesName[MAX_PORT_NUM][MAX_CHAR_PER_PORT];
    char _outEdgesName[MAX_PORT_NUM][MAX_CHAR_PER_PORT];
    char* _name;
    int _inEdges[MAX_PORT_NUM];
    int _indegree;
    int _outdegree;
    int _number;
    int _freeEdge;
    int _state;
    lnode* _outlst;
    lnode* _inlst;
    node* _clstr;
};


/*---------------------------------------------------------------------------------------------
                                        NodeNum
----------------------------------------------------------------------------------------------*/
struct nodeNum{
    int _num;
    int _removed;
    nodeNum* _next;
};


/*---------------------------------------------------------------------------------------------
                                        edge
----------------------------------------------------------------------------------------------*/
typedef struct edge{
    /*members*/
    int source, target, width;
    edge* _next;
} edge;

/*---------------------------------------------------------------------------------------------
                                        edge List
---------------------------------------------------------------------------------------------*/
struct edgeList{
    /*members*/
    int _size;
    int _numOfCurrentElem;
    edge** _list;
};

/*--------------------------------------------------------------------------------------------
                                        graph
----------------------------------------------------------------------------------------------*/
struct graph{
    /*members*/
    int _size;
    int _currCount;

    nodeList* _nodes;
    nodeList* _clusters;
    edgeList* _edges;
};

/*--------------------------------------------------------------------------------------------
                                        module
---------------------------------------------------------------------------------------------*/
struct module{

    /*members*/
    int _numOfInputs, _numOfOutputs, _numOfConstants, _numOfLoad,_numOfStore;
    int _defaultOut;
    char** _inputs;
    char** _outputs;
    char** _constants;
    graph* _graph;
    lnode* _load;
    lnode *_entry, *_forLoop, *_forEnd;
    lnode* _store;
    lnode* _return;
    node* _forscon;                                                 /* for loop start condition */
    node* _forendcon;                                               /* for loop end condition */
    mArrayList* _arraysList;
};

#endif
