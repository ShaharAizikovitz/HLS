/* version 16.10.2018 */

#ifndef SCAN_H_INCLUDED
#define SCAN_H_INCLUDED

#include "definitions.h"
#include "utilities.h"
#include "graph.h"
#include <stdlib.h>




/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
int snumOfEdges(const char* _fileName)
{
    int _esource, _etarget;
    int _result = 0;
    char _line[LINE_LENGTH];
    char _arrbody[1];
    char _arrhead[1];
    FILE* _file;

    if ((_file = fopen(_fileName, "r+")) != NULL)
    {

        while (fgets(_line, sizeof(_line), _file) != '\0')
        {
            if ( sscanf(_line, "%d %c %c %d", &_esource, _arrbody, _arrhead ,&_etarget ) && *_arrbody == '-' && *_arrhead == '>')
                _result++;
        }
    }


    return _result;
}



/** \brief: count the number of clusters in the input file pointed by _fileName
 *
 * \param: const char* _fileName, the string that holds the name of the input file
 * \return: int, number of the found objects
 *
 */
int snumberOfClusters(const char* _fileName)
{
    int _result = 0;
    char* _name;
    char _line[LINE_LENGTH];
    FILE* _file;


    if ((_file = fopen(_fileName, "r+")) == NULL)
        exit(1);

    while (fgets(_line, sizeof(_line), _file) != '\0')
        if ( (_name = strstr(_line, "cluster")) )
            _result++;

    return _result;
}


/** \brief: count the number of objects in the input file pointed by _fileName
 *
 * \param: const char* _fileName, the string that holds the name of the input file
 * \return: int, number of the found objects
 *
 */
int snumOfNodes(const char* _fileName)
{
    int _result = 0, _count = 0;
    char _line[LINE_LENGTH];
    char* _bracket;
    char* _shape;
    FILE* _file;


    if ((_file = fopen(_fileName, "r+")) == NULL)
        exit(1);

    _shape = (char*)malloc(sizeof(char)*6);
    _bracket = (char*)malloc(sizeof(char));

    while (fgets(_line, sizeof(_line), _file) != '\0')
    {
        if (sscanf(_line, "%d %c %s", &_count, _bracket, _shape))
        {
            /* if bracket is '[' and 'shape' string follows*/
            if (*_bracket == 91 && strcmp(_shape, "shape"))
                _result++;

        }
    }

    /*clean up the mess*/
    fclose(_file);
    _bracket = NULL;
    _shape = NULL;
    return _result;
}


/** \brief: add a a string to node in/output port with a given name
 *
 * \param: node* _node, a pointer to the node being updated
 * \param: char* _port, the string name of the port
 * \param: char* _addr, a string that represent the port address in hex.
 * \param: int _state, 1. input, 2. name/operator, 3. output
 * \return: void
 *
 */
void saddPort(node* _node, char* _port, char* _addr, int _state)
{
    int i = 0;

    switch (_state)
    {
        case 1:/*input*/
            while (1)
            {
                /* find first free input string in object in. ports */
                if (*(_node->_inEdgesName[i]) == '\0')
                {
                    sprintf(_node->_inEdgesName[i], _port);
                    break;
                }
                else
                    i++;
            }
            /*increment indegree*/
            _node->_indegree++;
            break;

        case 2:/*name*/
            sprintf(_node->_name, _port);
            break;

        case 3:/*output*/
            while (1)
            {
                /* find first free output string in object out. ports */
                if (*(_node->_outEdgesName[i]) == '\0')
                {
                    sprintf(_node->_outEdgesName[i], _port);
                    break;
                }
                else
                    i++;
            }
            break;
    }

}


/** \brief: scan domain, create a new node object AND create and add its appropriate cluster to the graph clusters list
 *
 * \param: graph* _g, a pointer to the graph object
 * \param: char* _inpString, the line found
 * \param: int _num, the node number in the nodeList
 * \return: node*, a pointer to the new node
 *
 */
