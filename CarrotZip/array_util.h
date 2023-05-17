#ifndef __ARRAY_UTIL_H__
#define __ARRAY_UTIL_H__

#include <stdint.h>
#include <stdbool.h>

#define PI 3.14159265

#ifdef __cplusplus
extern "C" {
#endif

	bool array_equal_uint32(uint32_t* arr1, uint32_t* arr2, uint32_t len);


#ifdef __cplusplus
}
#endif


#endif