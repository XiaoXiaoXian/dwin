#include "dwin_transport.h" 

/* �������ַ�����״̬ */
#define DWIN_STATE_NONE     (0)
#define DWIN_STATE_FD_FS    (1)
#define DWIN_STATE_FD_FE    (2)
#define DWIN_STATE_PARSE    (3)

/* ��������ݽṹ���� */
typedef struct dwin_transport
{
    rt_device_t device;     /* �����豸 */
    rt_thread_t thread;     /* �������߳� */
    rt_sem_t    rxsem;      /* �����ź��� */
    struct dwin_var var;    /* ��������֡ */
} *dwin_transport_t;

/* ��������ݽṹ */
static struct dwin_transport handle;

/* �����ַ� */
static dwin_err_t dwin_putc(char ch)
{
    if(rt_device_write(handle.device, (-1), &ch, 1) != 1)
    {
        return dwin_err_error;
    }
    
    return dwin_err_none;
}

/* �����ַ� */
static char dwin_getc(void)
{
    char ch;
    
    while(rt_device_read(handle.device, (-1), &ch, 1) != 1)
    {
        rt_sem_take(handle.rxsem, RT_WAITING_FOREVER);
    }
    
    return ch;
}

/* ���ڽ��ջص����� */
static rt_err_t dwin_uart_rx_callback(rt_device_t dev, rt_size_t size)
{
    return rt_sem_release(handle.rxsem);
}

/* ������ʾ���Զ��ϴ����ݼ����� */
static void dwin_autoupload_watch(void *p)
{
    char ch;
    static uint8_t pos = 0;
    static uint8_t state = DWIN_STATE_NONE;
    
    for(;;)
    {
        /* ������ȡ�ַ� */
        ch = dwin_getc();
        
        /* �жϽ��յ��ַ��Ƿ��Ǳ�������֡ͷ���ֽ� */
        if(state == DWIN_STATE_NONE)
        {
            if(ch == DWIN_HEAD1)
            {
                state = DWIN_STATE_FD_FS;
                continue;
            }
            
            continue;
        }
        
        /* �жϽ��յ��ַ��Ƿ��Ǳ�������֡ͷ���ֽ� */
        if(state == DWIN_STATE_FD_FS)
        {
            if(ch == DWIN_HEAD2)
            {
                state = DWIN_STATE_FD_FE;
                continue;
            }
            else
            {
                state = DWIN_STATE_NONE;
                continue;
            }
        }
        
        /* �жϽ��յ��ַ��Ƿ������ݳ��� */
        if(state == DWIN_STATE_FD_FE)
        {
            state = DWIN_STATE_PARSE;
            
            handle.var.length = ch;
            pos = 0;
            
            #if defined(DWIN_DEBUG)
            dwin_print("Autoupload len = %d ", handle.var.length);
            #endif
            
            continue;
        }
        
        /* �������� */
        if(state == DWIN_STATE_PARSE)
        {
            ((dwin_var_t)(&handle.var))->sendbuffer[pos+3] = ch;
            pos++;
            
            if(pos == handle.var.length)
            {
                /* ���ý����� */
                
                /* ���Դ�ӡ */
                #if defined(DWIN_DEBUG)
                {
                    uint8_t index = 0;
                    for(;index < handle.var.length+3; index++)
                    {
                        dwin_print("[0x%.2x] ", ((dwin_var_t)(&handle.var))->sendbuffer[index]);
                    }
                    dwin_print("\n");
                }
                #endif
                
                /* ������� */
                handle.var.length = 0;
                pos = 0;
                
                state = DWIN_STATE_NONE;
                continue;
            }
        }
    }
}

/* ����������ʾ���Զ��ϴ����ݼ����� */
static dwin_err_t dwin_autoupload_watch_start(void)
{
    /* �����Զ��ϴ����ݼ����� */
    handle.thread = rt_thread_create("twatch", dwin_autoupload_watch, 
        RT_NULL, 1024, 10, 10);
    if(handle.thread == NULL)
    {
        return dwin_err_nosys;
    }
    
    /* �����������߳� */
    rt_thread_startup(handle.thread);
    
    return dwin_err_none;
}

/* ֹͣ������ʾ���Զ��ϴ����ݼ����� */
static dwin_err_t dwin_autoupload_watch_stop(void)
{
    /* ɾ���������߳� */
    if(rt_thread_delete(handle.thread) != RT_EOK)
    {
        return dwin_err_error;
    }
    
    return dwin_err_none;
}

/* ���䷢�ͱ�������֡ */
dwin_err_t dwin_var_send(dwin_var_t var, uint32_t timeout)
{
    return dwin_err_none;
}

