#ifndef CICLOINST_H_
#define CICLOINST_H_
#include "main.h"
#include <utils/mensajesPropios.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <utils/io_operation.h>


typedef struct t_decode {
instrucciones op_code;
t_list* registroCpu;
int valor;
char* recurso;
char* interfaz;
bool logicaAFisica;
}t_decode;

typedef struct {
    const char* nombre;
    void* puntero;
    size_t tamano; 
} registro_mapa;


t_instruccion* fetch(t_pcb* pcb, int conexion_memoria);
t_decode* decode(t_instruccion* ins);
instrucciones obtener_instruccion(char *nombre);
// Momentaneo

#endif