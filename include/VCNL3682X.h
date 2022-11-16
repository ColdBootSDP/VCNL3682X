/**
 * \file VCNL3682X.h
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
#include <stddef.h>

/// @brief  The Addressable Registers accessable from I2C
typedef enum {

    PS_CONF1    = 0x00,  ///< Proximity Sensor First Configurartion Register
    PS_CONF2    = 0x03,  ///< Proximity Sensor Second Configuration Register
    PS_CONF3    = 0x04,  ///< Proximity Sensor Third Configuration Register
    PS_THDL     = 0x05,  ///< Proximity Low Threshold Register for Int
    PS_THDH     = 0x06,  ///< Proximity High Threshold Register for Int
    PS_CANC     = 0x07,  ///< The Proximity Sensor Sunlight Cancellation Value Register
    PS_CONF4    = 0x08,  ///< The Fourth Proximity Sensor Configuration Register, see docs 
    PS_DATA     = 0xF8,  ///< The Proximity Sensor Data Register
    INT_FLAG    = 0xF9,  ///< Interrupt Flags Register
    ID          = 0xFA,  ///< Device ID Register
    PS_AC_DATA  = 0xFB   ///< Proximity Sensor Auto Calibration Data

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

    VCNL36825,  ///< We are using the VCNL36825

} VCNLType;

/// @brief The Basic Configuration, the Adapter for the Driver
typedef struct VCNLCONFIG {

    uint16_t (*read_reg)(const uint8_t s_addr, const uint8_t reg, void* const data, const uint16_t size);
    uint16_t (*write_reg)(const uint8_t s_addr, const uint8_t reg, const void* const data, const uint16_t size);

    VCNLType type;   ///< Which type of Device we are using
    

} VCNLConfig;

/// @brief The Flags that can be set by the device, aka interrupt parameters
typedef enum VCNLFLAGS {

    PS_IF_AWAY  = (1 << 8),       ///< If the Upper threshold has been passed and Interrupt has been triggered
    PS_IF_CLOSE = (1 << 9),       ///< If the Lower threshold has been passed and the Interrupt has been triggered
    PS_ACFLAG   = (1 << 13),      ///< Auto Calibration Complete
    PS_SPFLAG   = (1 << 12)       ///< Protection Mode is being entered

} VCNLFlags;

/// @brief The Structure for the Device
typedef struct VCNL3682X {

    VCNLConfig config;  ///< The Configuration Structure 
    VCNLFlags flags;    ///< Flags having to do with the run time 
    uint8_t address;    ///< The I2C Slave Address of the Device, Init resolves this for you

} VCNL3682X;

/**
 * \brief Initializes a device with a given configuration 
 * 
 * \param[in] dev: The Device to Initialize
 * \param[in] config: The config for the device
 * \return VCNL3682X*: A Device Pointer For the created device 
 */
VCNL3682X* VCNLInit(VCNL3682X* const dev, const VCNLConfig* const config);

/**
 * \brief Deinitializes the device and makes the
 * 
 * \param[in] dev: Device to deinitialize
 */
void VCNLDeinit(VCNL3682X* const dev);

/**
 * \brief Turns on the device and enables it 
 * 
 * \param[in] dev
 * \returns uint16_t: How many bytes were written/read, should be 2
 */
uint16_t VCNLEnable(const VCNL3682X* const dev);

/**
 * \brief Disables a given device
 * 
 * \param[in] dev: Device to disable
 * \returns uint16_t: How many bytes were written/read, should be 2
 */
uint16_t VCNLDisable(const VCNL3682X* const dev);

/**
 * \brief Starts a device proximity sensing
 * 
 * \param[in] dev: Device to start
 * \returns uint16_t: How many bytes were written/read, should be 2
 */
uint16_t VCNLStart(const VCNL3682X* const dev);

/**
 * \brief Stops a device getting proximity 
 * 
 * \param[in] dev: Device to stop
 * \returns uint16_t: How many bytes were written/read, should be 2
 */
uint16_t VCNLStop(const VCNL3682X* const dev);

/**
 * \brief Writes to a device and its 22c accessible registers 
 * 
 * \param[in] dev: Device to Write to
 * \param[in] command: Address/Register to Write to
 * \param[in] value: Value to write at that register/address
 * \returns uint16_t: How many bytes were written/read, should be 2
 */
uint16_t VCNLWrite(const VCNL3682X* const dev, const VCNLCommand command, const uint16_t value);

/**
 * \brief Sets the Low Threshold for the Interrupt
 * 
 * \param[in] dev: Device to set the Threshold for
 * \param[in] thresh: Threshold to set
 * \returns uint16_t: How many bytes were written/read, should be 2
 */
uint16_t VCNLSetLowThresh(const VCNL3682X* const dev, const uint16_t thresh);

/**
 * \brief Sets the Cancellation for the Sensor, Subtracts this value from the result for denoising
 * 
 * \param[in] dev: Device to set the Cancellation for
 * \param[in] canc: The Value to set
 * \returns uint16_t: How many bytes were read/written, should be 2
 */
uint16_t VCNLSetCancellation(const VCNL3682X* const dev, const uint16_t canc);

/**
 * \brief Sets the High Threshold for the Interrupt for a device
 * 
 * \param[in] dev: Device to set the Threshold of
 * \param[in] thresh: Threshold to set
 * \returns uint16_t: How Many Bytes were written and read, should be 2
 */
uint16_t VCNLSetHighThresh(const VCNL3682X* const dev, const uint16_t thresh);

/**
 * \brief Reads 2 Bytes from the Device from an Address
 * 
 * \param[in] dev: Device to Read from
 * \param[in] command: Address/Register to read from
 * \return uint16_t: 2 bytes from the register 
 */
uint16_t VCNLRead(const VCNL3682X* const dev, const VCNLCommand command, uint16_t* const data);

/**
 * \brief Reads the Proximity Value from the device
 * 
 * \param[in] dev: Device to Read from
 * \return uint16_t: How many bytes were read, 0 indicates a failure
 */
uint16_t VCNLReadProx(const VCNL3682X* const dev, uint16_t* const proximity);

/**
 * \brief Runs the Interrupt Service Routine, updates the local flags and such
 * 
 * \param[in] dev: Device to run the Interrupt Service Routine for
 */
void VCNLISR(VCNL3682X* const dev, uint16_t* const proximity);
