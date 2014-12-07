#ifndef __SYS_LIN_H__
#define __SYS_LIN_H__

#include "o_objet.h"
#include "t_geometrie.h"

Grille_flottant duplique_Grille_flottant(Grille_flottant* original);
Table_flottant duplique_Table_flottant(Table_flottant* original);

void traite_pivot_max(Grille_flottant* A, Table_flottant* f, int colonne);

int resolution_systeme_lineaire(Grille_flottant* A,
                                Table_flottant* f,
                                Table_flottant* x);

#endif
