/*
 * @File:   dwin_button.c 
 * @Author: liu2guang 
 * @Date:   2018-06-07 17:27:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-07     liu2guang    implementing the button object. 
 */ 

#include "dwin_button.h" 
#include "dwin_obj.h" 
#include "dwin_page.h" 
#include "dwin_parse.h" 

#define DWIN_BUTTON_VAR_SIZE (1) 

static struct dwin_parse *parse = RT_NULL; 

/* Button�������¼������� */ 
static void dwin_button_event(struct dwin_obj *obj)
{
    ((struct dwin_button *)obj)->push_cb(); 
}

struct dwin_button *dwin_button_create(struct dwin_page *page, rt_uint16_t addr, void (*push_cb)(void))
{
    struct dwin_button *button = RT_NULL; 
    
    button = (struct dwin_button *)rt_malloc(sizeof(struct dwin_button)); 
    if(button == RT_NULL)
    {
        DWIN_DBG("Create button failed memory is not enough.\n"); 
        goto failed; 
    }
    
    dwin_obj_init(&(button->obj), addr, DWIN_BUTTON_VAR_SIZE, DWIN_WIDGET_TYPE_BUTTON); 
    dwin_page_add_obj(page, &(button->obj)); 
    button->push_cb = push_cb; 
    
    return button; 
    
failed:
    if(button != RT_NULL)
    {
        rt_free(button); 
    }
    
    return RT_NULL; 
}

/* ��ʼ��Button�ؼ������� */ 
rt_err_t dwin_button_init(void)
{
    /* ע��ؼ������� */ 
    parse = dwin_parse_create(DWIN_WIDGET_TYPE_BUTTON, dwin_button_event); 
    dwin_parse_register(parse); 
    
    return RT_EOK; 
}
