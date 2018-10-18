/* version 16.10.2018 */

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <time.h>
#include <stdarg.h>



void writeToLog(const char* _msg, const char* _funName)
{
    FILE* _log;
    time_t rawtime;
    struct tm *info;
    char _log_time[30];

    time( &rawtime );
    info = localtime( &rawtime );
    sprintf(_log_time, asctime(info));

    if ((_log = fopen("log.txt", "a")) != NULL)
    {
        fprintf(_log, "* at %s: %s in function: %s\n", asctime(info), _msg, _funName);

    }
    if ( _log != NULL )
        fclose(_log);
}

void writetolog(const char* _fmt, ...)
{
    FILE* _logfile;
    va_list _arglist;
    va_start(_arglist, _fmt);

    if ( (_logfile = fopen("log.txt", "w")) != NULL )
        vfprintf(_logfile, _fmt, _arglist);

    va_end(_arglist);
    fclose(_logfile);
}

FILE* lcreate()
{
    FILE* _file;
    char* _fname = "log.txt";
    time_t rawtime;
    struct tm *info;

    if( (_file = fopen(_fname, "w")) == NULL )
        exit(1);

    time( &rawtime );
    info = localtime( &rawtime );
    fprintf(_file, "%s: ", "open log file..");
    fprintf(_file, "%s\n", asctime(info));

    fclose(_file);

    return _file;
}

#endif
