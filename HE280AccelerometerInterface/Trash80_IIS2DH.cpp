
#include "Trash80_IIS2DH.h"
  
/**
 * Init
 * Enables Wire library for IC
 * @param fastMode boolean enable 400khz i2c
 */
void Trash80_IIS2DH::init(bool fastMode)
{
    Wire.begin(); // join i2c bus
    if(fastMode) {
        Wire.setClock(400000L);
    }
    delay(100); // wait for boot
}

/**
 * Is Connectected?
 * @return connected boolean
 * Performs a read by requesting a WHO_AM_I from IC, returns false if not connected or WHO_AM_I does not match expected result
 */
bool Trash80_IIS2DH::connected(void)
{
    return (getWhoAmI() == IIS2DH_WHO_AM_I_VALUE);
}

/**
 * Get the internal counter value
 * @return value integer internal counter
 */
uint8_t Trash80_IIS2DH::getInternalCounter(void)
{
    return readRegister(IIS2DH_INT_COUNTER_REG);
}

/**
 * Get Who Am I register. Should return 0x33
 * @return value integer (0x33)
 */
uint8_t Trash80_IIS2DH::getWhoAmI(void)
{
    return readRegister(IIS2DH_WHO_AM_I);
}

/**
 * Self-test enable
 * @param test integer 0 to 3
 * 0: Normal, 1: Self Test 0, 2: Self Test 1
 */
void Trash80_IIS2DH::setSelfTestMode(uint8_t test)
{
    writeRegister(IIS2DH_CTRL_REG4, (readRegister(IIS2DH_CTRL_REG4) & B11111001) | ((test&B00000011)<<1));
}

/**
 * Enable SPI 3 wire interface
 * @param wire boolean
 * 0: 4 wire, 1: 3 wire
 */
void Trash80_IIS2DH::enableSpi3Wire(bool wire)
{
    writeRegister(IIS2DH_CTRL_REG4, (readRegister(IIS2DH_CTRL_REG4) & B11111110) | (wire&B00000001));
}

/**
 * Reset memory to power-on settings (reboot memory content)
 */
void Trash80_IIS2DH::resetMemory(void)
{
    writeRegister(IIS2DH_CTRL_REG5, readRegister(IIS2DH_CTRL_REG5) | B10000000);
    writeRegister(IIS2DH_CTRL_REG5, readRegister(IIS2DH_CTRL_REG5) & B01111111);
}


/**
 * ODR Output Data Rate
 * @param rate integer 0 to 9
 * 0:Power-down mode, 1:1Hz, 2:10Hz, 3:25Hz, 4:50Hz, 5:100Hz, 6:200Hz, 7:400Hz, 8:1.620kHz, 9:1.344kHz Normal / 5.376 kHz Low power)
 */
void Trash80_IIS2DH::setDataRate(uint8_t rate)
{
    writeRegister(IIS2DH_CTRL_REG1, (readRegister(IIS2DH_CTRL_REG1) & B00001111) | ((rate&B00001111)<<4));
}

/**
 * Low power, normal, and high resolution mode
 * @param mode integer 0 to 2
 * 0:Low-power mode (8bit resolution), 1:Normal mode (10bit), 2:High-resolution mode (12bit)
 */
void Trash80_IIS2DH::setDataResolution(uint8_t mode)
{
    switch(mode) {
        case 0:
            writeRegister(IIS2DH_CTRL_REG1, (readRegister(IIS2DH_CTRL_REG1) & B11110111) | B00001000);
            writeRegister(IIS2DH_CTRL_REG4, (readRegister(IIS2DH_CTRL_REG4) & B11110111));
            break;
        case 1:
            writeRegister(IIS2DH_CTRL_REG1, (readRegister(IIS2DH_CTRL_REG1) & B11110111));
            writeRegister(IIS2DH_CTRL_REG4, (readRegister(IIS2DH_CTRL_REG4) & B11110111));
            break;
        case 2:
            writeRegister(IIS2DH_CTRL_REG1, (readRegister(IIS2DH_CTRL_REG1) & B11110111));
            writeRegister(IIS2DH_CTRL_REG4, (readRegister(IIS2DH_CTRL_REG4) & B11110111) | B00001000);
            break;
    }
}

