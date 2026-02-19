#ifndef NOTAS_H
#define NOTAS_H
#include "estructuras.h"
void menuNotas(Usuario usuario_actual);
void listarNotas();
void agregarNota(Usuario usuario_actual);
void editarNota(Usuario usuario_actual);
void eliminarNota(Usuario usuario_actual);
void verNotasAlumno(const char id_alumno[]);
void verNotasPorMateria(const char id_materia[]);
#endif
