#ifndef HORARIOS_H
#define HORARIOS_H
#include "estructuras.h"
void menuHorarios(Usuario usuario_actual);
void listarHorarios();
void agregarHorario(Usuario usuario_actual);
void editarHorario(Usuario usuario_actual);
void eliminarHorario(Usuario usuario_actual);
void verHorarioMateria(const char id_materia[]);
#endif