/**
 * Set Output Scaling
 * @param g integer 0 to 3
 * 0: ±2g, 1: ±4g, 2: ±8g, 3: ±16g
 */
void Trash80_IIS2DH::setScale(uint8_t g)
{
    writeRegister(IIS2DH_CTRL_REG4, (readRegister(IIS2DH_CTRL_REG4) & B11001111) | ((g&B00000011)<<4));
}


/**
 * Enable Temperature reading
 * @param enable boolean
 */
void Trash80_IIS2DH::enableTemperature(bool enable)
{
    if(enable) {
        writeRegister(IIS2DH_TEMP_CFG_REG, 0b11000000);
    } else {
        writeRegister(IIS2DH_TEMP_CFG_REG, 0b00000000);
    }
}
/**
 * Check if a new Temperature reading is available
 * @return available boolean
 */
bool Trash80_IIS2DH::temperatureAvailable(void)
{
    uint8_t d = readRegister(IIS2DH_REG_STATUS_AUX);
    return (d>>2)&0x01;
}

/**
 * Get the current temperature
 * @return value integer value in Celsius
 */
int Trash80_IIS2DH::getTemperature(void)
{
    return _toInt16(readRegister(IIS2DH_OUT_TEMP_L),readRegister(IIS2DH_OUT_TEMP_H));
}

/**
 * Axis Enable (X, Y, Z)
 * @param x boolean
 * @param y boolean
 * @param z boolean
 */
void Trash80_IIS2DH::enableAxis(bool x, bool y, bool z)
{
    uint8_t d = ((z&0x01)<<2)|((y&0x01)<<1)|(x&0x01);
    writeRegister(IIS2DH_CTRL_REG1, (readRegister(IIS2DH_CTRL_REG1) & B11111000) | d);
}

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
void Trash80_IIS2DH::setFilter(uint8_t mode, uint8_t cutoff, bool filterDataOutput, bool enableClick, bool enableInt1, bool enableInt2)
{
    uint8_t filter = ((mode&0b00000011)<<6)|((cutoff&0b00000011)<<4)|((filterDataOutput&0x01)<<3)|((enableClick&0x01)<<2)|((enableInt2&0x01)<<1)|(enableInt1&0x01);
    writeRegister(IIS2DH_CTRL_REG2, filter);
}

/**
 * Set INT1 And-Or interrupt mode
 * @param mode int 0: OR combination of int events, 1: 6-direction movement recognition, 2: AND combination of interrupt events, 3: 6D position recognition
 */
void Trash80_IIS2DH::setInt1Mode(uint8_t mode)
{
    writeRegister(IIS2DH_INT1_CFG, (readRegister(IIS2DH_INT1_CFG) & B00111111) | ((mode&B00000011)<<6));
}

/**
 * Set INT1 axis trigger on direction
 * @param xDown boolean
 * @param xUp boolean
 * @param yDown boolean
 * @param yUp boolean
 * @param zDown boolean
 * @param zUp boolean
 */
void Trash80_IIS2DH::setInt1AxisTrigger(bool xDown, bool xUp, bool yDown, bool yUp, bool zDown, bool zUp)
{
    uint8_t d = ((zUp&0x01)<<5)|((zDown&0x01)<<4)|((yUp&0x01)<<3)|((yDown&0x01)<<2)|((xUp&0x01)<<1)|(xDown&0x01);
    writeRegister(IIS2DH_INT1_CFG, (readRegister(IIS2DH_INT1_CFG) & B11000000) | d);
}

/**
 * Set INT1 trigger movement threshold
 * @param threshold int 0 to 127
 * Scaling    THS Unit    MG
 *    2g          1       16mg
 *    4g          1       32mg
 *    8g          1       62mg
 *    16g         1       186mg
 */
