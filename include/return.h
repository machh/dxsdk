/**
 * @file return.h  返回值定义
 *
 * Copyright (C) 2009 北京大讯科技有限公司研发部 
 * 
 * @author wenming <wenming.studio@gmail.com>
 * @version 0.1
 * @date 2009.5.31
 *
 *
 */

#ifndef __RETURN_H__
#define __RETURN_H__


/** 
 * @name 通用函数返回值定义
 * @{
 */
/** 操作成功 */
#define GEN_OK     (0)
/** 操作错误 */ 
#define GEN_ERR    (-1)
/** 操作失败 */ 
#define GEN_FAIL   (-2)
/** 容器已满 */
#define GEN_FULL   (-6)
/** 已经创建 */
#define GEN_EXIST  (-7)
/** 完成/结束 */
#define GEN_DONE   (-8)

/** 内存分配失败 */
#define GEN_MEM    (-11)
/** 超时 */
#define GEN_TIME_OUT (-301)
/** 超过数目上限 */
#define GEN_UPPER_LIMIT (-302)
/** 没有权限 */
#define GEN_REFUSE   (-303)
/** 过期的，结束的 */
#define GEN_PAST     (-305)
/** 参数错误 */
#define GEN_INVALID_PARAM  (-306)

/** 
 * @}
 */ 


















#endif ///__RETURN_H__

