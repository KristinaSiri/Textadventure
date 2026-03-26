// Test für die Makros der Lebensanzeige
#include <stdio.h>
#include "farben.h" // eigener Header mit Makros für die Farben
#include "menu.h" // eigener Header mit Makros für die Textformatierung der Menüs
#include "lebensanzeige.h" // eigener Header mit Makros für die Darstellung der verschiedenen Lebensanzeigen
#include <stdlib.h>

int lebensanzeige(void)
{
int Auswahl;
printf(löschen);
Lebensanzeige:
printf("1. Anzeige von 100 bis 50 Leben\n");
printf("2. Anzeige von 45 bis 0 Leben\n");
printf("3. Beenden\n");
printf("Was möchtest du tun? ");
scanf(" %d", &Auswahl);

        if(Auswahl == 1) {
                printf(löschen);
		printf(hp100);
		printf(hp95);
		printf(hp90);
		printf(hp85);
		printf(hp80);
		printf(hp75);
		printf(hp70);
 		printf(hp65);
		printf(hp60);
		printf(hp55);
		printf(hp50);
		goto Lebensanzeige;
        }

        else if (Auswahl == 2) {
                printf(löschen);
		printf(hp45);
		printf(hp40);
		printf(hp35);
		printf(hp30);
		printf(hp25);
		printf(hp20);
		printf(hp15);
		printf(hp10);
		printf(hp5);
		printf(hp0);
		goto Lebensanzeige;
	}
	else if(Auswahl == 3) {
		printf(löschen);
		exit(0);
	}

	else {
		printf(löschen);
		printf("Ungültige Eingabe!\n");
		goto Lebensanzeige;
	}
}

