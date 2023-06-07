/*
 * modele_mastermind.c
 *
 * Ce fichier contient les fonctions pour l'aspect modèle(MVC) du jeu Mastermind.
 *
 * @author: Naa Marco s175610 -- Hoorelbeke Jordi s175615
 * @date: 06/05/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <gtk/gtk.h>
#include "modele_mastermind.h"

Ordi *creer_ordi()
{
    Ordi *ordi = malloc(sizeof(Ordi));
    if (ordi == NULL)
        return NULL;

    srand(time(NULL));
    ordi->couleur = rand() % 7 + 1;
    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        ordi->tabTmp[i] = videP;
    }
    ordi->pionNoirs = 0;
    ordi->pionBlancs = 0;

    return ordi;
}

Combinaison *creer_combinaison()
{
    Combinaison *combinaison = malloc(sizeof(Combinaison));
    if (combinaison == NULL)
        return NULL;

    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        combinaison->proposition[i] = videP;
        combinaison->score[i] = videS;
    }
    combinaison->victoire = 0;

    return combinaison;
}

Historique *creer_historique(Combinaison *combinaison[])
{
    assert(combinaison != NULL);

    Historique *historique = malloc(sizeof(Historique));
    if (historique == NULL)
        return NULL;

    for (unsigned int i = 0; i < NB_PIONS; ++i)
        historique->reponse[i] = videP;
    for (unsigned int i = 0; i < NB_MANCHES; ++i)
        historique->manche[i] = combinaison[i];

    return historique;
}

Records *creer_records()
{
    Records *rec = malloc(sizeof(Records));
    if (rec == NULL)
        return NULL;

    for (unsigned int i = 0; i < 10; ++i)
        rec->tab_records[i] = 0;
    rec->t1 = 0;

    return rec;
}

ModeleMastermind *creer_modele_mastermind(Historique *historique, Records *rec, Ordi *ordi)
{
    assert(historique != NULL && rec != NULL);

    ModeleMastermind *mm = malloc(sizeof(ModeleMastermind));
    if (mm == NULL)
        return NULL;

    mm->ordi = ordi;
    mm->historique = historique;
    mm->rec = rec;
    mm->manche = 1;
    mm->couleurCourante = vert;
    mm->modeFacile = 0;
    mm->modeOrdinateur = 0;
    mm->tab_imageP = malloc(4 * sizeof(char *));
    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        mm->tab_imageP[i] = malloc(50 * sizeof(char));
        sprintf(mm->tab_imageP[i], "images/green.png");
    }

    return mm;
}

void remplissage_reponse_aleatoire(ModeleMastermind *mm)
{
    assert(mm != NULL);

    srand(time(NULL)); // rand dépend du temps pour plus de hasard
    for (unsigned int i = 0; i < NB_PIONS; ++i)
        mm->historique->reponse[i] = rand() % 7 + 1;
}

GtkWidget *charger_image(char *nom_image, unsigned int longeur, unsigned int largeur)
{
    assert(nom_image != NULL);

    GdkPixbuf *pb_temp, *pb;
    GtkWidget *image;

    pb_temp = gdk_pixbuf_new_from_file(nom_image, NULL);
    if (pb_temp == NULL)
    {
        printf("Erreur de chargement de l'image!\n");
        return NULL;
    }
    pb = gdk_pixbuf_scale_simple(pb_temp, 200, 200, GDK_INTERP_NEAREST);
    image = gtk_image_new_from_pixbuf(pb);

    gtk_widget_set_size_request(image, longeur, largeur);

    return image;
}

void manche_maj(GtkWidget *pLabel, unsigned short manche)
{
    assert(pLabel != NULL);

    char message[100];
    sprintf(message, "Manche = %hu", manche);
    gtk_label_set_text(GTK_LABEL(pLabel), message);
}

void calcul_record(unsigned short manche, ModeleMastermind *mm)
{
    assert(mm != NULL);

    unsigned long t2 = clock() / 100000;
    unsigned int capital = 1200;
    t2 = 3 * (t2 - mm->rec->t1);
    if (t2 > 1000)
        mm->rec->scoreCourant = 0;
    else
        mm->rec->scoreCourant = capital - 100 * manche - t2;
    if (mm->modeFacile == 1)
        mm->rec->scoreCourant /= 2;
}

void tri_records(Records *rec)
{
    assert(rec != NULL);
    unsigned int i = 1;
    if (rec->scoreCourant > rec->tab_records[0])
    {
        while (i < 10)
        {
            if (i == 9)
            {
                rec->tab_records[i - 1] = rec->tab_records[i];
                rec->max = rec->scoreCourant;
                rec->tab_records[i] = rec->scoreCourant;
                i++;
            }
            if (rec->scoreCourant > rec->tab_records[i])
            {
                rec->tab_records[i - 1] = rec->tab_records[i];
                i++;
            }
            if (rec->scoreCourant < rec->tab_records[i])
            {
                rec->tab_records[i - 1] = rec->scoreCourant;
                i += 10; // pour arreter manuellement la boucle
            }
        }
    }
}

void decale(Combinaison *manche)
{
    assert(manche != NULL);
    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        if (manche->score[i] == videS)
        {
            for (unsigned int j = i + 1; j < NB_PIONS; ++j)
            {
                if (manche->score[j] != videS)
                {
                    manche->score[i] = manche->score[j];
                    manche->score[j] = videS;
                    i++;
                }
            }
        }
    }
}

void compare_reponse_proposition(ModeleMastermind *mm, Combinaison *manche)
{
    assert(manche != NULL);

    unsigned int copieRep[NB_PIONS];
    unsigned int copieProp[NB_PIONS];
    for (unsigned int i = 0; i < NB_PIONS; ++i)
        copieRep[i] = mm->historique->reponse[i];

    for (unsigned int i = 0; i < NB_PIONS; ++i)
        copieProp[i] = manche->proposition[i];

    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        if (copieRep[i] == copieProp[i] && copieRep[i] != 0 && copieProp[i] != 0)
        {
            manche->score[i] = noir;
            copieRep[i] = 0;
            copieProp[i] = 0;
        }
    }

    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        for (unsigned int j = 0; j < NB_PIONS; ++j)
        {
            if (copieRep[i] == copieProp[j] && copieRep[i] != 0 && copieProp[j] != 0)
            {
                manche->score[i] = blanc;
                copieRep[i] = 0;
                copieProp[j] = 0;
                break;
            }
        }
    }
    if (mm->modeFacile == 0)
        decale(manche);
}

void algorithme_ordinateur(ModeleMastermind *mm, Combinaison *manche)
{
    assert(mm != NULL);

    mm->ordi->couleur++;
    if (mm->ordi->couleur > orange)
        mm->ordi->couleur = vert;
    unsigned short pion_blancs_prec = mm->ordi->pionBlancs;
    unsigned short pion_noirs_prec = mm->ordi->pionNoirs;
    mm->ordi->pionNoirs = 0;
    mm->ordi->pionBlancs = 0;
    unsigned int boucle = 1;
    unsigned int presence_case_vide = 0;

    if (mm->manche == 2)
    {
        for (unsigned int i = 0; i < NB_PIONS; ++i)
        {
            mm->historique->manche[0]->proposition[i] = mm->ordi->couleur;
            mm->ordi->tabTmp[i] = videP;
        }
    }

    if (mm->manche != 2)
    {
        // COMPTAGE PIONS NOIRS ET BLANCS
        for (unsigned int i = 0; i < NB_PIONS; ++i)
        {
            if (mm->historique->manche[mm->manche - 3]->score[i] == 2)
                mm->ordi->pionNoirs++;
            if (mm->historique->manche[mm->manche - 3]->score[i] == 1)
                mm->ordi->pionBlancs++;
        }

        // SI PIONS = 0
        if (mm->ordi->pionNoirs + mm->ordi->pionBlancs == 0)
            for (unsigned int i = 0; i < NB_PIONS; ++i)
                manche->proposition[i] = mm->ordi->couleur;

        // SI LE NOMBRE DE PIONS S'INCRÉMENTE
        if (mm->ordi->pionNoirs + mm->ordi->pionBlancs > pion_noirs_prec + pion_blancs_prec)
        {
            for (unsigned int i = 0; i < NB_PIONS; ++i)
                if (!mm->ordi->tabTmp[i])
                    presence_case_vide++;
            if (presence_case_vide != 0)
            {
                for (int i = 0; i < mm->ordi->pionBlancs - 1; ++i)
                {
                    boucle = 1;
                    for (unsigned int j = 0; boucle; ++j)
                    {
                        if (mm->ordi->tabTmp[j % NB_PIONS] != 0 && mm->ordi->tabTmp[(j + 1) % NB_PIONS] == 0)
                        {
                            mm->ordi->tabTmp[(j + 1) % NB_PIONS] = mm->ordi->tabTmp[j % NB_PIONS];
                            mm->ordi->tabTmp[j % NB_PIONS] = 0;
                            boucle = 0;
                        }
                    }
                }
            }
            unsigned short difference = (mm->ordi->pionNoirs + mm->ordi->pionBlancs) - (pion_noirs_prec + pion_blancs_prec);
            boucle = 1;
            for (unsigned int i = 0; i < NB_PIONS && boucle; ++i)
            {
                if (!mm->ordi->tabTmp[i])
                {
                    if (mm->ordi->couleur - 1 == videP)
                        mm->ordi->tabTmp[i] = mm->ordi->couleur + NB_COULEURS - 1;
                    else
                        mm->ordi->tabTmp[i] = mm->ordi->couleur - 1;
                    difference--;
                    if (difference <= 0)
                        boucle = 0;
                }
            }
        }

        // SI LE NOMBRE DE PIONS RESTE INCHANGÉ
        if (mm->ordi->pionNoirs + mm->ordi->pionBlancs == pion_blancs_prec + pion_noirs_prec)
        {
            unsigned int presence_case_vide = 0;
            unsigned int temp = 0;
            for (unsigned int i = 0; i < NB_PIONS; ++i)
                if (!mm->ordi->tabTmp[i])
                    presence_case_vide++;
            if (presence_case_vide != 0)
            {
                for (int i = 0; i < mm->ordi->pionBlancs; ++i)
                {
                    boucle = 1;
                    for (unsigned int j = 0; boucle; ++j)
                    {
                        if (mm->ordi->tabTmp[j % NB_PIONS] != 0)
                        {
                            temp = mm->ordi->tabTmp[j % NB_PIONS];
                            mm->ordi->tabTmp[j % NB_PIONS] = 0;
                            for (unsigned int k = (j + 1) % NB_PIONS; boucle; ++k)
                            {
                                if (mm->ordi->tabTmp[k % NB_PIONS] == 0)
                                {
                                    mm->ordi->tabTmp[k % NB_PIONS] = temp;
                                    boucle = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // CAS PARTICULIERS
    presence_case_vide = 0;
    for (unsigned int i = 0; i < NB_PIONS; ++i)
        if (!mm->ordi->tabTmp[i])
            presence_case_vide++;
    if (!presence_case_vide)
    {
        unsigned int gobelet = 0;
        if (mm->ordi->pionBlancs == 4)
        {
            gobelet = mm->ordi->tabTmp[1];
            mm->ordi->tabTmp[1] = mm->ordi->tabTmp[2];
            mm->ordi->tabTmp[2] = gobelet;

            gobelet = mm->ordi->tabTmp[0];
            mm->ordi->tabTmp[0] = mm->ordi->tabTmp[3];
            mm->ordi->tabTmp[3] = gobelet;
        }
        if (mm->ordi->pionBlancs == 3)
        {
            gobelet = mm->ordi->tabTmp[0];
            mm->ordi->tabTmp[0] = mm->ordi->tabTmp[3];
            mm->ordi->tabTmp[3] = gobelet;

            gobelet = mm->ordi->tabTmp[0];
            mm->ordi->tabTmp[0] = mm->ordi->tabTmp[1];
            mm->ordi->tabTmp[1] = gobelet;
        }
        if (mm->ordi->pionBlancs == 2)
        {
            unsigned short a, b;
            a = 0;
            b = 0;
            for (unsigned int i = 0; i < NB_PIONS; ++i)
            {
                if (mm->historique->manche[mm->manche - 3]->score[i] == 1)
                {
                    if (!a)
                        a = i;
                    else
                    {
                        if (!b)
                            b = i;
                    }
                }
            }
            gobelet = mm->ordi->tabTmp[a];
            mm->ordi->tabTmp[a] = mm->ordi->tabTmp[b];
            mm->ordi->tabTmp[b] = gobelet;
        }
    }

    // COMBINAISON DE L'ORDINATEUR
    if (mm->ordi->pionNoirs + mm->ordi->pionBlancs != 0)
    {
        for (unsigned int i = 0; i < NB_PIONS; ++i)
        {
            manche->proposition[i] = mm->ordi->couleur;
            if (mm->ordi->tabTmp[i] != 0)
                manche->proposition[i] = mm->ordi->tabTmp[i];
        }
    }

    compare_reponse_proposition(mm, manche);

    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        switch (manche->proposition[i])
        {
        case vert:
            mm->tab_imageP[i] = "images/green.png";
            break;
        case jaune:
            mm->tab_imageP[i] = "images/yellow.png";
            break;
        case rouge:
            mm->tab_imageP[i] = "images/red.png";
            break;
        case bleu:
            mm->tab_imageP[i] = "images/blue.png";
            break;
        case cyan:
            mm->tab_imageP[i] = "images/cyan.png";
            break;
        case mauve:
            mm->tab_imageP[i] = "images/purple.png";
            break;
        case orange:
            mm->tab_imageP[i] = "images/orange.png";
            break;
        default:
            mm->tab_imageP[i] = "images/green.png";
            break;
        }
    }
}