
#ifndef VL53L4CD_API_H_
#define VL53L4CD_API_H_

#include <gpiod.h>
#include <thread>
#include <vector>

/**
 *  @brief Driver version
 */

#define VL53L4CD_IMPLEMENTATION_VER_MAJOR       2
#define VL53L4CD_IMPLEMENTATION_VER_MINOR       2
#define VL53L4CD_IMPLEMENTATION_VER_BUILD       2
#define VL53L4CD_IMPLEMENTATION_VER_REVISION  	0

/**
 *  @brief Driver error type
 */

typedef uint8_t VL53L4CD_Error;

#define VL53L4CD_ERROR_NONE					((uint8_t)0U)
#define VL53L4CD_ERROR_XTALK_FAILED			((uint8_t)253U)
#define VL53L4CD_ERROR_INVALID_ARGUMENT		((uint8_t)254U)
#define VL53L4CD_ERROR_TIMEOUT				((uint8_t)255U)

#define DEFAULT_VL53L4CD_ADDRESS 0x29

/**
 *  @brief Inner Macro for API. Not for user, only for development.
 */

#define VL53L4CD_SOFT_RESET     ((uint16_t)0x0000))
#define VL53L4CD_I2C_SLAVE__DEVICE_ADDRESS      ((uint16_t)0x0001)
#define VL53L4CD_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND  ((uint16_t)0x0008)
#define VL53L4CD_XTALK_PLANE_OFFSET_KCPS ((uint16_t)0x0016)
#define VL53L4CD_XTALK_X_PLANE_GRADIENT_KCPS     ((uint16_t)0x0018)
#define VL53L4CD_XTALK_Y_PLANE_GRADIENT_KCPS     ((uint16_t)0x001A)
#define VL53L4CD_RANGE_OFFSET_MM     ((uint16_t)0x001E)
#define VL53L4CD_INNER_OFFSET_MM     ((uint16_t)0x0020)
#define VL53L4CD_OUTER_OFFSET_MM     ((uint16_t)0x0022)
#define VL53L4CD_GPIO_HV_MUX__CTRL      ((uint16_t)0x0030)
#define VL53L4CD_GPIO__TIO_HV_STATUS    ((uint16_t)0x0031)
#define VL53L4CD_SYSTEM__INTERRUPT  ((uint16_t)0x0046)
#define VL53L4CD_RANGE_CONFIG_A     ((uint16_t)0x005E)
#define VL53L4CD_RANGE_CONFIG_B      ((uint16_t)0x0061)
#define VL53L4CD_RANGE_CONFIG__SIGMA_THRESH     ((uint16_t)0x0064)
#define VL53L4CD_MIN_COUNT_RATE_RTN_LIMIT_MCPS    ((uint16_t)0x0066)
#define VL53L4CD_INTERMEASUREMENT_MS ((uint16_t)0x006C)
#define VL53L4CD_THRESH_HIGH    ((uint16_t)0x0072)
#define VL53L4CD_THRESH_LOW     ((uint16_t)0x0074)
#define VL53L4CD_SYSTEM__INTERRUPT_CLEAR        ((uint16_t)0x0086)
#define VL53L4CD_SYSTEM_START     ((uint16_t)0x0087)
#define VL53L4CD_RESULT__RANGE_STATUS   ((uint16_t)0x0089)
#define VL53L4CD_RESULT__SPAD_NB   ((uint16_t)0x008C)
#define VL53L4CD_RESULT__SIGNAL_RATE   ((uint16_t)0x008E)
#define VL53L4CD_RESULT__AMBIENT_RATE   ((uint16_t)0x0090)
#define VL53L4CD_RESULT__SIGMA   ((uint16_t)0x0092)
#define VL53L4CD_RESULT__DISTANCE   ((uint16_t)0x0096)


#define VL53L4CD_RESULT__OSC_CALIBRATE_VAL      ((uint16_t)0x00DE)
#define VL53L4CD_FIRMWARE__SYSTEM_STATUS        ((uint16_t)0x00E5)
#define VL53L4CD_IDENTIFICATION__MODEL_ID       ((uint16_t)0x010F)

//defining the default chip and interrupt pins
#define drdy_chip 4
#define Default_interrupt_pin 14;

/**
 *  @brief defines Software Version
 */

typedef struct {
	uint8_t      major;    /*!< major number */
	uint8_t      minor;    /*!< minor number */
	uint8_t      build;    /*!< build number */
	uint32_t     revision; /*!< revision number */
} VL53L4CD_Version_t;

/**
 *  @brief Packed reading results type
 */

typedef struct {

	/* Status of measurements. If the status is equal to 0, the data are valid*/
	uint8_t range_status;
	/* Measured distance in millimeters */
	uint16_t distance_mm;
	/* Ambient noise in kcps */
	uint16_t ambient_rate_kcps;
	/* Ambient noise in kcps/SPAD */
	uint16_t ambient_per_spad_kcps;
	/* Measured signal of the target in kcps */
	uint16_t signal_rate_kcps;
	/* Measured signal of the target in kcps/SPAD */
	uint16_t signal_per_spad_kcps;
	/* Number of SPADs enabled */
	uint16_t number_of_spad;
	/* Estimated measurements std deviation in mm */
	uint16_t sigma_mm;
} VL53L4CD_ResultsData_t;

/**
 * @brief This function programs the software driver version.
 * @param (VL53L4CD_Version_t) pVersion : Pointer of structure, containing the
 * software version.
 * @return (VL53L4CD_ERROR) status : 0 if SW version is OK.
 */

//adding cpp class 
class VL53L4CD_API{

	
	public:

		//function to boot the sensor
		void boot_sensor(uint8_t address, int interrupt_pin);

		//function to set the sensor to stop recording data internally 
		void stop_sensor_ranging();

		//function to begin recording data
		void start_recording_data();

		//function to stop recording data through the I2C
		void stop_recording_data();

		//virtual void function for use in callback
		struct VL53L4CD_Callback_Interface {
			virtual void hasVL53L4CDSample(uint16_t sample) = 0;
		};

		void registerCallback(VL53L4CD_Callback_Interface* ci) {
			VL53L4CDcallbackinterface.push_back(ci);
		};

		//function to change I2C_Address
		void VL53L4CD_SetI2CAddress(uint8_t new_address);


	private: 

		bool running = false; 

		//defining a default pin for interrupt 
		struct gpiod_line *pinDRDY = nullptr;
		struct gpiod_chip *chipDRDY = nullptr;

		std::thread thr;

		int i2c_read_conversion(uint8_t reg);

		std::vector<VL53L4CD_Callback_Interface*> VL53L4CDcallbackinterface;

		int interrupt_pin;
		int chip;
		int fd_i2c = -1;


		//defining the address of the sensor 
		uint8_t address = DEFAULT_VL53L4CD_ADDRESS;


		//function to write a 2 byte long word to a register 
		uint8_t I2C_WrWord(uint16_t reg,uint16_t value);

		//function to write a single byte to a register
		uint8_t I2C_WrByte(uint16_t reg, uint8_t value);

		//function to write a 32 bit number to a 16 bit register
		uint8_t I2C_Wr_four_bytes(uint16_t reg, uint32_t value);

		int i2c_read_Byte(uint16_t reg);

		//function to read data when data ready pin is high
		void DataReady();

		//function using blocking IO to wait for the interrupt pin before reading data in real-time
		void worker();

		//function to set range timing 
		void VL53L4CD_SetRangeTiming_RealTime(uint32_t timing_budget_ms,
			uint32_t inter_measurement_ms);



};
#endif  //VL53L4CD_API_H_
