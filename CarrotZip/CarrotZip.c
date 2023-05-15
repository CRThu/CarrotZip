// CarrotZip.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include "data_gen.h"
#include "file_util.h"


#define BUF_LEN 2048
double sine_buf[BUF_LEN];
uint32_t sine_codes[BUF_LEN];
int32_t codes_diff_1[BUF_LEN];
int32_t codes_diff_2[BUF_LEN];
int32_t codes_diff_3[BUF_LEN];
int32_t codes_diff_4[BUF_LEN];


#define FIN 999.888
#define FSAMPLE 233345.666
#define FULLSCALE 10
#define VIN 0.95 * FULLSCALE
#define ADC_BITS 16
#define NOISE_RANGE 6

// 斜率降采样率需大于 每周期波形采样点数/4以下 实现最大压缩率
#define SLOPE_DOWNSAMPLE ((int)(((double)FSAMPLE)/((double)FIN)/4.0))

int main()
{
	gen_sine(sine_buf, BUF_LEN, FIN, FSAMPLE, VIN);
	adc(sine_codes, sine_buf, BUF_LEN, ADC_BITS, FULLSCALE, NOISE_RANGE);

	//save_double_arr("arr.txt", sine_buf, BUF_LEN);
	save_uint32_arr("codes.txt", sine_codes, BUF_LEN);

	// codes_diff_1 为2code之间差值
	// codes_diff_2 为2code之间斜率
	// codes_diff_3 为2code之间斜率的降采样 TODO 滤波
	// codes_diff_4 为2code之间斜率计算后误差
	for (uint32_t i = 0; i < BUF_LEN - 1; i++)
	{
		codes_diff_1[i] = (int32_t)sine_codes[i + 1] - (int32_t)sine_codes[i];
	}
	for (uint32_t i = 0; i < BUF_LEN - 1; i += SLOPE_DOWNSAMPLE)
	{
		for (int j = 0; j < SLOPE_DOWNSAMPLE; j++)
		{
			codes_diff_2[i + j] = codes_diff_1[i + j + 1] - codes_diff_1[i + j];
			codes_diff_3[i + j] = codes_diff_1[i + 1] - codes_diff_1[i];
			codes_diff_4[i + j] = codes_diff_3[i + j] - codes_diff_2[i + j];
		}
	}
	for (uint32_t i = 0; i < BUF_LEN - 1; i++)
	{
		printf("%10lu\t%10ld\t%10ld\t%10ld\t%10ld\n",
			sine_codes[i], codes_diff_1[i], codes_diff_2[i], codes_diff_3[i], codes_diff_4[i]);
	}
}
