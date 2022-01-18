#ifndef MEMORYSPIEL_H
#define MEMORYSPIEL_H

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include "memorykarte.h"
#include <QTimer>
#include <QMessageBox>

//unsere Klasse memoryspiel erbt von QWidget
class memoryspiel : public QWidget
{
    //das Makro Q_OBJECT
    Q_OBJECT
    //die Attribute
    //die Tabelle für das Spielfeld
    QTableWidget* spielfeld;
    //die Methoden
    //der Konstruktor
    public:
    memoryspiel();

    private:
    void karteOeffnen(memorykarte *karte);
    void kartenSchliessen();
    void computerZug();

    void paarPruefen(int kartenID);
    void paarGefunden();
    void spielerWechseln();
    //die Slots
    private slots:
    void mausKlickSlot(int x, int y);
    void timerSlot();

    QLabel *labelTextMensch, *labelMensch, *labelTextComputer,
    *labelComputer;

    QString bildNamen[21] = {
    ":/bilder/Memoryspiel/apfel.bmp", ":/bilder/Memoryspiel/birne.bmp",
    ":/bilder/Memoryspiel/blume.bmp", ":/bilder/Memoryspiel/blume2.bmp",
    ":/bilder/Memoryspiel/ente.bmp", ":/bilder/Memoryspiel/fisch.bmp",
    ":/bilder/Memoryspiel/fuchs.bmp", ":/bilder/Memoryspiel/igel.bmp",
    ":/bilder/Memoryspiel/kaenguruh.bmp", ":/bilder/Memoryspiel/katze.bmp",
    ":/bilder/Memoryspiel/kuh.bmp", ":/bilder/Memoryspiel/maus1.bmp",
    ":/bilderMemoryspiel//maus2.bmp", ":/bilder/Memoryspiel/maus3.bmp",
    ":/bilder/Memoryspiel/melone.bmp", ":/bilder/Memoryspiel/pilz.bmp",
    ":/bilder/Memoryspiel/ronny.bmp", ":/bilder/Memoryspiel/schmetterling.bmp",
    ":/bilder/Memoryspiel/sonne.bmp", ":/bilder/Memoryspiel/wolke.bmp",
    ":/bilder/Memoryspiel/maus4.bmp"
    };

    memorykarte *karten[42];

    //für den Timer
    QTimer *timerUmdrehen;
    //für die offenen Karten
    int umgedrehteKarten;
    //für den Spieler
    int spieler;
    //für das aktuell umgedrehte Paar
    memorykarte *paar[2];
    //für die Punkte
    int menschPunkte, computerPunkte;
    //das "Gedächtnis" für den Computer
    int gemerkteKarten[2][21];
};
#endif // MEMORYSPIEL_H
