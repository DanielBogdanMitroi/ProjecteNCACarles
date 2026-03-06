#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "materias.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de materias */
void menuMaterias(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE MATERIAS ===\n");
        printf("1. Listar materias\n");
        printf("2. Buscar materia\n");
        if (usuario_actual.tipo == 1) {
            printf("3. Agregar materia\n");
            printf("4. Editar materia\n");
            printf("5. Eliminar materia\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarMaterias(); break;
            case 2: buscarMateria(); break;
            case 3:
                if (usuario_actual.tipo == 1) agregarMateria();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 4:
                if (usuario_actual.tipo == 1) editarMateria();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 5:
                if (usuario_actual.tipo == 1) eliminarMateria();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todas las materias activas */
void listarMaterias() {
    Materia materias[MAX_MATERIAS];
    int total, i, activos = 0;
    cargarMaterias(materias, &total);
    limpiarPantalla();
    printf("=== LISTA DE MATERIAS ===\n");
    printf("Total de registros en archivo: %d\n", total);
    printf("%-10s %-30s %-10s %-15s\n", "ID", "Nombre", "Creditos", "Horas/Semana");
    printf("%-10s %-30s %-10s %-15s\n", "------", "------", "--------", "------------");
    for (i = 0; i < total; i++) {
        if (materias[i].activo) {
            printf("%-10s %-30s %-10d %-15d\n",
                   materias[i].id, materias[i].nombre,
                   materias[i].creditos, materias[i].horas_semanales);
            activos++;
        }
    }
    if (activos == 0) {
        if (total == 0) {
            printf("\nNo hay materias registradas en el sistema.\n");
            printf("Usa la opcion 'Agregar materia' para crear el primer registro.\n");
        } else {
            printf("\nTodas las materias estan marcadas como inactivas.\n");
            printf("Total de registros inactivos: %d\n", total);
        }
    } else {
        printf("\nTotal de materias activas: %d\n", activos);
        if (total > activos) {
            printf("Materias inactivas: %d\n", total - activos);
        }
    }
    pausar();
}

/* Busca una materia por ID o nombre */
void buscarMateria() {
    Materia materias[MAX_MATERIAS];
    int total, i, encontrado = 0;
    char busqueda[50];
    cargarMaterias(materias, &total);
    do {
        leerString(busqueda, sizeof(busqueda), "Ingrese ID o nombre de la materia: ");
        if (!validarCadenaNoVacia(busqueda, 1)) {
            printf("Error: La busqueda no puede estar vacia.\n");
        }
    } while (!validarCadenaNoVacia(busqueda, 1));
    limpiarPantalla();
    printf("=== RESULTADOS DE BUSQUEDA ===\n");
    for (i = 0; i < total; i++) {
        if (strcmp(materias[i].id, busqueda) == 0 ||
            strstr(materias[i].nombre, busqueda) != NULL) {
            printf("ID: %s\n", materias[i].id);
            printf("Nombre: %s\n", materias[i].nombre);
            printf("Descripcion: %s\n", materias[i].descripcion);
            printf("Creditos: %d\n", materias[i].creditos);
            printf("Horas semanales: %d\n", materias[i].horas_semanales);
            printf("Estado: %s\n", materias[i].activo ? "Activa" : "Inactiva");
            printf("---\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No se encontraron resultados.\n");
    pausar();
}

/* Agrega una nueva materia */
void agregarMateria() {
    Materia materias[MAX_MATERIAS];
    int total;
    Materia nueva;
    memset(&nueva, 0, sizeof(Materia));

    cargarMaterias(materias, &total);
    generarID(nueva.id, "MAT", obtenerSiguienteNumeroID(ARCHIVO_MATERIAS, "MAT"));

    printf("=== AGREGAR MATERIA ===\n");
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
        leerString(nueva.descripcion, sizeof(nueva.descripcion), "Descripcion: ");
        if (strlen(nueva.descripcion) == 0) {
            printf("\nERROR: La descripcion no puede estar vacia.\n");
            printf("Por favor, introduce una descripcion valida.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nueva.descripcion, 3)) {
            printf("\nERROR: La descripcion debe tener al menos 3 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nueva.descripcion);
            continue;
        }
        break;
    } while (1);

    do {
        leerEntero(&nueva.creditos, "Creditos: ");
        if (!validarCreditos(nueva.creditos)) {
            printf("Error: Numero de creditos invalido (1-20).\n");
        }
    } while (!validarCreditos(nueva.creditos));

    do {
        leerEntero(&nueva.horas_semanales, "Horas semanales: ");
        if (nueva.horas_semanales < 1 || nueva.horas_semanales > 40) {
            printf("Error: Horas semanales invalidas (1-40).\n");
        }
    } while (nueva.horas_semanales < 1 || nueva.horas_semanales > 40);

    nueva.activo = 1;

    materias[total] = nueva;
    total++;
    guardarMaterias(materias, total);
    printf("Materia %s agregada exitosamente.\n", nueva.id);
    pausar();
}

/* Edita una materia existente */
void editarMateria() {
    Materia materias[MAX_MATERIAS];
    int total, i;
    char id_buscar[20];
    char temp_str[200];
    int temp_int;

    cargarMaterias(materias, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID de la materia a editar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(materias[i].id, id_buscar) == 0) {
            printf("=== EDITAR MATERIA %s ===\n", materias[i].id);
            printf("(Presione Enter para mantener el valor actual)\n\n");

            mostrarValorActual("Nombre actual", materias[i].nombre);
            do {
                leerString(temp_str, sizeof(temp_str), "Nuevo nombre: ");
                if (strlen(temp_str) == 0) break;
                if (!validarCadenaNoVacia(temp_str, 2)) {
                    printf("Error: El nombre debe tener al menos 2 caracteres.\n");
                } else {
                    strcpy(materias[i].nombre, temp_str);
                    break;
                }
            } while (1);

            mostrarValorActual("Descripcion actual", materias[i].descripcion);
            do {
                leerString(temp_str, sizeof(temp_str), "Nueva descripcion: ");
                if (strlen(temp_str) == 0) break;
                if (!validarCadenaNoVacia(temp_str, 3)) {
                    printf("Error: La descripcion debe tener al menos 3 caracteres.\n");
                } else {
                    strcpy(materias[i].descripcion, temp_str);
                    break;
                }
            } while (1);

            mostrarValorActualEntero("Creditos actuales", materias[i].creditos);
            printf("Nuevos creditos (0 para mantener): ");
            leerEntero(&temp_int, "");
            if (temp_int != 0) {
                do {
                    if (!validarCreditos(temp_int)) {
                        printf("Error: Numero de creditos invalido (1-20).\n");
                        leerEntero(&temp_int, "Nuevos creditos: ");
                    } else {
                        materias[i].creditos = temp_int;
                        break;
                    }
                } while (!validarCreditos(temp_int));
            }

            mostrarValorActualEntero("Horas semanales actuales", materias[i].horas_semanales);
            printf("Nuevas horas semanales (0 para mantener): ");
            leerEntero(&temp_int, "");
            if (temp_int != 0) {
                do {
                    if (temp_int < 1 || temp_int > 40) {
                        printf("Error: Horas semanales invalidas (1-40).\n");
                        leerEntero(&temp_int, "Nuevas horas semanales: ");
                    } else {
                        materias[i].horas_semanales = temp_int;
                        break;
                    }
                } while (temp_int < 1 || temp_int > 40);
            }

            guardarMaterias(materias, total);
            printf("Materia actualizada.\n");
            pausar();
            return;
        }
    }
    printf("Materia no encontrada.\n");
    pausar();
}

/* Elimina (desactiva) una materia */
void eliminarMateria() {
    Materia materias[MAX_MATERIAS];
    int total, i;
    char id_buscar[20];

    cargarMaterias(materias, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID de la materia a eliminar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(materias[i].id, id_buscar) == 0) {
            char msg[200];
            sprintf(msg, "Desea eliminar la materia %s? (s/n): ", materias[i].nombre);
            if (solicitarConfirmacion(msg)) {
                materias[i].activo = 0;
                guardarMaterias(materias, total);
                printf("Materia desactivada.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Materia no encontrada.\n");
    pausar();
}
