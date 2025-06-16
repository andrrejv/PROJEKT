#define _CRT_SECURE_NO_WARNINGS

#include "MOTO_OPREMA.h"

static const char* DATOTEKA = "oprema.txt";

void dodajOpremu(void) {
    FILE* f = fopen(DATOTEKA, "a");
    if (!f) {
        perror("Greska pri otvaranju opreme datoteke");
        return;
    }

    MotoOprema oprema;
    oprema.id = generate_id();
    oprema.kategorija = odaberiKategoriju();
    oprema.velicina = odaberiVelicinu();
    oprema.stanje = odaberiStanje();

    printf("Unesite cijenu opreme: ");
    while (scanf("%f", &oprema.cijena) != 1) {
        printf("Pogrešan unos. Pokušajte ponovo: ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }

    
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    printf("Unesite brand opreme:\n");
    scanf("%s", oprema.brand);
    fprintf(f, "%d,%s,%d,%.2f,%d,%d\n",
        oprema.id,
        oprema.brand,
        oprema.velicina,   
        oprema.cijena,
        oprema.stanje,
        oprema.kategorija);
    fclose(f);
}

Velicina odaberiVelicinu(void) {
    int odabranaVelicinaKaoInt = 0;

    while (1) {
        printf("Odaberite Velicinu: \n");
        printf("1. XS\n");
        printf("2. S\n");
        printf("3. M\n");
        printf("4. L\n");
        printf("5. XL\n");
        printf("6. XXL\n");
        printf("Vas izbor: ");

        if (scanf("%d", &odabranaVelicinaKaoInt) != 1) {
            printf("Nevažeći unos! Molimo unesite broj.\n\n");
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (odabranaVelicinaKaoInt >= 1 && odabranaVelicinaKaoInt <= 6) {
            return (Velicina)(odabranaVelicinaKaoInt - 1);
        }
        else {
            printf("Nevažeća velicina! Molimo odaberite broj između 1 i 6.\n\n");
        }
    }
}

int porediBrandove(const void* a, const void* b) {
    const MotoOprema* oprema1 = (const MotoOprema*)a;
    const MotoOprema* oprema2 = (const MotoOprema*)b;

    return strcmp(oprema1->brand, oprema2->brand);
}

int porediBrandoveZaSortiranje(const void* a, const void* b) {
    const MotoOprema* oprema1 = (const MotoOprema*)a;
    const MotoOprema* oprema2 = (const MotoOprema*)b;

    return strcmp(oprema1->brand, oprema2->brand);
}

MotoOprema* pronadjiSvuOpremuPoBrendu(char* trazeniBrend, int* brojPronadjenih) {
    if (!trazeniBrend || !brojPronadjenih) {
        if (brojPronadjenih) *brojPronadjenih = 0;
        return NULL;
    }

    
    int ukupnoBrojOpreme;
    MotoOprema* lista = dohvatiOpremu(&ukupnoBrojOpreme);

    if (!lista || ukupnoBrojOpreme <= 0) {
        *brojPronadjenih = 0;
        return NULL;
    }

    
    qsort(lista, ukupnoBrojOpreme, sizeof(MotoOprema), porediBrandoveZaSortiranje);

    
    MotoOprema kljuc;
    strncpy(kljuc.brand, trazeniBrend, sizeof(kljuc.brand) - 1);
    kljuc.brand[sizeof(kljuc.brand) - 1] = '\0';

    MotoOprema* prvi = (MotoOprema*)bsearch(&kljuc, lista, ukupnoBrojOpreme,
        sizeof(MotoOprema), porediBrandove);

    if (!prvi) {
        free(lista); 
        *brojPronadjenih = 0;
        return NULL;
    }

    
    int indeksPrvog = prvi - lista;
    int pocetni = indeksPrvog;
    int krajnji = indeksPrvog;

    
    while (pocetni > 0 && strcmp(lista[pocetni - 1].brand, trazeniBrend) == 0) {
        pocetni--;
    }

    
    while (krajnji < ukupnoBrojOpreme - 1 && strcmp(lista[krajnji + 1].brand, trazeniBrend) == 0) {
        krajnji++;
    }

    
    *brojPronadjenih = krajnji - pocetni + 1;

    
    MotoOprema* rezultati = malloc(*brojPronadjenih * sizeof(MotoOprema));
    if (!rezultati) {
        free(lista);
        *brojPronadjenih = 0;
        return NULL;
    }

    
    for (int i = 0; i < *brojPronadjenih; i++) {
        rezultati[i] = lista[pocetni + i];
    }

    
    free(lista);

    return rezultati;
}

Stanje odaberiStanje(void) {
    int odabranoStanjeKaoInt = 0;

    while (1) {
        printf("Odaberite Stanje: \n");
        printf("1. Novo\n");
        printf("2. Polovno\n");
        printf("Vas izbor: ");

        if (scanf("%d", &odabranoStanjeKaoInt) != 1) {
            printf("Nevažeći unos! Molimo unesite broj.\n\n");
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (odabranoStanjeKaoInt >= 1 && odabranoStanjeKaoInt <= 2) {
            return (Stanje)(odabranoStanjeKaoInt - 1);
        }
        else {
            printf("Nevažeće stanje! Molimo odaberite broj između 1 i 2.\n\n");
        }
    }
}

Kategorija odaberiKategoriju(void) {
    int odabraniBroj = 0;

    while (1) {
        printf("Odaberite Kategoriju: \n");
        printf("1. Kaciga\n");
        printf("2. Jakna\n");
        printf("3. Hlace\n");
        printf("4. Rukavice\n");
        printf("Vas izbor: ");

        if (scanf("%d", &odabraniBroj) != 1) {
            printf("Nevažeći unos! Molimo unesite broj.\n\n");
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (odabraniBroj >= 1 && odabraniBroj <= 4) {
            return (Kategorija)(odabraniBroj - 1);
        }
        else {
            printf("Nevažeća kategorija! Molimo odaberite broj između 1 i 4.\n\n");
        }
    }
}

MotoOprema* dohvatiOpremu(int* brojOpreme) {
    FILE* f = fopen(DATOTEKA, "r");
    if (!f) {
        perror("Greška pri otvaranju datoteke");
        *brojOpreme = 0;
        return NULL;
    }

    int capacity = 10;
    int count = 0;
    MotoOprema* lista = malloc(capacity * sizeof(MotoOprema));
    if (!lista) {
        fclose(f);
        *brojOpreme = 0;
        return NULL;
    }

    while (1) {
        MotoOprema temp;
        int velicinaInt, stanjeInt, kategorijaInt;

        int res = fscanf(f, "%d,%29[^,],%d,%f,%d,%d\n",
            &temp.id,
            temp.brand,
            &velicinaInt,
            &temp.cijena,
            &stanjeInt,
            &kategorijaInt);
        if (res != 6) break;

        
        temp.velicina = (Velicina)velicinaInt;
        temp.stanje = (Stanje)stanjeInt;
        temp.kategorija = (Kategorija)kategorijaInt;

        if (count == capacity) {
            capacity *= 2;
            MotoOprema* tempList = realloc(lista, capacity * sizeof(MotoOprema));
            if (!tempList) {
                free(lista);
                fclose(f);
                *brojOpreme = 0;
                return NULL;
            }
            lista = tempList;
        }

        lista[count++] = temp;
    }

    fclose(f);

    if (count < capacity) {
        MotoOprema* tempList = realloc(lista, count * sizeof(MotoOprema));
        if (tempList) lista = tempList;
    }

    *brojOpreme = count;

    return lista;
}

int ispisiSvuOpremu(bool saIzborom) {
    int brojOpreme;
    MotoOprema* oprema = dohvatiOpremu(&brojOpreme);

    if (!oprema || brojOpreme == 0) {
        printf("Nema opreme za prikaz\n");
        return -1;
    }

    printf("\n=== SVI OPREMA ===\n");

    for (int i = 0; i < brojOpreme; i++) {
        printf("\nOprema #%d\n", i + 1);
        printf("Brand: %s (ID: %d)\n", oprema[i].brand, oprema[i].id);
        printf("Kategorija: %s\n", mapirajKategoriju(oprema[i].kategorija));
        printf("Velicina: %s\n", mapirajVelicinu(oprema[i].velicina));
        printf("Cijena: %.2f EUR\n", oprema[i].cijena);
        printf("Stanje: %s\n", mapirajStanje(oprema[i].stanje));
        printf("---\n");
    }

    printf("Ukupno stavki: %d\n", brojOpreme);

    if (!saIzborom) {
        free(oprema);
        return -1;
    }

    printf("\nIzaberite opremu (1-%d) ili 0 za izlaz: ", brojOpreme);

    int izbor;
    if (scanf("%d", &izbor) != 1) {
        printf("Neispravna opcija!\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        free(oprema);
        return -1;
    }

    if (izbor >= 1 && izbor <= brojOpreme) {
        printf("Odabrana oprema #%d\n", izbor);
        free(oprema);
        return izbor - 1;
    }
    else if (izbor == 0) {
        printf("Izlaz bez izbora.\n");
        free(oprema);
        return -1;
    }
    else {
        printf("Neispravna opcija '%d'! Molimo odaberite broj između 0 i %d.\n", izbor, brojOpreme);
        free(oprema);
        return -1;
    }
}

const char* mapirajVelicinu(Velicina velicina) {
    switch (velicina) {
    case XS:
        return "XS";
    case S:
        return "S";
    case M:
        return "M";
    case L:
        return "L";
    case XL:
        return "XL";
    case XXL:
        return "XXL";
    default:
        return "M";
    }
}

const char* mapirajStanje(Stanje stanje) {
    switch (stanje) {
    case NOVO:
        return "Novo";
    case POLOVNO:
        return "Koristeno";
    default:
        return "NaN";
    }
}

const char* mapirajKategoriju(Kategorija kategorija) {
    switch (kategorija) {
    case KACIGA:
        return "Kaciga";
    case JAKNA:
        return "Jakna";
    case HLACE:
        return "Hlace";
    case RUKAVICE:
        return "Rukavice";
    default:
        return "NaN";
    }
}
