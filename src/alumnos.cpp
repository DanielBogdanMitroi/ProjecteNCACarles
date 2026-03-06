#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alumnos.h"
#include "archivos.h"
#include "utilidades.h"
#include "validaciones.h"

/* Menu principal de gestion de alumnos */
void menuAlumnos(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== GESTION DE ALUMNOS ===\n");
        printf("1. Listar alumnos\n");
        printf("2. Buscar alumno\n");
        if (usuario_actual.tipo == 1) {
            printf("3. Agregar alumno\n");
            printf("4. Editar alumno\n");
            printf("5. Eliminar alumno\n");
        }
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarAlumnos(); break;
            case 2: buscarAlumno(); break;
            case 3:
                if (usuario_actual.tipo == 1) agregarAlumno();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 4:
                if (usuario_actual.tipo == 1) editarAlumno();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 5:
                if (usuario_actual.tipo == 1) eliminarAlumno();
                else { printf("Sin permisos.\n"); pausar(); }
                break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los alumnos activos */
void listarAlumnos() {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i, activos = 0;
    cargarAlumnos(alumnos, &total);
    limpiarPantalla();
    printf("=== LISTA DE ALUMNOS ===\n");
    printf("Total de registros en archivo: %d\n", total);
    printf("%-10s %-20s %-20s %-30s\n", "ID", "Nombre", "Apellido", "Email");
    printf("%-10s %-20s %-20s %-30s\n", "------", "------", "-------", "-----");
    for (i = 0; i < total; i++) {
        if (alumnos[i].activo) {
            printf("%-10s %-20s %-20s %-30s\n",
                   alumnos[i].id, alumnos[i].nombre, alumnos[i].apellido, alumnos[i].email);
            activos++;
        }
    }
    if (activos == 0) {
        if (total == 0) {
            printf("\nNo hay alumnos registrados en el sistema.\n");
            printf("Usa la opcion 'Agregar alumno' para crear el primer registro.\n");
        } else {
            printf("\nTodos los alumnos estan marcados como inactivos.\n");
            printf("Total de registros inactivos: %d\n", total);
        }
    } else {
        printf("\nTotal de alumnos activos: %d\n", activos);
        if (total > activos) {
            printf("Alumnos inactivos: %d\n", total - activos);
        }
    }
    pausar();
}

/* Busca un alumno por ID o nombre */
void buscarAlumno() {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i, encontrado = 0;
    char busqueda[50];
    cargarAlumnos(alumnos, &total);
    do {
        leerString(busqueda, sizeof(busqueda), "Ingrese ID o nombre del alumno: ");
        if (!validarCadenaNoVacia(busqueda, 1)) {
            printf("Error: La busqueda no puede estar vacia.\n");
        }
    } while (!validarCadenaNoVacia(busqueda, 1));
    limpiarPantalla();
    printf("=== RESULTADOS DE BUSQUEDA ===\n");
    for (i = 0; i < total; i++) {
        if (strcmp(alumnos[i].id, busqueda) == 0 ||
            strstr(alumnos[i].nombre, busqueda) != NULL ||
            strstr(alumnos[i].apellido, busqueda) != NULL) {
            printf("ID: %s\n", alumnos[i].id);
            printf("Nombre: %s %s\n", alumnos[i].nombre, alumnos[i].apellido);
            printf("Email: %s\n", alumnos[i].email);
            printf("Telefono: %s\n", alumnos[i].telefono);
            printf("Fecha Nacimiento: %s\n", alumnos[i].fecha_nacimiento);
            printf("Direccion: %s\n", alumnos[i].direccion);
            printf("Estado: %s\n", alumnos[i].activo ? "Activo" : "Inactivo");
            printf("---\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No se encontraron resultados.\n");
    pausar();
}

/* Agrega un nuevo alumno */
void agregarAlumno() {
    Alumno alumnos[MAX_ALUMNOS];
    int total;
    Alumno nuevo;
    memset(&nuevo, 0, sizeof(Alumno));

    cargarAlumnos(alumnos, &total);
    generarID(nuevo.id, "ALU", total + 1);

    printf("=== AGREGAR ALUMNO ===\n");
    printf("ID generado: %s\n", nuevo.id);

    do {
        leerString(nuevo.nombre, sizeof(nuevo.nombre), "Nombre: ");

        // DEBUG: Show what was read
        printf("[DEBUG] Valor leido: '%s'\n", nuevo.nombre);
        printf("[DEBUG] strlen(nuevo.nombre) = %d\n", (int)strlen(nuevo.nombre));
        printf("[DEBUG] Bytes: ");
        for (int i = 0; i < (int)strlen(nuevo.nombre) && i < 20; i++) {
            printf("%02X ", (unsigned char)nuevo.nombre[i]);
        }
        printf("\n");

        if (strlen(nuevo.nombre) == 0) {
            printf("[DEBUG] Entrando en if (strlen == 0)\n");
            printf("\nERROR: El nombre no puede estar vacio.\n");
            printf("   Por favor, introduce un nombre valido.\n\n");
            continue;
        }
        printf("[DEBUG] Paso la validacion strlen == 0\n");

        if (!validarNombreApellido(nuevo.nombre, 2)) {
            printf("[DEBUG] validarNombreApellido retorno FALSE\n");
            printf("\nERROR: El nombre debe tener al menos 2 letras.\n");
            printf("   No se permiten numeros ni caracteres especiales.\n");
            printf("   Nombre introducido: '%s'\n\n", nuevo.nombre);
            continue;
        }
        printf("[DEBUG] validarNombreApellido retorno TRUE\n");

        break;
    } while (1);

    do {
        leerString(nuevo.apellido, sizeof(nuevo.apellido), "Apellido: ");

        printf("[DEBUG] Apellido leido: '%s' (len=%d)\n", nuevo.apellido, (int)strlen(nuevo.apellido));

        if (strlen(nuevo.apellido) == 0) {
            printf("[DEBUG] Apellido vacio detectado\n");
            printf("\nERROR: El apellido no puede estar vacio.\n");
            printf("   Por favor, introduce un apellido valido.\n\n");
            continue;
        }
        if (!validarNombreApellido(nuevo.apellido, 2)) {
            printf("[DEBUG] validarNombreApellido(apellido) = FALSE\n");
            printf("\nERROR: El apellido debe tener al menos 2 letras.\n");
            printf("   No se permiten numeros ni caracteres especiales.\n");
            printf("   Apellido introducido: '%s'\n\n", nuevo.apellido);
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
            printf("   Formato correcto: usuario@dominio.com\n");
            printf("   Email introducido: '%s'\n\n", nuevo.email);
            continue;
        }
        if (emailYaExiste(nuevo.email, ARCHIVO_ALUMNOS, "")) {
            printf("\nERROR: Ya existe un alumno con ese email.\n");
            printf("   Email duplicado: '%s'\n\n", nuevo.email);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.fecha_nacimiento, sizeof(nuevo.fecha_nacimiento), "Fecha nacimiento (YYYY-MM-DD): ");
        if (strlen(nuevo.fecha_nacimiento) == 0) {
            printf("\nERROR: La fecha no puede estar vacia.\n\n");
            continue;
        }
        if (strlen(nuevo.fecha_nacimiento) != 10) {
            printf("\nERROR: Formato de fecha invalido.\n");
            printf("   Formato correcto: YYYY-MM-DD (10 caracteres)\n");
            printf("   Fecha introducida: '%s' (longitud: %d)\n\n",
                   nuevo.fecha_nacimiento, (int)strlen(nuevo.fecha_nacimiento));
            continue;
        }
        if (!validarFecha(nuevo.fecha_nacimiento)) {
            printf("\nERROR: Fecha invalida.\n");
            printf("   Formato correcto: YYYY-MM-DD\n");
            printf("   Ejemplo: 2000-05-15\n");
            printf("   Fecha introducida: '%s'\n\n", nuevo.fecha_nacimiento);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.telefono, sizeof(nuevo.telefono), "Telefono: ");

        printf("[DEBUG] Telefono leido: '%s' (len=%d)\n", nuevo.telefono, (int)strlen(nuevo.telefono));

        if (strlen(nuevo.telefono) == 0) {
            printf("[DEBUG] Telefono vacio detectado\n");
            printf("\nERROR: El telefono no puede estar vacio.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.telefono, 9)) {
            printf("[DEBUG] validarCadenaNoVacia(telefono, 9) = FALSE\n");
            printf("\nERROR: El telefono debe tener al menos 9 caracteres.\n");
            printf("   Telefono introducido: '%s' (longitud: %d)\n\n",
                   nuevo.telefono, (int)strlen(nuevo.telefono));
            continue;
        }
        if (!validarTelefono(nuevo.telefono)) {
            printf("[DEBUG] validarTelefono() = FALSE\n");
            printf("\nERROR: Formato de telefono invalido.\n");
            printf("   Debe contener al menos 9 digitos.\n");
            printf("   Telefono introducido: '%s'\n\n", nuevo.telefono);
            continue;
        }
        break;
    } while (1);

    do {
        leerString(nuevo.direccion, sizeof(nuevo.direccion), "Direccion: ");

        printf("[DEBUG] Direccion leida: '%s' (len=%d)\n", nuevo.direccion, (int)strlen(nuevo.direccion));

        if (strlen(nuevo.direccion) == 0) {
            printf("[DEBUG] Direccion vacia detectada\n");
            printf("\nERROR: La direccion no puede estar vacia.\n\n");
            continue;
        }
        if (!validarCadenaNoVacia(nuevo.direccion, 5)) {
            printf("[DEBUG] validarCadenaNoVacia(direccion, 5) = FALSE\n");
            printf("\nERROR: La direccion debe tener al menos 5 caracteres.\n");
            printf("   Direccion introducida: '%s'\n\n", nuevo.direccion);
            continue;
        }
        break;
    } while (1);

    nuevo.activo = 1;

    alumnos[total] = nuevo;
    total++;
    guardarAlumnos(alumnos, total);
    printf("Alumno %s agregado exitosamente.\n", nuevo.id);
    pausar();
}

/* Edita un alumno existente */
void editarAlumno() {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i;
    char id_buscar[20];
    char temp[200];
    int email_valido, telefono_valido, fecha_valida;

    cargarAlumnos(alumnos, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del alumno a editar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(alumnos[i].id, id_buscar) == 0) {
            printf("=== EDITAR ALUMNO %s ===\n", alumnos[i].id);
            printf("(Presione Enter para mantener el valor actual)\n\n");

            mostrarValorActual("Nombre actual", alumnos[i].nombre);
            do {
                leerString(temp, sizeof(temp), "Nuevo nombre: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 2)) {
                    printf("Error: El nombre debe tener al menos 2 caracteres.\n");
                } else {
                    strcpy(alumnos[i].nombre, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Apellido actual", alumnos[i].apellido);
            do {
                leerString(temp, sizeof(temp), "Nuevo apellido: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 2)) {
                    printf("Error: El apellido debe tener al menos 2 caracteres.\n");
                } else {
                    strcpy(alumnos[i].apellido, temp);
                    break;
                }
            } while (1);

            mostrarValorActual("Email actual", alumnos[i].email);
            email_valido = 0;
            do {
                leerString(temp, sizeof(temp), "Nuevo email: ");
                if (strlen(temp) == 0) { email_valido = 1; break; }
                if (!validarEmail(temp)) {
                    printf("Error: Formato de email invalido. Formato correcto: usuario@dominio.com\n");
                } else if (emailYaExiste(temp, ARCHIVO_ALUMNOS, alumnos[i].id)) {
                    printf("Error: Ya existe un alumno con ese email.\n");
                } else {
                    strcpy(alumnos[i].email, temp);
                    email_valido = 1;
                }
            } while (!email_valido);

            mostrarValorActual("Fecha nacimiento actual", alumnos[i].fecha_nacimiento);
            fecha_valida = 0;
            do {
                leerString(temp, sizeof(temp), "Nueva fecha nacimiento (YYYY-MM-DD): ");
                if (strlen(temp) == 0) { fecha_valida = 1; break; }
                if (!validarFecha(temp)) {
                    printf("Error: Fecha invalida. Formato: YYYY-MM-DD\n");
                } else {
                    strcpy(alumnos[i].fecha_nacimiento, temp);
                    fecha_valida = 1;
                }
            } while (!fecha_valida);

            mostrarValorActual("Telefono actual", alumnos[i].telefono);
            telefono_valido = 0;
            do {
                leerString(temp, sizeof(temp), "Nuevo telefono: ");
                if (strlen(temp) == 0) { telefono_valido = 1; break; }
                if (!validarTelefono(temp)) {
                    printf("Error: Formato de telefono invalido.\n");
                } else {
                    strcpy(alumnos[i].telefono, temp);
                    telefono_valido = 1;
                }
            } while (!telefono_valido);

            mostrarValorActual("Direccion actual", alumnos[i].direccion);
            do {
                leerString(temp, sizeof(temp), "Nueva direccion: ");
                if (strlen(temp) == 0) break;
                if (!validarCadenaNoVacia(temp, 5)) {
                    printf("Error: La direccion debe tener al menos 5 caracteres.\n");
                } else {
                    strcpy(alumnos[i].direccion, temp);
                    break;
                }
            } while (1);

            guardarAlumnos(alumnos, total);
            printf("Alumno actualizado.\n");
            pausar();
            return;
        }
    }
    printf("Alumno no encontrado.\n");
    pausar();
}

/* Elimina (desactiva) un alumno */
void eliminarAlumno() {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i;
    char id_buscar[20];

    cargarAlumnos(alumnos, &total);
    do {
        leerString(id_buscar, sizeof(id_buscar), "ID del alumno a eliminar: ");
        if (!validarCadenaNoVacia(id_buscar, 1)) {
            printf("Error: El ID no puede estar vacio.\n");
        }
    } while (!validarCadenaNoVacia(id_buscar, 1));

    for (i = 0; i < total; i++) {
        if (strcmp(alumnos[i].id, id_buscar) == 0) {
            char msg[200];
            sprintf(msg, "Desea eliminar a %s %s? (s/n): ", alumnos[i].nombre, alumnos[i].apellido);
            if (solicitarConfirmacion(msg)) {
                alumnos[i].activo = 0;
                guardarAlumnos(alumnos, total);
                printf("Alumno desactivado.\n");
            } else {
                printf("Operacion cancelada.\n");
            }
            pausar();
            return;
        }
    }
    printf("Alumno no encontrado.\n");
    pausar();
}

/* Ver perfil del alumno logueado */
void verMiPerfil(const char id_alumno[]) {
    Alumno alumnos[MAX_ALUMNOS];
    int total, i;
    cargarAlumnos(alumnos, &total);
    limpiarPantalla();
    for (i = 0; i < total; i++) {
        if (strcmp(alumnos[i].id, id_alumno) == 0) {
            printf("=== MI PERFIL ===\n");
            printf("ID: %s\n", alumnos[i].id);
            printf("Nombre: %s %s\n", alumnos[i].nombre, alumnos[i].apellido);
            printf("Email: %s\n", alumnos[i].email);
            printf("Telefono: %s\n", alumnos[i].telefono);
            printf("Fecha Nacimiento: %s\n", alumnos[i].fecha_nacimiento);
            printf("Direccion: %s\n", alumnos[i].direccion);
            pausar();
            return;
        }
    }
    printf("Perfil no encontrado.\n");
    pausar();
}