void Trash80_IIS2DH::setInt1Threshold(uint8_t threshold)
{
    writeRegister(IIS2DH_INT1_THS, threshold&B01111111);
}

/**
 * Set INT1 trigger movement duration
 * @param duration int 0 to 127 : 1 = 1/Output Data Rate
 */
void Trash80_IIS2DH::setInt1Duration(uint8_t duration)
{
    writeRegister(IIS2DH_INT1_DURATION, duration&B01111111);
}

/**
 * Get INT1 Status Register - Clears/Resets interrupt on read
 * @return value integer raw value from register
 */
uint8_t Trash80_IIS2DH::int1Status(void)
{
    _int1Status = readRegister(IIS2DH_INT1_SRC);
    return _int1Status;
}

/**
 * Get INT1 axis source from status register
 * @param axis integer 0:x,1:y,2:z
 * @return value integer
 */
uint8_t Trash80_IIS2DH::int1AxisSource(uint8_t axis)
{
    switch(axis) {
        case 1:
            return ((_int1Status>>2)&0x03);
        case 2:
            return ((_int1Status>>4)&0x03);
        case 0:
        default:
            return (_int1Status&0x03);
    }
}

/**
 * Set INT1 to Latching mode
 * @param enable boolean
 */
void Trash80_IIS2DH::setInt1Latch(bool enable)
{
    writeRegister(IIS2DH_CTRL_REG5, (readRegister(IIS2DH_CTRL_REG5) & B11110111) | ((enable&B00000001)<<3));
}

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
void Trash80_IIS2DH::setInt1Options(bool onClick, bool onAndOr1, bool onAndOr2, bool onDataReady1, bool onDataReady2, bool onFifoWatermark, bool onFifoOverrun)
{
    uint8_t d = ((onClick&0x01)<<7)|((onAndOr1&0x01)<<6)|((onAndOr2&0x01)<<5)|((onDataReady1&0x01)<<4)|((onDataReady2&0x01)<<3)|((onFifoWatermark&0x01)<<2)|((onFifoOverrun&0x01)<<1);
    writeRegister(IIS2DH_CTRL_REG3, (readRegister(IIS2DH_CTRL_REG3) & B00000001) | d);
}

/**
 * Set INT2 And-Or interrupt mode
 * @param mode int 0: OR combination of int events, 1: 6-direction movement recognition, 2: AND combination of interrupt events, 3: 6D position recognition
 */
void Trash80_IIS2DH::setInt2Mode(uint8_t mode)
{
    writeRegister(IIS2DH_INT2_CFG, (readRegister(IIS2DH_INT2_CFG) & B00111111) | ((mode&B00000011)<<6));
}

/**
 * Set INT2 axis trigger on direction
 * @param xDown boolean
 * @param xUp boolean
 * @param yDown boolean
 * @param yUp boolean
 * @param zDown boolean
 * @param zUp boolean
 */
void Trash80_IIS2DH::setInt2AxisTrigger(bool xDown, bool xUp, bool yDown, bool yUp, bool zDown, bool zUp)
{
    uint8_t d = ((zUp&0x01)<<5)|((zDown&0x01)<<4)|((yUp&0x01)<<3)|((yDown&0x01)<<2)|((xUp&0x01)<<1)|(xDown&0x01);
    writeRegister(IIS2DH_INT2_CFG, (readRegister(IIS2DH_INT2_CFG) & B11000000) | d);
}

/**
 * Set INT2 trigger movement threshold
 * @param threshold int 0 to 127
 * Scaling    THS Unit    MG
 *    2g          1       16mg
 *    4g          1       32mg
 *    8g          1       62mg
 *    16g         1       186mg
 */
void Trash80_IIS2DH::setInt2Threshold(uint8_t threshold)
{
    writeRegister(IIS2DH_INT2_THS, threshold&B01111111);
}

