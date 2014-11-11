/**
 * @file dxnvt.h 大讯公司网络视频终端PC SDK接口  
 *
 * Copyright (C) 2009 北京大讯科技有限公司 
 * 
 * @version 0.1
 * @date 2009.12.10
 *
 * @version 0.2
 * @date 2010.01.04 
 * -1. 增加了指定本地IP功能
 *
 * @version 1.0
 * @date 2011.01.05 
 * -1. 增加了消息上下文
 * -2. 内置Keep Alive功能
 * -3. 修改了设备删除机制和删除函数接口
 *
 * @version 1.1
 * @date 2011.02.08 
 * -1. 修改了DX_NVT_init_media_param接口参数
 * -2. 增加DX_NVT_get_device_base_info接口
 * -3. 增加DX_NVT_get_tcp_local_addr接口
 */
#ifndef __DX_NVT_H__
#define __DX_NVT_H__


#ifdef _WIN32
#define  DXSDKAPI  __stdcall
#else
#define  DXSDKAPI  
#endif 


#ifdef __cplusplus
extern "C"
{
#endif


#include "portable.h"
#include "return.h"

/** 
 * @name 媒体类型
 * @{
 */
typedef enum __STREAM_MEDIA_TYPE
{
	STREAM_MEDIA_INVALID = -1,
	/** 复合媒体数据 */
	STREAM_MEDIA_COMPLEX = 0,
	STREAM_MEDIA_VIDEO = 1,
	STREAM_MEDIA_AUDIO,
	STREAM_MEDIA_UART,

	/** 自定义数据 */
	STREAM_CUSTOM_DATA,

	MEDIA_TYPE_PICTURE_BMP = 9,
	MEDIA_TYPE_PICTURE_JPG = 10
}STREAM_MEDIA_TYPE;
/** 
 * @}
 */

/** 
 * @name 图像抓拍格式
 * @{
 */
typedef enum __SNAP_FORMAT
{
	SNAP_FORMAT_BMP  = 1,
	SNAP_FORMAT_JPG = 2
}SNAP_FORMAT;
/*  
 * @}
 */ 

/** 
 * @name 流传输协议 
 * @{
 */
typedef enum __STREAM_TRANS_PROTOCOL
{
	/** UDP 单播 原始流 */
	UDP_UNICAST_RAW = 1,
	/** UDP 单播 RTP 封装 */ 
	UDP_UNICAST_RTP,

	/** UDP 组播 原始流 */
	UDP_MULTICAST_RAW,
	/** UDP 组播 RTP 封装 */ 
	UDP_MULTICAST_RTP,

	/** UDP 单播 FEC 流 */
	UDP_UNICAST_FEC = 10,
	
	/** TCP 原始流 */
	TCP_RAW = 20,
	/** TCP RTP 封装 */
	TCP_RTP,
	/** TCP 自定义格式 */ 
	TCP_CUSTOM

}STREAM_TRANS_PROTOCOL;
/** 
 * @}
 */

/** 
 * @name 流传输方向
 * @{
 */
typedef enum __STREAM_TRASN_DIR
{
	/** 接收, 对应设备编码通道 */
	STREAM_TRASN_POSITIVE = 0,
	/** 发送，对应设备解码通道 */
	STREAM_TRASN_NEGATIVE 
}STREAM_TRASN_DIR;
/** 
 * @}
 */

/** 
 * @name 设备上线方式
 * @{
 */
typedef enum __DEVICE_CREATE_MODE
{
	/** 主动连接创建 */
	DEVICE_CREATE_ACTIVE = 0,
	/** 被动连接创建 */
	DEVICE_CREATE_PASSIVE
		
}DEVICE_CREATE_MODE;
/** 
 * @}
 */

/** 
 * @name 设备断开原因
 * @{
 */
typedef enum __DEVICE_DELETE_REASON
{
	/** 主动断开, 由上层原因断开 */ 
	DEVICE_DELETE_ACTIVE = 1,
	/** 被动断开，由设备等原因断开 */ 
	DEVICE_DELETE_PASSIVE = 2,
	/** 超时断开 */ 
	DEVICE_DELETE_TIMEOUT = 3,
	/** 遇到错误(包括发送/接收消息数据错误，网络异常等) */
	DEVICE_DELETE_ERROR = 4
}DEVICE_DELETE_REASON;
/** 
 * @}
 */

/** 
 * @name 返回代码
 * @{
 */
typedef enum __RESULT_CODE
{
	/** 成功 */
	RESULT_CODE_OK  = 0,
	/** 出现错误 */
	RESULT_CODE_ERR = -1,
	/** 操作失败 */
	RESULT_CODE_FAIL = -2, 
	/** 会话过期 */
	RESULT_CODE_PAST = -301,
	/** 没有权限 */
	RESULT_CODE_REFUSE = -302,
	/** 超时 */
	RESULT_CODE_TIMEOUT	= -401

}RESULT_CODE;
/** 
 * @}
 */

/** 
 * @name 设备参数
 * @{
 */
typedef struct __DEVICE_PARAM
{
	struct __DEVICE_PARAM *next;

	int8 path[64];
	int8 *pvalue;
}DEVICE_PARAM;
/** 
 * @}
 */

/** 
 * @name 设备基本信息
 * @{
 */
typedef struct __DEVICE_BASE_INFO
{
	/** 设备ID */
	int8 id[128];
	/** 设备名称 */
	int8 name[128];
	/** 版本列表 */
	int8 ver_list[128];

	/** 视频输入通道数 */
	int32 vin_num;
	/** 视频编码通道数，为vin_num的倍数 */
	int32 venc_num;

	/** 视频解码通道数 */
	int32 vdec_num;

	/** 音频输入通道数 */
	int32 ain_num;
	/** 音频编码通道数，为ain_num的倍数 */
	int32 aenc_num;

	/** 串口通道数 */ 
	int32 uart_num;
}DEVICE_BASE_INFO;
/** 
 * @}
 */

typedef void * DEVICE_Handle;

typedef void ( DXSDKAPI *DX_NVT_CB_device_create)(DEVICE_Handle handle, void *context, int8 *ip, int32 port, DEVICE_CREATE_MODE mode);

typedef void (DXSDKAPI * DX_NVT_CB_set_response)(DEVICE_Handle handle, int32 result_code, void *msg_context);
typedef void (DXSDKAPI * DX_NVT_CB_get_response)(DEVICE_Handle handle, int32 result_code, DEVICE_PARAM *param_list, void *msg_context);
typedef void (DXSDKAPI * DX_NVT_CB_exe_response)(DEVICE_Handle handle, int32 result_code, void *msg_context);
typedef void (DXSDKAPI * DX_NVT_CB_snap_response)(DEVICE_Handle handle, int32 result_code, SNAP_FORMAT format, int32 chn, int8 *pdata, int32 len);
typedef void (DXSDKAPI * DX_NVT_CB_open_media_response)(DEVICE_Handle handle, int32 result_code, void *msg_context);
typedef void (DXSDKAPI * DX_NVT_CB_close_media_response)(DEVICE_Handle handle, int32 result_code, void *msg_context);
/** 
 *  param 为媒体流类型参数
 *  param为一个32bit复合数，共5个域，表示5个类型，定义如下：
 *  Bit31~Bit28	Bit27~Bit24	Bit23~Bit16	Bit15~Bit8	Bit7~Bit0
 *     类型5	   类型4	   类型3	   类型2	  类型1
 *  类型1定义：1-视频，2-音频，3-串口，9-BMP图片，10-JPG图片；
 *  类型2仅对音视频有意义，表示编码算法，具体定义见相关文档；
 *  类型3定义：视频时表示画面大小，音频时表示位宽；
 *  类型4定义：视频表示编码帧类型，音频表示采样率；
 *  类型5为数据片类型，具体定义见相关文档
 */

typedef void (DXSDKAPI *  DX_NVT_CB_media_arrive)(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int8 *buf, int32 len, uint32 param);

/** 
 * @n 设备断开三种情况：
 * @n 1、上层调用DX_NVT_delete_device主动去断开设备
 * @n 2、设备主动断开
 * @n 3、遇到异常情况或者超时SDK主动断开
 * @n 以上三种情况都会回调并且仅回调一次DX_NVT_CB_delete函数表示设备已经断开
 * @n DX_NVT_CB_delete回调执行完后，设备变为句柄无效
 * @n
 */
typedef void (DXSDKAPI * DX_NVT_CB_delete)(DEVICE_Handle handle, DEVICE_DELETE_REASON reason);


void DXSDKAPI DX_NVT_set_callback(
						 DX_NVT_CB_device_create cb_create, 
						 DX_NVT_CB_set_response cb_set,  DX_NVT_CB_get_response cb_get, 
						 DX_NVT_CB_exe_response cb_exe,  DX_NVT_CB_snap_response cb_snap,
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
 * @brief 主动断开设备步骤
 * @n 1. 调用DX_NVT_request_delete_device通知sdk需要断开设备
 * @n 2. 回调DX_NVT_CB_delete执行，表示设备已经断开
 * @n 3. 调用DX_NVT_release_device释放设备句柄资源
 * @n 如果被动断开，则为步骤2, 3
 */ 
void DXSDKAPI DX_NVT_request_delete_device(DEVICE_Handle handle);
void DXSDKAPI DX_NVT_release_device_handle(DEVICE_Handle handle);


int32 DXSDKAPI DX_NVT_set_device_context(DEVICE_Handle handle, void *device_context);
void * DXSDKAPI DX_NVT_get_device_context(DEVICE_Handle handle);

DEVICE_BASE_INFO * DXSDKAPI DX_NVT_get_device_base_info(DEVICE_Handle handle);
int32 DXSDKAPI DX_NVT_get_tcp_local_addr(DEVICE_Handle handle, int8 *local_ip, int32 *local_port);

int32 DXSDKAPI DX_NVT_set(DEVICE_Handle handle, int8 *path, int8 *pvalue, void *msg_context);
int32 DXSDKAPI DX_NVT_get(DEVICE_Handle handle, int8 *path, void *msg_context);
int32 DXSDKAPI DX_NVT_exe(DEVICE_Handle handle, int8 *path[], int8 *pvalue[], int32 count, void *msg_context);
int32 DXSDKAPI DX_NVT_snap(DEVICE_Handle handle, int32 chn, SNAP_FORMAT format, int32 quality); /** SDK 当前版本 DX_NVT_snap 不支持消息上下文 */ 

int32 DXSDKAPI DX_NVT_init_media_param(DEVICE_Handle handle, int8 *local_ip);

int32 DXSDKAPI DX_NVT_open_media(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int32 dir, int32 protocol, void *msg_context);
int32 DXSDKAPI  DX_NVT_close_media(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int32 dir, int32 protocol);

int32 DXSDKAPI DX_NVT_open_media2(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int32 dir, int32 protocol, int8 *ip, int32 port, void *msg_context);
int32 DXSDKAPI DX_NVT_close_media2(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int32 dir, int32 protocol, int8 *ip, int32 port);

int32 DXSDKAPI DX_NVT_set_media_receive_buf(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int8 *buf, int32 buf_len);
int32 DXSDKAPI DX_NVT_send_media(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int8 *buf, int32 len);
 
const char* DXSDKAPI DX_NVT_getParamPath( DEVICE_PARAM * param );

#ifdef __cplusplus
}
#endif

#endif // __DX_NVT_H__
