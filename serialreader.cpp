#include "serialreader.h"
#include "parser.h"
#include "payload_state.h"

#include <QByteArray>
#include <QSerialPort>

SerialReader::SerialReader() {}

void SerialReader::runSerialReader(){
    PackageState state = START_BYTE_1;

    QSerialPort *serialPort = new QSerialPort();
    serialPort->setPortName("ttyUSB0");
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::Input);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->open(QIODevice::ReadOnly);

    if (serialPort->isOpen())
    {
        qDebug() << "Serial port is open...";
        while (serialPort->waitForReadyRead(50))
        {
            QByteArray datas = serialPort->readAll();
            if (datas.size() == 0){
                qDebug() << "ERROR data not read";
            } else {
                processData(datas, state);
            }
            QCoreApplication::processEvents();
        }
    } else {
        qDebug() << "OPEN ERROR: " << serialPort->errorString();
    }

    serialPort->close();
    qDebug() << "...serial port is closed!";
    //emit finished();

}

void SerialReader::processData(QByteArray datas, PackageState &state)
{
    for (int i = 0; i < datas.size(); i++)
    {
        switch (state)
        {
        case START_BYTE_1:
            if (datas.at(i) == 0xAA)
            {
                qDebug() << "entrato nello start byte 1";
                state = START_BYTE_2;
                serialBuffer.append(datas.at(i));
            }
            break;
        case START_BYTE_2:
            if (datas.at(i) == 0x55)
            {
                qDebug() << "entrato nello start byte 2";
                state = DEST_ID;
                serialBuffer.append(datas.at(i));
            }
            else
            {
                qDebug() << "errore --> start byte 1";
                state = START_BYTE_1;
                serialBuffer.clear();
            }
            break;
        case DEST_ID:
            if (datas.at(i) == 0x01)
            {
                qDebug() << "entrato nel dest id";
                state = INC_MSB;
                serialBuffer.append(datas.at(i));
            }
            else
            {
                qDebug() << "error --> start byte 1";
                state = START_BYTE_1;
                serialBuffer.clear();
            }
            break;
        case INC_MSB:
            qDebug() << "entrato inc msb";
            serialBuffer.append(datas.at(i));
            state = INC_LSB;
            break;
        case INC_LSB:
            qDebug() << "entrato inc lsb";
            serialBuffer.append(datas.at(i));
            state = PACK_LENGHT;
            break;
        case PACK_LENGHT:
            qDebug() << "entrato pack length";
            serialBuffer.append(datas.at(i));
            payload_length = datas.at(i);
            if (payload_length == 0x00){
                state = EXIT;
                qDebug() << "payload length 0";
            }
            else
            {
                qDebug() << "payload length != 0";
                state = PAYLOAD;
            }
            break;
        case PAYLOAD:
            serialBuffer.append(datas.at(i));
            if (payload_index == payload_length){
                state = CRC_MSB;
                qDebug() << "~payload_index " << payload_index;
                qDebug() << "~payload_length" << payload_length;
                qDebug() << "~buffer " << serialBuffer.size();
            }
            payload_index++;
            break;
        case CRC_MSB:
            qDebug() << "entrato nel crc msb";
            serialBuffer.append(datas.at(i));
            state = CRC_B3;
            break;
        case CRC_B3:
            qDebug() << "entrato nel crc b3";
            serialBuffer.append(datas.at(i));
            state = CRC_B2;
            break;
        case CRC_B2:
            qDebug() << "entrato nel crc b2";
            serialBuffer.append(datas.at(i));
            state = CRC_LSB;
            break;
        case CRC_LSB:
            qDebug() << "entrato nel crc lsb";
            serialBuffer.append(datas.at(i));
            state = EXIT;
            break;
        case EXIT:
            qDebug() << "entrato exit";
            if (datas.at(i) == 0x0F) { //0x0F fine del pacchetto
                qDebug() << "payload corretto";
                serialBuffer.append(datas.at(i));
                //status = "CORRECT_PAYLOAD";
                qDebug() << serialBuffer.toHex();
                qDebug() << serialBuffer.size();
                parser.setPackage(serialBuffer);
                state = START_BYTE_1;
                serialBuffer.clear();
                payload_index = 0x00;
                payload_length = 0x00;
                status = "";
            } else{
                qDebug() << "errore ultimo byte";
                serialBuffer.append(datas.at(i));
                //status = "ERROR_PAYLOAD";
                qDebug() << serialBuffer.toHex();
                qDebug() << serialBuffer.size();
                //setPackage(serialBuffer, sendBuffer, status);
                state = START_BYTE_1;
                serialBuffer.clear();
                payload_index = 0x00;
                payload_length = 0x00;
                //status = null;
            }
            break;
        default:
            break;
        }
    }
}
