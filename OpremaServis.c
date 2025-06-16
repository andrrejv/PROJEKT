#define _CRT_SECURE_NO_WARNINGS

#include "MOTO_OPREMA.h"
#include "NARUDZBE.h"
#include "KorisnikServis.h"

static const char* DATOTEKA = "oprema.txt";

int generate_id(void) {
    return (int)time(NULL);
}

static inline void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

MotoOprema* nadjiOpremuPoID(int id) {
    FILE* f = fopen(DATOTEKA, "rb");
    if (!f) return NULL;

    MotoOprema* o = malloc(sizeof(MotoOprema));
    while (fread(o, sizeof(MotoOprema), 1, f)) {
        if (o->id == id) {
            fclose(f);
            return o;
        }
    }
    fclose(f);
    free(o);
    return NULL;
}

int compareCijena(const void* a, const void* b) {
    const MotoOprema* o1 = (const MotoOprema*)a;
    const MotoOprema* o2 = (const MotoOprema*)b;
    return (o1->cijena > o2->cijena) - (o1->cijena < o2->cijena);
}


void rekurzivniIspisOpreme(MotoOprema* niz, int indeks, int broj) {
    if (indeks >= broj) return;
    printf("%-3d %-15s %-12s %-10s €%.2f\n",
        indeks + 1,
        niz[indeks].brand,
        mapirajKategoriju(niz[indeks].kategorija),
        mapirajVelicinu(niz[indeks].velicina),
        niz[indeks].cijena);
    rekurzivniIspisOpreme(niz, indeks + 1, broj);  
}

void sortirajOpremuPoCijeni(void) {
    int broj = 0;
    MotoOprema* niz = dohvatiOpremu(&broj);

    if (!niz || broj == 0) {
        printf("Nema opreme za prikaz.\n");
        return;
    }

    qsort(niz, broj, sizeof(MotoOprema), compareCijena);

    printf("\n========== OPREMA SORTIRANA PO CIJENI ==========\n");
    printf("%-3s %-15s %-12s %-10s %-10s\n",
        "#", "BRAND", "KATEGORIJA", "VELICINA", "CIJENA");
    printf("------------------------------------------------\n");

    rekurzivniIspisOpreme(niz, 0, broj);  

    printf("================================================\n");
    printf("Ukupno: %d stavki\n", broj);

    free(niz);
}

void pretraziOpremuPoBredu() {
    char trazeniBrend[30];
    int brojPronadjenih;

    
    printf("Unesite naziv brenda koji tražite: ");
    fflush(stdout);

    if (!fgets(trazeniBrend, sizeof(trazeniBrend), stdin)) {
        printf("Greska pri ucitavanju unosa\n");
        return;
    }

    
    trazeniBrend[strcspn(trazeniBrend, "\n")] = 0;

    
    if (strlen(trazeniBrend) == 0) {
        printf("Niste unjeli naziv brenda\n");
        return;
    }

    printf("Trazim opremu brenda: '%s'...\n", trazeniBrend);

    MotoOprema* rezultati = pronadjiSvuOpremuPoBrendu(trazeniBrend, &brojPronadjenih);

    if (rezultati && brojPronadjenih > 0) {
        printf("\nPronadjeno %d stavki brenda '%s':\n", brojPronadjenih, trazeniBrend);
        printf("----------------------------------------\n");
        for (int i = 0; i < brojPronadjenih; i++) {
            printf("%d. ID=%d, Brand=%s, Cijena=%.2f\n",
                i + 1, rezultati[i].id, rezultati[i].brand, rezultati[i].cijena);
        }
        printf("----------------------------------------\n");
        free(rezultati); 
    }
    else {
        printf("Oprema brenda '%s' nije pronađena\n", trazeniBrend);
    }
}

MotoOprema* nizGlobal = NULL;
int brojGlobal = 0;

int compareID(const void* a, const void* b) {
    const MotoOprema* o1 = (const MotoOprema*)a;
    const MotoOprema* o2 = (const MotoOprema*)b;
    return o1->id - o2->id;
}

void upravljajNarduzbama(void) {
    int izbor;
    do {
        printf("\n");
        printf("---------------------------------------\n");
        printf("        UPRAVLJANJE NARUDZBAMA         \n");
        printf("---------------------------------------\n");
        printf("\n");
        printf("  1 -> Dodaj Narudzbu\n");
        printf("  2 -> Ispisi sve narudzbe\n");
        printf("  3 -> Obrisi Narudzbu\n");
        printf("\n");
        printf("  0 -> Izlaz\n");
        printf("\n");
        printf("---------------------------------------\n");
        printf("Izbor: ");

        if (scanf("%d", &izbor) != 1) {
            printf(" Neispravna opcija\n");
            clearInputBuffer();
            continue;
        }

        switch (izbor) {
        case 1:
            clearInputBuffer();
            printf("\n Dodavanje narudzbe...\n");
            dodajNarudzbu();
            printf("\n[Enter za nastavak]");
            getchar();
            break;

        case 2:
            clearInputBuffer();
            printf("\n Prikaz narudzbi...\n");
            ispisiSveNarudzbe(false);
            printf("\n[Enter za nastavak]");
            getchar();
            break;

        case 3:
            clearInputBuffer();
            printf("\n Brisanje narudzbe...\n");
            obrisiNarudzbu();
            printf("\n[Enter za nastavak]");
            getchar();
            break;

        case 0:
            printf("\nDovidjenja! \n");
            break;

        default:
            printf(" Opcija '%d' ne postoji!\n", izbor);
            printf("[Enter za nastavak]");
            clearInputBuffer();
            getchar();
            break;
        }
    } while (izbor != 0);
}


