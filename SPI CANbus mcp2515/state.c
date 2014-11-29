#include <stdio.h>
#include "state.h"
#include "mcp2515.h"

void Make_data(unsigned char *dat,unsigned char *data,unsigned char cnt){
    unsigned char p;
    for(p=0;p<cnt;p++){
        *dat |= (((unsigned char)data[p])<<(p*8));
    }
}

void print(unsigned char *data,unsigned char dat,unsigned char cnt){
    unsigned char p;
    for(p=0;p<cnt;p++){
        data[p] = (unsigned char)dat;
        dat >>= 8;
    }
}

void REC_Errer(void){
    unsigned char eflg;
    unsigned char check_eflg;

    eflg = CANReadEFLG();

    if((eflg&0x01) == 0x01){  //�G���[�x��
        //�����͂ЂƂ܂��������悤
    }else if((eflg&0x02) == 0x02){    //��M�G���[�x��
        //������������
    }else if((eflg&0x08) == 0x08){    //��M�p�b�V�u�G���[
        check_eflg = 0;
        check_eflg = CANReadEFLG();
        //REC��128�����ɂȂ�܂ŋ󃋁[�v
        while((check_eflg&0x08) != 0x00);
    }else if((eflg&0x40) == 0x40){  //��M�o�b�t�@0�I�[�o�[�t���[�t���O
        CANBitModCmd(CAN_ADRS_EFLG,0x40,0x00);  //�t���O�N���A
    }else if((eflg&0x80) == 0x80){  //��M�o�b�t�@1�I�[�o�[�t���[�t���O
        CANBitModCmd(CAN_ADRS_EFLG,0x80,0x00);  //�t���O�N���A
    }
}

void TEC_Errer(void){
    unsigned char eflg;
    unsigned char check_eflg;

    eflg = CANReadEFLG();

    if((eflg&0x01)==0x01){  //�G���[�x��
        //�����͂ЂƂ܂��������悤
    }else if((eflg&0x04)==0x04){    //���M�G���[�x��
        //������������
    }else if((eflg&0x10)==0x10){    //���M�p�b�V�u�G���[
        //TEC��128�����ɂȂ�܂ŋ󃋁[�v
        for(;;){
            check_eflg = 0;
            check_eflg = CANReadEFLG();
            if((check_eflg&0x10) == 0x00) break;
        }
    }else if((eflg&0x20)==0x20){    //�o�X�I�t�G���[
        CANTxBufOFF();  //���ׂĂ̑��M�o�b�t�@���M��~
        //�o�X�񕜃V�[�P���X�����M�����܂ŋ󃋁[�v
        for(;;){
            check_eflg = 0;
            check_eflg = CANReadEFLG();
            if((check_eflg&0x20) == 0x00) break;
        }
        CANTxBufON();   //�S���M��~�̏I��
    }
}

void MsgsendB0(unsigned char data,unsigned short MsgID,unsigned char mode,unsigned char DLength,unsigned char rk){
    unsigned char MsgBuf[6] = {0,0,0,0,0,0};
    unsigned char DatBuf[8] = {0,0,0,0,0,0,0,0};
    struct TXBnCTRL_t *pctrl;
    struct TXBnDLC_t *pdlc;
    unsigned char ctrl,dlc;
    unsigned char check_ctrl;
    unsigned char i;

    pdlc = (struct TXBnDLC_t *)&dlc;
    pctrl = (struct TXBnCTRL_t *)&ctrl;

//���M�����푗�M��������܂�1��ȏ�J��Ԃ�
    do{ //���b�Z�[�W���M

        //�o�b�t�@�Ƀf�[�^�𑗐M����O��TXREQ��0
        CANTXB0TXREQReset();

        //SID
        MsgBuf[1] = GET_SIDH(MsgID);
        MsgBuf[2] = GET_SIDL(MsgID);
        MsgBuf[3] = 0;
        MsgBuf[4] = 0;

        //DLC�̐ݒ�
        dlc = 0;
        pdlc->DLC = DLength;
        pdlc->RTR = mode;
        MsgBuf[5] = dlc;

        //�f�[�^�E�t�B�[���h�̐ݒ�
        print(&DatBuf[0],data,DLength);

        //TXB0�Ƀf�[�^�����[�h
        CANTxB0BufMsgWrite(&MsgBuf[1]);
        CANTxB0BufDatWrite(&DatBuf[0],DLength);

        //���M�J�n
        ctrl = 0;
        pctrl->TXP = rk;
        pctrl->TXREQ = 1;
        CANWriteReg(CAN_ADRS_TXB0_CTRL,ctrl);

        CANSetRTS(SPI_INST_RTS_TXB0);

        check_ctrl = 0;
        check_ctrl = CANReadReg(CAN_ADRS_TXB0_CTRL);

        if((check_ctrl&0x10) == 0x10){   //���M���Ƀo�X�G���[������
            TEC_Errer();
        }else if((check_ctrl&0x20) == 0x20){  //�A�[�r�g���[�V����������
            ctrl = 0;
            pctrl->TXREQ = 0;   //���b�Z�[�W���M�҂����N���A
            CANWriteReg(CAN_ADRS_TXB0_CTRL,ctrl);
        }
    }while((check_ctrl&0x40) != 0x00);    //���b�Z�[�W���M���튮���ŋU
}

