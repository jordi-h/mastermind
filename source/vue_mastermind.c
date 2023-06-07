/*
 * vue_mastermind.c
 *
 * Ce fichier contient les fonctions pour l'aspect vue(MVC) du jeu Mastermind.
 *
 * @author: Naa Marco s175610 -- Hoorelbeke Jordi s175615
 * @date: 06/05/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <gtk/gtk.h>
#include "vue_mastermind.h"

struct vue_t *creer_vue_mastermind(ModeleMastermind *mm)
{
    assert(mm != NULL);

    struct vue_t *vm = malloc(sizeof(struct vue_t));
    if (vm == NULL)
        return NULL;

    vm->mm = mm;
    vm->pLabelManche = gtk_label_new("Manche = 1");
    vm->pLabelMode = gtk_label_new("Proposant : Humain (normal)");
    sprintf(vm->imageP, "images/green.png");
    sprintf(vm->imageS, "images/pion_blanc.png");

    return vm;
}

GtkWidget *creer_fenetre()
{
    // création fenêtre
    GtkWidget *pF = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pF), "Mastermind");
    gtk_window_set_default_size(GTK_WINDOW(pF), 400, 800);
    gtk_window_set_position(GTK_WINDOW(pF), GTK_WIN_POS_CENTER);

    // Mouchard pour la destruction de la fenêtre
    g_signal_connect(G_OBJECT(pF), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return pF;
}

void fenetre_records(GtkWidget *pF, gpointer data)
{
    assert(pF != NULL);

    Records *rec = (Records *)data;
    assert(rec != NULL);

    GtkWidget *pFenetre;
    GtkWidget *pTable;
    GtkWidget *pLabel1;
    GtkWidget *pLabel[10];

    unsigned short L = 1;
    pFenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(pFenetre), 100, 200);
    gtk_window_set_title(GTK_WINDOW(pFenetre), "Records");
    pTable = gtk_table_new(1, 12, TRUE);
    pLabel1 = gtk_label_new("Top 10 des meilleurs scores");
    gtk_table_attach(GTK_TABLE(pTable), pLabel1, 0, 10, 0, 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    char message[1000];
    for (unsigned int i = 0; i < 10; ++i)
    {
        pLabel[i] = gtk_label_new("");
        sprintf(message, "%u : %d", i + 1, rec->tab_records[10 - (i + 1)]);
        gtk_label_set_text(GTK_LABEL(pLabel[i]), message);
        gtk_table_attach(GTK_TABLE(pTable), pLabel[i], 0, 1, L, L + 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
        L++;
    }
    gtk_container_add(GTK_CONTAINER(pFenetre), GTK_WIDGET(pTable));
    gtk_widget_show_all(pFenetre);
}

void apropos(GtkWidget *pF, gpointer data)
{
    assert(pF != NULL && data == NULL);

    GtkWidget *pFenetre;
    GtkWidget *pTable;
    GtkWidget *pLabel[3];

    pFenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(pFenetre), 250, 100);
    gtk_window_set_title(GTK_WINDOW(pFenetre), "A propos");
    pTable = gtk_table_new(4, 4, TRUE);
    pLabel[0] = gtk_label_new("-Mastermind-");
    pLabel[1] = gtk_label_new("Auteurs : Jordi Hoorelbeke & Marco Naa");
    for (unsigned int i = 0; i < 2; i++)
        gtk_table_attach(GTK_TABLE(pTable), pLabel[i], 0, 4, i, i + 1, GTK_EXPAND, GTK_EXPAND, 0, 0);
    gtk_container_add(GTK_CONTAINER(pFenetre), GTK_WIDGET(pTable));
    gtk_widget_show_all(pFenetre);
}

void regles(GtkWidget *pF, gpointer data)
{
    assert(pF != NULL && data == NULL);

    GtkWidget *pFenetre;
    GtkWidget *pTable;
    GtkWidget *pLabel[8];

    pFenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(pFenetre), 800, 800);
    gtk_window_set_title(GTK_WINDOW(pFenetre), "Règles du jeu");

    pTable = gtk_table_new(1, 1, TRUE);

    pLabel[0] = gtk_label_new("Proposant : Humain (normal)");
    pLabel[1] = gtk_label_new("Ce mode de jeu par défaut se joue avec les règles classiques du Mastermind."
                              "\nTentez de déchiffrer la combinaison secrète de couleurs en un nombre limité de propositions"
                              "\ngrâce aux indications données par l'ordinateur. Un pion noir indique une bonne couleur bien placée"
                              "\ntandis qu'un pion blanc indique une bonne couleur mal placée."
                              "\nDes points seront accordés en fonction du nombre de propositions qui vous ont été nécessaires"
                              "\net du temps que cela vous a pris.");
    pLabel[2] = gtk_label_new("Proposant : Humain (facile)");
    pLabel[3] = gtk_label_new("Les règles de ce mode de jeu sont les mêmes que celles du mode de jeu normal à deux exceptions"
                              "\nprès :"
                              "\n- Les scores donnés à chaque fin de manche occupent les mêmes positions que leurs propositions correspondantes."
                              "\n- Moins de points sont accordés à chaque fin de partie.");
    pLabel[4] = gtk_label_new("Proposant : Ordinateur");
    pLabel[5] = gtk_label_new("Les rôles s'inversent ! Dans ce mode de jeu, vous cachez une combinaison de couleurs à"
                              "\nl'ordinateur et celui-ci devra la déchiffrer en un nombre limité de propositions. Appuyez sur OK à chaque"
                              "\nfois que vous désirez connaître la combinaison de couleurs proposée par l'ordinateur."
                              "\nIl n'y a pas de système de points dans ce mode de jeu.");
    pLabel[6] = gtk_label_new("Bon jeu !");

    for (unsigned int i = 0; i < 7; i++)
        gtk_table_attach(GTK_TABLE(pTable), pLabel[i], 0, 1, i, i + 1, GTK_EXPAND || GTK_FILL, GTK_EXPAND || GTK_FILL, 0, 0);
    gtk_container_add(GTK_CONTAINER(pFenetre), GTK_WIDGET(pTable));
    gtk_widget_show_all(pFenetre);
}