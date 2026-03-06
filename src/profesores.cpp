#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "profesores.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de profesores */
void menuProfesores(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE PROFESORES ===\n");
        printf("1. Listar profesores\n");
        printf("2. Buscar profesor\n");
        if (usuario_actual.tipo == 1) {
            printf("3. Agregar profesor\n");
            printf("4. Editar profesor\n");
            printf("5. Eliminar profesor\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarProfesores(); break;
            case 2: buscarProfesor(); break;
            case 3:
                if (usuario_actual.tipo == 1) agregarProfesor();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 4:
                if (usuario_actual.tipo == 1) editarProfesor();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 5:
                if (usuario_actual.tipo == 1) eliminarProfesor();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los profesores activos */
void listarProfesores() {
    Profesor profesores[MAX_PROFESORES];
    int total, i, activos = 0;
    cargarProfesores(profesores, &total);
    limpiarPantalla();
    printf("=== LISTA DE PROFESORES ===\n");
    printf("Total de registros en archivo: %d\n", total);
    printf("%-10s %-20s %-20s %-25s\n", "ID", "Nombre", "Apellido", "Especialidad");
    printf("%-10s %-20s %-20s %-25s\n", "------", "------", "-------", "------------");
    for (i = 0; i < total; i++) {
        if (profesores[i].activo) {
            printf("%-10s %-20s %-20s %-25s\n",
                   profesores[i].id, profesores[i].nombre,
                   profesores[i].apellido, profesores[i].especialidad);
            activos++;
        }
    }
    if (activos == 0) {
        if (total == 0) {
            printf("\nNo hay profesores registrados en el sistema.\n");
            printf("Usa la opcion 'Agregar profesor' para crear el primer registro.\n");
        } else {
            printf("\nTodos los profesores estan marcados como inactivos.\n");
            printf("Total de registros inactivos: %d\n", total);
        }
    } else {
        printf("\nTotal de profesores activos: %d\n", activos);
        if (total > activos) {
            printf("Profesores inactivos: %d\n", total - activos);
        }
    }
    pausar();
}

/* Busca un profesor por ID o nombre */
void buscarProfesor() {
    Profesor profesores[MAX_PROFESORES];
    int total, i, encontrado = 0;
    char busqueda[50];
    cargarProfesores(profesores, &total);
    do {
        leerString(busqueda, sizeof(busqueda), "Ingrese ID o nombre del profesor: ");
        if (!validarCadenaNoVacia(busqueda, 1)) {
            printf("Error: La busqueda no puede estar vacia.\n");
        }
    } while (!validarCadenaNoVacia(busqueda, 1));
    limpiarPantalla();
    printf("=== RESULTADOS DE BUSQUEDA ===\n");
    for (i = 0; i < total; i++) {
        if (strcmp(profesores[i].id, busqueda) == 0 ||
            strstr(profesores[i].nombre, busqueda) != NULL ||
            strstr(profesores[i].apellido, busqueda) != NULL) {
            printf("ID: %s\n", profesores[i].id);
            printf("Nombre: %s %s\n", profesores[i].nombre, profesores[i].apellido);
            printf("Email: %s\n", profesores[i].email);
            printf("Especialidad: %s\n", profesores[i].especialidad);
            printf("Telefono: %s\n", profesores[i].telefono);
            printf("Estado: %s\n", profesores[i].activo ? "Activo" : "Inactivo");
            printf("---\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No se encontraron resultados.\n");
    pausar();
}

/* Agrega un nuevo profesor */
void agregarProfesor() {
    Profesor profesores[MAX_PROFESORES];
    int total;
    Profesor nuevo;
    memset(&nuevo, 0, sizeof(Profesor));

    cargarProfesores(profesores, &total);
    generarID(nuevo.id, "PROF", obtenerSiguienteNumeroID(ARCHIVO_PROFESORES, "PROF"));

    printf("=== AGREGAR PROFESOR ===\n");
    printf("ID generado: %s\n", nuevo.id);

    do {
        leerString(nuevo.nombre, sizeof(nuevo.nombre), "Nombre: ");
        if (strlen(nuevo.nombre) == 0) {
            printf("\nERROR: El nombre no puede estar vacio.\n");
            printf("Por favor, introduce un nombre valido.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.nombre, 2)) {
            printf("\nERROR: El nombre debe tener al menos 2 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nuevo.nombre);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.apellido, sizeof(nuevo.apellido), "Apellido: ");
        if (strlen(nuevo.apellido) == 0) {
            printf("\nERROR: El apellido no puede estar vacio.\n");
            printf("Por favor, introduce un apellido valido.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.apellido, 2)) {
            printf("\nERROR: El apellido debe tener al menos 2 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nuevo.apellido);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.email, sizeof(nuevo.email), "Email: ");
        if (strlen(nuevo.email) == 0) {
            printf("\nERROR: El email no puede estar vacio.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.email, 5)) {
            printf("\nERROR: El email debe tener al menos 5 caracteres.\n\n");
            continue;
        }
        if (!validarEmail(nuevo.email)) {
            printf("\nERROR: Formato de email invalido.\n");
            printf("Formato correcto: usuario@dominio.com\n");
            printf("Email introducido: '%s'\n\n", nuevo.email);
            continue;
        }
        if (emailYaExiste(nuevo.email, ARCHIVO_PROFESORES, "")) {
            printf("\nERROR: Ya existe un profesor con ese email.\n");
            printf("Email duplicado: '%s'\n\n", nuevo.email);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.especialidad, sizeof(nuevo.especialidad), "Especialidad: ");
        if (strlen(nuevo.especialidad) == 0) {
            printf("\nERROR: La especialidad no puede estar vacia.\n");
            printf("Por favor, introduce una especialidad valida.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.especialidad, 3)) {
            printf("\nERROR: La especialidad debe tener al menos 3 caracteres.\n");
            printf("Valor introducido: '%s'\n\n", nuevo.especialidad);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.telefono, sizeof(nuevo.telefono), "Telefono: ");
        if (strlen(nuevo.telefono) == 0) {
            printf("\nERROR: El telefono no puede estar vacio.\n\n");
            continue;
        }
        if (!validarTelefono(nuevo.telefono)) {
            printf("\nERROR: El telefono debe contener al menos 9 digitos.\n");
            printf("Telefono introducido: '%s'\n\n", nuevo.telefono);
            continue;
        }
        break;
    } while (1);

    nuevo.activo = 1;

    profesores[total] = nuevo;
    total++;
    guardarProfesores(profesores, total);
    printf("Profesor %s agregado exitosamente.\n", nuevo.id);
    pausar();
}

/* Edita un profesor existente */
void editarProfesor() {
    Profesor profesores[MAX_PROFESORES];
    int total, i;
    char id_buscar[20];
    char temp[200];
    int email_valido, telefono_valido;

    cargarProfesores(profesores, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del profesor a editar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(profesores[i].id, id_buscar) == 0) {
            printf("=== EDITAR PROFESOR %s ===\n", profesores[i].id);
            printf("(Presione Enter para mantener el valor actual)\n\n");

            mostrarValorActual("Nombre actual", profesores[i].nombre);
            do {
                leerString(temp, sizeof(temp), "Nuevo nombre: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 2)) {
                    printf("Error: El nombre debe tener al menos 2 caracteres.\n");
                } else {
                    strcpy(profesores[i].nombre, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Apellido actual", profesores[i].apellido);
            do {
                leerString(temp, sizeof(temp), "Nuevo apellido: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 2)) {
                    printf("Error: El apellido debe tener al menos 2 caracteres.\n");
                } else {
                    strcpy(profesores[i].apellido, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Email actual", profesores[i].email);
            email_valido = 0;
            do {
                leerString(temp, sizeof(temp), "Nuevo email: ");
                if (strlen(temp) == 0) { email_valido = 1; break; }
                if (!validarEmail(temp)) {
                    printf("Error: Formato de email invalido. Formato correcto: usuario@dominio.com\n");
                } else if (emailYaExiste(temp, ARCHIVO_PROFESORES, profesores[i].id)) {
                    printf("Error: Ya existe un profesor con ese email.\n");
                } else {
                    strcpy(profesores[i].email, temp);
                    email_valido = 1;
                }
            } while (!email_valido);

            mostrarValorActual("Especialidad actual", profesores[i].especialidad);
            do {
                leerString(temp, sizeof(temp), "Nueva especialidad: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 2)) {
                    printf("Error: La especialidad debe tener al menos 2 caracteres.\n");
                } else {
                    strcpy(profesores[i].especialidad, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Telefono actual", profesores[i].telefono);
            telefono_valido = 0;
            do {
                leerString(temp, sizeof(temp), "Nuevo telefono: ");
                if (strlen(temp) == 0) { telefono_valido = 1; break; }
                if (!validarTelefono(temp)) {
                    printf("Error: Formato de telefono invalido.\n");
                } else {
                    strcpy(profesores[i].telefono, temp);
                    telefono_valido = 1;
                }
            } while (!telefono_valido);

            guardarProfesores(profesores, total);
            printf("Profesor actualizado.\n");
            pausar();
            return;
        }
    }
    printf("Profesor no encontrado.\n");
    pausar();
}

/* Elimina (desactiva) un profesor */
void eliminarProfesor() {
    Profesor profesores[MAX_PROFESORES];
    int total, i;
    char id_buscar[20];

    cargarProfesores(profesores, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del profesor a eliminar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(profesores[i].id, id_buscar) == 0) {
            char msg[200];
            sprintf(msg, "Desea eliminar a %s %s? (s/n): ", profesores[i].nombre, profesores[i].apellido);
            if (solicitarConfirmacion(msg)) {
                profesores[i].activo = 0;
                guardarProfesores(profesores, total);
                printf("Profesor desactivado.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Profesor no encontrado.\n");
    pausar();
}
