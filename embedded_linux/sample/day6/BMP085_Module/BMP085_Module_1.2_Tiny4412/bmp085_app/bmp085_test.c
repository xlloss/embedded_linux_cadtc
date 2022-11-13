/*

  BMP085 Module Tester code
  Author:     Howard  Lin <cdpda.teacher@gmail.com>

  Connect VCC of the BMP085 Module to 3.3V or 5V (Auto detect)
  Connect GND to Ground
  Connect SCL to i2c clock
  Connect SDA to i2c data

  EOC is not used, it signifies an end of conversion
  XCLR is a reset pin, also not used here

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#define BMP085_TEMPERATURE_SYS_PATH "/sys/bus/i2c/devices/0-0077/temp_input"
#define BMP085_PRESSURE_SYS_PATH    "/sys/bus/i2c/devices/0-0077/pressure_input"
#define BMP085_OVERSAMPLING_SYS_PATH  "/sys/bus/i2c/devices/0-0077/oversampling"

#define TEMP_BUF_MAX_SIZE 4
#define PRESSURE_BUF_MAX_SIZE 7

#define SEA_LEVEL_HPA  1013.25

float readAltitude(float bmp085_pressure)
{
	float altitude;
	
	altitude = 44330 * ( 1.0 - pow((bmp085_pressure/SEA_LEVEL_HPA),0.1903));
	
	return altitude;
}

int main(int argc,char *argv[]){
	
	int bmp085_temperature_fd,bmp085_pressure_fd,bmp085_oversampling_fd;
	int res;
	float bmp085_temperature,bmp085_pressure;
        char  temperature_buf[TEMP_BUF_MAX_SIZE],pressure_buf[PRESSURE_BUF_MAX_SIZE],oversampling_buf[1];

	//Open Oversampling sysfile
        bmp085_oversampling_fd = open (BMP085_OVERSAMPLING_SYS_PATH,O_RDWR);
        if(-1 == bmp085_oversampling_fd)
        {
                printf("ERROR:Can not detect bmp085 module\n");
                return -1;
        }


	//Open Temperature sysfile
	bmp085_temperature_fd = open (BMP085_TEMPERATURE_SYS_PATH,O_RDONLY);
	if(-1 == bmp085_temperature_fd)
	{
		printf("ERROR:Can not detect bmp085 module\n");
                return -1;
	}
	
	//Open pressure sysfile
	bmp085_pressure_fd = open (BMP085_PRESSURE_SYS_PATH,O_RDONLY);
        if(-1 == bmp085_pressure_fd)
        {
                printf("ERROR:Can not detect bmp085 module\n");
                return -1;
        }

	//Read oversampling value
	res= read(bmp085_oversampling_fd,oversampling_buf,1);
	if(res <=0)
        {
                printf("Can not read oversampling value\n");
                return 1;
        }
        printf("Oversampling = %d\n",atoi(oversampling_buf));

	//Read bmp085 temperature value
        res=read(bmp085_temperature_fd,temperature_buf,TEMP_BUF_MAX_SIZE);
        if(res <=0)
        {
                printf("Can not read temperature value\n");
                return 1;
        }
        bmp085_temperature=atof(temperature_buf);
        printf("Temperature = %.1f 'C \n",bmp085_temperature/10);


	//Read bmp085 pressure value
        res=read(bmp085_pressure_fd,pressure_buf,PRESSURE_BUF_MAX_SIZE);
        if(res <=0)
        {
                printf("Can not read pressure value\n");
                return 1;
        }
        bmp085_pressure=atof(pressure_buf);
        printf("Pressure = %.2f hPa \n",bmp085_pressure/100);

	//Calculate Altitude
        printf("Altitude = %.3f meters \n",readAltitude(bmp085_pressure/100));

	return 0;
}
