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
                do {
                    leerString(id, sizeof(id), "ID de la materia: ");
                    if (!validarCadenaNoVacia(id, 1)) {
                        printf("Error: El ID no puede estar vacio.\n");
                    }
                } while (!validarCadenaNoVacia(id, 1));
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
    printf("Total de registros en archivo: %d\n", total);
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
    if (total == 0) {
        printf("\nNo hay horarios registrados en el sistema.\n");
        printf("Usa la opcion 'Agregar horario' para crear el primer registro.\n");
    } else {
        printf("\nTotal: %d horarios.\n", total);
    }
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

    do {
        leerString(nuevo.id_materia, sizeof(nuevo.id_materia), "ID Materia: ");
        if (!validarCadenaNoVacia(nuevo.id_materia, 2)) {
            printf("Error: El ID de materia no puede estar vacio.\n");
            continue;
        }
        if (!existeIDEnArchivo(ARCHIVO_MATERIAS, nuevo.id_materia)) {
            printf("Error: No existe una materia con ID %s.\n", nuevo.id_materia);
        } else {
            break;
        }
    } while (1);

    do {
        leerString(nuevo.id_aula, sizeof(nuevo.id_aula), "ID Aula: ");
        if (!validarCadenaNoVacia(nuevo.id_aula, 2)) {
            printf("Error: El ID de aula no puede estar vacio.\n");
            continue;
        }
        if (!existeIDEnArchivo(ARCHIVO_AULAS, nuevo.id_aula)) {
            printf("Error: No existe un aula con ID %s.\n", nuevo.id_aula);
        } else {
            break;
        }
    } while (1);

    do {
        leerString(nuevo.dia_semana, sizeof(nuevo.dia_semana), "Dia semana (Lunes/Martes/Miercoles/Jueves/Viernes/Sabado/Domingo): ");
        if (!validarDiaSemana(nuevo.dia_semana)) {
            printf("Error: Dia invalido. Use: Lunes, Martes, Miercoles, Jueves, Viernes, Sabado, Domingo.\n");
        }
    } while (!validarDiaSemana(nuevo.dia_semana));

    do {
        leerString(nuevo.hora_inicio, sizeof(nuevo.hora_inicio), "Hora inicio (HH:MM): ");
        if (!validarHora(nuevo.hora_inicio)) {
            printf("Error: Hora de inicio invalida. Formato: HH:MM\n");
        }
    } while (!validarHora(nuevo.hora_inicio));

    do {
        leerString(nuevo.hora_fin, sizeof(nuevo.hora_fin), "Hora fin (HH:MM): ");
        if (!validarHora(nuevo.hora_fin)) {
            printf("Error: Hora de fin invalida. Formato: HH:MM\n");
        } else if (!validarHoraRango(nuevo.hora_inicio, nuevo.hora_fin)) {
            printf("Error: La hora de fin debe ser posterior a la hora de inicio (%s).\n", nuevo.hora_inicio);
        } else {
            break;
        }
    } while (1);

    do {
        leerString(nuevo.periodo, sizeof(nuevo.periodo), "Periodo: ");
        if (!validarCadenaNoVacia(nuevo.periodo, 2)) {
            printf("Error: El periodo no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(nuevo.periodo, 2));

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
    char temp[20];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para editar horarios.\n");
        pausar();
        return;
    }

    cargarHorarios(horarios, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del horario a editar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(horarios[i].id, id_buscar) == 0) {
            printf("=== EDITAR HORARIO %s ===\n", horarios[i].id);
            printf("(Presione Enter para mantener el valor actual)\n\n");

            mostrarValorActual("Dia semana actual", horarios[i].dia_semana);
            do {
                leerString(temp, sizeof(temp), "Nuevo dia semana: ");
                if (strlen(temp) == 0) break;
                if (!validarDiaSemana(temp)) {
                    printf("Error: Dia invalido. Use: Lunes, Martes, Miercoles, Jueves, Viernes, Sabado, Domingo.\n");
                } else {
                    strcpy(horarios[i].dia_semana, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Hora inicio actual", horarios[i].hora_inicio);
            do {
                leerString(temp, sizeof(temp), "Nueva hora inicio (HH:MM): ");
                if (strlen(temp) == 0) break;
                if (!validarHora(temp)) {
                    printf("Error: Hora invalida. Formato: HH:MM\n");
                } else {
                    strcpy(horarios[i].hora_inicio, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Hora fin actual", horarios[i].hora_fin);
            do {
                leerString(temp, sizeof(temp), "Nueva hora fin (HH:MM): ");
                if (strlen(temp) == 0) break;
                if (!validarHora(temp)) {
                    printf("Error: Hora invalida. Formato: HH:MM\n");
                } else if (!validarHoraRango(horarios[i].hora_inicio, temp)) {
                    printf("Error: La hora de fin debe ser posterior a la hora de inicio (%s).\n",
                           horarios[i].hora_inicio);
                } else {
                    strcpy(horarios[i].hora_fin, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Periodo actual", horarios[i].periodo);
            do {
                leerString(temp, sizeof(temp), "Nuevo periodo: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 2)) {
                    printf("Error: El periodo no puede estar vacio.\n");
                } else {
                    strcpy(horarios[i].periodo, temp);
                    break;
                }
            } while (1);

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

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para eliminar horarios.\n");
        pausar();
        return;
    }

    cargarHorarios(horarios, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del horario a eliminar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(horarios[i].id, id_buscar) == 0) {
            char msg[100];
            sprintf(msg, "Confirmar eliminacion del horario %s? (s/n): ", id_buscar);
            if (solicitarConfirmacion(msg)) {
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
    if (!validarCadenaNoVacia(id_materia, 1)) {
        printf("Error: El ID de materia no puede estar vacio.\n");
        pausar();
        return;
    }
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
