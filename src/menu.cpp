#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "utilidades.h"
#include "alumnos.h"
#include "profesores.h"
#include "materias.h"
#include "notas.h"
#include "trabajos.h"
#include "asistencias.h"
#include "horarios.h"
#include "aulas.h"
#include "periodos.h"
#include "matriculas.h"
#include "reportes.h"
#include "usuarios.h"

void menuPrincipal(Usuario usuario_actual) {
    if (usuario_actual.tipo == 1) menuAdmin(usuario_actual);
    else if (usuario_actual.tipo == 2) menuProfesor(usuario_actual);
    else menuAlumno(usuario_actual);
}

void menuAdmin(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== MENU ADMINISTRADOR ===\n");
        printf("Usuario: %s\n\n", usuario_actual.username);
        printf("1. Alumnos\n");
        printf("2. Profesores\n");
        printf("3. Materias\n");
        printf("4. Matriculas\n");
        printf("5. Notas\n");
        printf("6. Trabajos\n");
        printf("7. Asistencias\n");
        printf("8. Horarios\n");
        printf("9. Aulas\n");
        printf("10. Periodos\n");
        printf("11. Usuarios\n");
        printf("12. Reportes\n");
        printf("0. Salir\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: menuAlumnos(usuario_actual); break;
            case 2: menuProfesores(usuario_actual); break;
            case 3: menuMaterias(usuario_actual); break;
            case 4: menuMatriculas(usuario_actual); break;
            case 5: menuNotas(usuario_actual); break;
            case 6: menuTrabajos(usuario_actual); break;
            case 7: menuAsistencias(usuario_actual); break;
            case 8: menuHorarios(usuario_actual); break;
            case 9: menuAulas(usuario_actual); break;
            case 10: menuPeriodos(usuario_actual); break;
            case 11: menuUsuarios(usuario_actual); break;
            case 12: menuReportes(usuario_actual); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

void menuProfesor(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== MENU PROFESOR ===\n");
        printf("Usuario: %s\n\n", usuario_actual.username);
        printf("1. Ver alumnos\n");
        printf("2. Ver materias\n");
        printf("3. Gestionar notas\n");
        printf("4. Gestionar trabajos\n");
        printf("5. Registrar asistencia\n");
        printf("6. Ver horarios\n");
        printf("7. Reportes\n");
        printf("0. Salir\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarAlumnos(); break;
            case 2: listarMaterias(); break;
            case 3: menuNotas(usuario_actual); break;
            case 4: menuTrabajos(usuario_actual); break;
            case 5: menuAsistencias(usuario_actual); break;
            case 6: listarHorarios(); break;
            case 7: menuReportes(usuario_actual); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

void menuAlumno(Usuario usuario_actual) {
    int opcion;
    do {
        limpiarPantalla();
        printf("=== MENU ALUMNO ===\n");
        printf("Usuario: %s\n\n", usuario_actual.username);
        printf("1. Ver mi perfil\n");
        printf("2. Ver mis notas\n");
        printf("3. Ver mis materias\n");
        printf("4. Ver mis trabajos\n");
        printf("5. Ver mi asistencia\n");
        printf("6. Ver horarios\n");
        printf("0. Salir\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: verMiPerfil(usuario_actual.id_referencia); break;
            case 2: verNotasAlumno(usuario_actual.id_referencia); break;
            case 3: verMatriculasAlumno(usuario_actual.id_referencia); break;
            case 4: verEntregasAlumno(usuario_actual.id_referencia); break;
            case 5: verAsistenciasAlumno(usuario_actual.id_referencia); break;
            case 6: listarHorarios(); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}
