#include "sys_lin.h"
#include "u_xt_interface.h"

#include <math.h>

#define ZERO 1E-08

Grille_flottant duplique_Grille_flottant(Grille_flottant *original)
{
   int i,j;  
   Grille_flottant copie;

   ALLOUER(copie.grille,original->nb_lignes);
   copie.nb_lignes = original->nb_lignes;
   copie.nb_colonnes = original->nb_colonnes;
   
   for (i=0 ; i<original->nb_lignes ; i++)
     {
      ALLOUER(copie.grille[i],original->nb_colonnes);
      for (j=0 ; j<original->nb_colonnes ; j++)
            copie.grille[i][j] = original->grille[i][j];
     }

   return copie;
}

/*-------------------------------------------------------------------------*/

Table_flottant duplique_Table_flottant(Table_flottant *original)
{
   int i;  
   Table_flottant copie;

   ALLOUER(copie.table,original->nb);
   copie.nb = original->nb ;
   
   for (i=0 ; i<original->nb ; i++)     
            copie.table[i] = original->table[i];

   return copie;
}

/*-------------------------------------------------------------------------*/
void traite_pivot_max(Grille_flottant  *A,Table_flottant  *f, int colonne)
{
  int i,indice;
  double pivot,tps;
  Flottant *tmp;

  indice = colonne;
  pivot = fabs(A->grille[colonne][colonne]);

  //recherche pivot max

  for (i=colonne+1 ; i<A->nb_lignes ; i++)
    if (fabs(A->grille[i][colonne]) > pivot)
      {
	pivot = fabs(A->grille[i][colonne]);
	indice = i;
      }

  // traite pivot max

  if (indice != colonne)
    { 
      tmp = A->grille[colonne];
      A->grille[colonne] = A->grille[indice];
      A->grille[indice] = tmp;

      tps = f->table[colonne];
      f->table[colonne] = f->table[indice];
      f->table[indice] = tps;
    }
  
}

/*-------------------------------------------------------------------------*/
int resolution_systeme_lineaire(Grille_flottant  *A, 
                                Table_flottant  *f, 
                                Table_flottant  *x)
/*-------------------------------------------------------------------------*/
/* 
   Resolution du systeme lineaire   Ax=f par  methode de gauss
*/
/*-------------------------------------------------------------------------*/
{
 double  s,r;
 int i,j,k,n;
 Grille_flottant T;
 Table_flottant  b;
 

   n = A->nb_lignes; 
   if ( (A->nb_lignes != A->nb_colonnes) || (f->nb != A->nb_lignes) ) 
     return 1;

/* copie de la matrice et du second membre pour triangulation  */
   T = duplique_Grille_flottant(A);
   b = duplique_Table_flottant(f);

   /* allocation  solution */ 
   ALLOUER(x->table,n);
   x->nb = n; 

   for (k=0 ; k<n ; k++)
     {
       traite_pivot_max(&T,&b,k);
       if ( fabs(T.grille[k][k]) < ZERO)
         {
          free(x->table);
	  x->nb = 0;
          return(1);
	 }
       for (i=k+1 ; i<n ; i++)
       {   
          
         s = T.grille[i][k]/T.grille[k][k];	
         for (j=k+1 ; j<n ; j++)
             T.grille[i][j]=T.grille[i][j] - T.grille[k][j]*s;	
         b.table[i] -= s*b.table[k]; 
       }
     }

   if (fabs(T.grille[n-1][n -1]) < ZERO)  
   {
       free(x->table);
       x->nb = 0;
       return(1);
   }  

   x->table[n-1] = b.table[n-1]/T.grille[n-1][n-1];
   for (i=n-2 ; i>=0 ; i--)
   {
      r = 0.0;
      for (j=i+1 ; j<n ; j++)
         r += (T.grille[i][j])*(x->table[j]);
      if (fabs(T.grille[i][i]) < ZERO)   return(1);  
      x->table[i] =  (b.table[i] - r)/T.grille[i][i]; 
   }



   free(b.table);
   for (i=0 ; i<T.nb_lignes ; i++)
     free(T.grille[i]);
   free(T.grille);
   
   return(0); 
} 
/*-------------------------------------------------------------------------*/