void upravljajKorisnicima(void) {
    int izbor;
    do {
        printf("\n");
        printf("---------------------------------------\n");
        printf("        UPRAVLJANJE KORISNICIMA        \n");
        printf("---------------------------------------\n");
        printf("\n");
        printf("  1 -> Dodaj Korisnika\n");
        printf("  2 -> Ispisi Korisnike\n");
        printf("  3 -> Obrisi Korisnika\n");
        printf("\n");
        printf("  0 -> Izlaz\n");
        printf("\n");
        printf("---------------------------------------\n");
        printf("Izbor: ");

        if (scanf("%d", &izbor) != 1) {
            printf(" Neispravna opcija\n");
            clearInputBuffer();
            continue;
        }

        switch (izbor) {
        case 1:
            clearInputBuffer();
            printf("\n Dodavanje korisnika...\n");
            dodajKorisnika();
            printf("\n[Enter za nastavak]");
            getchar();
            break;

        case 2:
            clearInputBuffer();
            printf("\n Prikaz korisnika...\n");
            ispisiSveKorisnike(false);
            printf("\n[Enter za nastavak]");
            getchar();
            break;

        case 3:
            clearInputBuffer();
            printf("\n Brisanje korisnika...\n");
            obrisiKorisnika();
            printf("\n[Enter za nastavak]");
            getchar();
            break;

        case 0:
            printf("\nDo vidjenja! \n");
            break;

        default:
            printf(" Opcija '%d' ne postoji!\n", izbor);
            printf("[Enter za nastavak]");
            clearInputBuffer();
            getchar();
            break;
        }
    } while (izbor != 0);
}

void upravljajOpremom(void) {
    int izbor;
    do {
        printf("\n");
        printf("---------------------------------------\n");
        printf("         UPRAVLJANJE OPREMOM          \n");
        printf("---------------------------------------\n");
        printf("\n");
        printf("  1 -> Dodaj Opremu\n");
        printf("  2 -> Ispisi Opremu\n");
        printf("  3 -> Ispisi sortiranu Opremu po cijeni\n");
        printf("  4 -> Pretrazi opremu po brendu\n");
        printf("\n");
        printf("  0 -> Izlaz\n");
        printf("\n");
        printf("---------------------------------------\n");
        printf("Izbor: ");

        if (scanf("%d", &izbor) != 1) {
            printf(" Neispravna opcija\n");
            clearInputBuffer();
            continue;
        }

        switch (izbor) {
        case 1:
            clearInputBuffer();
            printf("\n Dodavanje opreme...\n");
            dodajOpremu();
            printf("\n[Enter za nastavak]");
            getchar();
            break;

        case 2:
            clearInputBuffer();
            printf("\n Prikaz opreme...\n");
            ispisiSvuOpremu(false);
            printf("\n[Enter za nastavak]");
            getchar();
            break;

        case 3:
            clearInputBuffer();
            printf("\n Sortirana oprema po cijeni...\n");
            sortirajOpremuPoCijeni();
            printf("\n[Enter za nastavak]");
            getchar();
            break;
        case 4:
            clearInputBuffer();
            pretraziOpremuPoBredu();
            getchar();
        case 0:
            printf("\nDo vidjenja! \n");
            break;

        default:
            printf(" Opcija '%d' ne postoji!\n", izbor);
            printf("[Enter za nastavak]");
            clearInputBuffer();
            getchar();
            break;
        }
    } while (izbor != 0);
}

int main(void) {
    int izbor;
    do {
        printf("\n");
        printf("---------------------------------------\n");
        printf("         MOTO OPREMA IZBORNIK         \n");
        printf("---------------------------------------\n");
        printf("\n");
        printf("  1 -> Upravljaj opremom\n");
        printf("  2 -> Upravljaj narudzbama\n");
        printf("  3 -> Upravljaj korisnicima\n");
        printf("\n");
        printf("  0 -> Izlaz\n");
        printf("\n");
        printf("---------------------------------------\n");
        printf("Izbor: ");

        if (scanf("%d", &izbor) != 1) {
            printf(" Neispravna opcija\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        switch (izbor) {
        case 1:
            printf("\n Prelazak na upravljanje opremom...\n");
            upravljajOpremom();
            break;

        case 2:
            printf("\n Prelazak na upravljanje narudzbama...\n");
            upravljajNarduzbama();
            break;

        case 3:
            printf("\n Prelazak na upravljanje korisnicima...\n");
            upravljajKorisnicima();
            break;

        case 0:
            printf("\n  Hvala sto ste koristili Moto Oprema sistem!\n");
            printf("Do videnja! \n");
            break;

        default:
            printf(" Opcija '%d' ne postoji!\n", izbor);
            printf("Molimo odaberite opciju između 0 i 3.\n");
            printf("[Enter za nastavak]");
            getchar();
            break;
        }
    } while (izbor != 0);

    return 0;
}
