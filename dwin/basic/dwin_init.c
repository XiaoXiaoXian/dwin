/*
 * @File:   dwin_init.c 
 * @Author: liu2guang 
 * @Date:   2018-04-22 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-22     liu2guang    update v2 framework. 
 */ 

#include "dwin_init.h" 
#include "dwin_system.h"

/* �ؼ�ͷ�ļ� */ 
#include "dwin_button.h" 
#include "dwin_scale.h" 

struct dwin dwin = {0}; 

/* ��ʼ��dwin�� */ 
int dwin_init(void)
{
    rt_err_t ret = RT_EOK; 
    
    /* ����Ƿ��ظ���ʼ�� */ 
    if(dwin.init == RT_TRUE)
    {
        DWIN_DBG("The dwin lib initialized.\n"); 
        return RT_EBUSY;
    }
    
    /* ��ʼ��dwin���� */ 
    rt_memset(&dwin, 0x00, sizeof(struct dwin)); 
    rt_list_init(&(dwin.pages)); 
    dwin.page_num = 0; 
    dwin.page_cur = RT_NULL; 
    rt_list_init(&(dwin.parses));
    dwin.parse_num = 0; 
    
    /* ��ӡ��Ϣ */ 
#if defined(DWIN_USING_DEBUG)
    DWIN_PRINT("\n");
    DWIN_INFO("Welcome to the dwin \033[32mv%d.%d.%d\033[0m lib.\n", DWIN_VERSION_M, DWIN_VERSION_S, DWIN_VERSION_R); 
    DWIN_INFO("You can find the laster from <\033[31mhttps://github.com/liu2guang/dwin\033[0m>.\n\n"); 
#else
    DWIN_INFO("Welcome to the dwin \033[32mv%d.%d.%d\033[0m lib.\n", DWIN_VERSION_M, DWIN_VERSION_S, DWIN_VERSION_R); 
    DWIN_INFO("You can find the laster from <\033[31mhttps://github.com/liu2guang/dwin\033[0m>.\n"); 
#endif 
    
    /* �����Զ��ϴ����ݼ����� */ 
    extern rt_err_t dwin_watch_init(dwin_t dwin, const char *name, rt_uint32_t baudrate); 
    ret = dwin_watch_init(&dwin, DWIN_USING_UART, DWIN_USING_BAUDRATE); 
    if(ret != RT_EOK)
    {
        DWIN_INFO("The dwin watch start failed!\n"); 
        return ret; 
    } 
    
#if defined(DWIN_USING_DEBUG)
    DWIN_PRINT("\n"); 
#endif 
    dwin_system_touch(RT_TRUE); 
    
    /* �����Ļ��������, ��������Ϊ���¹���ʱ, dwin���޷���������:
     * 1. �����Ļ�ϵ��VAR�����Ƿ�����L22�ֿ��ļ����г�ʼ��. 
     * 2. ��ⴥ�����⹦���Ƿ���. 
     * 3. �������֡CRC�Ƿ�ر�, Ŀǰdwin�ⲻ֧��CRCУ�칦��, �Ժ�Ҳ����֧��, ������. 
     */ 
    rt_bool_t enable = RT_FALSE; 
    ret = dwin_system_var_from_l22_upload(&enable); 
    if(ret != RT_EOK || enable != RT_TRUE)
    {
        DWIN_INFO("The dwin lib need var L22 upload mode. dwin kill rtt system!\n"); 
        while(1); 
    }
    
    ret = dwin_system_backlight_by_touch_ctr(&enable); 
    if(ret != RT_EOK || enable != RT_TRUE)
    {
        DWIN_INFO("The dwin lib need enable backlight by touch. dwin kill rtt system!\n"); 
        while(1); 
    }
    
    ret = dwin_system_crc(&enable); 
    if(ret != RT_EOK || enable != RT_TRUE)
    {
        DWIN_INFO("The dwin lib need disable crc. dwin kill rtt system!\n"); 
        while(1); 
    }
    
    dwin.init = RT_TRUE; 
    
    /* �����Զ���ӡ��Ϣ */ 
#if defined(DWIN_USING_DEBUG)
    rt_uint32_t data = 0; 
    dwin_system_version(&data); 
    DWIN_PRINT("\n");
#endif 
    
    /* ��ʼ���ؼ�(ע��ؼ�������) */ 
    dwin_button_init(); 
    dwin_scale_init(); 
    
    return RT_EOK; 
}
INIT_APP_EXPORT(dwin_init); 
