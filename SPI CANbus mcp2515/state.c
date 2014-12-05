#include <stdio.h>
#include "mcp2515.h"
#include "state.h"

void CANset(){
    CANSETMODE(CAN_MODE_CONFIG);  //�R���t�B�M�����[�V�������[�h

    CANinit(CAN_BRP_20MHZ_1MBPS);
    CANSETSJW(1);
    CANSETBTLMODE();
    CANSETPHSEG(2);
    CANSETPHSEG1(2);
    CANSETPHSEG2(2);
    CANRX0IE_PERMIT();
    CANRX1IE_PERMIT();
    CANTX0IE_PERMIT();
    CANTX1IE_PERMIT();
    CANTX2IE_PERMIT();
    CANERRIE_PERMIT();
    CANWAKIE_FORBID();
    CANMERRE_PERMIT();
    CANB0RTSM_OFF();
    CANB1RTSM_OFF();
    CANB2RTSM_OFF();

    CANSETFILTERRXB0(CAN_RXBnCTRL_MODE_STDON);
    CANSETFILTERRXB1(CAN_RXBnCTRL_MODE_STDON);

    CANSETSIDFILTER0(0x0111);
    CANSETSIDFILTER1(0x0222);
    CANSETSIDFILTER2(0x0333);
    CANSETSIDFILTER3(0x0444);
    CANSETSIDFILTER4(0x0555);
    CANSETSIDFILTER5(0x0666);

    CANSETSIDMASK0(MASK_SID_NOT);   //���S�t�B���^��v
    CANSETSIDMASK1(MASK_SID_NOT);   //���S�t�B���^��v
}

void Make_rx(unsigned char *dat,unsigned char *data,unsigned char cnt){
    unsigned char p;
    for(p=0;p<cnt;p++){
        *dat |= (((unsigned char)data[p])<<(p*8));
    }
}

void Make_tx(unsigned char *data,unsigned int dat,unsigned char cnt){
    unsigned char p;
    for(p=0;p<cnt;p++){
        data[p] = (unsigned char)dat;
        dat >>= 8;
    }
}

void REC_errer(void){
    unsigned char eflg;
    unsigned char check_eflg;

    eflg = CANReadEFLG();

    if((eflg&0x01) == 0x01){  //�G���[�x��
        //�����͂ЂƂ܂��������悤
    }else if((eflg&0x02) == 0x02){    //��M�G���[�x��
        //������������
    }else if((eflg&0x08) == 0x08){    //��M�p�b�V�u�G���[
        //REC��128�����ɂȂ�܂Ń��[�v
        for(;;){
            check_eflg = 0;
            check_eflg = CANReadEFLG();
            if((check_eflg&0x08) == 0x00) break;
        }
    }else if((eflg&0x40) == 0x40){  //��M�o�b�t�@0�I�[�o�[�t���[�t���O
        CANBitModCmd(CAN_ADRS_EFLG,0x40,0x00);  //�t���O�N���A
    }else if((eflg&0x80) == 0x80){  //��M�o�b�t�@1�I�[�o�[�t���[�t���O
        CANBitModCmd(CAN_ADRS_EFLG,0x80,0x00);  //�t���O�N���A
    }
}

