#include <stdio.h>
#include "farben.h" // eigener Header mit Makros für die Farben
#include "menu.h" // eigener Header mit Makros für die Textformatierung der Menüs
#include "lebensanzeige.h" // eigener Header mit Makros, für die Darstellung der verschiedenen Lebensanzeigen
#include <stdlib.h>
#include <stddef.h>
#include "gamestate.h"
#include "functions.h"

void SpielUI(GameState *gs)
{
char Auswahl;
char Spielername[15];
char* hp;

printf(löschen);
Startmenu:
printf(Spielmenu);
printf(normal "\nWas möchtest du tun? ");
scanf(" %c", &Auswahl);

	if (Auswahl == '1') {
		Schwierigkeit:
		printf(löschen);
		printf(SpielSchwierigkeit);
		printf("\nIch wähle ");
		scanf(" %c", &Auswahl);

		if (Auswahl == '1') {
			hp = hp100;
			}

		else if (Auswahl == '2') {
			hp = hp75;
			}

		else if (Auswahl == '3') {
			hp = hp50;
			}

		else {
			printf(löschen);
			printf(SpielmenuFalscheEingabe);
			goto Schwierigkeit;
			}
	}

	else if (Auswahl == '2') {
		printf(löschen);
		printf(SpielBeenden);
		exit(0);
	}

	else {
		printf(löschen);
		printf(SpielmenuFalscheEingabe);
		goto Startmenu;
	}

Spielstart:
printf(löschen);
printf(SpielerName);
printf("\nMein Name ist ");
scanf(" %14s", Spielername);
printf(löschen);
printf(SpielStart, Spielername);
printf(Lebensbalken, hp);

}
