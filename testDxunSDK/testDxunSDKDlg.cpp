
// testDxunSDKDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testDxunSDK.h"
#include "testDxunSDKDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CtestDxunSDKDlg::CtestDxunSDKDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestDxunSDKDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDxunSDKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestDxunSDKDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CtestDxunSDKDlg::OnBnClickedButton1)
END_MESSAGE_MAP()



BOOL CtestDxunSDKDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	return TRUE;  
}

#include <stdio.h> 
#include <string.h> 
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#define sleep(m) Sleep((m)*1000)
#define usleep(m) Sleep((m)/1000+1)
#endif

#include "dxnvt.h"


#pragma comment(lib, "dxnvtsdk.lib")
//#include "thread_mutex.h"

//#include "comm_macro.h"

#define MAX_DEVICE_NUM  1024



/** 
* �豸���ݽṹ�����������豸��Ϣ  
*/ 
typedef struct {
	int32 id;

	DEVICE_Handle handle;

	int8 *video_rcv_buf;
	int8 *audio_rcv_buf;
	int8 *uart_rcv_buf;
	int8 *data_rcv_buf;
	int32 rcv_buf_len;

	int8 *stream_snd_buf;
	int32 stream_snd_buf_len;

	int8 device_id[128];
	int8 device_ip[32];
	int32 device_port;

	DEVICE_CREATE_MODE mode;

	int32 free;

}DEVICE_OBJECT;

/** 
* Ϊ�˷��㣬Ԥ�ȶ���һ���̶������豸����
* ��ʵ��Ӧ���У����Զ�̬�����豸���ݻ����豸����Ȼ���ö��л����б�ȷ�ʽȥ���й���
*/ 
static DEVICE_OBJECT device_array[MAX_DEVICE_NUM];

/** 
* ����һ��ȫ�ֻ�����
* MUTEXHanle ��SDK ���а�����һ������ֲ���̻߳������������ز���������"thread_mutex.h"
* ���ϲ�Ӧ�ó����п���ֱ��ʹ��
*/ 
//static MUTEXHanle mutex;
static int mutex;

#define THREAD_MUTEX_LOCK(handle) { }
#define THREAD_MUTEX_UNLOCK(handle) { } 


/** 
* @brief ��ʼ���豸����
* 
* @return �ɹ�����GEN_OK
*/
static int32 example_init_device_object(void)
{
	int32 i;
	DEVICE_OBJECT *pdevice;

	for(i=0; i<MAX_DEVICE_NUM; i++){
		pdevice = &device_array[i];
		pdevice->id = i;
		pdevice->handle = NULL;

		/** 
		* ����ý�����ݽ��ջ���������������������
		* ÿ��ý���������ݵ�ÿ��ͨ�������Ҫ���������ݣ���Ӧ�õ����������ݽ��ջ�����
		* ÿ������������2K����, ��Ϊ�豸ÿ�η��͵����ݰ������ᳬ��MTU��Ԫ�� 
		*/ 
		if((pdevice->video_rcv_buf = (int8 *)malloc(1024 * 2)) == NULL){
			return GEN_ERR;
		}
		if((pdevice->audio_rcv_buf= (int8 *)malloc(1024 * 2)) == NULL){
			// !!! 
			return GEN_ERR;
		}
		if((pdevice->uart_rcv_buf= (int8 *)malloc(1024 * 2)) == NULL){
			// !!! 
			return GEN_ERR;
		}
		if((pdevice->data_rcv_buf= (int8 *)malloc(1024 * 2)) == NULL){
			// !!! 
			return GEN_ERR;
		}
		pdevice->rcv_buf_len = 1024 * 2;

		if((pdevice->stream_snd_buf= (int8 *)malloc(1024 * 2)) == NULL){
			// !!! 
			return GEN_ERR;
		}
		pdevice->stream_snd_buf_len = 1024 * 2;

		pdevice->device_id[0] = 0;
		pdevice->device_ip[0] = 0;
		pdevice->device_port = 0;

		pdevice->free = 0;
	}

// 	mutex = thread_mutex_create();
// 	if(mutex == NULL){
// 		// !!!
// 		return GEN_ERR;
// 	}

	return GEN_OK;
}

static DEVICE_OBJECT *example_get_free_device_object(void)
{
	int32 i;
	DEVICE_OBJECT *pdevice;

	THREAD_MUTEX_LOCK(mutex);

	for(i=0; i<MAX_DEVICE_NUM; i++){
		pdevice = &device_array[i];
		if(pdevice->free == 0){
			pdevice->free = 1;
			THREAD_MUTEX_UNLOCK(mutex);
			return pdevice;
		}
	}	

	THREAD_MUTEX_UNLOCK(mutex);
	return NULL;
}

