
/** @file MAX1247_H
  *  @brief
  *
  *  @author Matt Casari, matthew.casari@noaa.org
  *  @date 
  *  @version
  *
  *  @copyright National Oceanic and Atmospheric Administration
  *  @copyright Pacific Marine Environmental Lab
  *  @copyright Environmental Development Division
  *
  *	@note
  *
  *  @bug  No known bugs
  */
#ifndef MAX1247_H
#define MAX1247_H
/************************************************************************
*						STANDARD LIBRARIES
************************************************************************/
#include "spi.h";

/************************************************************************
*							CONSTANTS
************************************************************************/
/* Following constants assume Unipolar, sing-ended conversions with internal clock */
const unit8_t MAX1247_CHAN0_START = 0x9E;
const uint8_t MAX1247_CHAN1_START = 0xDE;
const uint8_t MAX1247_CHAN2_START = 0xAE;
const uint8_t MAX1247_CHAN3_START = 0xEE;
const uint8_t MAX1247_FAST_SHUTDOWN = 0x81;
const uint8_t MAX1247_FULL_SHUTDOWN = 0x80;
const uint8_t MAX1247_DUMMY = 0x00;
/************************************************************************
*							MACROS
************************************************************************/
#define MAX1247_CHAN0   (0)
#define MAX1247_CHAN1   (1)
#define MAX1247_CHAN2   (2)
#define MAX1247_CHAN3   (3)

#define MAX1247_VALID   (0)
#define MAX1247_INVALID (1)

#define MAX1247_NUM_CHANNELS    (4)

#define MAX1247_TIMEOUT_VALUE   (10)            /* Milli-seconds */

#define MAX1247_SHDN_PIN_LOW()      (ADC_SHDN_PIN_LOW())
#define MAX1247_SHDN_PIN_INIT()     (ADC_SHDN_PIN_INIT())


#define MAX1247_CS_INIT()           (ADC_CS_INIT())
#define MAX1247_CS_CLEAR()          (ADC_CS_CLEAR())
#define MAX1247_CS_SET()            (ADC_CS_SET())

#define MAX1247_STRB_INIT()         (ADC_STRB_INIT())
#define MAX1247_STRB_CLEAR()        (ADC_STRB_CLEAR())

  
/************************************************************************
*							STRUCTURES
************************************************************************/
/** @brief 
 * 
 * 
 *
 */
typedef struct MAX1247 {
  MAX1247_State_t state;
  MAX1247_Channel_t chan;
  uint8_t strobe;
}
/************************************************************************
*							ENUMERATIONS
************************************************************************/
/** @brief System State Enumeration
 *
 *  
 */
typdef enum MAX1247_State {
  MAX_Idle,
  MAX_Read,
  MAX_Write,
  MAX_Conversion
}MAX1247_State_t;


typedef enum MAX1247_Channel {
  MAX_Channel0,
  MAX_Channel1,
  MAX_Channel2,
  MAX_Channel3
}MAX1247_Channel_t;
/************************************************************************
*					GLOBAL FUNCTION PROTOTYPES
************************************************************************/
/** @brief Initialize the MAX1247 Chip
 *
 *	Starts the SPI bus and Peripheral I/O required for operation
 *
 *  @param None
 *
 *  @return Status
 */
 uint8_t MAX1247_Init(void);   
    
 /** @brief Reads data from the MAX1247
 *
 *	Reads a 16-bit value from the sensor and converts to the actual 12-bit value
 *
 *  @param *value Pointer to the return value save location
 *
 *  @return Status
 */
 uint8_t MAX1247_Read(uint16_t *value);    
 
 /** @brief Start Conversion on selected Channel
 *
 *	Start ADC conversion on the selected channel.
 *
 *  @param None
 *
 *  @return Status
 */
 uint8_t MAX1247_Start(uint8_t channel); 
    
    #endif // MAX1247_H
