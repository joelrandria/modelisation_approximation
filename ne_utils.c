#include "ne_utils.h"

#include <math.h>

Table_flottant* malloc_table_flottant(int size)
{
  Table_flottant* t;

  ALLOUER(t, 1);
  t->nb = size;
  ALLOUER(t->table, t->nb);

  return t;
}
void print_table_flottant(Table_flottant* table)
{
  int i;

  for (i = 0; i < table->nb; ++i)
    printf("%f ", table->table[i]);

  printf("\r\n");
}
void free_table_flottant(Table_flottant* table)
{
  if (table->nb > 0)
    free(table->table);
  free(table);
}

void print_table_triplet(Table_triplet* table)
{
  int i;

  for (i = 0; i < table->nb; ++i)
    printf("(%f, %f, %f) ", table->table[i].x, table->table[i].y, table->table[i].z);

  printf("\r\n");
}

Flottant triplet_distance(Triplet* t1, Triplet* t2)
{
  return sqrt(pow(t2->x - t1->x, 2) + pow(t2->y - t1->y, 2) + pow(t2->z - t1->z, 2));
}
Flottant triplet_total_distance(Table_triplet* table)
{
  int i;
  Flottant total;

  total = 0;

  for (i = 0; i < (table->nb - 1); ++i)
    total += triplet_distance(&table->table[i], &table->table[i + 1]);

  return total;
}

void get_triplets_x_values(Table_triplet* table, Table_flottant* r)
{
  int i;

  // Tables compatibles ?
  if (table->nb != r->nb)
  {
    fprintf(stderr, "get_triplets_x_values(table, r): Tailles incompatibles: table(%d), r(%d)\r\n",
	    table->nb, r->nb);
    EXIT;
  }

  for (i = 0; i < table->nb; ++i)
    r->table[i] = table->table[i].x;
}
void get_triplets_y_values(Table_triplet* table, Table_flottant* r)
{
  int i;

  // Tables compatibles ?
  if (table->nb != r->nb)
  {
    fprintf(stderr, "get_triplets_y_values(table, r): Tailles incompatibles: table(%d), r(%d)\r\n",
	    table->nb, r->nb);
    EXIT;
  }

  for (i = 0; i < table->nb; ++i)
    r->table[i] = table->table[i].y;
}
void get_triplets_z_values(Table_triplet* table, Table_flottant* r)
{
  int i;

  // Tables compatibles ?
  if (table->nb != r->nb)
  {
    fprintf(stderr, "get_triplets_z_values(table, r): Tailles incompatibles: table(%d), r(%d)\r\n",
	    table->nb, r->nb);
    EXIT;
  }

  for (i = 0; i < table->nb; ++i)
    r->table[i] = table->table[i].z;
}

void set_triplets_x_values(Table_triplet* table, Table_flottant* values)
{
  int i;

  for (i = 0; i < values->nb; ++i)
    table->table[i].x = values->table[i];
}
void set_triplets_y_values(Table_triplet* table, Table_flottant* values)
{
  int i;

  for (i = 0; i < values->nb; ++i)
    table->table[i].y = values->table[i];
}
void set_triplets_z_values(Table_triplet* table, Table_flottant* values)
{
  int i;

  for (i = 0; i < values->nb; ++i)
    table->table[i].z = values->table[i];
}

Table_triplet* malloc_table_triplet(int size)
{
  Table_triplet* table;

  ALLOUER(table, 1);

  table->nb = size;
  ALLOUER(table->table, table->nb);

  return table;
}
void copy_table_triplet(Table_triplet* table_src, Table_triplet* table_dst)
{
  int i;

  table_dst->nb = table_src->nb;

  for (i = 0; i < table_src->nb; ++i)
    table_dst->table[i] = table_src->table[i];
}
void free_table_triplet(Table_triplet* table)
{
  if (table == 0)
    return;

  if (table->nb > 0)
    free(table->table);

  free(table);
}

void triplet_linear_interpolation(Triplet* point1, Triplet* point2,
				  Triplet* result, float position)
{
  float origin_offset;

  origin_offset = 1.0f - position;

  result->x = point1->x * origin_offset + point2->x * position;
  result->y = point1->y * origin_offset + point2->y * position;
  result->z = point1->z * origin_offset + point2->z * position;
}
