#include "memoryspiel.h"
#include <algorithm>
#include <ctime>

memoryspiel::memoryspiel()
{
    //die Größe setzen
    resize(394, 550);
    //und den Titel
    setWindowTitle("Memoryspiel");

    //den Timer erstellen
    timerUmdrehen = new QTimer(this);
    //die Attribute initialisieren
    //es ist keine Karte umgedreht
    umgedrehteKarten = 0;
    //der Spieler 0 – der Mensch – fängt an
    spieler = 0;
    //es hat noch keiner Punkte
    menschPunkte = 0;
    computerPunkte = 0;
    //gemerkteKarten wird mit -1 initialisiert
    for (int aussen = 0; aussen < 2; aussen++)
    for (int innen = 0; innen < 21; innen++)
    gemerkteKarten[aussen][innen] = -1;

    //eine neue Tabelle mit sieben Zeilen und sechs Spalten
    //erzeugen
    spielfeld = new QTableWidget(7, 6, this);

    //die Spalten- und Zeilenköpfe ausblenden
    spielfeld->horizontalHeader()->hide();
    spielfeld->verticalHeader()->hide();
    //das Gitternetz ausblenden
    spielfeld->setShowGrid(false);
    //die Auswahl deaktivieren
    spielfeld->setSelectionMode(QTableWidget::NoSelection);
    //das Bearbeiten deaktivieren
    spielfeld->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //die Höhe und Breite der Zellen setzen
    for (int zeile = 0; zeile < 7; zeile++)
    spielfeld->setRowHeight(zeile, 64);
    for (int spalte = 0; spalte < 6; spalte++)
    spielfeld->setColumnWidth(spalte, 64);

    labelTextMensch = new QLabel(this);
    labelTextMensch->setText("Mensch");
    labelTextMensch->setGeometry(10, 460, 50, 25);
    labelMensch = new QLabel(this);
    labelMensch->setText("0");
    labelMensch->setGeometry(80, 460, 50, 25);
    labelTextComputer = new QLabel(this);
    labelTextComputer->setText("Computer");
    labelTextComputer->setGeometry(10, 480, 50, 25);
    labelComputer = new QLabel(this);
    labelComputer->setText("0");
    labelComputer->setGeometry(80, 480, 50, 25);

    int bildZaehler = 0;
    for (int schleife = 0; schleife < 42; schleife++)
    {
        //bitte in einer Zeile eingeben
        karten[schleife] = new memorykarte(bildNamen[bildZaehler],
        bildZaehler);
        if ((schleife + 1) % 2 == 0)
        bildZaehler++;
    }

    //die Karten durcheinander werfen
    srand(time(NULL));
    std::random_shuffle(karten, karten + 42);

    //die Größe der Icons einstellen
    spielfeld->setIconSize(QSize(64,64));
    //die Karten in die Tabelle stellen
    for (int zeile = 0; zeile < 7; zeile++)
    for (int spalte = 0; spalte < 6; spalte++)
    {
        //bitte jeweils in einer Zeile eingeben
        spielfeld->setItem(zeile, spalte, karten[(spalte * 7) +
        zeile]);
        //die Position setzen
        karten[(spalte * 7) + zeile]-> setBildPos((spalte * 7) +
        zeile);
    }
/*
    for (int zeile = 0; zeile < 7; zeile++)
    for (int spalte = 0; spalte < 6; spalte++)
    karten[(spalte * 7) + zeile]->umdrehen();
*/
    //die Größe anpassen
    spielfeld->resize(394, 458);

    //das Anklicken einer Zelle mit dem Slot mausKlickSlot
    //verbinden
    //bitte jeweils in einer Zeile eingeben
    QObject::connect(spielfeld,SIGNAL(cellClicked(int, int)),
    this, SLOT(mausKlickSlot(int, int)));
    //den Timer mit dem Slot verbinden
    QObject::connect(timerUmdrehen,SIGNAL(timeout()), this,
    SLOT(timerSlot()));
}

void memoryspiel::karteOeffnen(memorykarte *karte)
{
    //zum Zwischenspeichern der Nummer und der Position
    int kartenID, kartenPos;
    //die Karten zwischenspeichern
    paar[umgedrehteKarten] = karte;
    //die Nummer und die Position beschaffen
    kartenID = karte->getBildID();
    kartenPos = karte->getBildPos();
    //die Karten in das Gedächtnis eintragen
    //aber nur dann, wenn es noch keinen Eintrag an der
    //entsprechenden Stelle gibt
    if ((gemerkteKarten[0][kartenID] == -1))
        gemerkteKarten[0][kartenID] = kartenPos;
        else
        //wenn es schon einen Eintrag gibt und der nicht
        //mit der aktuellen Position übereinstimmt, haben
        //wir die zweite Karte gefunden
        //Sie wird in die zweite Dimension eingetragen
        if ((gemerkteKarten[0][kartenID] != kartenPos))
        gemerkteKarten[1][kartenID] = kartenPos;
        //umgedrehte Karten erhöhen
        umgedrehteKarten++;
        //sind zwei Karten umgedreht worden?
        //dann prüfen wir, ob es ein Paar ist
        if (umgedrehteKarten == 2)
        {
            paarPruefen(kartenID);
            //die Karten wieder umdrehen
            kartenSchliessen();
        }
        //haben wir zusammen 21 Paare?
        //dann ist das Spiel vorbei
        if (menschPunkte + computerPunkte == 21)
        {
            //bitte in einer Zeile eingeben
            QMessageBox::information(this, "Spielende", "Das Spiel ist vorbei");
            this->close();
        }
}


