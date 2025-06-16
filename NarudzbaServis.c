#define _CRT_SECURE_NO_WARNINGS

#include "SLOZENI_TIPOVI.h"
#include <stdio.h>
#include "MOTO_OPREMA.h"
#include "NARUDZBE.h"
#include "KorisnikServis.h"


static const char* NARDUZBADATOTEKA = "narudzba.txt";


int ispisiSveNarudzbe(bool saIzborom) {
    int brojNarudzbi;
    Narudzba* narudzbe = dohatiSveNarudzbe(&brojNarudzbi);

    if (!narudzbe || brojNarudzbi == 0) {
        printf("Nema narudžbi za prikaz.\n");
        return -1;
    }

    
    for (int i = 0; i < brojNarudzbi; i++) {
        printf("Narudzba #%d\n", i);
        printf("Narudzba ID: %d\n", narudzbe[i].idNarudzbe);
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

    
    if (!saIzborom) {
        
        for (int i = 0; i < brojNarudzbi; i++) {
            free(narudzbe[i].idOpreme);
        }
        free(narudzbe);
        return -1;
    }

    
    printf("Izaberite narudzbu (0-%d) ili %d za izlaz: ", brojNarudzbi - 1, brojNarudzbi);

    int izbor;
    scanf("%d", &izbor);

    if (izbor >= 0 && izbor < brojNarudzbi) {
        
        for (int i = 0; i < brojNarudzbi; i++) {
            free(narudzbe[i].idOpreme);
        }
        free(narudzbe);
        return izbor;

    }
    else if (izbor == brojNarudzbi) {
        
        printf("Izlaz bez izbora.\n");
        
        for (int i = 0; i < brojNarudzbi; i++) {
            free(narudzbe[i].idOpreme);
        }
        free(narudzbe);
        return -1;
    }
    else {
        
        printf("Neispravna opcija.\n");
        
        for (int i = 0; i < brojNarudzbi; i++) {
            free(narudzbe[i].idOpreme);
        }
        free(narudzbe);
        return -1;
    }
}

Narudzba* dohatiSveNarudzbe(int* brojNarudzbi) {
    FILE* f = fopen(NARDUZBADATOTEKA, "rb");  
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
        Narudzba narudzba = { 0 };

        
        if (fread(&narudzba.idNarudzbe, sizeof(int), 1, f) != 1 ||
            fread(&narudzba.brojOpreme, sizeof(int), 1, f) != 1 ||
            fread(&narudzba.cijena, sizeof(int), 1, f) != 1 ||
            fread(&narudzba.idKupca, sizeof(int), 1, f) != 1 ||
            fread(&narudzba.zavrsena, sizeof(bool), 1, f) != 1) {

            
            if (feof(f)) {
                break;  
            }
            else {
                
                fprintf(stderr, "Greska pri citanju datoteke.\n");
                free(lista);
                fclose(f);
                *brojNarudzbi = 0;
                return NULL;
            }
        }

        
        if (narudzba.brojOpreme < 0 || narudzba.brojOpreme > 1000) {
            fprintf(stderr, "Neispravna vrijednost brojOpreme: %d\n", narudzba.brojOpreme);
            free(lista);
            fclose(f);
            *brojNarudzbi = 0;
            return NULL;
        }

        
        if (narudzba.brojOpreme > 0) {
            narudzba.idOpreme = malloc(narudzba.brojOpreme * sizeof(int));
            if (!narudzba.idOpreme) {
                fprintf(stderr, "Greska pri alokaciji memorije za idOpreme.\n");
                
                for (int i = 0; i < count; i++) {
                    free(lista[i].idOpreme);
                }
                free(lista);
                fclose(f);
                *brojNarudzbi = 0;
                return NULL;
            }

            
            if (fread(narudzba.idOpreme, sizeof(int), narudzba.brojOpreme, f) != (size_t)narudzba.brojOpreme) {
                fprintf(stderr, "Greska pri citanju idOpreme niza.\n");
                free(narudzba.idOpreme);
                
                for (int i = 0; i < count; i++) {
                    free(lista[i].idOpreme);
                }
                free(lista);
                fclose(f);
                *brojNarudzbi = 0;
                return NULL;
            }
        }
        else {
            narudzba.idOpreme = NULL;
        }

        
        if (count == capacity) {
            capacity *= 2;
            Narudzba* tempList = realloc(lista, capacity * sizeof(Narudzba));
            if (!tempList) {
                
                free(narudzba.idOpreme);
                for (int i = 0; i < count; i++) {
                    free(lista[i].idOpreme);
                }
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

    
    if (count == 0) {
        free(lista);
        *brojNarudzbi = 0;
        return NULL;
    }

    
    if (count < capacity / 2) {
        Narudzba* tempList = realloc(lista, count * sizeof(Narudzba));
        if (tempList) {
            lista = tempList;
        }
    }

    *brojNarudzbi = count;
    return lista;
}

void dodajNarudzbu(void) {
    FILE* f = fopen(NARDUZBADATOTEKA, "ab");
    if (!f) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    Narudzba narudzba = { 0 };
    narudzba.zavrsena = false;
    narudzba.idOpreme = NULL;
    narudzba.brojOpreme = 0;
    narudzba.cijena = 0;

    int brojOpreme;
    MotoOprema* sveOpreme = dohvatiOpremu(&brojOpreme);
    if (!sveOpreme) {
        fprintf(stderr, "Greska pri dohvaćanju opreme.\n");
        fclose(f);
        return;
    }

    int izbor = 0;
    do {
        int indexOpreme = ispisiSvuOpremu(true);
        MotoOprema odabrana = sveOpreme[indexOpreme];
        if (odabrana.id == -1) break; 

        
        bool postoji = false;
        for (int i = 0; i < brojOpreme; i++) {
            if (sveOpreme[i].id == odabrana.id) {
                postoji = true;
                break;
            }
        }
        if (!postoji) {
            printf("Odabrana oprema nije pronadjena.\n");
            izbor = 1; 
            continue;
        }

        
        bool duplikat = false;
        for (int i = 0; i < narudzba.brojOpreme; i++) {
            if (narudzba.idOpreme[i] == odabrana.id) {
                printf("Ova oprema je već dodana u narudzbu.\n");
                duplikat = true;
                break;
            }
        }
        if (duplikat) {
            printf("Zelite li dodati jos jednu opremu? (1 = da, 0 = ne): ");
            if (scanf("%d", &izbor) != 1) {
                fprintf(stderr, "Neispravan unos.\n");
                
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                goto cleanup;
            }
            continue;
        }

       
        int* novaLista = realloc(narudzba.idOpreme, (narudzba.brojOpreme + 1) * sizeof(int));
        if (!novaLista) {
            perror("Neuspjesna alokacija memorije");
            goto cleanup;
        }

        narudzba.idOpreme = novaLista;
        narudzba.idOpreme[narudzba.brojOpreme] = odabrana.id;
        narudzba.brojOpreme++;
        narudzba.cijena += (int)odabrana.cijena;

        printf("Dodano. Zelite li dodati jos jednu opremu? (1 = da, 0 = ne): ");
        if (scanf("%d", &izbor) != 1) {
            fprintf(stderr, "Neispravan unos.\n");
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            goto cleanup;
        }

    } while (izbor == 1);

    
    if (narudzba.brojOpreme == 0) {
        printf("Narudzba mora imati barem jednu opremu.\n");
        goto cleanup;
    }

    narudzba.idNarudzbe = generate_id();


    int brojKorisnika;
    Korisnik* sviKorisnici = dohvatiSveKorisnike(&brojKorisnika);

    int indexKorisnika = ispisiSveKorisnike(true);

    if (indexKorisnika == -1) {
        fprintf(stderr, "Greska pri dohvacanju korisnika.\n");
        goto cleanup;
    }

    narudzba.idKupca = sviKorisnici[indexKorisnika].broj;

    
    if (fwrite(&narudzba.idNarudzbe, sizeof(int), 1, f) != 1 ||
        fwrite(&narudzba.brojOpreme, sizeof(int), 1, f) != 1 ||
        fwrite(&narudzba.cijena, sizeof(int), 1, f) != 1 ||
        fwrite(&narudzba.idKupca, sizeof(int), 1, f) != 1 ||
        fwrite(&narudzba.zavrsena, sizeof(bool), 1, f) != 1 ||
        fwrite(narudzba.idOpreme, sizeof(int), narudzba.brojOpreme, f) != (size_t)narudzba.brojOpreme) {
        perror("Greska pri pisanju u datoteku");
        free(sviKorisnici);
        goto cleanup;
    }

    printf("Narudzba uspjesno spremljena (ID: %d).\n", narudzba.idNarudzbe);
    free(sviKorisnici);
    fclose(f);
    free(sveOpreme);
    free(narudzba.idOpreme);
    return;

cleanup:
    if (f) fclose(f);
    if (sveOpreme) free(sveOpreme);
    if (narudzba.idOpreme) free(narudzba.idOpreme);
}

void obrisiNarudzbu(void) {
    int brojNarudzbi;
    Narudzba* sveNarudzbe = dohatiSveNarudzbe(&brojNarudzbi);

    if (!sveNarudzbe || brojNarudzbi == 0) {
        printf("Nema narudzbi za brisanje.\n");
        return;
    }

    int indexNarudzbe = ispisiSveNarudzbe(true);

    if (indexNarudzbe < 0 || indexNarudzbe >= brojNarudzbi) {
        printf("Nijedna narudzba nije odabrana za brisanje.\n");
        
        for (int i = 0; i < brojNarudzbi; i++) {
            free(sveNarudzbe[i].idOpreme);
        }
        free(sveNarudzbe);
        return;
    }

    Narudzba narudzbaZaBrisanje = sveNarudzbe[indexNarudzbe];

    printf("Potvrda brisanja narudzbe: %d\n", narudzbaZaBrisanje.idNarudzbe);
    printf("Da li ste sigurni? (d/n): ");

    char potvrda;
    if (scanf(" %c", &potvrda) != 1) {
        printf("Greska pri citanju unosa.\n");
       
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        
        for (int i = 0; i < brojNarudzbi; i++) {
            free(sveNarudzbe[i].idOpreme);
        }
        free(sveNarudzbe);
        return;
    }

    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (potvrda != 'd' && potvrda != 'D') {
        printf("Brisanje otkazano.\n");
        
        for (int i = 0; i < brojNarudzbi; i++) {
            free(sveNarudzbe[i].idOpreme);
        }
        free(sveNarudzbe);
        return;
    }

    FILE* f = fopen(NARDUZBADATOTEKA, "wb"); 
    if (!f) {
        perror("Greska pri otvaranju datoteke za pisanje");
        
        for (int i = 0; i < brojNarudzbi; i++) {
            free(sveNarudzbe[i].idOpreme);
        }
        free(sveNarudzbe);
        return;
    }

    bool writeSuccess = true;
    int writtenCount = 0;

    for (int i = 0; i < brojNarudzbi; i++) {
        if (i == indexNarudzbe) {
            continue; 
        }

        
        if (sveNarudzbe[i].brojOpreme < 0 ||
            (sveNarudzbe[i].brojOpreme > 0 && !sveNarudzbe[i].idOpreme)) {
            printf("Greska: neispravni podaci u narudzbi %d\n", sveNarudzbe[i].idNarudzbe);
            writeSuccess = false;
            break;
        }

        
        if (fwrite(&sveNarudzbe[i].idNarudzbe, sizeof(int), 1, f) != 1 ||
            fwrite(&sveNarudzbe[i].brojOpreme, sizeof(int), 1, f) != 1 ||
            fwrite(&sveNarudzbe[i].cijena, sizeof(int), 1, f) != 1 ||
            fwrite(&sveNarudzbe[i].idKupca, sizeof(int), 1, f) != 1 ||
            fwrite(&sveNarudzbe[i].zavrsena, sizeof(bool), 1, f) != 1) {
            perror("Greska pri pisanju osnovnih podataka u datoteku");
            writeSuccess = false;
            break;
        }

        
        if (sveNarudzbe[i].brojOpreme > 0) {
            if (fwrite(sveNarudzbe[i].idOpreme, sizeof(int), sveNarudzbe[i].brojOpreme, f) !=
                (size_t)sveNarudzbe[i].brojOpreme) {
                perror("Greska pri pisanju idOpreme niza u datoteku");
                writeSuccess = false;
                break;
            }
        }

        writtenCount++;
    }

    fclose(f);

    
    for (int i = 0; i < brojNarudzbi; i++) {
        free(sveNarudzbe[i].idOpreme);
    }
    free(sveNarudzbe);

    if (writeSuccess && writtenCount == (brojNarudzbi - 1)) {
        printf("Narudzba ID %d je uspješno obrisana.\n", narudzbaZaBrisanje.idNarudzbe);
    }
    else {
        printf("Greska pri brisanju narudzbe. Datoteka mozda nije ispravno azurirana.\n");
       
    }
}


int odaberiKorisnika(Korisnik* sviKorisnici, int brojKorisnika) {
    int izbor = -1;
    printf("Odaberite korisnika\n");
    ispisiSveKorisnike(true);

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
