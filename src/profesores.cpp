#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "profesores.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de profesores */
void menuProfesores(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE PROFESORES ===\n");
        printf("1. Listar profesores\n");
        printf("2. Buscar profesor\n");
        if (usuario_actual.tipo == 1) {
            printf("3. Agregar profesor\n");
            printf("4. Editar profesor\n");
            printf("5. Eliminar profesor\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarProfesores(); break;
            case 2: buscarProfesor(); break;
            case 3:
                if (usuario_actual.tipo == 1) agregarProfesor();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 4:
                if (usuario_actual.tipo == 1) editarProfesor();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 5:
                if (usuario_actual.tipo == 1) eliminarProfesor();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los profesores activos */
void listarProfesores() {
    Profesor profesores[MAX_PROFESORES];
    int total, i;
    cargarProfesores(profesores, &total);
    limpiarPantalla();
    printf("=== LISTA DE PROFESORES ===\n");
    printf("%-10s %-20s %-20s %-25s\n", "ID", "Nombre", "Apellido", "Especialidad");
    printf("%-10s %-20s %-20s %-25s\n", "------", "------", "-------", "------------");
    for (i = 0; i < total; i++) {
        if (profesores[i].activo) {
            printf("%-10s %-20s %-20s %-25s\n",
                   profesores[i].id, profesores[i].nombre,
                   profesores[i].apellido, profesores[i].especialidad);
        }
    }
    printf("\nTotal activos mostrados.\n");
    pausar();
}

/* Busca un profesor por ID o nombre */
void buscarProfesor() {
    Profesor profesores[MAX_PROFESORES];
    int total, i, encontrado = 0;
    char busqueda[50];
    cargarProfesores(profesores, &total);
    leerString(busqueda, sizeof(busqueda), "Ingrese ID o nombre del profesor: ");
    limpiarPantalla();
    printf("=== RESULTADOS DE BUSQUEDA ===\n");
    for (i = 0; i < total; i++) {
        if (strcmp(profesores[i].id, busqueda) == 0 ||
            strstr(profesores[i].nombre, busqueda) != NULL ||
            strstr(profesores[i].apellido, busqueda) != NULL) {
            printf("ID: %s\n", profesores[i].id);
            printf("Nombre: %s %s\n", profesores[i].nombre, profesores[i].apellido);
            printf("Email: %s\n", profesores[i].email);
            printf("Especialidad: %s\n", profesores[i].especialidad);
            printf("Telefono: %s\n", profesores[i].telefono);
            printf("Estado: %s\n", profesores[i].activo ? "Activo" : "Inactivo");
            printf("---\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No se encontraron resultados.\n");
    pausar();
}

/* Agrega un nuevo profesor */
void agregarProfesor() {
    Profesor profesores[MAX_PROFESORES];
    int total;
    Profesor nuevo;
    memset(&nuevo, 0, sizeof(Profesor));

    cargarProfesores(profesores, &total);
    generarID(nuevo.id, "PROF", total + 1);

    printf("=== AGREGAR PROFESOR ===\n");
    printf("ID generado: %s\n", nuevo.id);
    leerString(nuevo.nombre, sizeof(nuevo.nombre), "Nombre: ");
    leerString(nuevo.apellido, sizeof(nuevo.apellido), "Apellido: ");
    leerString(nuevo.email, sizeof(nuevo.email), "Email: ");
    if (!validarEmail(nuevo.email)) {
        printf("Email invalido.\n");
        pausar();
        return;
    }
    leerString(nuevo.especialidad, sizeof(nuevo.especialidad), "Especialidad: ");
    leerString(nuevo.telefono, sizeof(nuevo.telefono), "Telefono: ");
    nuevo.activo = 1;

    profesores[total] = nuevo;
    total++;
    guardarProfesores(profesores, total);
    printf("Profesor %s agregado exitosamente.\n", nuevo.id);
    pausar();
}

/* Edita un profesor existente */
void editarProfesor() {
    Profesor profesores[MAX_PROFESORES];
    int total, i;
    char id_buscar[20];

    cargarProfesores(profesores, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del profesor a editar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(profesores[i].id, id_buscar) == 0) {
            printf("=== EDITAR PROFESOR %s ===\n", profesores[i].id);
            leerString(profesores[i].nombre, sizeof(profesores[i].nombre), "Nombre: ");
            leerString(profesores[i].apellido, sizeof(profesores[i].apellido), "Apellido: ");
            leerString(profesores[i].email, sizeof(profesores[i].email), "Email: ");
            leerString(profesores[i].especialidad, sizeof(profesores[i].especialidad), "Especialidad: ");
            leerString(profesores[i].telefono, sizeof(profesores[i].telefono), "Telefono: ");
            guardarProfesores(profesores, total);
            printf("Profesor actualizado.\n");
            pausar();
            return;
        }
    }
    printf("Profesor no encontrado.\n");
    pausar();
}

/* Elimina (desactiva) un profesor */
void eliminarProfesor() {
    Profesor profesores[MAX_PROFESORES];
    int total, i;
    char id_buscar[20];
    char confirmacion[5];

    cargarProfesores(profesores, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del profesor a eliminar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(profesores[i].id, id_buscar) == 0) {
            printf("Desea eliminar a %s %s? (s/n): ", profesores[i].nombre, profesores[i].apellido);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
                profesores[i].activo = 0;
                guardarProfesores(profesores, total);
                printf("Profesor desactivado.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Profesor no encontrado.\n");
    pausar();
}
