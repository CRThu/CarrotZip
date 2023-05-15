#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	// Your code goes here
	void save_double_arr(char* filename, double* arr, uint32_t len);
	void save_uint32_arr(char* filename, uint32_t* arr, uint32_t len);

#ifdef __cplusplus
}
#endif


#endif