#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alumnos.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de alumnos */
void menuAlumnos(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE ALUMNOS ===\n");
        printf("1. Listar alumnos\n");
        printf("2. Buscar alumno\n");
        if (usuario_actual.tipo == 1) {
            printf("3. Agregar alumno\n");
            printf("4. Editar alumno\n");
            printf("5. Eliminar alumno\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarAlumnos(); break;
            case 2: buscarAlumno(); break;
            case 3:
                if (usuario_actual.tipo == 1) agregarAlumno();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 4:
                if (usuario_actual.tipo == 1) editarAlumno();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 5:
                if (usuario_actual.tipo == 1) eliminarAlumno();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los alumnos activos */
void listarAlumnos() {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i;
    cargarAlumnos(alumnos, &total);
    limpiarPantalla();
    printf("=== LISTA DE ALUMNOS ===\n");
    printf("%-10s %-20s %-20s %-30s\n", "ID", "Nombre", "Apellido", "Email");
    printf("%-10s %-20s %-20s %-30s\n", "------", "------", "-------", "-----");
    for (i = 0; i < total; i++) {
        if (alumnos[i].activo) {
            printf("%-10s %-20s %-20s %-30s\n",
                   alumnos[i].id, alumnos[i].nombre, alumnos[i].apellido, alumnos[i].email);
        }
    }
    printf("\nTotal activos mostrados.\n");
    pausar();
}

/* Busca un alumno por ID o nombre */
void buscarAlumno() {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i, encontrado = 0;
    char busqueda[50];
    cargarAlumnos(alumnos, &total);
    leerString(busqueda, sizeof(busqueda), "Ingrese ID o nombre del alumno: ");
    limpiarPantalla();
    printf("=== RESULTADOS DE BUSQUEDA ===\n");
    for (i = 0; i < total; i++) {
        if (strcmp(alumnos[i].id, busqueda) == 0 ||
            strstr(alumnos[i].nombre, busqueda) != NULL ||
            strstr(alumnos[i].apellido, busqueda) != NULL) {
            printf("ID: %s\n", alumnos[i].id);
            printf("Nombre: %s %s\n", alumnos[i].nombre, alumnos[i].apellido);
            printf("Email: %s\n", alumnos[i].email);
            printf("Telefono: %s\n", alumnos[i].telefono);
            printf("Fecha Nacimiento: %s\n", alumnos[i].fecha_nacimiento);
            printf("Direccion: %s\n", alumnos[i].direccion);
            printf("Estado: %s\n", alumnos[i].activo ? "Activo" : "Inactivo");
            printf("---\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No se encontraron resultados.\n");
    pausar();
}

/* Agrega un nuevo alumno */
void agregarAlumno() {
    Alumno alumnos[MAX_ALUMNOS];
    int total;
    Alumno nuevo;
    memset(&nuevo, 0, sizeof(Alumno));

    cargarAlumnos(alumnos, &total);
    generarID(nuevo.id, "ALU", total + 1);

    printf("=== AGREGAR ALUMNO ===\n");
    printf("ID generado: %s\n", nuevo.id);
    leerString(nuevo.nombre, sizeof(nuevo.nombre), "Nombre: ");
    leerString(nuevo.apellido, sizeof(nuevo.apellido), "Apellido: ");
    leerString(nuevo.email, sizeof(nuevo.email), "Email: ");
    if (!validarEmail(nuevo.email)) {
        printf("Email invalido.\n");
        pausar();
        return;
    }
    leerString(nuevo.fecha_nacimiento, sizeof(nuevo.fecha_nacimiento), "Fecha nacimiento (YYYY-MM-DD): ");
    leerString(nuevo.telefono, sizeof(nuevo.telefono), "Telefono: ");
    leerString(nuevo.direccion, sizeof(nuevo.direccion), "Direccion: ");
    nuevo.activo = 1;

    alumnos[total] = nuevo;
    total++;
    guardarAlumnos(alumnos, total);
    printf("Alumno %s agregado exitosamente.\n", nuevo.id);
    pausar();
}

/* Edita un alumno existente */
void editarAlumno() {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i;
    char id_buscar[20];

    cargarAlumnos(alumnos, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del alumno a editar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(alumnos[i].id, id_buscar) == 0) {
            printf("=== EDITAR ALUMNO %s ===\n", alumnos[i].id);
            leerString(alumnos[i].nombre, sizeof(alumnos[i].nombre), "Nombre: ");
            leerString(alumnos[i].apellido, sizeof(alumnos[i].apellido), "Apellido: ");
            leerString(alumnos[i].email, sizeof(alumnos[i].email), "Email: ");
            leerString(alumnos[i].telefono, sizeof(alumnos[i].telefono), "Telefono: ");
            leerString(alumnos[i].direccion, sizeof(alumnos[i].direccion), "Direccion: ");
            guardarAlumnos(alumnos, total);
            printf("Alumno actualizado.\n");
            pausar();
            return;
        }
    }
    printf("Alumno no encontrado.\n");
    pausar();
}

/* Elimina (desactiva) un alumno */
void eliminarAlumno() {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i;
    char id_buscar[20];
    char confirmacion[5];

    cargarAlumnos(alumnos, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del alumno a eliminar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(alumnos[i].id, id_buscar) == 0) {
            printf("Desea eliminar a %s %s? (s/n): ", alumnos[i].nombre, alumnos[i].apellido);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
                alumnos[i].activo = 0;
                guardarAlumnos(alumnos, total);
                printf("Alumno desactivado.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Alumno no encontrado.\n");
    pausar();
}

/* Ver perfil del alumno logueado */
void verMiPerfil(const char id_alumno[]) {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i;
    cargarAlumnos(alumnos, &total);
    limpiarPantalla();
    for (i = 0; i < total; i++) {
        if (strcmp(alumnos[i].id, id_alumno) == 0) {
            printf("=== MI PERFIL ===\n");
            printf("ID: %s\n", alumnos[i].id);
            printf("Nombre: %s %s\n", alumnos[i].nombre, alumnos[i].apellido);
            printf("Email: %s\n", alumnos[i].email);
            printf("Telefono: %s\n", alumnos[i].telefono);
            printf("Fecha Nacimiento: %s\n", alumnos[i].fecha_nacimiento);
            printf("Direccion: %s\n", alumnos[i].direccion);
            pausar();
            return;
        }
    }
    printf("Perfil no encontrado.\n");
    pausar();
}
