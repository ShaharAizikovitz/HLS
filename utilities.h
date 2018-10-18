/* version 16.10.2018 */

#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED
#include <stdbool.h>
#include <string.h>
#include "log.h"
#include "definitions.h"

typedef enum intypes { load, store, data_in, address, data_out, reg_out } intypes;
const char* _inputTypes[] = { "load_", "store_", "data_in_", "addr_", "data_out_", "out_reg_" };

/** \brief: scan domain, convert cluster name to integer, indicating state
 *
 * \param: const char *_cname, the cluster name
 * \return: integer, indicating state
 *
 */
 int c2i(const char *_name)
 {
     int _result = -1;
     if( (strcmp(_name, "entry") == 0) ) _result = entry;
     else if ( (strcmp(_name, "for_body") == 0) ) _result = for_body;
     else if ( (strcmp(_name, "for_end") == 0) ) _result = for_end;

     return _result;
 }


/** \brief: utilities domain, set suffix to .v file
 *
 * \param: char* _name, a pointer to the name
 * \return: void
 *
 */
char* usets(char* _name)
{
    /* erase the old suffix */
    char* _result;
    char* _ptr = NULL;
    char _p = '.';

    _result = (char*)malloc(sizeof(char) * strlen(_name) + 1);
    strcpy(_result, _name);
    _ptr = strrchr(_result, _p);
    /*strcat(_ptr, ".v");*/
    strcpy(_ptr, ".v");

    return _result;
}

char* int_to_bin(int k)
{
   int _size = 32;
   static char bin[32];
   int _index;

   for(_index = 0; _index < _size; _index++) sprintf(bin + _index, "0");

   while (k >= 0)
   {
        if(k % 2) bin[_size - 1] = '1';
        else bin[_size - 1] = '0';
        _size--;
        k = k / 2;

        if( k == 0 )
            return (bin);
   }
   return bin;
}



int isAlphaBeticChar(char _c)
{
    return (_c >= 'a' && _c< 'z') || (_c >= 'A' && _c< 'Z');
}

char* createString(char* _functionName, int _size)
{
    char _msg[256];
    char* _result;

    sprintf(_msg, "allocated string of size: %d", _size);

    if ((_result = (char*)malloc(_size + 1)) == NULL)
    {
        printf("cannot allocate memory in function: %s", _functionName);
        exit(1);
    }
    return _result;
}

#endif
