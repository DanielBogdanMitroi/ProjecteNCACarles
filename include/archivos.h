#ifndef ARCHIVOS_H
#define ARCHIVOS_H
#include "estructuras.h"
void crearArchivosIniciales();
void diagnosticarCargaArchivo(const char archivo[]);
int contarLineas(const char archivo[]);
int existeIDEnArchivo(const char archivo[], const char id[]);

int cargarAlumnos(Alumno alumnos[], int *total);
void guardarAlumnos(Alumno alumnos[], int total);
int cargarProfesores(Profesor profesores[], int *total);
void guardarProfesores(Profesor profesores[], int total);
int cargarMaterias(Materia materias[], int *total);
void guardarMaterias(Materia materias[], int total);
int cargarNotas(Nota notas[], int *total);
void guardarNotas(Nota notas[], int total);
int cargarTrabajos(Trabajo trabajos[], int *total);
void guardarTrabajos(Trabajo trabajos[], int total);
int cargarEntregas(Entrega_Trabajo entregas[], int *total);
void guardarEntregas(Entrega_Trabajo entregas[], int total);
int cargarAsistencias(Asistencia asistencias[], int *total);
void guardarAsistencias(Asistencia asistencias[], int total);
int cargarHorarios(Horario horarios[], int *total);
void guardarHorarios(Horario horarios[], int total);
int cargarAulas(Aula aulas[], int *total);
void guardarAulas(Aula aulas[], int total);
int cargarPeriodos(Periodo periodos[], int *total);
void guardarPeriodos(Periodo periodos[], int total);
int cargarMatriculas(Matricula matriculas[], int *total);
void guardarMatriculas(Matricula matriculas[], int total);
int cargarProfesorMaterias(Profesor_Materia pm[], int *total);
void guardarProfesorMaterias(Profesor_Materia pm[], int total);
int cargarUsuarios(Usuario usuarios[], int *total);
void guardarUsuarios(Usuario usuarios[], int total);
#endif
