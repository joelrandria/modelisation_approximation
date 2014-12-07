#include "casteljau.h"
#include "ne_utils.h"

Triplet casteljau(Table_triplet* polycontrol, float position)
{
  int i;
  int j;
  Triplet curve_point;
  Table_triplet* temp_points;
  Table_triplet* current_points;

  // Allocation des tables de travail
  temp_points = malloc_table_triplet(polycontrol->nb);
  current_points = malloc_table_triplet(polycontrol->nb);

  // Initialisation de la 1ère colonne: Points du polygone de contrôle
  copy_table_triplet(polycontrol, current_points);

  // Itération jusqu'à obtention du point final
  for (i = 1; i < polycontrol->nb; ++i)
  {
    for (j = 0; j < polycontrol->nb - i; ++j)
    {
      triplet_linear_interpolation(&current_points->table[j],
				   &current_points->table[j + 1],
				   &temp_points->table[j], position);
    }

    copy_table_triplet(temp_points, current_points);
  }

  // Sauvegarde du résultat
  curve_point = current_points->table[0];

  // Libération des tables de travail
  free_table_triplet(temp_points);
  free_table_triplet(current_points);

  return curve_point;
}
