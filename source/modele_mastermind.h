/**
 * \file modele_mastermind.h
 * \brief Modèle (pattern MVC) pour la gestion du jeu Mastermind
 * \author Naa Marco s175610 -- Hoorelbeke Jordi s175615
 * \version 1.0
 * \date 06/05/2019
 *
 * Gestion du jeu Mastermind via une approche MVC. Ce module gère l'aspect modèle.
 */

#ifndef _MODELE_MASTERMIND_
#define _MODELE_MASTERMIND_

#include <gtk/gtk.h>

/**
 * \brief Nombre de pions
 */
#define NB_PIONS 4

/**
 * \brief Nombre de manches
 */
#define NB_MANCHES 10

/**
 * \brief Nombre de couleurs
 */
#define NB_COULEURS 7

/**
 * \enum CouleurProposition
 * \brief Enumération fournissant les couleurs pour les propositions.
 */
typedef enum
{
    videP = 0,
    vert = 1,
    jaune = 2,
    rouge = 3,
    bleu = 4,
    cyan = 5,
    mauve = 6,
    orange = 7
} CouleurProposition;

/**
 * \enum CouleurScore
 * \brief Enumération fournissant les couleurs pour les scores.
 */
typedef enum
{
    videS = 0,
    blanc = 1,
    noir = 2
} CouleurScore;

/**
 * \struct combinaison_t Combinaison
 * \brief Représentation d'une combinaison de couleurs et de son score.
 */
typedef struct combinaison_t
{
    CouleurProposition proposition[NB_PIONS];
    CouleurScore score[NB_PIONS];
    unsigned short victoire;
} Combinaison;

/**
 * \struct ordi_t Ordi
 * \brief Structure contenant les outils suplémentaires nécessaires pour l'algorithme de l'ordinateur.
 */
typedef struct ordi_t
{
    CouleurProposition couleur;
    CouleurProposition tabTmp[NB_PIONS];
    unsigned short pionNoirs;
    unsigned short pionBlancs;
} Ordi;

/**
 * \struct historique_t Historique
 * \brief Structure contenant l'historique de toutes les combinaisons.
 */
typedef struct historique_t
{
    CouleurProposition reponse[NB_PIONS];
    Combinaison *manche[10];
} Historique;

/**
 * \struct records_t Records
 * \brief Structure contenant les outils suplémentaires nécessaires pour les records.
 */
typedef struct records_t
{
    unsigned int tab_records[10];
    unsigned int max;
    unsigned int scoreCourant;
    unsigned long t1;
} Records;

/**
 * \struct modele_t modele
 * \brief Implémentation du modèle pour le jeu Mastermind.
 */
typedef struct modele_t
{
    Ordi *ordi;
    Historique *historique;
    Records *rec;
    unsigned short manche;
    CouleurProposition couleurCourante;
    unsigned short modeFacile;
    unsigned short modeOrdinateur;
    char **tab_imageP;
} ModeleMastermind;

/**
 * \fn Ordi *creer_ordi(void)
 * \brief Crée une structure de type Ordi.
 *
 * \return ordi
 */
Ordi *creer_ordi(void);

/**
 * \fn Combinaison *creer_combinaison(void)
 * \brief Crée une structure de type Combinaison.
 *
 * \return combinaison
 */
Combinaison *creer_combinaison(void);

/**
 * \fn Historique *creer_historique(Combinaison *combinaison[])
 * \brief Crée une structure de type Historique.
 *
 * \param combinaison un tableau de type Combinaison.
 *
 * \return historique
 */
Historique *creer_historique(Combinaison *combinaison[]);

/**
 * \fn ModeleMastermind *creer_modele_mastermind(Historique *historique, Records *rec, Ordi *ordi)
 * \brief Crée une structure de type ModeleMastermind.
 *
 * \param mm une structure de type Historique.
 * \param mm une structure de type Records.
 * \param mm une structure de type Ordi.
 *
 * \return mm
 */
ModeleMastermind *creer_modele_mastermind(Historique *historique, Records *rec, Ordi *ordi);
/**
 * \fn Records *creer_records(void)
 * \brief Crée une structure de type Records.
 *
 * \return rec
 */
Records *creer_records(void);

/**
 * \fn void remplissage_reponse_aleatoire(ModeleMastermind *mm)
 * \brief Génère aléatoirement une combinaison de NB_PIONS couleurs.
 *
 * \param mm une structure de type ModeleMastermind.
 */
void remplissage_reponse_aleatoire(ModeleMastermind *mm);

/**
 * \fn GtkWidget *charger_image(char *nom_image, unsigned int longeur, unsigned int largeur)
 * \brief Charge une image de dimension donnée.
 *
 * \param nom_image le chemin vers l'image à charger.
 * \param longueur la longueur de l'image à charger.
 * \param largeur la largeur de l'image à charger.
 *
 * \return image
 */
GtkWidget *charger_image(char *nom_image, unsigned int longeur, unsigned int largeur);

/**
 * \fn void manche_maj(GtkWidget *pLabel, unsigned short manche)
 * \brief Mise à jour de la manche.
 *
 * \param pLabel un label de type GtkWidget.
 * \param manche la manche courante.
 */
void manche_maj(GtkWidget *pLabel, unsigned short manche);

/**
 * \fn void calcul_record(unsigned short manche, ModeleMastermind * mm)
 * \brief Calcule le score de la manche courante.
 *
 * \param manche la manche courante.
 * \param mm une structure de type ModeleMastermind.
 */
void calcul_record(unsigned short manche, ModeleMastermind *mm);

/**
 * \fn void tri_records(Records *rec)
 * \brief Trie les records par ordre décroissant.
 *
 * \param rec une structure de type Records.
 */
void tri_records(Records *rec);

/**
 * \fn void decale(Combinaison *manche)
 * \brief Décale les scores.
 *
 * \param manche une structure de type Combinaison.
 */
void decale(Combinaison *manche);

/**
 * \fn compare_reponse_proposition(ModeleMastermind *mm, Combinaison *manche)
 * \brief Compare la proposition avec la réponse et remplit la combinaison du score.
 *
 * \param mm une structure de type ModeleMastermind.
 * \param manche une structure de type Combinaison.
 */
void compare_reponse_proposition(ModeleMastermind *mm, Combinaison *manche);

/**
 * \fn void algorithme_ordinateur(ModeleMastermind *mm, Combinaison *manche)
 * \brief Génère la combinaison de NB_PIONS couleurs de l'ordinateur.
 *
 * \param mm une structure de type ModeleMastermind.
 * \param manche une structure de type Combinaison.
 */
void algorithme_ordinateur(ModeleMastermind *mm, Combinaison *manche);

#endif //__MODELE_MASTERMIND__