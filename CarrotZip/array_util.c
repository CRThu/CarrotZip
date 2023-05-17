#include "array_util.h"

#ifdef __cplusplus
extern "C" {
#endif

	bool array_equal_uint32(uint32_t* arr1, uint32_t* arr2, uint32_t len)
	{
		bool equal = 1;
		for (uint32_t i = 0; i < len; i++)
		{
			if (arr1[i] != arr2[i])
			{
				equal = 0;
				break;
			}
		}
		return equal;
	}

#ifdef __cplusplus
}
#endif
