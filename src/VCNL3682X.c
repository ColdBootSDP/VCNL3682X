/**
 * \file VCNL3682X.c
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-08-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../include/VCNL3682X.h"

VCNL3682X* VCNLInit(VCNL3682X* const dev, const VCNLConfig* const config) {

    //assert(config && config->read_reg && config->write_reg);
    if(config->read_reg == NULL || config->write_reg == NULL)
        return NULL;

    dev->config = *config;
    dev->flags = 0;

    dev->address = (config->type == VCNL36825? 0x60: 0x13);

    return dev;

}

uint16_t VCNLWrite(const VCNL3682X* const dev, const VCNLCommand command, const uint16_t value) {

    //uint8_t data[2] = { (uint8_t)(value & 0xff), (uint8_t)(value >> 8) };

    return dev->config.write_reg(dev->address, command, &value, 2);

}

uint16_t VCNLEnable(const VCNL3682X* const dev) {

    static uint16_t data;

    if(VCNLRead(dev, PS_CONF1, &data) != 2)
        return 0;
    
    data |= (1 << 1); // clear the ps_sd bit taking the chip into power on mode
    
    if (VCNLWrite(dev, PS_CONF1, data) != 2)
        return 0;

    data |= (1 << 7) | (1 << 9);

    return VCNLWrite(dev, PS_CONF1, data);

}

uint16_t VCNLDisable(const VCNL3682X* const dev) {

    static uint16_t data;

    if(VCNLRead(dev, PS_CONF1, &data) != 2)
        return 0;

    data &= ~((1 << 1) | (1 << 7) | (1 << 9)); // reset the ps_on bit

    return VCNLWrite(dev, PS_CONF1, data); 

}

uint16_t VCNLRead(const VCNL3682X* const dev, const VCNLCommand command, uint16_t* const data) {

    static uint8_t value[2];
    uint16_t bytesread = dev->config.read_reg(dev->address, command, value, 2);

    *data = (value[0]) | (((uint16_t)value[1]) << 8); // make sure we are endian independent

    return bytesread;    

}

uint16_t VCNLReadProx(const VCNL3682X* const dev, uint16_t* const proximity) { return VCNLRead(dev, PS_DATA, proximity); }

uint16_t VCNLStart(const VCNL3682X* const dev) {

    static uint16_t value;  

    if(VCNLRead(dev, PS_CONF2, &value) != 2)
        return 0;
        
    value &= ~(1 << 0); 
    
    return VCNLWrite(dev, PS_CONF2, value);
}

uint16_t VCNLStop(const VCNL3682X* const dev) {

    static uint16_t value;

    if(VCNLRead(dev, PS_CONF2, &value) != 2)
        return 0;

    value |= 1;
    
    return VCNLWrite(dev, PS_CONF2, value);

}

void VCNLDeinit(VCNL3682X* const dev) {

    VCNLStop(dev);
    VCNLDisable(dev);

    dev->config.read_reg = NULL;
    dev->config.write_reg = NULL;

}

void VCNLISR(VCNL3682X* const dev) {

    VCNLRead(dev, INT_FLAG, &dev->flags);

}
