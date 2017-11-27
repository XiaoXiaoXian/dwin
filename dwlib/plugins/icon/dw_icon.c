#include "dw_icon.h"
#include "dw_system.h"
#include "string.h"

bool dw_plugin_icon_init(void)
{
    return true;
}

/* ����icon */
dw_icon_t dw_plugin_icon_create(const char *name, uint8_t num, uint8_t init_icon)
{
    dw_icon_t icon = (dw_icon_t)DW_MALLOC(sizeof(struct dw_icon));
    if(icon == NULL)
    {
        return NULL;
    }
    
    icon->icon = dw_plugin_sm_apply(name, 1);
    if(icon->icon == NULL)
    {
        DW_FREE(icon);
        return NULL;
    }
    
    /* �������� */
    icon->list = list_new();
    if(icon->list == NULL)
    {
        DW_FREE(icon);
        return NULL;
    }
    
    return icon;
}

/* ��ָ�����ͼ�� */
/* ���������ͼ�� */
bool dw_plugin_icon_add(dw_icon_t icon, const char *name, uint16_t match_value)
{
    list_node_t *node = NULL;
    dw_icon_index_t icon_index = NULL;
    
    /* �����ڴ� */
    icon_index = DW_MALLOC(sizeof(struct dw_icon_index));
    if(icon_index == NULL)
    {
        return false;
    }
    
    /* �������� */
    if(strcpy(icon_index->name, name) == NULL)
    {
        DW_FREE(icon_index);
        return false;
    }
    
    icon_index->match_value = match_value;
    
    /* ����ڴ�ṹ��ָ����뵽������ */
    node = list_node_new((void *)icon_index);
    list_rpush(icon->list, node);
    
    return true;
}

/* �������л�ͼ�� */
/* ������л�ͼ�� */
bool dw_plugin_icon_change(dw_icon_t icon, uint16_t value)
{
    dw_write_val(icon->icon->addr, &value, 1);
    
    return true;
}

