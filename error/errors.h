#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <stdio.h>

void    MEMORY_ALLOCATION_ERROR             (char * type_name, char * function_name, char * file_name);
void    UNINITIALIZED_STRUCTURE_ERROR       (char * type_name, char * function_name, char * file_name);
void    NULL_STRING_ERROR                   (char * function_name, char * file_name);

#endif /* __ERRORS_H__*/



