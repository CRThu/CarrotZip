#include "data_gen.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// 理想正弦波生成
	/// </summary>
	void gen_sine(double* arr, uint32_t len, double fin, double fs, double vpp)
	{
		srand(time(NULL));
		double phase = (double)rand() / RAND_MAX * 2 * PI;

		int random_num = 0;
		for (uint32_t i = 0; i < len; i++)
		{
			arr[i] = vpp / 2.0 * sin(2.0 * PI * fin * ((double)i / fs + phase));
		}
	}

	/// <summary>
	/// 理想adc转换
	/// </summary>
	void adc(uint32_t* arr_out, double* arr_in, uint32_t len, uint32_t adc_bits, double FS, double noise_range)
	{
		uint32_t adc_codes = 1 << adc_bits;
		for (uint32_t i = 0; i < len; i++)
		{
			arr_in[i] = arr_in[i] / FS * (double)adc_codes + (double)adc_codes / 2;
		}

		noise(arr_in, len, noise_range);

		for (uint32_t i = 0; i < len; i++)
		{
			int32_t code = round(arr_in[i]);
			if (code > (int32_t)adc_codes - 1)
				arr_out[i] = adc_codes - 1;
			else if (code < 0)
				arr_out[i] = 0;
			else
				arr_out[i] = code;
		}
	}

	/// <summary>
	/// 生成噪声（此处为随机数，非高斯白噪声）
	/// </summary>
	void noise(double* arr, uint32_t len, double range)
	{
		srand(time(NULL));

		for (uint32_t i = 0; i < len; i++)
		{
			double random_num = (double)rand() / RAND_MAX * range - range / 2.0;
			arr[i] += random_num;
		}
	}

#ifdef __cplusplus
}
#endif
