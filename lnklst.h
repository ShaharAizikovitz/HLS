/* version 16.10.2018 */

#ifndef LNKLST_H_INCLUDED
#define LNKLST_H_INCLUDED

#include <stdio.h>
#include "objects.h"

lnode* lnewlnode()
{
    lnode* _result;
    if( (_result = (lnode*)malloc(sizeof(lnode))) == NULL )
        exit(1);
    _result->_value = -1;
    _result->_state = -1;
    _result->_next = NULL;
    _result->_prev = NULL;
    return _result;
}

int lfreelnode(lnode* _node)
{
    if( _node == NULL ) return 0;
    if( _node->_next != NULL )
    {
        _node->_next = NULL;
        _node->_prev = NULL;
        free(_node);
        return 1;
    }
    return 1 + lfreelnode(_node->_next);
}

int laddtail(lnode* _list, int _num, int _count, int _state)
{
    int _result;
    lnode* _new;

    /* the first node is empty */
    if( _list->_value == -1 )
    {
        _list->_state = _state;
        _list->_value = _num;
        return (_result = 1);
    }
    else if( _list->_next == NULL )
    {
        if( (_new = lnewlnode()) == NULL ) exit(1);
        _new->_state = _state;
        _new->_value = _num;
        _list->_next = _new;
        _new->_prev = _list;
        return (_result = 2);
    }
    else if( _list->_value != _num )
        return (_result = 1 + laddtail(_list->_next, _num, _count, _state));
    else
        return _count;
}

lnode* laddhead(lnode* _list, lnode* _new)
{
    _list->_prev = _new;
    _new->_next = _list;
    return _new;
}

void lprintl(const char* _title, lnode* _list)
{
    printf("%s:\n", _title);
    while(_list != NULL )
    {
        printf("%d->", _list->_value);
        _list = _list->_next;
    }
    printf("|\n");
}

void flprintl(const char* _fname, lnode* _list)
{
    FILE* _file;

    if( (_file = fopen(_fname, "a")) == NULL )
        exit(1);
    if( _list == NULL ) return;
    fprintf(_file, "%d->", _list->_value);
    printf("%d->", _list->_value);
    if( _list->_next != NULL )
    {
        lprintl("", _list->_next);
        flprintl(_fname, _list->_next);
    }
    else
    {
        printf("|\n");
        fprintf(_file, "|\n");
    }

    fclose(_file);
}

#endif
