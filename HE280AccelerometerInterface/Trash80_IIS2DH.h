#ifndef Trash80_IIS2DH_H
#define Trash80_IIS2DH_H

#include <Arduino.h>
#include <Wire.h>


#define IIS2DH_I2C_ADDR 0x19
#define IIS2DH_WHO_AM_I_VALUE 0x33

#define IIS2DH_N 0
#define IIS2DH_R 1
#define IIS2DH_W 2

#define IIS2DH_X 0
#define IIS2DH_Y 1
#define IIS2DH_Z 2
#define IIS2DH_ALL 4

#define IIS2DH_NUMBER_REGISTERS 64

#define IIS2DH_REG_STATUS_AUX 0x07
#define IIS2DH_OUT_TEMP_L 0x0C
#define IIS2DH_OUT_TEMP_H 0x0D
#define IIS2DH_INT_COUNTER_REG 0x0E
#define IIS2DH_WHO_AM_I 0x0F
#define IIS2DH_TEMP_CFG_REG 0x1F
#define IIS2DH_CTRL_REG1 0x20
#define IIS2DH_CTRL_REG2 0x21
#define IIS2DH_CTRL_REG3 0x22
#define IIS2DH_CTRL_REG4 0x23
#define IIS2DH_CTRL_REG5 0x24
#define IIS2DH_CTRL_REG6 0x25
#define IIS2DH_REFERENCE 0x26
#define IIS2DH_DATACAPTURE 0x26
#define IIS2DH_STATUS_REG 0x27
#define IIS2DH_OUT_X_L 0x28
#define IIS2DH_OUT_X_H 0x29
#define IIS2DH_OUT_Y_L 0x2A
#define IIS2DH_OUT_Y_H 0x2B
#define IIS2DH_OUT_Z_L 0x2C
#define IIS2DH_OUT_Z_H 0x2D
#define IIS2DH_FIFO_CTRL_REG 0x2E
#define IIS2DH_FIFO_SRC_REG 0x2F
#define IIS2DH_INT1_CFG 0x30
#define IIS2DH_INT1_SRC 0x31
#define IIS2DH_INT1_THS 0x32
#define IIS2DH_INT1_DURATION 0x33
#define IIS2DH_INT2_CFG 0x34
#define IIS2DH_INT2_SRC 0x35
#define IIS2DH_INT2_THS 0x36
#define IIS2DH_INT2_DURATION 0x37
#define IIS2DH_CLICK_CFG 0x38
#define IIS2DH_CLICK_SRC 0x39
#define IIS2DH_CLICK_THS 0x3A
#define IIS2DH_TIME_LIMIT 0x3B
#define IIS2DH_TIME_LATENCY 0x3C
#define IIS2DH_TIME_WINDOW 0x3D
#define IIS2DH_ACT_THS 0x3E
#define IIS2DH_ACT_DUR 0x3F

// Mapping reading and writing available registers to prevent writing to bad addresses (prevent IC corruption)
const uint8_t IIS2DH_REGISTER_RW_MAP[IIS2DH_NUMBER_REGISTERS] = {
    IIS2DH_N, // 0x00
    IIS2DH_N, // 0x01
    IIS2DH_N, // 0x02
    IIS2DH_N, // 0x03
    IIS2DH_N, // 0x04
    IIS2DH_N, // 0x05
    IIS2DH_N, // 0x06
    IIS2DH_R, // 0x07
    IIS2DH_N, // 0x08
    IIS2DH_N, // 0x09
    IIS2DH_N, // 0x0A
    IIS2DH_N, // 0x0B
    IIS2DH_R, // 0x0C
    IIS2DH_R, // 0x0D
    IIS2DH_R, // 0x0E
    IIS2DH_R, // 0x0F
    IIS2DH_N, // 0x10
    IIS2DH_N, // 0x11
    IIS2DH_N, // 0x12
    IIS2DH_N, // 0x13
    IIS2DH_N, // 0x14
    IIS2DH_N, // 0x15
    IIS2DH_N, // 0x16
    IIS2DH_N, // 0x17
    IIS2DH_N, // 0x18
    IIS2DH_N, // 0x19
    IIS2DH_N, // 0x1A
    IIS2DH_N, // 0x1B
    IIS2DH_N, // 0x1C
    IIS2DH_N, // 0x1D
    IIS2DH_N, // 0x1E
    IIS2DH_W, // 0x1F
    IIS2DH_W, // 0x20
    IIS2DH_W, // 0x21
    IIS2DH_W, // 0x22
    IIS2DH_W, // 0x23
    IIS2DH_W, // 0x24
    IIS2DH_W, // 0x25
    IIS2DH_W, // 0x26
    IIS2DH_R, // 0x27
    IIS2DH_R, // 0x28
    IIS2DH_R, // 0x29
    IIS2DH_R, // 0x2A
    IIS2DH_R, // 0x2B
    IIS2DH_R, // 0x2C
    IIS2DH_R, // 0x2D
    IIS2DH_W, // 0x2E
    IIS2DH_R, // 0x2F
    IIS2DH_W, // 0x30
    IIS2DH_R, // 0x31
    IIS2DH_W, // 0x32
    IIS2DH_W, // 0x33
    IIS2DH_W, // 0x34
    IIS2DH_R, // 0x35
    IIS2DH_W, // 0x36
    IIS2DH_W, // 0x37
    IIS2DH_W, // 0x38
    IIS2DH_R, // 0x39
    IIS2DH_W, // 0x3A
    IIS2DH_W, // 0x3B
    IIS2DH_W, // 0x3C
    IIS2DH_W, // 0x3D
    IIS2DH_W, // 0x3E
    IIS2DH_W, // 0x3F
};

