#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "list.h"

#define SIMULATION_TIME 1000 // kiek laiko veikia simuliacija
#define MAX_CLIENTS 1000
#define MAX_DIRTY 1000

typedef struct {
    int finishTime;
} Client;

typedef struct {
    int cleanTime;
} DirtyPlate;

int main() {
    Node* cleanPlates = NULL;
    Client clients[MAX_CLIENTS];
    int clientCount = 0;

    DirtyPlate dirty[MAX_DIRTY];
    int dirtyCount = 0;

    int totalPlates = 10; // pradinis kiekis, galėsi keisti
    int eatingTime = 15;  // kiek laiko klientas valgo
    int cleaningTime = 10; // kiek laiko plaunama lėkštė
    float arrivalProbability = 0.3; // tikimybė, kad ateis klientas

    int t;
    int noPlateCount = 0;
    int totalCheckpoints = 0;
    int sumCleanCount = 0;
    int minClean = totalPlates;

    srand(time(NULL));

    // Užpildyti švarias lėkštes
    for (int i = 0; i < totalPlates; i++) push(&cleanPlates, 1);

    for (t = 0; t < SIMULATION_TIME; t++) {
        // Patikriname, ar yra baigusių valgyti klientų
        for (int i = 0; i < clientCount; i++) {
            if (clients[i].finishTime == t) {
                dirty[dirtyCount].cleanTime = t + cleaningTime;
                dirtyCount++;
                // Pašalinti klientą – paprastai naudotume sąrašą, bet čia tiesiog ignoruojam
            }
        }

        // Patikriname, ar yra išplautų lėkščių
        for (int i = 0; i < dirtyCount; i++) {
            if (dirty[i].cleanTime == t) {
                push(&cleanPlates, 1);
                // Pašalinti išplautą – čia irgi galėtume perkelti paskutinį elementą
                for (int j = i; j < dirtyCount - 1; j++) {
                    dirty[j] = dirty[j + 1];
                }
                dirtyCount--;
                i--;
            }
        }

        // Tikimybė, kad ateina klientas
        float r = (float)rand() / RAND_MAX;
        if (r < arrivalProbability) {
            if (cleanPlates != NULL) {
                popFront(&cleanPlates);
                clients[clientCount].finishTime = t + eatingTime;
                clientCount++;
            } else {
                noPlateCount++;
            }
        }

        // Statistika
        int cleanNow = 0;
        Node* temp = cleanPlates;
        while (temp != NULL) {
            cleanNow++;
            temp = temp->next;
        }

        sumCleanCount += cleanNow;
        totalCheckpoints++;
        if (cleanNow < minClean) minClean = cleanNow;
    }

    printf("Simuliacija baigta\n");
    printf("Is viso lėkščių: %d\n", totalPlates);
    printf("Vidutiniškai švarių lėkščių: %.2f\n", (float)sumCleanCount / totalCheckpoints);
    printf("Mažiausias švarių kiekis: %d\n", minClean);
    printf("Kiek kartų nebuvo švarių lėkščių: %d iš %d laiko vienetų (%.2f%%)\n", noPlateCount, SIMULATION_TIME, (100.0 * noPlateCount / SIMULATION_TIME));

    freeList(&cleanPlates);
    return 0;
}
