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
    if (!imeTemp) {
        printf("Greska pri unosu imena.\n");
        fclose(f);
        return;
    }

    char* adresaTemp = upisiAdresu();
    if (!adresaTemp) {
        printf("Greska pri unosu adrese.\n");
        free(imeTemp);
        fclose(f);
        return;
    }

    char* emailTemp = upisiEmail();
    if (!emailTemp) {
        printf("Greska pri unosu email-a.\n");
        free(imeTemp);
        free(adresaTemp);
        fclose(f);
        return;
    }

    int broj = upisiBroj();
    if (broj == 404) {
        printf("Greska pri unosu broja.\n");
        free(imeTemp);
        free(adresaTemp);
        free(emailTemp);
        fclose(f);
        return;
    }

    
    if (strlen(imeTemp) >= sizeof(korisnik.ime) ||
        strlen(adresaTemp) >= sizeof(korisnik.adresa) ||
        strlen(emailTemp) >= sizeof(korisnik.email)) {
        printf("Jedan od unosa je predugacak.\n");
        free(imeTemp);
        free(adresaTemp);
        free(emailTemp);
        fclose(f);
        return;
    }

    
    strcpy(korisnik.ime, imeTemp);
    strcpy(korisnik.adresa, adresaTemp);
    strcpy(korisnik.email, emailTemp);
    korisnik.broj = broj;

    
    if (fprintf(f, "%s, %d, %s, %s\n", korisnik.ime, korisnik.broj, korisnik.adresa, korisnik.email) < 0) {
        printf("Greska pri pisanju u datoteku.\n");
    }
    else {
        printf("Korisnik je uspjesno dodat.\n");
    }

    
    free(imeTemp);
    free(adresaTemp);
    free(emailTemp);
    fclose(f);
}

int ispisiSveKorisnike(bool saIzborom) {
    int brojKorisnika;
    Korisnik* korisnici = dohvatiSveKorisnike(&brojKorisnika);

    if (!korisnici || brojKorisnika == 0) {
        printf("Nema korisnika za ispis.\n");
        return -1;
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

    
    if (!saIzborom) {
        free(korisnici);
        return -1;
    }

    
    printf("Izaberite korisnika (0-%d) ili %d za izlaz: ", brojKorisnika - 1, brojKorisnika);

    int izbor;
    scanf("%d", &izbor);

    if (izbor >= 0 && izbor < brojKorisnika) {
        
        free(korisnici);
        return izbor;

    }
    else if (izbor == brojKorisnika) {
        
        printf("Izlaz bez izbora.\n");
        free(korisnici);
        return -1;
    }
    else {
        
        printf("Neispravna opcija.\n");
        free(korisnici);
        return -1;
    }
}


Korisnik* dohvatiSveKorisnike(int* brojKorisnika) {
    FILE* f = fopen(KORISNIKDATOTEKA, "r");
    if (!f) {
        perror("Greska pri otvaranju datoteke");
        *brojKorisnika = 0;
        return NULL;
    }

    int kapacitet = 10;
    int broj = 0;
    Korisnik* lista = malloc(kapacitet * sizeof(Korisnik));
    if (!lista) {
        perror("Greska pri alokaciji memorije");
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
                perror("Greska pri realokaciji memorije");
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


void obrisiKorisnika(void) {
    int brojKorisnika;
    Korisnik* sviKorisnici = dohvatiSveKorisnike(&brojKorisnika);

    if (!sviKorisnici || brojKorisnika == 0) {
        printf("Nema korisnika za brisanje.\n");
        return;
    }

    int idOdabranoKorisnika = ispisiSveKorisnike(true);

   
    if (idOdabranoKorisnika < 0 || idOdabranoKorisnika >= brojKorisnika) {
        printf("Nijedan korisnik nije odabran za brisanje.\n");
        free(sviKorisnici);
        return;
    }

    
    Korisnik korisnikZaBrisanje = sviKorisnici[idOdabranoKorisnika];

    printf("Potvrda brisanja korisnika: %s\n", korisnikZaBrisanje.ime);
    printf("Da li ste sigurni? (d/n): ");

    char potvrda;
    scanf(" %c", &potvrda);

    if (potvrda != 'd' && potvrda != 'D') {
        printf("Brisanje otkazano.\n");
        free(sviKorisnici);
        return;
    }

    FILE* f = fopen(KORISNIKDATOTEKA, "w"); 
    if (!f) {
        perror("Greska pri otvaranju datoteke za pisanje");
        free(sviKorisnici);
        return;
    }

    int obrisan = 0;
    for (int i = 0; i < brojKorisnika; i++) {
        if (i == idOdabranoKorisnika) {
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
        printf("Korisnik '%s' je uspjesno obrisan.\n", korisnikZaBrisanje.ime);
    }
    else {
        printf("Greska pri brisanju korisnika.\n");
    }
}

char* upisiIme(void) {
    char buffer[1024];

    printf("Unesite ime: ");

    
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return NULL;  
    }

   
    buffer[strcspn(buffer, "\n")] = 0;

   
    char* ime = malloc(strlen(buffer) + 1);
    if (!ime) {
        return NULL;  

    strcpy(ime, buffer);
    return ime;
}

char* upisiAdresu(void) {
    char buffer[1024];

    printf("Unesite adresu: ");

   
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return NULL;  
    }

    
    buffer[strcspn(buffer, "\n")] = 0;

    
    char* adresa = malloc(strlen(buffer) + 1);
    if (!adresa) {
        return NULL;  
    }

    strcpy(adresa, buffer);
    return adresa;
}

int upisiBroj(void) {
    char buffer[1024];

    printf("Unesite broj: ");

    
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 404;  
    }

    
    buffer[strcspn(buffer, "\n")] = 0;

    char* endptr;
    int broj = strtol(buffer, &endptr, 10);

    
    if (endptr == buffer || *endptr != '\0') {
        return 404;  
    }

    return broj;
}


char* upisiEmail(void) {
    char buffer[1024];

    printf("Unesite email: ");

    
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return NULL;  
    }

    
    buffer[strcspn(buffer, "\n")] = 0;

    
    char* email = malloc(strlen(buffer) + 1);
    if (!email) {
        return NULL;  
    }

    strcpy(email, buffer);
    return email;
}
