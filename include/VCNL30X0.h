/**
 * \file VCNL30X0.h
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-08-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/// @brief  The Addressable Registers accessable from I2C
typedef enum {

    PS_CONF     = 0x3,  ///< Proximity Sensor Config Register
    PS_CONF3_MS = 0x4,  ///< Proximity Sensor Config and Mode Selection Register
    PS_CANC     = 0x5,  ///< Proximity Cancellation Register
    PS_THDL     = 0x6,  ///< Proximity Low Threshold Register
    PS_THDH     = 0x7,  ///< Proximity High Threshold Register
    PS_DATA     = 0x8,  ///< The Proximity Sensor Data Register
    INT_FLAG    = 0xd,  ///< Interrupt Flags Register
    ID          = 0xe   ///< Device ID Register

} VCNLCommand;

/// @brief Interrupt Modes
typedef enum VNCLINT {

    INT_DISABLE      = 0x0, ///< Interrupt Disabled
    INT_CLOSING      = 0x1, ///< Interrupt By Closing
    INT_AWAY         = 0x2, ///< Trigger By Away
    INT_CLOSING_AWAY = 0x3  ///< Trigger By Closing and Away

} VCNLInt;

/// @brief The Type Of chip we are using
typedef enum VNCLTYPE {

    VCNL3030,   ///< We are using the VCNL3030
    VCNL3040    ///< We are using the VCNL3040

} VCNLType;

/// @brief The Basic Configuration, the Adapter for the Driver
typedef struct VCNLCONFIG {

    uint16_t (*read_reg)(const uint8_t s_addr, const uint8_t reg, void* const data, const uint16_t size);
    uint16_t (*write_reg)(const uint8_t s_addr, const uint8_t reg, const void* const data, const uint16_t size);

    VCNLType type; ///< Which type of Device we are using

} VCNLConfig;

/// @brief The Flags that can be set by the device, aka interrupt parameters
typedef struct VCNLFLAGS {

    bool far;    ///< If the Upper threshold has been passed and Interrupt has been triggered
    bool close;  ///< If the Lower threshold has been passed and the Interrupt has been triggered

} VCNLFlags;

/// @brief The Structure for the Device
typedef struct VCNL30X0 {

    VCNLConfig config;  ///< The Configuration Structure 
    VCNLFlags flags;    ///< Flags having to do with the run time 

} VCNL30X0;

/**
 * \brief Initializes a device with a given configuration 
 * 
 * \param[in] dev: The Device to Initialize
 * \param[in] config: The config for the device
 * \return VCNL30X0*: A Device Pointer For the created device 
 */
VCNL30X0* VCNLInit(VCNL30X0* const dev, const VCNLConfig* const config);

/**
 * \brief Deinitializes the device and makes the
 * 
 * \param[in] dev: Device to deinitialize
 */
void VCNLDeinit(VCNL30X0* const dev);

/**
 * \brief Turns on the device and enables it 
 * 
 * \param[in] dev
 */
void VCNLEnable(const VCNL30X0* const dev);

/**
 * \brief Disables a given device
 * 
 * \param[in] dev: Device to disable
 */
void VCNLDisable(const VCNL30X0* const dev);

/**
 * \brief Starts a device proximity sensing
 * 
 * \param[in] dev: Device to start
 */
void VCNLStart(const VCNL30X0* const dev);

/**
 * \brief Stops a device getting proximity 
 * 
 * \param[in] dev: Device to stop
 */
void VCNLStop(const VCNL30X0* const dev);


/**
 * \brief Writes to a device and its 22c accessible registers 
 * 
 * \param[in] dev: Device to Write to
 * \param[in] command: Address/Register to Write to
 * \param[in] value: Value to write at that register/address
 */
void VCNLWrite(const VCNL30X0* const dev, const VCNLCommand command, const uint16_t value);

/**
 * \brief Sets the Current for the IR LED
 * 
 * \param[in] dev: Device to set the IR Current on
 * \param[in] mA: The Current in Milliamps
 */
void VCNLSetIRCurrent(const VCNL30X0* const dev, const uint8_t mA);

/**
 * \brief Sets the Low Threshold for the Interrupt
 * 
 * \param[in] dev: Device to set the Threshold for
 * \param[in] thresh: Threshold to set
 */
void VCNLSetLowThresh(const VCNL30X0* const dev, const uint16_t thresh);

/**
 * \brief Sets the Cancellation for the Sensor, Subtracts this value from the result for denoising
 * 
 * \param[in] dev: Device to set the Cancellation for
 * \param[in] canc: The Value to set
 */
void VCNLSetCancellation(const VCNL30X0* const dev, const uint16_t canc);

/**
 * \brief Sets the High Threshold for the Interrupt for a device
 * 
 * \param[in] dev: Device to set the Threshold of
 * \param[in] thresh: Threshold to set
 */
void VCNLSetHighThresh(const VCNL30X0* const dev, const uint16_t thresh);

/**
 * \brief Reads 2 Bytes from the Device from an Address
 * 
 * \param[in] dev: Device to Read from
 * \param[in] command: Address/Register to read from
 * \return uint16_t: 2 bytes from the register 
 */
uint16_t VCNLRead(const VCNL30X0* const dev, const VCNLCommand command);

/**
 * \brief Reads the Proximity Value from the device
 * 
 * \param[in] dev: Device to Read from
 * \return uint16_t: The Proximity Value
 */
uint16_t VCNLReadProx(const VCNL30X0* const dev);

/**
 * \brief Runs the Interrupt Service Routine, updates the local flags and such
 * 
 * \param[in] dev: Device to run the Interrupt Service Routine for
 */
void VCNLISR(const VCNL30X0* const dev, uint16_t* const proximity);
