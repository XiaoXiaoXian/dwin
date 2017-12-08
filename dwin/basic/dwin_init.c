#include "dwin_init.h" 
#include "dwin_trans.h" 
#include "dwin_space.h" 

/* dwin���ʼ������ */
uint8_t dwin_init(const char *name, uint32_t baudrate)
{
    uint8_t ret = dwin_err_none;
    
    /* ��ʼ�������� */
    ret = dwin_watch_init(name, baudrate);
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    /* ��ʼ���ռ���� */
    ret = dwin_space_init();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    return dwin_err_none;
}
