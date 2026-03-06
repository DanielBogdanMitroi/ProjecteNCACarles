#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aulas.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de aulas */
void menuAulas(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE AULAS ===\n");
        printf("1. Listar aulas\n");
        if (usuario_actual.tipo == 1) {
            printf("2. Agregar aula\n");
            printf("3. Editar aula\n");
            printf("4. Eliminar aula\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarAulas(); break;
            case 2: agregarAula(usuario_actual); break;
            case 3: editarAula(usuario_actual); break;
            case 4: eliminarAula(usuario_actual); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todas las aulas activas */
void listarAulas() {
    Aula aulas[MAX_AULAS];
    int total, i, activos = 0;
    cargarAulas(aulas, &total);
    limpiarPantalla();
    printf("=== LISTA DE AULAS ===\n");
    printf("Total de registros en archivo: %d\n", total);
    printf("%-10s %-20s %-10s %-15s\n", "ID", "Nombre", "Capacidad", "Tipo");
    printf("%-10s %-20s %-10s %-15s\n", "------", "------", "---------", "----");
    for (i = 0; i < total; i++) {
        if (aulas[i].activo) {
            printf("%-10s %-20s %-10d %-15s\n",
                   aulas[i].id, aulas[i].nombre, aulas[i].capacidad, aulas[i].tipo);
            activos++;
        }
    }
    if (activos == 0) {
        if (total == 0) {
            printf("\nNo hay aulas registradas en el sistema.\n");
            printf("Usa la opcion 'Agregar aula' para crear el primer registro.\n");
        } else {
            printf("\nTodas las aulas estan marcadas como inactivas.\n");
            printf("Total de registros inactivos: %d\n", total);
        }
    } else {
        printf("\nTotal de aulas activas: %d\n", activos);
        if (total > activos) {
            printf("Aulas inactivas: %d\n", total - activos);
        }
    }
    pausar();
}

/* Agrega una nueva aula */
void agregarAula(Usuario usuario_actual) {
    Aula aulas[MAX_AULAS];
    int total;
    Aula nueva;
    memset(&nueva, 0, sizeof(Aula));

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para agregar aulas.\n");
        pausar();
        return;
    }

    cargarAulas(aulas, &total);
    generarID(nueva.id, "AULA", total + 1);

    printf("=== AGREGAR AULA ===\n");
    printf("ID generado: %s\n", nueva.id);

    do {
        leerString(nueva.nombre, sizeof(nueva.nombre), "Nombre: ");
        if (strlen(nueva.nombre) == 0) {
            printf("\nERROR: El nombre no puede estar vacio.\n");
            printf("Por favor, introduce un nombre valido.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nueva.nombre, 2)) {
            printf("\nERROR: El nombre debe tener al menos 2 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nueva.nombre);
            continue;
        }
        break;
    } while (1);

    do {
        leerEntero(&nueva.capacidad, "Capacidad (1-500): ");
        if (nueva.capacidad < 1 || nueva.capacidad > 500) {
            printf("\nERROR: La capacidad debe estar entre 1 y 500.\n");
            printf("Por favor, introduce un valor valido.\n\n");
        }
    } while (nueva.capacidad < 1 || nueva.capacidad > 500);

    do {
        leerString(nueva.tipo, sizeof(nueva.tipo), "Tipo (Teoria/Laboratorio): ");
        if (strlen(nueva.tipo) == 0) {
            printf("\nERROR: El tipo no puede estar vacio.\n");
            printf("Por favor, introduce un tipo valido.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nueva.tipo, 2)) {
            printf("\nERROR: El tipo debe tener al menos 2 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nueva.tipo);
            continue;
        }
        break;
    } while (1);

    nueva.activo = 1;

    aulas[total] = nueva;
    total++;
    guardarAulas(aulas, total);
    printf("Aula %s agregada exitosamente.\n", nueva.id);
    pausar();
}

/* Edita un aula existente */
void editarAula(Usuario usuario_actual) {
    Aula aulas[MAX_AULAS];
    int total, i;
    char id_buscar[20];
    char temp_str[50];
    int temp_int;

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para editar aulas.\n");
        pausar();
        return;
    }

    cargarAulas(aulas, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del aula a editar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(aulas[i].id, id_buscar) == 0) {
            printf("=== EDITAR AULA %s ===\n", aulas[i].id);
            printf("(Presione Enter para mantener el valor actual)\n\n");

            mostrarValorActual("Nombre actual", aulas[i].nombre);
            do {
                leerString(temp_str, sizeof(temp_str), "Nuevo nombre: ");
                if (strlen(temp_str) == 0) break;
                if (!validarCadenaNoVacia(temp_str, 2)) {
                    printf("Error: El nombre debe tener al menos 2 caracteres.\n");
                } else {
                    strcpy(aulas[i].nombre, temp_str);
                    break;
                }
            } while (1);

            mostrarValorActualEntero("Capacidad actual", aulas[i].capacidad);
            printf("Nueva capacidad (1-500, 0 para mantener): ");
            leerEntero(&temp_int, "");
            if (temp_int != 0) {
                do {
                    if (temp_int < 1 || temp_int > 500) {
                        printf("Error: La capacidad debe estar entre 1 y 500.\n");
                        leerEntero(&temp_int, "Nueva capacidad (1-500): ");
                    } else {
                        aulas[i].capacidad = temp_int;
                        break;
                    }
                } while (temp_int < 1 || temp_int > 500);
            }

            mostrarValorActual("Tipo actual", aulas[i].tipo);
            do {
                leerString(temp_str, sizeof(temp_str), "Nuevo tipo: ");
                if (strlen(temp_str) == 0) break;
                if (!validarCadenaNoVacia(temp_str, 2)) {
                    printf("Error: El tipo debe tener al menos 2 caracteres.\n");
                } else {
                    strcpy(aulas[i].tipo, temp_str);
                    break;
                }
            } while (1);

            guardarAulas(aulas, total);
            printf("Aula actualizada.\n");
            pausar();
            return;
        }
    }
    printf("Aula no encontrada.\n");
    pausar();
}

/* Elimina (desactiva) un aula */
void eliminarAula(Usuario usuario_actual) {
    Aula aulas[MAX_AULAS];
    int total, i;
    char id_buscar[20];

    if (usuario_actual.tipo != 1) {
        printf("Sin permisos para eliminar aulas.\n");
        pausar();
        return;
    }

    cargarAulas(aulas, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del aula a eliminar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(aulas[i].id, id_buscar) == 0) {
            char msg[100];
            sprintf(msg, "Desea eliminar el aula %s? (s/n): ", aulas[i].nombre);
            if (solicitarConfirmacion(msg)) {
                aulas[i].activo = 0;
                guardarAulas(aulas, total);
                printf("Aula desactivada.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Aula no encontrada.\n");
    pausar();
}
