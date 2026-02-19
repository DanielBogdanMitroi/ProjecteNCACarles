#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "materias.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de materias */
void menuMaterias(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE MATERIAS ===\n");
        printf("1. Listar materias\n");
        printf("2. Buscar materia\n");
        if (usuario_actual.tipo == 1) {
            printf("3. Agregar materia\n");
            printf("4. Editar materia\n");
            printf("5. Eliminar materia\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarMaterias(); break;
            case 2: buscarMateria(); break;
            case 3:
                if (usuario_actual.tipo == 1) agregarMateria();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 4:
                if (usuario_actual.tipo == 1) editarMateria();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 5:
                if (usuario_actual.tipo == 1) eliminarMateria();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todas las materias activas */
void listarMaterias() {
    Materia materias[MAX_MATERIAS];
    int total, i;
    cargarMaterias(materias, &total);
    limpiarPantalla();
    printf("=== LISTA DE MATERIAS ===\n");
    printf("%-10s %-30s %-10s %-15s\n", "ID", "Nombre", "Creditos", "Horas/Semana");
    printf("%-10s %-30s %-10s %-15s\n", "------", "------", "--------", "------------");
    for (i = 0; i < total; i++) {
        if (materias[i].activo) {
            printf("%-10s %-30s %-10d %-15d\n",
                   materias[i].id, materias[i].nombre,
                   materias[i].creditos, materias[i].horas_semanales);
        }
    }
    printf("\nTotal activas mostradas.\n");
    pausar();
}

/* Busca una materia por ID o nombre */
void buscarMateria() {
    Materia materias[MAX_MATERIAS];
    int total, i, encontrado = 0;
    char busqueda[50];
    cargarMaterias(materias, &total);
    leerString(busqueda, sizeof(busqueda), "Ingrese ID o nombre de la materia: ");
    limpiarPantalla();
    printf("=== RESULTADOS DE BUSQUEDA ===\n");
    for (i = 0; i < total; i++) {
        if (strcmp(materias[i].id, busqueda) == 0 ||
            strstr(materias[i].nombre, busqueda) != NULL) {
            printf("ID: %s\n", materias[i].id);
            printf("Nombre: %s\n", materias[i].nombre);
            printf("Descripcion: %s\n", materias[i].descripcion);
            printf("Creditos: %d\n", materias[i].creditos);
            printf("Horas semanales: %d\n", materias[i].horas_semanales);
            printf("Estado: %s\n", materias[i].activo ? "Activa" : "Inactiva");
            printf("---\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No se encontraron resultados.\n");
    pausar();
}

/* Agrega una nueva materia */
void agregarMateria() {
    Materia materias[MAX_MATERIAS];
    int total;
    Materia nueva;
    memset(&nueva, 0, sizeof(Materia));

    cargarMaterias(materias, &total);
    generarID(nueva.id, "MAT", total + 1);

    printf("=== AGREGAR MATERIA ===\n");
    printf("ID generado: %s\n", nueva.id);
    leerString(nueva.nombre, sizeof(nueva.nombre), "Nombre: ");
    leerString(nueva.descripcion, sizeof(nueva.descripcion), "Descripcion: ");
    leerEntero(&nueva.creditos, "Creditos: ");
    if (!validarCreditos(nueva.creditos)) {
        printf("Numero de creditos invalido (1-20).\n");
        pausar();
        return;
    }
    leerEntero(&nueva.horas_semanales, "Horas semanales: ");
    nueva.activo = 1;

    materias[total] = nueva;
    total++;
    guardarMaterias(materias, total);
    printf("Materia %s agregada exitosamente.\n", nueva.id);
    pausar();
}

/* Edita una materia existente */
void editarMateria() {
    Materia materias[MAX_MATERIAS];
    int total, i;
    char id_buscar[20];

    cargarMaterias(materias, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID de la materia a editar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(materias[i].id, id_buscar) == 0) {
            printf("=== EDITAR MATERIA %s ===\n", materias[i].id);
            leerString(materias[i].nombre, sizeof(materias[i].nombre), "Nombre: ");
            leerString(materias[i].descripcion, sizeof(materias[i].descripcion), "Descripcion: ");
            leerEntero(&materias[i].creditos, "Creditos: ");
            leerEntero(&materias[i].horas_semanales, "Horas semanales: ");
            guardarMaterias(materias, total);
            printf("Materia actualizada.\n");
            pausar();
            return;
        }
    }
    printf("Materia no encontrada.\n");
    pausar();
}

/* Elimina (desactiva) una materia */
void eliminarMateria() {
    Materia materias[MAX_MATERIAS];
    int total, i;
    char id_buscar[20];
    char confirmacion[5];

    cargarMaterias(materias, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID de la materia a eliminar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(materias[i].id, id_buscar) == 0) {
            printf("Desea eliminar la materia %s? (s/n): ", materias[i].nombre);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
                materias[i].activo = 0;
                guardarMaterias(materias, total);
                printf("Materia desactivada.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Materia no encontrada.\n");
    pausar();
}
