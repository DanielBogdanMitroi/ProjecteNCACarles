#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matriculas.h"
#include "archivos.h"
#include "utilidades.h"

/* Menu principal de gestion de matriculas */
void menuMatriculas(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE MATRICULAS ===\n");
        printf("1. Listar matriculas\n");
        if (usuario_actual.tipo == 1) {
            printf("2. Agregar matricula\n");
            printf("3. Cancelar matricula\n");
            printf("4. Asignar profesor a materia\n");
        }
        printf("5. Ver matriculas de alumno\n");
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarMatriculas(); break;
            case 2: agregarMatricula(usuario_actual); break;
            case 3: cancelarMatricula(usuario_actual); break;
            case 4: asignarProfesorMateria(usuario_actual); break;
            case 5: {
                char id[20];
                leerString(id, sizeof(id), "ID del alumno: ");
                verMatriculasAlumno(id);
                break;
            }
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todas las matriculas activas */
void listarMatriculas() {
    Matricula matriculas[MAX_MATRICULAS];
    int total, i;
    cargarMatriculas(matriculas, &total);
    limpiarPantalla();
    printf("=== LISTA DE MATRICULAS ===\n");
    printf("%-12s %-10s %-10s %-11s %-10s\n",
           "ID", "Alumno", "Materia", "Fecha", "Periodo");
    printf("%-12s %-10s %-10s %-11s %-10s\n",
           "------", "------", "-------", "-----", "-------");
    for (i = 0; i < total; i++) {
        if (matriculas[i].activo) {
            printf("%-12s %-10s %-10s %-11s %-10s\n",
                   matriculas[i].id, matriculas[i].id_alumno,
                   matriculas[i].id_materia, matriculas[i].fecha_matricula,
                   matriculas[i].periodo);
        }
    }
    printf("\nTotal activas mostradas.\n");
    pausar();
}

/* Agrega una nueva matricula */
void agregarMatricula(Usuario usuario_actual) {
    Matricula matriculas[MAX_MATRICULAS];
    int total;
    Matricula nueva;
    memset(&nueva, 0, sizeof(Matricula));

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para agregar matriculas.\n");
        pausar();
        return;
    }

    cargarMatriculas(matriculas, &total);
    generarID(nueva.id, "MATRI", total + 1);

    printf("=== AGREGAR MATRICULA ===\n");
    printf("ID generado: %s\n", nueva.id);
    leerString(nueva.id_alumno, sizeof(nueva.id_alumno), "ID Alumno: ");
    leerString(nueva.id_materia, sizeof(nueva.id_materia), "ID Materia: ");
    leerString(nueva.fecha_matricula, sizeof(nueva.fecha_matricula), "Fecha matricula (YYYY-MM-DD): ");
    leerString(nueva.periodo, sizeof(nueva.periodo), "Periodo: ");
    nueva.activo = 1;

    matriculas[total] = nueva;
    total++;
    guardarMatriculas(matriculas, total);
    printf("Matricula %s agregada exitosamente.\n", nueva.id);
    pausar();
}

/* Cancela una matricula */
void cancelarMatricula(Usuario usuario_actual) {
    Matricula matriculas[MAX_MATRICULAS];
    int total, i;
    char id_buscar[20];
    char confirmacion[5];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para cancelar matriculas.\n");
        pausar();
        return;
    }

    cargarMatriculas(matriculas, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID de la matricula a cancelar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(matriculas[i].id, id_buscar) == 0) {
            printf("Confirmar cancelacion de matricula %s? (s/n): ", id_buscar);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
                matriculas[i].activo = 0;
                guardarMatriculas(matriculas, total);
                printf("Matricula cancelada.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Matricula no encontrada.\n");
    pausar();
}

/* Ver matriculas de un alumno especifico */
void verMatriculasAlumno(const char id_alumno[]) {
    Matricula matriculas[MAX_MATRICULAS];
    Materia materias[MAX_MATERIAS];
    int total_m, total_mat, i, j, encontrado = 0;
    cargarMatriculas(matriculas, &total_m);
    cargarMaterias(materias, &total_mat);
    limpiarPantalla();
    printf("=== MATERIAS DEL ALUMNO %s ===\n", id_alumno);
    printf("%-12s %-10s %-30s %-10s\n", "ID Matric.", "ID Mat.", "Nombre Materia", "Periodo");
    printf("%-12s %-10s %-30s %-10s\n", "----------", "-------", "-------------", "-------");
    for (i = 0; i < total_m; i++) {
        if (strcmp(matriculas[i].id_alumno, id_alumno) == 0 && matriculas[i].activo) {
            char nombre_mat[100] = "Desconocida";
            for (j = 0; j < total_mat; j++) {
                if (strcmp(materias[j].id, matriculas[i].id_materia) == 0) {
                    strcpy(nombre_mat, materias[j].nombre);
                    break;
                }
            }
            printf("%-12s %-10s %-30s %-10s\n",
                   matriculas[i].id, matriculas[i].id_materia,
                   nombre_mat, matriculas[i].periodo);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No hay matriculas para este alumno.\n");
    pausar();
}

/* Asigna un profesor a una materia para un periodo */
void asignarProfesorMateria(Usuario usuario_actual) {
    Profesor_Materia pm[MAX_PROFESOR_MATERIA];
    int total;
    Profesor_Materia nuevo;
    memset(&nuevo, 0, sizeof(Profesor_Materia));

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para asignar profesores.\n");
        pausar();
        return;
    }

    cargarProfesorMaterias(pm, &total);
    generarID(nuevo.id, "PM", total + 1);

    printf("=== ASIGNAR PROFESOR A MATERIA ===\n");
    printf("ID generado: %s\n", nuevo.id);
    leerString(nuevo.id_profesor, sizeof(nuevo.id_profesor), "ID Profesor: ");
    leerString(nuevo.id_materia, sizeof(nuevo.id_materia), "ID Materia: ");
    leerString(nuevo.periodo, sizeof(nuevo.periodo), "Periodo: ");
    nuevo.activo = 1;

    pm[total] = nuevo;
    total++;
    guardarProfesorMaterias(pm, total);
    printf("Asignacion %s registrada exitosamente.\n", nuevo.id);
    pausar();
}