/**
 * Set INT2 trigger movement duration
 * @param duration int 0 to 127 : 1 = 1/Output Data Rate
 */
void Trash80_IIS2DH::setInt2Duration(uint8_t duration)
{
    writeRegister(IIS2DH_INT2_DURATION, duration&B01111111);
}

/**
 * Get INT2 Status Register - Clears/Resets interrupt on read
 * @return value integer raw value from register
 */
uint8_t Trash80_IIS2DH::int2Status(void)
{
    _int2Status = readRegister(IIS2DH_INT2_SRC);
    return _int2Status;
}

/**
 * Get INT2 axis source from status register
 * @param axis integer 0:x,1:y,2:z
 * @return value integer
 */
uint8_t Trash80_IIS2DH::int2AxisSource(uint8_t axis)
{
    switch(axis) {
        case 1:
            return ((_int2Status>>2)&0x03);
        case 2:
            return ((_int2Status>>4)&0x03);
        case 0:
        default:
            return (_int2Status&0x03);
    }
}

/**
 * Set INT2 to Latching mode
 * @param enable boolean
 */
void Trash80_IIS2DH::setInt2Latch(bool enable)
{
    writeRegister(IIS2DH_CTRL_REG5, (readRegister(IIS2DH_CTRL_REG5) & B11111101) | ((enable&B00000001)<<1));
}

/**
 * Relay internal interrupts to INT2 Pin
 * @param onClick boolean
 * @param onAndOr1 boolean
 * @param onAndOr2 boolean
 * @param onBoot boolean
 * @param polarity boolean 0: interrupt active-high; 1: interrupt active-low
  */
    void Trash80_IIS2DH::setInt2Options(bool onClick, bool onAndOr1, bool onAndOr2, bool onBoot, bool onActivity, bool polarity)
{
    uint8_t d = ((onClick&0x01)<<7)|((onAndOr1&0x01)<<6)|((onAndOr2&0x01)<<5)|((onBoot&0x01)<<4)|((onActivity&0x01)<<3)|((polarity&0x01)<<1);
    writeRegister(IIS2DH_CTRL_REG6, (readRegister(IIS2DH_CTRL_REG3) & B00000001) | d);
}

/**
 * Click is available/triggered
 * @return direction int 0:none, -1:up direction, 1: down direction
 */
uint8_t Trash80_IIS2DH::clickAvailable(void)
{
    _clickStatus = readRegister(IIS2DH_CLICK_SRC)>>7;
    return _clickStatus;
}

/**
 * Clicked axis
 * @param axis int 0:x, 1:y, 2:z
 * @return triggered boolean
 */
bool Trash80_IIS2DH::clickedAxis(uint8_t axis)
{
    switch(axis) {
        case 1:
            return ((_clickStatus>>1)&0x03);
        case 2:
            return ((_clickStatus>>2)&0x03);
        case 0:
        default:
            return (_clickStatus&0x03);
    }
}

/**
 * Set click axis enable
 * @param xSingle boolean enable
 * @param xDouble boolean enable
 * @param ySingle boolean enable
 * @param yDouble boolean enable
 * @param zSingle boolean enable
 * @param zDouble boolean enable
 */
void Trash80_IIS2DH::setClickAxis(bool xSingle, bool xDouble, bool ySingle, bool yDouble, bool zSingle, bool zDouble)
{
    uint8_t d = ((zDouble&0x01)<<5)|((zSingle&0x01)<<4)|((yDouble&0x01)<<3)|((ySingle&0x01)<<2)|((xDouble&0x01)<<1)|((xSingle&0x01));
    writeRegister(IIS2DH_CLICK_CFG, (readRegister(IIS2DH_CLICK_CFG) & B11000000) | d);
}

/**
 * Set click sensitivity threshold
 * @param threshold int 0 to 127
 */
void Trash80_IIS2DH::setClickThreshold(uint8_t threshold)
{
    writeRegister(IIS2DH_CLICK_THS, threshold&B01111111);
}

