#ifndef __SW_CONF_H_ 
#define __SW_CONF_H_ 

/* ֡ͷ���� */
#if !defined(DW_FRAME_HBYTE) 
#define DW_FRAME_HBYTE (0x5a)
#endif
#if !defined(DW_FRAME_LBYTE) 
#define DW_FRAME_LBYTE (0xa5)
#endif

/* �ڴ���� */
#if !defined(DW_MALLOC) 
#if defined(PKG_DWLIB_MM_RTT)
#define DW_MALLOC rt_malloc
#endif
#if defined(PKG_DWLIB_MM_C)
#define DW_MALLOC malloc
#endif
#endif

#if !defined(DW_FREE) 
#if defined(PKG_DWLIB_MM_RTT)
#define DW_FREE rt_free
#endif
#if defined(PKG_DWLIB_MM_C)
#define DW_FREE free
#endif
#endif

/* ��ӡ�ӿ� */
#if !defined(dw_print) 
#include "rtthread.h"
#if defined(PKG_DWLIB_PRINT_RTT)
#define dw_print(...) rt_kprintf("[dwlib] "); rt_kprintf(__VA_ARGS__); rt_kprintf("\n")
#endif
#if defined(PKG_DWLIB_PRINT_ELOG)
#include "elog.h"
#define dw_print(...) elog_i("dwlib", __VA_ARGS__)
#endif
#if defined(PKG_DWLIB_PRINT_C)
#define dw_print(...) printf("[dwlib] "); printf(__VA_ARGS__); printf("\n")
#endif
#endif

/* dwlib��ռ������ */
#if defined(PKG_DWLIB_ENABLE_SM) 

/* �ռ������ */
#if !defined(PKG_DWLIB_SM_NAME_LENGTH) 
    #define PKG_DWLIB_SM_NAME_LENGTH    (12)
#endif
    
/* �ռ��������ַ(�ֽ�) */
#if !defined(PKG_DWLIB_SM_SPACE_MAXSIZE) 
    #define PKG_DWLIB_SM_SPACE_MAXSIZE  (4096)
#endif

#endif /* PKG_DWLIB_ENABLE_SM */

/* dwlib�ⰴ����� */
#if defined(PKG_DWLIB_ENABLE_BUTTON) 

#if !defined(PKG_DWLIB_BTN_NAME_LENGTH) 
    #define PKG_DWLIB_BTN_NAME_LENGTH   (12)
#endif 

#endif /* PKG_DWLIB_ENABLE_BUTTON */

#endif
