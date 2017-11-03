#ifndef __SW_CONF_H_ 
#define __SW_CONF_H_ 

#include "dw_port.h"
#include "rtthread.h"

/* �ڴ���� */
#if !defined(DW_MALLOC) 
#define DW_MALLOC malloc
#endif

#if !defined(DW_FREE) 
#define DW_FREE free
#endif

/* dwlib��ռ������ */
#if defined(PKG_DWLIB_ENABLE_SM) 

/* �ռ������ */
#if !defined(PKG_DWLIB_SM_NAME_LENGTH) 
    #define PKG_DWLIB_SM_NAME_LENGTH (8)
#endif
    
/* �ռ��������ַ(�ֽ�) */
#if !defined(PKG_DWLIB_SM_SPACE_MAXSIZE) 
    #define PKG_DWLIB_SM_SPACE_MAXSIZE (4096)
#endif

#endif

#endif
