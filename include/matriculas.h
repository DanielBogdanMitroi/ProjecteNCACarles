#ifndef MATRICULAS_H
#define MATRICULAS_H
#include "estructuras.h"
void menuMatriculas(Usuario usuario_actual);
void listarMatriculas();
void agregarMatricula(Usuario usuario_actual);
void cancelarMatricula(Usuario usuario_actual);
void verMatriculasAlumno(const char id_alumno[]);
void asignarProfesorMateria(Usuario usuario_actual);
#endif
