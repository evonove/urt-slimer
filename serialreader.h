#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QByteArray>
#include <QString>
#include <QObject>
#include <QSerialPort>

#include "payload_state.h"
#include "parser.h"


class SerialReader : public QObject
{
    Q_OBJECT

public:
    SerialReader();
    ~SerialReader();
    QByteArray m_serialBuffer;
    QByteArray m_sendBuffer;
    quint8 m_payloadIndex;
    quint8 m_payloadLength;
    void processData(QByteArray datas, PackageState &state);

public slots:
    void runSerialReader();

signals:
    void finished();

private:
    QSerialPort *m_serialPort;
    Parser m_parser;
};

#endif // SERIALREADER_H
