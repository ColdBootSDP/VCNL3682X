/**
 * \file VCNL3036.c
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-08-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../include/VCNL30X0.h"

VCNL30X0* VCNLInit(VCNL30X0* const dev, const VCNLConfig* const config) {

    //assert(config && config->read_reg && config->write_reg);
    if(config->read_reg == NULL || config->write_reg == NULL)
        return NULL;

    dev->config.read_reg = config->read_reg;
    dev->config.write_reg = config->write_reg;
    dev->config.type = config->type;
    dev->flags = (VCNLFlags){false, false};

    dev->address = (dev->config.type == VCNL3030? 0x41: 0x13);

    return dev;

}

uint16_t VCNLWrite(const VCNL30X0* const dev, const VCNLCommand command, const uint16_t value) {

    uint8_t data[2] = { (uint8_t)(value & 0xff), (uint8_t)(value >> 8) };

    return dev->config.write_reg(dev->address, command, data, 2);

}

uint16_t VCNLEnable(const VCNL30X0* const dev) {

    static uint16_t data;

    if(VCNLRead(dev, PS_CONF, &data) != 2)
        return 0;
    
    data &= ~0x1; // clear the ps_sd bit taking the chip into power on mode
    
    return VCNLWrite(dev, PS_CONF, data);

}

uint16_t VCNLDisable(const VCNL30X0* const dev) {

    static uint16_t data;

    if(VCNLRead(dev, PS_CONF, &data) != 2)
        return 0;

    data |= 0x1; // set the ps_sd bit

    return VCNLWrite(dev, PS_CONF, data); 

}

uint16_t VCNLRead(const VCNL30X0* const dev, const VCNLCommand command, uint16_t* const data) {

    if(dev == NULL)
        return 0;

    static uint8_t value[2];
    uint16_t bytesread = dev->config.read_reg(dev->address, command, value, 2);
    *data = value[0] + value[1] << 8;

    return bytesread;    

}

uint16_t VCNLReadProx(const VCNL30X0* const dev, uint16_t* const proximity) { return VCNLRead(dev, PS_DATA, proximity); }

uint16_t VCNLStart(const VCNL30X0* const dev) {

    static uint16_t value;

    if(VCNLRead(dev, PS_CONF3_MS, &value) != 2)
        return 0;
        
    value &= ~1; 
    
    return VCNLWrite(dev, PS_CONF, value);
}

uint16_t VCNLStop(const VCNL30X0* const dev) {

    static uint16_t value;

    if(VCNLRead(dev, PS_CONF, &value) != 2)
        return 0;

    value |= 1;
    
    return VCNLWrite(dev, PS_CONF, value);

}

void VCNLDeinit(VCNL30X0* const dev) {

    VCNLStop(dev);
    VCNLDisable(dev);

    dev->config.read_reg = NULL;
    dev->config.write_reg = NULL;

}

void VCNLISR(const VCNL30X0* const dev, uint16_t* const proximity) {

    //assert(dev && dev->config.read_reg && dev->config.write_reg);

    static uint16_t int_data;

    VCNLRead(dev, INT_FLAG, &int_data);

}
