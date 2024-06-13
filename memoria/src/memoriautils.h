#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <utils/catedra/inicio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "main.h"
#include <math.h>
#include <utils/funcionesUtiles.h>

extern int retardo;
extern int tam_pagina;
extern int tam_memoria;
extern t_list* tabla_pags;
extern void* memoria;
extern t_bitarray* bitarray;


void cargar_a_mem(char* instruccion, uint32_t pid);
int buscar_frame_disp(t_bitarray* bit, int tam);
char* cortar_string_final(char* cadena, int longitud);
char* cortar_string(char* cadena, int longitud);
bool buscar_por_pid_bool(uint32_t pid);
t_tabla* buscar_por_pid_return(uint32_t pid); 



#endif