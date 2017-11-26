#include "dw_init.h"
#include "dw_port.h"
#include "dw_system.h"

#if defined(PKG_DWLIB_ENABLE_SM)
#include "dw_space_manager.h"
#endif

#if defined(PKG_DWLIB_ENABLE_BUTTON)
#include "dw_button.h"
#endif

#if defined(PKG_DWLIB_ENABLE_BUTTON)
void dwin_event_thread_entry(void *p)
{
    for(;;)
    {
        /* �����¼�����ص� */
#if defined(PKG_DWLIB_ENABLE_BUTTON)
        dw_plugin_button_event_poll();
#endif
        
        rt_thread_delay(RT_TICK_PER_SECOND/10);
    }
}
#endif

/* dwlib��ʼ�� */
bool dw_init(const char *name, uint32_t baudrate)
{
#if defined(PKG_DWLIB_ENABLE_BUTTON)
    rt_thread_t tdwin_event = NULL;
#endif
    
    if(dw_port_serial_resource_init() == false)
    {
        dw_print("serial resource init failed");
        return false;
    }
    
    if(dw_port_serial_init(name, baudrate) == false)
    {
        dw_print("serial init failed");
        return false;
    }
    
    /* �ر��Զ��ϴ����� */
    dw_touch_data_update_enable(false); /* ��������? */
    
    /* ��ʼ��L22�ֿ���ؿռ�������� */
    dw_l22_mode_init();
    
    /* ������������ */
#if defined(PKG_DWLIB_ENABLE_TOUCH_BEEP)
    dw_touch_beep_enable(true);
#else
    dw_touch_beep_enable(false);
#endif
    
    /* ������������ */
#if defined(PKG_DWLIB_ENABLE_TOUCH_BACKLIGHT)
    dw_touch_blacklight_enable(true);
#else
    dw_touch_blacklight_enable(false);
#endif
    
#if defined(PKG_DWLIB_ENABLE_SM)
    if(dw_plugin_sm_init() == false)
    {
        dw_print("space manager init failed");
        return false;
    }
#endif
    
#if defined(PKG_DWLIB_ENABLE_BUTTON)
    if(dw_plugin_button_init() == false)
    {
        dw_print("button init failed");
        return false;
    }
#endif
    
    
    /* �¼������� */
#if defined(PKG_DWLIB_ENABLE_BUTTON)
    tdwin_event = rt_thread_create(
        "tdwin_event", dwin_event_thread_entry, RT_NULL, 
        512, 5, 10);
    
    if(tdwin_event == RT_NULL)
    {
        dw_print("event thread create failed");
        return false;
    }
    
    rt_thread_startup(tdwin_event);
#endif
    
    return true;
}
