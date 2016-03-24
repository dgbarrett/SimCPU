#include "errors.h"

/*These functions all print standardized error messages for common error situations*/

void MEMORY_ALLOCATION_ERROR(char * type_name, char * function_name, char * file_name)
{
    fprintf(stderr, "ERROR: Failed to allocate memory to %s \n\t In Function: %s \n\t In File: %s \n\n", type_name, function_name, file_name);
}

void UNINITIALIZED_STRUCTURE_ERROR(char * type_name, char * function_name, char * file_name)
{
    fprintf(stderr, "ERROR: Uninitialized %s passed to function \n\t In Function: %s \n\t In File: %s \n\n", type_name, function_name, file_name);
}

void NULL_STRING_ERROR(char * function_name, char * file_name)
{
    fprintf(stderr, "ERROR: Uninitialized char * passed to function \n\t In Function: %s \n\t In File: %s \n\n", function_name, file_name);
}

