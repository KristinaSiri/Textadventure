#include "farben.h"

#define Lebensbalken "%s\n"
#define Spielname weißH "                                    " blau kursiv unterstrichen "Textadventure" normal weißH "                                     " normal "\n"
#define Option1 weißH "                                     " schwarz "1. Spielen                                       " normal "\n"
#define Option2 weißH "                                     " schwarz "2. Beenden                                       " normal "\n"
#define SpielBeendenText weißH "                            " schwarz "Das Spiel wurde beendet.                                  " normal "\n"
#define SpielSchwierigkeitText weißH "                            " schwarz "Wähle eine Schwierigkeit                                  " normal "\n"
#define SpielSchwierigkeitOption1 weißH "                               " schwarz "  1. Leicht                                            " normal "\n"
#define SpielSchwierigkeitOption2 weißH "                               " schwarz "  2. Mittel                                            " normal "\n"
#define SpielSchwierigkeitOption3 weißH   "                               " schwarz "  3. Schwer                                            " normal "\n"
#define SpielmenuFalscheEingabeText weißH rot "                Du musst eine Option mit den Zahlen 1 oder 2 auswählen!               " normal "\n"
#define SpielerNameText weißH schwarz "                              Wie lautet dein Name?                                   " normal "\n"
#define SpielStartText1 weißH schwarz "                                 Willkommen, %-14s                           " normal "\n"
#define SpielStartText2 weißH schwarz "                           Dein Abenteuer beginnt jetzt...                            " normal "\n"

// Höhe:30 Breite:86
#define leerezeile weißH "                                                                                      " normal "\n"
#define leerezeile2 leerezeile leerezeile
#define leerezeile3 leerezeile2 leerezeile
#define leerezeile4 leerezeile2 leerezeile2
#define leerezeile5 leerezeile4 leerezeile
#define leerezeile10 leerezeile5 leerezeile5
#define leerezeile15 leerezeile10 leerezeile5
#define leerezeile20 leerezeile10 leerezeeile10
#define leerezeile25 leerezeile20 leerezeile5
#define leerezeile30 leerezeile25 leerezeile5

#define Spielmenu leerezeile5 Spielname leerezeile5 Option1 leerezeile2 Option2 leerezeile15
#define SpielBeenden leerezeile10 leerezeile4 SpielBeendenText leerezeile15
#define SpielSchwierigkeit leerezeile5 leerezeile3 SpielSchwierigkeitText leerezeile4 SpielSchwierigkeitOption1 leerezeile2 SpielSchwierigkeitOption2 leerezeile2 \
SpielSchwierigkeitOption3 leerezeile10
#define SpielmenuFalscheEingabe leerezeile SpielmenuFalscheEingabeText leerezeile
#define SpielerName leerezeile5 SpielerNameText leerezeile4 leerezeile15 leerezeile2
#define SpielStart leerezeile10 leerezeile3 SpielStartText1 SpielStartText2 leerezeile10 leerezeile2
