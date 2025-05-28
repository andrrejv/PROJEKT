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
    printf("Odaberite Velicin: \n");
    printf("1. XS\n");
    printf("2. S\n");
    printf("3. M\n");
    printf("4. L\n");
    printf("5. XL\n");
    printf("6. XXL\n");

    int odabranaVelicinaKaoInt = 0;
    scanf("%d", &odabranaVelicinaKaoInt);
    if (odabranaVelicinaKaoInt < 0 || odabranaVelicinaKaoInt > 5) {
        printf("Nevažeća velicina odabrana \n");
        return M;
    }

    return (Velicina)odabranaVelicinaKaoInt;
}

Stanje odaberiStanje(void) {
    printf("Odaberite Stanje: \n");
    printf("1. Novo\n");
    printf("2. Polovno\n");

    int odabranoStanjeKaoInt = 0;
    scanf("%d", &odabranoStanjeKaoInt);
    if (odabranoStanjeKaoInt < 0 || odabranoStanjeKaoInt > 5) {
        printf("Nevažeća stanje odabrana \n");
        return NOVO;
    }

    return (Stanje)odabranoStanjeKaoInt;
}

Kategorija odaberiKategoriju(void) {
    printf("Odaberite Kategoriju: \n");
    printf("1. Kaciga\n");
    printf("2. Jakna\n");
    printf("3. Hlace\n");
    printf("4. Rukavice\n");

    int odabraniBroj = 0;
    scanf("%d", &odabraniBroj);
    odabraniBroj--;
    if (odabraniBroj < 0 || odabraniBroj > 3) {
        printf("Nevažeća kategorija odabrana \n");
        return KACIGA;
    }
    return (Kategorija)odabraniBroj;
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

void ispisiSvuOpremu(void) {
    int brojOpreme;
    MotoOprema* oprema = dohvatiOpremu(&brojOpreme);
    for (int i = 0; i < brojOpreme; i++) {
        printf("ID: ");
        printf("%d\n", oprema[i].id);
        printf("Velicina: ");
        printf("%s\n", mapirajVelicinu(oprema[i].velicina));
        printf("Cijena: ");
        printf("%f\n", oprema[i].cijena);
        printf("Stanje:");
        printf("%s\n", mapirajStanje(oprema[i].stanje));
        printf("Kategorija: ");
        printf("%s\n", mapirajKategoriju(oprema[i].kategorija));
        printf("Brand: ");
        printf("%s\n", oprema[i].brand);
        printf("-------------------------------\n");
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
        return "Novo";
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