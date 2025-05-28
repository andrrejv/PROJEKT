#ifndef KorisnikServis_h
#define KorisnikServis_h
#include <stdio.h>
#include "SLOZENI_TIPOVI.h"


void dodajKorisnika(void);
void ispisiSveKorisnike(void);
Korisnik* dohvatiSveKorisnike(int* brojKorisnika);
void obrisiKorisnika(Korisnik korisnik);

char* upisiIme(void);
char* upisiAdresu(void);
int upisiBroj(void);
char* upisiEmail(void);
#endif 
