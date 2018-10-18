/* version 16.10.2018 */

#ifndef QRAM_H_INCLUDED
#define QRAM_H_INCLUDED

#include "dram.h"
#include "utilities.h"

/** \brief: dual RAM (memory) domain, write the title of the memory module to the file
            input/output arrangement: []input data, [] input addresses, []load conditions, clock, [] ouput data
 *
 * \param: const char* _fileName, the name of the output file
 * \param: int _lcount, the number of loads to load from
 * \param: int _scount, the number of stores to store to
 * \return: void
 *
 */
void qrwriteTitle(const char* _fileName, int _lcount, int _scount)
{
    /*int _i;*/
    FILE* _file;

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    fprintf(_file, "module %s(\n", "qram");

    fprintf(_file, "input[%s-1:0]%s%d, %s%d, ", WIDTH, _inputTypes[data_in], 1, _inputTypes[data_in], 2 );
    /*fprintf(_file, "input[%s-1:0]%s%d, %s%d, ", WIDTH, _inputTypes[data_in], 1, _inputTypes[data_in], 2 );*/
    fprintf(_file, "input[%s-1:0]%s%d, %s%d, %s%d, %s%d, ", WIDTH, _inputTypes[address], 1, _inputTypes[address], 2,
                                                                  _inputTypes[address], 3, _inputTypes[address], 4);
    fprintf(_file, "input %s%d, %s%d, %s,\n", _inputTypes[store], 1, _inputTypes[store], 2, "clk" );
    fprintf(_file, "output %s[%s-1:0]%s%d, %s%d, %s%d, %s%d\n", REG, INT_32, _inputTypes[data_out], 1, _inputTypes[data_out], 2,
                                                                 _inputTypes[data_out], 3, _inputTypes[data_out], 4);

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
void qrwriteMemory(const char* _fileName, int _lcount, int _scount, int _size)
{
    /*int _i;*/
    FILE* _file;

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    fprintf(_file, "wire [%s - 1:0] addr_x, addr_y;\n", WIDTH );

    /* always block for reading from memory */
    /* first port:
    fprintf(_file, "//reading memory");
    fprintf(_file, "\nalways @(posedge clk) begin\n");*/

    /* if not load (write enable) then store */
    fprintf(_file, "assign %s%c = (%s%d) ? %s%d : %s%d;\n", _inputTypes[address], 'x', _inputTypes[store], 1, _inputTypes[address], 1, _inputTypes[address], 3);
    fprintf(_file, "assign %s%c = (%s%d) ? %s%d : %s%d;\n\n", _inputTypes[address], 'y', _inputTypes[store], 2, _inputTypes[address], 2, _inputTypes[address], 4);
    fprintf(_file, "dram mem1 (");
    fprintf(_file, "%s%d, %s%d, ", _inputTypes[data_in], 1, _inputTypes[data_in], 2 );
    fprintf(_file, "%s%s, %s%s, ", _inputTypes[address], "x", _inputTypes[address], "y");
    fprintf(_file, "%s%d, %s%d, %s,", _inputTypes[load], 1, _inputTypes[load], 2, "clk" );
    fprintf(_file, "%s%d, %s%d", _inputTypes[data_out], 1, _inputTypes[data_out], 2);
    fprintf(_file, ");\n");

    fprintf(_file, "dram mem2 (");
    fprintf(_file, "%s%d, %s%d, ", _inputTypes[data_in], 1, _inputTypes[data_in], 2 );
    fprintf(_file, "%s%s, %s%s, ", _inputTypes[address], "x", _inputTypes[address], "y");
    fprintf(_file, "%s%d, %s%d, %s,", _inputTypes[load], 1, _inputTypes[load], 2, "clk" );
    fprintf(_file, "%s%d, %s%d", _inputTypes[data_out], 3, _inputTypes[data_out], 4);
    fprintf(_file, ");");


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
FILE* qrcreateMemory(char* _fileName, module* _m)
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

    qrwriteTitle(_fileName, _lcount, _scount);

    /*mwriteIOs(_fileName, _lcount, _scount);*/

    qrwriteMemory(_fileName, _lcount, _scount, _size);
    fclose(_file);
    return _file;
}


#endif
