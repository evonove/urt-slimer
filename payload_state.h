#ifndef PAYLOAD_STATE_H
#define PAYLOAD_STATE_H

enum PackageState
{
    START_BYTE_1,
    START_BYTE_2,
    DEST_ID,
    INC_MSB,
    INC_LSB,
    PACK_LENGTH,
    PAYLOAD,
    CRC_MSB,
    CRC_B3,
    CRC_B2,
    CRC_LSB,
    EXIT,

};

#endif // PAYLOAD_STATE_H
