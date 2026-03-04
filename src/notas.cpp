#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "notas.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de notas */
void menuNotas(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE NOTAS ===\n");
        printf("1. Listar notas\n");
        if (usuario_actual.tipo == 1 || usuario_actual.tipo == 2) {
            printf("2. Agregar nota\n");
            printf("3. Editar nota\n");
            printf("4. Eliminar nota\n");
        }
        printf("5. Ver notas por alumno\n");
        printf("6. Ver notas por materia\n");
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarNotas(); break;
            case 2: agregarNota(usuario_actual); break;
            case 3: editarNota(usuario_actual); break;
            case 4: eliminarNota(usuario_actual); break;
            case 5: {
                char id[20];
                leerString(id, sizeof(id), "ID del alumno: ");
                verNotasAlumno(id);
                break;
            }
            case 6: {
                char id[20];
                leerString(id, sizeof(id), "ID de la materia: ");
                verNotasPorMateria(id);
                break;
            }
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todas las notas */
void listarNotas() {
    Nota notas[MAX_NOTAS];
    int total, i;
    cargarNotas(notas, &total);
    limpiarPantalla();
    printf("=== LISTA DE NOTAS ===\n");
    printf("Total de registros en archivo: %d\n", total);
    printf("%-10s %-10s %-10s %-8s %-15s %-11s\n",
           "ID", "Alumno", "Materia", "Nota", "Tipo", "Fecha");
    printf("%-10s %-10s %-10s %-8s %-15s %-11s\n",
           "------", "------", "-------", "----", "----", "-----");
    for (i = 0; i < total; i++) {
        printf("%-10s %-10s %-10s %-8.2f %-15s %-11s\n",
               notas[i].id, notas[i].id_alumno, notas[i].id_materia,
               notas[i].calificacion, notas[i].tipo, notas[i].fecha);
    }
    if (total == 0) {
        printf("\nNo hay notas registradas en el sistema.\n");
        printf("Usa la opcion 'Agregar nota' para crear el primer registro.\n");
    } else {
        printf("\nTotal: %d notas.\n", total);
    }
    pausar();
}

/* Agrega una nueva nota */
void agregarNota(Usuario usuario_actual) {
    Nota notas[MAX_NOTAS];
    int total;
    Nota nueva;
    int fecha_valida;
    memset(&nueva, 0, sizeof(Nota));

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para agregar notas.\n");
        pausar();
        return;
    }

    cargarNotas(notas, &total);
    generarID(nueva.id, "NOTA", total + 1);

    printf("=== AGREGAR NOTA ===\n");
    printf("ID generado: %s\n", nueva.id);

    do {
        leerString(nueva.id_alumno, sizeof(nueva.id_alumno), "ID Alumno: ");
        if (!validarCadenaNoVacia(nueva.id_alumno, 2)) {
            printf("Error: El ID de alumno no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(nueva.id_alumno, 2));

    do {
        leerString(nueva.id_materia, sizeof(nueva.id_materia), "ID Materia: ");
        if (!validarCadenaNoVacia(nueva.id_materia, 2)) {
            printf("Error: El ID de materia no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(nueva.id_materia, 2));

    do {
        leerFloat(&nueva.calificacion, "Calificacion (0-10): ");
        if (!validarNota(nueva.calificacion)) {
            printf("Error: Calificacion invalida (0-10).\n");
        }
    } while (!validarNota(nueva.calificacion));

    do {
        leerString(nueva.tipo, sizeof(nueva.tipo), "Tipo (Examen/Trabajo/Practica): ");
        if (!validarCadenaNoVacia(nueva.tipo, 3)) {
            printf("Error: El tipo debe tener al menos 3 caracteres.\n");
        }
    } while (!validarCadenaNoVacia(nueva.tipo, 3));

    leerString(nueva.descripcion, sizeof(nueva.descripcion), "Descripcion: ");

    fecha_valida = 0;
    do {
        leerString(nueva.fecha, sizeof(nueva.fecha), "Fecha (YYYY-MM-DD): ");
        if (!validarFecha(nueva.fecha)) {
            printf("Error: Fecha invalida. Formato: YYYY-MM-DD\n");
        } else {
            fecha_valida = 1;
        }
    } while (!fecha_valida);

    leerString(nueva.periodo, sizeof(nueva.periodo), "Periodo: ");

    notas[total] = nueva;
    total++;
    guardarNotas(notas, total);
    printf("Nota %s agregada exitosamente.\n", nueva.id);
    pausar();
}

/* Edita una nota existente */
void editarNota(Usuario usuario_actual) {
    Nota notas[MAX_NOTAS];
    int total, i;
    char id_buscar[20];

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para editar notas.\n");
        pausar();
        return;
    }

    cargarNotas(notas, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID de la nota a editar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(notas[i].id, id_buscar) == 0) {
            printf("=== EDITAR NOTA %s ===\n", notas[i].id);

            do {
                leerFloat(&notas[i].calificacion, "Nueva calificacion (0-10): ");
                if (!validarNota(notas[i].calificacion)) {
                    printf("Error: Calificacion invalida (0-10).\n");
                }
            } while (!validarNota(notas[i].calificacion));

            do {
                leerString(notas[i].tipo, sizeof(notas[i].tipo), "Tipo: ");
                if (!validarCadenaNoVacia(notas[i].tipo, 3)) {
                    printf("Error: El tipo debe tener al menos 3 caracteres.\n");
                }
            } while (!validarCadenaNoVacia(notas[i].tipo, 3));

            leerString(notas[i].descripcion, sizeof(notas[i].descripcion), "Descripcion: ");
            guardarNotas(notas, total);
            printf("Nota actualizada.\n");
            pausar();
            return;
        }
    }
    printf("Nota no encontrada.\n");
    pausar();
}

/* Elimina una nota */
void eliminarNota(Usuario usuario_actual) {
    Nota notas[MAX_NOTAS];
    int total, i, j;
    char id_buscar[20];
    char confirmacion[5];

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para eliminar notas.\n");
        pausar();
        return;
    }

    cargarNotas(notas, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID de la nota a eliminar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(notas[i].id, id_buscar) == 0) {
            printf("Confirmar eliminacion de nota %s? (s/n): ", id_buscar);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
                /* Desplazar elementos hacia atras */
                for (j = i; j < total - 1; j++) {
                    notas[j] = notas[j + 1];
                }
                total--;
                guardarNotas(notas, total);
                printf("Nota eliminada.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Nota no encontrada.\n");
    pausar();
}

/* Ver notas de un alumno especifico */
void verNotasAlumno(const char id_alumno[]) {
    Nota notas[MAX_NOTAS];
    int total, i, encontrado = 0;
    float suma = 0.0f;
    int cant = 0;
    cargarNotas(notas, &total);
    limpiarPantalla();
    printf("=== NOTAS DEL ALUMNO %s ===\n", id_alumno);
    printf("%-10s %-10s %-8s %-15s %-30s %-11s\n",
           "ID", "Materia", "Nota", "Tipo", "Descripcion", "Fecha");
    printf("%-10s %-10s %-8s %-15s %-30s %-11s\n",
           "------", "-------", "----", "----", "-----------", "-----");
    for (i = 0; i < total; i++) {
        if (strcmp(notas[i].id_alumno, id_alumno) == 0) {
            printf("%-10s %-10s %-8.2f %-15s %-30s %-11s\n",
                   notas[i].id, notas[i].id_materia, notas[i].calificacion,
                   notas[i].tipo, notas[i].descripcion, notas[i].fecha);
            suma += notas[i].calificacion;
            cant++;
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("No hay notas para este alumno.\n");
    } else {
        printf("\nPromedio general: %.2f\n", suma / cant);
    }
    pausar();
}

/* Ver notas de una materia especifica */
void verNotasPorMateria(const char id_materia[]) {
    Nota notas[MAX_NOTAS];
    int total, i, encontrado = 0;
    float suma = 0.0f;
    int cant = 0;
    cargarNotas(notas, &total);
    limpiarPantalla();
    printf("=== NOTAS DE MATERIA %s ===\n", id_materia);
    printf("%-10s %-10s %-8s %-15s %-11s\n",
           "ID", "Alumno", "Nota", "Tipo", "Fecha");
    printf("%-10s %-10s %-8s %-15s %-11s\n",
           "------", "------", "----", "----", "-----");
    for (i = 0; i < total; i++) {
        if (strcmp(notas[i].id_materia, id_materia) == 0) {
            printf("%-10s %-10s %-8.2f %-15s %-11s\n",
                   notas[i].id, notas[i].id_alumno, notas[i].calificacion,
                   notas[i].tipo, notas[i].fecha);
            suma += notas[i].calificacion;
            cant++;
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("No hay notas para esta materia.\n");
    } else {
        printf("\nPromedio de la materia: %.2f\n", suma / cant);
    }
    pausar();
}
