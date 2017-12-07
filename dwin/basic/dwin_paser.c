#include "dwin_paser.h" 
#include "dwin_space.h" 
#include "list.h" 

#include "dwin_plugin_button.h" 

extern list_t *dwin_space_list;

/* ����api */
/* �Զ��ϴ������������ */
uint8_t dwin_paser(uint8_t *data, uint8_t len)
{
    list_node_t *node = RT_NULL;
    list_iterator_t *iterator = RT_NULL;
    
    /* �ж�����֡�Ƿ��ǺϷ��� */
    if(data[3] != DWIN_VAR_READ)
    {
        dwin_println("dwin paser data error");
        return dwin_err_error;
    }
    
    iterator = list_iterator_new(dwin_space_list, LIST_HEAD);
    
    /* �����������б� */
    while((node = list_iterator_next(iterator)) != RT_NULL)
    {
        /* ��ַƥ�� and ����ƥ�� */
        if(((dwin_space_t)(node->val))->addr == ((data[4] << 8) + (data[5])) && 
           ((dwin_space_t)(node->val))->len  == data[6])
        {
            switch(((dwin_space_t)(node->val))->type)
            {
                /* ������������ֹͣ����ʵ��!!!! ���뻹δд */
                case dwin_type_button:
                {
                    /* �ж��Ƿ�ע���˻ص����� */
                    if(((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->press_cb != RT_NULL && 
                       ((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->match_value == ((data[7]<<8)+data[8]))
                    {
                        ((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->press_cb(((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->args);
                    }
                }
                break;
            }
        }
    }
    
    return dwin_err_none;
}
