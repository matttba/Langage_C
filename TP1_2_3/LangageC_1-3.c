//%cflags: -I .
#include "affichage.h"

typedef unsigned long long int ull_t; // On met souvent _t à la fin d'un nom de type, 
                                      // pour signifier que c'est un type
typedef short entier_t; // Peut être que short n'est pas assez grand
//typedef int entier_t; // On prépare d'autres alternatives (une seule à la fois)
//typedef long entier_t;

ull_t plus_2(ull_t x) {
    return x + 2;
}

entier_t gros_calcul(entier_t x) {
    entier_t a = 121, b = 34, c = 54;
    entier_t y = a * x / b + c;
    entier_t z = 2 * y - x;
    return z;
}

int main() {
    ull_t a = 1234681232L;
    afficher_entier(a);
    unsigned long long int b = a - 2; // Les types sont absolument identiques et interchangeables
    a = plus_2(b);
    afficher_entier(a);
    
    entier_t z = gros_calcul(1);
    afficher_entier(z);
    
    return 0;
}

//%cflags: -I .
#include "affichage.h"

enum couleur {
    rouge, bleu, vert, jaune
}; // C'est une instruction, on oublie pas le point-virgule !

void est_bleu(enum couleur c) {
    if (c == bleu) {
        puts("La couleur est bien le bleu");
    } else {
        puts("La couleur n'est pas le bleu");
    }
}

int main() {
    enum couleur c1 = rouge; // Le type est bien `enum couleur`
    enum couleur c2 = bleu;
    
    est_bleu(c1);
    est_bleu(c2);
    
    return 0;
}

//%cflags: -I .
#include "affichage.h"

enum couleur {
    rouge = 1, bleu = 5, vert = 10, jaune = 20, 
    violet /** pas de valeur : probablement 21 (successeur de 20) */
};
typedef enum couleur couleur; // alias "couleur" pour le type "enum couleur" pour que ça soit moins long à écrire

void afficher_couleur(couleur c) {
    switch (c) {
        case rouge:
            puts("la couleur est le rouge");
            break;
        case bleu:
            puts("la couleur est le bleu");
            break;
        case vert:
            puts("la couleur est le vert");
            break;
        case jaune:
            puts("la couleur est le jaune");
            break;
        case violet:
            puts("la couleur est le violet");
            break;
        default: // default obligatoire sachant que la valeur pourraît être tout autre chose !
            puts("la couleur est le ???");
    }
}

int main() {
    couleur c = jaune; // Plus besoin du mot-clef enum
    afficher_entier(c);
    afficher_entier(rouge + 1); // Ça se comporte vraiment comme un entier
    afficher_entier(violet);
    
    // Ce genre de tour de passe-passe est un peu dangereux...
    afficher_couleur(c);
    c -= 10;
    afficher_couleur(c);
    c += 1;
    afficher_couleur(c);
    
    return 0;
}

//%cflags: -I .
#include "affichage.h"

struct monnaie {
    int unites;
    int centimes;
    char devise;
};
typedef struct monnaie monnaie; // Pour éviter de devoir écrire "struct monnaie" partout

void afficher_devise(monnaie m) {
    afficher_carac(m.devise); // Accès au champ "devise" en lecture
    m.unites = 'a';
    afficher_entier(m.unites); // Attention cette modification n'est par répercutée en dehors de la fonction 
                    // (paramètre in par défaut)
}

int main() {
    monnaie m1;
    m1.unites = 1;   // Accès au champ "unites" en écriture
    m1.centimes = 5;
    m1.devise = 'c';
    afficher_devise(m1);
    
    // On peut initialiser un enregistrement avec un autre (ce qui en fait une copie)
    monnaie m2 = m1;
    afficher_entier(m2.unites);
    afficher_entier(m2.centimes);
    afficher_devise(m2);
    afficher_carac(m2.devise);
    m2.unites = 25;
    afficher_entier(m2.unites);
    afficher_entier(m1.unites);
    
    return 0;
}

//%cflags: -I .
#include "affichage.h"

enum couleur {
    COEUR, PIQUE, CARREAU, TREFLE
};
typedef enum couleur couleur;

struct carte {
    int valeur;
    couleur couleur;
};
typedef struct carte carte;

void changer_valeur(carte* carte, int valeur) { // carte en out (en in-out mais on ne se sert pas du in)
    carte->valeur = valeur;
    // (*carte).valeur = valeur;     // < absolument équivalent
}

