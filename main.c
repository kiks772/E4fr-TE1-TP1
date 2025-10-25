#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   DÉCLARATION DES VARIABLES GLOBALES
   ============================================================ */
int G[4][7] = {
    {1, 0, 0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0, 1, 1},
    {0, 0, 1, 0, 1, 1, 1},
    {0, 0, 0, 1, 1, 0, 1}
};

int H[3][7] = {
    {1, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 0, 1, 0},
    {0, 1, 1, 1, 0, 0, 1}
};

int codes[16][7];
int syndromes[16][3];
int info[4];
int code[7];
int syndrome[3];

/* ============================================================
   DÉCLARATION DES FONCTIONS
   ============================================================ */
void multiplier_matrice_vecteur(int G[4][7], int info[4], int code[7]);
void calculer_syndrome(int H[3][7], int code[7], int syndrome[3]);
int distance_hamming(int code1[7], int code2[7]);
int trouver_distance_minimale(int codes[16][7]);
void afficher_binaire(int vecteur[], int taille);
void generer_tous_codes(void);
void calculer_tous_syndromes(void);
void afficher_resultats(void);

/* ============================================================
   DÉFINITION DES FONCTIONS
   ============================================================ */

// FONCTION: multiplier_matrice_vecteur
// DESCRIPTION: Effectue la multiplication de la matrice génératrice G par le vecteur d'information
//              pour générer un mot de code. L'opération se fait en arithmétique modulo 2 (XOR).
// PARAMETRES ENTREE:
//   - G[4][7] : Matrice génératrice du code de Hamming (4 lignes, 7 colonnes)
//   - info[4] : Vecteur d'information de 4 bits à encoder
// PARAMETRES SORTIE:
//   - code[7] : Vecteur résultant du mot de code de 7 bits
// FORMULE: code = G * info (mod 2)
void multiplier_matrice_vecteur(int G[4][7], int info[4], int code[7]) {
    for (int i = 0; i < 7; i++) {
        code[i] = 0;
        for (int j = 0; j < 4; j++) {
            code[i] = (code[i] + G[j][i] * info[j]) % 2;
        }
    }
}

// FONCTION: calculer_syndrome
// DESCRIPTION: Calcule le syndrome d'un mot de code pour détecter et localiser les erreurs.
//              Le syndrome est obtenu en multipliant la matrice de contrôle H par le mot de code.
//              Un syndrome = 000 indique qu'il n'y a pas d'erreur.
//              Sinon, le syndrome indique la position de l'erreur.
// PARAMETRES ENTREE:
//   - H[3][7] : Matrice de contrôle de parité (3 lignes, 7 colonnes)
//   - code[7] : Mot de code de 7 bits reçu
// PARAMETRES SORTIE:
//   - syndrome[3] : Vecteur de 3 bits représentant le syndrome
// FORMULE: syndrome = H * code^T (mod 2)
void calculer_syndrome(int H[3][7], int code[7], int syndrome[3]) {
    for (int i = 0; i < 3; i++) {
        syndrome[i] = 0;
        for (int j = 0; j < 7; j++) {
            syndrome[i] = (syndrome[i] + H[i][j] * code[j]) % 2;
        }
    }
}

// FONCTION: distance_hamming
// DESCRIPTION: Calcule la distance de Hamming entre deux mots de code.
//              C'est le nombre de positions où les deux codes diffèrent (nombre de bits différents).
// PARAMETRES ENTREE:
//   - code1[7] : Premier mot de code de 7 bits
//   - code2[7] : Deuxième mot de code de 7 bits
// PARAMETRES SORTIE:
//   - Retour: Nombre entier représentant la distance (nombre de positions différentes)
// EXEMPLE: distance_hamming([1,0,1,0,0,0,0], [1,0,0,0,0,0,0]) = 1
int distance_hamming(int code1[7], int code2[7]) {
    int dist = 0;
    for (int i = 0; i < 7; i++) {
        if (code1[i] != code2[i]) {
            dist++;
        }
    }
    return dist;
}

// FONCTION: trouver_distance_minimale
// DESCRIPTION: Parcourt tous les couples de mots de code et trouve la distance minimale.
//              La distance minimale du code détermine sa capacité de correction et détection d'erreurs.
//              - Capacité de correction: floor((d_min - 1) / 2)
//              - Capacité de détection: d_min - 1
// PARAMETRES ENTREE:
//   - codes[16][7] : Tableau contenant tous les 16 mots de code possibles
// PARAMETRES SORTIE:
//   - Retour: Valeur entière de la distance minimale
// NOTE: Pour le code Hamming (7,4), la distance minimale est toujours 3
int trouver_distance_minimale(int codes[16][7]) {
    int dist_min = 7;
    for (int i = 0; i < 16; i++) {
        for (int j = i + 1; j < 16; j++) {
            int dist = distance_hamming(codes[i], codes[j]);
            if (dist < dist_min) {
                dist_min = dist;
            }
        }
    }
    return dist_min;
}

