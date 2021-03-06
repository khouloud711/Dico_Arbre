#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "dico.h"

int nombreAleatoire(int nombreMax)
{
    srand(time(NULL));           // initialiser le générateur de nombres aléatoires
    return (rand() % nombreMax); //renvoie d'un nombre au hasard entre 0 et nombreMax
}

int piocherMot(char *motPioche)
{
    FILE *dico = NULL;
    int nombreMots = 0, numMotChoisi = 0, i = 0;
    int caractereLu = 0;

    dico = fopen("dico.txt", "r"); //lecture seule

    if (dico == NULL)
    {
        printf("\nImpossible de charger le dictionnaire de mots");
        return 0;
    }

    //nombre de mots dans le fichier
    do
    {
        caractereLu = fgetc(dico);
        if (caractereLu == '\n')
            nombreMots++;
    } while (caractereLu != EOF);

    numMotChoisi = nombreAleatoire(nombreMots); // On pioche un mot au hasard

    rewind(dico);
    while (numMotChoisi > 0)
    {
        caractereLu = fgetc(dico);
        if (caractereLu == '\n')
            numMotChoisi--;
    }

    //lire la ligne du mot choisi
    fgets(motPioche, 100, dico);

    //vire le \n à la fin
    motPioche[strlen(motPioche) - 1] = '\0';
    fclose(dico);

    return 1;
}

void afficherElements(char mot[], TArbre a)
{
    if (a != NULL)
    {
        if (arbreRacineLettre(a) != '\0')
        {
            if (a->fg != NULL)
            {
                afficherElements(mot, a->fd);
            }
            if (a->fg != NULL)
            {
                strcat(mot, (char[2]){arbreRacineLettre(a), '\0'});
                afficherElements(mot, arbreFilsGauche(a));
            }
            mot[strlen(mot) - 1] = '\0';
        }
        else
        {
            printf("\n \t\t \"%s\"\n", mot);
            if (a->fd != NULL)
                afficherElements(mot, arbreFilsDroit(a));
        }
    }
}

void dicoAfficher(TArbre a)
{
    printf("\n \t **** L'arbre contient ces mots: ****\n");
    char mots[255] = "";
    afficherElements(mots, a);
}

void dicoInsererMot(char mot[], TArbre *pa)
{
    if (*pa != NULL)
    {
        if (mot[0] != '\0')
        {
            if ((*pa)->lettre == mot[0])
            {
                mot++;
                dicoInsererMot(mot, &((*pa)->fg));
            }
            else
            {
                if ((*pa)->fd != NULL)
                {
                    dicoInsererMot(mot, &((*pa)->fd));
                }
                else
                {
                    (*pa)->fd = arbreCons(mot[0], 0, NULL, NULL);
                    dicoInsererMot(mot, &(*pa));
                }
            }
        }
        else if ((*pa)->lettre != '\0' && mot[0] == '\0')
        {
            TArbre a = arbreCons('\0', 1, NULL, *pa);
            *pa = a;
        }
        else if ((*pa)->lettre == '\0' && mot[0] == '\0')
        {
            (*pa)->nb = (*pa)->nb + 1;
        }
    }
    else
    {
        if (mot[0] != '\0')
        {
            *pa = arbreCons(mot[0], 0, NULL, NULL);
            mot++;
            dicoInsererMot(mot, &((*pa)->fg));
        }
        else
        {
            *pa = arbreCons('\0', 1, NULL, NULL);
        }
    }
}

int dicoNbOcc(char mot[], TArbre a)
{
    int i = 0, tmp = 0;

    //comparer les noeuds à droite jusqu'à trouver la premiere lettre du mot
    if (mot[i] != arbreRacineLettre(a))
    {
        while (!arbreEstVide(a) && mot[i] != arbreRacineLettre(a))
        {
            a = arbreFilsDroit(a);
        }
        if (arbreEstVide(a))
        {
            return 0;
        }
    }
    //1er lettre trouvé, continuer à gauche
    while (mot[i] != '\0' && !arbreEstVide(a))
    {

        if (mot[i] != '\0')
        {
            if (mot[i] == arbreRacineLettre(a))
            {
                i++;
                a = arbreFilsGauche(a);
            }
            else
            {
                a = arbreFilsDroit(a);
            }
        }
    }
    if (mot[i] == '\0')
    {
        return arbreRacineNbOcc(a);
    }
    return 0;
}

int dicoNbMotsDifferents(TArbre a)
{
    if (!arbreEstVide(a))
    {
        if (arbreRacineLettre(a) == '\0')
        {
            return 1 + dicoNbMotsDifferents(arbreFilsGauche(a)) + dicoNbMotsDifferents(arbreFilsDroit(a));
        }

        return dicoNbMotsDifferents(arbreFilsGauche(a)) + dicoNbMotsDifferents(arbreFilsDroit(a));
    }
    else
    {
        return 0;
    }
}

int dicoNbMotsTotal(TArbre a)
{
    int nb = 0;
    if (arbreEstVide(a) == 1)
    {
        return 0;
    }
    else
    {
        if (a->lettre == '\0')
        {
            nb = nb + a->nb;
        }
        return nb + dicoNbMotsTotal(arbreFilsGauche(a)) + dicoNbMotsTotal(arbreFilsDroit(a));
    }
}

