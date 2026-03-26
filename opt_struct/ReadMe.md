


### prxys, tokens und actions



Zünächst habe ich mir in env_handleInteract() überlegt wie ich am besten eine interaction proxy aufbauen kann in welcher auf den "inhalt" der Instanz (also Door, Person oder Objekt) zugreifen kann. Die Antwort war klar: ein Pointer auf die Instanz, übergeben in env_handleLook(). Ich wusste blöß nicht wie den pointer im head von env_handleInteract() deklarieren muss weil er ja auf drei verschiedene Strukturen zeigen kann. 
Dann habe ich void pointer entdeckt aber ich musste ein weiters schnipsel an art der struktur übergeben. Ich wollte es wieder über enum machen aber es muste typdef enum sein, damit ich auch das deklarieren kann... 
Dann gibt es drei switch statements in env_handleInteract(). Fand ich schon da etwas klobig da sie im prinzip das gleiche machen. Und ich es auch über einen token lösen kann.

Der token void pointer ist zusammen mit der Actions struct wahrscheinlich mein magnum opus in diesem Projekt. Actions.actionFunc und .token erlauben es ziemlich flexible aktionen zu bauen da der token im argument von actionFunc unmittelbar aus Action übergeben werden kann.

Wie mächtig diese struct ist, ist mir gestern aufgefallen als ich bei mehreren probloemen begann die recht umständlich zu lösen, mir dann aber aufgefallen ist, dass ich sie relativ einfach über den Action.token lösen kann. Oder das ich die Action struktur neben den uhrsprünglichen act_funkionen bei env_handleInteract auch für itm_ oder sat_funktionen aus relativ anderen strukturen nutzen kann (Die Prefixe sind etwas lose weil theoretisch alle funktionen überall eingesetzt werden könn, unübersichtlich). Oder wie es mir sowieso schon kratz env_funktionen wie env_hanleInteract auch als actionFunc zu schreiben, was eigentlich ziemlich einfach gehen sollte, wo es aber auch noch mehr potential gibt.

Und mir ist grade beim schreiben auch aufgefallen wie man das machen könnte. Und es hängt eigentlich ehr mit einem anderen itch zusammen der mich die ganze zeit stört. Um allgemeine Funktionen wie unOrLock, satisfy oder giveItem zu schreiben, überge ich als token momentan den Pointer auf den relevanten Eintrag der Instanz, und packe ihn in der Funktion unter Anahme, dasd jede instanz einen eintrag der genau dieser Form hat aus und arbeite damit. 
Was mich etwas stört, und was u.a. ja auch der Grund war ist, dass prxy für diese allgemeinen Funktionen einfach mitgeschleppt wird, weil es schwer ist die struct info im allgemeinen Fall mit zu übergeben ohne den token dafür zu verbrennen (was ja theorteisch auch gehen würde die funktionen aber zumindest für diese fälle unnötig mit switch cases für vollkommen identische Abläufe aufblähen würde). 


Falls man aber mal allgemeien funktionen mit leicht strukturabhängigen abläufen braucht oder man noch viel komplexere funktionen bauen möchte sollte ich dem token eine eigene struktur aus prxyTyp und einer liste and void* pointern bauen. Ich weiß zwar nicht wofür, aber die möglichkeiten wären insane.


Das ganze neigt aber denke ich dazu unübersichtlich zu werde.







### nutzung von KI
#### UPDATE

Bis auf einzelne strukturen in der gs.h datei welche im ersten push als blueprint für mich dienten, befindet sich nun kein Code mehr in opt_struct welcher unmittelbar durch KI erstellt wurde. Unten angeführte funktionsskizzen wurden dabei im zuge der neuen logik vollständig durch selbstproduzierte ersetzt.

Ausnahmen sind einzelne Zeilen welche im zuge von bugfixes übernommen wurden. Die KI eigenen kommentare dienen als markierung. Sie müssten sich von ein paar meiner Kommtare abheben. Bisher sind das jedoch nur drei kleine bugs in verschieden fuktionen in functions.c.

KI wird jedoch fern von bugfixes weiterhin indirekt als Nachschlagewerk/Tutor verwendet. Dabei habe ich z.B. noch ein paar nachfragen zum robusten handeln von void pointern gestellt. 

#### UPDATE


Wie bei der einzelnen Abschnitten vielleicht auffällt wurde mit KI gearbeitet. Die Nutzung dieser beschränkte sich dabei aber auf eine art tutor mit nachfragen zur Implementation.
Sie wurde also nur genutz wenn ich bereits wusste was ich tun wollt, jedoch nicht wusste wie z.B. der syntax oder einzelne schritte zur implementation.

Zum Beispiel wusste ich, dass ich Instanzen in Locations über structs definieren wollte, und auch das ich diese definierten instanzen dann irgendwie (wahrscheinlich mit pointern) in die struct der Location verlinken will um die structs der instanzen verknüpft an die location auslesen zu können, wusste aber nicht wie genau der syntax dafür aussieht. Ich hab mir dann ein snippet für Doors geben lassen was das macht und die logik später in der erweiterung auf objekte und personen übertragen. Sonst ist die Logik der Strukturen im GameState vollständig eigenständig.

Zusammenhängend damit, wurde auch für das tatsächliche "bauen" der Welt in functions KI zur hilfe herangezogen. In der vorherigen implementatioin habe ich die welt über ein großes statisches Array der locations gebaut in welchen doors und objects fest eingebettet waren. Ich wollte beim bauen dann in drei großen arrays zunächst alle instanzen definieren. Um diese dann nach belieben in die Locations zu legen. Da war ich mir wieder wegen der pointer logik unsicher und außerdem wusste ich nicht wie ich solche " [DOOR_START_CHAMBER] = " "Aliase" für statische array geschrieben werden müssen, dachte mir aber das es geht. (^^)

Es gab/gibt einzelne funktionen wie otherside() oder teile von showMenu() welche zu teilen auch mit KI geschrieben wurden. Diese wurden/werden aber im zuge neuer logik mit eigenem code ersetzt und dienten als erste stütze in der umsetzung. 

Gernerell kam die logik zur umsetzung aber immer von meiner seite und ich habe alle vorschläge, falls ich sie übernommen habe, gründlich nachvollzogen und auf meine situation angepasst.
Vieles davon ist aber z.B. auch durch vorschläge aus bugfixes an eigenem code hervorgegangen.

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
