/* version 16.10.2018 */

/*
conventions:
------------
1. memory files are called "mem_" concatenated with fileName
2. this version has memory built into the module
3. version 16.10.2018
*/

#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "memory.h"
#include "dram.h"
#include "qram.h"
#include "file.h"
#include "mux.h"
#include "module.h"
#include "scan.h"
#include "log.h"


int main(int argc, char* argv[])
{
	int _result = 0;
    char* _fileName = argv[1];
    char* _name;
    FILE* _outputFile;
    FILE* _logFile;
    FILE* _dmemory;
    FILE* _qmemory;
    FILE* _mux;
    /*FILE* _clock;*/
    graph* _graph;
    module* _module;

    if(  argc < 2 )
    {
		printf("no input..\n");
		_fileName = "f4.txt";
		_name = usets(_fileName);
	}

	else
	{
		_name = usets(argv[1]);
		printf("output: %s\n", argv[1]);
		printf("output file name:%s\n", _name);
	}
    /*
    *       open log file
    *       -------------
    */
    _logFile = lcreate();

    /*
    *       scan file
    *       ---------
    */
    _graph = sscanFile(_fileName);

    /*
    *       extract data from graph
    *       -----------------------
    */
	/*printf("end of module\n"); */
    _module = mnewModule(_graph);
    _outputFile = mwriteModule(_name, _module);
    if ((_module->_numOfLoad + _module->_numOfStore) > 4)
    {
        printf("insuficient memory access..");
        /*exit(1);*/
    }


    /*
    *       memory creation
    *       ---------------
    */
    _dmemory = drcreateMemory("dram.v", _module);
    _qmemory = qrcreateMemory("qram.v", _module);

    /*
    *       memory creation
    *       ---------------
    */
    _mux = muwriteMux(_module);

    /*
    *       clean up
    *       --------
    */
    if ( _outputFile != NULL )
        fclose(_outputFile);
    else if( _dmemory != NULL )
        fclose(_dmemory);
    else if( _qmemory != NULL )
        fclose(_qmemory);
    else if ( _logFile != NULL )
        fclose(_logFile);
    /*else if (_clock != NULL )
        fclose(_clock);*/
    else if( mfreeModule(_module) == 0 )
        printf("success !");
	else
	{
		printf("failure !!");
		_result = 1;
	}
    return _result;
}
