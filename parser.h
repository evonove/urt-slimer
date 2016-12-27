#ifndef PARSER_H
#define PARSER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QCoreApplication>
#include <QByteArray>
#include <QQueue>
#include <QObject>

class Parser : public QObject
{
    Q_OBJECT

public:
    Parser();
    quint8 m_cmdTel;
    QByteArray m_sendBuffer;
    QByteArray m_bufferToParse;
    QByteArray m_bufferToPacket;
    quint16 m_payloadLength;
    QQueue<QByteArray> m_queue;
    void setPackage(QByteArray &serialBuffer);
    void sendPacketToAPI(QByteArray &m_bufferToPacket);

private:
    QNetworkAccessManager *m_manager;
    QEventLoop m_eventLoop;
    quint32 m_temp;
    quint16 m_rpm;
    quint16 m_inj1;
    quint16 m_inj2;
    quint16 m_ign1;
    quint16 m_ign2;
    qint8 m_wtemp;
    qint8 m_airtemp;
    qint16 m_airp;
    qint8 m_oilt;
    quint8 m_tps;
    quint8 m_lambda_1;
    quint8 m_lambda_trg;
    quint8 m_speed_fl;
    quint8 m_speed_fr;
    quint8 m_speed_rl;
    quint8 m_speed_rr;
    quint8 m_vbatt;
    quint16 m_aux;
    quint16 m_det;
    quint32 m_fcmp;
    quint8 m_ecu_error;
    quint8 m_fpga_error;
    quint8 m_status;
    quint16 m_delta;
    quint16 m_dwell;
    qint8 m_map_tc_sel;
    qint8 m_slpf_fl;
    qint8 m_slpf_fr;
    qint8 m_slpf_rl;
    qint8 m_slpf_rr;
    quint8 m_cutoff;
    quint16 m_time;
    quint8 m_aux_data1;
    quint8 m_aux_data2;
    quint8 m_aux_data3;
    quint8 m_aux_data4;
    quint8 m_aux_data5;
    quint8 m_aux_data6;
    quint8 m_aux_data7;
    quint8 m_aux_data8;
    quint8 m_aux_data9;
    quint8 m_aux_data10;
    quint8 m_aux_data11;
    quint8 m_aux_data12;
    quint8 m_aux_data13;
    quint8 m_aux_data14;
    quint8 m_aux_data15;
    quint8 m_aux_data16;
    quint8 m_aux_data17;
    quint8 m_aux_data18;
    quint8 m_aux_data19;
    quint8 m_aux_data20;
    quint8 m_aux_data21;
    quint8 m_aux_data22;
    quint8 m_aux_data23;
    quint8 m_aux_data24;
    quint8 m_aux_data25;
    quint8 m_aux_data26;
    quint8 m_aux_data27;
    quint8 m_aux_data28;
    quint8 m_aux_data29;
    quint8 m_aux_data30;
    quint8 m_aux_data31;
    quint8 m_aux_data32;
};

#endif // PARSER_H
