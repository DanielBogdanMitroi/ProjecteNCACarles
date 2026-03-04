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
                leerString(id, sizeof(id), "ID del alumno: ");
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
    int fecha_valida;
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
        if (!validarCadenaNoVacia(nuevo.id_materia, 2)) {
            printf("Error: El ID de materia no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(nuevo.id_materia, 2));

    do {
        leerString(nuevo.titulo, sizeof(nuevo.titulo), "Titulo: ");
        if (!validarCadenaNoVacia(nuevo.titulo, 3)) {
            printf("Error: El titulo debe tener al menos 3 caracteres.\n");
        }
    } while (!validarCadenaNoVacia(nuevo.titulo, 3));

    do {
        leerString(nuevo.descripcion, sizeof(nuevo.descripcion), "Descripcion: ");
        if (!validarCadenaNoVacia(nuevo.descripcion, 3)) {
            printf("Error: La descripcion debe tener al menos 3 caracteres.\n");
        }
    } while (!validarCadenaNoVacia(nuevo.descripcion, 3));

    fecha_valida = 0;
    do {
        leerString(nuevo.fecha_asignacion, sizeof(nuevo.fecha_asignacion), "Fecha asignacion (YYYY-MM-DD): ");
        if (!validarFecha(nuevo.fecha_asignacion)) {
            printf("Error: Fecha invalida. Formato: YYYY-MM-DD\n");
        } else {
            fecha_valida = 1;
        }
    } while (!fecha_valida);

    fecha_valida = 0;
    do {
        leerString(nuevo.fecha_entrega, sizeof(nuevo.fecha_entrega), "Fecha entrega (YYYY-MM-DD): ");
        if (!validarFecha(nuevo.fecha_entrega)) {
            printf("Error: Fecha invalida. Formato: YYYY-MM-DD\n");
        } else {
            fecha_valida = 1;
        }
    } while (!fecha_valida);

    leerFloat(&nuevo.puntuacion_maxima, "Puntuacion maxima: ");
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

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para editar trabajos.\n");
        pausar();
        return;
    }

    cargarTrabajos(trabajos, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del trabajo a editar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(trabajos[i].id, id_buscar) == 0) {
            printf("=== EDITAR TRABAJO %s ===\n", trabajos[i].id);
            leerString(trabajos[i].titulo, sizeof(trabajos[i].titulo), "Titulo: ");
            leerString(trabajos[i].descripcion, sizeof(trabajos[i].descripcion), "Descripcion: ");
            leerString(trabajos[i].fecha_entrega, sizeof(trabajos[i].fecha_entrega), "Fecha entrega (YYYY-MM-DD): ");
            leerFloat(&trabajos[i].puntuacion_maxima, "Puntuacion maxima: ");
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
    char confirmacion[5];

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para eliminar trabajos.\n");
        pausar();
        return;
    }

    cargarTrabajos(trabajos, &total);
    leerString(id_buscar, sizeof(id_buscar), "ID del trabajo a eliminar: ");

    for (i = 0; i < total; i++) {
        if (strcmp(trabajos[i].id, id_buscar) == 0) {
            printf("Desea eliminar el trabajo %s? (s/n): ", trabajos[i].titulo);
            leerString(confirmacion, sizeof(confirmacion), "");
            if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
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
    int total;
    Entrega_Trabajo nueva;
    memset(&nueva, 0, sizeof(Entrega_Trabajo));

    if (usuario_actual.tipo == 3) {
        printf("Sin permisos para registrar entregas.\n");
        pausar();
        return;
    }

    cargarEntregas(entregas, &total);
    generarID(nueva.id, "ENTR", total + 1);

    printf("=== REGISTRAR ENTREGA ===\n");
    printf("ID generado: %s\n", nueva.id);
    leerString(nueva.id_trabajo, sizeof(nueva.id_trabajo), "ID Trabajo: ");
    leerString(nueva.id_alumno, sizeof(nueva.id_alumno), "ID Alumno: ");
    leerString(nueva.fecha_entrega, sizeof(nueva.fecha_entrega), "Fecha entrega (YYYY-MM-DD): ");
    leerFloat(&nueva.calificacion, "Calificacion: ");
    leerString(nueva.comentarios, sizeof(nueva.comentarios), "Comentarios: ");
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
