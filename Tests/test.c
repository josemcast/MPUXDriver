#include "unity.h"
#include "mpu_driver.h"

//Callback functions for TX and RX
static mpuRetStatus_t TXData(uint8_t addr, uint8_t *data, uint8_t size)
    {
        if(size > 1)
        {
            if( (data[0] == 0x6B) && (data[1] == 0x00) )
            {
                return MPU_STATUS_OK;
            }
        }

        if(size == 1)
            return MPU_STATUS_OK;

        return MPU_STATUS_ERROR;
}

static mpuRetStatus_t RXData(uint8_t addr, uint8_t *data, uint8_t size)
    {
        if(size == 14)
        {
			data[0] = 0x01;
            data[1] = 0x02;
            data[2] = 0x03;
            data[3] = 0x04;
            data[4] = 0x05;
            data[5] = 0x06;
			data[6] = 0x07;
            data[7] = 0x08;
            data[8] = 0x09;
            data[9] = 0x10;
            data[10] = 0x11;
            data[11] = 0x12;
			data[12] = 0x13;
			data[13] = 0x14;
            
            return MPU_STATUS_OK;

        } else if (size == 2) {

			data[0] = 0x02;
			data[1] = 0x01;
			
			return MPU_STATUS_OK;

		} else {

			data[0] = 0x01;
            data[1] = 0x02;
            data[2] = 0x03;
            data[3] = 0x04;
            data[4] = 0x05;
            data[5] = 0x06;

			return MPU_STATUS_OK;
		}

        return MPU_STATUS_ERROR;
}

void setUp(void)
{

}

void tearDown(void)
{

}

void return_status_should_maintain_enumeration_order(void)
{
	TEST_ASSERT_EQUAL(0, MPU_STATUS_OK);
	TEST_ASSERT_EQUAL(1, MPU_STATUS_NOT_INIT);
	TEST_ASSERT_EQUAL(2, MPU_STATUS_ERROR);
	TEST_ASSERT_EQUAL(3, MPU_STATUS_COMM_ERROR);
}

void full_scale_should_maintain_enumeration_order(void)
{
	TEST_ASSERT_EQUAL(0, FS_SEL0);
	TEST_ASSERT_EQUAL(1, FS_SEL1);
	TEST_ASSERT_EQUAL(2, FS_SEL2);
	TEST_ASSERT_EQUAL(3, FS_SEL3);
}

void driver_state_should_not_be_ready_before_call_init()
{
	TEST_ASSERT_EQUAL(MPU_STATUS_ERROR, isMPUDriverReady());
}

void call_read_action_without_init_should_return_not_init()
{
	uint16_t buffer[3];

	mpuRetStatus_t ret = mpuReadAccelerometerRaw(buffer);
	TEST_ASSERT_EQUAL(MPU_STATUS_NOT_INIT, ret);

	ret = mpuReadGyroRaw(buffer);
	TEST_ASSERT_EQUAL(MPU_STATUS_NOT_INIT, ret);
}

void mpuInit_should_return_error_after_call_init_with_Wrong_Configs()
{
	mpuRetStatus_t ret = mpuInit(4, FS_SEL0, TXData, RXData);
	TEST_ASSERT_EQUAL(MPU_STATUS_WRONG_CONFIGS, ret);

	ret = mpuInit(FS_SEL1, 5, TXData, RXData);
	TEST_ASSERT_EQUAL(MPU_STATUS_WRONG_CONFIGS, ret);

	ret = mpuInit(FS_SEL1, FS_SEL0, 0, RXData);
	TEST_ASSERT_EQUAL(MPU_STATUS_WRONG_CONFIGS, ret);


}

void mpuInit_should_return_OK_after_call_init()
{
	mpuRetStatus_t ret = mpuInit(FS_SEL0, FS_SEL0, TXData, RXData);
	TEST_ASSERT_EQUAL(MPU_STATUS_OK, ret);
}

