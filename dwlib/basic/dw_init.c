#include "dw_init.h"
#include "dw_port.h"
#include "dw_system.h"

#if defined(PKG_DWLIB_ENABLE_SM)
#include "dw_space_manager.h"
#endif

/* dwlib��ʼ�� */
bool dw_init(const char *name, uint32_t baudrate)
{
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
    dw_touch_data_update_enable(false);
    
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
    
    return true;
}
