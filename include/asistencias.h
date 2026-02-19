#ifndef ASISTENCIAS_H
#define ASISTENCIAS_H
#include "estructuras.h"
void menuAsistencias(Usuario usuario_actual);
void registrarAsistencia(Usuario usuario_actual);
void verAsistenciasAlumno(const char id_alumno[]);
void verAsistenciasPorMateria(const char id_materia[]);
void reporteAsistencias();
#endif
