// CarrotZip.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include "data_gen.h"
#include "file_util.h"
#include "array_util.h"
#include "CarrotZipLib.h"


#define BUF_LEN 2048
#define BUF_BYTES_LEN (BUF_LEN * sizeof(uint32_t))
double sine_buf[BUF_LEN];
uint32_t sine_codes[BUF_LEN];
int32_t codes_diff_1[BUF_LEN];
int32_t codes_diff_2[BUF_LEN];
int32_t codes_diff_3[BUF_LEN];
int32_t codes_diff_4[BUF_LEN];

carrot_zip_t zip;
uint8_t comp_byte_stream[BUF_BYTES_LEN];
uint32_t comp_byte_stream_cursor = 0;
uint32_t comp_byte_stream_len = 0;
uint32_t decomp_data[BUF_LEN];
uint32_t decomp_data_cursor = 0;
uint32_t decomp_byte_stream_len = 0;
uint32_t decomp_byte_stream_cursor = 0;


#define FIN 999.888
#define FSAMPLE 233345.666
#define FULLSCALE 10
#define VIN 0.95 * FULLSCALE
#define ADC_BITS 16
#define NOISE_RANGE 6

// 斜率降采样率需大于 每周期波形采样点数/4以下 实现最大压缩率
// SLOPE_DOWNSAMPLE 32 适用于 FIN < FSAMPLE / 128 采样频率压缩

#define SLOPE_DOWNSAMPLE 32

void sin_analysis()
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

void compress_test()
{
	carrot_zip_init(&zip, SLOPE_DOWNSAMPLE);

	gen_sine(sine_buf, BUF_LEN, FIN, FSAMPLE, VIN);
	adc(sine_codes, sine_buf, BUF_LEN, ADC_BITS, FULLSCALE, NOISE_RANGE);

	for (int i = 0; i < BUF_LEN; i++)
	{
		uint32_t sample_code = sine_codes[i];
		carrot_zip_stream_compression(&zip, &sample_code, comp_byte_stream, comp_byte_stream_cursor, &comp_byte_stream_len);
		comp_byte_stream_cursor += comp_byte_stream_len;
	}

	while (decomp_byte_stream_cursor < comp_byte_stream_cursor)
	{
		uint32_t* decomp_code = &decomp_data[decomp_data_cursor];
		carrot_zip_stream_decompression(&zip, decomp_code, comp_byte_stream, decomp_byte_stream_cursor, &decomp_byte_stream_len);

		decomp_byte_stream_cursor += decomp_byte_stream_len;
		decomp_data_cursor++;
	}

	printf("raw data len: %llu\n", BUF_BYTES_LEN);
	printf("compressed data len: %lu\n", comp_byte_stream_cursor);
	printf("decompressed data len: %llu\n", (decomp_data_cursor * sizeof(uint32_t)));
	printf("compress ratio: %.3f %%\n", (float)(decomp_data_cursor * sizeof(uint32_t) / (float)comp_byte_stream_cursor * 100));
	bool is_equal = (BUF_LEN == decomp_data_cursor) && array_equal_uint32(sine_codes, decomp_data, BUF_LEN);
	printf("compare raw data and decompressed data: %s\n", is_equal ? "true" : "false");
}

int main()
{
	// sin_analysis();
	compress_test();
}
