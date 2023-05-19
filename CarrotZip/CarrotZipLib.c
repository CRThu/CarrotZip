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

    void carrot_zip_buf_info(uint8_t* buf, uint32_t offset, uint32_t* len)
    {
        for (uint32_t i = 0; i < *len; i++)
        {
            carrot_zip_info("%02X ", buf[offset + i]);
        }
        carrot_zip_info("\n");
    }
#endif // _DEBUG


    /// <summary>
    /// 压缩参数结构体初始化
    /// </summary>
    /// <param name="zip"></param>
    /// <param name="ds"></param>
    void carrot_zip_init(carrot_zip_t* zip, uint16_t ds)
    {
        zip->state = 0;
        zip->z_1 = 0;
        zip->k_1 = 0;
        zip->dk_1 = 0;
        zip->ds_cnt = ds;
        zip->ds = ds;
        zip->err_width = 8;
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
        buf[offset + 0] = CARROT_ZIP_CTL_START;
        buf[offset + 1] = 0x00;
        buf[offset + 2] = 0x00;
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
            buf[offset + 1] = CARROT_ZIP_CTL_START;
            buf[offset + 2] = CARROT_ZIP_CTL_TAIL;
            buf[offset + 3] = CARROT_ZIP_END_BYTE;
            *len = 4;
        }
        else
        {
            buf[offset + 0] = CARROT_ZIP_CTL_START;
            buf[offset + 1] = CARROT_ZIP_CTL_TAIL;
            buf[offset + 2] = CARROT_ZIP_END_BYTE;
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

            uint32_t err_word = err + (1 << (zip->err_width - 1));
            if (err_word >= ((1 << zip->err_width) - 1))
            {
                zip->err_width += 8;
                buf[offset + 0] = CARROT_ZIP_CTL_START;
                buf[offset + 1] = CARROT_ZIP_CTL_CHANGE_Z_WIDTH(zip->err_width);
                *len = 2;
                for (int i = (zip->err_width >> 3) - 1; i >= 0; i--)
                {
                    buf[offset + *len] = err >> (i << 3);
                    *len = *len + 1;
                }
                carrot_zip_buf_info(buf, offset, len);
            }

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

                buf[offset + 0] = CARROT_ZIP_CTL_START;
                buf[offset + 1] = CARROT_ZIP_CTL_CHANGE_Z_WIDTH(32);
                buf[offset + 2] = zip->z_1 >> 24;
                buf[offset + 3] = zip->z_1 >> 16;
                buf[offset + 4] = zip->z_1 >> 8;
                buf[offset + 5] = zip->z_1 >> 0;
                *len = 6;
                carrot_zip_buf_info(buf, offset, len);
            }
            else if (zip->state == 2)
            {
                zip->state = 3;
                zip->k_1 = *data - zip->z_1;
                zip->z_1 = *data;
                carrot_zip_info("ds_cnt = %8u, 预测帧, dat = %8u, k = %8d\n", zip->ds_cnt, *data, zip->k_1);

                buf[offset + 0] = CARROT_ZIP_CTL_START;
                buf[offset + 1] = CARROT_ZIP_CTL_CHANGE_K_WIDTH(32);
                buf[offset + 2] = zip->k_1 >> 24;
                buf[offset + 3] = zip->k_1 >> 16;
                buf[offset + 4] = zip->k_1 >> 8;
                buf[offset + 5] = zip->k_1 >> 0;
                *len = 6;
                carrot_zip_buf_info(buf, offset, len);
            }
            else if (zip->state == 3)
            {
                zip->state = 4;
                zip->dk_1 = *data - zip->z_1 - zip->k_1;
                zip->k_1 = *data - zip->z_1;
                zip->z_1 = *data;
                carrot_zip_info("ds_cnt = %8u, 预测帧, dat = %8u, k = %8d, dk = %8d\n", zip->ds_cnt, *data, zip->k_1, zip->dk_1);
                zip->ds_cnt = 0;

                buf[offset + 0] = CARROT_ZIP_CTL_START;
                buf[offset + 1] = CARROT_ZIP_CTL_CHANGE_DK_WIDTH(32);
                buf[offset + 2] = zip->dk_1 >> 24;
                buf[offset + 3] = zip->dk_1 >> 16;
                buf[offset + 4] = zip->dk_1 >> 8;
                buf[offset + 5] = zip->dk_1 >> 0;
                *len = 6;
                carrot_zip_buf_info(buf, offset, len);
            }
            else if (zip->state == 4)
            {
                zip->dk_1 = *data - zip->z_1 - zip->k_1;
                zip->k_1 = *data - zip->z_1;
                zip->z_1 = *data;
                carrot_zip_info("ds_cnt = %8u, 预测帧, dat = %8u, k = %8d, dk = %8d\n", zip->ds_cnt, *data, zip->k_1, zip->dk_1);
                zip->ds_cnt = 0;

                buf[offset + 0] = CARROT_ZIP_CTL_START;
                buf[offset + 1] = CARROT_ZIP_CTL_CHANGE_DK_WIDTH(32);
                buf[offset + 2] = zip->dk_1 >> 24;
                buf[offset + 3] = zip->dk_1 >> 16;
                buf[offset + 4] = zip->dk_1 >> 8;
                buf[offset + 5] = zip->dk_1 >> 0;
                *len = 6;
                carrot_zip_buf_info(buf, offset, len);
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
