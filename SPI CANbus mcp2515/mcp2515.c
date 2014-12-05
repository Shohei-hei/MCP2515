#include <stdio.h>
#include "mcp2515.h"

//--------------------------------------------------------
//
//  mcp2515�Ɋւ���֐�����
//
//  �ҏW����邱�Ƃ�]�݂܂���
//
//--------------------------------------------------------

//SPI����M�֐�
unsigned char SPI_txrx(unsigned char Tx_data,unsigned char cmd){
    unsigned char i = 0;
    unsigned char Rx_data = 0;
    CAN_SPI_SO = 0;
    CAN_SPI_SCK = 0;
    if(cmd == WRITE){
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
    }else if(cmd == READ){
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
void CANwritereg(unsigned char adrs,unsigned char data){
    CAN_SPI_CS = 0;
    SPI_TxRx(SPI_INST_WRITE,WRITE);
    SPI_TxRx(adrs,WRITE);
    SPI_TxRx(data,WRITE);
    CAN_SPI_CS = 1;
}
//CAN��/��M�o�b�t�@�@�V�[�P���V�����E���[�h/���C�g ���L
void CANtxrxbufrw(unsigned char inst,unsigned char readop,unsigned char *data,unsigned char cnt){
    unsigned char dat = 0;
    unsigned char p = 0;
    CAN_SPI_CS = 0;
    SPI_TxRx(inst,WRITE);
    if(readop == READ){
        for(p=0;p<cnt;p++){
            dat = 0;
            dat = SPI_TxRx(0xFF,READ);
            data[p] = dat;
        }
    }else if(readop == WRITE){
        for(p=0;p<cnt;p++){
            dat = data[p];
            SPI_TxRx(dat,WRITE);
        }
    }
    CAN_SPI_CS = 1;
}

//CAN�r�b�g���f�B�t�@�C�R�}���h
void CANbitmodcmd(unsigned char adrs,unsigned char mask,unsigned char data){
    CAN_SPI_CS = 0;
    SPI_TxRx(SPI_INST_BITMODIFY,WRITE);
    SPI_TxRx(adrs,WRITE);
    SPI_TxRx(mask,WRITE);
    SPI_TxRx(data,WRITE);
    CAN_SPI_CS = 1;
}
//CAN SID �t�B���^�}�X�N�ݒ�
void CANsetsidfiltermask(unsigned char adrs,unsigned short sid){
    CANWriteReg(adrs,GET_SIDH(sid));
    CANWriteReg(adrs + 1,GET_SIDL(sid));
}
//���W�X�^�E���[�h���L���[�`��
unsigned char CANregread2b(unsigned char inst,unsigned char adrs){
    unsigned char dat = 0;
    CAN_SPI_CS = 0;
    SPI_TxRx(inst,WRITE);
    SPI_TxRx(adrs,WRITE);
    dat = SPI_TxRx(0xFF,READ);
    CAN_SPI_CS = 1;
    return (dat);
}
//CAN���Z�b�g
void CANreset(void){
    CAN_SPI_CS = 0;
    SPI_TxRx(SPI_INST_RESET,WRITE);
    CAN_SPI_CS = 1;
}
//CAN���M�v������
void CANsetrts(unsigned char inst){
    CAN_SPI_CS = 0;
    SPI_TxRx(inst,WRITE);
    CAN_SPI_CS = 1;
}
//CAN��M�`�F�b�N
/*unsigned char CANrxcheck(unsigned char rxbnum){
    unsigned char rxstat;
    rxstat = CANReadRXStat();
    if(rxbnum == 0){
        return (rxstat & (1<<MsgRXB0));
    }else if(rxbnum == 1){
        return (rxstat & (1<<MsgRXB1));
    }
    return (rxstat & ((1<<MsgRXB0))|(1<<MsgRXB1));
}*/
//CAN�R���g���[��������
void CANinit(unsigned char brp){
    CANSetBRP(brp);
}