int main() {
    carte c;
    c.valeur = 10;
    c.couleur = COEUR;
    
    afficher_entier(c.valeur);
    afficher_entier(c.couleur);
    
    nouvelle_ligne();
    
    changer_valeur(&c, 11);
    
    afficher_entier(c.valeur);
    afficher_entier(c.couleur);
    
    return 0;
}

//%cflags: -I .
//%ldflags: -L. -lt
#include "affichage.h"
#include "tests.h"

enum type_cara {
    ESPACES, MINUSCULES, MAJUSCULES, CHIFFRES
};

struct resultat {
    enum type_cara type;
    int valeur;
};

struct resultat compter(const char* chaine, enum type_cara type) {
    int val = 0;
    char min;
    char max;
    if (type == MINUSCULES) {
        min = 'a';
        max = 'z'; 
    }
    else if (type == MAJUSCULES) {
        min = 'A';
        max = 'Z';
    }
    else if (type == CHIFFRES) {
        min = '0';
        max = '9';
    }
    else if (type == ESPACES) {
        min = ' ';
        max = ' ';
    }
    else {}
    int i = 0;
    while (chaine[i] != 0) {
        if (chaine[i] <= max && chaine[i] >= min) {
            ++val; 
        }
        else {}
        ++i;
    }
    struct resultat res;
    res.type = type;
    res.valeur = val;
    return res;
}

int main() {
INITIALIZE_TESTS();
    
BEGIN_SECTION("nominal")
    struct resultat res;
    char* chaine = "J'ai 700 Copies A Corriger";
    
    BEGIN_TESTI("compter-espaces")
    res = compter(chaine, ESPACES);
        
    ASSERT_EQ(res.type, ESPACES);
    ASSERT_EQ(res.valeur, 4);
    END_TEST
    
    BEGIN_TESTI("compter-min")
    res = compter(chaine, MINUSCULES);
    ASSERT_EQ(res.type, MINUSCULES);
    ASSERT_EQ(res.valeur, 14);
    END_TEST
    
    BEGIN_TESTI("compter-espaces")
    res = compter(chaine, MAJUSCULES);
    ASSERT_EQ(res.type, MAJUSCULES);
    ASSERT_EQ(res.valeur, 4);
    END_TEST
    
    BEGIN_TESTI("compter-espaces")
    res = compter(chaine, CHIFFRES);
    ASSERT_EQ(res.type, CHIFFRES);
    ASSERT_EQ(res.valeur, 3);
    END_TEST
    
    REPORT_TO_STDOUT

END_SECTION()
    
    return 0;
}

#include <stdio.h> // Les fonctions se trouvent dans le module stdio

int main() {
    putchar('a');
    putchar('b');
    putchar('\n');
    
    puts("J'aime le C");
    puts("C'est un très bon\nlangage, bien meilleur que\nPython");
    
    for (char c = 'a'; c <= 'z'; c++) { // Attention il faut bien que 'a' < 'z' sinon ça ne finit pas
        putchar(c);
    }
    putchar('\n');
    
    return 0;
}

#include <stdio.h>

int main() {
    printf("Ceci est une chaîne sans trou");
    printf("\n"); // Par défaut pas de retour à la ligne avec printf ; donc j'en ajoute un pour la présentation
    
    printf("Il y a un trou de type entier : %d\n", 151 + 293);
    printf("Il y a un trou de type flottant : %f\n", 1.0 / 0.00213);
    
    printf("Des trous : %d, %c, %f, %s\n", 151, 'a', 0.1, "<<>>");
    
    double sqrt2 = 1.41421356237;
    printf("Un nombre pas précis : %.1f\nUn nombre très précis : %.10f\n", sqrt2, sqrt2);
    return 0;
}

// On intègre la librairie math (simplement appelée m) pour l'édition des liens
//%ldflags: -lm
#include <stdlib.h>
#include <stdio.h>
#include <math.h>     // le module math

double f(double x) {
    return sqrt(fabs(x)) / (1.0 + atan(x));
}

int main() {
    printf("exp(1) = %.6f\n", exp(1.0));
    
    for (int i = -10; i <= 10; i++) {
        double x = (double) i;
        printf("f(%+6.2f) = %+e\n", x, f(x));
    }
    return 0;
}
