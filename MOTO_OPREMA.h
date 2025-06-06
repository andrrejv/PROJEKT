#ifndef MOTO_OPREMA_H
#define MOTO_OPREMA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "SLOZENI_TIPOVI.h"

void dodajOpremu(void);
MotoOprema* dohvatiOpremu(int* brojOpreme);
void ispisiSvuOpremu(void);
void azurirajOpremu(MotoOprema* oprema); 
void obrišiOpremu(MotoOprema* oprema); 


MotoOprema* nadjiOpremuPoID(int id);


MotoOprema* dohvatiPoKategoriji(Kategorija kategorija);
MotoOprema* dohvatiPoVelicini(Velicina velicina);
MotoOprema* dohvatiPoStanju(Stanje stanje);

void sortirajOpremuPoCijeni(void);
void pretraziOpremuPoID(void);

int generate_id(void);

Kategorija odaberiKategoriju(void);
Velicina odaberiVelicinu(void);
Stanje odaberiStanje(void);

const char* mapirajVelicinu(Velicina velicina);
const char* mapirajStanje(Stanje velicina);
const char* mapirajKategoriju(Kategorija kategorija);
#endif
