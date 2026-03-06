#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#define MAX_ALUMNOS 1000
#define MAX_PROFESORES 500
#define MAX_MATERIAS 200
#define MAX_NOTAS 5000
#define MAX_TRABAJOS 1000
#define MAX_ENTREGAS 5000
#define MAX_ASISTENCIAS 10000
#define MAX_HORARIOS 500
#define MAX_AULAS 100
#define MAX_PERIODOS 50
#define MAX_MATRICULAS 5000
#define MAX_USUARIOS 200
#define MAX_PROFESOR_MATERIA 1000

#ifdef _WIN32
    #define DATA_DIR "..\\datos\\"
#else
    #define DATA_DIR "../datos/"
#endif

#define ARCHIVO_ALUMNOS DATA_DIR "alumnos.txt"
#define ARCHIVO_PROFESORES DATA_DIR "profesores.txt"
#define ARCHIVO_MATERIAS DATA_DIR "materias.txt"
#define ARCHIVO_NOTAS DATA_DIR "notas.txt"
#define ARCHIVO_TRABAJOS DATA_DIR "trabajos.txt"
#define ARCHIVO_ENTREGAS DATA_DIR "entregas.txt"
#define ARCHIVO_ASISTENCIAS DATA_DIR "asistencias.txt"
#define ARCHIVO_HORARIOS DATA_DIR "horarios.txt"
#define ARCHIVO_AULAS DATA_DIR "aulas.txt"
#define ARCHIVO_PERIODOS DATA_DIR "periodos.txt"
#define ARCHIVO_MATRICULAS DATA_DIR "matriculas.txt"
#define ARCHIVO_PROF_MATERIA DATA_DIR "profesor_materia.txt"
#define ARCHIVO_USUARIOS DATA_DIR "usuarios.txt"

struct Usuario {
    char username[50];
    char password[100];
    int tipo;
    char id_referencia[20];
};

struct Alumno {
    char id[20];
    char nombre[50];
    char apellido[50];
    char email[100];
    char fecha_nacimiento[11];
    char telefono[15];
    char direccion[200];
    int activo;
};

struct Profesor {
    char id[20];
    char nombre[50];
    char apellido[50];
    char email[100];
    char especialidad[100];
    char telefono[15];
    int activo;
};

struct Materia {
    char id[20];
    char nombre[100];
    char descripcion[200];
    int creditos;
    int horas_semanales;
    int activo;
};

struct Matricula {
    char id[20];
    char id_alumno[20];
    char id_materia[20];
    char fecha_matricula[11];
    char periodo[20];
    int activo;
};

struct Profesor_Materia {
    char id[20];
    char id_profesor[20];
    char id_materia[20];
    char periodo[20];
    int activo;
};

struct Nota {
    char id[20];
    char id_alumno[20];
    char id_materia[20];
    float calificacion;
    char tipo[30];
    char descripcion[100];
    char fecha[11];
    char periodo[20];
};

struct Trabajo {
    char id[20];
    char id_materia[20];
    char titulo[100];
    char descripcion[300];
    char fecha_asignacion[11];
    char fecha_entrega[11];
    float puntuacion_maxima;
    int activo;
};

struct Entrega_Trabajo {
    char id[20];
    char id_trabajo[20];
    char id_alumno[20];
    char fecha_entrega[11];
    float calificacion;
    char comentarios[300];
    int entregado;
};

struct Asistencia {
    char id[20];
    char id_alumno[20];
    char id_materia[20];
    char fecha[11];
    int presente;
    char observaciones[200];
};

struct Aula {
    char id[20];
    char nombre[50];
    int capacidad;
    char tipo[30];
    int activo;
};

struct Horario {
    char id[20];
    char id_materia[20];
    char id_aula[20];
    char dia_semana[15];
    char hora_inicio[6];
    char hora_fin[6];
    char periodo[20];
};

struct Periodo {
    char id[20];
    char nombre[50];
    char fecha_inicio[11];
    char fecha_fin[11];
    int activo;
};

// Descomentar para activar mensajes de depuracion
// #define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif

#endif
