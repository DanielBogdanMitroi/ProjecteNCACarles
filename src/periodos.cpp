#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "periodos.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de periodos */
void menuPeriodos(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE PERIODOS ===\n");
        printf("1. Listar periodos\n");
        if (usuario_actual.tipo == 1) {
            printf("2. Agregar periodo\n");
            printf("3. Editar periodo\n");
            printf("4. Eliminar periodo\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarPeriodos(); break;
            case 2: agregarPeriodo(usuario_actual); break;
            case 3: editarPeriodo(usuario_actual); break;
            case 4: eliminarPeriodo(usuario_actual); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los periodos */
void listarPeriodos() {
    Periodo periodos[MAX_PERIODOS];
    int total, i;
    cargarPeriodos(periodos, &total);
    limpiarPantalla();
    printf("=== LISTA DE PERIODOS ===\n");
    printf("%-10s %-30s %-11s %-11s %-8s\n",
           "ID", "Nombre", "Inicio", "Fin", "Estado");
    printf("%-10s %-30s %-11s %-11s %-8s\n",
           "------", "------", "------", "---", "------");
    for (i = 0; i < total; i++) {
        printf("%-10s %-30s %-11s %-11s %-8s\n",
               periodos[i].id, periodos[i].nombre,
               periodos[i].fecha_inicio, periodos[i].fecha_fin,
               periodos[i].activo ? "Activo" : "Inactivo");
    }
    printf("\nTotal: %d periodos.\n", total);
    pausar();
}

/* Agrega un nuevo periodo */
void agregarPeriodo(Usuario usuario_actual) {
    Periodo periodos[MAX_PERIODOS];
    int total;
    Periodo nuevo;
    memset(&nuevo, 0, sizeof(Periodo));

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para agregar periodos.\n");
        pausar();
        return;
    }

    cargarPeriodos(periodos, &total);
    generarID(nuevo.id, "PER", total + 1);

    printf("=== AGREGAR PERIODO ===\n");
    printf("ID generado: %s\n", nuevo.id);
    leerString(nuevo.nombre, sizeof(nuevo.nombre), "Nombre: ");
    leerString(nuevo.fecha_inicio, sizeof(nuevo.fecha_inicio), "Fecha inicio (YYYY-MM-DD): ");
    if (!validarFecha(nuevo.fecha_inicio)) {
        printf("Fecha de inicio invalida.\n");
        pausar();
        return;
    }
    leerString(nuevo.fecha_fin, sizeof(nuevo.fecha_fin), "Fecha fin (YYYY-MM-DD): ");
    if (!validarFecha(nuevo.fecha_fin)) {
        printf("Fecha de fin invalida.\n");
        pausar();
        return;
    }
    nuevo.activo = 1;

    periodos[total] = nuevo;
    total++;
    guardarPeriodos(periodos, total);
    printf("Periodo %s agregado exitosamente.\n", nuevo.id);
    pausar();
}

/* Edita un periodo existente */
void editarPeriodo(Usuario usuario_actual) {
    Periodo periodos[MAX_PERIODOS];
    int total, i;
    char id_buscar[20];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para editar periodos.\n");
        pausar();
        return;
    }

    cargarPeriodos(periodos, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del periodo a editar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(periodos[i].id, id_buscar) == 0) {
            printf("=== EDITAR PERIODO %s ===\n", periodos[i].id);
            leerString(periodos[i].nombre, sizeof(periodos[i].nombre), "Nombre: ");
            leerString(periodos[i].fecha_inicio, sizeof(periodos[i].fecha_inicio), "Fecha inicio (YYYY-MM-DD): ");
            leerString(periodos[i].fecha_fin, sizeof(periodos[i].fecha_fin), "Fecha fin (YYYY-MM-DD): ");
            leerEntero(&periodos[i].activo, "Activo (1=Si, 0=No): ");
            guardarPeriodos(periodos, total);
            printf("Periodo actualizado.\n");
            pausar();
            return;
        }
    }
    printf("Periodo no encontrado.\n");
    pausar();
}

/* Elimina (desactiva) un periodo */
void eliminarPeriodo(Usuario usuario_actual) {
    Periodo periodos[MAX_PERIODOS];
    int total, i;
    char id_buscar[20];
    char confirmacion[5];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para eliminar periodos.\n");
        pausar();
        return;
    }

    cargarPeriodos(periodos, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del periodo a eliminar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(periodos[i].id, id_buscar) == 0) {
            printf("Desea eliminar el periodo %s? (s/n): ", periodos[i].nombre);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
                periodos[i].activo = 0;
                guardarPeriodos(periodos, total);
                printf("Periodo desactivado.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Periodo no encontrado.\n");
    pausar();
}
