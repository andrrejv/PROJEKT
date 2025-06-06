#ifndef KorisnikServis_h
#define KorisnikServis_h
#include <stdio.h>
#include "SLOZENI_TIPOVI.h"


void dodajKorisnika(void);
void ispisiSveKorisnike(void);
Korisnik* dohvatiSveKorisnike(int* brojKorisnika);
void obrisiKorisnika(Korisnik korisnik);

inline char* dohvatiString(char* ispisnaVrijednost) {
    char buffer[1024];
    printf("%s", ispisnaVrijednost);
    fgets(buffer, sizeof(buffer), stdin);

    buffer[strcspn(buffer, "\n")] = 0;

    char* ime = malloc(strlen(buffer) + 1);
    if (!ime) {
        perror("Greška pri alokaciji memorije");
        exit(1);
    }
    strcpy(ime, buffer);
    return ime;
}

extern int brojKorisnika;
char* upisiIme(void);
char* upisiAdresu(void);
int upisiBroj(void);
char* upisiEmail(void);
#endif 
