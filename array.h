/* version 16.10.2018 */

#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#include "objects.h"


mArray* newmArray(char* _name, char* _type, int _size, int _address)
{
    mArray* _result = (mArray*)malloc(sizeof(mArray));
    _result->_name = (char*)malloc(sizeof(strlen(_name)));
    _result->_type = (char*)malloc(sizeof(strlen(_type)));
    _result->_size = _size;
    _result->_address = _address;
    strcpy(_result->_name, _name);
    strcpy(_result->_type, _type);
    return _result;
}

/*struct mArrayList{
    int _count;
    int _totalSpace;
    mArray** _list;
};
*/
mArrayList* newmArrayList(int _maxsize)
{
    mArrayList* _result = (mArrayList*)malloc(sizeof(mArrayList));
    _result->_list = (mArray**)malloc(sizeof(mArray) * _maxsize);
    _result->_count = 0;
    _result->_totalSpace = 0;
    return _result;
}

void freemArray(mArray* _a)
{
    free(_a->_name);
    free(_a->_type);
}

int addmArray(mArrayList* _alist, mArray* _arr)
{
    _alist->_list[_alist->_count] = _arr;
    _alist->_totalSpace += _arr->_size;
    return _alist->_count++;
}

void freemArrayList(mArrayList* _a)
{
    int _i, _size = _a->_count;

    for(_i = 0; _i < _size; _i++)
        freemArray(_a->_list[_i]);

    free(_a);
}

/** \brief: array domain, find array by name, return the address of a given name
 *
 * \param: const char* _name, the array name
 * \param: mArrayList* _list, a pointer to the array list
 * \return: int, the array address
 *
 */
int afindabn(mArrayList* _list, const char* _name)
{
    int _i = 0;
    int _address = 0;

    while( strcmp(_name, _list->_list[_i]->_name) != 0)
            _address += _list->_list[_i]->_size;

    return _address;
}

#endif
