#include "sht20.h"



Sht20::Sht20 (I2c *d)
{
  driver = d;
}

void Sht20::readTemperature ()
{
  driver->start ();
  while (!driver->flagBusy());
  //Записываем в регистр данных адрес ведомого устройства
  driver->setAddress (selfAddress, I2c::directionBit::write);
  driver->waitAck();
  driver->putData (sht20commands::tMesurement);
  driver->waitAck();
  delay_us (30);
  //Посылаем СТОП-посылку
  driver->stop ();
  while (driver->flagBusy());
  delay_ms (15);
  
  driver->start ();
  while (!driver->flagBusy());
  driver->setAddress (selfAddress, I2c::directionBit::read);
  driver->stop ();
  while (driver->flagBusy());
  delay_ms (15);
  
  driver->start ();
  while (!driver->flagBusy());
  driver->setAddress (selfAddress, I2c::directionBit::read);
  driver->waitAck();
  driver->setMode (I2c::mode::receiver);
  driver->generateAck();
  temperatureCode = driver->getData();
  driver->waitAck();
  temperatureCode <<= 8;
  temperatureCode |= driver->getData();
  driver->waitAck();
  driver->generateNack ();
  checksum = driver->getData();
  driver->waitAck();
  driver->stop ();
  while (driver->flagBusy());
}

void Sht20::readHummidity  ()
{
	 driver->start ();
	  while (!driver->flagBusy());
	  //Записываем в регистр данных адрес ведомого устройства
	  driver->setAddress (selfAddress, I2c::directionBit::write);
	  driver->waitAck();
	  driver->putData (sht20commands::rMesurement);
	  driver->waitAck();
	  delay_us (30);
	  //Посылаем СТОП-посылку
	  driver->stop ();
	  while (driver->flagBusy());
	  delay_ms (15);

	  driver->start ();
	  while (!driver->flagBusy());
	  driver->setAddress (selfAddress, I2c::directionBit::read);
	  driver->stop ();
	  while (driver->flagBusy());
	  delay_ms (15);

	  driver->start ();
	  while (!driver->flagBusy());
	  driver->setAddress (selfAddress, I2c::directionBit::read);
	  driver->waitAck();
	  driver->setMode (I2c::mode::receiver);
	  driver->generateAck();
	  hummdityCode = driver->getData();
	  driver->waitAck();
	  hummdityCode <<= 8;
	  hummdityCode |= driver->getData();
	  driver->waitAck();
	  driver->generateNack ();
	  checksum = driver->getData();
	  driver->waitAck();
	  driver->stop ();
	  while (driver->flagBusy());
}

void Sht20::setResolution (resolution r)
{
  res = r;
  
}

void Sht20::readUserR ()
{
	driver->start ();
	while (!driver->flagBusy());
	driver->setAddress (selfAddress, I2c::directionBit::write);
	driver->waitAck();
	driver->putData(sht20commands::readUserRegister);
	driver->waitAck();
	driver->restart();
	driver->setAddress (selfAddress, I2c::directionBit::read);
	driver->waitAck();
	driver->setMode (I2c::mode::receiver);
	driver->generateNack ();
	uRegister = driver->getData();
	driver->waitAck();
	driver->stop ();
	while (driver->flagBusy());
}

void Sht20::writeUserR  (uint8_t data)
{
  driver->start ();
  while (!driver->flagBusy());
  //Записываем в регистр данных адрес ведомого устройства
  driver->setAddress (selfAddress, I2c::directionBit::write);
  driver->waitAck();
  driver->putData (sht20commands::writeUserRegister);
  driver->waitAck();
  //Отправка данных
  driver->putData (data);
  driver->waitAck();
  driver->stop ();  
  while (driver->flagBusy());
}

uint8_t & Sht20::getUserRegister ()
{
  return uRegister;
}
  
void Sht20::convertHummdity ()
{
  hummdity = (((125*hummdityCode)>>16) - 60)/10;
}

void Sht20::convertTemperature ()
{
  temperature = (((17572*temperatureCode)>>16) - 4685)/10;
}