/**
 * Set click time limit
 * @param time int 0 to 127
 */
void Trash80_IIS2DH::setClickTimeLimit(uint8_t time)
{
    writeRegister(IIS2DH_TIME_LIMIT, time&B01111111);
}

/**
 * Set click time latency
 * @param latency int 0 to 255
 */
void Trash80_IIS2DH::setClickTimeLatency(uint8_t latency)
{
    writeRegister(IIS2DH_TIME_LATENCY, latency);
}

/**
 * Set click time window
 * @param window int 0 to 255
 */
void Trash80_IIS2DH::setClickTimeWindow(uint8_t window)
{
    writeRegister(IIS2DH_TIME_WINDOW, window);
}

/**
 * Set sleep activation sensitivity threshold
 * @param threshold int 0 to 127
 */
void Trash80_IIS2DH::setActivationThreshold(uint8_t threshold)
{
    writeRegister(IIS2DH_ACT_THS, threshold&B01111111);
}

/**
 * Set sleep activation time duration
 * @param duration int 0 to 255
 */
void Trash80_IIS2DH::setActivationDuration(uint8_t duration)
{
    writeRegister(IIS2DH_ACT_DUR, duration);
}

/**
 * Block data uodate - Blocks data being updated until read is performed
 * @param block boolean
 * 0: continuous update; 1: output registers not updated until MSB and LSB have been read
 */
void Trash80_IIS2DH::blockDataUpdate(bool block)
{
    uint8_t d = readRegister(IIS2DH_CTRL_REG4);
    writeRegister(IIS2DH_CTRL_REG4,(((uint8_t) block<<7) | (d & 0b01111111)));
}

/**
 * Set Data selection - Switch between MSB and LSB on data output for higher resolution mode
 * @param selection boolean
 * 0: data LSb at lower address; 1: data MSb at lower address
 */
void Trash80_IIS2DH::setDataSelection(bool selection)
{
    writeRegister(IIS2DH_CTRL_REG4, (readRegister(IIS2DH_CTRL_REG4) & B10111111) | ((selection&0x01)<<6));
}

/**
 * Get value in Data Capture register
 * @return value integer
 */
uint8_t Trash80_IIS2DH::getReference(void)
{
    return readRegister(IIS2DH_DATACAPTURE);
}

/**
 * Set value in Data Capture register
 * @param enable boolean
 */
void Trash80_IIS2DH::setReference(uint8_t value)
{
    writeRegister(IIS2DH_DATACAPTURE, value);
}

/**
 * Has Data been overwriten before reading
 * @param axis integer 0: X axis, 1: Y axis, 2: Z axis, 3: All axis
 */
bool Trash80_IIS2DH::overrun(uint8_t axis)
{
    return ((readRegister(IIS2DH_STATUS_REG)>>3)>>axis)&0x01;
}

/**
 * Is data available
 * @param axis integer 0: X axis, 1: Y axis, 2: Z axis, 3: All axis
 */
bool Trash80_IIS2DH::available(uint8_t axis)
{
    return (readRegister(IIS2DH_STATUS_REG)>>axis)&0x01;
}

/**
 * Read data from accelerometer's axis data register
 * @param axis integer 0: X axis, 1: Y axis, 2: Z axis
 * @return value integer
 */
int16_t Trash80_IIS2DH::read(uint8_t axis)
{   
    switch(axis) {
        case 1:
            return _toInt16(readRegister(IIS2DH_OUT_Y_L), readRegister(IIS2DH_OUT_Y_H));
        case 2:
            return _toInt16(readRegister(IIS2DH_OUT_Z_L), readRegister(IIS2DH_OUT_Z_H));
        case 0:
        default:
            return _toInt16(readRegister(IIS2DH_OUT_X_L), readRegister(IIS2DH_OUT_X_H));
    }
}

/**
 * Enable FIFO internal buffer
 * @param enable boolean
 */
