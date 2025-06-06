#define _CRT_SECURE_NO_WARNINGS

#include "SLOZENI_TIPOVI.h"
#include "KorisnikServis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* KORISNIKDATOTEKA = "korisnik.txt";

void dodajKorisnika(void) {
    FILE* f = fopen(KORISNIKDATOTEKA, "a");
    if (!f) {
        perror("Greska pri otvaranju korisnik datoteke");
        return;
    }

    Korisnik korisnik;

   
    char* imeTemp = upisiIme();
    char* adresaTemp = upisiAdresu();
    char* emailTemp = upisiEmail();

    
    strcpy(korisnik.ime, imeTemp);
    strcpy(korisnik.adresa, adresaTemp);
    strcpy(korisnik.email, emailTemp);

    korisnik.broj = upisiBroj();

    
    fprintf(f, "%s, %d, %s, %s\n", korisnik.ime, korisnik.broj, korisnik.adresa, korisnik.email);

    
    free(imeTemp);
    free(adresaTemp);
    free(emailTemp);

    fclose(f);
}

void ispisiSveKorisnike(void) {
    int brojKorisnika = 0;
    Korisnik* korisnici = dohvatiSveKorisnike(&brojKorisnika);

    if (!korisnici || brojKorisnika == 0) {
        printf("Nema korisnika za ispis.\n");
        return;
    }

    for (int i = 0; i < brojKorisnika; i++) {
        Korisnik trenutniKorisnik = korisnici[i];
        printf("Korisnik #%d\n", i);
        printf("Ime: %s\n", trenutniKorisnik.ime);
        printf("Adresa: %s\n", trenutniKorisnik.adresa);
        printf("Email: %s\n", trenutniKorisnik.email);
        printf("Broj: %d\n", trenutniKorisnik.broj);
        printf("-----------------------------\n");
    }

    free(korisnici);
}


Korisnik* dohvatiSveKorisnike(int* brojKorisnika) {
    FILE* f = fopen(KORISNIKDATOTEKA, "r");
    if (!f) {
        perror("Greška pri otvaranju datoteke");
        *brojKorisnika = 0;
        return NULL;
    }

    int kapacitet = 10;
    int broj = 0;
    Korisnik* lista = malloc(kapacitet * sizeof(Korisnik));
    if (!lista) {
        perror("Greška pri alokaciji memorije");
        fclose(f);
        *brojKorisnika = 0;
        return NULL;
    }

    while (1) {
        Korisnik korisnik;
        int res = fscanf(f, " %49[^,], %d, %99[^,], %49[^\n]",
            korisnik.ime,
            &korisnik.broj,
            korisnik.adresa,
            korisnik.email);

        if (res != 4) break;  

        if (broj == kapacitet) {
            kapacitet *= 2;
            Korisnik* temp = realloc(lista, kapacitet * sizeof(Korisnik));
            if (!temp) {
                perror("Greška pri realokaciji memorije");
                free(lista);
                fclose(f);
                *brojKorisnika = 0;
                return NULL;
            }
            lista = temp;
        }

        lista[broj++] = korisnik;
    }

    fclose(f);

    
    if (broj == 0) {
        free(lista);
        *brojKorisnika = 0;
        return NULL;
    }

   
    if (broj < kapacitet) {
        Korisnik* temp = realloc(lista, broj * sizeof(Korisnik));
        if (temp) lista = temp;
    }

    *brojKorisnika = broj;
    return lista;
}


void obrisiKorisnika(Korisnik korisnikZaBrisanje) {
    int brojKorisnika;
    Korisnik* sviKorisnici = dohvatiSveKorisnike(&brojKorisnika);
    if (!sviKorisnici || brojKorisnika == 0) {
        printf("Nema korisnika za brisanje.\n");
        return;
    }

    FILE* f = fopen(KORISNIKDATOTEKA, "w"); 
    if (!f) {
        perror("Greška pri otvaranju datoteke za pisanje");
        free(sviKorisnici);
        return;
    }

    int obrisan = 0;
    for (int i = 0; i < brojKorisnika; i++) {
        if (
            strcmp(sviKorisnici[i].ime, korisnikZaBrisanje.ime) == 0 &&
            sviKorisnici[i].broj == korisnikZaBrisanje.broj &&
            strcmp(sviKorisnici[i].adresa, korisnikZaBrisanje.adresa) == 0 &&
            strcmp(sviKorisnici[i].email, korisnikZaBrisanje.email) == 0
            ) {
            obrisan = 1; 
            continue;
        }

        fprintf(f, "%s, %d, %s, %s\n",
            sviKorisnici[i].ime,
            sviKorisnici[i].broj,
            sviKorisnici[i].adresa,
            sviKorisnici[i].email);
    }

    fclose(f);
    free(sviKorisnici);

    if (obrisan) {
        printf("Korisnik je uspješno obrisan.\n");
    }
    else {
        printf("Korisnik nije pronađen.\n");
    }
}



char* dohvatiString(char* ispisnaVrijednost) {
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

char* upisiIme(void) {
    char buffer[1024];
    printf("Unesite ime: ");
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

char* upisiAdresu(void) {
    char buffer[1024];
    printf("Unesite adresu: ");
    fgets(buffer, sizeof(buffer), stdin);

    buffer[strcspn(buffer, "\n")] = 0;

    char* adresa = malloc(strlen(buffer) + 1);
    if (!adresa) {
        perror("Greška pri alokaciji memorije");
        exit(1);
    }
    strcpy(adresa, buffer);
    return adresa;
}

int upisiBroj(void) {
    char buffer[1024];
    printf("Unesite broj: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        perror("Greška pri čitanju unosa");
        exit(1);
    }

    buffer[strcspn(buffer, "\n")] = 0;

    char* endptr;
    int broj = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\0') {
        fprintf(stderr, "Neispravan broj.\n");
        exit(1);
    }

    return broj;
}


char* upisiEmail(void) {
    char buffer[1024];
    printf("Unesite email: ");
    fgets(buffer, sizeof(buffer), stdin);

    buffer[strcspn(buffer, "\n")] = 0;

    char* email = malloc(strlen(buffer) + 1);
    if (!email) {
        perror("Greška pri alokaciji memorije");
        exit(1);
    }
    strcpy(email, buffer);
    return email;
}
