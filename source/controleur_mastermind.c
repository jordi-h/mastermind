/*
 * controleur_mastermind.c
 *
 * Ce fichier contient les fonctions pour l'aspect contrôleur(MVC) du jeu Mastermind.
 *
 * @author: Naa Marco s175610 -- Hoorelbeke Jordi s175615
 * @date: 06/05/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <gtk/gtk.h>
#include "controleur_mastermind.h"

ControleurMastermind *creer_controleur_mastermind(ModeleMastermind *mm, struct vue_t *vm)
{
    ControleurMastermind *cm = malloc(sizeof(ControleurMastermind));
    if (cm == NULL)
        return NULL;

    cm->mm = mm;
    cm->vm = vm;
    cm->nb_boutons_clicked = 0;

    for (unsigned int i = 0; i < NB_MANCHES; ++i)
    {
        for (unsigned int j = 0; j < NB_PIONS; ++j)
        {
            cm->pBoutonProposition[i][j] = gtk_button_new();
            cm->pBoutonScore[i][j] = gtk_button_new();
            gtk_widget_set_size_request(cm->pBoutonProposition[i][j], 40, 40);
            gtk_widget_set_size_request(cm->pBoutonScore[i][j], 20, 20);
        }
    }

    for (unsigned int i = 0; i < NB_COULEURS; ++i)
    {
        cm->pBoutonCouleur[i] = gtk_button_new();
        gtk_widget_set_size_request(cm->pBoutonCouleur[i], 50, 50);
    }

    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        cm->pBoutonOrdi[i] = gtk_button_new();
        gtk_widget_set_size_request(cm->pBoutonOrdi[i], 40, 40);
    }

    cm->pBoutonOK = gtk_button_new_with_mnemonic("   _Ok   ");

    return cm;
}

void nouveau(GtkWidget *pF, gpointer data)
{
    assert(pF != NULL);

    ControleurMastermind *cm = (ControleurMastermind *)data;
    assert(cm != NULL);

    if (cm->mm->modeOrdinateur)
    { // MODE ORDINATEUR
        // création d'un modèle de mastermind
        cm->mm->ordi = creer_ordi();
        for (unsigned int i = 0; i < NB_MANCHES; ++i)
            cm->mm->historique->manche[i] = creer_combinaison();
        cm->mm->historique = creer_historique(cm->mm->historique->manche);
        cm->mm = creer_modele_mastermind(cm->mm->historique, cm->mm->rec, cm->mm->ordi);
        manche_maj(cm->vm->pLabelManche, cm->mm->manche - 1);
        cm->mm->rec->t1 = clock() / 100000;

        for (unsigned int i = 0; i < NB_MANCHES; i++)
        {
            for (unsigned int j = 0; j < NB_PIONS; j++)
            {
                gtk_widget_set_sensitive(cm->pBoutonProposition[i][j], FALSE);
                gtk_button_set_image(GTK_BUTTON(cm->pBoutonProposition[i][j]), charger_image(cm->vm->imageP, 0, 0));
                gtk_widget_set_sensitive(cm->pBoutonScore[i][j], FALSE);
                gtk_button_set_image(GTK_BUTTON(cm->pBoutonScore[i][j]), charger_image(cm->vm->imageS, 0, 0));
            }
        }
        for (unsigned int i = 0; i < NB_PIONS; ++i)
        {
            gtk_widget_set_sensitive(cm->pBoutonOrdi[i], TRUE);
            gtk_button_set_image(GTK_BUTTON(cm->pBoutonOrdi[i]), charger_image("images/green.png", 0, 0));
        }

        cm->mm->modeOrdinateur = 1;
    }
    else
    { // MODE ETRE-HUMAIN
        // création d'un modèle de mastermind
        cm->mm->ordi = creer_ordi();
        for (unsigned int i = 0; i < NB_MANCHES; ++i)
            cm->mm->historique->manche[i] = creer_combinaison();
        cm->mm->historique = creer_historique(cm->mm->historique->manche);
        cm->mm->rec->t1 = clock() / 100000;
        cm->mm = creer_modele_mastermind(cm->mm->historique, cm->mm->rec, cm->mm->ordi);
        remplissage_reponse_aleatoire(cm->mm);
        manche_maj(cm->vm->pLabelManche, cm->mm->manche);

        for (unsigned int i = 0; i < NB_MANCHES; i++)
        {
            for (unsigned int j = 0; j < NB_PIONS; j++)
            {
                gtk_widget_set_sensitive(cm->pBoutonProposition[i][j], FALSE);
                gtk_button_set_image(GTK_BUTTON(cm->pBoutonProposition[i][j]), charger_image(cm->vm->imageP, 0, 0));
                gtk_widget_set_sensitive(cm->pBoutonScore[i][j], FALSE);
                gtk_button_set_image(GTK_BUTTON(cm->pBoutonScore[i][j]), charger_image(cm->vm->imageS, 0, 0));
            }
        }

        for (unsigned int i = 0; i < NB_PIONS; ++i)
            gtk_widget_set_sensitive(cm->pBoutonProposition[0][i], TRUE);

        for (unsigned int i = 0; i < NB_PIONS; ++i)
        {
            gtk_widget_set_sensitive(cm->pBoutonOrdi[i], FALSE);
            gtk_button_set_image(GTK_BUTTON(cm->pBoutonOrdi[i]), charger_image("images/green.png", 0, 0));
        }
    }
}

void click_bouton_proposition(GtkWidget *pBouton, gpointer data)
{
    assert(pBouton != NULL);
    ControleurMastermind *cm = (ControleurMastermind *)data;
    assert(cm != NULL);

    cm->nb_boutons_clicked++;

    gtk_button_set_image(GTK_BUTTON(pBouton), charger_image(cm->vm->imageP, 30, 30));

    for (unsigned int i = 1; i <= NB_MANCHES; ++i)
        if (cm->mm->manche == i)
            for (unsigned int j = 0; j < NB_PIONS; ++j)
                if (pBouton == cm->pBoutonProposition[cm->mm->manche - 1][j])
                    cm->mm->historique->manche[i - 1]->proposition[j] = cm->mm->couleurCourante;

    if (cm->nb_boutons_clicked == NB_PIONS)
        gtk_widget_set_sensitive(cm->pBoutonOK, TRUE);
}

void click_bouton_ordi(GtkWidget *pBouton, gpointer data)
{
    assert(pBouton != NULL);

    ControleurMastermind *cm = (ControleurMastermind *)data;
    assert(cm != NULL);

    cm->nb_boutons_clicked++;
    if (cm->nb_boutons_clicked == NB_PIONS)
        gtk_widget_set_sensitive(cm->pBoutonOK, TRUE);

    gtk_button_set_image(GTK_BUTTON(pBouton), charger_image(cm->vm->imageP, 30, 30));
    for (unsigned int i = 0; i < NB_PIONS; ++i)
        if (pBouton == cm->pBoutonOrdi[i])
            cm->mm->historique->reponse[i] = cm->mm->couleurCourante;
}

void click_bouton_couleur(GtkWidget *pBouton, gpointer data)
{
    assert(pBouton != NULL);

    ControleurMastermind *cm = (ControleurMastermind *)data;
    assert(cm != NULL);

    if (pBouton == cm->pBoutonCouleur[0])
    {
        sprintf(cm->vm->imageP, "images/green.png");
        cm->mm->couleurCourante = vert;
    }
    if (pBouton == cm->pBoutonCouleur[1])
    {
        sprintf(cm->vm->imageP, "images/yellow.png");
        cm->mm->couleurCourante = jaune;
    }
    if (pBouton == cm->pBoutonCouleur[2])
    {
        sprintf(cm->vm->imageP, "images/red.png");
        cm->mm->couleurCourante = rouge;
    }
    if (pBouton == cm->pBoutonCouleur[3])
    {
        sprintf(cm->vm->imageP, "images/blue.png");
        cm->mm->couleurCourante = bleu;
    }
    if (pBouton == cm->pBoutonCouleur[4])
    {
        sprintf(cm->vm->imageP, "images/cyan.png");
        cm->mm->couleurCourante = cyan;
    }
    if (pBouton == cm->pBoutonCouleur[5])
    {
        sprintf(cm->vm->imageP, "images/purple.png");
        cm->mm->couleurCourante = mauve;
    }
    if (pBouton == cm->pBoutonCouleur[6])
    {
        sprintf(cm->vm->imageP, "images/orange.png");
        cm->mm->couleurCourante = orange;
    }
}

void click_bouton_ok(GtkWidget *pBouton, gpointer data)
{
    assert(pBouton != NULL);

    ControleurMastermind *cm = (ControleurMastermind *)data;
    assert(cm != NULL);

    cm->mm->manche++;

    gtk_widget_set_sensitive(cm->pBoutonOK, FALSE);
    cm->nb_boutons_clicked = 0;

    if (!cm->mm->modeOrdinateur)
    {
        if (cm->mm->manche > 10)
            gtk_label_set_text(GTK_LABEL(cm->vm->pLabelManche), "Perdu");
        else
            manche_maj(cm->vm->pLabelManche, cm->mm->manche);

        for (unsigned int k = 2; k <= NB_MANCHES; ++k)
        {
            if (cm->mm->manche == k)
            {
                for (unsigned int i = 0; i < NB_PIONS; ++i)
                {
                    gtk_widget_set_sensitive(cm->pBoutonProposition[k - 1][i], TRUE);
                    gtk_widget_set_sensitive(cm->pBoutonProposition[k - 2][i], FALSE);
                }
                compare_reponse_proposition(cm->mm, cm->mm->historique->manche[k - 2]);
                score(cm, cm->mm->historique->manche[k - 2]);
                victoire(cm, cm->mm->historique->manche[k - 2]);
            }
        }
        if (cm->mm->manche == 11)
        {
            for (unsigned int i = 0; i < NB_PIONS; ++i)
                gtk_widget_set_sensitive(cm->pBoutonProposition[9][i], FALSE);

            compare_reponse_proposition(cm->mm, cm->mm->historique->manche[9]);
            score(cm, cm->mm->historique->manche[9]);
            victoire(cm, cm->mm->historique->manche[9]);
        }
    }
    else
    {
        if (cm->mm->manche > 10)
            gtk_label_set_text(GTK_LABEL(cm->vm->pLabelManche), "Perdu");
        else
            manche_maj(cm->vm->pLabelManche, cm->mm->manche - 1);

        gtk_widget_set_sensitive(cm->pBoutonOK, TRUE);
        for (unsigned int i = 0; i < NB_PIONS; ++i)
            gtk_widget_set_sensitive(cm->pBoutonOrdi[i], FALSE);

        for (unsigned int k = 1; k <= NB_MANCHES + 1; ++k)
        {
            if (cm->mm->manche == k)
            {
                algorithme_ordinateur(cm->mm, cm->mm->historique->manche[k - 2]);
                score(cm, cm->mm->historique->manche[k - 2]);
                for (unsigned int i = 0; i < NB_PIONS; ++i)
                    gtk_button_set_image(GTK_BUTTON(cm->pBoutonProposition[k - 2][i]), charger_image(cm->mm->tab_imageP[i], 30, 30));
            }
        }
        unsigned int compteur = 0;
        for (unsigned int i = 0; i < NB_PIONS; ++i)
            if (cm->mm->historique->manche[cm->mm->manche - 2]->score[i] == 2)
                compteur++;
        if (compteur == NB_PIONS)
        {
            gtk_label_set_text(GTK_LABEL(cm->vm->pLabelManche), "L'ordinateur a trouvé votre combinaison!");
            gtk_widget_set_sensitive(cm->pBoutonOK, FALSE);
        }
        if (cm->mm->manche > NB_MANCHES && compteur != NB_PIONS)
        {
            gtk_label_set_text(GTK_LABEL(cm->vm->pLabelManche), "L'ordinateur n'a pas trouvé votre combinaison!");
            gtk_widget_set_sensitive(cm->pBoutonOK, FALSE);
        }
    }
}

void victoire(ControleurMastermind *cm, Combinaison *manche)
{
    assert(cm != NULL && manche != NULL);
    unsigned int cnt = 0;
    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        if (manche->score[i] == noir)
            cnt++;
    }

    if (cnt == NB_PIONS)
        manche->victoire = 1;

    if (manche->victoire == 1)
    {
        for (unsigned int i = 0; i < NB_MANCHES; i++)
        {
            for (unsigned int j = 0; j < NB_PIONS; j++)
            {
                gtk_widget_set_sensitive(cm->pBoutonProposition[i][j], FALSE);
                gtk_widget_set_sensitive(cm->pBoutonScore[i][j], FALSE);
            }
        }

        calcul_record(cm->mm->manche, cm->mm);
        tri_records(cm->mm->rec);
        char msg[50];
        sprintf(msg, "Gagné ! Tu as marqué %d points !", cm->mm->rec->scoreCourant);
        gtk_label_set_text(GTK_LABEL(cm->vm->pLabelManche), msg);
    }
}

void mode_humain_facile(GtkWidget *pF, gpointer data)
{
    assert(pF != NULL);

    ControleurMastermind *cm = (ControleurMastermind *)data;
    assert(cm != NULL);

    cm->mm->modeOrdinateur = 0;
    nouveau(pF, cm);
    cm->mm->modeFacile = 1;

    char message[100];
    sprintf(message, "Proposant : Humain (facile)");
    gtk_label_set_text(GTK_LABEL(cm->vm->pLabelMode), message);
}

void mode_humain_normal(GtkWidget *pF, gpointer data)
{
    assert(pF != NULL);

    ControleurMastermind *cm = (ControleurMastermind *)data;
    assert(cm != NULL);

    cm->mm->modeOrdinateur = 0;
    nouveau(pF, cm);
    cm->mm->modeFacile = 0;

    char message[100];
    sprintf(message, "Proposant : Humain (normal)");
    gtk_label_set_text(GTK_LABEL(cm->vm->pLabelMode), message);
}

void mode_ordinateur(GtkWidget *pF, gpointer data)
{
    assert(pF != NULL);

    ControleurMastermind *cm = (ControleurMastermind *)data;
    assert(cm != NULL);

    cm->mm->modeOrdinateur = 1;
    nouveau(pF, cm);

    char message[100];
    sprintf(message, "Proposant : Ordinateur");
    gtk_label_set_text(GTK_LABEL(cm->vm->pLabelMode), message);
}

void score(ControleurMastermind *cm, Combinaison *manche)
{
    assert(cm != NULL);

    for (unsigned int i = 0; i < NB_PIONS; ++i)
    {
        if (manche->score[i] == blanc)
        {
            sprintf(cm->vm->imageS, "images/pion_blanc.png");
            gtk_button_set_image(GTK_BUTTON(cm->pBoutonScore[cm->mm->manche - 2][i]), charger_image(cm->vm->imageS, 10, 10));
            gtk_widget_set_sensitive(cm->pBoutonScore[cm->mm->manche - 2][i], TRUE);
        }
        if (manche->score[i] == noir)
        {
            sprintf(cm->vm->imageS, "images/pion_noir.png");
            gtk_button_set_image(GTK_BUTTON(cm->pBoutonScore[cm->mm->manche - 2][i]), charger_image(cm->vm->imageS, 10, 10));
            gtk_widget_set_sensitive(cm->pBoutonScore[cm->mm->manche - 2][i], TRUE);
        }
    }
}