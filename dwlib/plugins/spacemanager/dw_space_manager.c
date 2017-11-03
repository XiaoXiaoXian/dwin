#include "dw_space_manager.h"
#include "string.h"

/* ���пռ��׵�ַ */
static uint16_t sm_idle_addr = 0x0000;

/* �Ѿ�����Ŀռ��������ͷ */
static list_t *sm_used_head_list = NULL;

/* ��ʼ�������ռ������ */
bool dw_plugin_sm_init(void)
{
    /* ��ʼ�����пռ� */
    sm_idle_addr = 0x0000;
    
    sm_used_head_list = list_new();
    if(sm_used_head_list == NULL)
    {
        return false;
    }
    
    return true;
}

/* ����n�ֽڵı����ռ�, ������ */
dw_space_block_t dw_plugin_sm_apply(const char *name, uint32_t size)
{
    list_node_t *node = NULL;
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
    
    /* ����ڴ�ṹ��ָ����뵽������ */
    node = list_node_new((void *)space);
    list_rpush(sm_used_head_list, node);
    
    /* �����µĿռ���е�ַ */
    sm_idle_addr += size;
    
    return space;
}

/* ��ȡ�ռ�ʣ���С */
uint16_t dw_plugin_sm_idle_space(void)
{
    return (PKG_DWLIB_SM_SPACE_MAXSIZE - sm_idle_addr);
}

/* ��ӡ�Ѿ�ʹ�õ��ڴ� */
void dw_plugin_sm_used_space(void)
{
    list_node_t *node = NULL;
    list_iterator_t *iterator = NULL;
    
    if(sm_used_head_list == NULL)
    {
        dw_print("no used space");
        return;
    }
    
    iterator = list_iterator_new(sm_used_head_list, LIST_HEAD);
    
    while((node = list_iterator_next(iterator)) != NULL)
    {
        dw_print("<space name> = %s <addr> = 0x%.4x <size> = %d", 
            ((struct dw_space_block *)(node->val))->name, 
            ((struct dw_space_block *)(node->val))->addr, 
            ((struct dw_space_block *)(node->val))->size);
    }
    
    dw_print("current space addr = 0x%.4x, used space size = %dByte, free space size = %dByte", 
        sm_idle_addr, sm_idle_addr, dw_plugin_sm_idle_space());
    
    list_iterator_destroy(iterator);
}

/* ����Ĵ�׼�����Ƶ�finsh��������, �ռ����������Ҫclist������ⲿ������ */
#include "finsh.h"
MSH_CMD_EXPORT_ALIAS(dw_plugin_sm_used_space, sm, print used space);
FINSH_FUNCTION_EXPORT_ALIAS(dw_plugin_sm_used_space, sm, print used space);
