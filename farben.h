/*
Makros für die Farben
Beispiel:
	ohne Makro:
        printf("\033[33mText in der Farbe Gelb.\n");
        printf("\033[33mText in der Farbe Gelb\033[32m und in der Farbe Grün.\n");

	mit Makro:
        printf(gelb "Text in der Farbe Gelb mit Makro.\n");
        printf(gelb "Text in der Farbe Gelb mit Makro" grün " und in der Farbe Grün.\n");
*/

// Vordergrund (Text Farbe)
#define normal "\033[0m" // Entfernt die zuvor festgelegte Farbe
#define schwarz "\033[30m"
#define grau "\033[90m"
#define hellgrau "\033[37m"
#define rot "\033[31m"
#define hellrot "\033[91m"
#define grün "\033[32m"
#define hellgrün "\033[92m"
#define gelb "\033[33m"
#define hellgelb "\033[93m"
#define blau "\033[34m"
#define hellblau "\033[94m"
#define lila "\033[35m"
#define pink "\033[95m"
#define cyan "\033[36m"
#define hellcyan "\033[96m"
#define weiß "\033[97m"

// Hintergrund (Farbe hinter dem Text)
#define schwarzH "\033[40m"
#define grauH "\033[100m"
#define hellgrauH "\033[47m"
#define rotH "\033[41m"
#define hellrotH "\033[101m"
#define grünH "\033[42m"
#define hellgrünH "\033[102m"
#define gelbH "\033[43m"
#define hellgelbH "\033[103m"
#define blauH "\033[44m"
#define hellblauH "\033[104m"
#define lilaH "\033[45m"
#define pinkH "\033[105m"
#define cyanH "\033[46m"
#define hellcyanH "\033[106m"
#define weißH "\033[107m"

// Text
// #define fett "\033[1m" // die Textfarbe wird hell anstatt fett
#define a "\033[2m"
#define kursiv "\033[3m"
#define unterstrichen "\033[4m"
#define blinken "\033[5m"
#define durchgestrichen "\033[9m"

