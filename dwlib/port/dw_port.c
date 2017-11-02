#include "dw_port.h"

/* �û�ͷ�ļ� */
#include "rtthread.h"

/* �û����� */
static rt_device_t dev = RT_NULL;
static struct rt_semaphore serial_res;

// ���ڳ�ʼ��port����
bool dw_port_serial_init(const char *name, uint32_t baudrate)
{
    rt_err_t ret = RT_ERROR;
    
    dev = rt_device_find(name);
    if(dev == RT_NULL)
    {
        return false;
    }
    
    /* �ɶ�д���� + �жϽ��� */
    ret = rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    if(ret != RT_EOK)
    {
        return false;
    }
    
    return true;
}

// ����һ���ֽ����ݵ�����
bool dw_port_serial_putc(char byte)
{
    rt_size_t size = 0;
    
    size = rt_device_write(dev, 0, &byte, 1);
    if(size != 1)
    {
        return false;
    }
    
    return true;
}

// �Ӵ��ڽ���һ������, Ҫ�󴮿����첽����
bool dw_port_serial_getc(char *byte)
{
    rt_size_t size = 0;
    
    size = rt_device_read(dev, 0, byte, 1);
    if(size != 1)
    {
        return false;
    }
    
    return true;
}

/* ��ʼ��������Դ�� */
bool dw_port_serial_resource_init(void)
{
    rt_err_t ret = RT_ERROR;
    
    ret = rt_sem_init(&serial_res, "dw res", 0x01 , RT_IPC_FLAG_PRIO);
    if(ret != RT_EOK)
    {
        return false;
    }
    
    return true;
}

/* ����ʱʱ���ȡ������Դ�� */
bool dw_port_serial_resource_take(int32_t timeout)
{
    rt_err_t ret = RT_ERROR;
    
    ret = rt_sem_take(&serial_res, timeout);
    if(ret != RT_EOK)
    {
        return false;
    }
    
    return true;
}

/* �ͷŴ�����Դ�� */
bool dw_port_serial_resource_release(void)
{
    rt_err_t ret = RT_ERROR;
    
    ret = rt_sem_release(&serial_res);
    if(ret != RT_EOK)
    {
        return false;
    }
    
    return true;
}