void Trash80_IIS2DH::enableFifo(bool enable)
{
    writeRegister(IIS2DH_CTRL_REG5, (readRegister(IIS2DH_CTRL_REG5) & B10111111) | ((enable&0x01)<<6));
}

/**
 * Set FIFO Stream Mode
 * @param mode int 0: Bypass mode, 1:FIFO mode, 2:Stream mode, 3:Stream-to-FIFO mode
 */
void Trash80_IIS2DH::setFifoMode(uint8_t mode)
{
    _fifoMode = mode;
    writeRegister(IIS2DH_FIFO_CTRL_REG, (readRegister(IIS2DH_FIFO_CTRL_REG) & B00111111)); //reset to 0
    writeRegister(IIS2DH_FIFO_CTRL_REG, (readRegister(IIS2DH_FIFO_CTRL_REG) & B00111111) | ((mode&0x03)<<6));
}

/**
 * Set FIFO INT1 Trigger
 * @param trigger integer 0:Trigger INT1, 1:Trigger INT2
 */
void Trash80_IIS2DH::setFifoTrigger(uint8_t trigger)
{
    writeRegister(IIS2DH_FIFO_CTRL_REG, (readRegister(IIS2DH_FIFO_CTRL_REG) & B11011111) | ((trigger&0x01)<<5));
}

/**
 * Set FIFO Watermark size
 * @param size int 0 to 31
 */
void Trash80_IIS2DH::setFifoWatermarkSize(uint8_t size)
{
    _fifoWatermarkSize = size;
    writeRegister(IIS2DH_FIFO_CTRL_REG, (readRegister(IIS2DH_FIFO_CTRL_REG) & B11100000) | (size&0x1F));
}

/**
 * FIFO Available
 * @return available boolean returns true if buffer contains data more than or equal to the FIFO watermark size.
 */
bool Trash80_IIS2DH::fifoAvailable(void)
{
    if(_fifoWatermarkSize) {
        return (readRegister(IIS2DH_FIFO_SRC_REG)>>7);
    }
    
    return (readRegister(IIS2DH_FIFO_SRC_REG)>>6);
}

/**
 * FIFO Overrun returns true
 * @return overrun boolean returns true if watermark size in buffer was overrun
 */
bool Trash80_IIS2DH::fifoOverrun(void)
{
    return (readRegister(IIS2DH_FIFO_SRC_REG)>>6)&0x01;
}

/**
 * Get FIFO unread samples
 * @return value integer number of unread samples
 */
uint8_t Trash80_IIS2DH::fifoUnreadSamples(void)
{
    return readRegister(IIS2DH_FIFO_SRC_REG)&0x1F;
}

/**
 * Reset the FIFO flags
 */
void Trash80_IIS2DH::fifoReset(void)
{
    setFifoMode(_fifoMode);
}

/**
 * Read raw data from register
 * @param reg integer register to read
 * @return value integer
 */
int Trash80_IIS2DH::readRegister(uint8_t reg)
{
    if(IIS2DH_REGISTER_RW_MAP[reg] == IIS2DH_N) return -1;

    Wire.beginTransmission(IIS2DH_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(IIS2DH_I2C_ADDR,1);

    if(!Wire.available()) return -1;

    return Wire.read();
}

/**
 * Write raw data to register
 * @param reg integer register to write to
 * @param value integer value to write
 */
void Trash80_IIS2DH::writeRegister(uint8_t reg, uint8_t value)
{
    if(IIS2DH_REGISTER_RW_MAP[reg] != IIS2DH_W) return ;

    Wire.beginTransmission(IIS2DH_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
    /*
    Serial.print("Set REG:");
    Serial.print(reg,HEX);
    Serial.print(" Value: ");
    Serial.println(value,HEX);
    */
}

inline int16_t Trash80_IIS2DH::_toInt16(uint8_t l, uint8_t h)
{
    int16_t data = (h<<8) | l;
    if(data&0x8000) {
      data = (data>>6)|0b1111111000000000;
    } else {
      data >>= 6;
    }
    return data;
}