class Trash80_IIS2DH {
    public:
        Trash80_IIS2DH() {};
        /**
         * Init
         * Enables Wire library for IC
         * @param fastMode boolean enable 400khz i2c
         */
        void init(bool fastMode);
        void init(void) { init(false); };

        /**
         * Is Connectected?
         * @return connected boolean
         * Performs a read by requesting a WHO_AM_I from IC, returns false if not connected or WHO_AM_I does not match expected result
         */
        bool connected(void);

        /**
         * Get the internal counter value
         * @return value integer internal counter
         */
        uint8_t getInternalCounter(void);

        /**
         * Get Who Am I register. Should return 0x33
         * @return value integer (0x33)
         */
        uint8_t getWhoAmI(void);

        /**
         * Self-test enable
         * @param test integer 0 to 3
         * 0: Normal, 1: Self Test 0, 2: Self Test 1
         */
        void setSelfTestMode(uint8_t test);

        /**
         * Enable SPI 3 wire interface
         * @param wire boolean
         * 0: 4 wire, 1: 3 wire
         */
        void enableSpi3Wire(bool wire);

        /**
         * Reset memory to power-on settings (reboot memory content)
         */
        void resetMemory(void);

        /**
         * ODR Output Data Rate
         * @param rate integer 0 to 9
         * 0:Power-down mode, 1:1Hz, 2:10Hz, 3:25Hz, 4:50Hz, 5:100Hz, 6:200Hz, 7:400Hz, 8:1.620kHz, 9:1.344kHz Normal / 5.376 kHz Low power)
         */
        void setDataRate(uint8_t rate);

        /**
         * Low power, normal, and high resolution mode
         * @param mode integer 0 to 2
         * 0:Low-power mode (8bit resolution), 1:Normal mode (10bit), 2:High-resolution mode (12bit)
         */
        void setDataResolution(uint8_t mode);

        /**
         * Set Output Scaling
         * @param g integer 0 to 3
         * 0: ±2g, 1: ±4g, 2: ±8g, 3: ±16g
         */
        void setScale(uint8_t g);

        /**
         * Enable Temperature reading
         * @param enable boolean
         */
        void enableTemperature(bool enable);

        /**
         * Check if a new Temperature reading is available
         * @return available boolean
         */
        bool temperatureAvailable(void);

        /**
         * Get the current temperature
         * @return value integer value in Celsius
         */
        int getTemperature(void);

        /**
         * Axis Enable (X, Y, Z)
         * @param x boolean
         * @param y boolean
         * @param z boolean
         */
        void enableAxis(bool x, bool y, bool z);

        /**
         * Set Filter
         * @param mode integer 0 to 3
         * @param cutoff integer 0 to 3
         * @param filterDataOutput boolean filters the data in output buffer
         * @param enableClick boolean enable for click interrupt event
         * @param enableAndOr1 boolean enable for AndOr1 interrupt event
         * @param enableAndOr2 boolean enable for AndOr2 interrupt event
         * mode: 0:Normal mode (reset by reading REFERENCE/DATACAPTURE (26h) register), 1:Reference signal for filtering, 2: Normal mode, 3:Autoreset on interrupt event
         * cutoff  0ODR         1ODR        2ODR        3ODR       4ODR     5ODR      6ODR      7ODR       8ODR
         *    0:   0.02hz       0.2hz       0.5hz       1hz        2hz      4hz       8hz       32hz       100hz
         *    1:   0.008hz      0.08hz      0.2hz       0.5hz      1hz      2hz       4hz       16hz       50hz
         *    2:   0.004hz      0.04hz      0.1hz       0.2hz      0.5hz    1hz       2hz       8hz        25hz
         *    3:   0.002hz      0.02hz      0.05hz      0.1hz      0.2hz    0.5hz     1hz       4hz        12hz
         */
        void setFilter(uint8_t mode, uint8_t cutoff, bool filterDataOutput, bool enableClick, bool enableInt1, bool enableInt2);

