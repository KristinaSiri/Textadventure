#include <stdio.h>
#include "farben.h" // eigener Header mit Makros für die Farben

int farbentest(void)
{
	printf("\n\n\n");
	printf("Alle Textfarben:\n");
	printf(schwarz " Schwarz" grau " Grau" hellgrau " Hellgrau" weiß " Weiß" rot " Rot" hellrot " Hellrot" grün " Grün"
               hellgrün " Hellgrün" "\n" gelb " Gelb" hellgelb " Hellgelb" blau " Blau" hellblau " Hellblau"  lila " Lila"
               pink " Pink" cyan " Cyan" hellcyan " Hellcyan\n\n\n" normal);

	printf("Alle Hintergrundfarben:\n");
	printf(schwarzH "  Schwarz  " grauH schwarz "  Grau  " hellgrauH "  Hellgrau  " weißH "  Weiß  " rotH "  Rot  " hellrotH "  Hellrot  " grünH "  Grün  "
               hellgrünH "  Hellgrün  " normal "\n" gelbH schwarz "  Gelb  " hellgelbH "  Hellgelb  " blauH "  Blau  " hellblauH "  Hellblau  " lilaH "  Lila  "
               pinkH "  Pink  " cyanH "  Cyan  " hellcyanH "  Hellcyan   " normal "\n\n\n");


	printf("Beispiele:\n");
	printf(unterstrichen "Text der unterstrichen ist\n" normal);
	printf(blinken "Text der blinkt\n" normal);
	printf(durchgestrichen "Text der durchgestrichen ist \n" normal);
	printf(kursiv "Text der kursiv geschrieben ist\n" normal);
	return (0);
}