void driver_state_should_be_ready_after_call_init()
{

	mpuInit(FS_SEL0, FS_SEL0, TXData, RXData);
	TEST_ASSERT_EQUAL(MPU_STATUS_OK, isMPUDriverReady());
}

void util_function_should_generate_16bits_variable_from_bytes()
{
	uint8_t upper = 0x1A;
	uint8_t lower = 0x10;
	uint16_t word = concatenateBytesIntoHalfWord(upper, lower);
	TEST_ASSERT_EQUAL(0x1A10, word);
}


void read_accelerometer_should_return_three_valid_measurements()
{
	mpuInit(FS_SEL0, FS_SEL0, TXData, RXData);
	
	uint16_t buffer[3];

	mpuRetStatus_t ret = mpuReadAccelerometerRaw(buffer);

	TEST_ASSERT_EQUAL(0x0102, buffer[0]);
	TEST_ASSERT_EQUAL(0x0304, buffer[1]);
	TEST_ASSERT_EQUAL(0x0506, buffer[2]);
	TEST_ASSERT_EQUAL(MPU_STATUS_OK, ret);
}


void read_gyro_should_return_three_valid_measurements()
{

	mpuInit(FS_SEL0, FS_SEL0, TXData, RXData);
	
	uint16_t buffer[3];

	mpuRetStatus_t ret = mpuReadGyroRaw(buffer);

	TEST_ASSERT_EQUAL(0x0102, buffer[0]);
	TEST_ASSERT_EQUAL(0x0304, buffer[1]);
	TEST_ASSERT_EQUAL(0x0506, buffer[2]);
	TEST_ASSERT_EQUAL(MPU_STATUS_OK, ret);
}

void read_temperature_should_return_valid_measurement()
{
	mpuInit(FS_SEL0, FS_SEL0, TXData, RXData);
	
	uint16_t buffer[1];

	mpuRetStatus_t ret = mpuReadTemperatureRaw(buffer);

	TEST_ASSERT_EQUAL(0x0201, buffer[0]);
	TEST_ASSERT_EQUAL(MPU_STATUS_OK, ret);
}

void read_all_in_one_batch()
{

	mpuInit(FS_SEL0, FS_SEL0, TXData, RXData);
	
	uint16_t buffer[7];

	mpuRetStatus_t ret = mpuReadAllRaw(buffer);

	TEST_ASSERT_EQUAL(0x0102, buffer[0]);
	TEST_ASSERT_EQUAL(0x0304, buffer[1]);
	TEST_ASSERT_EQUAL(0x0506, buffer[2]);
	TEST_ASSERT_EQUAL(0x0708, buffer[3]);
	TEST_ASSERT_EQUAL(0x0910, buffer[4]);
	TEST_ASSERT_EQUAL(0x1112, buffer[5]);
	TEST_ASSERT_EQUAL(0x1314, buffer[6]);
	TEST_ASSERT_EQUAL(MPU_STATUS_OK, ret);

}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(return_status_should_maintain_enumeration_order);
	RUN_TEST(full_scale_should_maintain_enumeration_order);
	
	
	RUN_TEST(driver_state_should_not_be_ready_before_call_init);
	RUN_TEST(mpuInit_should_return_error_after_call_init_with_Wrong_Configs);
	RUN_TEST(call_read_action_without_init_should_return_not_init);
	RUN_TEST(mpuInit_should_return_OK_after_call_init);
	RUN_TEST(driver_state_should_be_ready_after_call_init);
	
	RUN_TEST(util_function_should_generate_16bits_variable_from_bytes);

	RUN_TEST(read_accelerometer_should_return_three_valid_measurements);
	
	RUN_TEST(read_temperature_should_return_valid_measurement);

	RUN_TEST(read_gyro_should_return_three_valid_measurements);

	RUN_TEST(read_all_in_one_batch);


	return UNITY_END();
}
