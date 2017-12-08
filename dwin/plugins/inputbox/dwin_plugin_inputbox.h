#ifndef __DWIN_PLUGIN_INPUTBOX_H_ 
#define __DWIN_PLUGIN_INPUTBOX_H_ 

#include "dwin_space.h" 

typedef struct dwin_inputbox
{
    uint8_t state;
    uint32_t input_value;
    void (*inputbox_cb)(uint32_t input_value, void *args);
    void *args;
} *dwin_inputbox_t; 

/* ���»ص����� */
typedef void (*inputbox_cb)(uint32_t input_value, void *args);

/* ����ģʽ */
enum inputbox_state
{
    inputbox_stop = 0,    /* ֹͣģʽ */
    inputbox_start        /* ����ģʽ */
};

/* �ⲿ���� */
uint8_t  dwin_plugin_inputbox_create(const char *name, inputbox_cb cb, void *p);
uint8_t  dwin_plugin_inputbox_mode(const char *name);
uint8_t  dwin_plugin_inputbox_update(const char *name,  inputbox_cb cb, void *p);
uint32_t dwin_plugin_inputbox_read(const char *name);

#endif