static void example_release_device_object(DEVICE_OBJECT *pdevice)
{
	THREAD_MUTEX_LOCK(mutex);
	pdevice->handle = NULL;
	pdevice->device_id[0] = 0;
	pdevice->device_ip[0] = 0;
	pdevice->device_port = 0;

	pdevice->free = 0;
	THREAD_MUTEX_UNLOCK(mutex);
	return;
}

static void example_colse_devices(void)
{
	int32 i;
	DEVICE_OBJECT *pdevice;

	THREAD_MUTEX_LOCK(mutex);

	for(i=0; i<MAX_DEVICE_NUM; i++){
		pdevice = &device_array[i];
		if((pdevice->free == 1) && (pdevice->handle != NULL)){
			DX_NVT_request_delete_device(pdevice->handle);
		}
	}	

	THREAD_MUTEX_UNLOCK(mutex);
	return;	
}

void DXSDKAPI example_cb_device_create(DEVICE_Handle handle, void *context, int8 *ip, int32 port, DEVICE_CREATE_MODE mode)
{
	DEVICE_OBJECT *pdevice;
	static int32 create_count = 0;
	DEVICE_BASE_INFO *pbase;
	int8 local_ip[64];
	int32 local_port;
	int32 ret;


	printf("[create] device count: %d.\n", ++create_count);

	if(handle == NULL){
		printf("fail to connect device: ip=%s, port=%d!\n", ip, port);
		return;
	}

	/** 
	* �豸�����ɹ�����ʾ�������ϵ�DXNVT�豸�ɹ�������һ��HTTP������
	* �豸������SDK���Զ���DXNVT�豸��������
	* �豸�������û�����ȵ���DX_NVT_get_device_base_info��ȡ�豸������Ϣ���Ա���������ʼ��������
	*/ 
	pbase = DX_NVT_get_device_base_info(handle);

	if((mode == DEVICE_CREATE_ACTIVE) && (context != NULL)){
		pdevice = (DEVICE_OBJECT *)context;
	}
	else{
		pdevice = example_get_free_device_object();
		if(pdevice == NULL){
			DX_NVT_set_device_context(handle, NULL);
			printf("device queue full! ip:%s, port:%d, mode:%d!\n", ip, port, mode);
			return;
		}
	}

	pdevice->handle = handle;
	pdevice->device_port = port;
	strcpy(pdevice->device_ip, ip);
	strcpy(pdevice->device_id, pbase->id);
	pdevice->mode = mode;

	printf("device[%d] ip: %s, name: %s, id: %s, ver: %s\n", pdevice->id, pdevice->device_ip, pbase->name, pbase->id, pbase->ver_list);


	/** �����豸������ */ 
	DX_NVT_set_device_context(handle, pdevice);

	/** 
	* �豸������Ӧ�õ���DX_NVT_init_media_param��������Ϣ��ʼ��
	* ������豸�Ĳ����������ڲ������ã�������ȡ��������������漰���������Ĳ��������Բ���������Ϣ��ʼ�� 
	*/ 
	DX_NVT_init_media_param(handle, NULL);

	/** 
	* ����ͨ��
	��* 1�������豸���ڶ�ͨ����ý����������4ͨ����Ƶ��2ͨ�����ڣ����ﶼֻ�򿪵�1·ͨ��
	 * 2��һ�㣬��Ӧ�ó����У�Ӧ���������߳�(��������̡߳�ת���̣߳������̵߳�)����Ҫ�򿪻��߹ر�ý�����������ڴ���������ֱ�Ӵ���Ϊ�˼�
	 * 3��STREAM_TRASN_POSITIVE Ϊ�������ݷ���STREAM_TRASN_NEGATIVEΪ�������ݷ���
	 * 4���ھ������ڣ�����Ƶ�����Ƽ���UDP������Э��ʹ��UDP_UNICAST_RAW��Ч�ʱȽϸߣ�����ڹ���������ʹ��TCP_CUSTOM
	 * 5��STREAM_CUSTOM_DATA���͵����ݣ��������䱨�����������ݣ�һ�����������󣬵��Ǻ���Ҫ���Ƽ���TCP������Э��ʹ��TCP_CUSTOM
	 */

	ret = DX_NVT_open_media(handle, STREAM_MEDIA_VIDEO, 1, 
		STREAM_TRASN_POSITIVE, UDP_UNICAST_RAW, NULL);
	ret = DX_NVT_set_media_receive_buf(handle, STREAM_MEDIA_VIDEO, 1, pdevice->video_rcv_buf, pdevice->rcv_buf_len);


	ret = DX_NVT_open_media(handle, STREAM_MEDIA_AUDIO, 1, STREAM_TRASN_POSITIVE, UDP_UNICAST_RAW, NULL);
	ret = DX_NVT_set_media_receive_buf(handle, STREAM_MEDIA_AUDIO, 1, pdevice->audio_rcv_buf, pdevice->rcv_buf_len);

	ret = DX_NVT_open_media(handle, STREAM_MEDIA_UART, 1, STREAM_TRASN_POSITIVE, UDP_UNICAST_RAW, NULL);
	ret = DX_NVT_set_media_receive_buf(handle, STREAM_MEDIA_UART, 1, pdevice->uart_rcv_buf, pdevice->rcv_buf_len);


	ret = DX_NVT_open_media(handle, STREAM_CUSTOM_DATA, 1, STREAM_TRASN_POSITIVE, TCP_CUSTOM, NULL);
	ret = DX_NVT_set_media_receive_buf(handle, STREAM_CUSTOM_DATA, 1, pdevice->data_rcv_buf, pdevice->rcv_buf_len);


	/** 
	* �򿪴��ڷ���ͨ��
	*/
	DX_NVT_open_media(handle, STREAM_MEDIA_UART, 1, STREAM_TRASN_NEGATIVE, UDP_UNICAST_RAW, NULL);

	return;
}