node* screateNode(graph* _g, const char* _inpString, int _num, int _clstrNm)
{
    int _portNum = -1;
    char _char;
    char _openB[1];
    char _opentriB[1];
    char _name[MAX_CHAR_PER_PORT];
    char _port[MAX_CHAR_PER_PORT];
    node* _result;

    _result = onewNode();
    _result->_number = _num;
    _result->_clstr = _g->_clusters->_list[_clstrNm - 1];
    /*_result->_state = c2i(_result->_clstr->_name);*/


    /* skip the '{{{' pattern and the white spaces*/
    _inpString += 3;
    while ((_char = *(_inpString++)) == ' '){}

    while(*(_inpString) != '\0')
    {
        /* a '{<[address]>[string1]|..|<[address]>[string5]}' pattern  */
        if( sscanf(_inpString, "%c%c", _openB, _opentriB) != 0 && *_openB == '{' && *_opentriB == '<')
        {
            while(*(_inpString) != '}')
            {
                sprintf(_port, "%s", EMPTY_STRING);
                while(*(_inpString++) != '>' ) {}
                while(*(_inpString) != '|' && *(_inpString) != '}')
                {
                    sprintf(_port + strlen(_port), "%c", *_inpString);
                    _inpString++;
                }
                saddPort(_result, _port, "", _portNum);
            }
        }
        else if (*(_inpString) == ' ') _inpString++;
        else if (*(_inpString) == '|')
        {
            _portNum++;
            _inpString++;
        }

        /* a { [string] } pattern */
        else if (*_inpString == '{' && *(_inpString + 1) == ' ' && isAlphaBeticChar(*(_inpString + 2)))
        {
            _portNum = 2;
            _inpString += 2;
            sprintf(_name, "%s", EMPTY_STRING);
            while(*(_inpString) != '}')
            {
                if(*(_inpString) == ' ' && *(_inpString + 1) == '}') break;
                else if(*(_inpString) == ' ' )
                {
                    sprintf(_name + strlen(_name), "%c", '_');
                    _inpString++;
                }
                else
                    sprintf(_name + strlen(_name), "%c", *(_inpString++));
            }

            strcpy(_result->_name, _name);
        }

        /* a {[string]|[string]..} pattern */
        else if (*_inpString == '{' && isAlphaBeticChar(*(_inpString + 1)))
        {
            _inpString++;
            sprintf(_name, "%s", EMPTY_STRING);
            if( _portNum < 3 ) _portNum++;
            while(*(_inpString) != '}' )
            {
                sprintf(_name + strlen(_name), "%c", *(_inpString++));
                if(*(_inpString) == '|' )
                {
                    saddPort(_result, _name, "", _portNum);
                    sprintf(_name, "%s", EMPTY_STRING);
                    _inpString++;
                }
            }
        }

        /* a [string] pattern */
        else if( isAlphaBeticChar(*_inpString) )
        {
            sprintf(_port, "%s", EMPTY_STRING);
            while(*(_inpString) != '|' && *(_inpString) != '}')
                sprintf(_port + strlen(_port), "%c", *(_inpString++));

            saddPort(_result, _port, "", _portNum);
        }

        else
            _inpString++;
    }

    return _result;
}


/** \brief: scan domain, scan the input file to extract the graph that represent the input
 *
 * \param: const char* _fileName, the string that holds the input fiel name
 * \return: graph*, a pointer to the newly created graph object
 *
 */
