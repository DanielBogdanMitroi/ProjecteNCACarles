#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archivos.h"
#include "estructuras.h"

/* Cuenta las lineas de un archivo */
int contarLineas(const char archivo[]) {
    FILE *f = fopen(archivo, "r");
    int count = 0;
    char linea[512];
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f)) {
        if (strlen(linea) > 1) count++;
    }
    fclose(f);
    return count;
}

/* Verifica si un ID existe en el archivo */
int existeIDEnArchivo(const char archivo[], const char id[]) {
    FILE *f = fopen(archivo, "r");
    char linea[512];
    char id_linea[20];
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f)) {
        sscanf(linea, "%19[^|]", id_linea);
        if (strcmp(id_linea, id) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

/* Crea los archivos de datos iniciales si no existen */
void crearArchivosIniciales() {
    FILE *f;
    /* Crear directorio datos si no existe */
    system("mkdir -p datos");

    /* Crear archivo de usuarios si no existe */
    f = fopen(ARCHIVO_USUARIOS, "r");
    if (f == NULL) {
        f = fopen(ARCHIVO_USUARIOS, "w");
        if (f) {
            fprintf(f, "admin|admin123|1|ADMIN\n");
            fclose(f);
        }
    } else {
        fclose(f);
    }

    /* Crear archivos vacios para las demas entidades si no existen */
    const char *archivos[] = {
        ARCHIVO_ALUMNOS, ARCHIVO_PROFESORES, ARCHIVO_MATERIAS,
        ARCHIVO_NOTAS, ARCHIVO_TRABAJOS, ARCHIVO_ENTREGAS,
        ARCHIVO_ASISTENCIAS, ARCHIVO_HORARIOS, ARCHIVO_AULAS,
        ARCHIVO_PERIODOS, ARCHIVO_MATRICULAS, ARCHIVO_PROF_MATERIA
    };
    int i;
    for (i = 0; i < 12; i++) {
        f = fopen(archivos[i], "r");
        if (f == NULL) {
            f = fopen(archivos[i], "w");
            if (f) fclose(f);
        } else {
            fclose(f);
        }
    }
}

/* ============ ALUMNOS ============ */
static void parsearAlumno(const char linea[], Alumno *a) {
    sscanf(linea, "%19[^|]|%49[^|]|%49[^|]|%99[^|]|%10[^|]|%14[^|]|%199[^|]|%d",
           a->id, a->nombre, a->apellido, a->email,
           a->fecha_nacimiento, a->telefono, a->direccion, &a->activo);
}

int cargarAlumnos(Alumno alumnos[], int *total) {
    FILE *f = fopen(ARCHIVO_ALUMNOS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_ALUMNOS) {
        if (strlen(linea) > 1) {
            parsearAlumno(linea, &alumnos[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarAlumnos(Alumno alumnos[], int total) {
    FILE *f = fopen(ARCHIVO_ALUMNOS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%s|%s|%s|%d\n",
                alumnos[i].id, alumnos[i].nombre, alumnos[i].apellido, alumnos[i].email,
                alumnos[i].fecha_nacimiento, alumnos[i].telefono, alumnos[i].direccion, alumnos[i].activo);
    }
    fclose(f);
}

/* ============ PROFESORES ============ */
static void parsearProfesor(const char linea[], Profesor *p) {
    sscanf(linea, "%19[^|]|%49[^|]|%49[^|]|%99[^|]|%99[^|]|%14[^|]|%d",
           p->id, p->nombre, p->apellido, p->email,
           p->especialidad, p->telefono, &p->activo);
}

int cargarProfesores(Profesor profesores[], int *total) {
    FILE *f = fopen(ARCHIVO_PROFESORES, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_PROFESORES) {
        if (strlen(linea) > 1) {
            parsearProfesor(linea, &profesores[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarProfesores(Profesor profesores[], int total) {
    FILE *f = fopen(ARCHIVO_PROFESORES, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%s|%s|%d\n",
                profesores[i].id, profesores[i].nombre, profesores[i].apellido, profesores[i].email,
                profesores[i].especialidad, profesores[i].telefono, profesores[i].activo);
    }
    fclose(f);
}

/* ============ MATERIAS ============ */
static void parsearMateria(const char linea[], Materia *m) {
    sscanf(linea, "%19[^|]|%99[^|]|%199[^|]|%d|%d|%d",
           m->id, m->nombre, m->descripcion, &m->creditos, &m->horas_semanales, &m->activo);
}

int cargarMaterias(Materia materias[], int *total) {
    FILE *f = fopen(ARCHIVO_MATERIAS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_MATERIAS) {
        if (strlen(linea) > 1) {
            parsearMateria(linea, &materias[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarMaterias(Materia materias[], int total) {
    FILE *f = fopen(ARCHIVO_MATERIAS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%d|%d|%d\n",
                materias[i].id, materias[i].nombre, materias[i].descripcion,
                materias[i].creditos, materias[i].horas_semanales, materias[i].activo);
    }
    fclose(f);
}

/* ============ NOTAS ============ */
static void parsearNota(const char linea[], Nota *n) {
    sscanf(linea, "%19[^|]|%19[^|]|%19[^|]|%f|%29[^|]|%99[^|]|%10[^|]|%19[^\n]",
           n->id, n->id_alumno, n->id_materia, &n->calificacion,
           n->tipo, n->descripcion, n->fecha, n->periodo);
}

int cargarNotas(Nota notas[], int *total) {
    FILE *f = fopen(ARCHIVO_NOTAS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_NOTAS) {
        if (strlen(linea) > 1) {
            parsearNota(linea, &notas[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarNotas(Nota notas[], int total) {
    FILE *f = fopen(ARCHIVO_NOTAS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%.2f|%s|%s|%s|%s\n",
                notas[i].id, notas[i].id_alumno, notas[i].id_materia, notas[i].calificacion,
                notas[i].tipo, notas[i].descripcion, notas[i].fecha, notas[i].periodo);
    }
    fclose(f);
}

/* ============ TRABAJOS ============ */
static void parsearTrabajo(const char linea[], Trabajo *t) {
    sscanf(linea, "%19[^|]|%19[^|]|%99[^|]|%299[^|]|%10[^|]|%10[^|]|%f|%d",
           t->id, t->id_materia, t->titulo, t->descripcion,
           t->fecha_asignacion, t->fecha_entrega, &t->puntuacion_maxima, &t->activo);
}

int cargarTrabajos(Trabajo trabajos[], int *total) {
    FILE *f = fopen(ARCHIVO_TRABAJOS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_TRABAJOS) {
        if (strlen(linea) > 1) {
            parsearTrabajo(linea, &trabajos[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarTrabajos(Trabajo trabajos[], int total) {
    FILE *f = fopen(ARCHIVO_TRABAJOS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%s|%s|%.2f|%d\n",
                trabajos[i].id, trabajos[i].id_materia, trabajos[i].titulo, trabajos[i].descripcion,
                trabajos[i].fecha_asignacion, trabajos[i].fecha_entrega,
                trabajos[i].puntuacion_maxima, trabajos[i].activo);
    }
    fclose(f);
}

/* ============ ENTREGAS ============ */
static void parsearEntrega(const char linea[], Entrega_Trabajo *e) {
    sscanf(linea, "%19[^|]|%19[^|]|%19[^|]|%10[^|]|%f|%299[^|]|%d",
           e->id, e->id_trabajo, e->id_alumno, e->fecha_entrega,
           &e->calificacion, e->comentarios, &e->entregado);
}

int cargarEntregas(Entrega_Trabajo entregas[], int *total) {
    FILE *f = fopen(ARCHIVO_ENTREGAS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_ENTREGAS) {
        if (strlen(linea) > 1) {
            parsearEntrega(linea, &entregas[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarEntregas(Entrega_Trabajo entregas[], int total) {
    FILE *f = fopen(ARCHIVO_ENTREGAS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%.2f|%s|%d\n",
                entregas[i].id, entregas[i].id_trabajo, entregas[i].id_alumno,
                entregas[i].fecha_entrega, entregas[i].calificacion,
                entregas[i].comentarios, entregas[i].entregado);
    }
    fclose(f);
}

/* ============ ASISTENCIAS ============ */
static void parsearAsistencia(const char linea[], Asistencia *a) {
    a->observaciones[0] = '\0';
    sscanf(linea, "%19[^|]|%19[^|]|%19[^|]|%10[^|]|%d|%199[^\n]",
           a->id, a->id_alumno, a->id_materia, a->fecha,
           &a->presente, a->observaciones);
}

int cargarAsistencias(Asistencia asistencias[], int *total) {
    FILE *f = fopen(ARCHIVO_ASISTENCIAS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_ASISTENCIAS) {
        if (strlen(linea) > 1) {
            parsearAsistencia(linea, &asistencias[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarAsistencias(Asistencia asistencias[], int total) {
    FILE *f = fopen(ARCHIVO_ASISTENCIAS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%d|%s\n",
                asistencias[i].id, asistencias[i].id_alumno, asistencias[i].id_materia,
                asistencias[i].fecha, asistencias[i].presente, asistencias[i].observaciones);
    }
    fclose(f);
}

/* ============ HORARIOS ============ */
static void parsearHorario(const char linea[], Horario *h) {
    sscanf(linea, "%19[^|]|%19[^|]|%19[^|]|%14[^|]|%5[^|]|%5[^|]|%19[^\n]",
           h->id, h->id_materia, h->id_aula, h->dia_semana,
           h->hora_inicio, h->hora_fin, h->periodo);
}

int cargarHorarios(Horario horarios[], int *total) {
    FILE *f = fopen(ARCHIVO_HORARIOS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_HORARIOS) {
        if (strlen(linea) > 1) {
            parsearHorario(linea, &horarios[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarHorarios(Horario horarios[], int total) {
    FILE *f = fopen(ARCHIVO_HORARIOS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%s|%s|%s\n",
                horarios[i].id, horarios[i].id_materia, horarios[i].id_aula,
                horarios[i].dia_semana, horarios[i].hora_inicio, horarios[i].hora_fin, horarios[i].periodo);
    }
    fclose(f);
}

/* ============ AULAS ============ */
static void parsearAula(const char linea[], Aula *a) {
    sscanf(linea, "%19[^|]|%49[^|]|%d|%29[^|]|%d",
           a->id, a->nombre, &a->capacidad, a->tipo, &a->activo);
}

int cargarAulas(Aula aulas[], int *total) {
    FILE *f = fopen(ARCHIVO_AULAS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_AULAS) {
        if (strlen(linea) > 1) {
            parsearAula(linea, &aulas[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarAulas(Aula aulas[], int total) {
    FILE *f = fopen(ARCHIVO_AULAS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%d|%s|%d\n",
                aulas[i].id, aulas[i].nombre, aulas[i].capacidad,
                aulas[i].tipo, aulas[i].activo);
    }
    fclose(f);
}

/* ============ PERIODOS ============ */
static void parsearPeriodo(const char linea[], Periodo *p) {
    sscanf(linea, "%19[^|]|%49[^|]|%10[^|]|%10[^|]|%d",
           p->id, p->nombre, p->fecha_inicio, p->fecha_fin, &p->activo);
}

int cargarPeriodos(Periodo periodos[], int *total) {
    FILE *f = fopen(ARCHIVO_PERIODOS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_PERIODOS) {
        if (strlen(linea) > 1) {
            parsearPeriodo(linea, &periodos[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarPeriodos(Periodo periodos[], int total) {
    FILE *f = fopen(ARCHIVO_PERIODOS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%d\n",
                periodos[i].id, periodos[i].nombre, periodos[i].fecha_inicio,
                periodos[i].fecha_fin, periodos[i].activo);
    }
    fclose(f);
}

/* ============ MATRICULAS ============ */
static void parsearMatricula(const char linea[], Matricula *m) {
    sscanf(linea, "%19[^|]|%19[^|]|%19[^|]|%10[^|]|%19[^|]|%d",
           m->id, m->id_alumno, m->id_materia, m->fecha_matricula, m->periodo, &m->activo);
}

int cargarMatriculas(Matricula matriculas[], int *total) {
    FILE *f = fopen(ARCHIVO_MATRICULAS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_MATRICULAS) {
        if (strlen(linea) > 1) {
            parsearMatricula(linea, &matriculas[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarMatriculas(Matricula matriculas[], int total) {
    FILE *f = fopen(ARCHIVO_MATRICULAS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%s|%d\n",
                matriculas[i].id, matriculas[i].id_alumno, matriculas[i].id_materia,
                matriculas[i].fecha_matricula, matriculas[i].periodo, matriculas[i].activo);
    }
    fclose(f);
}

/* ============ PROFESOR_MATERIA ============ */
static void parsearProfesorMateria(const char linea[], Profesor_Materia *pm) {
    sscanf(linea, "%19[^|]|%19[^|]|%19[^|]|%19[^|]|%d",
           pm->id, pm->id_profesor, pm->id_materia, pm->periodo, &pm->activo);
}

int cargarProfesorMaterias(Profesor_Materia pm[], int *total) {
    FILE *f = fopen(ARCHIVO_PROF_MATERIA, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_PROFESOR_MATERIA) {
        if (strlen(linea) > 1) {
            parsearProfesorMateria(linea, &pm[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarProfesorMaterias(Profesor_Materia pm[], int total) {
    FILE *f = fopen(ARCHIVO_PROF_MATERIA, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%s|%s|%d\n",
                pm[i].id, pm[i].id_profesor, pm[i].id_materia,
                pm[i].periodo, pm[i].activo);
    }
    fclose(f);
}

/* ============ USUARIOS ============ */
static void parsearUsuario(const char linea[], Usuario *u) {
    sscanf(linea, "%49[^|]|%99[^|]|%d|%19[^\n]",
           u->username, u->password, &u->tipo, u->id_referencia);
}

int cargarUsuarios(Usuario usuarios[], int *total) {
    FILE *f = fopen(ARCHIVO_USUARIOS, "r");
    char linea[512];
    *total = 0;
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f) && *total < MAX_USUARIOS) {
        if (strlen(linea) > 1) {
            parsearUsuario(linea, &usuarios[*total]);
            (*total)++;
        }
    }
    fclose(f);
    return *total;
}

void guardarUsuarios(Usuario usuarios[], int total) {
    FILE *f = fopen(ARCHIVO_USUARIOS, "w");
    int i;
    if (f == NULL) return;
    for (i = 0; i < total; i++) {
        fprintf(f, "%s|%s|%d|%s\n",
                usuarios[i].username, usuarios[i].password,
                usuarios[i].tipo, usuarios[i].id_referencia);
    }
    fclose(f);
}
