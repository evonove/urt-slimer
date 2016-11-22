#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QByteArray>
#include <QString>
#include <QObject>

#include "payload_state.h"
#include "parser.h"


class SerialReader : public QObject
{
    Q_OBJECT

public:
    SerialReader();
    QByteArray serialBuffer;
    QByteArray sendBuffer;
    quint8 payload_index = 0x00;
    quint8 payload_length = 0x00;
    QString status;
    void processData(QByteArray datas, PackageState &state);

public slots:
    void runSerialReader();

/*signals:
    void finished();*/

private:
    Parser parser;

};

#endif // SERIALREADER_H
