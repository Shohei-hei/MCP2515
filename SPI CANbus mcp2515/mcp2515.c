#include <stdio.h>
#include "mcp2515.h"

//SPI����M�֐�
unsigned char SPI_TxRx(unsigned char Tx_data,unsigned char cmd){
    unsigned char i;
    unsigned char Rx_data;
    CAN_SPI_SO = 0;
    CAN_SPI_SCK = 0;
    if(cmd == 1){
        //���M�i1�o�C�g���j
        for(i=0;i<8;i++){
            if((Tx_data & 0x80) == 0x80){
                CAN_SPI_SO = 1;
            }else{
                CAN_SPI_SO = 0;
            }
            Tx_data <<= 1;
            CAN_SPI_SCK = 1;
            CAN_SPI_SCK = 0;
        }
    }else{
        //��M�i�P�o�C�g���j
        Rx_data = 0;
        for(i=0;i<8;i++){
            Rx_data <<= 1;
            if(CAN_SPI_SI == 1){
                Rx_data |= 0x01;
            }else{
                Rx_data |= 0x00;
            }
            CAN_SPI_SCK = 1;
            CAN_SPI_SCK = 0;
        }
    }
    return (Rx_data);
}
//CAN���W�X�^�o�C�g���C�g
void CANWriteReg(unsigned char adrs,unsigned char data){
    CAN_SPI_CS = 0;
    SPI_TxRx(SPI_INST_WRITE,1);
    SPI_TxRx(adrs,1);
    SPI_TxRx(data,1);
    CAN_SPI_CS = 1;
}
//CAN��/��M�o�b�t�@�@�V�[�P���V�����E���[�h/���C�g ���L
void CANTxRxBufRW(unsigned char inst,unsigned char readop,unsigned char *data,unsigned char cnt){
    unsigned char dat;
    unsigned char p;
    CAN_SPI_CS = 0;
    SPI_TxRx(inst,1);
    if(readop == READ){
        for(p=0;p<cnt;p++){
            dat = 0;
            dat = SPI_TxRx(0xFF,0);
            data[p] = dat;
        }
    }else if(readop == WRITE){
        for(p=0;p<cnt;p++){
            dat = data[p];
            SPI_TxRx(dat,1);
        }
    }
    CAN_SPI_CS = 1;
}

//CAN�r�b�g���f�B�t�@�C�R�}���h
void CANBitModCmd(unsigned char adrs,unsigned char mask,unsigned char data){
    CAN_SPI_CS = 0;
    SPI_TxRx(SPI_INST_BITMODIFY,1);
    SPI_TxRx(adrs,1);
    SPI_TxRx(mask,1);
    SPI_TxRx(data,1);
    CAN_SPI_CS = 1;
}
//CAN SID �t�B���^�}�X�N�ݒ�
void CANSetSidFilterMask(unsigned char adrs,unsigned short sid){
    CANWriteReg(adrs,GET_SIDH(sid));
    CANWriteReg(adrs + 1,GET_SIDL(sid));
}
//���W�X�^�E���[�h���L���[�`��
unsigned char CANRegRead2B(unsigned char inst,unsigned char adrs){
    unsigned char dat;
    CAN_SPI_CS = 0;
    SPI_TxRx(inst,1);
    SPI_TxRx(adrs,1);
    dat = SPI_TxRx(0xFF,0);
    CAN_SPI_CS = 1;
    return (dat);
}
//CAN���Z�b�g
void CANReset(void){
    CAN_SPI_CS = 0;
    SPI_TxRx(SPI_INST_RESET,1);
    CAN_SPI_CS = 1;
}
//CAN���M�v������
void CANSetRTS(unsigned char inst){
    CAN_SPI_CS = 0;
    SPI_TxRx(inst,1);
    CAN_SPI_CS = 1;
}
//CAN��M�`�F�b�N
unsigned char CANRxCheck(unsigned char rxbnum){
    unsigned char rxstat;
    rxstat = CANReadRXStat();
    if(rxbnum == 0){
        return (rxstat & (1<<MsgRXB0));
    }else if(rxbnum == 1){
        return (rxstat & (1<<MsgRXB1));
    }
    return (rxstat & ((1<<MsgRXB0))|(1<<MsgRXB1));
}
//CAN�R���g���[��������
void CANInit(unsigned char brp){
    CANSetBRP(brp);
}
