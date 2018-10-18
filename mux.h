/* version 16.10.2018 */

#ifndef MUX_H_INCLUDED
#define MUX_H_INCLUDED

#include "file.h"
#include "definitions.h"

/** \brief: module domain, prints the modules title AND constants declarations to the output file
 *
 * \param: const char* _fileName, the output file name
 * \param: module* _m, a pointer to the struct holding the modules info
 * \return: void
 *
 */
void muwriteTitle(const char* _fileName, module* _m)
{
    char _name[20];
    int _lcount = 0, _i;
    lnode *_ls;
    FILE* _file;                                                /* pointer to the file to write to */

    if( (_file = fopen(_fileName, "a")) == NULL )
        exit(1);

    /*copy just the name of the file without the .v suffix, and print it to the file
	  as the title of the module */
    memcpy(_name, &_fileName[0], strlen(_fileName) - 2);
	_name[strlen(_fileName) - 2] = '\0';
    fprintf(_file, "module %s(\n", _name);

    /* store values ports */
    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sinput [%s-1:0] store_val_%d,\n", "", INT_32, _ls->_value);
        _ls = _ls->_next;
    }

    /* store addresses ports */
    _ls = _m->_store;
    while(_ls != NULL )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sinput [%s-1:0] store_add_%d,\n", "", INT_32, _ls->_value);
        _ls = _ls->_next;
    }

    /* load addresses output ports, at most MAX_NUM_LOADS */
    _ls = _m->_load;
    while(_ls != NULL && _lcount < MAX_NUM_LOADS )
    {
        if( _ls->_value == -1 ) break;
        fprintf(_file, "%5sinput [%s-1:0] load_add_%d,\n", "", INT_32, _ls->_value);
        _ls = _ls->_next;
        _lcount++;
    }

    /* output ports */
    for(_i = 0; _i < MAX_NUM_LOADS; _i++)
    {
        fprintf(_file, "%5soutput reg[%s-1:0] out_add_%d,\n", "", INT_32, _i+1);
    }

    for(_i = 0; _i < MAX_NUM_STORE; _i++)
    {
        /*fprintf(_file, "%5soutput reg[%s-1:0] out_store_add_%d,\n", "", INT_32, _i+1);*/
        fprintf(_file, "%5soutput reg[%s-1:0] out_store_val_%d,\n", "", INT_32, _i+1);
    }

    /* output ports: store conditions */
    fprintf(_file, "%5soutput reg %s, %s,\n", "", STORE1, STORE2);

    /* input ports: clock and state */
    fprintf(_file, "%5sinput [%s-1:0] %s,\n%5sinput %s\n", "", INT_2, STATE, "", CLK);
    fprintf(_file, ");\n\n");

    fclose(_file);
}

/** \brief: multiplexer domain, write the multiplexer logic into the target file ("mux.v")
 *
 * \param: const char *_mux, name of the file
 * \param: module *_m, pointer to the module object
 * \return: void
 *
 */
