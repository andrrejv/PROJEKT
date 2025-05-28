#define _CRT_SECURE_NO_WARNINGS

#include "SLOZENI_TIPOVI.h"
#include <stdio.h>
#include "MOTO_OPREMA.h"
#include "NARUDZBE.h"
#include "KorisnikServis.h"


static const char* NARDUZBADATOTEKA = "narudzba.txt";


void ispisiSveNarudzbe(void) {
    int brojNarudzbi;
    Narudzba* narudzbe = dohatiSveNarudzbe(&brojNarudzbi);
    if (!narudzbe) {
        printf("Nema narudžbi za prikaz.\n");
        return;
    }

    for (int i = 0; i < brojNarudzbi; i++) {
        printf("Narudžba ID: %d\n", narudzbe[i].idNarudzbe);
        printf("Kupac ID: %d\n", narudzbe[i].idKupca);
        printf("Broj opreme: %d\n", narudzbe[i].brojOpreme);
        printf("ID-jevi opreme: ");
        for (int j = 0; j < narudzbe[i].brojOpreme; j++) {
            printf("%d ", narudzbe[i].idOpreme[j]);
        }
        printf("\n");
        printf("Ukupna cijena: %d\n", narudzbe[i].cijena);
        printf("Status: %s\n", narudzbe[i].zavrsena ? "Završena" : "U tijeku");
        printf("------------------------------------------\n");
    }

    
    for (int i = 0; i < brojNarudzbi; i++) {
        free(narudzbe[i].idOpreme);
    }
    free(narudzbe);
}


Narudzba* dohatiSveNarudzbe(int* brojNarudzbi) {
    FILE* f = fopen(NARDUZBADATOTEKA, "r");
    if (!f) {
        perror("Greška pri otvaranju datoteke");
        *brojNarudzbi = 0;
        return NULL;
    }

    int capacity = 10;
    int count = 0;
    Narudzba* lista = malloc(capacity * sizeof(Narudzba));
    if (!lista) {
        fclose(f);
        *brojNarudzbi = 0;
        return NULL;
    }

    while (1) {
        Narudzba narudzba;
        int res = fscanf(f, "%d, %d, %d, %d, %d, ",
            &narudzba.idNarudzbe,
            narudzba.idOpreme,
            &narudzba.brojOpreme,
            &narudzba.cijena,
            &narudzba.idKupca);
        if (res != 4) break;  

        if (count == capacity) {
            capacity *= 2;  
            Narudzba* tempList = realloc(lista, capacity * sizeof(Narudzba));
            if (!tempList) {
                free(lista);
                fclose(f);
                *brojNarudzbi = 0;
                return NULL;  
            }
            lista = tempList;
        }

        lista[count++] = narudzba;
    }

    fclose(f);

    if (count < capacity) {
        Narudzba* tempList = realloc(lista, count * sizeof(Narudzba));
        if (tempList) lista = tempList;
    }

    *brojNarudzbi = count;

    return lista;
}

void dodajNarudzbu(void) {
    FILE* f = fopen(NARDUZBADATOTEKA, "ab"); 
    if (!f) {
        perror("Greška pri otvaranju datoteke");
        return;
    }

    Narudzba narudzba = { 0 }; 
    narudzba.zavrsena = false;

    int brojOpreme;
    MotoOprema* sveOpreme = dohvatiOpremu(&brojOpreme); 

    narudzba.idOpreme = NULL;  
    narudzba.brojOpreme = 0;
    narudzba.cijena = 0;

    int izbor = 0;

    do {
        MotoOprema odabrana = odaberiOpremu(sveOpreme, brojOpreme);
        if (odabrana.id == -1) break; 

        narudzba.brojOpreme++;
        narudzba.cijena += odabrana.cijena;
        narudzba.idOpreme = realloc(narudzba.idOpreme, narudzba.brojOpreme * sizeof(int));
        if (!narudzba.idOpreme) {
            perror("Neuspješna alokacija memorije");
            fclose(f);
            free(sveOpreme);
            return;
        }

        narudzba.idOpreme[narudzba.brojOpreme - 1] = odabrana.id;
        narudzba.cijena += (int)odabrana.cijena;

        printf("Dodano. Želite li dodati još jednu opremu? (1 = da, 0 = ne): ");
        scanf("%d", &izbor);

    } while (izbor == 1);

    narudzba.idNarudzbe = generate_id();
    int brojKorisnika;
    Korisnik* sviKorisnici = dohvatiSveKorisnike(&brojKorisnika);
    int indexIzabranogKorisnika = odaberiKorisnika(sviKorisnici, brojKorisnika);
    if (indexIzabranogKorisnika == -1) return;
    Korisnik odabraniKorisnik = sviKorisnici[indexIzabranogKorisnika];
    narudzba.idKupca = odabraniKorisnik.broj;

   
    fwrite(&narudzba.idNarudzbe, sizeof(int), 1, f);
    fwrite(&narudzba.brojOpreme, sizeof(int), 1, f);
    fwrite(&narudzba.cijena, sizeof(int), 1, f);
    fwrite(&narudzba.idKupca, sizeof(int), 1, f);
    fwrite(&narudzba.zavrsena, sizeof(bool), 1, f);
    fwrite(narudzba.idOpreme, sizeof(int), narudzba.brojOpreme, f);

    fclose(f);

    printf("Narudžba uspješno spremljena.\n");

    
    free(narudzba.idOpreme);
    free(sveOpreme);
}



MotoOprema odaberiOpremu(MotoOprema* oprema, int brojOpreme) {
    int izbor = -1;

    ispisiSvuOpremu();
    printf("-1 Ništa\nIzaberite ID: ");
    scanf("%d", &izbor);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    if (izbor < 0 || izbor >= brojOpreme) {
        MotoOprema nista = { .id = -1 };
        return nista;
    }

    return oprema[izbor];
}

int odaberiKorisnika(Korisnik* sviKorisnici, int brojKorisnika) {
    int izbor = -1;
    printf("-1 Ništa. \n");
    printf("-2 Napravi novog korisnika.\n");
    printf("Odaberite korisnika\n");
    ispisiSveKorisnike();

    scanf("%d", &izbor);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  

    if (izbor == -1) {
        return -1;
    }
    else if (izbor == -2) {
        dodajKorisnika();
        return -2; 
    }
    else {
        return izbor;
    }
}


void ispisiNarudzbeRekurzivno(Narudzba* glava) {

}