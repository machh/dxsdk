#ifndef __DX_NVT_AUTOLINK_H__
#define __DX_NVT_AUTOLINK_H__

#ifdef _DEBUG
#	ifdef _DLL
#		define DXNVT_LIB_NAME "dxnvtsdkd.lib"
#	else
#		define DXNVT_LIB_NAME "dxnvtsdk-rd.lib"
#	endif
#else
#	ifdef _DLL
#		define DXNVT_LIB_NAME "dxnvtsdk.lib"
#	else
#		define DXNVT_LIB_NAME "dxnvtsdk-r.lib"
#	endif
#endif

#ifdef DXNVT_LIB_PATH
#pragma comment(lib, DXNVT_LIB_PATH DXNVT_LIB_NAME)
#else
#pragma comment(lib, DXNVT_LIB_NAME)
#endif
#pragma message("Linking to dxnvt library file: "DXNVT_LIB_NAME)

#endif /// __DX_NVT_AUTOLINK_H__
