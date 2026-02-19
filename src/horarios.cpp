#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "horarios.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de horarios */
void menuHorarios(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE HORARIOS ===\n");
        printf("1. Listar horarios\n");
        if (usuario_actual.tipo == 1) {
            printf("2. Agregar horario\n");
            printf("3. Editar horario\n");
            printf("4. Eliminar horario\n");
        }
        printf("5. Ver horario por materia\n");
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarHorarios(); break;
            case 2: agregarHorario(usuario_actual); break;
            case 3: editarHorario(usuario_actual); break;
            case 4: eliminarHorario(usuario_actual); break;
            case 5: {
                char id[20];
                leerString(id, sizeof(id), "ID de la materia: ");
                verHorarioMateria(id);
                break;
            }
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los horarios */
void listarHorarios() {
    Horario horarios[MAX_HORARIOS];
    int total, i;
    cargarHorarios(horarios, &total);
    limpiarPantalla();
    printf("=== LISTA DE HORARIOS ===\n");
    printf("%-10s %-10s %-10s %-12s %-8s %-8s %-10s\n",
           "ID", "Materia", "Aula", "Dia", "Inicio", "Fin", "Periodo");
    printf("%-10s %-10s %-10s %-12s %-8s %-8s %-10s\n",
           "------", "-------", "----", "---", "------", "---", "-------");
    for (i = 0; i < total; i++) {
        printf("%-10s %-10s %-10s %-12s %-8s %-8s %-10s\n",
               horarios[i].id, horarios[i].id_materia, horarios[i].id_aula,
               horarios[i].dia_semana, horarios[i].hora_inicio,
               horarios[i].hora_fin, horarios[i].periodo);
    }
    printf("\nTotal: %d horarios.\n", total);
    pausar();
}

/* Agrega un nuevo horario */
void agregarHorario(Usuario usuario_actual) {
    Horario horarios[MAX_HORARIOS];
    int total;
    Horario nuevo;
    memset(&nuevo, 0, sizeof(Horario));

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para agregar horarios.\n");
        pausar();
        return;
    }

    cargarHorarios(horarios, &total);
    generarID(nuevo.id, "HOR", total + 1);

    printf("=== AGREGAR HORARIO ===\n");
    printf("ID generado: %s\n", nuevo.id);
    leerString(nuevo.id_materia, sizeof(nuevo.id_materia), "ID Materia: ");
    leerString(nuevo.id_aula, sizeof(nuevo.id_aula), "ID Aula: ");
    leerString(nuevo.dia_semana, sizeof(nuevo.dia_semana), "Dia semana (Lunes/Martes/...): ");
    leerString(nuevo.hora_inicio, sizeof(nuevo.hora_inicio), "Hora inicio (HH:MM): ");
    if (!validarHora(nuevo.hora_inicio)) {
        printf("Hora de inicio invalida.\n");
        pausar();
        return;
    }
    leerString(nuevo.hora_fin, sizeof(nuevo.hora_fin), "Hora fin (HH:MM): ");
    if (!validarHora(nuevo.hora_fin)) {
        printf("Hora de fin invalida.\n");
        pausar();
        return;
    }
    leerString(nuevo.periodo, sizeof(nuevo.periodo), "Periodo: ");

    horarios[total] = nuevo;
    total++;
    guardarHorarios(horarios, total);
    printf("Horario %s agregado exitosamente.\n", nuevo.id);
    pausar();
}

/* Edita un horario existente */
void editarHorario(Usuario usuario_actual) {
    Horario horarios[MAX_HORARIOS];
    int total, i;
    char id_buscar[20];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para editar horarios.\n");
        pausar();
        return;
    }

    cargarHorarios(horarios, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del horario a editar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(horarios[i].id, id_buscar) == 0) {
            printf("=== EDITAR HORARIO %s ===\n", horarios[i].id);
            leerString(horarios[i].dia_semana, sizeof(horarios[i].dia_semana), "Dia semana: ");
            leerString(horarios[i].hora_inicio, sizeof(horarios[i].hora_inicio), "Hora inicio (HH:MM): ");
            leerString(horarios[i].hora_fin, sizeof(horarios[i].hora_fin), "Hora fin (HH:MM): ");
            leerString(horarios[i].periodo, sizeof(horarios[i].periodo), "Periodo: ");
            guardarHorarios(horarios, total);
            printf("Horario actualizado.\n");
            pausar();
            return;
        }
    }
    printf("Horario no encontrado.\n");
    pausar();
}

/* Elimina un horario */
void eliminarHorario(Usuario usuario_actual) {
    Horario horarios[MAX_HORARIOS];
    int total, i, j;
    char id_buscar[20];
    char confirmacion[5];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para eliminar horarios.\n");
        pausar();
        return;
    }

    cargarHorarios(horarios, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del horario a eliminar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(horarios[i].id, id_buscar) == 0) {
            printf("Confirmar eliminacion del horario %s? (s/n): ", id_buscar);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
                for (j = i; j < total - 1; j++) {
                    horarios[j] = horarios[j + 1];
                }
                total--;
                guardarHorarios(horarios, total);
                printf("Horario eliminado.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Horario no encontrado.\n");
    pausar();
}

/* Ver horario de una materia especifica */
void verHorarioMateria(const char id_materia[]) {
    Horario horarios[MAX_HORARIOS];
    int total, i, encontrado = 0;
    cargarHorarios(horarios, &total);
    limpiarPantalla();
    printf("=== HORARIO DE MATERIA %s ===\n", id_materia);
    printf("%-10s %-10s %-12s %-8s %-8s\n",
           "ID", "Aula", "Dia", "Inicio", "Fin");
    printf("%-10s %-10s %-12s %-8s %-8s\n",
           "------", "----", "---", "------", "---");
    for (i = 0; i < total; i++) {
        if (strcmp(horarios[i].id_materia, id_materia) == 0) {
            printf("%-10s %-10s %-12s %-8s %-8s\n",
                   horarios[i].id, horarios[i].id_aula,
                   horarios[i].dia_semana, horarios[i].hora_inicio, horarios[i].hora_fin);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No hay horarios para esta materia.\n");
    pausar();
}
