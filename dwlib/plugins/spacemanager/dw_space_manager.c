#include "dw_space_manager.h"
#include "string.h"

/* ���пռ��׵�ַ */
static uint16_t sm_idle_addr = 0x0000;

/* ��ʼ�������ռ������ */
bool dw_plugin_sm_init(void)
{
    /* ��ʼ�����пռ� */
    sm_idle_addr = 0x0000;
    
    return true;
}

/* ����n�ֽڵı����ռ�, ������ */
dw_space_block_t dw_plugin_sm_apply(const char *name, uint32_t size)
{
    dw_space_block_t space = NULL;
    
    /* �жϲ��� */
    if(name == NULL || size == 0)
    {
        return NULL;
    }
    
    /* ��������ڴ��С */
    if(dw_plugin_sm_idle_space() < size)
    {
        return NULL;
    }
    
    /* �����ڴ� */
    space = DW_MALLOC(sizeof(struct dw_space_block));
    
    /* �������� */
    if(strcpy(space->name, name) == NULL)
    {
        return NULL;
    }
    
    /* ��ȡ����Ŀռ��С���ַ */
    space->size = size;
    space->addr = sm_idle_addr;
    
    /* �����µĿռ���е�ַ */
    sm_idle_addr += size;
    
    return space;
}

/* ��ȡ�ռ�ʣ���С */
uint16_t dw_plugin_sm_idle_space(void)
{
    return (PKG_DWLIB_SM_SPACE_MAXSIZE - sm_idle_addr);
}
