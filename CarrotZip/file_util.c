#include "file_util.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

	void save_double_arr(char* filename, double* arr, uint32_t len)
	{
		FILE* fp = fopen(filename, "w");
		for (int i = 0; i < len; i++) {
			fprintf(fp, "%lf\n", arr[i]);
		}
		fclose(fp);
	}

	void save_uint32_arr(char* filename, uint32_t* arr, uint32_t len)
	{
		FILE* fp = fopen(filename, "w");
		for (int i = 0; i < len; i++) {
			fprintf(fp, "%lu\n", arr[i]);
		}
		fclose(fp);
	}

#ifdef __cplusplus
}
#endif
