#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#define DAC_ADDR 0x61

#define TENSION_MAX 3.3

#define DAC_CMD_WRITE_REGISTER 		0x00
#define DAC_CMD_SET_VOLTAGE_VOLATIL 0x02
#define DAC_CMD_SET_VOLTAGE_ALL 	0x03
#define DAC_CMD_CONFIG_BITS 		0x04

#define DAC_VREF_VDD		0x00
#define DAC_VREF_UNBUFFER 	0x02
#define DAC_VREF_BUFFER		0x03

#define DAC_PD_ON		0x00
#define DAC_PD_OFF_1k	0x01
#define DAC_PD_OFF_100k	0x02
#define DAC_PD_OFF_500k	0x03

#define DAC_GAIN_1	0x00
#define DAC_GAIN_2	0x01

struct Comando {
	char arg1[50];
	char arg2[50];
	char arg3[50];
	char arg4[50];
};

int formatoValido (const char *comando);
int numArgumentos(const char *comando);
struct Comando saveArguments (const char *comando);
int comandoParaTestTool (const char *comando);
int segundoArgumento (const char *comando);
int tercerArgumento (const char *comando);
uint16_t calculate_voltage_register(float voltage);
void dac_set_voltage(uint8_t cmd, uint8_t *cmd_str, uint8_t vref, uint8_t pd, uint8_t gain, float voltage);
float cuartoArgumento (const char *comando);

