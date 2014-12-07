#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

Grille_flottant* matrix_create(int row_count, int col_count)
{
  int i;
  Grille_flottant* m;

  ALLOUER(m, 1);
  m->nb_lignes = row_count;
  m->nb_colonnes = col_count;

  if ((m->nb_lignes > 0) && (m->nb_colonnes > 0))
  {
    ALLOUER(m->grille, m->nb_lignes);

    for (i = 0; i < m->nb_lignes; ++i)
      ALLOUER(m->grille[i], m->nb_colonnes);
  }

  return m;
}
void matrix_delete(Grille_flottant* m)
{
  int i;

  if (m->nb_lignes > 0)
  {
    for (i = 0; i < m->nb_lignes; ++i)
      free(m->grille[i]);

    free(m->grille);
  }

  free(m);
}

void matrix_print(Grille_flottant* m)
{
  int row;
  int col;

  for (row = 0; row < m->nb_lignes; ++row)
  {
    for (col = 0; col < m->nb_colonnes; ++col)
    {
      printf("%f ", m->grille[row][col]);
    }

    printf("\r\n");
  }
}

Grille_flottant* matrix_product(Grille_flottant* m1, Grille_flottant* m2)
{
  int i;
  int m1_row;
  int m2_col;
  Grille_flottant* r;

  // Matrices à multiplier valides ?
  if (m1->nb_lignes <= 0 || m1->nb_colonnes <= 0)
  {
    fprintf(stderr, "matrix_product(m1, m2): m1 invalide: m1(%dx%d)\r\n",
	    m1->nb_lignes, m1->nb_colonnes);
    EXIT;
  }
  if (m2->nb_lignes <= 0 || m2->nb_colonnes <= 0)
  {
    fprintf(stderr, "matrix_product(m1, m2): m2 invalide: m2(%dx%d)\r\n",
	    m2->nb_lignes, m2->nb_colonnes);
    EXIT;
  }

  // Matrices compatibles pour produit ?
  if (m1->nb_colonnes != m2->nb_lignes)
  {
    fprintf(stderr, "matrix_product(m1, m2): matrices incompatibles: m1(%dx%d), m2(%dx%d)\r\n",
	    m1->nb_lignes, m1->nb_colonnes, m2->nb_lignes, m2->nb_colonnes);
    EXIT;
  }

  // Allocation de la matrice de résultat
  r = matrix_create(m1->nb_lignes, m2->nb_colonnes);

  // Produit
  for (m1_row = 0; m1_row < m1->nb_lignes; ++m1_row)
  {
    for (m2_col = 0; m2_col < m2->nb_colonnes; ++m2_col)
    {
      r->grille[m1_row][m2_col] = 0;

      for (i = 0; i < m1->nb_colonnes; ++i)
	r->grille[m1_row][m2_col] += m1->grille[m1_row][i] * m2->grille[i][m2_col];
    }
  }

  return r;
}
void matrix_vector_product(Grille_flottant* m, Table_flottant* v, Table_flottant* r)
{
  int i;
  int row;

  // Matrice valide ?
  if (m->nb_lignes <= 0 || m->nb_colonnes <= 0)
  {
    fprintf(stderr, "matrix_vector_product(m, v, r): m invalide: m(%dx%d)\r\n",
	    m->nb_lignes, m->nb_colonnes);
    EXIT;
  }
  // Vecteurs valides ?
  if (v->nb <= 0)
  {
    fprintf(stderr, "matrix_vector_product(m, v, r): v invalide: taille=%d\r\n",
	    v->nb);
    EXIT;
  }

  // Produit possible ?
  if (m->nb_colonnes != v->nb)
  {
    fprintf(stderr, "matrix_vector_product(m, v, r): tailles incompatibles: m(%dx%d), v(%d)\r\n",
	    m->nb_lignes, m->nb_colonnes, v->nb);
    EXIT;
  }

  // Vecteur résultat valide ?
  if (r->nb != m->nb_lignes)
  {
    fprintf(stderr, "matrix_vector_product(m, v, r): r invalide: m(%dx%d), v(%d), r(%d)\r\n",
	    m->nb_lignes, m->nb_colonnes, v->nb, r->nb);
    EXIT;
  }

  // Produit
  for (row = 0; row < m->nb_lignes; ++row)
  {
    r->table[row] = 0;
    
    for (i = 0; i < v->nb; ++i)
      r->table[row] += m->grille[row][i] * v->table[i];
  }
}

Grille_flottant* matrix_transpose(Grille_flottant* m)
{
  int row;
  int col;
  Grille_flottant* r;

  // Matrice à transposer valide ?
  if (m->nb_lignes <= 0 || m->nb_colonnes <= 0)
  {
    fprintf(stderr, "matrix_transpose(m): m invalide: m(%dx%d)\r\n",
	    m->nb_lignes, m->nb_colonnes);
    EXIT;
  }

  // Allocation de la matrice transposée
  r = matrix_create(m->nb_colonnes, m->nb_lignes);

  // Transposition
  for (row = 0; row < m->nb_lignes; ++row)
    for (col = 0; col < m->nb_colonnes; ++col)
      r->grille[col][row] = m->grille[row][col];

  return r;
}
