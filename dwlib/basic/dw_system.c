#include "dw_system.h"
#include "dw_port.h"

/* BCD��תINT��, ����0x12 -> 12 */
static uint8_t dw_bcd2int(uint8_t bcd)
{
    return bcd - (bcd >> 4)*6;
}

/* INT��תBCD��, ����12 -> 0x12 */
static uint8_t dw_int2bcd(uint8_t dec)
{
    return (uint8_t)(dec + (dec/10)*6);
}

/* ���Ĵ��� */
static bool dw_read_reg(uint8_t addr, uint8_t *buffer, uint8_t length)
{
    uint8_t c;
    uint8_t index = 0;
    static uint8_t rxbuffer[256] = {0};
    static uint8_t txbuffer[  6] = 
    {
        DW_FRAME_HBYTE,     /* ͨ��֡ͷ���ֽ� offset 0 */
        DW_FRAME_LBYTE,     /* ͨ��֡ͷ���ֽ� offset 1 */
        3,                  /* ͨ�����ݳ���   offset 2 */
        DW_REG_R_CMD,       /* ���Ĵ���ָ��   offset 3 */
        0,                  /* ���Ĵ�����ַ   offset 4 */
        0                   /* ���Ĵ�������   offset 5 */
    };
    
    /* �������, ���д�׶�ʹ��, ��д��ɺ����ɾ�� */
    if((buffer == NULL) || (length == 0))
    {
        return false;
    }
    
    /* �޸�rxbuffer */
    txbuffer[4] = addr;
    txbuffer[5] = length;
    
    /* ��ȡ������Դ */
    dw_port_serial_resource_take();
    
    /* �������� */
    for(index = 0; index < 6; index++)
    {
        if(dw_port_serial_putc((char)txbuffer[index]) == false)
        {
            dw_port_serial_resource_release();
            return false;
        }
    }
    
    /* �ȴ�����ָ�����ȵ����� */
    index = 0;
    while(index != (length + 6))
    {
        if(dw_port_serial_getc((char *)&c) == true)
        {
            rxbuffer[index++] = c;
        }
    }
    
    /* �������� */
    if((rxbuffer[0] != DW_FRAME_HBYTE) || 
       (rxbuffer[1] != DW_FRAME_LBYTE) || 
       (rxbuffer[2] != length + 3    ) || 
       (rxbuffer[3] != DW_REG_R_CMD  ) || 
       (rxbuffer[4] != addr          ) ||
       (rxbuffer[5] != length        ))
    {
        dw_port_serial_resource_release();
        return false;
    }
    
    for(index = 0; index < length; index++)
    {
        buffer[index] = rxbuffer[index+6];
    }
    
    /* �ͷŴ�����Դ */
    dw_port_serial_resource_release();
    
    return true;
}

/* д�Ĵ��� */
static bool dw_write_reg(uint8_t addr, uint8_t *buffer, uint8_t length)
{
    uint8_t index = 0;
    static uint8_t txbuffer[256] = 
    {
        DW_FRAME_HBYTE,     /* ͨ��֡ͷ���ֽ� offset 0 */
        DW_FRAME_LBYTE,     /* ͨ��֡ͷ���ֽ� offset 1 */
        0,                  /* ͨ�����ݳ���   offset 2 */
        DW_REG_W_CMD,       /* д�Ĵ���ָ��   offset 3 */
        0                   /* д�Ĵ�����ַ   offset 4 */
    };
    
    /* �������, ���д�׶�ʹ��, ��д��ɺ����ɾ�� */
    if((buffer == NULL) || (length == 0))
    {
        return false;
    }
    
    /* �޸�rxbuffer */
    txbuffer[2] = length+2;
    txbuffer[4] = addr;
    
    for(index = 0; index < length; index++)
    {
        txbuffer[index+5] = buffer[index];
    }
    
    /* ��ȡ������Դ */
    dw_port_serial_resource_take();
    
    /* д�Ĵ��� */
    for(index = 0; index < (length + 5); index++)
    {
        if(dw_port_serial_putc((char)txbuffer[index]) == false)
        {
            dw_port_serial_resource_release();
            return false;
        }
    }
    
    /* �ͷŴ�����Դ */
    dw_port_serial_resource_release();
    
    return true;
}

