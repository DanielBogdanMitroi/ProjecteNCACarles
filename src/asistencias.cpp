#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asistencias.h"
#include "archivos.h"
#include "utilidades.h"

/* Menu principal de gestion de asistencias */
void menuAsistencias(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE ASISTENCIAS ===\n");
        if (usuario_actual.tipo == 1 || usuario_actual.tipo == 2) {
            printf("1. Registrar asistencia\n");
        }
        printf("2. Ver asistencias por alumno\n");
        printf("3. Ver asistencias por materia\n");
        printf("4. Reporte de asistencias\n");
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: registrarAsistencia(usuario_actual); break;
            case 2: {
                char id[20];
                leerString(id, sizeof(id), "ID del alumno: ");
                verAsistenciasAlumno(id);
                break;
            }
            case 3: {
                char id[20];
                leerString(id, sizeof(id), "ID de la materia: ");
                verAsistenciasPorMateria(id);
                break;
            }
            case 4: reporteAsistencias(); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Registra la asistencia de un alumno */
void registrarAsistencia(Usuario usuario_actual) {
    Asistencia asistencias[MAX_ASISTENCIAS];
    int total;
    Asistencia nueva;
    memset(&nueva, 0, sizeof(Asistencia));

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para registrar asistencia.\n");
        pausar();
        return;
    }

    cargarAsistencias(asistencias, &total);
    generarID(nueva.id, "ASIS", obtenerSiguienteNumeroID(ARCHIVO_ASISTENCIAS, "ASIS"));

    printf("=== REGISTRAR ASISTENCIA ===\n");
    printf("ID generado: %s\n", nueva.id);
    leerString(nueva.id_alumno, sizeof(nueva.id_alumno), "ID Alumno: ");
    leerString(nueva.id_materia, sizeof(nueva.id_materia), "ID Materia: ");
    leerString(nueva.fecha, sizeof(nueva.fecha), "Fecha (YYYY-MM-DD): ");
    leerEntero(&nueva.presente, "Presente (1=Si, 0=No): ");
    leerString(nueva.observaciones, sizeof(nueva.observaciones), "Observaciones (Enter para ninguna): ");

    asistencias[total] = nueva;
    total++;
    guardarAsistencias(asistencias, total);
    printf("Asistencia %s registrada exitosamente.\n", nueva.id);
    pausar();
}

/* Ver asistencias de un alumno especifico */
void verAsistenciasAlumno(const char id_alumno[]) {
    Asistencia asistencias[MAX_ASISTENCIAS];
    int total, i, presentes = 0, ausentes = 0, encontrado = 0;
    cargarAsistencias(asistencias, &total);
    limpiarPantalla();
    printf("=== ASISTENCIAS DEL ALUMNO %s ===\n", id_alumno);
    printf("%-10s %-10s %-11s %-10s\n", "ID", "Materia", "Fecha", "Estado");
    printf("%-10s %-10s %-11s %-10s\n", "------", "-------", "-----", "------");
    for (i = 0; i < total; i++) {
        if (strcmp(asistencias[i].id_alumno, id_alumno) == 0) {
            printf("%-10s %-10s %-11s %-10s\n",
                   asistencias[i].id, asistencias[i].id_materia,
                   asistencias[i].fecha,
                   asistencias[i].presente ? "Presente" : "Ausente");
            if (asistencias[i].presente) presentes++;
            else ausentes++;
            encontrado = 1;
        }
    }
    if (!encontrado) {
        printf("No hay registros de asistencia para este alumno.\n");
    } else {
        int total_reg = presentes + ausentes;
        printf("\nTotal clases: %d | Presentes: %d | Ausentes: %d\n",
               total_reg, presentes, ausentes);
        if (total_reg > 0) {
            printf("Porcentaje asistencia: %.1f%%\n",
                   (float)presentes / total_reg * 100.0f);
        }
    }
    pausar();
}

/* Ver asistencias de una materia especifica */
void verAsistenciasPorMateria(const char id_materia[]) {
    Asistencia asistencias[MAX_ASISTENCIAS];
    int total, i, encontrado = 0;
    cargarAsistencias(asistencias, &total);
    limpiarPantalla();
    printf("=== ASISTENCIAS DE MATERIA %s ===\n", id_materia);
    printf("%-10s %-10s %-11s %-10s %-20s\n",
           "ID", "Alumno", "Fecha", "Estado", "Observaciones");
    printf("%-10s %-10s %-11s %-10s %-20s\n",
           "------", "------", "-----", "------", "-------------");
    for (i = 0; i < total; i++) {
        if (strcmp(asistencias[i].id_materia, id_materia) == 0) {
            printf("%-10s %-10s %-11s %-10s %-20s\n",
                   asistencias[i].id, asistencias[i].id_alumno,
                   asistencias[i].fecha,
                   asistencias[i].presente ? "Presente" : "Ausente",
                   asistencias[i].observaciones);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No hay registros de asistencia para esta materia.\n");
    pausar();
}

/* Reporte general de asistencias por alumno y materia */
void reporteAsistencias() {
    Asistencia asistencias[MAX_ASISTENCIAS];
    int total, i;
    cargarAsistencias(asistencias, &total);
    limpiarPantalla();
    printf("=== REPORTE GENERAL DE ASISTENCIAS ===\n\n");
    /* Contar por alumno */
    char alumnos_vistos[MAX_ASISTENCIAS][20];
    int n_vistos = 0;
    int k, ya_visto;
    for (i = 0; i < total; i++) {
        ya_visto = 0;
        for (k = 0; k < n_vistos; k++) {
            if (strcmp(alumnos_vistos[k], asistencias[i].id_alumno) == 0) {
                ya_visto = 1;
                break;
            }
        }
        if (!ya_visto) {
            strcpy(alumnos_vistos[n_vistos++], asistencias[i].id_alumno);
        }
    }
    printf("%-10s %-10s %-10s %-10s %-10s\n",
           "Alumno", "Materia", "Total", "Present.", "Porcent.");
    printf("%-10s %-10s %-10s %-10s %-10s\n",
           "------", "-------", "-----", "--------", "--------");
    for (k = 0; k < n_vistos; k++) {
        /* Obtener materias del alumno */
        char materias_vistas[MAX_ASISTENCIAS][20];
        int n_mat = 0;
        int m, ya_mat;
        for (i = 0; i < total; i++) {
            if (strcmp(asistencias[i].id_alumno, alumnos_vistos[k]) != 0) continue;
            ya_mat = 0;
            for (m = 0; m < n_mat; m++) {
                if (strcmp(materias_vistas[m], asistencias[i].id_materia) == 0) {
                    ya_mat = 1;
                    break;
                }
            }
            if (!ya_mat) {
                strcpy(materias_vistas[n_mat++], asistencias[i].id_materia);
            }
        }
        for (m = 0; m < n_mat; m++) {
            int tot = 0, pres = 0;
            for (i = 0; i < total; i++) {
                if (strcmp(asistencias[i].id_alumno, alumnos_vistos[k]) == 0 &&
                    strcmp(asistencias[i].id_materia, materias_vistas[m]) == 0) {
                    tot++;
                    if (asistencias[i].presente) pres++;
                }
            }
            float pct = (tot > 0) ? (float)pres / tot * 100.0f : 0.0f;
            printf("%-10s %-10s %-10d %-10d %-9.1f%%\n",
                   alumnos_vistos[k], materias_vistas[m], tot, pres, pct);
        }
    }
    pausar();
}
