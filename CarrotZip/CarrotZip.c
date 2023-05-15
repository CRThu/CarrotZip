// CarrotZip.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include "data_gen.h"
#include "file_util.h"


#define BUF_LEN 65536
double sine_buf[BUF_LEN];
uint32_t sine_codes[BUF_LEN];


#define FIN 999.888
#define FSAMPLE 233345.666
#define FULLSCALE 10
#define VIN 0.95 * FULLSCALE
#define ADC_BITS 16
#define NOISE_RANGE 10

int main()
{
	gen_sine(sine_buf, BUF_LEN, FIN, FSAMPLE, VIN);
	adc(sine_codes, sine_buf, BUF_LEN, ADC_BITS, FULLSCALE, NOISE_RANGE);

	//save_double_arr("arr.txt", sine_buf, BUF_LEN);
	save_uint32_arr("codes.txt", sine_codes, BUF_LEN);

}
