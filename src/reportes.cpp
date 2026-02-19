#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reportes.h"
#include "archivos.h"
#include "utilidades.h"

/* Menu de reportes */
void menuReportes(Usuario usuario_actual) {
    int opcion;
    (void)usuario_actual;
    do {
        limpiarPantalla();
        printf("=== MENU DE REPORTES ===\n");
        printf("1. Notas de un alumno\n");
        printf("2. Asistencia general\n");
        printf("3. Promedios por materia\n");
        printf("4. Alumnos por materia\n");
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: reporteNotasAlumno(); break;
            case 2: reporteAsistenciaGeneral(); break;
            case 3: reportePromediosPorMateria(); break;
            case 4: reporteAlumnosMateria(); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Reporte de notas de un alumno especifico */
void reporteNotasAlumno() {
    Nota notas[MAX_NOTAS];
    Alumno alumnos[MAX_ALUMNOS];
    Materia materias[MAX_MATERIAS];
    int total_n, total_a, total_m, i, j;
    char id_alumno[20];
    char nombre_alumno[110] = "";
    float suma = 0.0f;
    int cnt = 0;

    leerString(id_alumno, sizeof(id_alumno), "ID del alumno: ");
    cargarNotas(notas, &total_n);
    cargarAlumnos(alumnos, &total_a);
    cargarMaterias(materias, &total_m);

    /* Buscar nombre del alumno */
    for (j = 0; j < total_a; j++) {
        if (strcmp(alumnos[j].id, id_alumno) == 0) {
            sprintf(nombre_alumno, "%s %s", alumnos[j].nombre, alumnos[j].apellido);
            break;
        }
    }

    limpiarPantalla();
    printf("=== REPORTE DE NOTAS ===\n");
    printf("Alumno: %s (%s)\n\n", nombre_alumno, id_alumno);
    printf("%-10s %-30s %-8s %-15s %-11s\n",
           "ID", "Materia", "Nota", "Tipo", "Fecha");
    printf("%-10s %-30s %-8s %-15s %-11s\n",
           "------", "-------", "----", "----", "-----");

    for (i = 0; i < total_n; i++) {
        if (strcmp(notas[i].id_alumno, id_alumno) == 0) {
            char nombre_mat[100] = "Desconocida";
            for (j = 0; j < total_m; j++) {
                if (strcmp(materias[j].id, notas[i].id_materia) == 0) {
                    strcpy(nombre_mat, materias[j].nombre);
                    break;
                }
            }
            printf("%-10s %-30s %-8.2f %-15s %-11s\n",
                   notas[i].id, nombre_mat, notas[i].calificacion,
                   notas[i].tipo, notas[i].fecha);
            suma += notas[i].calificacion;
            cnt++;
        }
    }
    if (cnt == 0) {
        printf("No hay notas registradas.\n");
    } else {
        printf("\nTotal notas: %d | Promedio: %.2f\n", cnt, suma / cnt);
    }
    pausar();
}

/* Reporte general de asistencia */
void reporteAsistenciaGeneral() {
    Asistencia asistencias[MAX_ASISTENCIAS];
    Alumno alumnos[MAX_ALUMNOS];
    Materia materias[MAX_MATERIAS];
    int total_asis, total_a, total_m, i, j, k;

    cargarAsistencias(asistencias, &total_asis);
    cargarAlumnos(alumnos, &total_a);
    cargarMaterias(materias, &total_m);

    limpiarPantalla();
    printf("=== REPORTE GENERAL DE ASISTENCIA ===\n\n");
    printf("%-25s %-20s %-8s %-8s %-10s\n",
           "Alumno", "Materia", "Total", "Pres.", "Porcentaje");
    printf("%-25s %-20s %-8s %-8s %-10s\n",
           "------", "-------", "-----", "-----", "----------");

    for (j = 0; j < total_a; j++) {
        if (!alumnos[j].activo) continue;
        for (k = 0; k < total_m; k++) {
            if (!materias[k].activo) continue;
            int tot = 0, pres = 0;
            for (i = 0; i < total_asis; i++) {
                if (strcmp(asistencias[i].id_alumno, alumnos[j].id) == 0 &&
                    strcmp(asistencias[i].id_materia, materias[k].id) == 0) {
                    tot++;
                    if (asistencias[i].presente) pres++;
                }
            }
            if (tot > 0) {
                char nombre_alu[110];
                sprintf(nombre_alu, "%s %s", alumnos[j].nombre, alumnos[j].apellido);
                float pct = (float)pres / tot * 100.0f;
                printf("%-25s %-20s %-8d %-8d %-9.1f%%\n",
                       nombre_alu, materias[k].nombre, tot, pres, pct);
            }
        }
    }
    pausar();
}

/* Reporte de promedios por materia */
void reportePromediosPorMateria() {
    Nota notas[MAX_NOTAS];
    Materia materias[MAX_MATERIAS];
    int total_n, total_m, i, j;

    cargarNotas(notas, &total_n);
    cargarMaterias(materias, &total_m);

    limpiarPantalla();
    printf("=== PROMEDIOS POR MATERIA ===\n\n");
    printf("%-10s %-30s %-10s %-10s\n", "ID", "Materia", "Cant.Notas", "Promedio");
    printf("%-10s %-30s %-10s %-10s\n", "------", "-------", "----------", "--------");

    for (j = 0; j < total_m; j++) {
        if (!materias[j].activo) continue;
        float suma = 0.0f;
        int cnt = 0;
        for (i = 0; i < total_n; i++) {
            if (strcmp(notas[i].id_materia, materias[j].id) == 0) {
                suma += notas[i].calificacion;
                cnt++;
            }
        }
        if (cnt > 0) {
            printf("%-10s %-30s %-10d %-10.2f\n",
                   materias[j].id, materias[j].nombre, cnt, suma / cnt);
        } else {
            printf("%-10s %-30s %-10d %-10s\n",
                   materias[j].id, materias[j].nombre, 0, "Sin notas");
        }
    }
    pausar();
}

/* Reporte de alumnos matriculados en una materia */
void reporteAlumnosMateria() {
    Matricula matriculas[MAX_MATRICULAS];
    Alumno alumnos[MAX_ALUMNOS];
    int total_mat, total_a, i, j;
    char id_materia[20];

    leerString(id_materia, sizeof(id_materia), "ID de la materia: ");
    cargarMatriculas(matriculas, &total_mat);
    cargarAlumnos(alumnos, &total_a);

    limpiarPantalla();
    printf("=== ALUMNOS EN MATERIA %s ===\n\n", id_materia);
    printf("%-10s %-25s %-30s\n", "ID", "Nombre", "Email");
    printf("%-10s %-25s %-30s\n", "------", "------", "-----");

    int cnt = 0;
    for (i = 0; i < total_mat; i++) {
        if (strcmp(matriculas[i].id_materia, id_materia) == 0 && matriculas[i].activo) {
            for (j = 0; j < total_a; j++) {
                if (strcmp(alumnos[j].id, matriculas[i].id_alumno) == 0) {
                    char nombre[110];
                    sprintf(nombre, "%s %s", alumnos[j].nombre, alumnos[j].apellido);
                    printf("%-10s %-25s %-30s\n",
                           alumnos[j].id, nombre, alumnos[j].email);
                    cnt++;
                    break;
                }
            }
        }
    }
    printf("\nTotal alumnos matriculados: %d\n", cnt);
    pausar();
}