//die Methode für den Computerzug
void memoryspiel::computerZug()
{
    //zur Steuerung der Spielstärke
    if ((rand() % spielstaerke) == 0)
    {
        int kartenZaehler = 0;
        int zufall = 0;
        bool treffer = false;
        //erst einmal nach einem Paar suchen
        //dazu durchsuchen wir das Feld gemerkteKarten, bis wir
        //in den beiden Dimensionen einen Wert für eine Karte
        //finden

        while ((kartenZaehler < 21) && (treffer == false))
        {
            //gibt es in beiden Dimensionen einen Wert größer
            //oder gleich 0?
            //bitte in einer Zeile eingeben
            if ((gemerkteKarten[0][kartenZaehler] >= 0) &&
            (gemerkteKarten[1][kartenZaehler] >= 0))
            {
                //dann haben wir ein Paar
                treffer = true;
                //die erste Karte umdrehen
                karten[gemerkteKarten[0][kartenZaehler]]-> umdrehen();
                karteOeffnen(karten[gemerkteKarten[0] [kartenZaehler]]);
                //und die zweite auch
                karten[gemerkteKarten[1][kartenZaehler]]-> umdrehen();
                karteOeffnen(karten[gemerkteKarten[1] [kartenZaehler]]);
            }
            kartenZaehler++;
        }
    }


    //wenn wir kein Paar gefunden haben, drehen wir
    //zufällig zwei Karten um
    if (treffer == false)
    {
        //so lange eine Zufallszahl suchen, bis eine Karte
        //gefunden wird, die noch im Spiel ist
        do
        {
            zufall = rand() % 42;
        } while (karten[zufall]->getNochImSpiel() == false);

            //die erste Karte umdrehen
            karten[zufall]->umdrehen();
            karteOeffnen(karten[zufall]);
            //für die zweite Karte müssen wir außerdem prüfen,
            //ob sie nicht gerade angezeigt wird
            do
            {
                zufall = rand() % 42;
            //bitte in einer Zeile eingeben
            } while ((karten[zufall]->getNochImSpiel() == false) || (karten[zufall]->getUmgedreht() == true));
    //und die zweite Karte umdrehen
    karten[zufall]->umdrehen();
    karteOeffnen(karten[zufall]);
    }
}

//die Methode prüft, ob ein Paar gefunden wurde
void memoryspiel::paarPruefen(int kartenID)
{
    if (paar[0]->getBildID() == paar[1]->getBildID())
    {
        //die Punkte setzen
        paarGefunden();
        //die Karten aus dem Gedächtnis löschen
        gemerkteKarten[0][kartenID] = -2;
        gemerkteKarten[1][kartenID] = -2;
    }
}

//die Methode setzt die Punkte, wenn ein Paar gefunden wurde
void memoryspiel::paarGefunden()
{
    //spielt gerade der Mensch?
    if (spieler == 0)
    {
        menschPunkte++;
        labelMensch->setNum(menschPunkte);
    }
    else
    {
        computerPunkte++;
        labelComputer->setNum(computerPunkte);
    }
}

//die Methode dreht die Karten wieder um bzw. nimmt sie
//aus dem Spiel
void memoryspiel::kartenSchliessen()
{
    bool raus = false;
    //ist es ein Paar?
    if (paar[0]->getBildID() == paar[1]->getBildID())
    {
        //dann nehmen wir die Karten aus dem Spiel
        paar[0]->rausnehmen();
        paar[1]->rausnehmen();
        //raus wird auf true gesetzt
        raus = true;
    }
    else
    {
        //sonst drehen wir die Karten nur wieder um
        paar[0]->umdrehen();
        paar[1]->umdrehen();
    }
    //es ist keine Karte mehr geöffnet
    umgedrehteKarten = 0;
    //hat der Spieler kein Paar gefunden
    if (raus == false)
    //dann wird der Spieler gewechselt
    spielerWechseln();
    else
        //hat der Computer eine Paar gefunden?
        //dann ist er noch einmal an der Reihe
        if (spieler == 1)
        computerZug();
}

//die Methode wechselt den Spieler
void memoryspiel::spielerWechseln()
{
//wenn der Mensch an der Reihe war, kommt jetzt der Computer
if (spieler == 0)
{
    spieler = 1;
    computerZug();
}
else
    spieler = 0;
}

//der Slot für das Anklicken einer Zelle
void memoryspiel::mausKlickSlot(int x, int y)
{
    //ist die Karte nicht schon umgedreht und ist sie noch
    //im Spiel?
    //bitte in einer Zeile eingeben
    if ((karten[(y * 7) + x]->getUmgedreht() == false) &&  (karten[(y * 7) + x]->getNochImSpiel() == true))
    {
        //dann umdrehen
        karten[(y * 7) + x]->umdrehen();
        karteOeffnen(karten[(y * 7) + x]);
    }
}
//der Slot für den Timer
//Er ist ebenfalls noch leer!
void memoryspiel::timerSlot()
{
}
