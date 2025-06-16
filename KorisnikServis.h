#define _CRT_SECURE_NO_WARNINGS

#ifndef KorisnikServis_h
#define KorisnikServis_h
#include <stdio.h>
#include "SLOZENI_TIPOVI.h"
#include "stdlib.h"
#include "string.h"

void dodajKorisnika(void);
int ispisiSveKorisnike(bool saIzborom);
Korisnik* dohvatiSveKorisnike(int* brojKorisnika);
void obrisiKorisnika(void);

char* upisiIme(void);
char* upisiAdresu(void);
int upisiBroj(void);
char* upisiEmail(void);

inline char* dohvatiString(char* ispisnaVrijednost) {
    char buffer[1024];
    printf("%s", ispisnaVrijednost);
    fgets(buffer, sizeof(buffer), stdin);

    buffer[strcspn(buffer, "\n")] = 0;

    char* upisnaVrijednost = malloc(strlen(buffer) + 1);
    if (!upisnaVrijednost) {
        perror("Greska pri alokaciji memorije");
        exit(1);
    }
    strcpy(upisnaVrijednost, buffer);
    return upisnaVrijednost;
}

extern int brojKorisnika;
#endif 
