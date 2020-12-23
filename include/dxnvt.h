
#ifndef __DX_NVT_H__
#define __DX_NVT_H__

#ifdef _WIN32
#define DXSDKAPI __stdcall
#else
#define DXSDKAPI
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "portable.h"
#include "return.h"

	/** 
 * @name ý������
 * @{
 */
	typedef enum __STREAM_MEDIA_TYPE
	{
		STREAM_MEDIA_INVALID = -1,
		/** ����ý������ */
		STREAM_MEDIA_COMPLEX = 0,
		STREAM_MEDIA_VIDEO = 1,
		STREAM_MEDIA_AUDIO,
		STREAM_MEDIA_UART,

		/** �Զ������� */
		STREAM_CUSTOM_DATA,

		MEDIA_TYPE_PICTURE_BMP = 9,
		MEDIA_TYPE_PICTURE_JPG = 10
	} STREAM_MEDIA_TYPE;
	/** 
 * @}
 */

	/** 
 * @name ͼ��ץ�ĸ�ʽ
 * @{
 */
	typedef enum __SNAP_FORMAT
	{
		SNAP_FORMAT_BMP = 1,
		SNAP_FORMAT_JPG = 2
	} SNAP_FORMAT;
	/*  
 * @}
 */

	/** 
 * @name ������Э�� 
 * @{
 */
	typedef enum __STREAM_TRANS_PROTOCOL
	{
		/** UDP ���� ԭʼ�� */
		UDP_UNICAST_RAW = 1,
		/** UDP ���� RTP ��װ */
		UDP_UNICAST_RTP,

		/** UDP �鲥 ԭʼ�� */
		UDP_MULTICAST_RAW,
		/** UDP �鲥 RTP ��װ */
		UDP_MULTICAST_RTP,

		/** UDP ���� FEC �� */
		UDP_UNICAST_FEC = 10,

		/** TCP ԭʼ�� */
		TCP_RAW = 20,
		/** TCP RTP ��װ */
		TCP_RTP,
		/** TCP �Զ����ʽ */
		TCP_CUSTOM

	} STREAM_TRANS_PROTOCOL;
	/** 
 * @}
 */

	/** 
 * @name �����䷽��
 * @{
 */
	typedef enum __STREAM_TRASN_DIR
	{
		/** ����, ��Ӧ�豸����ͨ�� */
		STREAM_TRASN_POSITIVE = 0,
		/** ���ͣ���Ӧ�豸����ͨ�� */
		STREAM_TRASN_NEGATIVE
	} STREAM_TRASN_DIR;
	/** 
 * @}
 */

	/** 
 * @name �豸���߷�ʽ
 * @{
 */
	typedef enum __DEVICE_CREATE_MODE
	{
		/** �������Ӵ��� */
		DEVICE_CREATE_ACTIVE = 0,
		/** �������Ӵ��� */
		DEVICE_CREATE_PASSIVE

	} DEVICE_CREATE_MODE;
	/** 
 * @}
 */

	/** 
 * @name �豸�Ͽ�ԭ��
 * @{
 */
	typedef enum __DEVICE_DELETE_REASON
	{
		/** �����Ͽ�, ���ϲ�ԭ��Ͽ� */
		DEVICE_DELETE_ACTIVE = 1,
		/** �����Ͽ������豸��ԭ��Ͽ� */
		DEVICE_DELETE_PASSIVE = 2,
		/** ��ʱ�Ͽ� */
		DEVICE_DELETE_TIMEOUT = 3,
		/** ��������(��������/������Ϣ���ݴ��������쳣��) */
		DEVICE_DELETE_ERROR = 4
	} DEVICE_DELETE_REASON;
	/** 
 * @}
 */

	/** 
 * @name ���ش���
 * @{
 */
	typedef enum __RESULT_CODE
	{
		/** �ɹ� */
		RESULT_CODE_OK = 0,
		/** ���ִ��� */
		RESULT_CODE_ERR = -1,
		/** ����ʧ�� */
		RESULT_CODE_FAIL = -2,
		/** �Ự���� */
		RESULT_CODE_PAST = -301,
		/** û��Ȩ�� */
		RESULT_CODE_REFUSE = -302,
		/** ��ʱ */
		RESULT_CODE_TIMEOUT = -401

	} RESULT_CODE;
	/** 
 * @}
 */

	/** 
 * @name �豸����
 * @{
 */
	typedef struct __DEVICE_PARAM
	{
		struct __DEVICE_PARAM *next;

		int8 path[64];
		int8 *pvalue;
	} DEVICE_PARAM;
	/** 
 * @}
 */

	/** 
 * @name �豸������Ϣ
 * @{
 */
	typedef struct __DEVICE_BASE_INFO
	{
		/** �豸ID */
		int8 id[128];
		/** �豸���� */
		int8 name[128];
		/** �汾�б� */
		int8 ver_list[128];

		/** ��Ƶ����ͨ���� */
		int32 vin_num;
		/** ��Ƶ����ͨ������Ϊvin_num�ı��� */
		int32 venc_num;

		/** ��Ƶ����ͨ���� */
		int32 vdec_num;

		/** ��Ƶ����ͨ���� */
		int32 ain_num;
		/** ��Ƶ����ͨ������Ϊain_num�ı��� */
		int32 aenc_num;

		/** ����ͨ���� */
		int32 uart_num;
	} DEVICE_BASE_INFO;
	/** 
 * @}
 */

	typedef void *DEVICE_Handle;

	typedef void(DXSDKAPI *DX_NVT_CB_device_create)(DEVICE_Handle handle, void *context, int8 *ip, int32 port, DEVICE_CREATE_MODE mode);

	typedef void(DXSDKAPI *DX_NVT_CB_set_response)(DEVICE_Handle handle, int32 result_code, void *msg_context);
	typedef void(DXSDKAPI *DX_NVT_CB_get_response)(DEVICE_Handle handle, int32 result_code, DEVICE_PARAM *param_list, void *msg_context);
	typedef void(DXSDKAPI *DX_NVT_CB_exe_response)(DEVICE_Handle handle, int32 result_code, void *msg_context);
	typedef void(DXSDKAPI *DX_NVT_CB_snap_response)(DEVICE_Handle handle, int32 result_code, SNAP_FORMAT format, int32 chn, int8 *pdata, int32 len);
	typedef void(DXSDKAPI *DX_NVT_CB_open_media_response)(DEVICE_Handle handle, int32 result_code, void *msg_context);
	typedef void(DXSDKAPI *DX_NVT_CB_close_media_response)(DEVICE_Handle handle, int32 result_code, void *msg_context);
	/** 
 *  param Ϊý�������Ͳ���
 *  paramΪһ��32bit����������5���򣬱�ʾ5�����ͣ��������£�
 *  Bit31~Bit28	Bit27~Bit24	Bit23~Bit16	Bit15~Bit8	Bit7~Bit0
 *     ����5	   ����4	   ����3	   ����2	  ����1
 *  ����1���壺1-��Ƶ��2-��Ƶ��3-���ڣ�9-BMPͼƬ��10-JPGͼƬ��
 *  ����2��������Ƶ�����壬��ʾ�����㷨�����嶨�������ĵ���
 *  ����3���壺��Ƶʱ��ʾ�����С����Ƶʱ��ʾλ����
 *  ����4���壺��Ƶ��ʾ����֡���ͣ���Ƶ��ʾ�����ʣ�
 *  ����5Ϊ����Ƭ���ͣ����嶨�������ĵ�
 */

	typedef void(DXSDKAPI *DX_NVT_CB_media_arrive)(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int8 *buf, int32 len, uint32 param);

	/** 
 * @n �豸�Ͽ����������
 * @n 1���ϲ����DX_NVT_delete_device����ȥ�Ͽ��豸
 * @n 2���豸�����Ͽ�
 * @n 3�������쳣������߳�ʱSDK�����Ͽ�
 * @n ���������������ص����ҽ��ص�һ��DX_NVT_CB_delete������ʾ�豸�Ѿ��Ͽ�
 * @n DX_NVT_CB_delete�ص�ִ������豸��Ϊ�����Ч
 * @n
 */
	typedef void(DXSDKAPI *DX_NVT_CB_delete)(DEVICE_Handle handle, DEVICE_DELETE_REASON reason);

	void DXSDKAPI DX_NVT_set_callback(
		DX_NVT_CB_device_create cb_create,
		DX_NVT_CB_set_response cb_set, DX_NVT_CB_get_response cb_get,
		DX_NVT_CB_exe_response cb_exe, DX_NVT_CB_snap_response cb_snap,
		DX_NVT_CB_open_media_response cb_open_media,
		DX_NVT_CB_close_media_response cb_close_media,
		DX_NVT_CB_media_arrive cb_meida_arrive,
		DX_NVT_CB_delete cb_delete);

	int32 DXSDKAPI DX_NVT_open(int32 udp_min_port, int32 udp_max_port);
	void DXSDKAPI DX_NVT_close(void);

	int32 DXSDKAPI DX_NVT_start_listen(int32 port, int8 *ip);
	void DXSDKAPI DX_NVT_stop_listen(int32 port);
	void DXSDKAPI DX_NVT_stop_all_listen(void);

	int32 DXSDKAPI DX_NVT_add_device(int8 *ip, int32 port, int8 *local_ip, void *context);

	/** 
 * @brief �����Ͽ��豸����
 * @n 1. ����DX_NVT_request_delete_device֪ͨsdk��Ҫ�Ͽ��豸
 * @n 2. �ص�DX_NVT_CB_deleteִ�У���ʾ�豸�Ѿ��Ͽ�
 * @n 3. ����DX_NVT_release_device�ͷ��豸�����Դ
 * @n ��������Ͽ�����Ϊ����2, 3
 */
	void DXSDKAPI DX_NVT_request_delete_device(DEVICE_Handle handle);
	void DXSDKAPI DX_NVT_release_device_handle(DEVICE_Handle handle);

	int32 DXSDKAPI DX_NVT_set_device_context(DEVICE_Handle handle, void *device_context);
	void *DXSDKAPI DX_NVT_get_device_context(DEVICE_Handle handle);

	DEVICE_BASE_INFO *DXSDKAPI DX_NVT_get_device_base_info(DEVICE_Handle handle);
	int32 DXSDKAPI DX_NVT_get_tcp_local_addr(DEVICE_Handle handle, int8 *local_ip, int32 *local_port);

	int32 DXSDKAPI DX_NVT_set(DEVICE_Handle handle, int8 *path, int8 *pvalue, void *msg_context);
	int32 DXSDKAPI DX_NVT_get(DEVICE_Handle handle, int8 *path, void *msg_context);
	int32 DXSDKAPI DX_NVT_exe(DEVICE_Handle handle, int8 *path[], int8 *pvalue[], int32 count, void *msg_context);
	int32 DXSDKAPI DX_NVT_snap(DEVICE_Handle handle, int32 chn, SNAP_FORMAT format, int32 quality); /** SDK ��ǰ�汾 DX_NVT_snap ��֧����Ϣ������ */

	int32 DXSDKAPI DX_NVT_init_media_param(DEVICE_Handle handle, int8 *local_ip);

	int32 DXSDKAPI DX_NVT_open_media(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int32 dir, int32 protocol, void *msg_context);
	int32 DXSDKAPI DX_NVT_close_media(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int32 dir, int32 protocol);

	int32 DXSDKAPI DX_NVT_open_media2(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int32 dir, int32 protocol, int8 *ip, int32 port, void *msg_context);
	int32 DXSDKAPI DX_NVT_close_media2(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int32 dir, int32 protocol, int8 *ip, int32 port);

	int32 DXSDKAPI DX_NVT_set_media_receive_buf(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int8 *buf, int32 buf_len);
	int32 DXSDKAPI DX_NVT_send_media(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int8 *buf, int32 len);

	const char *DXSDKAPI DX_NVT_getParamPath(DEVICE_PARAM *param);

#ifdef __cplusplus
}
#endif

#endif // __DX_NVT_H__