/* ����api�ӿ� */

/* ��ȡdw��ʾ��Ӳ���汾��, ��ĻӲ��ֻ�ṩvx.x��ʽ�汾�� */
bool dw_hard_version(uint8_t *version, uint8_t *subversion) 
{
    bool ret = false;
    uint8_t buffer[1] = {0};
    
    ret = dw_read_reg(DW_REG_ADDR_HARD_VERSION, buffer, 1);
    if(ret == false)
    {
        return false;
    }
    
    *version    = dw_bcd2int(buffer[0])/10;
    *subversion = dw_bcd2int(buffer[0])%10;
    
    return true;
}

/* ��ȡdwlib��汾�� */
bool dw_soft_version(uint8_t *version, uint8_t *subversion, uint8_t *revision)
{
    *version    = DW_VERSION;
    *subversion = DW_SUBVERSION;
    *revision   = DW_REVISION;
    
    return true;
}

/* ��ȡ�������� */
bool dw_get_backlight(uint8_t *level)
{
    bool ret = false;
    uint8_t buffer[1] = {0};
    
    ret = dw_read_reg(DW_REG_ADDR_BACKLIGHT, buffer, 1);
    if(ret == false)
    {
        return false;
    }
    
    *level = buffer[0];
    
    return true;
}


/* ���ñ������� */
bool dw_set_backlight(uint8_t level)
{
    bool ret = false;
    
    /* �޶�������� */
    if(level > DW_BACKLIGHT_MAX)
    {
        level = DW_BACKLIGHT_MAX;
    }
    
    ret = dw_write_reg(DW_REG_ADDR_BACKLIGHT, &level, 1);
    if(ret == false)
    {
        return false;
    }
    
    return true;
}

/* ����time*10(ms) */
bool dw_beep_10ms(uint8_t time)
{
    bool ret = false;
    
    ret = dw_write_reg(DW_REG_ADDR_BEEP, &time, 1);
    if(ret == false)
    {
        return false;
    }
    
    return true;
}

/* ��ȡ��ǰҳID */
bool dw_get_now_pageid(uint16_t *id) 
{
    bool ret = false;
    uint8_t buffer[2] = {0};
    
    ret = dw_read_reg(DW_REG_ADDR_PAGEID, buffer, 2);
    if(ret == false)
    {
        return false;
    }
    
    *id = (uint16_t)((buffer[0] << 8) + (buffer[1]));
    
    return true;
}

/* ���õ�ǰҳID */
bool dw_set_now_pageid(uint16_t id) 
{
    bool ret = false;
    uint8_t buffer[2] = {0};
    
    buffer[0] = (id >> 8);
    buffer[1] = (id << 8) >> 8;
    
    ret = dw_write_reg(DW_REG_ADDR_PAGEID, buffer, 2);
    if(ret == false)
    {
        return false;
    }
    
    return true;
}

/* ������Ļ���� */
bool dw_set_screen_dir(uint16_t dir)
{
    bool ret = false;
    uint8_t buffer[1] = {0};
    
    /* ת���Ƕ� */
    dir %= 360;
    dir /= 90;
    
    /* ��ȡR2ֵ */
    ret = dw_read_reg(DW_REG_ADDR_R2, buffer, 1);
    if(ret == false)
    {
        return false;
    }
    
    /* ����R2���� */
    switch(dir)
    {
        case 0: DW_BITS_CLR(buffer[0], DW_BITx(7)); DW_BITS_CLR(buffer[0], DW_BITx(8)); break;
        case 1: DW_BITS_SET(buffer[0], DW_BITx(7)); DW_BITS_SET(buffer[0], DW_BITx(8)); break;
        case 2: DW_BITS_SET(buffer[0], DW_BITx(7)); DW_BITS_CLR(buffer[0], DW_BITx(8)); break;
        case 3: DW_BITS_CLR(buffer[0], DW_BITx(7)); DW_BITS_SET(buffer[0], DW_BITx(8)); break;
    }
    
    ret = dw_write_reg(DW_REG_ADDR_R2, buffer, 1);
    if(ret == false)
    {
        return false;
    }
    
    return true;
}
