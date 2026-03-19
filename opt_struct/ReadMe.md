
### parsexec.c
parsexec.c und showMenu() kann in dieser logik wahrscheinlich vollständig durch look ersetzt werden.



### Ein paar überlegungen zur Umsetzung und Nachfragen meinerseits:


hab mir das gestern und vorgestern mal angeschaut
ich habe das gefühl, dass die implimentierung da etwas verschachtelt ist
also sehr viele verschiedene Funtkionen die alle durch die freie Texteingabe an verschieden Stellen robust in der Ausführung sein müssen
ich habe gestern auch schon etwas mit schreiben angefangen
bin aber auch nicht sehr weit weil ich das ganze etwas anders aufrollen würde und ich erstmal fragen wollte

Ich hätte gesagt wir machen das ganze zwecks der Robustheit etwas mehr geführt. 
Die Interaktion des Spielers erfolgt über eine Art Menü. Statt freien funktionsaufrufen in welchen auch Argumente übergeben werden, werden einzelne Funktionen zuerst über das Menü aufgerufen. Es folgt ein neues Menü für die ausgewählte Funktion in welchem die möglichen Argumente aufgelistet sind und übergeben werden können.

Um den Ablauf pro Location für uns so übersichtlich wie möglich zu halten würde ich weitestgehend bei den beiden funktionen look und go bleiben, wobei ich den "go" Aspekt von go ehr implizit zu look packen würde und go ehr als eine sehr mächtige interact funktion schreiben würde.
Der grobe Ablauf pro Location sieht in meinem kopf grade so aus:
Der spieler betritt einen Raum (oder wird das erste mal "gespwant") und look() wird aufgerufen. Look() listet jetzt alle Instanzen der Location auf und nummeriert sie. Ich würde die Instanzen als klasse auch relativ grob halten und einfach gleich Personen, Türen und Objekte  hintereinander auflisten. Ich würde aber sagen wir limitieren uns erstmal auf diese Instanzen wobei insbesonder geschaut werden muss wie komplex wir Objekte und Personen gestaleten wollen. 
Das argument von look(), also Person, Tür oder Objekt kann über die zahl in der liste ausgewählt werden. Look() übergibt die description der Instanz "Große Tür...", "alter Mann" (nach belieben ausgeschmückt und mit Inhalt versehen für Interactions oder Rätsel) und ruft Interact() auf.

Interact muss jetzt also relativ komplex sein und lässt sich grob in drei unterfunktionen für die jeweilöige Instanz untereilen. Hier wird dann auch ein untermenü für die möglichen aktionen aufgerufen. Objekte wie kisten oder schränke könnten geöffnet werden um Items zu bekommen. Türen könnten verschlossen sein und mit Items wie schlüsseln geöffnet werden. Mit NPCs kann gerdet und gehandelt werden. Aber da muss ich noch überlegen wie man dass am besten so allgmein hält das wir nicht 80 verschiedene funktionen schreiben müssen. 
Generell würde ich sehr viel mehr mit structs arbeiten für so ziemlich alles was ich oben genannt habe
so sollte das dann auch mit einer allgemeineren interact funktion funktionieren

Ich würde mich am liebsten erstmal drauf konzentrieren ein möglichst gutes framework zu entwerfen. 
Wenn wir es schaffen eine interact funktion zu bauen die im wesentlichen nur drei dinge können muss:
Objekte durchsuchen -> items bekommen: Schlüssel, Texte
Türen aufschließen mit schlüsseln
Mit Personen reden

Dann können wir wenn wir die strukturen der Instanzen einmal klar definieren, relativ einfach und modular ein komplexes spiel bauen. Also, denke ich. Ich würde so UI schnittstellen erstmal relativ offen lassen.
Ich meine die Zahlen eingabe ist ja relativ flexibel. Also wenn das wirklich nur eine separate UI ist und die eingabe gleich bleibt dann können wir es komplett parallel schreiben.
Ich weiß halt nur nicht wie es wäre wenn wir zumbeispiel mouseclicks auf felder als steuerung machen wollen.
Das müssten wir uns halt dann angucken