graph* sscanFile(const char* _fileName)
{
    FILE* _file;
    FILE* _log;
    graph* _graph;
    node* _newNode;
    node* _cluster;
    char _clusterName[MAX_CHAR_PER_CLUSTER];
    char* _tail;
    char* _head;
    char _shape[20];
    char _bracket[1];
    char _line[LINE_LENGTH];
    int _objNum;
    int _sourceNode = 0, _targetNode = 0;
    int _clstrNum = 0;


    if((_file = fopen(_fileName, "r+")) == NULL)
    {
        printf("no input file found");
        exit(1);
    }


    /* find how many objects in the input file and init. the node List accordingly  */
    _graph = gnewGraph(snumOfNodes(_fileName), snumberOfClusters(_fileName));

    /* scan the file line by line to map clusters and nodes */
    while (fgets(_line, sizeof(_line), _file))
    {
        /* find the nodes cluster and add to cluster list */
        if ( strstr(_line, "subgraph") )
        {
            sscanf(_line + strlen("subgraph cluster_"), "%s", _clusterName);
            _clstrNum = gaddc(_graph, _clusterName);
            _graph->_clusters->_numOfObjects = _clstrNum;
        }

        /*find an int'[' pattern in the beginning of a line*/
        else if (sscanf(_line, "%d%c%s", &_objNum, _bracket, _shape) && *_bracket == '[' && strcmp(_shape, "shape"))
        {
            /* discard branch node that's outside of 'entry' and 'for_body' clusters
            if( strstr(_line, "BR") && ( (strcmp(_clusterName, "for_body") != 0) && (strcmp(_clusterName, "entry") != 0) ) )  continue; */

            /*if a pattern was found, search for the word "shape" right after the '[' char*/
            {
                /* find the location of the string 'label=' and call createObject with the string that starts after 'label=' */
                _newNode = screateNode(_graph, strstr(_line, "label=") + strlen("label="), _objNum, _clstrNum);
                /*_graph->_list[_objNum] = gnewNodeNum(_objNum);*/
                oaddNode(_graph->_nodes, _newNode);
                sprintf(_shape, "%s", EMPTY_STRING);
                sprintf(_clusterName, "%s", EMPTY_STRING);
            }
        }
        /* the first time an edge is found */
        else if( sscanf(_line, "%d->%d", &_sourceNode, &_targetNode ))
            break;
    }
    gadde(_graph, _sourceNode, _targetNode);

    /* scan the rest of the file to find edges between nodes, and edges between clusters */
    while (fgets(_line, sizeof(_line), _file))
    {
        /* end of the input file */
        if(*_line == '}' ) break;

        /* find int->int pattern, i.e. an edge */
        else if ( sscanf(_line, "%d->%d", &_sourceNode, &_targetNode) )
        {
            /* find the location of the strings 'ltail' and 'lhead' in the "line" string */
            _tail = strstr(_line, "ltail");
            _head = strstr(_line, "lhead");

            /* if _ltail and _lhead are null then it's a line represent edge between nodes */
            if( (_tail) == '\0' && (_head) == '\0' )
                gadde(_graph, _sourceNode, _targetNode);

            /* if _ltail and _lhead are NOT null then it's a line represent edge between clusters */
            else if( _tail != '\0' && _head != '\0' )
            {
                /* finds the tail cluster */
                sscanf(_tail + strlen("ltail= cluster_"), "%s", _tail);
                _tail[strlen(_tail) - 2] = '\0';

                /* finds the head cluster */
                sscanf(_head + strlen("lhead= cluster_"), "%s", _head);
                _head[strlen(_head) - 2] = '\0';

                _sourceNode = gfindcn(_graph, _tail);
                _targetNode = gfindcn(_graph, _head);

                /* get a pointer of the source cluster object */
                _cluster = _graph->_clusters->_list[_sourceNode];
                strcpy(_cluster->_outEdgesName[_cluster->_outdegree++], _head);

                /* get a pointer of the target cluster object */
                _cluster = _graph->_clusters->_list[_targetNode];
                strcpy(_cluster->_inEdgesName[_cluster->_indegree++], _tail);
            }
        }
        memset(_line, '0', LINE_LENGTH);
    }

    fprintf((_log = fopen("log.txt", "a")), "\nprint node list\n");
    fclose(_log);
    for(_objNum = 0; _objNum < _graph->_nodes->_size; _objNum++)
        if( _graph->_nodes->_list[_objNum]->_number != -1 )
            ofprint("log.txt", _graph->_nodes->_list[_objNum]);


    fclose(_file);
    /*fprinte("log.txt", _graph->_edges);
    gprintnl("log.txt", _graph);*/
    return _graph;
}

#endif