void TEC_errer(void){
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

void Msgsendb0(unsigned int data,unsigned short msgid,unsigned char mode,unsigned char dlength,unsigned char rnk){
    unsigned char msgbuf[6] = {0};
    unsigned char datbuf[8] = {0};
    unsigned char ctrl = 0;
    unsigned char dlc = 0;
    unsigned char check_ctrl = 0;
    unsigned char i = 0;
    struct TXBnCTRL_t *pctrl;
    struct TXBnDLC_t *pdlc;

    pdlc = (struct TXBnDLC_t *)&dlc;
    pctrl = (struct TXBnCTRL_t *)&ctrl;

//���M�����푗�M��������܂�1��ȏ�J��Ԃ�
    do{ //���b�Z�[�W���M

        //�o�b�t�@�Ƀf�[�^�𑗐M����O��TXREQ��0
        CANTXB0TXREQReset();

        //SID
        msgbuf[1] = GET_SIDH(msgid);
        msgbuf[2] = GET_SIDL(msgid);
        msgbuf[3] = 0;
        msgbuf[4] = 0;

        //DLC�̐ݒ�
        dlc = 0;
        pdlc->DLC = dlength;
        pdlc->RTR = mode;
        msgbuf[5] = dlc;

        //�f�[�^�E�t�B�[���h�̐ݒ�
        Make_tx(&datbuf[0],data,dlength);

        //TXB0�Ƀf�[�^�����[�h
        CANTxB0BufMsgWrite(&msgBuf[1]);
        CANTxB0BufDatWrite(&datBuf[0],dlength);

        //���M�J�n
        ctrl = 0;
        pctrl->TXP = rnk;
        pctrl->TXREQ = 1;
        CANWriteReg(CAN_ADRS_TXB0_CTRL,ctrl);

        CANSetRTS(SPI_INST_RTS_TXB0);

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
    unsigned char rcv_id[6] = {0};
    unsigned char rcv_dat[8] = {0};
    unsigned char rx0_data = 0;
    unsigned char rx1_data = 0;
    unsigned short msgid = 0;
    unsigned char dlc = 0;
    unsigned char intf = 0;
    unsigned char check_rxstat = 0;

    REC_Errer();

    intf = CANREADSTAT();

    if((intf&0x01) == 0x01){    //��M�o�b�t�@0�t�����荞��

        check_rxstat = CANREADRXSTAT();

        if((check_rxstat&0x18) == 0x18){   //�g�������[�g�t���[��
        }else if((check_rxstat&0x10) == 0x10){ //�g���f�[�^�t���[��
        }else if((check_rxstat&0x08) == 0x08){ //�W�������[�g�t���[��
        }else if((check_rxstat&0x00) == 0x00){  //�W���f�[�^�t���[��

            rcv_id[0] = CANREADREG(CAN_ADRS_RXB0_CTRL);

            //SPI�C���X�g���N�V�����ɂ�莩���I�Ɏ�M�o�b�t�@�O�t���O�N���A
            CANRXB0BUFMSGREAD(&rcv_id[1]);
            msgid = MAKE_SID(rcv_id[1],rcv_id[2]);  //���ʎq�쐬
            dlc = rcv_id[5]&0x0F;   //�f�[�^���𒲂ׂ�

            CANRXB0BUFDATREAD(&rcv_dat[0],dlc);

            Make_rx(&rx0_data,&rcv_dat[0],dlc);

            if(msgid == 0x0111){

            }else if(msgid == 0x0222){

            }
        }
    }else if((intf&0x02) == 0x02){  //��M�o�b�t�@1�t�����荞��

        check_rxstat = CANREADRXSTAT();

        if((check_rxstat&0x18) == 0x18){   //�g�������[�g�t���[��
        }else if((check_rxstat&0x10) == 0x10){ //�g���f�[�^�t���[��
        }else if((check_rxstat&0x08) == 0x08){ //�W�������[�g�t���[��
        }else if((check_rxstat&0x00) == 0x00){  //�W���f�[�^�t���[��

            rcv_id[0] = CANREADREG(CAN_ADRS_RXB1_CTRL);

            //SPI�C���X�g���N�V�����ɂ�莩���I�Ɏ�M�o�b�t�@�O�t���O�N���A
            CANRXB1BUFMSGREAD(&rcv_id[1]);
            msgid = MAKE_SID(rcv_id[1],rcv_id[2]);  //���ʎq�쐬
            dlc = rcv_id[5]&0x0F;   //�f�[�^���𒲂ׂ�

            CANRXB1BUFDATREAD(&rcv_dat[0],dlc);

            Make_data(&rx1_data,&rcv_dat[0],dlc);

            if(msgid == 0x0333){

            }else if(msgid == 0x0444){

            }else if(msgid == 0x0555){

            }
        }
    }else if((intf&0x08) == 0x08){  //���M�o�b�t�@0�󊄂荞��
        CANbitmodcmd(CAN_ADRS_CANINTF,0x04,0x00);
    }else if((intf&0x20) == 0x20){  //���M�o�b�t�@1�󊄂荞��
        CANbitmodcmd(CAN_ADRS_CANINTF,0x08,0x00);
    }else if((intf&0x80) == 0x80){  //���M�o�b�t�@2�󊄂荞��
        CANbitmodcmd(CAN_ADRS_CANINTF,0x10,0x00);
    }
}