void DXSDKAPI example_cb_set_response(DEVICE_Handle handle, int32 result_code, void *msg_context)
{
	DEVICE_OBJECT *pdevice =(DEVICE_OBJECT *) DX_NVT_get_device_context(handle);
	printf("device[%d] set ok, result:%d.\n", pdevice->id, result_code);
	return;
}

void DXSDKAPI example_cb_get_response(DEVICE_Handle handle, int32 result_code, DEVICE_PARAM *param_list, void *msg_context)
{
	DEVICE_PARAM *param = param_list;
	DEVICE_OBJECT *pdevice = (DEVICE_OBJECT *) DX_NVT_get_device_context(handle);

	printf("device[%d] get ok.\n", pdevice->id);
	while(param != NULL){
		printf("[param] %s: %s\n", param->path, param->pvalue);
		param = param->next;
	}

	return;
}

void DXSDKAPI example_cb_exe_response(DEVICE_Handle handle, int32 result_code, void *msg_context)
{
	DEVICE_OBJECT *pdevice = (DEVICE_OBJECT *) DX_NVT_get_device_context(handle);
	printf("device[%d] exe ok, result: %d.\n", pdevice->id, result_code);
	return;
}

void DXSDKAPI example_cb_snap_response(DEVICE_Handle handle, int32 result_code, SNAP_FORMAT typ, int32 chn, int8 *pdata, int32 len)
{
	DEVICE_OBJECT *pdevice = (DEVICE_OBJECT *) DX_NVT_get_device_context(handle);
	FILE *fp = NULL;
	static int32 jpg_count = 0;
	int8 tmp[128];

	printf("device[%d] snap ok...\n", pdevice->id);

	sprintf(tmp, "./%d.jpg", jpg_count++);
	fp = fopen(tmp, "wb+");
	if(fp == NULL){
		return;
	}

	fwrite(pdata, len, 1, fp);
	fclose(fp);
	return;
}

void DXSDKAPI example_cb_open_media_response(DEVICE_Handle handle, int32 result_code, void *msg_context)
{
	DEVICE_OBJECT *pdevice = (DEVICE_OBJECT *) DX_NVT_get_device_context(handle);
	printf("device[%d] open media ok.\n", pdevice->id);
	return;
}

void DXSDKAPI example_cb_close_media_response(DEVICE_Handle handle, int32 result_code, void *msg_context)
{
	DEVICE_OBJECT *pdevice =(DEVICE_OBJECT *)  DX_NVT_get_device_context(handle);
	printf("device[%d] close media ok.\n", pdevice->id);
	return;
}

