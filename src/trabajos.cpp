#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trabajos.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de trabajos */
void menuTrabajos(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE TRABAJOS ===\n");
        printf("1. Listar trabajos\n");
        if (usuario_actual.tipo == 1 || usuario_actual.tipo == 2) {
            printf("2. Agregar trabajo\n");
            printf("3. Editar trabajo\n");
            printf("4. Eliminar trabajo\n");
            printf("5. Registrar entrega\n");
        }
        printf("6. Ver entregas por alumno\n");
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarTrabajos(); break;
            case 2: agregarTrabajo(usuario_actual); break;
            case 3: editarTrabajo(usuario_actual); break;
            case 4: eliminarTrabajo(usuario_actual); break;
            case 5: registrarEntrega(usuario_actual); break;
            case 6: {
                char id[20];
                do {
                    leerString(id, sizeof(id), "ID del alumno: ");
                    if (!validarCadenaNoVacia(id, 1)) {
                        printf("Error: El ID no puede estar vacio.\n");
                    }
                } while (!validarCadenaNoVacia(id, 1));
                verEntregasAlumno(id);
                break;
            }
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los trabajos activos */
void listarTrabajos() {
    Trabajo trabajos[MAX_TRABAJOS];
    int total, i, activos = 0;
    cargarTrabajos(trabajos, &total);
    limpiarPantalla();
    printf("=== LISTA DE TRABAJOS ===\n");
    printf("Total de registros en archivo: %d\n", total);
    printf("%-10s %-10s %-30s %-11s %-11s\n",
           "ID", "Materia", "Titulo", "F.Asign.", "F.Entrega");
    printf("%-10s %-10s %-30s %-11s %-11s\n",
           "------", "-------", "------", "--------", "---------");
    for (i = 0; i < total; i++) {
        if (trabajos[i].activo) {
            printf("%-10s %-10s %-30s %-11s %-11s\n",
                   trabajos[i].id, trabajos[i].id_materia, trabajos[i].titulo,
                   trabajos[i].fecha_asignacion, trabajos[i].fecha_entrega);
            activos++;
        }
    }
    if (activos == 0) {
        if (total == 0) {
            printf("\nNo hay trabajos registrados en el sistema.\n");
            printf("Usa la opcion 'Agregar trabajo' para crear el primer registro.\n");
        } else {
            printf("\nTodos los trabajos estan marcados como inactivos.\n");
            printf("Total de registros inactivos: %d\n", total);
        }
    } else {
        printf("\nTotal de trabajos activos: %d\n", activos);
        if (total > activos) {
            printf("Trabajos inactivos: %d\n", total - activos);
        }
    }
    pausar();
}

/* Agrega un nuevo trabajo */
void agregarTrabajo(Usuario usuario_actual) {
    Trabajo trabajos[MAX_TRABAJOS];
    int total;
    Trabajo nuevo;
    memset(&nuevo, 0, sizeof(Trabajo));

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para agregar trabajos.\n");
        pausar();
        return;
    }

    cargarTrabajos(trabajos, &total);
    generarID(nuevo.id, "TRAB", total + 1);

    printf("=== AGREGAR TRABAJO ===\n");
    printf("ID generado: %s\n", nuevo.id);

    do {
        leerString(nuevo.id_materia, sizeof(nuevo.id_materia), "ID Materia: ");
        if (strlen(nuevo.id_materia) == 0) {
            printf("\nERROR: El ID de materia no puede estar vacio.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.id_materia, 2)) {
            printf("\nERROR: El ID de materia debe tener al menos 2 caracteres.\n\n");
            continue;
        }
        if (!existeIDEnArchivo(ARCHIVO_MATERIAS, nuevo.id_materia)) {
            printf("\nERROR: No existe una materia con ID %s.\n", nuevo.id_materia);
            printf("Por favor, verifica el ID e intenta nuevamente.\n\n");
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.titulo, sizeof(nuevo.titulo), "Titulo: ");
        if (strlen(nuevo.titulo) == 0) {
            printf("\nERROR: El titulo no puede estar vacio.\n");
            printf("Por favor, introduce un titulo valido.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.titulo, 3)) {
            printf("\nERROR: El titulo debe tener al menos 3 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nuevo.titulo);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.descripcion, sizeof(nuevo.descripcion), "Descripcion: ");
        if (strlen(nuevo.descripcion) == 0) {
            printf("\nERROR: La descripcion no puede estar vacia.\n");
            printf("Por favor, introduce una descripcion valida.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.descripcion, 3)) {
            printf("\nERROR: La descripcion debe tener al menos 3 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nuevo.descripcion);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.fecha_asignacion, sizeof(nuevo.fecha_asignacion), "Fecha asignacion (YYYY-MM-DD): ");
        if (strlen(nuevo.fecha_asignacion) == 0) {
            printf("\nERROR: La fecha de asignacion no puede estar vacia.\n");
            printf("Por favor, introduce una fecha valida en formato YYYY-MM-DD.\n\n");
            continue;
        }
        if (strlen(nuevo.fecha_asignacion) != 10) {
            printf("\nERROR: La fecha debe tener exactamente 10 caracteres (YYYY-MM-DD).\n");
            printf("Valor introducido: '%s' (longitud: %d)\n\n",
                   nuevo.fecha_asignacion, (int)strlen(nuevo.fecha_asignacion));
            continue;
        }
        if (!validarFecha(nuevo.fecha_asignacion)) {
            printf("\nERROR: Fecha invalida o formato incorrecto.\n");
            printf("Use el formato YYYY-MM-DD (ejemplo: 2026-03-06).\n\n");
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.fecha_entrega, sizeof(nuevo.fecha_entrega), "Fecha entrega (YYYY-MM-DD): ");
        if (strlen(nuevo.fecha_entrega) == 0) {
            printf("\nERROR: La fecha de entrega no puede estar vacia.\n");
            printf("Por favor, introduce una fecha valida en formato YYYY-MM-DD.\n\n");
            continue;
        }
        if (strlen(nuevo.fecha_entrega) != 10) {
            printf("\nERROR: La fecha debe tener exactamente 10 caracteres (YYYY-MM-DD).\n");
            printf("Valor introducido: '%s' (longitud: %d)\n\n",
                   nuevo.fecha_entrega, (int)strlen(nuevo.fecha_entrega));
            continue;
        }
        if (!validarFecha(nuevo.fecha_entrega)) {
            printf("\nERROR: Fecha invalida o formato incorrecto.\n");
            printf("Use el formato YYYY-MM-DD (ejemplo: 2026-03-06).\n\n");
            continue;
        }
        if (!validarRangoFechas(nuevo.fecha_asignacion, nuevo.fecha_entrega)) {
            printf("\nERROR: La fecha de entrega debe ser posterior a la de asignacion (%s).\n\n",
                   nuevo.fecha_asignacion);
            continue;
        }
        break;
    } while (1);

    do {
        leerFloat(&nuevo.puntuacion_maxima, "Puntuacion maxima (1-100): ");
        if (nuevo.puntuacion_maxima < 0.01f || nuevo.puntuacion_maxima > 100.0f) {
            printf("\nERROR: La puntuacion debe estar entre 0.01 y 100.\n");
            printf("Por favor, introduce un valor valido.\n\n");
        }
    } while (nuevo.puntuacion_maxima < 0.01f || nuevo.puntuacion_maxima > 100.0f);

    nuevo.activo = 1;

    trabajos[total] = nuevo;
    total++;
    guardarTrabajos(trabajos, total);
    printf("Trabajo %s agregado exitosamente.\n", nuevo.id);
    pausar();
}

/* Edita un trabajo existente */
void editarTrabajo(Usuario usuario_actual) {
    Trabajo trabajos[MAX_TRABAJOS];
    int total, i;
    char id_buscar[20];
    char temp[300];
    int fecha_valida;

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para editar trabajos.\n");
        pausar();
        return;
    }

    cargarTrabajos(trabajos, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del trabajo a editar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(trabajos[i].id, id_buscar) == 0) {
            printf("=== EDITAR TRABAJO %s ===\n", trabajos[i].id);
            printf("(Presione Enter para mantener el valor actual)\n\n");

            mostrarValorActual("Titulo actual", trabajos[i].titulo);
            do {
                leerString(temp, sizeof(temp), "Nuevo titulo: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 3)) {
                    printf("Error: El titulo debe tener al menos 3 caracteres.\n");
                } else {
                    strcpy(trabajos[i].titulo, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Descripcion actual", trabajos[i].descripcion);
            do {
                leerString(temp, sizeof(temp), "Nueva descripcion: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 3)) {
                    printf("Error: La descripcion debe tener al menos 3 caracteres.\n");
                } else {
                    strcpy(trabajos[i].descripcion, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Fecha entrega actual", trabajos[i].fecha_entrega);
            fecha_valida = 0;
            do {
                leerString(temp, sizeof(temp), "Nueva fecha entrega (YYYY-MM-DD): ");
                if (strlen(temp) == 0) { fecha_valida = 1; break; }
                if (!validarFecha(temp)) {
                    printf("Error: Fecha invalida. Formato: YYYY-MM-DD\n");
                } else if (!validarRangoFechas(trabajos[i].fecha_asignacion, temp)) {
                    printf("Error: La fecha de entrega debe ser posterior a la de asignacion (%s).\n",
                           trabajos[i].fecha_asignacion);
                } else {
                    strcpy(trabajos[i].fecha_entrega, temp);
                    fecha_valida = 1;
                }
            } while (!fecha_valida);

            mostrarValorActualFloat("Puntuacion maxima actual", trabajos[i].puntuacion_maxima);
            do {
                leerString(temp, sizeof(temp), "Nueva puntuacion maxima (1-100, Enter para mantener): ");
                if (strlen(temp) == 0) break;
                float val;
                if (sscanf(temp, "%f", &val) == 1) {
                    if (val < 0.01f || val > 100.0f) {
                        printf("Error: La puntuacion debe estar entre 0.01 y 100.\n");
                    } else {
                        trabajos[i].puntuacion_maxima = val;
                        break;
                    }
                } else {
                    printf("Error: Debe ingresar un numero.\n");
                }
            } while (1);

            guardarTrabajos(trabajos, total);
            printf("Trabajo actualizado.\n");
            pausar();
            return;
        }
    }
    printf("Trabajo no encontrado.\n");
    pausar();
}

/* Elimina (desactiva) un trabajo */
void eliminarTrabajo(Usuario usuario_actual) {
    Trabajo trabajos[MAX_TRABAJOS];
    int total, i;
    char id_buscar[20];

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para eliminar trabajos.\n");
        pausar();
        return;
    }

    cargarTrabajos(trabajos, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del trabajo a eliminar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(trabajos[i].id, id_buscar) == 0) {
            char msg[150];
            sprintf(msg, "Desea eliminar el trabajo %s? (s/n): ", trabajos[i].titulo);
            if (solicitarConfirmacion(msg)) {
                trabajos[i].activo = 0;
                guardarTrabajos(trabajos, total);
                printf("Trabajo desactivado.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Trabajo no encontrado.\n");
    pausar();
}

/* Registra la entrega de un trabajo por un alumno */
void registrarEntrega(Usuario usuario_actual) {
    Entrega_Trabajo entregas[MAX_ENTREGAS];
    Trabajo trabajos[MAX_TRABAJOS];
    int total, total_t, j;
    Entrega_Trabajo nueva;
    int fecha_valida;
    float puntuacion_max = 10.0f;
    memset(&nueva, 0, sizeof(Entrega_Trabajo));

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para registrar entregas.\n");
        pausar();
        return;
    }

    cargarEntregas(entregas, &total);
    cargarTrabajos(trabajos, &total_t);
    generarID(nueva.id, "ENTR", total + 1);

    printf("=== REGISTRAR ENTREGA ===\n");
    printf("ID generado: %s\n", nueva.id);

    do {
        leerString(nueva.id_trabajo, sizeof(nueva.id_trabajo), "ID Trabajo: ");
        if (!validarCadenaNoVacia(nueva.id_trabajo, 2)) {
            printf("Error: El ID de trabajo no puede estar vacio.\n");
            continue;
        }
        if (!existeIDEnArchivo(ARCHIVO_TRABAJOS, nueva.id_trabajo)) {
            printf("Error: No existe un trabajo con ID %s.\n", nueva.id_trabajo);
        } else {
            break;
        }
    } while (1);

    /* Obtener puntuacion_maxima del trabajo */
    for (j = 0; j < total_t; j++) {
        if (strcmp(trabajos[j].id, nueva.id_trabajo) == 0) {
            puntuacion_max = trabajos[j].puntuacion_maxima;
            break;
        }
    }

    do {
        leerString(nueva.id_alumno, sizeof(nueva.id_alumno), "ID Alumno: ");
        if (!validarCadenaNoVacia(nueva.id_alumno, 2)) {
            printf("Error: El ID de alumno no puede estar vacio.\n");
            continue;
        }
        if (!existeIDEnArchivo(ARCHIVO_ALUMNOS, nueva.id_alumno)) {
            printf("Error: No existe un alumno con ID %s.\n", nueva.id_alumno);
        } else {
            break;
        }
    } while (1);

    fecha_valida = 0;
    do {
        leerString(nueva.fecha_entrega, sizeof(nueva.fecha_entrega), "Fecha entrega (YYYY-MM-DD): ");
        if (!validarFecha(nueva.fecha_entrega)) {
            printf("Error: Fecha invalida. Formato: YYYY-MM-DD\n");
        } else {
            fecha_valida = 1;
        }
    } while (!fecha_valida);

    do {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Calificacion (0-%.2f): ", puntuacion_max);
        leerFloat(&nueva.calificacion, prompt);
        if (!validarPuntuacion(nueva.calificacion, puntuacion_max)) {
            printf("Error: Calificacion invalida (0-%.2f).\n", puntuacion_max);
        }
    } while (!validarPuntuacion(nueva.calificacion, puntuacion_max));

    do {
        leerString(nueva.comentarios, sizeof(nueva.comentarios), "Comentarios: ");
        if (!validarCadenaNoVacia(nueva.comentarios, 2)) {
            printf("Error: Los comentarios deben tener al menos 2 caracteres.\n");
        }
    } while (!validarCadenaNoVacia(nueva.comentarios, 2));

    nueva.entregado = 1;

    entregas[total] = nueva;
    total++;
    guardarEntregas(entregas, total);
    printf("Entrega %s registrada exitosamente.\n", nueva.id);
    pausar();
}

/* Ver entregas de un alumno especifico */
void verEntregasAlumno(const char id_alumno[]) {
    Entrega_Trabajo entregas[MAX_ENTREGAS];
    Trabajo trabajos[MAX_TRABAJOS];
    int total_e, total_t, i, j, encontrado = 0;
    if (!validarCadenaNoVacia(id_alumno, 1)) {
        printf("Error: El ID de alumno no puede estar vacio.\n");
        pausar();
        return;
    }
    cargarEntregas(entregas, &total_e);
    cargarTrabajos(trabajos, &total_t);
    limpiarPantalla();
    printf("=== ENTREGAS DEL ALUMNO %s ===\n", id_alumno);
    printf("%-10s %-10s %-25s %-8s %-11s\n",
           "ID", "Trabajo", "Titulo", "Nota", "F.Entrega");
    printf("%-10s %-10s %-25s %-8s %-11s\n",
           "------", "-------", "------", "----", "---------");
    for (i = 0; i < total_e; i++) {
        if (strcmp(entregas[i].id_alumno, id_alumno) == 0) {
            char titulo[100] = "Desconocido";
            for (j = 0; j < total_t; j++) {
                if (strcmp(trabajos[j].id, entregas[i].id_trabajo) == 0) {
                    strcpy(titulo, trabajos[j].titulo);
                    break;
                }
            }
            printf("%-10s %-10s %-25s %-8.2f %-11s\n",
                   entregas[i].id, entregas[i].id_trabajo, titulo,
                   entregas[i].calificacion, entregas[i].fecha_entrega);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No hay entregas para este alumno.\n");
    pausar();
}