void muwritem(const char *_mux, module *_m)
{
    int _lcount = 0, _scount = 0, _index;
    int _prtcount = 0;
    lnode *_s, *_l;
    FILE* _file;

    if((_file = fopen(_mux, "a")) == NULL )
        exit(1);

    /* print clock and reset inputs */
    fprintf(_file, "%5salways @(posedge %s) begin\n", "", CLK);
    fprintf(_file, "%5scase(%s)\n", "", STATE);

    /********************************** entry state ************************************/
    fprintf(_file, "%5s01:\n", "");
    fprintf(_file, "%10sbegin\n", "");
    _l = _m->_load;
    _s = _m->_store;
    while( _s != NULL )
    {
        /* find a store node in the entry cluster */
        _index = _s->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_s->_value]->_clstr->_name, "entry") == 0 )
            {
                _scount++;
                _prtcount++;
                if( _scount > 2)
                {
                    printf("error!!");
                    exit(1);
                }
                fprintf(_file, "%10sout_add_%d <= %s%d;\n", "",_prtcount, port_attr[_store_add], _index);
                fprintf(_file, "%10sout_store_val_%d <= %s%d;\n", "", _scount, port_attr[_store_val], _index);
            }
        }
        _s = _s->_next;
    }
    while( _l != NULL )
    {
        /* find a load node in the entry cluster */
        _index = _l->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_l->_value]->_clstr->_name, "entry") == 0 )
            {
                fprintf(_file, "%10sout_add_%d <= %s%d;\n", "", ++_prtcount, port_attr[_load_add], _index);
                _lcount++;
            }
        }
        _l = _l->_next;
    }
    /* count number of loads = 1 output load1 is true, more than 1 : error */
    if( _scount == 0 ) { fprintf(_file, "%10sstore_1 <= 0;\n", ""); fprintf(_file, "%10sstore_2 <= 0;\n", ""); }
    else if( _scount == 1 ) { fprintf(_file, "%10sstore_1 <= 1;\n", ""); fprintf(_file, "%10sstore_2 <= 0;\n", ""); }
    else if( _scount == 2 ) { fprintf(_file, "%10sstore_1 <= 1;\n", ""); fprintf(_file, "%10sstore_2 <= 1;\n", ""); }
    fprintf(_file, "%10send\n", "");

    /******************************* for loop state **********************************/
    fprintf(_file, "%5s10:\n", "");
    fprintf(_file, "%10sbegin\n", "");
    _scount = _lcount = _prtcount = 0;
    _l = _m->_load;
    _s = _m->_store;
    while( _s != NULL )
    {
        /* find a store node in the entry cluster */
        _index = _s->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_s->_value]->_clstr->_name, "for_body") == 0 )
            {
                _scount++;
                _prtcount++;
                if( _scount > 2)
                {
                    printf("error!!");
                    exit(1);
                }
                fprintf(_file, "%10sout_add_%d <= %s%d;\n", "", _prtcount, port_attr[_store_add], _prtcount);
                fprintf(_file, "%10sout_store_val_%d <= %s%d;\n", "", _scount, port_attr[_store_val], _index);
            }
        }
        _s = _s->_next;
    }
    while( _l != NULL )
    {
        /* find a load node in the entry cluster */
        _index = _l->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_l->_value]->_clstr->_name, "for_body") == 0 )
            {
                fprintf(_file, "%10sout_add_%d <= %s%d;\n", "", ++_prtcount, port_attr[_load_add], _index);
                _lcount++;
            }
        }
        _l = _l->_next;
    }
    /* count number of loads = 1 output load1 is true, more than 1 : error */
    if( _scount == 0 ) { fprintf(_file, "%10sstore_1 <= 0;\n", ""); fprintf(_file, "%10sstore_2 <= 0;\n", ""); }
    else if( _scount == 1 ) { fprintf(_file, "%10sstore_1 <= 1;\n", ""); fprintf(_file, "%10sstore_2 <= 0;\n", ""); }
    else if( _scount == 2 ) { fprintf(_file, "%10sstore_1 <= 1;\n", ""); fprintf(_file, "%10sstore_2 <= 1;\n", ""); }

    fprintf(_file, "%10send\n", "");

    /*********************************** for end state ******************************/
    fprintf(_file, "%5s11:\n", "");
    fprintf(_file, "%10sbegin\n", "");
    _scount = _lcount = _prtcount = 0;
    _l = _m->_load;
    _s = _m->_store;
    while( _s != NULL )
    {
        /* find a store node in the entry cluster */
        _index = _s->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_s->_value]->_clstr->_name, "for_end") == 0 )
            {
                _scount++;
                _prtcount++;
                if( _scount > 2)
                {
                    printf("error!!");
                    exit(1);
                }
                fprintf(_file, "%10sout_add_%d <= %s%d;\n", "",_prtcount, port_attr[_store_add], _index);
                fprintf(_file, "%10sout_store_val_%d <= %s%d;\n", "", _scount, port_attr[_store_val], _index);
            }
        }
        _s = _s->_next;
    }
    while( _l != NULL )
    {
        /* find a load node in the entry cluster */
        _index = _l->_value;
        if( _index != -1 )
        {
            if( strcmp(_m->_graph->_nodes->_list[_l->_value]->_clstr->_name, "for_end") == 0 )
            {
                fprintf(_file, "%10sout_add_%d <= %s%d;\n", "", ++_prtcount, port_attr[_load_add], _index);
                _lcount++;
            }
        }
        _l = _l->_next;
    }
    /* count number of loads = 1 output load1 is true, more than 1 : error */
    if( _scount == 0 ) { fprintf(_file, "%10sstore_1 <= 0;\n", ""); fprintf(_file, "%10sstore_2 <= 0;\n", ""); }
    else if( _scount == 1 ) { fprintf(_file, "%10sstore_1 <= 1;\n", ""); fprintf(_file, "%10sstore_2 <= 0;\n", ""); }
    else if( _scount == 2 ) { fprintf(_file, "%10sstore_1 <= 1;\n", ""); fprintf(_file, "%10sstore_2 <= 1;\n", ""); }
    fprintf(_file, "%10send\n", "");

    fprintf(_file, "\n%5s//end case \n%5sendcase", "", "");
    fprintf(_file, "\n%5s//end always block \n%5send", "", "");

    fclose(_file);
}


/** \brief: multiplexer domain, writes the multiplexer itself to the multiplexer file (mux.v)
 *
 * \param: module *_m, pointer to the module object
 * \return: void
 *
 */
FILE* muwriteMux(module *_m)
{
    const char* _mux = "mux.v";
    FILE* mux;

    if((mux = fopen(_mux, "w")) == NULL )
        exit(1);
    fclose(mux);

    fwriteHeader(_mux, "shahar");
    muwriteTitle(_mux,  _m);
    muwritem(_mux,  _m);

    if( (mux = fopen(_mux, "a")) == NULL )
        exit(1);
    fprintf(mux, "\nendmodule");
    fclose(mux);

    return mux;
}

#endif
