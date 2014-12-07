#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "o_objet.h"
#include "t_geometrie.h"

Grille_flottant* matrix_create(int row_count, int col_count);
void matrix_delete(Grille_flottant* m);

void matrix_print(Grille_flottant* m);

Grille_flottant* matrix_product(Grille_flottant* m1, Grille_flottant* m2);
void matrix_vector_product(Grille_flottant* m, Table_flottant* v, Table_flottant* r);

Grille_flottant* matrix_transpose(Grille_flottant* m);

#endif