void DXSDKAPI example_cb_media_arrive(DEVICE_Handle handle, STREAM_MEDIA_TYPE type, int32 chn, int8 *buf, int32 len, uint32 param)
{
	DEVICE_OBJECT *pdevice = (DEVICE_OBJECT *) DX_NVT_get_device_context(handle);

	//printf("receive type: %d, len: %d\n", type, len);

	/** 
	* 1���ûص����������buf��ΪDX_NVT_set_media_receive_buf�������õ�buf��lenΪ���յ���ý�����ݳ���
	* 2�����ݴﵽ��Ӧ�þ��콫���ݻ����������ȡ�ߣ���Ҫ�ڸûص�������̫������飬����Ӱ�����ݽ���Ч��
	* 3�����ݻ����������ȡ�ߺ�Ӧ���ٵ���DX_NVT_set_media_receive_buf�������������ý��ջ�������������һ������
	*/ 
	DX_NVT_set_media_receive_buf(handle, type, chn, buf, 1024*2);
	return;
}

void DXSDKAPI example_cb_delete(DEVICE_Handle handle, DEVICE_DELETE_REASON reason)
{
	DEVICE_OBJECT *pdevice =(DEVICE_OBJECT *)  DX_NVT_get_device_context(handle);
	static int32 delete_count = 0;

	printf("[delete] device count: %d!\n", ++delete_count);

	if(pdevice != NULL){
		printf("device[%d] closed, ip=%s, reason=%d!\n", pdevice->id, pdevice->device_ip, reason);
		example_release_device_object(pdevice);
	}

	return;
}


/** 
* �����1·��Ƶ�ؼ�֡(I֡)
*/
static int32 example_request_key_frame(DEVICE_Handle handle)
{
	int8 path[16][128];
	int8 value[16][128];
	int8 *ppath[16];
	int8 *pvalue[16];	
	int32 i, ret;

	for(i=0; i<16; i++){
		ppath[i] = path[i];
		pvalue[i] = value[i];
	}

	sprintf(path[0], "/videoEncExe/forceKeyFrame/veId");
	sprintf(value[0], "1");
	ret = DX_NVT_exe(handle, ppath, pvalue, 1, NULL);

	return GEN_OK; 
}

/** 
* �ƾ����ƾ���
*/ 
int32 example_ptz(DEVICE_Handle handle)
{
	return GEN_OK;
}

DWORD WINAPI example_work_thread (PVOID pParam) 
{
	DEVICE_OBJECT *pdevice = example_get_free_device_object();
	int32 ret;

	if(pdevice == NULL){
		return 0;
	}

	/** 
	* 1����������һ���豸����
	* 2��8000Ϊ�豸�ļ����˿�
	* 3���豸������ӳɹ���example_cb_device_create������
	*/ 
	ret = DX_NVT_add_device("192.168.1.100", 8000, NULL, (void *)pdevice);


	while(pdevice->handle==NULL){
		usleep(1);
	}

	/** ��ȡ��1·��Ƶ������� */ 
	ret = DX_NVT_get(pdevice->handle, "/videoEnc/n1", NULL);
	/** �޸��豸���� */ 
	ret = DX_NVT_set(pdevice->handle, "base/name", "DXV8200T1", NULL);			

	example_request_key_frame(pdevice->handle);
	example_ptz(pdevice->handle);

	while(pdevice->handle){
		sleep(1);
	}

	return 1;
}


void CtestDxunSDKDlg::OnBnClickedButton1()
{
	int32 i, ret ;
	uint32 count = 0;
#ifndef _WIN32
	pthread_t thread_handle;
#endif

	example_init_device_object();

	/** ��1�������ûص����� */ 
	DX_NVT_set_callback(
		example_cb_device_create,
		example_cb_set_response,
		example_cb_get_response,
		example_cb_exe_response,
		example_cb_snap_response,
		example_cb_open_media_response,
		example_cb_close_media_response,
		example_cb_media_arrive,
		example_cb_delete);

	/** ��2��������SDKģ�� */
	DX_NVT_open(18000, 28000);

	/** ��3��������һ·������1��������࿪��16·���� */ 
	ret = DX_NVT_start_listen(8080, NULL);


	/** ����һ�������߳� */
#ifndef _WIN32
	ret = pthread_create(&thread_handle, NULL,	(void *(*)(void*))example_work_thread, (void *)NULL);
#else
	HANDLE hThread=CreateThread(NULL, 0,example_work_thread,NULL,0,NULL);

//	WaitForSingleObject( hThread, INFINITE );
	
	CloseHandle(hThread);

#endif 

	for(i=0; i<100; i++){
		sleep(50);
		count++;	
	}

	printf("to close deivces...\n");
	example_colse_devices();
	DX_NVT_close();
	printf("over...\n");
	
	
	//return GEN_OK;

}




void CtestDxunSDKDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CtestDxunSDKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


