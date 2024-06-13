#include "consola.h"

void consola_interactiva(){
   pthread_t consola;
   
    int resultado_creacion = pthread_create(&consola, NULL, leer_consola, NULL);
    if (resultado_creacion != 0) {
        fprintf(stderr, "Error: No se pudo crear el hilo. Código de error: %d\n", resultado_creacion);
    }
    pthread_detach(consola);
}

void* leer_consola() 
{
    char* leido;
    while (1) {  
        leido = readline("> ");
    if (string_is_empty(leido)) {
        break; 
        }
    funciones(leido);
    free(leido);
    }
    pthread_exit(NULL);
}

void funciones(char* leido) {
    char** funcion;
    funcion = string_split(leido, " ");
    if (string_equals_ignore_case(funcion[0], "EJECUTAR_SCRIPT") && string_array_size(funcion) == 2){
        ejecutar_script(funcion[1]);
    } else if (string_equals_ignore_case(funcion[0], "INICIAR_PROCESO") && string_array_size(funcion) == 2) {
        iniciar_proceso(funcion[1]);
    } else if (string_equals_ignore_case(funcion[0], "FINALIZAR_PROCESO") && string_array_size(funcion) == 2) {
        finalizar_proceso(atoi(funcion[1]));
    } else if (string_equals_ignore_case(funcion[0], "INICIAR_PLANIFICACION")) {
        iniciar_planificacion();
    } else if (string_equals_ignore_case(funcion[0], "DETENER_PLANIFICACION")) {
        detener_planificacion();
    } else if (string_equals_ignore_case(funcion[0], "MULTIPROGRAMACION") && string_array_size(funcion) == 2) {
        multiprogramacion(funcion[1]);
    } else if (string_equals_ignore_case(funcion[0], "PROCESO_ESTADO")) {
        proceso_estado();
    } else if (!string_is_empty(leido)){
        log_error(kernel_log, ">> COMANDO ERRONEO!");
    }
}


void ejecutar_script(char* path){
    log_info(kernel_log, ">> Se ejecuta el script %s", path);
}
void iniciar_proceso(char* path){

    t_pcb *pcb;

    static uint32_t pid_contador = 0;

    pcb = crear_nuevo_pcb(&pid_contador);
    queue_push(colaNew, pcb);
    //int grado_multiprog = config_get_int_value(kernel_config, "GRADO_MULTIPROGRAMACION");

    /*if(nivel_multiprog<grado_actual){
        queue_pop(colaNew);
        queue_push(colaReady, pcb);
        pcb->estado=READY;
        log_info(kernel_log,"Proceso con PID %u pasado a la cola READY",pcb->pid);
    }*/

    //sem_wait(&grado_actual);
    pcb = queue_pop(colaNew);
    queue_push(colaReady, pcb);
    //ver si se puede pasar a ready (nivel multiprog)
    pcb->estado=READY;
    log_info(kernel_log,"Proceso con PID %u pasado a la cola READY",pcb->pid);
    log_info(kernel_log, ">> Se crea el proceso %s en NEW", path);

    char* pathpid[_PC_PATH_MAX];
    strcpy(pathpid, path);
    strcat(pathpid, "$");
    strcat(pathpid, int_to_char(pcb->pid));
    enviar_mensaje(pathpid,conexion_memoria);

}

void finalizar_proceso(uint32_t pid){

    borrar_pcb(pid);
    char* pid_char= int_to_char(pid);
    //enviar_mensaje_finalizacion(pid_char,conexion_memoria);

    log_info(kernel_log, ">> Se finaliza proceso %u <<", pid);
}

bool tabla_pid;

void iniciar_planificacion(){
    char* algoritmo=config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");
    log_info(kernel_log, "El algoritmo configurado es: %s", algoritmo);
    if(strcmp(algoritmo, "FIFO") == 0){
        pthread_t alg_planificacion;
        pthread_create(&alg_planificacion, NULL, (void*) fifo,  conexion_cpu_dispatch);
        pthread_detach(alg_planificacion);

    } else if(strcmp(algoritmo, "RR") == 0){
        /*pthread_t alg_planificacion;
        pthread_create(&alg_planificacion, NULL, (void*) rr, conexion_cpu_dispatch);
        pthread_detach(alg_planificacion);*/
    }
    log_info(kernel_log, ">> Se inicio la planificacion");
}
void detener_planificacion(){
    log_info(kernel_log, ">> Se detiene la planificacion");
}

void multiprogramacion(char* valor){
    

    config_set_value(kernel_config, "GRADO_MULTIPROGRAMACION", valor);

    int cambio_ok = config_get_int_value(kernel_config, "GRADO_MULTIPROGRAMACION");

    if (cambio_ok == atoi(valor))
    {
        log_info(kernel_log, "Se cambio el grado de multiprogramacion a %s", valor);
        nivel_multiprog = cambio_ok;
    }
    else
    {
        log_warning(kernel_log, "No se pudo cambiar el grado de multiprogramacion");
    }

    log_info(kernel_log, ">> Se cambio el grado de multiprogamacion a %s", valor);
}

void proceso_estado(){

    log_info(kernel_log, ">> Listado de procesos por estado ...");
    for (t_proceso_estado estado = NEW; estado <= EXIT; estado++)
    {
        mostrar_pids_en_estado(estado);
    }
}
