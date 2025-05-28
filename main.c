#define _CRT_SECURE_NO_WARNINGS

#include "MOTO_OPREMA.h"
#include "NARUDZBE.h"
#include "KorisnikServis.h"

static const char* DATOTEKA = "oprema.txt";

int generate_id(void) {
    return (int)time(NULL);  
}

MotoOprema* nadjiOpremuPoID(int id) {
    FILE* f = fopen(DATOTEKA, "rb");
    if (!f) return NULL;

    MotoOprema* o = malloc(sizeof(MotoOprema));
    while (fread(&o, sizeof(MotoOprema), 1, f)) {
        if (o->id == id) {
            fclose(f);
            return o;
        }
    }
    fclose(f);
    return NULL;
}


int compareCijena(const void* a, const void* b) {
    const MotoOprema* o1 = (const MotoOprema*)a;
    const MotoOprema* o2 = (const MotoOprema*)b;
    return (o1->cijena > o2->cijena) - (o1->cijena < o2->cijena);
}

void sortirajOpremuPoCijeni(void) {
    FILE* f = fopen(DATOTEKA, "rb");
    if (!f) return;

    fseek(f, 0, SEEK_END);
    long velicina = ftell(f);
    rewind(f);
    int broj = velicina / sizeof(MotoOprema);

    MotoOprema* niz = malloc(velicina);
    fread(niz, sizeof(MotoOprema), broj, f);
    fclose(f);

    qsort(niz, broj, sizeof(MotoOprema), compareCijena);

    for (int i = 0; i < broj; i++) {
        printf("[SORT] %s %.2f\n", niz[i].brand, niz[i].cijena);
    }

    free(niz);
}

MotoOprema* nizGlobal = NULL;
int brojGlobal = 0;

int compareID(const void* a, const void* b) {
    const MotoOprema* o1 = (const MotoOprema*)a;
    const MotoOprema* o2 = (const MotoOprema*)b;
    return o1->id - o2->id;
}


void pretraziOpremuPoID(void) {
    FILE* f = fopen(DATOTEKA, "rb");
    if (!f) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    fseek(f, 0, SEEK_END);
    long velicina = ftell(f);
    rewind(f);
    brojGlobal = velicina / sizeof(MotoOprema);

    nizGlobal = malloc(velicina);
    if (!nizGlobal) {
        perror("Greska pri alokaciji memorije");
        fclose(f);
        return;
    }

    fread(nizGlobal, sizeof(MotoOprema), brojGlobal, f);
    fclose(f);

    qsort(nizGlobal, brojGlobal, sizeof(MotoOprema), compareID);

    int trazeniID;
    printf("Unesite ID za pretragu: ");
    scanf("%d", &trazeniID);

    MotoOprema kljuc;
    kljuc.id = trazeniID;

    MotoOprema* rez = bsearch(&kljuc, nizGlobal, brojGlobal, sizeof(MotoOprema), compareID);

    if (rez) {
        printf("Pronadena oprema: %s %.2f EUR [%s]\n",
            rez->brand,
            rez->cijena,
            rez->stanje == NOVO ? "NOVO" : "POLOVNO");
    }
    else {
        printf("Nije pronadena oprema s ID-om %d.\n", trazeniID);
    }

    free(nizGlobal);
}

void upravljajNarduzbama(void) {
    int izbor;
    do {
        printf("\n===== Moto Oprema Izbornik =====\n");
        printf("1. Dodaj Narudzbu\n");
        printf("2. Ispisi sve narudzbe\n");
        printf("3. Obrisi Narudzbu\n");
        printf("4. Uredi Narudzbu\n");
        printf("5. Ispisi Korisnike\n");
        printf("0. Izlaz\n");
        scanf("%d", &izbor);
        switch (izbor) {
        case 1:

            while ((izbor = getchar()) != '\n' && izbor != EOF);
            dodajNarudzbu();
            break;
        case 2:
            while ((izbor = getchar()) != '\n' && izbor != EOF);
            ispisiSveNarudzbe();
            break;
        case 3:
            break;
        case 4:
            break;
        case 0:
            break;

        default:
            break;
        }
    } while (izbor != 0);
}

void upravljajKorisnicima(void) {
    int izbor;
    do {
        printf("\n===== UPRAVLJANJE KORISNIIMA =====\n");
        printf("1. Dodaj Korisnika\n");
        printf("2. Ispisi sve narudzbe\n");
        printf("3. Obrisi Narudzbu\n");
        printf("4. Uredi Narudzbu\n");
        printf("5. Ispisi Korisnike\n");
        printf("0. Izlaz\n");
        scanf("%d", &izbor);
        switch (izbor) {
        case 1:

            while ((izbor = getchar()) != '\n' && izbor != EOF);
            dodajNarudzbu();
            break;
        case 2:
            while ((izbor = getchar()) != '\n' && izbor != EOF);
            ispisiSveNarudzbe();
            break;
        case 3:
            break;
        case 4:
            break;
        case 0:
            break;

        default:
            break;
        }
    } while (izbor != 0);

}

int main(void) {
    int izbor;
    do {
        printf("\n===== Moto Oprema Izbornik =====\n");
        printf("1. Dodaj opremu\n");
        printf("2. Ispisi opremu\n");
        printf("3. Upravljaj nardzbama\n");
        printf("4. Upravljaj korisnicima");
        printf("0. Izlaz\n");
        printf("Unesite izbor: ");
        scanf("%d", &izbor);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        switch (izbor) {
        case 1:
            dodajOpremu(); break;
        case 2:
            ispisiSvuOpremu(); break;
        case 3:
            upravljajNarduzbama(); break;
        case 4:
            upravljajKorisnicima(); break;

        default:
            printf("Neispravan unos!\n");
        }
    } while (izbor != 0);
    return 0;
}