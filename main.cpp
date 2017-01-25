#include <QCoreApplication>
#include <QByteArray>
#include <QSerialPort>
#include <QObject>
#include <QTimer>
#include <QProcessEnvironment>

#include "serialreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto env = QProcessEnvironment::systemEnvironment();
    if (!env.contains("SLIMER_BACKEND_URL") && !env.contains("SLIMER_BACKEND_USERNAME") && !env.contains("SLIMER_BACKEND_PASSWORD")) {
        qDebug() << "Environment variables not set";
        a.quit();
    }

    SerialReader serialReader;

    QTimer::singleShot(0, &serialReader, SLOT(runSerialReader()));

    QObject::connect(&serialReader, SIGNAL(finished()), &a, SLOT(quit()));

    return a.exec();
}
