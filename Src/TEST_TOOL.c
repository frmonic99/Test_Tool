#include "TEST_TOOL.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

extern int indice;

//variables para configuración del DAC
extern uint8_t cmd_str[3];
uint8_t cfg_byte = 0x00;
uint16_t voltage_value = 0x00;
uint8_t voltage_byte_0 = 0x00;
uint8_t voltage_byte_1 = 0x00;
uint8_t tmp = 0x00;

int formatoValido (const char *comando){
	int longitudComando = strlen(comando);
	if (comando[0] != ':'){
		return 0; // no válido
	}
	for (int i=0; i<(longitudComando-1); i++){
        if (comando[i] == ':' && comando[i + 1] == ':') {
            return 0; // Comando no válido
        }
	}
	if (longitudComando < 3){
		return 0; // no válido
	}
/*	if (comando[longitudComando-1] != ';'){
		return 0; // no válido
	}*/
	return 1; // Válido
}

int numArgumentos(const char *comando){
	int longitudComando = strlen(comando);
	int argumentos = 0;
	if (formatoValido (comando)){
		for (int i=0; i<(longitudComando-1); i++){
	        if (comando[i] == ':') {
	        	argumentos++;
	        }
	    }
	}
	return argumentos;
}

struct Comando saveArguments (const char *comando){
	struct Comando resultado;
	char *argumento;
	char copiaComando[50];
	int posicion = 0;
	int nArgs = numArgumentos(comando);
	strcpy(copiaComando, comando);
	// Dividimos el comando en argumentos usando ":"
	argumento = strtok(copiaComando, ":");
	while (posicion < nArgs ){
		switch(posicion){
		case 0:
			strcpy(resultado.arg1, argumento);
			break;
		case 1:
			strcpy(resultado.arg2, argumento);
			break;
		case 2:
			strcpy(resultado.arg3, argumento);
			break;
		case 3:
			strcpy(resultado.arg4, argumento);
			break;
		}
	 posicion++;
	 argumento = strtok(NULL, ":"); //siguiente argumento
	}
	return resultado;
}

int comandoParaTestTool (const char *comando){
	struct Comando c = saveArguments(comando);
	if (strcmp(c.arg1, "testtool") == 0) {
		return 1;
	}
	return 0;
}

int segundoArgumento (const char *comando){
	struct Comando c = saveArguments(comando);
	if ((strcmp(c.arg2, "relay") == 0) || (strcmp(c.arg2, "relay;") == 0)){
		return 1;
	} else if ((strcmp(c.arg2, "dac") == 0) || (strcmp(c.arg2, "dac;") == 0)) {
		return 2;
	} else  if ((strcmp(c.arg2, "current") == 0) || (strcmp(c.arg2, "current;") == 0)){
		return 3;
	}
	return 0;
}

int tercerArgumento (const char *comando){
	struct Comando c = saveArguments(comando);
	if ((strcmp(c.arg3, "set_voltage") == 0) || (strcmp(c.arg2, "set_voltage;") == 0)){
		return 1;
	}
	return 0;
}

float cuartoArgumento (const char *comando){
	struct Comando c = saveArguments(comando);
	char *endptr;
	float voltage = strtof(c.arg4, &endptr);
	return voltage;
}

uint16_t calculate_voltage_register(float voltage){
	  uint16_t volt = (voltage * 1023) / TENSION_MAX;
	  return volt;
 }

void dac_set_voltage(uint8_t cmd, uint8_t *cmd_str, uint8_t vref, uint8_t pd, uint8_t gain, float voltage){
	  voltage_value = calculate_voltage_register(voltage);
	  tmp = (cmd & 0x07) << 5; //primeros 3 bits de cfg_byte
	  tmp = tmp | ((vref & 0x03) << 4); // 4º y 5º bit de cfg_byte
	  tmp = tmp | ((pd & 0x03) << 1); // 6º y 7º bit de cfg_byte
	  tmp = tmp | gain; // 8º bit de cfg_byte
	  cfg_byte = tmp;
	  voltage_byte_0 = voltage_value >> 2; // 8 primeros bits de voltage_value
	  voltage_byte_1 = (voltage_value & 0x03) << 6; // 2 últimos bits de voltage_value
	  cmd_str[0] = cfg_byte;
	  cmd_str[1] = voltage_byte_0;
	  cmd_str[2] = voltage_byte_1;
}


