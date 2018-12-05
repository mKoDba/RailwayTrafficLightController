#include</home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/config.h>

#define F_SCL 100000UL  /* define SCL freq - 100kHz or 400kHz according to datasheet */
#define PRESCALER 1
#define TWBR_VAL ((((F_CPU / F_SCL) / PRESCALER) - 16 )/2)

uint8_t i2c_start(uint8_t slave_address);
void i2c_stop(void);

uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);