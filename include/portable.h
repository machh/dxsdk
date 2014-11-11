/**
 * @file portable.h  
 *
 * Copyright (C) 2009 北京大讯科技有限公司研发部 
 * 
 * @author wenming <wenming.studio@gmail.com>
 * @version 0.1
 * @date 2009.07.16
 *
 *
 */
 
#ifndef __PORTALBE_H__
#define __PORTALBE_H__


#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.VC80.CRT' version='8.0.50608.0' processorArchitecture='X86' publicKeyToken='1fc8b3b9a1e18e3b' language='*'\"")


typedef char    int8;
typedef short   int16;
typedef int     int32;
#ifndef WIN32
typedef long long   int64;
#else
typedef long        int64;
#endif

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
#ifndef WIN32
typedef unsigned long long   uint64;
#else
typedef unsigned long        uint64;
#endif

typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed int      int32_t;
#ifndef WIN32
typedef long long       int64_t;
#else
typedef long            int64_t;
#endif

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#ifndef WIN32
typedef unsigned long long   uint64_t;
#else
typedef unsigned long        uint64_t;
#endif

#ifdef __cplusplus

#else
typedef uint8 bool;
#endif 

#ifndef true
	#define true  1
#endif 
#ifndef false
	#define false 0
#endif 

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif 



#endif //__PORTALBE_H__
































