// FONCTION: afficher_binaire
// DESCRIPTION: Affiche un vecteur de bits (tableau d'entiers 0 et 1) au format binaire.
//              Les bits sont affichés sans séparation, l'un après l'autre.
// PARAMETRES ENTREE:
//   - vecteur[] : Tableau d'entiers contenant les bits à afficher
//   - taille : Nombre de bits à afficher
// PARAMETRES SORTIE:
//   - Affichage à l'écran (pas de retour de valeur)
// EXEMPLE: afficher_binaire([1,0,1,0,1,0,0], 7) affiche "1010100"
void afficher_binaire(int vecteur[], int taille) {
    for (int i = 0; i < taille; i++) {
        printf("%d", vecteur[i]);
    }
}

// FONCTION: generer_tous_codes
// DESCRIPTION: Génère tous les 16 mots de code possibles pour le code Hamming (7,4).
//              Pour chaque information de 4 bits (0000 à 1111), calcule le mot de code correspondant
//              en multipliant par la matrice génératrice G.
// PARAMETRES ENTREE:
//   - Aucun (utilise les variables globales G, info et codes)
// PARAMETRES SORTIE:
//   - Remplit le tableau global codes[16][7] avec tous les mots de code
// NOTE: Cette fonction doit être appelée avant calculer_tous_syndromes()
void generer_tous_codes(void) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            info[3 - j] = (i >> j) & 1;
        }
        multiplier_matrice_vecteur(G, info, codes[i]);
    }
}

// FONCTION: calculer_tous_syndromes
// DESCRIPTION: Calcule le syndrome pour chacun des 16 mots de code générés.
//              Les syndromes des mots de code valides doivent tous être égaux à 000.
// PARAMETRES ENTREE:
//   - Aucun (utilise les variables globales H, codes et syndromes)
// PARAMETRES SORTIE:
//   - Remplit le tableau global syndromes[16][3] avec les syndromes correspondants
// PREREQUIS: generer_tous_codes() doit avoir été appelée avant
// NOTE: Pour un code sans erreur, syndrome = 000
void calculer_tous_syndromes(void) {
    for (int i = 0; i < 16; i++) {
        calculer_syndrome(H, codes[i], syndromes[i]);
    }
}

// FONCTION: afficher_resultats
// DESCRIPTION: Affiche de manière formatée et lisible les résultats complets du code de Hamming (7,4):
//              1. Tous les 16 mots de code générés avec leur information correspondante
//              2. Les syndromes de chaque mot de code
//              3. La distance minimale et les capacités de correction/détection d'erreurs
// PARAMETRES ENTREE:
//   - Aucun (utilise les variables globales codes et syndromes)
// PARAMETRES SORTIE:
//   - Affichage formaté à l'écran avec tableaux et résultats
// NOTE: Cette fonction appelle trouver_distance_minimale() pour calculer la distance minimale
void afficher_resultats(void) {
    printf("\n=================================================\n");
    printf("  CODE DE HAMMING (7,4) - RESULTATS\n");
    printf("=================================================\n\n");

    printf("1. MOTS DE CODE GENERES\n");
    printf("------------------------------------------\n");
    printf("Information | Mot de code Hamming\n");
    printf("------------+---------------------\n");

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            info[3 - j] = (i >> j) & 1;
        }
        printf("    ");
        afficher_binaire(info, 4);
        printf("    |    ");
        afficher_binaire(codes[i], 7);
        printf("\n");
    }

    printf("\n2. SYNDROMES DES MOTS DE CODE\n");
    printf("------------------------------------------\n");
    printf("Mot de code | Syndrome\n");
    printf("------------+----------\n");

    for (int i = 0; i < 16; i++) {
        afficher_binaire(codes[i], 7);
        printf("   |   ");
        afficher_binaire(syndromes[i], 3);
        printf("\n");
    }

    printf("\n3. DISTANCE MINIMALE\n");
    printf("------------------------------------------\n");
    int dist_min = trouver_distance_minimale(codes);
    printf("Distance minimale du code : %d\n", dist_min);
    printf("Correction d'erreurs : %d erreur(s)\n", (dist_min - 1) / 2);
    printf("Detection d'erreurs : %d erreur(s)\n\n", dist_min - 1);
}

/* ============================================================
   FONCTION PRINCIPALE
   ============================================================ */

int main(void) {
    generer_tous_codes();
    calculer_tous_syndromes();
    afficher_resultats();

    return EXIT_SUCCESS;
}