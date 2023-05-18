#include "CarrotZipLib.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// <summary>
	/// 压缩参数结构体初始化
	/// </summary>
	/// <param name="zip"></param>
	/// <param name="ds"></param>
	void carrot_zip_init(carrot_zip_t* zip, uint32_t ds)
	{

		zip->z_1 = 0;
		zip->k_1 = 0;
		zip->dk_1 = 0;
		zip->ds_cnt = 0;
		zip->ds = ds;
		zip->store_bits = 0;
		zip->store_bits_len = 0;
	}

	/// <summary>
	/// 压缩头生成
	/// </summary>
	/// <param name="zip"></param>
	/// <param name="buf"></param>
	/// <param name="offset"></param>
	/// <param name="len"></param>
	void carrot_zip_start(carrot_zip_t* zip, uint8_t* buf, uint32_t offset, uint32_t* len)
	{
		buf[offset + 0] = 0xA5;
		buf[offset + 1] = zip->ds_cnt >> 24;
		buf[offset + 2] = zip->ds_cnt >> 16;
		buf[offset + 3] = zip->ds_cnt >> 8;
		buf[offset + 4] = zip->ds_cnt >> 0;
		*len = 5;
	}

	/// <summary>
	/// 压缩尾生成
	/// </summary>
	/// <param name="zip"></param>
	/// <param name="buf"></param>
	/// <param name="offset"></param>
	/// <param name="len"></param>
	void carrot_zip_end(carrot_zip_t* zip, uint8_t* buf, uint32_t offset, uint32_t* len)
	{
		if (zip->store_bits_len != 0)
		{
			buf[offset + 0] = zip->store_bits;
			buf[offset + 1] = 0xFF;
			buf[offset + 2] = 0xFF;
			buf[offset + 3] = 0x5A;
			*len = 4;
		}
		else
		{
			buf[offset + 0] = 0xFF;
			buf[offset + 1] = 0xFF;
			buf[offset + 2] = 0x5A;
			*len = 3;
		}
	}

	/// <summary>
	/// 流式压缩
	/// </summary>
	/// <param name="zip"></param>
	/// <param name="data"></param>
	/// <param name="buf"></param>
	/// <param name="offset"></param>
	/// <param name="len"></param>
	void carrot_zip_stream_compression(carrot_zip_t* zip, uint32_t* data, uint8_t* buf, uint32_t offset, uint32_t* len)
	{
		if (zip->ds < zip->ds_cnt)
		{
			// 误差帧


			zip->ds++;
		}
		else
		{
			// 预测帧
			zip->ds = 0;
		}
		for (uint8_t i = 0; i < 4; i++)
		{
			buf[offset + i] = (*data >> (i * 8)) & 0xFF;
		}
		*len = 4;
	}

	/// <summary>
	/// 流式解压
	/// </summary>
	/// <param name="zip"></param>
	/// <param name="data"></param>
	/// <param name="buf"></param>
	/// <param name="offset"></param>
	/// <param name="len"></param>
	void carrot_zip_stream_decompression(carrot_zip_t* zip, uint32_t* data, uint8_t* buf, uint32_t offset, uint32_t* len)
	{

		for (uint8_t i = 0; i < 4; i++)
		{
			*data |= buf[offset + i] << (i * 8);
		}
		*len = 4;
	}

#ifdef __cplusplus
	}
#endif
