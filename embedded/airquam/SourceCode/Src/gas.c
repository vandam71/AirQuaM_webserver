#include "gas.h"
#include "stm32f7xx_hal.h"
#include "average.h"
#include "mics4514.h"
#include "ccs811.h"


//sliding average objects
static average_t avrNO2;
static average_t avrCO;
static average_t avrCO2;
static average_t avrTVOC;



/**
  * @brief  initialization process for the mics4514 and ccs811 sensors
  * @retval None
  */
void gas_init(void)
{
	
	mics4514_init();
	ccs811_init();
	
	average_init(&avrNO2, mics4514_measureNO2());
	average_init(&avrCO, mics4514_measureCO());
	average_init(&avrCO2, ccs811_measureCO2());
	average_init(&avrTVOC, ccs811_measureTVOC());
	
}

/**
  * @brief  read all gases
  * @retval gas_t object with all measurements
  */
gas_t gas_read(void)
{
	gas_t gas;
	
	gas.NO2 	= gas_readNO2();
	gas.CO 		= gas_readCO();
	ccs811_measure_CO2_TVOC(&gas.CO2, &gas.TVOC);
	gas.CO2 	= fast_average(&avrCO2, gas.CO2);
	gas.TVOC 	= fast_average(&avrTVOC, gas.TVOC);
	
	return gas;
}

/**
  * @brief  read NO2 gas
  * @retval filtered value of NO2 measurement
  */
uint32_t gas_readNO2(void)
{	
	return fast_average(&avrNO2, mics4514_measureNO2() );
}

/**
  * @brief  read CO gas
  * @retval filtered value of CO measurement
  */
uint32_t gas_readCO(void)
{
	return fast_average(&avrCO, mics4514_measureCO() );
}

/**
  * @brief  read CO2 gas
  * @retval filtered value of CO2 measurement
  */
uint32_t gas_readCO2(void)
{
	return fast_average(&avrCO2, ccs811_measureCO2() );
}

/**
  * @brief  read TVOC gas
  * @retval filtered value of TVOC measurement
  */
uint32_t gas_readTVOC(void)
{
	return fast_average(&avrTVOC, ccs811_measureTVOC() );
}
