#include <QCoreApplication>
#include <QByteArray>
#include <QSerialPort>
#include <QObject>
#include <QTimer>

#include "serialreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SerialReader serialReader;

    QTimer::singleShot(0, &serialReader, SLOT(runSerialReader()));

    //QObject::connect(serialReader, SIGNAL(finished()), &a, SLOT(quit()));

    return a.exec();
}