/* ���䷢�ͱ�������֡ */
dwin_err_t dwin_var_recv(dwin_var_t var, uint16_t addr, uint8_t num, uint32_t timeout)
{
    char ch;
    static uint8_t pos = 0;
    static uint8_t state = DWIN_STATE_NONE;
    dwin_err_t ret = dwin_err_none;
    
    /* ֹͣ������ */
    ret = dwin_autoupload_watch_stop();
    if(ret != dwin_err_none)
    {
        return dwin_err_error;
    }
    
    /* ����var����֡ */
    dwin_putc(DWIN_HEAD1);
    dwin_putc(DWIN_HEAD2);
    dwin_putc(4);
    dwin_putc(0x83);
    dwin_putc((addr&0xFF00) >> 8);
    dwin_putc((addr&0x00FF) >> 0);
    dwin_putc(num);
    
    var->var.head1 = DWIN_HEAD1;
    var->var.head2 = DWIN_HEAD2;
    
    /* �ȴ������ϴ� */
    for(;;)
    {
        /* ������ȡ�ַ� */
        ch = dwin_getc();
        
        /* �жϽ��յ��ַ��Ƿ��Ǳ�������֡ͷ���ֽ� */
        if(state == DWIN_STATE_NONE)
        {
            if(ch == DWIN_HEAD1)
            {
                state = DWIN_STATE_FD_FS;
                continue;
            }
            
            continue;
        }
        
        /* �жϽ��յ��ַ��Ƿ��Ǳ�������֡ͷ���ֽ� */
        if(state == DWIN_STATE_FD_FS)
        {
            if(ch == DWIN_HEAD2)
            {
                state = DWIN_STATE_FD_FE;
                continue;
            }
            else
            {
                state = DWIN_STATE_NONE;
                continue;
            }
        }
        
        /* �жϽ��յ��ַ��Ƿ������ݳ��� */
        if(state == DWIN_STATE_FD_FE)
        {
            state = DWIN_STATE_PARSE;
            
            var->var.length = ch;
            pos = 0;
            
            #if defined(DWIN_DEBUG)
            dwin_print("Revcupload len = %d ", var->var.length);
            #endif
            
            continue;
        }
        
        /* �������� */
        if(state == DWIN_STATE_PARSE)
        {
            var->sendbuffer[pos+3] = ch;
            pos++;
            
            if(pos == var->var.length)
            {
                /* ���ý����� */
                
                /* ���Դ�ӡ */
                #if defined(DWIN_DEBUG)
                {
                    uint8_t index = 0;
                    for(;index < var->var.length+3; index++)
                    {
                        dwin_print("[0x%.2x] ", var->sendbuffer[index]);
                    }
                    dwin_print("\n");
                }
                #endif
                
                /* ������� */
                var->var.length = 0;
                pos = 0;
                
                state = DWIN_STATE_NONE;
                break;
            }
        }
    }
    
    dwin_autoupload_watch_start();
    
    return dwin_err_none;
}

dwin_err_t dwin_reg_send(dwin_reg_t reg, uint32_t timeout)
{
    return dwin_err_none;
}

dwin_err_t dwin_reg_recv(dwin_reg_t reg, uint16_t addr, uint8_t num, uint32_t timeout)
{
    return dwin_err_none;
}

/* ��ʼ��������ʾ���Զ��ϴ����ݼ����� */
dwin_err_t dwin_autoupload_watch_init(const char *name)
{
    rt_err_t ret = RT_EOK;
    
    /* ������� */
    dwin_assert(name != NULL);
    
    /* �򿪴����豸 */
    handle.device = rt_device_find(name);
    if(handle.device == NULL)
    {
        return dwin_err_nosys;
    }
    
    /* ���豸 */
    ret = rt_device_open(handle.device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    if(ret != RT_EOK)
    {
        handle.device = NULL;
        return dwin_err_nosys;
    }
    
    /* ���ô��ڽ��ջص����� */
    ret = rt_device_set_rx_indicate(handle.device, dwin_uart_rx_callback);
    if(ret != RT_EOK)
    {
        rt_device_close(handle.device);
        handle.device = NULL;
        return dwin_err_nosys;
    }
    
    /* �����ź��� */
    handle.rxsem = rt_sem_create("dwinrxsem", 0, RT_IPC_FLAG_FIFO);
    if(handle.rxsem == NULL)
    {
        rt_device_set_rx_indicate(handle.device, NULL);
        rt_device_close(handle.device);
        handle.device = NULL;
        return dwin_err_nosys;
    }
    
    /* ��ʼ��var���� */
    handle.var.head1 = DWIN_HEAD1;
    handle.var.head2 = DWIN_HEAD2;
    
    /* ���������� */
    if(dwin_autoupload_watch_start() != dwin_err_none)
    {
        rt_sem_delete(handle.rxsem);
        rt_device_set_rx_indicate(handle.device, NULL);
        rt_device_close(handle.device);
        handle.device = NULL;
        return dwin_err_nosys;
    }
    
    return dwin_err_none;
}