void State_interrupt(void){
    unsigned char rcv_ID[6];
    unsigned char rcv_Dat[8];
    unsigned char RX0_data;
    unsigned char RX1_data;
    unsigned short msgID;
    unsigned char dlc;
    unsigned char intf;
    unsigned char check_rxstat;

    REC_Errer();

    intf = 0;
    intf = CANReadStat();

    //RB1 = 1;
    if((intf&0x01) == 0x01){    //��M�o�b�t�@0�t�����荞��

        check_rxstat = 0;
        check_rxstat = CANReadRXStat();

        if((check_rxstat&0x18) == 0x18){   //�g�������[�g�t���[��
        }else if((check_rxstat&0x10) == 0x10){ //�g���f�[�^�t���[��
        }else if((check_rxstat&0x08) == 0x08){ //�W�������[�g�t���[��
        }else if((check_rxstat&0x00) == 0x00){  //�W���f�[�^�t���[��

            rcv_ID[0] = CANReadReg(CAN_ADRS_RXB0_CTRL);

            //SPI�C���X�g���N�V�����ɂ�莩���I�Ɏ�M�o�b�t�@�O�t���O�N���A
            CANRxB0BufMsgRead(&rcv_ID[1]);
            msgID = MAKE_SID(rcv_ID[1],rcv_ID[2]);  //���ʎq�쐬
            dlc = rcv_ID[5]&0x0F;   //�f�[�^���𒲂ׂ�

            CANRxB0BufDatRead(&rcv_Dat[0],dlc);

            RX0_data = 0;
            Make_data(&RX0_data,&rcv_Dat[0],dlc);

            if(msgID == 0x0111){

            }else{

            }
        }
    }else if((intf&0x02) == 0x02){  //��M�o�b�t�@1�t�����荞��

        check_rxstat = 0;
        check_rxstat = CANReadRXStat();

        if((check_rxstat&0x18) == 0x18){   //�g�������[�g�t���[��

        }else if((check_rxstat&0x10) == 0x10){ //�g���f�[�^�t���[��

        }else if((check_rxstat&0x08) == 0x08){ //�W�������[�g�t���[��

        }else if((check_rxstat&0x00) == 0x00){  //�W���f�[�^�t���[��

            rcv_ID[0] = CANReadReg(CAN_ADRS_RXB1_CTRL);

            //SPI�C���X�g���N�V�����ɂ�莩���I�Ɏ�M�o�b�t�@�O�t���O�N���A
            CANRxB1BufMsgRead(&rcv_ID[1]);
            msgID = MAKE_SID(rcv_ID[1],rcv_ID[2]);  //���ʎq�쐬
            dlc = rcv_ID[5]&0x0F;   //�f�[�^���𒲂ׂ�

            CANRxB1BufDatRead(&rcv_Dat[0],dlc);

            RX1_data = 0;
            Make_data(&RX1_data,&rcv_Dat[0],dlc);

            if(msgID == 0x0222){

            }else{

            }
        }
    }else if((intf&0x08) == 0x08){  //���M�o�b�t�@0�󊄂荞��
        CANBitModCmd(CAN_ADRS_CANINTF,0x04,0x00);
    }else if((intf&0x20) == 0x20){  //���M�o�b�t�@1�󊄂荞��
        CANBitModCmd(CAN_ADRS_CANINTF,0x08,0x00);
    }else if((intf&0x80) == 0x80){  //���M�o�b�t�@2�󊄂荞��
        CANBitModCmd(CAN_ADRS_CANINTF,0x10,0x00);
    }
}
