/**
 * \file vue_mastermind.h
 * \brief Vue (pattern MVC) pour la gestion du jeu Mastermind
 * \author Naa Marco s175610 -- Hoorelbeke Jordi s175615
 * \version 1.0
 * \date 06/05/2019
 *
 * Gestion du jeu Mastermind via une approche MVC. Ce module gère l'aspect vue.
 */

#ifndef __VUE_MASTERMIND__
#define __VUE_MASTERMIND__

#include <gtk/gtk.h>
#include "modele_mastermind.h"
#include "controleur_mastermind.h"

/**
 * \struct vue_t VueMastermind
 * \brief Implémentation de la vue pour le jeu Mastermind.
 */
typedef struct vue_t
{
    ModeleMastermind *mm;
    GtkWidget *pLabelManche;
    GtkWidget *pLabelMode;
    char imageP[50];
    char imageS[50];
} VueMastermind;

/**
 * \fn struct vue_t *creer_vue_mastermind(ModeleMastermind *mm)
 * \brief Crée une structure de type VueMastermind.
 *
 * \param mm une structure de type ModeleMastermind.
 *
 * \return vm
 */
struct vue_t *creer_vue_mastermind(ModeleMastermind *mm);

/**
 * \fn GtkWidget *creer_fenetre(void)
 * \brief Crée la fenêtre du jeu Mastermind.
 *
 * \return pF
 */
GtkWidget *creer_fenetre(void);

/**
 * \fn void fenetre_records(GtkWidget *pF, gpointer data)
 * \brief Crée la fenêtre des records.
 *
 * \param pF le widget en cause.
 * \param data pointeur vers rec.
 */
void fenetre_records(GtkWidget *pF, gpointer data);

/**
 * \fn void apropos(GtkWidget *pF, gpointer data)
 * \brief Crée la fenêtre apropos.
 *
 * \param pF le widget en cause.
 * \param data pointeur NULL.
 */
void apropos(GtkWidget *pF, gpointer data);

/**
 * \fn void regles(GtkWidget *pF, gpointer data)
 * \brief Crée la fenêtre des règles du jeu.
 *
 * \param pF le widget en cause.
 * \param data pointeur NULL.
 */
void regles(GtkWidget *pF, gpointer data);

#endif //__VUE_MASTERMIND__