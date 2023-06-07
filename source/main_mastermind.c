/*
 * main_mastermind.c
 *
 * Ce fichier contient la fonction main() du jeu Mastermind.
 *
 * @author: Naa Marco s175610 -- Hoorelbeke Jordi s175615
 * @date: 06/05/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include "modele_mastermind.h"
#include "vue_mastermind.h"
#include "controleur_mastermind.h"

int main(int argc, char **argv)
{
    // lancement de l'IHM
    GtkWidget *pFenetre;
    GtkWidget *pTable;
    GtkWidget *barre_menu;
    GtkWidget *menu_partie, *menu_aide, *menu_mode;
    GtkWidget *item_partie, *item_quitter, *item_nouvelle_partie, *item_records, *item_aide, *item_mode, *item_ordi, *item_humain_normal, *item_humain_facile;
    GtkWidget *item_separateur1, *item_apropos, *item_regles, *item_separateur2;

    // initialisation de l'environnement GTK
    gtk_init(&argc, &argv);

    // creation de la fenêtre
    pFenetre = creer_fenetre();

    // création du modèle de mastermind
    Ordi *ordi = creer_ordi();
    if (ordi == NULL)
        return EXIT_FAILURE;
    Combinaison *combinaison[NB_MANCHES];
    for (unsigned int i = 0; i < NB_MANCHES; ++i)
    {
        combinaison[i] = creer_combinaison();
        if (combinaison[i] == NULL)
            return EXIT_FAILURE;
    }
    Historique *historique = creer_historique(combinaison);
    if (historique == NULL)
        return EXIT_FAILURE;
    Records *rec = creer_records();
    if (rec == NULL)
        return EXIT_FAILURE;
    ModeleMastermind *mm = creer_modele_mastermind(historique, rec, ordi);
    if (mm == NULL)
        return EXIT_FAILURE;
    remplissage_reponse_aleatoire(mm);

    // création de la vue du mastermind
    struct vue_t *vm = creer_vue_mastermind(mm);
    if (vm == NULL)
        return EXIT_FAILURE;

    // création du contrôleur du mastermind
    ControleurMastermind *cm = creer_controleur_mastermind(mm, vm);
    if (cm == NULL)
        return EXIT_FAILURE;

    // Création de la barre de menu
    barre_menu = gtk_menu_bar_new();
    menu_partie = gtk_menu_new();
    menu_mode = gtk_menu_new();
    menu_aide = gtk_menu_new();
    item_partie = gtk_menu_item_new_with_mnemonic("_Partie");
    item_mode = gtk_menu_item_new_with_mnemonic("_Mode");
    item_aide = gtk_menu_item_new_with_mnemonic("_Aide");
    item_humain_facile = gtk_menu_item_new_with_label("Proposant: Humain (facile)");
    item_records = gtk_image_menu_item_new_with_label("Records");
    item_humain_normal = gtk_menu_item_new_with_label("Proposant: Humain (normal)");
    item_ordi = gtk_menu_item_new_with_label("Proposant: Ordinateur");
    item_nouvelle_partie = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
    item_separateur1 = gtk_separator_menu_item_new();
    item_separateur2 = gtk_separator_menu_item_new();
    item_quitter = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
    item_apropos = gtk_menu_item_new_with_label("A propos");
    item_regles = gtk_menu_item_new_with_label("Règles du jeu");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_partie), menu_partie);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_aide), menu_aide);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_mode), menu_mode);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_partie), item_nouvelle_partie);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_partie), item_records);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_partie), item_separateur1);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_partie), item_quitter);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_mode), item_humain_facile);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_mode), item_humain_normal);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_mode), item_separateur2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_mode), item_ordi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_aide), item_apropos);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_aide), item_regles);
    gtk_menu_shell_append(GTK_MENU_SHELL(barre_menu), item_partie);
    gtk_menu_shell_append(GTK_MENU_SHELL(barre_menu), item_mode);
    gtk_menu_shell_append(GTK_MENU_SHELL(barre_menu), item_aide);

    g_signal_connect(G_OBJECT(item_records), "activate", G_CALLBACK(fenetre_records), rec);
    g_signal_connect(G_OBJECT(item_nouvelle_partie), "activate", G_CALLBACK(nouveau), cm);
    g_signal_connect(G_OBJECT(item_quitter), "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(item_apropos), "activate", G_CALLBACK(apropos), NULL);
    g_signal_connect(G_OBJECT(item_regles), "activate", G_CALLBACK(regles), NULL);
    g_signal_connect(G_OBJECT(item_humain_facile), "activate", G_CALLBACK(mode_humain_facile), cm);
    g_signal_connect(G_OBJECT(item_humain_normal), "activate", G_CALLBACK(mode_humain_normal), cm);
    g_signal_connect(G_OBJECT(item_ordi), "activate", G_CALLBACK(mode_ordinateur), cm);

    // création de l'interface (pTable)
    pTable = gtk_table_new(15, 2 * NB_PIONS + 1, FALSE);
    gtk_table_attach(GTK_TABLE(pTable), barre_menu, 0, 2 * NB_PIONS, 0, 1, GTK_FILL, GTK_FILL, 0, 0);               // MENU
    gtk_table_attach(GTK_TABLE(pTable), vm->pLabelMode, 0, 2 * NB_PIONS, 1, 2, GTK_FILL, GTK_FILL, 0, 0);           // MODE
    gtk_table_attach(GTK_TABLE(pTable), cm->vm->pLabelManche, 0, 2 * NB_PIONS, 2, 3, GTK_EXPAND, GTK_EXPAND, 0, 0); // MANCHE COURANTE

    unsigned int C1, C2, L1, L2; // PROPOSITIONS | SCORES
    C1 = 0;
    C2 = 1;
    L1 = 12;
    L2 = 13;
    for (unsigned int i = 0; i < NB_MANCHES; i++)
    {
        for (unsigned int j = 0; j < NB_PIONS; j++)
        {
            gtk_table_attach(GTK_TABLE(pTable), cm->pBoutonProposition[i][j], C1, C2, L1, L2, GTK_EXPAND, GTK_EXPAND, 0, 0);
            g_signal_connect(G_OBJECT(cm->pBoutonProposition[i][j]), "clicked", G_CALLBACK(click_bouton_proposition), cm);
            gtk_widget_set_sensitive(cm->pBoutonProposition[i][j], FALSE);
            gtk_table_attach(GTK_TABLE(pTable), cm->pBoutonScore[i][j], C1 + NB_PIONS, C2 + NB_PIONS, L1, L2, GTK_EXPAND, GTK_EXPAND, 0, 0);
            gtk_widget_set_sensitive(cm->pBoutonScore[i][j], FALSE);
            C1++;
            C2++;
        }
        L1--;
        L2--;
        C1 = 0;
        C2 = 1;
    }
    for (unsigned int i = 0; i < NB_PIONS; ++i)
        gtk_widget_set_sensitive(cm->pBoutonProposition[0][i], TRUE);

    C1 = 0; // COULEURS POUR PROPOSITIONS
    C2 = 1;
    for (unsigned int i = 0; i < NB_COULEURS; i++)
    {
        gtk_table_attach(GTK_TABLE(pTable), cm->pBoutonCouleur[i], C1, C2, 13, 14, GTK_EXPAND, GTK_EXPAND, 10, 0);
        g_signal_connect(G_OBJECT(cm->pBoutonCouleur[i]), "clicked", G_CALLBACK(click_bouton_couleur), cm);
        C1++;
        C2++;
    }
    gtk_button_set_image(GTK_BUTTON(cm->pBoutonCouleur[0]), charger_image("images/green.png", 30, 30));
    gtk_button_set_image(GTK_BUTTON(cm->pBoutonCouleur[1]), charger_image("images/yellow.png", 30, 30));
    gtk_button_set_image(GTK_BUTTON(cm->pBoutonCouleur[2]), charger_image("images/red.png", 30, 30));
    gtk_button_set_image(GTK_BUTTON(cm->pBoutonCouleur[3]), charger_image("images/blue.png", 30, 30));
    gtk_button_set_image(GTK_BUTTON(cm->pBoutonCouleur[4]), charger_image("images/cyan.png", 30, 30));
    gtk_button_set_image(GTK_BUTTON(cm->pBoutonCouleur[5]), charger_image("images/purple.png", 30, 30));
    gtk_button_set_image(GTK_BUTTON(cm->pBoutonCouleur[6]), charger_image("images/orange.png", 30, 30));

    gtk_table_attach(GTK_TABLE(pTable), cm->pBoutonOK, 2 * NB_PIONS - 1, 2 * NB_PIONS, 13, 15, GTK_FILL, GTK_FILL, 10, 10); // BOUTON OK
    g_signal_connect(G_OBJECT(cm->pBoutonOK), "clicked", G_CALLBACK(click_bouton_ok), cm);
    gtk_widget_set_sensitive(cm->pBoutonOK, FALSE);

    C1 = 0; // BOUTONS MODE ORDI
    C2 = 1;
    for (unsigned int i = 0; i < NB_PIONS; i++)
    {
        gtk_table_attach(GTK_TABLE(pTable), cm->pBoutonOrdi[i], C1, C2, 14, 15, GTK_EXPAND, GTK_EXPAND, 10, 0);
        g_signal_connect(G_OBJECT(cm->pBoutonOrdi[i]), "clicked", G_CALLBACK(click_bouton_ordi), cm);
        C1++;
        C2++;
    }
    for (unsigned int i = 0; i < NB_PIONS; ++i)
        gtk_widget_set_sensitive(cm->pBoutonOrdi[i], FALSE);

    gtk_container_add(GTK_CONTAINER(pFenetre), GTK_WIDGET(pTable));

    // Affichage de la fenêtre et de ses éléments
    gtk_widget_show_all(pFenetre);

    // Boucle des évenements
    gtk_main();

    return EXIT_SUCCESS;
}