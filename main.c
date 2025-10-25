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

void multiplier_matrice_vecteur(int G[4][7], int info[4], int code[7]) {
    for (int i = 0; i < 7; i++) {
        code[i] = 0;
        for (int j = 0; j < 4; j++) {
            code[i] = (code[i] + G[j][i] * info[j]) % 2;
        }
    }
}

void calculer_syndrome(int H[3][7], int code[7], int syndrome[3]) {
    for (int i = 0; i < 3; i++) {
        syndrome[i] = 0;
        for (int j = 0; j < 7; j++) {
            syndrome[i] = (syndrome[i] + H[i][j] * code[j]) % 2;
        }
    }
}

int distance_hamming(int code1[7], int code2[7]) {
    int dist = 0;
    for (int i = 0; i < 7; i++) {
        if (code1[i] != code2[i]) {
            dist++;
        }
    }
    return dist;
}

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

void afficher_binaire(int vecteur[], int taille) {
    for (int i = 0; i < taille; i++) {
        printf("%d", vecteur[i]);
    }
}

void generer_tous_codes(void) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            info[3 - j] = (i >> j) & 1;
        }
        multiplier_matrice_vecteur(G, info, codes[i]);
    }
}

void calculer_tous_syndromes(void) {
    for (int i = 0; i < 16; i++) {
        calculer_syndrome(H, codes[i], syndromes[i]);
    }
}

void afficher_resultats(void) {
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║        CODE DE HAMMING (7,4) - RÉSULTATS             ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");

    printf("1. MOTS DE CODE GENERÉS\n");
    printf("────────────────────────────────────────\n");
    printf("Information | Mot de code Hamming\n");
    printf("────────────┼───────────────────\n");

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
    printf("────────────────────────────────────────\n");
    printf("Mot de code | Syndrome\n");
    printf("────────────┼──────────\n");

    for (int i = 0; i < 16; i++) {
        afficher_binaire(codes[i], 7);
        printf("   |   ");
        afficher_binaire(syndromes[i], 3);
        printf("\n");
    }

    printf("\n3. DISTANCE MINIMALE\n");
    printf("────────────────────────────────────────\n");
    int dist_min = trouver_distance_minimale(codes);
    printf("Distance minimale du code : %d\n", dist_min);
    printf("Correction d'erreurs : %d erreur(s)\n", (dist_min - 1) / 2);
    printf("Détection d'erreurs : %d erreur(s)\n\n", dist_min - 1);
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