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

#include <string.h>
#include <assert.h>

VCNL30X0* VCNLInit(VCNL30X0* const dev, const VCNLConfig* const config) {

    assert(config && config->read_reg && config->write_reg);

    memcpy(&dev->config, config, sizeof(VCNLConfig));
    VCNLEnable(dev);

    return dev;

}

void VCNLWrite(const VCNL30X0* const dev, const VCNLCommand command, const uint16_t value) {

    assert(dev);

    const uint8_t address = (dev->config.type == VCNL3030? 0x41: 0x60);
    dev->config.write_reg(address, command, (uint8_t*)&value, 2);

}

void VCNLEnable(const VCNL30X0* const dev) {

    assert(dev);

    static uint16_t data;

    data = VCNLRead(dev, PS_CONF) & ~0x100;
    VCNLWrite(dev, PS_CONF, data);

}

void VCNLDisable(const VCNL30X0* const dev) {

    static uint16_t data;

    data = VCNLRead(dev, PS_CONF) | 0x100;
    VCNLWrite(dev, PS_CONF, data);

}

uint16_t VCNLRead(const VCNL30X0* const dev, const VCNLCommand command) {

    assert(dev);

    static uint16_t value;
    const uint8_t address = (dev->config.type == VCNL3030? 0x41: 0x60);
    dev->config.read_reg(address, command, (uint8_t*)&value, 2);

    return value;    

}

void VCNLStart(const VCNL30X0* const dev) {

    static uint16_t value;

    value = VCNLRead(dev, PS_CONF) & ~1; // clear the start bit, starting the device
    VCNLWrite(dev, PS_CONF, value); // rewrite the value back in

}

void VCNLStop(const VCNL30X0* const dev) {

    static uint16_t value;

    value = VCNLRead(dev, PS_CONF) | 1;
    VCNLWrite(dev, PS_CONF, value);

}

void VCNLDeinit(VCNL30X0* const dev) {

    VCNLStop(dev);
    VCNLDisable(dev);

    dev->config.read_reg = NULL;
    dev->config.write_reg = NULL;

}

void VCNLISR(const VCNL30X0* const dev)  {

    assert(dev && dev->config.read_reg && dev->config.write_reg);

    static uint16_t int_data;

    int_data = VCNLRead(dev, INT_FLAG);

}
