#include "o_objet.h"
#include "t_geometrie.h"

#include "approximation_curve.h"
#include "approximation_curve_rendering.h"

#include "matrix.h"
#include "sys_lin.h"
#include "bernstein.h"
#include "casteljau.h"
#include "ne_utils.h"

#include <stdio.h>

static Table_flottant* ac_uniform_parameterization(Table_triplet* points)
{
  int i;
  Flottant step;
  Table_flottant* params;

  step = 1.f / (points->nb - 1);
  params = malloc_table_flottant(points->nb);

  for (i = 0; i < points->nb; ++i)
    params->table[i] = i * step;

  return params;
}
static Table_flottant* ac_non_uniform_parameterization(Table_triplet* points)
{
  int i;
  Table_flottant* params;

  Flottant total_distance;
  Flottant current_distance;

  params = malloc_table_flottant(points->nb);
  if (points->nb > 0)
    params->table[0] = 0.f;

  current_distance = 0;
  total_distance = triplet_total_distance(points);

  for (i = 0; i < (points->nb - 1); ++i)
  {
    current_distance += triplet_distance(&points->table[i], &points->table[i + 1]);

    params->table[i + 1] = current_distance / total_distance;
  }

  return params;
}

static void ac_bernstein_matrix(Grille_flottant* m, Table_flottant* params)
{
  int row;
  int col;
  int degree;

  if (m->nb_colonnes != params->nb)
  {
    fprintf(stderr, "ac_bernstein_matrix(): la largeur de la matrice spécifié (%d) ne correspond pas au nombre de paramètres spécifiés (%d)\r\n",
	    m->nb_colonnes, params->nb);
    EXIT;
  }

  degree = m->nb_lignes - 1;

  for (row = 0; row < m->nb_lignes; ++row)
    for (col = 0; col < m->nb_colonnes; ++col)
      m->grille[row][col] = bernstein_factor(degree, row, params->table[col]);
}

static void ac_update_curve_polygon(struct approximation_curve* ac)
{
  Grille_flottant* m;
  Grille_flottant* mt;
  Grille_flottant* mmt;

  Table_flottant* p;
  Table_flottant* a;
  Table_flottant* mp;

  Table_flottant* params;

  // Allocation de la table des points de contrôle de la courbe d'approximation
  if (ac->curve_polygon.nb != (ac->degree + 1))
  {
    free(ac->curve_polygon.table);

    ac->curve_polygon.nb = ac->degree + 1;
    ALLOUER(ac->curve_polygon.table, ac->curve_polygon.nb);
  }

  // Paramétrisation de la courbe d'approximation
  if (ac->use_uniform_parameterization)
    params = ac_uniform_parameterization(&ac->points);
  else
    params = ac_non_uniform_parameterization(&ac->points);

  // Construction de la matrice de Bernstein, de sa transposée et de leur produit
  m = matrix_create(ac->degree + 1, ac->points.nb);
  ac_bernstein_matrix(m, params);

  mt = matrix_transpose(m);
  mmt = matrix_product(m, mt);

  // Résolution des 3 systèmes d'équations (composantes x, y et z) des points de contrôle recherchés
  p = malloc_table_flottant(ac->points.nb);
  a = malloc_table_flottant(ac->degree + 1);
  mp = malloc_table_flottant(m->nb_lignes);

  // X
  get_triplets_x_values(&ac->points, p);
  matrix_vector_product(m, p, mp);
  if (resolution_systeme_lineaire(mmt, mp, a))
    EXIT;
  set_triplets_x_values(&ac->curve_polygon, a);

  // Y
  get_triplets_y_values(&ac->points, p);
  matrix_vector_product(m, p, mp);
  if (resolution_systeme_lineaire(mmt, mp, a))
    EXIT;
  set_triplets_y_values(&ac->curve_polygon, a);

  // Z
  get_triplets_z_values(&ac->points, p);
  matrix_vector_product(m, p, mp);
  if (resolution_systeme_lineaire(mmt, mp, a))
    EXIT;
  set_triplets_z_values(&ac->curve_polygon, a);

  // Libération des ressources
  matrix_delete(m);
  matrix_delete(mt);
  matrix_delete(mmt);

  free_table_flottant(a);
  free_table_flottant(p);
  free_table_flottant(mp);
  free_table_flottant(params);
}

static void ac_update_curve_points(struct approximation_curve* ac)
{
  int i;
  float step;

  // Réallocation de la table de points si nécessaire
  if (ac->curve_points.nb != ac->curve_point_count)
  {
    free(ac->curve_points.table);

    ac->curve_points.nb = ac->curve_point_count;
    ALLOUER(ac->curve_points.table, ac->curve_points.nb);
  }

  step = 1.f / (ac->curve_point_count - 1);

  for (i = 0; i < ac->curve_point_count; ++i)
    ac->curve_points.table[i] = casteljau(&ac->curve_polygon, i * step);
}

static void ac_update_curve(struct approximation_curve* ac)
{
  ac_update_curve_polygon(ac);
  ac_update_curve_points(ac);
}

static void update(struct approximation_curve* ac)
{
  if (!(UN_CHAMP_CHANGE(ac) || CREATION(ac)))
    return;

  if (CHAMP_CHANGE(ac, points)
      || CHAMP_CHANGE(ac, degree)
      || CHAMP_CHANGE(ac, curve_point_count)
      || CHAMP_CHANGE(ac, use_uniform_parameterization))
  {
    // Intégrité du degré d'approximation spécifié
    if (ac->degree < 0)
      ac->degree = 0;
    else if (ac->degree >= ac->points.nb)
      ac->degree = ac->points.nb - 1;

    // Intégrité du nombre de points de la courbe à afficher
    if (ac->curve_point_count < 0)
      ac->curve_point_count = 0;

    // MàJ globale de la courbe d'approximation
    ac_update_curve(ac);
  }
}

CLASSE(approximation_curve, struct approximation_curve,

       CHAMP(points,
	     LABEL("Points à approximer")
	     L_table_point P_table_triplet
	     Extrait Obligatoire Affiche Edite Sauve)
       CHAMP(degree,
	     LABEL("Degré de la courbe d'approximation")
	     L_entier  Edite Sauve DEFAUT("10"))
       CHAMP(use_uniform_parameterization,
	     LABEL("Paramétrage uniforme")
	     L_booleen Edite DEFAUT("1"))
       CHAMP(curve_point_count,
	     LABEL("Nombre de points à afficher")
	     L_entier  Edite Sauve DEFAUT("10"))

       CHANGEMENT(update)

       CHAMP_VIRTUEL(L_affiche_gl(approximation_curve_rendering))

       MENU("Jo/Approximation Bézier")

       EVENEMENT("Ctrl+RB")
       )
