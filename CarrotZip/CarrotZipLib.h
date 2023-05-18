#ifndef __CARROT_ZIP_LIB_H__
#define __CARROT_ZIP_LIB_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DEBUG
#include <stdarg.h>
#include <stdio.h>
#endif // _DEBUG


    // Your code goes here  
#define PI 3.14159265


#define BYTE_MIN(byte1,byte2)	((byte1) < (byte2) ? (byte1) : (byte2))
#define BYTE_MAX(byte1,byte2)	((byte1) > (byte2) ? (byte1) : (byte2))
#define BYTE_ABS(byte)			((byte) > 0 ? (byte) : - (byte))
#define BYTE_MASK(a, b)			(((1 << (BYTE_ABS(a - b) + 1)) - 1) << (BYTE_MIN(a, b)))
#define BITS_GET(byte, a, b)	(((byte) >> (BYTE_MIN(a, b))) & ((1 << (BYTE_ABS(a - b) + 1)) - 1))
#define BITS_SET(byte, a, b, value) ((byte) & ~(BYTE_MASK(a, b)) | ((value & ((1 << (BYTE_ABS(a - b) + 1)) - 1)) << (BYTE_MIN(a, b))))

    typedef struct carrot_zip_t carrot_zip_t;
    /// <summary>
    /// ѹ�������ṹ��
    /// </summary>
    struct carrot_zip_t
    {
        /// <summary>
        /// ״̬��
        /// </summary>
        uint8_t state;

        /// <summary>
        /// ������б��ͬ��ֵ
        /// </summary>
        uint32_t ds;

        /// <summary>
        /// z_nΪǰn��ʱ�̵�����
        /// </summary>
        uint32_t z_1;

        /// <summary>
        /// k_nΪǰnʱ�̵�б��
        /// </summary>
        uint32_t k_1;

        /// <summary>
        /// dk_nΪǰnʱ�̵�б�ʱ仯��
        /// </summary>
        uint32_t dk_1;

        /// <summary>
        /// ������б��ͬ��������
        /// </summary>
        uint32_t ds_cnt;

        /// <summary>
        /// ��ʱѹ��λ�洢
        /// </summary>
        uint8_t store_bits;

        /// <summary>
        /// ��ʱѹ��λ���س��ȴ洢
        /// </summary>
        uint8_t store_bits_len;
    };

#ifdef _DEBUG
    void carrot_zip_info(const char* format, ...);
#endif // _DEBUG


    void carrot_zip_init(carrot_zip_t* zip, uint32_t ds);
    void carrot_zip_start(carrot_zip_t* zip, uint8_t* buf, uint32_t offset, uint32_t* len);
    void carrot_zip_end(carrot_zip_t* zip, uint8_t* buf, uint32_t offset, uint32_t* len);
    void carrot_zip_stream_compression(carrot_zip_t* zip, uint32_t* data, uint8_t* buf, uint32_t offset, uint32_t* len);
    void carrot_zip_stream_decompression(carrot_zip_t* zip, uint32_t* data, uint8_t* buf, uint32_t offset, uint32_t* len);

#ifdef __cplusplus
}
#endif


#endif