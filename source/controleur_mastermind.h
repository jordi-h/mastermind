/**
 * \file controleur_mastermind.h
 * \brief Contrôleur (pattern MVC) pour la gestion du jeu Mastermind
 * \author Naa Marco s175610 -- Hoorelbeke Jordi s175615
 * \version 1.0
 * \date 06/05/2019
 *
 * Gestion du jeu Mastermind via une approche MVC. Ce module gère l'aspect contrôleur.
 */

#ifndef __CONTROLEUR_MASTERMIND__
#define __CONTROLEUR_MASTERMIND__

#include <gtk/gtk.h>
#include "modele_mastermind.h"
#include "vue_mastermind.h"

/**
 * \struct controleur_t Controleur
 * \brief Implémentation du contrôleur pour le jeu Mastermind.
 */
typedef struct controleur_t
{
    ModeleMastermind *mm;
    struct vue_t *vm;
    unsigned short nb_boutons_clicked;
    GtkWidget *pBoutonProposition[NB_MANCHES][NB_PIONS];
    GtkWidget *pBoutonScore[NB_MANCHES][NB_PIONS];
    GtkWidget *pBoutonCouleur[NB_COULEURS];
    GtkWidget *pBoutonOrdi[NB_PIONS];
    GtkWidget *pBoutonOK;
} ControleurMastermind;

/**
 * \fn ControleurMastermind *creer_controleur_mastermind(ModeleMastermind *mm, struct vue_t *vm)
 * \brief Crée une structure de type ControleurMastermind.
 *
 * \param mm une structure de type ModeleMastermind.
 * \param vm une structure de type VueMastermind.
 *
 * \return cm
 */
ControleurMastermind *creer_controleur_mastermind(ModeleMastermind *mm, struct vue_t *vm);

/**
 * \fn void nouveau(GtkWidget *pF, gpointer data)
 * \brief Réinitialise la partie.
 *
 * \param pF le widget en cause.
 * \param data pointeur vers cm.
 */
void nouveau(GtkWidget *pF, gpointer data);

/**
 * \fn void click_bouton_proposition(GtkWidget *pBouton, gpointer data)
 * \brief Signal pour le clic d'un bouton proposition.
 *
 * \param pF le widget en cause.
 * \param data pointeur vers cm.
 */
void click_bouton_proposition(GtkWidget *pBouton, gpointer data);

/**
 * \fn void click_bouton_ordi(GtkWidget *pBouton, gpointer data)
 * \brief Signal pour le clic du bouton Mode Ordinateur.
 *
 * \param pF le widget en cause.
 * \param data pointeur vers cm.
 */
void click_bouton_ordi(GtkWidget *pBouton, gpointer data);

/**
 * \fn void click_bouton_couleur(GtkWidget *pBouton, gpointer data)
 * \brief Signal pour le clic d'un bouton couleur.
 *
 * \param pF le widget en cause.
 * \param data pointeur vers cm.
 */
void click_bouton_couleur(GtkWidget *pBouton, gpointer data);

/**
 * \fn void click_bouton_ok(GtkWidget *pBouton, gpointer data)
 * \brief Signal pour le clic sur le bouton OK.
 *
 * \param pF le widget en cause.
 * \param data pointeur vers cm.
 */
void click_bouton_ok(GtkWidget *pBouton, gpointer data);

/**
 * \fn void victoire(ControleurMastermind *cm, Combinaison *manche)
 * \brief Détecte la victoire de la partie.
 *
 * \param cm une structure de type ControleurMastermind.
 * \param manche une structure Combinaison.
 */
void victoire(ControleurMastermind *cm, Combinaison *manche);

/**
 * \fn void mode_humain_facile(GtkWidget *pF, gpointer data)
 * \brief Signal pour le clic sur le bouton Mode Humain Facile.
 *
 * \param pF le widget en cause.
 * \param data pointeur vers cm.
 */
void mode_humain_facile(GtkWidget *pF, gpointer data);

/**
 * \fn void mode_humain_normal(GtkWidget *pF, gpointer data)
 * \brief Signal pour le clic sur le bouton Mode Humain Normal.
 *
 * \param pF le widget en cause.
 * \param data pointeur vers cm.
 */
void mode_humain_normal(GtkWidget *pF, gpointer data);

/**
 * \fn void mode_ordinateur(GtkWidget *pF, gpointer data)
 * \brief Signal pour le clic sur le bouton Mode Ordinateur
 *
 * \param pF le widget en cause.
 * \param data pointeur vers cm.
 */
void mode_ordinateur(GtkWidget *pF, gpointer data);

/**
 * \fn void score(ControleurMastermind *cm, Combinaison *manche);
 * \brief Remplit le s.
 *
 * \param cm une structure de type ControleurMastermind.
 * \param manche une structure Combinaison.
 */
void score(ControleurMastermind *cm, Combinaison *manche);

#endif //__CONTROLEUR_MASTERMIND__