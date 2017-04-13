#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(void) {
  char nameJ1 [11];
  char nameJ2 [11];
  char * mot = (char *) malloc(sizeof(char)*50);
  char * proposition;
  int mode;
  char retry;
  puts("\t\t*******PENDU*******\n\t(les mots sont entièrement en minuscule)");
  saisieNomJoueur(nameJ1, "J1");
  printf("Ok %s, veuillez choisir un mode :\n", nameJ1);
  do {
    choixMode(&mode);
     switch(mode) {
      case 1 : // J1 VS J2
        saisieNomJoueur(nameJ2, "J2");
        puts("L'un des 2 joueurs va être tiré au sort");
        if( (rand() % 2)+1 == 2) {
          initPartie(mot, &proposition, nameJ2);
          proposerLettre(mot, proposition, strlen(mot), nameJ1);
        }
        else {
          initPartie(mot, &proposition, nameJ1);
          proposerLettre(mot, proposition, strlen(mot), nameJ2);
        }
        break;
      case 2 : // J1 VS IA
        initPartie(mot, &proposition, nameJ1);
        proposerLettre(mot, proposition, strlen(mot), "IA");
        break;
      case 3 : // IA VS J1
        mot = motIA();
        affiche30ln();
        proposition = (char*) malloc(sizeof(char)*strlen(mot));
        memset(proposition, '-', strlen(mot));
        proposerLettre(mot, proposition, strlen(mot), nameJ1);
        break;
      default :
        break;
    }
    puts("Voulez vous recommencer ? [y/n]");
    scanf("%1c", &retry);
    clear_kb();
    affiche30ln();
  } while (retry == 'y' || retry == 'Y' );
  puts("Au revoir");
  return 0;
}
void saisieNomJoueur(char * nom, char * idJ) { 
  printf("Bonjour %s, veuillez saisir votre nom (10 caractères max): \n", idJ);
  char *positionEntree = NULL;
  if (fgets(nom, 11, stdin) != NULL){
      positionEntree = strchr(nom, '\n');
      if (positionEntree != NULL)
          *positionEntree = '\0';
  }
}
void clear_kb(void) {
  char junk[80];
  fgets(junk, sizeof(junk), stdin);
}
void choixMode(int * mode) {
  puts(" 1.Faire deviner un mot à un 2ème joueur\n 2.Faire deviner un mot à une IA\n 3.Deviner un mot choisie par une IA\n\nEntrez votre choix : ");
  scanf("%1d", mode);
  while (!(*mode >= 1 && *mode<=3)) {
    puts("Erreur! Choisissez une option du menu (1, 2 ou 3) : ");
    scanf("%d", mode);
  }
  clear_kb();
}
void saisieMot(char * mot) {
  char *positionEntree = NULL;
  if (fgets(mot, 50, stdin) != NULL){
      positionEntree = strchr(mot, '\n');
      if (positionEntree != NULL)
          *positionEntree = '\0';
  }
  mot = (char *) realloc(mot, sizeof(char)*strlen(mot)+1);
}
void affiche30ln() {
  int i = 0;
  while (i != 31) {
    puts("\n");
    i++;
  }
}
void initPartie(char * mot, char ** proposition, char * nom) {
  printf("Très bien %s, tu dois saisir le mot mystère : ", nom);
  saisieMot(mot);
  affiche30ln();
  *proposition = (char*) malloc(sizeof(char)*strlen(mot));
  //if(*proposition == NULL)
    //puts("error malloc\n");
  memset(*proposition, '-', strlen(mot));
}
int nbocc(char * proposition, char c) {
  int i = 0, occ = 0;
  while (*(proposition+i) != '\0') {
    if (*(proposition+i) == c)
      occ++;
    i++;
  }
  return occ;
}
void addTableau(char * proposition, char c, int essai) {
  if(*(proposition+essai) == ' ')
    *(proposition+essai) = c;
  else
    *(proposition+essai+1) = c;
}
int placement(char * mot, char * proposition, char c) {
  int i = 0, occ = 0;
  while (*(mot+i) != '\0') {
    if (*(mot+i) == c) {
      *(proposition+i) = *(mot+i);
      occ++;
    }
    i++;
  }
  return occ;
}
void proposerLettre(char * mot, char * proposition, int nbessai, char * nomJoueur) { 
  int essai = 1;
  char dejaPropose[strlen(mot)];
  memset(dejaPropose, ' ', strlen(mot));
  char c;
  printf("%s a le droit à %d tentatives pour deviner le mot mystère !\n", nomJoueur, nbessai);
  while (essai != nbessai + 1 && nbocc(proposition, '-') != 0) {
    printf("\nEssai n°%d, lettre :", essai);
    if(nomJoueur != "IA") { // Humain
      scanf("%1c", &c);
      clear_kb();
      if(nbocc(dejaPropose, c) != 0) {
        while(nbocc(dejaPropose, c) > 0) {
          printf("Erreur ! Tu as déjà proposé la lettre %c, tentes en une autre :", c);
          scanf("%1c", &c);
          clear_kb();
        }
      }
    } else { // IA
      c = (char) (rand() % 26) + 97;
      while (nbocc(dejaPropose, c) > 0)
        c = (char) (rand() % 26) + 97;
    }
    addTableau(dejaPropose, c, essai-1);
    printf("Lettres déjà proposées : %s\n", dejaPropose);
    if(placement(mot, proposition, c) == 0) {
      printf("Eh non, la lettre %c n'y est pas !\n", c);
      essai++;
    }
    else
      printf("Bravo la lettre %c est bien présente ! Cette tentative ne compte donc pas\n", c);
    printf("%s\n", proposition);
    if(nbocc(proposition, '-') >= 1 && essai == nbessai + 1) {
      printf("\nPerdu,le mot était %s\n", mot);
    } else if (nbocc(proposition, '-') == 0)
      printf("\t*******Bravo ! %s a trouvé le mot en %d essai(s)*******\n", nomJoueur, essai);
  }
}
char * motIA() {
  switch(rand() % 6) {
    case 0 :
      return "un";    
    case 1 :
      return "jazz";
    case 2 :
      return "hydrophobie";
    case 3 :
      return "eclectique";
    case 4 :
      return "ornithorynque";
    case 5 :
      return "aminomethylpyrimidinylhydroxyethylmethythiazolium";
    default :
      motIA();
  }
}