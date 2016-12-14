#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QByteArray>
#include <QQueue>
#include <QEventLoop>
#include <QProcessEnvironment>

#include "parser.h"

Parser::Parser()
    : m_manager { new QNetworkAccessManager }
    , m_payloadLength{0x00}
{
    QObject::connect(m_manager, SIGNAL(finished(QNetworkReply*)), &m_eventLoop, SLOT(quit()));
}

void Parser::setPackage(QByteArray &serialBuffer){
    m_sendBuffer = serialBuffer;
    m_queue.enqueue(m_sendBuffer);

    while (!m_queue.isEmpty()) {
        m_bufferToParse = m_queue.dequeue();

        m_cmdTel = m_bufferToParse.at(2);
        m_payloadLength = m_bufferToParse.at(5);

        for (int i = 6; i < 84; i++) {
            m_bufferToPacket.append(m_bufferToParse.at(i));
        }
        sendPacketToAPI(m_bufferToPacket);
    }
}

void Parser::sendPacketToAPI(QByteArray &bufferToPacket){
    m_rpm = bufferToPacket.at(0);
    m_rpm = m_rpm << 8;
    m_rpm += bufferToPacket.at(1);

    m_inj1 = bufferToPacket.at(2);
    m_inj1 = m_inj1 << 8;
    m_inj1 += bufferToPacket.at(3);

    m_inj2 = bufferToPacket.at(4);
    m_inj2 = m_inj2 << 8;
    m_inj2 += bufferToPacket.at(5);

    m_ign1 = bufferToPacket.at(6);
    m_ign1 = m_ign1 << 8;
    m_ign1 += bufferToPacket.at(7);

    m_ign2 = bufferToPacket.at(8);
    m_ign2 = m_ign2 << 8;
    m_ign2 += bufferToPacket.at(9);

    m_wtemp = bufferToPacket.at(10);

    m_airtemp = bufferToPacket.at(11);

    m_airp = bufferToPacket.at(12);
    m_airp = m_airp << 8;
    m_airp += bufferToPacket.at(13);

    m_oilt = bufferToPacket.at(14);

    m_tps = bufferToPacket.at(15);

    m_lambda_1 = bufferToPacket.at(16);

    m_lambda_trg = bufferToPacket.at(17);

    m_speed_fl = bufferToPacket.at(18);

    m_speed_fr = bufferToPacket.at(19);

    m_speed_rl = bufferToPacket.at(20);

    m_speed_rr = bufferToPacket.at(21);

    m_vbatt = bufferToPacket.at(22);

    m_aux = bufferToPacket.at(23);
    m_aux = m_aux << 8;
    m_aux += bufferToPacket.at(24);

    m_det = bufferToPacket.at(25);
    m_det = m_det << 8;
    m_det += bufferToPacket.at(26);

    m_fcmp = bufferToPacket.at(27);
    m_fcmp = m_fcmp << 24;
    m_temp = bufferToPacket.at(28);
    m_temp = m_temp << 16;
    m_fcmp += m_temp;
    m_temp = 0;
    m_temp = bufferToPacket.at(29);
    m_temp = m_temp << 8;
    m_fcmp += m_temp;
    m_temp = 0;
    m_fcmp += bufferToPacket.at(30);

    m_ecu_error = bufferToPacket.at(31);

    m_fpga_error = bufferToPacket.at(32);

    m_status = bufferToPacket.at(33);

    m_delta = bufferToPacket.at(34);
    m_delta = m_delta << 8;
    m_delta += bufferToPacket.at(35);

    m_dwell = bufferToPacket.at(36);
    m_dwell = m_dwell << 8;
    m_dwell += bufferToPacket.at(37);

    m_map_tc_sel = bufferToPacket.at(38);

    m_slpf_fl = bufferToPacket.at(39);

    m_slpf_fr = bufferToPacket.at(40);

    m_slpf_rl = bufferToPacket.at(41);

    m_slpf_rr = bufferToPacket.at(42);

    m_cutoff = bufferToPacket.at(43);

    m_time = bufferToPacket.at(44);
    m_time = m_time << 8;
    m_time += bufferToPacket.at(45);

    m_aux_data1 = bufferToPacket.at(46);
    m_aux_data2 = bufferToPacket.at(47);
    m_aux_data3 = bufferToPacket.at(48);
    m_aux_data4 = bufferToPacket.at(49);
    m_aux_data5 = bufferToPacket.at(50);
    m_aux_data6 = bufferToPacket.at(51);
    m_aux_data7 = bufferToPacket.at(52);
    m_aux_data8 = bufferToPacket.at(53);
    m_aux_data9 = bufferToPacket.at(54);
    m_aux_data10 = bufferToPacket.at(55);
    m_aux_data11 = bufferToPacket.at(56);
    m_aux_data12 = bufferToPacket.at(57);
    m_aux_data13 = bufferToPacket.at(58);
    m_aux_data14 = bufferToPacket.at(59);
    m_aux_data15 = bufferToPacket.at(60);
    m_aux_data16 = bufferToPacket.at(61);
    m_aux_data17 = bufferToPacket.at(62);
    m_aux_data18 = bufferToPacket.at(63);
    m_aux_data19 = bufferToPacket.at(64);
    m_aux_data20 = bufferToPacket.at(65);
    m_aux_data21 = bufferToPacket.at(66);
    m_aux_data22 = bufferToPacket.at(67);
    m_aux_data23 = bufferToPacket.at(68);
    m_aux_data24 = bufferToPacket.at(69);
    m_aux_data25 = bufferToPacket.at(70);
    m_aux_data26 = bufferToPacket.at(71);
    m_aux_data27 = bufferToPacket.at(72);
    m_aux_data28 = bufferToPacket.at(73);
    m_aux_data29 = bufferToPacket.at(74);
    m_aux_data30 = bufferToPacket.at(75);
    m_aux_data31 = bufferToPacket.at(76);
    m_aux_data32 = bufferToPacket.at(77);

    auto env = QProcessEnvironment::systemEnvironment();
    QUrl url(env.value("SLIMER_BACKEND_URL"));
    auto userInfo = QString("%s:%s")
            .arg(env.value("SLIMER_BACKEND_USERNAME"))
            .arg(env.value("SLIMER_BACKEND_USERNAME"));
    url.setUserInfo(userInfo);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString json = QString("{"
                           "\"rpm\":\"%1\",\"inj1\":\"%2\","
                           "\"inj2\":\"%3\",\"ign1\":\"%4\","
                           "\"ign2\":\"%5\",\"wtemp\":\"%6\","
                           "\"airtemp\":\"%7\",\"airp\":\"%8\","
                           "\"oilt\":\"%9\",\"tps\":\"%10\","
                           "\"lambda_1\":\"%11\",\"lambda_trg\":\"%12\","
                           "\"speed_fl\":\"%13\",\"speed_fr\":\"%14\","
                           "\"speed_rl\":\"%15\",\"speed_rr\":\"%16\","
                           "\"vbatt\":\"%17\",\"aux\":\"%18\","
                           "\"det\":\"%19\",\"fcmp\":\"%20\","
                           "\"ecu_error\":\"%21\",\"fpga_error\":\"%22\","
                           "\"status\":\"%23\",\"delta\":\"%24\","
                           "\"dwell\":\"%25\",\"map_tc_sel\":\"%26\","
                           "\"slpf_fl\":\"%27\",\"slpf_fr\":\"%28\","
                           "\"slpf_rl\":\"%29\",\"slpf_rr\":\"%30\","
                           "\"cutoff\":\"%31\",\"time\":\"%32\","
                           "\"aux_data1\":\"%33\",\"aux_data2\":\"%34\","
                           "\"aux_data3\":\"%35\",\"aux_data4\":\"%36\","
                           "\"aux_data5\":\"%37\",\"aux_data6\":\"%38\","
                           "\"aux_data7\":\"%39\",\"aux_data8\":\"%40\","
                           "\"aux_data9\":\"%41\",\"aux_data10\":\"%42\","
                           "\"aux_data11\":\"%43\",\"aux_data12\":\"%44\","
                           "\"aux_data13\":\"%45\",\"aux_data14\":\"%46\","
                           "\"aux_data15\":\"%47\",\"aux_data16\":\"%48\","
                           "\"aux_data17\":\"%49\",\"aux_data18\":\"%50\","
                           "\"aux_data19\":\"%51\",\"aux_data20\":\"%52\","
                           "\"aux_data21\":\"%53\",\"aux_data22\":\"%54\","
                           "\"aux_data23\":\"%55\",\"aux_data24\":\"%56\","
                           "\"aux_data25\":\"%57\",\"aux_data26\":\"%58\","
                           "\"aux_data27\":\"%59\",\"aux_data28\":\"%60\","
                           "\"aux_data29\":\"%61\",\"aux_data30\":\"%62\","
                           "\"aux_data31\":\"%63\",\"aux_data32\":\"%64\"}")
            .arg(m_rpm).arg(m_inj1)
            .arg(m_inj2).arg(m_ign1)
            .arg(m_ign2).arg(m_wtemp)
            .arg(m_airtemp).arg(m_airp)
            .arg(m_oilt).arg(m_tps)
            .arg(m_lambda_1).arg(m_lambda_trg)
            .arg(m_speed_fl).arg(m_speed_fr)
            .arg(m_speed_rl).arg(m_speed_rr)
            .arg(m_vbatt).arg(m_aux)
            .arg(m_det).arg(m_fcmp)
            .arg(m_ecu_error).arg(m_fpga_error)
            .arg(m_status).arg(m_delta)
            .arg(m_dwell).arg(m_map_tc_sel)
            .arg(m_slpf_fl).arg(m_slpf_fr)
            .arg(m_slpf_rl).arg(m_slpf_rr)
            .arg(m_cutoff).arg(m_time)
            .arg(m_aux_data1).arg(m_aux_data2)
            .arg(m_aux_data3).arg(m_aux_data4)
            .arg(m_aux_data5).arg(m_aux_data6)
            .arg(m_aux_data7).arg(m_aux_data8)
            .arg(m_aux_data9).arg(m_aux_data10)
            .arg(m_aux_data11).arg(m_aux_data12)
            .arg(m_aux_data13).arg(m_aux_data14)
            .arg(m_aux_data15).arg(m_aux_data16)
            .arg(m_aux_data17).arg(m_aux_data18)
            .arg(m_aux_data19).arg(m_aux_data20)
            .arg(m_aux_data21).arg(m_aux_data22)
            .arg(m_aux_data23).arg(m_aux_data24)
            .arg(m_aux_data25).arg(m_aux_data26)
            .arg(m_aux_data27).arg(m_aux_data28)
            .arg(m_aux_data29).arg(m_aux_data30)
            .arg(m_aux_data31).arg(m_aux_data32);
    qDebug() << "Json " << json;
    QNetworkReply *reply = m_manager->post(request, json.toUtf8());
    m_eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Success -> " << reply->readAll();
        delete reply;
    } else {
        qDebug() << "Failure -> " << reply->errorString();
        delete reply;
    }
}