        /**
         * Set INT1 And-Or interrupt mode
         * @param mode int 0: OR combination of int events, 1: 6-direction movement recognition, 2: AND combination of interrupt events, 3: 6D position recognition
         */
        void setInt1Mode(uint8_t mode);

        /**
         * Set INT1 axis trigger on direction
         * @param xDown boolean
         * @param xUp boolean
         * @param yDown boolean
         * @param yUp boolean
         * @param zDown boolean
         * @param zUp boolean
         */
        void setInt1AxisTrigger(bool xDown, bool xUp, bool yDown, bool yUp, bool zDown, bool zUp);

        /**
         * Set INT1 trigger movement threshold
         * @param threshold int 0 to 127
         * Scaling    THS Unit    MG
         *    2g          1       16mg
         *    4g          1       32mg
         *    8g          1       62mg
         *    16g         1       186mg
         */
        void setInt1Threshold(uint8_t threshold);

        /**
         * Set INT1 trigger movement duration
         * @param duration int 0 to 127 : 1 = 1/Output Data Rate
         */
        void setInt1Duration(uint8_t duration);

        /**
         * Get INT1 Status Register - Clears/Resets interrupt on read
         * @return value integer raw value from register
         */
        uint8_t int1Status(void);

        /**
         * Get INT1 axis source from status register
         * @param axis integer 0:x,1:y,2:z
         * @return value integer
         */
        uint8_t int1AxisSource(uint8_t axis);

        /**
         * Set INT1 to Latching mode
         * @param enable boolean
         */
        void setInt1Latch(bool enable);

        /**
         * Relay internal interrupts to INT1 Pin
         * @param onClick boolean
         * @param onAndOr1 boolean
         * @param onAndOr2 boolean
         * @param onDataReady1 boolean
         * @param onDataReady2 boolean
         * @param onFifoWatermark boolean
         * @param onFifoOverrun boolean
         */
        void setInt1Options(bool onClick, bool onAndOr1, bool onAndOr2, bool onDataReady1, bool onDataReady2, bool onFifoWatermark, bool onFifoOverrun);

        /**
         * Set INT2 And-Or interrupt mode
         * @param mode int 0: OR combination of int events, 1: 6-direction movement recognition, 2: AND combination of interrupt events, 3: 6D position recognition
         */
        void setInt2Mode(uint8_t mode);

        /**
         * Set INT2 axis trigger on direction
         * @param xDown boolean
         * @param xUp boolean
         * @param yDown boolean
         * @param yUp boolean
         * @param zDown boolean
         * @param zUp boolean
         */
        void setInt2AxisTrigger(bool xDown, bool xUp, bool yDown, bool yUp, bool zDown, bool zUp);

        /**
         * Set INT2 trigger movement threshold
         * @param threshold int 0 to 127
         * Scaling    THS Unit    MG
         *    2g          1       16mg
         *    4g          1       32mg
         *    8g          1       62mg
         *    16g         1       186mg
         */
        void setInt2Threshold(uint8_t threshold);

        /**
         * Set INT2 trigger movement duration
         * @param duration int 0 to 127 : 1 = 1/Output Data Rate
         */
        void setInt2Duration(uint8_t duration);

        /**
         * Get INT2 Status Register - Clears/Resets interrupt on read
         * @return value integer raw value from register
         */
        uint8_t int2Status(void);

        /**
         * Get INT2 axis source from status register
         * @param axis integer 0:x,1:y,2:z
         * @return value integer
         */
        uint8_t int2AxisSource(uint8_t axis);

        /**
         * Set INT2 to Latching mode
         * @param enable boolean
         */
        void setInt2Latch(bool enable);

        /**
         * Relay internal interrupts to INT2 Pin
         * @param onClick boolean
         * @param onAndOr1 boolean
         * @param onAndOr2 boolean
         * @param onBoot boolean
         * @param polarity boolean 0: interrupt active-high; 1: interrupt active-low
          */
        void setInt2Options(bool onClick, bool onAndOr1, bool onAndOr2, bool onBoot, bool onActivity, bool polarity);

