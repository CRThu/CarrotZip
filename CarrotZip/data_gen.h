#ifndef __DATA_GEN_H__
#define __DATA_GEN_H__

#include <stdint.h>

#define PI 3.14159265

#ifdef __cplusplus
extern "C" {
#endif

	// Your code goes here
	void gen_sine(double* arr, uint32_t len, double fin, double fs, double vpp);
	void adc(uint32_t* arr_out, double* arr_in, uint32_t len, uint32_t adc_bits, double FS, double noise_range);
	void noise(double* arr, uint32_t len, double range);

#ifdef __cplusplus
}
#endif


#endif