#ifndef __APPROXIMATION_CURVE_H__
#define __APPROXIMATION_CURVE_H__

#include "o_objet.h"
#include "t_geometrie.h"

struct approximation_curve
{
  // Ensemble de points à approximer
  Table_triplet points;
  int degree;

  // Courbe de Bézier d'approximation
  Table_triplet curve_polygon;		// Polygone de contrôle de la courbe d'approximation
  Table_triplet curve_points;		// Points de la courbe à afficher

  // Options
  int curve_point_count;		// Nombre de points de la courbe à afficher
  Booleen use_uniform_parameterization;	// Utilisation d'un paramétrage uniforme (par défaut), sinon non-uniforme

};

#endif
