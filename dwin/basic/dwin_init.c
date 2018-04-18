/*
 * @File:   dwin_init.c 
 * @Author: liu2guang 
 * @Date:   2017-12-18 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-18     liu2guang    ʵ��dwin���ʼ�������. 
 */ 
 
#include "dwin_init.h" 
#include "dwin_trans.h" 
#include "dwin_space.h" 
#include "dwin_system.h" 

static bool dwin_is_init = false;

/* ��ȡ��ʼ��״̬ */
bool dwin_get_init_state(void)
{
    return dwin_is_init; 
}

/* dwin���ʼ������ */
uint8_t dwin_init(const char *uart, uint32_t baudrate)
{
    uint8_t ret = dwin_err_none; 
    uint8_t major, minor, revise, data[8];
    
    /* ����Ƿ��Ѿ���ʼ�� */
    if(dwin_get_init_state() == true)
    {
        dwin_println("\nwarning! dwin lib already inited!"); 
        return dwin_err_reinit; 
    }
    
    /* ��ʼ�������� */
    ret = dwin_watch_init(uart, baudrate); 
    if(ret != dwin_err_none)
    {
        dwin_println("error! watch init failed %d!", ret); 
        return dwin_err_error; 
    }
    
    /* ��ʼ���ռ���� */
    ret = dwin_space_init();
    if(ret != dwin_err_none)
    {
        dwin_println("error! watch init failed %d!", ret); 
        return dwin_err_error; 
    }
    
    dwin_system_set_page_id(0); 
    
    /* DWIN��汾 */
    dwin_system_sw_version(&major, &minor, &revise);
    dwin_println("dwin lib ver %d.%d.%d.", major, minor, revise);
    
    /* ��Ļ�汾��Ϣ */
    dwin_system_hw_version(&major, &minor, &revise);
    dwin_println("Hardware ver %d.%d.%d.", major, minor, revise);
    
    /* ��ȡ��ǰ�������� */
    dwin_system_get_backlight_lvl(data);
    dwin_println("Backlight level %d(0x%.2x).", data[0], data[0]);
    
    /* ���������Զ��ϴ� */
    dwin_system_set_autoupdate(1); 
    dwin_println("Config dwin autoupdate."); 

    /* l22�������� */
    dwin_system_set_dataload_mode(1); 
    dwin_println("Config dwin load data from l22."); 

    /* Ĭ��ʹ�ܷ����� */
    dwin_system_set_buzzer(1); 
    dwin_println("Config dwin buzzer on."); 
    
    /* ��ʼ����� */
    dwin_is_init = true; 
    
    dwin_println("Welcome to the dwin library."); 
    dwin_println("You can find the latest version from https://github.com/liu2guang/dwin."); 
    
    return dwin_err_none;
}
