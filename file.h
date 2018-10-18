/* version 16.10.2018 */

#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "definitions.h"

char* createTitle(const char* _name, const int _count, ...)
{
    /*char* _result = NULL;*/
    char* _title;
    char* _arg;
    va_list _args;
    int i;
    int _titleLen;

    _titleLen = MODULE_LENGTH + strlen(_name) + 10;
    _title = (char*)malloc(_titleLen);
    _arg = (char*)malloc(10*sizeof(char));
    sprintf(_title, "module ");
    strcat(_title, _name);
    strcat(_title, " (\n");
    va_start(_args, _count);

    printf("_title length: %d", (int)strlen(_title));

    /*iterate through all strings (char*) in va_list _args*/
    for(i=0;i<_count;i++)
    {
        _arg = va_arg(_args, char*);


        /* if first char of _arg is 'o', then an output parameter is present */
        if (_arg[0] == 'o')
        {
            strcat(_title, _arg);

            if (i < _count-1)
                strcat(_title, ",");
        }
        /* input parameter */
        else if (_arg[0] == 'i')
        {
            strcat(_title, _arg);

            if (i < _count-1)
                strcat(_title, ",");
        }
        strcat(_title, "\n");
    }
    strcat(_title, ");\n");
    va_end(_args);

    /*free resources and return result
    free(_port);
    free(_arg);*/

    return _title;
}


/** \brief: writes the hrader of a given (output) file
 *
 * \param: const char* _fileName, the string representing the file name
 * \param: const char* _creator, the string representing the file creator name
 * \return: void
 *
 */
void fwriteHeader(const char* _fileName, const char* _creator)
{
    time_t rawtime;
    struct tm *info;
    FILE* _file;

    if( (_file = fopen(_fileName, "a")) == NULL)
        exit(1);

        time( &rawtime );
        info = localtime( &rawtime );
        fprintf(_file, "//`define timescale 1ns/1ps\n");
        fprintf(_file, "`define int_0 0\n");
        fprintf(_file, "`define int_1 1\n");
        fprintf(_file, "`define int_2 2\n");
        fprintf(_file, "`define int_10 10\n");
        fprintf(_file, "`define int_32 32\n");
        fprintf(_file, "`define int_64 64\n");
        fprintf(_file, "`define mem_size 1024\n");
        fprintf(_file, "//////////////////////////////////////////////////////////////////////////////////\n");
        fprintf(_file, "// Company: \n");
        fprintf(_file, "// Engineer: ");
        fprintf(_file, _creator);
        fprintf(_file, "\n");
        fprintf(_file, "// \n");
        fprintf(_file, "// Create Date: ");
        fprintf(_file, asctime(info));
        fprintf(_file, "\n");
        fprintf(_file, "// Design Name: \n");
        fprintf(_file, "// Module Name: ");
        fprintf(_file, _fileName);
        fprintf(_file, "\n");
        fprintf(_file, "// Project Name: \n");
        fprintf(_file, "// Target Devices: \n");
        fprintf(_file, "// Tool Versions: \n");
        fprintf(_file, "// Description: \n");
        fprintf(_file, "//\n");
        fprintf(_file, "// Dependencies: \n");
        fprintf(_file, "//\n");
        fprintf(_file, "// Revision:\n");
        fprintf(_file, "// Revision 0.01 - File Created\n");
        fprintf(_file, "// Additional Comments:\n");
        fprintf(_file, "//\n//////////////////////////////////////////////////////////////////////////////////\n\n");

    fclose(_file);
}

void printFile(char* _fileName)
{
    FILE* _file;
    char line[256];
    int i;


    if ((_file = fopen(_fileName, "r")) != NULL)
    {
        while (fgets(line, sizeof(line), _file) != NULL)
        {
            for(i=0;i<256;i++)
                printf("%c", line[i]);
        }
        fclose(_file);
    }
}

void printToFile(const char* _fileName, const char* _msg)
{
    FILE* _file;
    _file = fopen(_fileName, "w+");

    if (_file != NULL)
    {
        fprintf(_file, "%s", _msg);
        fclose(_file);
    }

}

#endif
