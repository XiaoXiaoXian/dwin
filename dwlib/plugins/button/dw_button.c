#include "dw_button.h" 
#include "dw_system.h" 
#include "dw_space_manager.h" 
#include "string.h"

static dw_space_block_t btn_space = NULL;

/* �Ѿ�����Ŀռ��������ͷ */
static list_t *btn_head_list = NULL;

/* ��ʼ�������ռ������ */
bool dw_plugin_button_init(void)
{
    /* ����2�ֽڿռ����ڰ������ */
    btn_space = dw_plugin_sm_apply("btn_plugin", 1);
    if(btn_space == NULL)
    {
        dw_print("button space apply failed");
        return false;
    }
    
    /* �������� */
    btn_head_list = list_new();
    if(btn_head_list == NULL)
    {
        return false;
    }
    
    return true;
}

/* ע�ᰴ�� */ 
bool dw_plugin_button_add(const char *name, uint16_t match_value, btn_callback func, void *args)
{
    list_node_t *node = NULL;
    dw_button_t btn = NULL;
    
    /* �жϲ��� */
    if(name == NULL)
    {
        return NULL;
    }
    
    /* �����ڴ� */
    btn = DW_MALLOC(sizeof(struct dw_button));
    if(btn == NULL)
    {
        return false;
    }
    
    /* �������� */
    if(strcpy(btn->name, name) == NULL)
    {
        return false;
    }
    
    /* ע��ص�������ƥ��ֵ */
    btn->func        = func;
    btn->args        = NULL;
    btn->match_value = match_value;
    
    /* ����ڴ�ṹ��ָ����뵽������ */
    node = list_node_new((void *)btn);
    list_rpush(btn_head_list, node);
    
    return true;
}

/* ��ӡ�Ѿ�ע��İ��� */
void dw_plugin_button_used_btn(void)
{
    uint16_t count = 0;
    list_node_t *node = NULL;
    list_iterator_t *iterator = NULL;
    
    if(btn_head_list == NULL)
    {
        dw_print("no used btn");
        return;
    }
    
    iterator = list_iterator_new(btn_head_list, LIST_HEAD);
    
    while((node = list_iterator_next(iterator)) != NULL)
    {
        count++;
        dw_print("<btn name> = %s <match_value> = 0x%.4x", 
            ((struct dw_button *)(node->val))->name, 
            ((struct dw_button *)(node->val))->match_value);
    }
    
    dw_print("current button add count = %d", count);
    
    list_iterator_destroy(iterator);
}
/* ����Ĵ�׼�����Ƶ�finsh��������, �����������Ҫclist������ⲿ������ */
#include "finsh.h"
MSH_CMD_EXPORT_ALIAS(dw_plugin_button_used_btn, smb, print used button);
FINSH_FUNCTION_EXPORT_ALIAS(dw_plugin_button_used_btn, smb, print used button);

void dw_plugin_button_event_poll(void)
{
    uint16_t val = 0x0000;
    list_node_t *node = NULL;
    list_iterator_t *iterator = NULL;
    
    if(btn_head_list == NULL)
    {
        dw_print("no used btn");
        return;
    }
    
    /* ��ȡ��ǰ����ֵ */
    dw_read_val(btn_space->addr, &val, 1);
    
    iterator = list_iterator_new(btn_head_list, LIST_HEAD);
    while((node = list_iterator_next(iterator)) != NULL)
    {
        if(((struct dw_button *)(node->val))->match_value == val && 
           ((struct dw_button *)(node->val))->func != NULL)
        {
            ((struct dw_button *)(node->val))->func(((struct dw_button *)(node->val))->args);
            
            val = 0x0000;
            dw_write_val(btn_space->addr, &val, 1);
        }
        
    }
    
    list_iterator_destroy(iterator);
}
