#include "approximation_curve_rendering.h"

#include <GL/gl.h>

void approximation_curve_render_polyline(Table_triplet* polyline)
{
  int i;

  glBegin(GL_LINE_STRIP);
  for (i = 0; i < polyline->nb; ++i)
  {
    glVertex3f(polyline->table[i].x,
	       polyline->table[i].y,
	       polyline->table[i].z);
  }
  glEnd();
}
void approximation_curve_render_points(Table_triplet* points)
{
  int i;

  glBegin(GL_LINE_STRIP);
  for (i = 0; i < points->nb; ++i)
  {
    glVertex3f(points->table[i].x,
  	       points->table[i].y,
  	       points->table[i].z);
  }
  glEnd();
}

void approximation_curve_rendering(struct approximation_curve* curve)
{
  glColor3f(1, 1, 1);
  approximation_curve_render_points(&curve->curve_points);
}
