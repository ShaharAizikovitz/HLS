/* version 16.10.2018 */

#ifndef DRAM_H_INCLUDED
#define DRAM_H_INCLUDED

#include "definitions.h"
#include "utilities.h"
#include "object.h"
#include "file.h"



/** \brief: dual RAM (memory) domain, write the title of the memory module to the file
            input/output arrangement: []input data, [] input addresses, []load conditions, clock, [] ouput data
 *
 * \param: const char* _fileName, the name of the output file
 * \param: int _lcount, the number of loads to load from
 * \param: int _scount, the number of stores to store to
 * \return: void
 *
 */
void drwriteTitle(const char* _fileName, int _lcount, int _scount)
{
    /*int _i;*/
    FILE* _file;

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    fprintf(_file, "module %s(\n", DRAM);

    fprintf(_file, "input[%s-1:0]%s%d, %s%d, ", WIDTH, _inputTypes[data_in], 1, _inputTypes[data_in], 2 );
    fprintf(_file, "input[%s-1:0]%s%d, %s%d, ", WIDTH, _inputTypes[address], 1, _inputTypes[address], 2 );
    fprintf(_file, "input %s%d, %s%d, %s,\n", _inputTypes[load], 1, _inputTypes[load], 2, "clk" );
    fprintf(_file, "output %s[%s-1:0]%s%d, %s%d\n", REG, INT_32, _inputTypes[data_out], 1, _inputTypes[data_out], 2 );

    fprintf(_file, ");\n\n");
    fclose(_file);
}

/** \brief: memory domain, write the memory functionality to file given by name. according to number of loads and stores
 *
 * \param: const char* _fieName, the name of the file to write to
 * \param: int _lcount, the number of loads
 * \param: int _scount, the number of stores
 * \param: int _size, the total size needed to complete the task
 * \return: void
 *
 */
void drwriteMemory(const char* _fileName, int _lcount, int _scount, int _size)
{
    /*int _i;*/
    FILE* _file;

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    fprintf(_file, "reg [%s - 1:0] memory[%s-1:0];\n", WIDTH, LINES);

    /* always block for reading from memory */
    /* first port: */
    fprintf(_file, "//reading memory");
    fprintf(_file, "\nalways @(posedge clk) begin\n");

    /* if not load (write enable) then store */
    fprintf(_file, "if(!%s%d) ", _inputTypes[load], 1);
    fprintf(_file, "memory[%s%d] <= %s%d;\n", _inputTypes[address], 1, _inputTypes[data_in], 1);
    fprintf(_file, "else %s%d <= memory[%s%d];\n",_inputTypes[data_out], 1, _inputTypes[address], 1);
    fprintf(_file, "end\n");
    /*fprintf(_file, "end");*/

    /* second port: */
    fprintf(_file, "\nalways @(posedge clk) begin\n");
    /* first port: if not load (write enable) then store */
    fprintf(_file, "if(!%s%d) ", _inputTypes[load], 2);
    fprintf(_file, "memory[%s%d] <= %s%d;\n", _inputTypes[address], 2, _inputTypes[data_in], 2);
    fprintf(_file, "else %s%d <= memory[%s%d];\n",_inputTypes[data_out], 2, _inputTypes[address], 2);
    fprintf(_file, "end\n");
    /*fprintf(_file, "end");*/


    fprintf(_file, "\nendmodule");
    fclose(_file);
}

/** \brief: memory domain, create a new memory file
 *
 * \param: char* _fileName, the name of the output file
 * \param: module* _m, a pointer to a module object
 * \return: FILE*, a pointer to the file created
 *
 */
FILE* drcreateMemory(char* _fileName, module* _m)
{
    int _size;
    int _lcount, _scount;
    FILE* _file;

    if( (_file = fopen(_fileName, "w")) == NULL )
        exit(1);
    _lcount = _m->_numOfLoad;
    _scount = _m->_numOfStore;
    _size = _m->_arraysList->_totalSpace;

    fwriteHeader(_fileName, "creator");

    drwriteTitle(_fileName, _lcount, _scount);

    /*mwriteIOs(_fileName, _lcount, _scount);*/

    drwriteMemory(_fileName, _lcount, _scount, _size);
    fclose(_file);
    return _file;
}

#endif
