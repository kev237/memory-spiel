#include <QApplication>
//die Header-Datei der eigenen Klasse einbinden
#include "memoryspiel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //eine neue Instanz der Klasse memoryspiel erzeugen
    memoryspiel *meinMemoryspiel = new memoryspiel();
    //das Formular anzeigen
    meinMemoryspiel->show();
    return app.exec();
}
