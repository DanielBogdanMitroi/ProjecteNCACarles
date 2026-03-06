#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "periodos.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de periodos */
void menuPeriodos(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE PERIODOS ===\n");
        printf("1. Listar periodos\n");
        if (usuario_actual.tipo == 1) {
            printf("2. Agregar periodo\n");
            printf("3. Editar periodo\n");
            printf("4. Eliminar periodo\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarPeriodos(); break;
            case 2: agregarPeriodo(usuario_actual); break;
            case 3: editarPeriodo(usuario_actual); break;
            case 4: eliminarPeriodo(usuario_actual); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los periodos */
void listarPeriodos() {
    Periodo periodos[MAX_PERIODOS];
    int total, i, activos = 0;
    cargarPeriodos(periodos, &total);
    limpiarPantalla();
    printf("=== LISTA DE PERIODOS ===\n");
    printf("Total de registros en archivo: %d\n", total);
    printf("%-10s %-30s %-11s %-11s %-8s\n",
           "ID", "Nombre", "Inicio", "Fin", "Estado");
    printf("%-10s %-30s %-11s %-11s %-8s\n",
           "------", "------", "------", "---", "------");
    for (i = 0; i < total; i++) {
        printf("%-10s %-30s %-11s %-11s %-8s\n",
               periodos[i].id, periodos[i].nombre,
               periodos[i].fecha_inicio, periodos[i].fecha_fin,
               periodos[i].activo ? "Activo" : "Inactivo");
        if (periodos[i].activo) activos++;
    }
    if (total == 0) {
        printf("\nNo hay periodos registrados en el sistema.\n");
        printf("Usa la opcion 'Agregar periodo' para crear el primer registro.\n");
    } else {
        printf("\nTotal: %d periodos (%d activos, %d inactivos).\n",
               total, activos, total - activos);
    }
    pausar();
}

/* Agrega un nuevo periodo */
void agregarPeriodo(Usuario usuario_actual) {
    Periodo periodos[MAX_PERIODOS];
    int total;
    Periodo nuevo;
    memset(&nuevo, 0, sizeof(Periodo));

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para agregar periodos.\n");
        pausar();
        return;
    }

    cargarPeriodos(periodos, &total);
    generarID(nuevo.id, "PER", total + 1);

    printf("=== AGREGAR PERIODO ===\n");
    printf("ID generado: %s\n", nuevo.id);

    do {
        leerString(nuevo.nombre, sizeof(nuevo.nombre), "Nombre: ");
        if (strlen(nuevo.nombre) == 0) {
            printf("\nERROR: El nombre no puede estar vacio.\n");
            printf("Por favor, introduce un nombre valido.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.nombre, 5)) {
            printf("\nERROR: El nombre debe tener al menos 5 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nuevo.nombre);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.fecha_inicio, sizeof(nuevo.fecha_inicio), "Fecha inicio (YYYY-MM-DD): ");
        if (strlen(nuevo.fecha_inicio) == 0) {
            printf("\nERROR: La fecha de inicio no puede estar vacia.\n");
            printf("Por favor, introduce una fecha valida en formato YYYY-MM-DD.\n\n");
            continue;
        }
        if (strlen(nuevo.fecha_inicio) != 10) {
            printf("\nERROR: La fecha debe tener exactamente 10 caracteres (YYYY-MM-DD).\n");
            printf("Valor introducido: '%s' (longitud: %d)\n\n",
                   nuevo.fecha_inicio, (int)strlen(nuevo.fecha_inicio));
            continue;
        }
        if (!validarFecha(nuevo.fecha_inicio)) {
            printf("\nERROR: Fecha invalida o formato incorrecto.\n");
            printf("Use el formato YYYY-MM-DD (ejemplo: 2026-03-06).\n\n");
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.fecha_fin, sizeof(nuevo.fecha_fin), "Fecha fin (YYYY-MM-DD): ");
        if (strlen(nuevo.fecha_fin) == 0) {
            printf("\nERROR: La fecha de fin no puede estar vacia.\n");
            printf("Por favor, introduce una fecha valida en formato YYYY-MM-DD.\n\n");
            continue;
        }
        if (strlen(nuevo.fecha_fin) != 10) {
            printf("\nERROR: La fecha debe tener exactamente 10 caracteres (YYYY-MM-DD).\n");
            printf("Valor introducido: '%s' (longitud: %d)\n\n",
                   nuevo.fecha_fin, (int)strlen(nuevo.fecha_fin));
            continue;
        }
        if (!validarFecha(nuevo.fecha_fin)) {
            printf("\nERROR: Fecha invalida o formato incorrecto.\n");
            printf("Use el formato YYYY-MM-DD (ejemplo: 2026-03-06).\n\n");
            continue;
        }
        if (!validarRangoFechas(nuevo.fecha_inicio, nuevo.fecha_fin)) {
            printf("\nERROR: La fecha de fin debe ser posterior a la de inicio (%s).\n\n",
                   nuevo.fecha_inicio);
            continue;
        }
        break;
    } while (1);

    nuevo.activo = 1;

    periodos[total] = nuevo;
    total++;
    guardarPeriodos(periodos, total);
    printf("Periodo %s agregado exitosamente.\n", nuevo.id);
    pausar();
}

/* Edita un periodo existente */
void editarPeriodo(Usuario usuario_actual) {
    Periodo periodos[MAX_PERIODOS];
    int total, i;
    char id_buscar[20];
    char temp[50];
    int fecha_valida;

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para editar periodos.\n");
        pausar();
        return;
    }

    cargarPeriodos(periodos, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del periodo a editar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(periodos[i].id, id_buscar) == 0) {
            printf("=== EDITAR PERIODO %s ===\n", periodos[i].id);
            printf("(Presione Enter para mantener el valor actual)\n\n");

            mostrarValorActual("Nombre actual", periodos[i].nombre);
            do {
                leerString(temp, sizeof(temp), "Nuevo nombre: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 5)) {
                    printf("Error: El nombre debe tener al menos 5 caracteres.\n");
                } else {
                    strcpy(periodos[i].nombre, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Fecha inicio actual", periodos[i].fecha_inicio);
            fecha_valida = 0;
            do {
                leerString(temp, sizeof(temp), "Nueva fecha inicio (YYYY-MM-DD): ");
                if (strlen(temp) == 0) { fecha_valida = 1; break; }
                if (!validarFecha(temp)) {
                    printf("Error: Fecha invalida. Formato: YYYY-MM-DD\n");
                } else {
                    strcpy(periodos[i].fecha_inicio, temp);
                    fecha_valida = 1;
                }
            } while (!fecha_valida);

            mostrarValorActual("Fecha fin actual", periodos[i].fecha_fin);
            fecha_valida = 0;
            do {
                leerString(temp, sizeof(temp), "Nueva fecha fin (YYYY-MM-DD): ");
                if (strlen(temp) == 0) { fecha_valida = 1; break; }
                if (!validarFecha(temp)) {
                    printf("Error: Fecha invalida. Formato: YYYY-MM-DD\n");
                } else if (!validarRangoFechas(periodos[i].fecha_inicio, temp)) {
                    printf("Error: La fecha de fin debe ser posterior a la de inicio (%s).\n",
                           periodos[i].fecha_inicio);
                } else {
                    strcpy(periodos[i].fecha_fin, temp);
                    fecha_valida = 1;
                }
            } while (!fecha_valida);

            guardarPeriodos(periodos, total);
            printf("Periodo actualizado.\n");
            pausar();
            return;
        }
    }
    printf("Periodo no encontrado.\n");
    pausar();
}

/* Elimina (desactiva) un periodo */
void eliminarPeriodo(Usuario usuario_actual) {
    Periodo periodos[MAX_PERIODOS];
    int total, i;
    char id_buscar[20];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para eliminar periodos.\n");
        pausar();
        return;
    }

    cargarPeriodos(periodos, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del periodo a eliminar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(periodos[i].id, id_buscar) == 0) {
            char msg[100];
            sprintf(msg, "Desea eliminar el periodo %s? (s/n): ", periodos[i].nombre);
            if (solicitarConfirmacion(msg)) {
                periodos[i].activo = 0;
                guardarPeriodos(periodos, total);
                printf("Periodo desactivado.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Periodo no encontrado.\n");
    pausar();
}
