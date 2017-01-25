#include "serialreader.h"
#include "parser.h"
#include "payload_state.h"

#include <QByteArray>
#include <QSerialPort>

SerialReader::SerialReader()
    : m_payloadIndex{0x00}
    , m_payloadLength{0x00}
    , m_serialPort{ new QSerialPort }
{
    m_serialPort->setPortName("ttyUSB0");
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::Input);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
}

SerialReader::~SerialReader()
{
    m_serialPort->deleteLater();
}

void SerialReader::runSerialReader(){
    PackageState state = START_BYTE_1;

    m_serialPort->open(QIODevice::ReadOnly);

    if (m_serialPort->isOpen()) {
        qDebug() << "Serial port is open...";
        while (m_serialPort->waitForReadyRead(50)) {
            QByteArray datas = m_serialPort->readAll();
            if (datas.size() == 0) {
                qDebug() << "ERROR data not read";
                break;
            }
            processData(datas, state);
            QCoreApplication::processEvents();
        }
    } else {
        qDebug() << "OPEN ERROR: " << m_serialPort->errorString();
    }

    m_serialPort->close();
    qDebug() << "...serial port is closed!";
    emit finished();
}

void SerialReader::processData(QByteArray datas, PackageState &state)
{
    for (int i = 0; i < datas.size(); i++) {
        switch (state) {
            case START_BYTE_1:
                if (datas.at(i) == 0xAA) {
                    qDebug() << "START_BYTE_1";
                    m_serialBuffer.append(datas.at(i));
                    state = START_BYTE_2;
                }
                break;
            case START_BYTE_2:
                if (datas.at(i) == 0x55) {
                    qDebug() << "START_BYTE_2";
                    m_serialBuffer.append(datas.at(i));
                    state = DEST_ID;
                } else {
                    qDebug() << "Error, resetting state to START_BYTE_1";
                    m_serialBuffer.clear();
                    state = START_BYTE_1;
                }
                break;
            case DEST_ID:
                if (datas.at(i) == 0x01) {
                    qDebug() << "DEST_ID";
                    m_serialBuffer.append(datas.at(i));
                    state = INC_MSB;
                } else {
                    qDebug() << "Error, resetting state to START_BYTE_1";
                    m_serialBuffer.clear();
                    state = START_BYTE_1;
                }
                break;
            case INC_MSB:
                qDebug() << "INC_MSB";
                m_serialBuffer.append(datas.at(i));
                state = INC_LSB;
                break;
            case INC_LSB:
                qDebug() << "INC_LSB";
                m_serialBuffer.append(datas.at(i));
                state = PACK_LENGTH;
                break;
            case PACK_LENGTH:
                qDebug() << "PACK_LENGTH";
                m_serialBuffer.append(datas.at(i));
                m_payloadLength = datas.at(i);
                if (m_payloadLength == 0x00) {
                    state = EXIT;
                    qDebug() << "Payload length is 0";
                } else {
                    state = PAYLOAD;
                    qDebug() << "Payload length is not 0";
                }
                break;
            case PAYLOAD:
                qDebug() << "PAYLOAD";
                m_serialBuffer.append(datas.at(i));
                if (m_payloadIndex == m_payloadLength){
                    state = CRC_MSB;
                    qDebug() << "Payload index" << m_payloadIndex;
                    qDebug() << "Payload length" << m_payloadLength;
                    qDebug() << "Buffer size " << m_serialBuffer.size();
                }
                m_payloadIndex++;
                break;
            case CRC_MSB:
                qDebug() << "CRC_MSB";
                m_serialBuffer.append(datas.at(i));
                state = CRC_B3;
                break;
            case CRC_B3:
                qDebug() << "CRC_B3";
                m_serialBuffer.append(datas.at(i));
                state = CRC_B2;
                break;
            case CRC_B2:
                qDebug() << "CRC_B2";
                m_serialBuffer.append(datas.at(i));
                state = CRC_LSB;
                break;
            case CRC_LSB:
                qDebug() << "CRC_LSB";
                m_serialBuffer.append(datas.at(i));
                state = EXIT;
                break;
            case EXIT:
                qDebug() << "EXIT";
                if (datas.at(i) == 0x0F) { // Packet end
                    qDebug() << "Correct payload";
                    m_serialBuffer.append(datas.at(i));
                    qDebug() << m_serialBuffer.toHex();
                    qDebug() << m_serialBuffer.size();
                    m_parser.setPackage(m_serialBuffer);
                } else{
                    qDebug() << "Error in last byte";
                }
                m_serialBuffer.clear();
                m_payloadIndex = 0x00;
                m_payloadLength = 0x00;
                state = START_BYTE_1;
                break;
            default:
                break;
        }
    }
}
