/* version 16.10.2018 */

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "object.h"
#include "lnklst.h"
#include "definitions.h"




nodeNum* gnewNodeNum()
{
    nodeNum* _result;
    if( (_result = (nodeNum*)malloc(sizeof(nodeNum))) == NULL )
        exit(1);
    _result->_num = -1;
    _result->_removed = 0;
    _result->_next = NULL;

    return _result;
}

/** \brief: create a new graph. edged and nodes lists are empty
 *
 * \param: int _nsize, the number of nodes, including none participating
 * \param: int _csize, the number of clusters
 * \return: graph* _result, a pointer to the graph created
 *
 */
 graph* gnewGraph(int _nsize, int _csize)
 {
    graph* _result;

    if( (_result = (graph*)malloc(sizeof(graph))) == NULL )
        exit(1);
 /*   if( (_result->_list = (nodeNum**)malloc(sizeof(nodeNum)*_nsize)) == NULL )
        exit(1);*/
    _result->_size = _nsize;
    _result->_currCount = 0;
    _result->_clusters = onewNodeList(_csize);
    _result->_nodes = onewNodeList(_nsize);
    _result->_edges = onewEdgeList(_nsize);
    return _result;
 }

/** \brief: graph domain, frees a graph object
 *
 * \param: graph* _g, a pointer to the graph to be freed
 * \return: int, 0 if succeded, 1 if failed
 *
 */
 int gfreeGraph(graph* _g)
 {
     int _i;
	 int _result = 0;
     for(_i = 0; _i < _g->_size; _i++){}

     if( ofreeNodeList(_g->_clusters) ) _result = 1;
     if( ofreeNodeList(_g->_nodes) ) _result = 1;
     if( ofreeEdgeList(_g->_edges) ) _result = 1;

	 return _result;
 }

/** \brief: graph domain, print a reduced version of a node list toa file of a given name
 *
 * \param:char* _fname, string represesnt the file name
 * \param: graph* _g, a pointer to a graph obejct
 * \return: void
 *
 */
void gprintnl(char* _fname, graph* _g)
{
    int _i, _count;
    lnode *_edge;
    FILE* _file;

    if( (_file = fopen("log.txt", "a")) == NULL )
        exit(1);
    _count = _g->_nodes->_size;
    fprintf(_file, "\ngraph node list:\n");
    for(_i = 0; _i < _count; _i++)
    {
        /*_edge = _g->_list[_i];*/
        fprintf(_file, "\n%d->", _i);
        /*if( _edge->_num == -1 ) continue;*/
        if( _edge->_value == -1 ) continue;
        while(_edge != NULL)
        {
            /*fprintf(_file, "%d->", _edge->_num);*/
            fprintf(_file, "%d->", _edge->_value);
            _edge = _edge->_next;
        }
        fprintf(_file, "|");
    }
    fclose(_file);
}

/** \brief: graph domain, add a new edge to the edge list: for each node, an edge is a node that one can
            get from the source node to the target node
 *
 * \param: graph* _g, a pointer to the graph object
 * \param: int _s, the number of the source node
 * \param: int _t, the number of the target node
 * \return: int, count the number of edges in the list
 *
 */
int gadde(graph* _g, int _s, int _t)
{
    node* _temp;
    edge* _head;
    edge* _new = onewEdge();
    edgeList* _list = _g->_edges;

    _temp = _g->_nodes->_list[_t];
    _temp->_indegree = laddtail(_temp->_inlst, _s, _temp->_indegree, _temp->_state);
    _temp->_inEdges[_temp->_indegree - 1] = _s;

    _temp = _g->_nodes->_list[_s];
    _temp->_outdegree = laddtail(_temp->_outlst, _t, _temp->_outdegree, _temp->_state);

    _head = _list->_list[_s];
    _new->source = _s;
    _new->target = _t;


    if( _head->source == -1 )
            _list->_list[_s] = _new;
    else
    {
        _new->_next = _head;
        _list->_list[_s] = _new;
    }

    return (_list->_numOfCurrentElem++);

}

/** \brief: graph domain add a new cluster node to the graph clusters list
 *
 * \param: graph* _g, a [pointer to the graph object
 * \param: const char* _name, the of the new cluster
 * \return:int, the number of objects in the list
 *
 */
int gaddc(graph* _g, const char* _name)
{
    int _result = _g->_clusters->_numOfObjects;
    int _i = 0, _exist = 0;

    if( _result == 0 )
    {
        _g->_clusters->_list[0] = onewNode();
        strcpy(_g->_clusters->_list[0]->_name, _name);
        return ++(_g->_clusters->_numOfObjects);
    }

    while( _i < _result)
        if ( strcmp(_name, _g->_clusters->_list[_i++]->_name) == 0 )  _exist++;

    if( _exist == 0 )
    {
        _g->_clusters->_list[_g->_clusters->_numOfObjects] = onewNode();
        strcpy(_g->_clusters->_list[_g->_clusters->_numOfObjects]->_name, _name);
        _result = ++(_g->_clusters->_numOfObjects);
    }

    return _result;
}

/** \brief: graph domain, finds the clusters running index in the cluster list according to clusters name
 *
 * \param: graph* _g, a pointer to the graph object
 * \param: const char* _name, the name of a given cluster
 * \return:int, the number of clusters index in the list, or -1 if does not exist
 *
 */
int gfindcn(graph* _g, const char* _name)
{
    int _result = -1;
    int _i = 0;
    while( _i < _g->_clusters->_numOfObjects)
        if( (strcmp(_name, _g->_clusters->_list[_i]->_name)) == 0 )
            return _i;
        else
            _i++;

    return _result;
}

/** \brief: graph domain, find previous cluster of a specific cluster node exist before a given cluster node
 *
 * \param: graph* _g, pointer to a graph object
 * \param: node* _cluster, pointer  to a node object, represent a cluster
 * \param: char* _name, the of cluster the exist before the cluster _c
 * \return: int
 *
 */
int gfindpc(graph* _g, node* _c)
{
    int _i;
    int _result = 0;
    char* _cluster = NULL;

    if( _c == NULL ) return _result;
    else if ( (strcmp(_c->_name, "entry") == 0) || (strcmp(_c->_name, "for_body") == 0) || (strcmp(_c->_name, "for_end") == 0) )
            return _result;

    for(_i = 0; _i < _c->_indegree; _i++)
    {
        _cluster = _c->_inEdgesName[_i];
        if( strcmp(_cluster, "for_body") == 0 ){ return for_body;}
        else if( strcmp(_cluster, "entry") == 0 ){ return entry;}
        else if( strcmp(_cluster, "for_end") == 0 ){ return for_end;}
    }
    if( _cluster != NULL )
        _result = gfindpc(_g, ofindbn(_g->_clusters, _cluster));

    return _result;
}

#endif
