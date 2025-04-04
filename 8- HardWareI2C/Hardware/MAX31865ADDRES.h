#ifndef _MAX31865ADDRES_H
#define _MAX31865ADDRES_H


#define MAX31865_CONFIGURATION_R                0x00
#define MAX31865_RTD_MSBS_R                     0x01
#define MAX31865_RTD_LSBS_R                     0x02
#define MAX31865_HIGH_FAULT_THRESHOLD_MSB_R     0x03
#define MAX31865_HIGH_FAULT_THRESHOLD_LSB_R     0x04
#define MAX31865_LOW_FAULT_THRESHOLD_MSB_R      0X05
#define MAX31865_LOW_FAULT_THRESHOLD_LSB_R      0X06
#define MAX31865_FAULT_STATUS_R                 0x07

#define MAX31865_CONFIGURATION_W                0x80
#define MAX31865_HIGH_FAULT_THRESHOLD_MSB_W     0x03
#define MAX31865_HIGH_FAULT_THRESHOLD_LSB_W     0x04
#define MAX31865_LOW_FAULT_THRESHOLD_MSB_W      0X05
#define MAX31865_LOW_FAULT_THRESHOLD_LSB_W      0X06



#endif
