#ifndef __DW_INIT_H_
#define __DW_INIT_H_

#include "dw_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ע�������Ļ�豸,Ŀǰֻ֧����ͨ����, �жϽ��յĴ����豸 */
rt_err_t rt_device_dw_init(rt_device_dw_t device, const char *name, rt_uint8_t init_bl);

#ifdef __cplusplus
}
#endif

#endif
