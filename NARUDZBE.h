#ifndef NARDUZBE_H
#define NARDUZBE_H

#include "SLOZENI_TIPOVI.h"

void dodajNarudzbu(void); // CREATE
void ispisiSveNarudzbe(void);
Narudzba* dohatiSveNarudzbe(int* brojNarudzbi); // READ
void obrisiNarudzbu(Narudzba narduzba); // DELETE

MotoOprema odaberiOpremu(MotoOprema* sveOpreme, int brojOpreme);
int odaberiKorisnika(Korisnik* sviKorisnici, int brojKorisnika);
#endif
