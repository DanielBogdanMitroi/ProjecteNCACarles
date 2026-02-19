#ifndef TRABAJOS_H
#define TRABAJOS_H
#include "estructuras.h"
void menuTrabajos(Usuario usuario_actual);
void listarTrabajos();
void agregarTrabajo(Usuario usuario_actual);
void editarTrabajo(Usuario usuario_actual);
void eliminarTrabajo(Usuario usuario_actual);
void registrarEntrega(Usuario usuario_actual);
void verEntregasAlumno(const char id_alumno[]);
#endif
