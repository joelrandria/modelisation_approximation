/**
 * Fournit des fonctions communes de manipulation des entités NanoEdit.
 */
#ifndef __NE_UTILS_H__
#define __NE_UTILS_H__

#include "o_objet.h"
#include "t_geometrie.h"

Table_flottant* malloc_table_flottant(int size);
void print_table_flottant(Table_flottant* table);
void free_table_flottant(Table_flottant* table);

Table_triplet* malloc_table_triplet(int size);
void copy_table_triplet(Table_triplet* table_src, Table_triplet* table_dst);
void free_table_triplet(Table_triplet* table);

void print_table_triplet(Table_triplet* table);

Flottant triplet_distance(Triplet* t1, Triplet* t2);
Flottant triplet_total_distance(Table_triplet* table);

void get_triplets_x_values(Table_triplet* table, Table_flottant* r);
void get_triplets_y_values(Table_triplet* table, Table_flottant* r);
void get_triplets_z_values(Table_triplet* table, Table_flottant* r);

void set_triplets_x_values(Table_triplet* table, Table_flottant* values);
void set_triplets_y_values(Table_triplet* table, Table_flottant* values);
void set_triplets_z_values(Table_triplet* table, Table_flottant* values);

void triplet_linear_interpolation(Triplet* point1, Triplet* point2,
				  Triplet* result, float position);

#endif
