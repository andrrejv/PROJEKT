#define _CRT_SECURE_NO_WARNINGS

#ifndef SLOZENI_TIPOVI_H
#define SLOZENI_TIPOVI_H
#include <stdbool.h>

typedef enum { NOVO, POLOVNO } Stanje;
typedef enum { KACIGA, JAKNA, HLACE, RUKAVICE } Kategorija;
typedef enum { XS, S, M, L, XL, XXL } Velicina;

typedef struct {
    int id;
    char brand[30];
    Velicina velicina;
    float cijena;
    Stanje stanje;
    Kategorija kategorija;
} MotoOprema;

typedef struct {
    char ime[50];
    char adresa[100];
    int broj;
    char email[50];
} Korisnik;

typedef struct {
    int idNarudzbe;
    int* idOpreme;
    int brojOpreme;
    int cijena;
    int idKupca;
    bool zavrsena;
} Narudzba;
#endif
