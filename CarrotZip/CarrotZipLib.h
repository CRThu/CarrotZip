#ifndef __CARROT_ZIP_LIB_H__
#define __CARROT_ZIP_LIB_H__

#include <stdint.h>

#define PI 3.14159265

#ifdef __cplusplus
extern "C" {
#endif

	// Your code goes here

	typedef struct carrot_zip_t carrot_zip_t;
	/// <summary>
	/// ѹ�������ṹ��
	/// </summary>
	struct carrot_zip_t
	{
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
	};

	void carrot_zip_init(carrot_zip_t* zip, uint32_t ds);
	void carrot_zip_stream_compression(carrot_zip_t* zip, uint32_t* data, uint8_t* buf, uint32_t offset, uint32_t* len);
	void carrot_zip_stream_decompression(carrot_zip_t* zip, uint32_t* data, uint8_t* buf, uint32_t offset, uint32_t* len);

#ifdef __cplusplus
}
#endif


#endif