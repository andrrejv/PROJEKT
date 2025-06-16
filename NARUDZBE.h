#define _CRT_SECURE_NO_WARNINGS

#ifndef NARDUZBE_H
#define NARDUZBE_H

#include "SLOZENI_TIPOVI.h"

void dodajNarudzbu(void); 
int ispisiSveNarudzbe(bool saIzborom);
Narudzba* dohatiSveNarudzbe(int* brojNarudzbi); 
void obrisiNarudzbu(void); 
MotoOprema odaberiOpremu(MotoOprema* sveOpreme, int brojOpreme);
int odaberiKorisnika(Korisnik* sviKorisnici, int brojKorisnika);
#endif