        /**
         * Click is available/triggered
         * @return direction int 0:none, -1:up direction, 1: down direction
         */
        uint8_t clickAvailable(void);

        /**
         * Clicked axis
         * @param axis int 0:x, 1:y, 2:z
         * @return triggered boolean
         */
        bool clickedAxis(uint8_t axis);

        /**
         * Set click axis enable
         * @param xSingle boolean enable
         * @param xDouble boolean enable
         * @param ySingle boolean enable
         * @param yDouble boolean enable
         * @param zSingle boolean enable
         * @param zDouble boolean enable
         */
        void setClickAxis(bool xSingle, bool xDouble, bool ySingle, bool yDouble, bool zSingle, bool zDouble);

        /**
         * Set click sensitivity threshold
         * @param threshold int 0 to 127
         */
        void setClickThreshold(uint8_t threshold);

        /**
         * Set click time limit
         * @param time int 0 to 127
         */
        void setClickTimeLimit(uint8_t time);

        /**
         * Set click time latency
         * @param latency int 0 to 255
         */
        void setClickTimeLatency(uint8_t latency);

        /**
         * Set click time window
         * @param window int 0 to 255
         */
        void setClickTimeWindow(uint8_t window);

        /**
         * Set sleep activation sensitivity threshold
         * @param threshold int 0 to 127
         */
        void setActivationThreshold(uint8_t threshold);

        /**
         * Set sleep activation time duration
         * @param duration int 0 to 255
         */
        void setActivationDuration(uint8_t duration);

        /**
         * Block data uodate - Blocks data being updated until read is performed
         * @param block boolean
         * 0: continuous update; 1: output registers not updated until MSB and LSB have been read
         */
        void blockDataUpdate(bool block);

        /**
         * Set Data selection - Switch between MSB and LSB on data output for higher resolution mode
         * @param selection boolean
         * 0: data LSb at lower address; 1: data MSb at lower address
         */
        void setDataSelection(bool selection);

        /**
         * Get value in Data Capture register
         * @return value integer
         */
        uint8_t getReference(void);

        /**
         * Set value in Data Capture register
         * @param enable boolean
         */
        void setReference(uint8_t value);

        /**
         * Has Data been overwriten before reading
         * @param axis integer 0: X axis, 1: Y axis, 2: Z axis, 3: All axis
         */
        bool overrun(uint8_t axis);

        /**
         * Is data available
         * @param axis integer 0: X axis, 1: Y axis, 2: Z axis, 3: All axis
         */
        bool available(uint8_t axis);

        /**
         * Read data from accelerometer's axis data register
         * @param axis integer 0: X axis, 1: Y axis, 2: Z axis
         * @return value integer
         */
        int16_t read(uint8_t axis);

        /**
         * Enable FIFO internal buffer
         * @param enable boolean
         */
        void enableFifo(bool enable);

        /**
         * Set FIFO Stream Mode
         * @param mode int 0: Bypass mode, 1:FIFO mode, 2:Stream mode, 3:Stream-to-FIFO mode
         */
        void setFifoMode(uint8_t mode);

        /**
         * Set FIFO INT1 Trigger
         * @param trigger integer 0:Trigger INT1, 1:Trigger INT2
         */
        void setFifoTrigger(uint8_t trigger);

        /**
         * Set FIFO Watermark size
         * @param size int 0 to 31
         */
        void setFifoWatermarkSize(uint8_t size);

        /**
         * FIFO Available
         * @return available boolean returns true if buffer contains data more than or equal to the FIFO watermark size.
         */
        bool fifoAvailable(void);

        /**
         * FIFO Overrun returns true
         * @return overrun boolean returns true if watermark size in buffer was overrun
         */
        bool fifoOverrun(void);

        /**
         * Get FIFO unread samples
         * @return value integer number of unread samples
         */
        uint8_t fifoUnreadSamples(void);
        
        /**
         * Reset the FIFO flags
         */
        void fifoReset(void);

        /**
         * Read raw data from register
         * @param reg integer register to read
         * @return value integer
         */
        int readRegister(uint8_t reg);

        /**
         * Write raw data to register
         * @param reg integer register to write to
         * @param value integer value to write
         */
        void writeRegister(uint8_t reg, uint8_t value);

        int x;
        int y;
        int z;

    private:

        uint8_t _int1Status;
        uint8_t _int2Status;
        uint8_t _clickStatus;
        uint8_t _fifoWatermarkSize;
        uint8_t _fifoMode;

        //const uint8_t intPin;
        const uint8_t STATUS_REG_AUX = 0x07;

        inline int16_t _toInt16(uint8_t l, uint8_t h);
};

#endif
