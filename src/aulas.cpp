#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aulas.h"
#include "archivos.h"
#include "utilidades.h"

/* Menu principal de gestion de aulas */
void menuAulas(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE AULAS ===\n");
        printf("1. Listar aulas\n");
        if (usuario_actual.tipo == 1) {
            printf("2. Agregar aula\n");
            printf("3. Editar aula\n");
            printf("4. Eliminar aula\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarAulas(); break;
            case 2: agregarAula(usuario_actual); break;
            case 3: editarAula(usuario_actual); break;
            case 4: eliminarAula(usuario_actual); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todas las aulas activas */
void listarAulas() {
    Aula aulas[MAX_AULAS];
    int total, i;
    cargarAulas(aulas, &total);
    limpiarPantalla();
    printf("=== LISTA DE AULAS ===\n");
    printf("%-10s %-20s %-10s %-15s\n", "ID", "Nombre", "Capacidad", "Tipo");
    printf("%-10s %-20s %-10s %-15s\n", "------", "------", "---------", "----");
    for (i = 0; i < total; i++) {
        if (aulas[i].activo) {
            printf("%-10s %-20s %-10d %-15s\n",
                   aulas[i].id, aulas[i].nombre, aulas[i].capacidad, aulas[i].tipo);
        }
    }
    printf("\nTotal activas mostradas.\n");
    pausar();
}

/* Agrega una nueva aula */
void agregarAula(Usuario usuario_actual) {
    Aula aulas[MAX_AULAS];
    int total;
    Aula nueva;
    memset(&nueva, 0, sizeof(Aula));

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para agregar aulas.\n");
        pausar();
        return;
    }

    cargarAulas(aulas, &total);
    generarID(nueva.id, "AULA", total + 1);

    printf("=== AGREGAR AULA ===\n");
    printf("ID generado: %s\n", nueva.id);
    leerString(nueva.nombre, sizeof(nueva.nombre), "Nombre: ");
    leerEntero(&nueva.capacidad, "Capacidad: ");
    leerString(nueva.tipo, sizeof(nueva.tipo), "Tipo (Teoria/Laboratorio): ");
    nueva.activo = 1;

    aulas[total] = nueva;
    total++;
    guardarAulas(aulas, total);
    printf("Aula %s agregada exitosamente.\n", nueva.id);
    pausar();
}

/* Edita un aula existente */
void editarAula(Usuario usuario_actual) {
    Aula aulas[MAX_AULAS];
    int total, i;
    char id_buscar[20];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para editar aulas.\n");
        pausar();
        return;
    }

    cargarAulas(aulas, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del aula a editar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(aulas[i].id, id_buscar) == 0) {
            printf("=== EDITAR AULA %s ===\n", aulas[i].id);
            leerString(aulas[i].nombre, sizeof(aulas[i].nombre), "Nombre: ");
            leerEntero(&aulas[i].capacidad, "Capacidad: ");
            leerString(aulas[i].tipo, sizeof(aulas[i].tipo), "Tipo: ");
            guardarAulas(aulas, total);
            printf("Aula actualizada.\n");
            pausar();
            return;
        }
    }
    printf("Aula no encontrada.\n");
    pausar();
}

/* Elimina (desactiva) un aula */
void eliminarAula(Usuario usuario_actual) {
    Aula aulas[MAX_AULAS];
    int total, i;
    char id_buscar[20];
    char confirmacion[5];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para eliminar aulas.\n");
        pausar();
        return;
    }

    cargarAulas(aulas, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del aula a eliminar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(aulas[i].id, id_buscar) == 0) {
            printf("Desea eliminar el aula %s? (s/n): ", aulas[i].nombre);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
                aulas[i].activo = 0;
                guardarAulas(aulas, total);
                printf("Aula desactivada.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Aula no encontrada.\n");
    pausar();
}
