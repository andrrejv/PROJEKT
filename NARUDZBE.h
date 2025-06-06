#ifndef NARDUZBE_H
#define NARDUZBE_H

#include "SLOZENI_TIPOVI.h"

void dodajNarudzbu(void); 
void ispisiSveNarudzbe(void);
Narudzba* dohatiSveNarudzbe(int* brojNarudzbi); 
void obrisiNarudzbu(Narudzba narduzba); 

MotoOprema odaberiOpremu(MotoOprema* sveOpreme, int brojOpreme);
int odaberiKorisnika(Korisnik* sviKorisnici, int brojKorisnika);
#endif
