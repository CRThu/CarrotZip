#include "CarrotZipLib.h"

#ifdef __cplusplus
extern "C" {
#endif

	void carrot_zip_init(carrot_zip_t* zip)
	{
		zip->z_1 = 0;
		zip->k_1 = 0;
		zip->dk_1 = 0;
		zip->ds_cnt = 0;
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
