#include "CarrotZipLib.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DEBUG
    void carrot_zip_info(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
#endif // _DEBUG


    /// <summary>
    /// 压缩参数结构体初始化
    /// </summary>
    /// <param name="zip"></param>
    /// <param name="ds"></param>
    void carrot_zip_init(carrot_zip_t* zip, uint32_t ds)
    {
        zip->state = 0;
        zip->z_1 = 0;
        zip->k_1 = 0;
        zip->dk_1 = 0;
        zip->ds_cnt = ds;
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
        zip->state = 1;
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
        if (zip->ds_cnt < zip->ds)
        {
            // 误差帧
            int32_t k = *data - zip->z_1;
            int32_t dk = k - zip->k_1;
            int32_t err = dk - zip->dk_1;
            carrot_zip_info("ds_cnt = %8u, 误差帧, dat = %8u, k = %8d, dk = %8d, err = %8d\n", zip->ds_cnt, *data, k, dk, err);

            // 压缩信息更新
            zip->z_1 = *data;
            zip->k_1 = k;
            zip->dk_1 = dk;
            zip->ds_cnt++;
        }
        else
        {
            // 预测帧

            if (zip->state == 1)
            {
                zip->state = 2;
                zip->z_1 = *data;
                carrot_zip_info("ds_cnt = %8u, 预测帧, dat = %8u\n", zip->ds_cnt, *data);
            }
            else if (zip->state == 2)
            {
                zip->state = 3;
                zip->k_1 = *data - zip->z_1;
                zip->z_1 = *data;
                carrot_zip_info("ds_cnt = %8u, 预测帧, dat = %8u, k = %8d\n", zip->ds_cnt, *data, zip->k_1);
            }
            else if (zip->state == 3)
            {
                zip->state = 4;
                zip->dk_1 = *data - zip->z_1 - zip->k_1;
                zip->k_1 = *data - zip->z_1;
                zip->z_1 = *data;
                carrot_zip_info("ds_cnt = %8u, 预测帧, dat = %8u, k = %8d, dk = %8d\n", zip->ds_cnt, *data, zip->k_1, zip->dk_1);
                zip->ds_cnt = 0;
            }
            else if (zip->state == 4)
            {
                zip->dk_1 = *data - zip->z_1 - zip->k_1;
                zip->k_1 = *data - zip->z_1;
                zip->z_1 = *data;
                carrot_zip_info("ds_cnt = %8u, 预测帧, dat = %8u, k = %8d, dk = %8d\n", zip->ds_cnt, *data, zip->k_1, zip->dk_1);
                zip->ds_cnt = 0;
            }

            /*  for (uint8_t i = 0; i < 4; i++)
              {
                  buf[offset + i] = (*data >> (i * 8)) & 0xFF;
              }
              *len = 4;*/
        }
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
