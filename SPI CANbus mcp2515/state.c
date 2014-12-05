#include <stdio.h>
#include "mcp2515.h"
#include "state.h"

void Make_rx(unsigned char *dat,unsigned char *data,unsigned char cnt){
    unsigned char p;
    for(p=0;p<cnt;p++){
        *dat |= (((unsigned char)data[p])<<(p*8));
    }
}

void Make_tx(unsigned char *data,unsigned char dat,unsigned char cnt){
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

    if((eflg&0x01) == 0x01){  //エラー警告
        //ここはひとまず無視しよう
    }else if((eflg&0x02) == 0x02){    //受信エラー警告
        //ここも無視で
    }else if((eflg&0x08) == 0x08){    //受信パッシブエラー
        //RECが128未満になるまでループ
        for(;;){
            check_eflg = 0;
            check_eflg = CANReadEFLG();
            if((check_eflg&0x08) == 0x00) break;
        }
    }else if((eflg&0x40) == 0x40){  //受信バッファ0オーバーフローフラグ
        CANBitModCmd(CAN_ADRS_EFLG,0x40,0x00);  //フラグクリア
    }else if((eflg&0x80) == 0x80){  //受信バッファ1オーバーフローフラグ
        CANBitModCmd(CAN_ADRS_EFLG,0x80,0x00);  //フラグクリア
    }
}

void TEC_errer(void){
    unsigned char eflg;
    unsigned char check_eflg;

    eflg = CANReadEFLG();

    if((eflg&0x01)==0x01){  //エラー警告
        //ここはひとまず無視しよう
    }else if((eflg&0x04)==0x04){    //送信エラー警告
        //ここも無視で
    }else if((eflg&0x10)==0x10){    //送信パッシブエラー
        //TECが128未満になるまで空ループ
        for(;;){
            check_eflg = 0;
            check_eflg = CANReadEFLG();
            if((check_eflg&0x10) == 0x00) break;
        }
    }else if((eflg&0x20)==0x20){    //バスオフエラー
        CANTxBufOFF();  //すべての送信バッファ送信停止
        //バス回復シーケンス正常受信完了まで空ループ
        for(;;){
            check_eflg = 0;
            check_eflg = CANReadEFLG();
            if((check_eflg&0x20) == 0x00) break;
        }
        CANTxBufON();   //全送信停止の終了
    }
}

void Msgsendb0(unsigned char data,unsigned short msgid,unsigned char mode,unsigned char dlength,unsigned char rnk){
    unsigned char msgbuf[6] = {0,0,0,0,0,0};
    unsigned char datbuf[8] = {0,0,0,0,0,0,0,0};
    struct TXBnCTRL_t *pctrl;
    struct TXBnDLC_t *pdlc;
    unsigned char ctrl,dlc;
    unsigned char check_ctrl;
    unsigned char i;

    pdlc = (struct TXBnDLC_t *)&dlc;
    pctrl = (struct TXBnCTRL_t *)&ctrl;

//送信が正常送信完了するまで1回以上繰り返す
    do{ //メッセージ送信

        //バッファにデータを送信する前にTXREQを0
        CANTXB0TXREQReset();

        //SID
        msgbuf[1] = GET_SIDH(msgid);
        msgbuf[2] = GET_SIDL(msgid);
        msgbuf[3] = 0;
        msgbuf[4] = 0;

        //DLCの設定
        dlc = 0;
        pdlc->DLC = dlength;
        pdlc->RTR = mode;
        msgbuf[5] = dlc;

        //データ・フィールドの設定
        Make_tx(&datbuf[0],data,dlength);

        //TXB0にデータをロード
        CANTxB0BufMsgWrite(&msgBuf[1]);
        CANTxB0BufDatWrite(&datBuf[0],dlength);

        //送信開始
        ctrl = 0;
        pctrl->TXP = rnk;
        pctrl->TXREQ = 1;
        CANWriteReg(CAN_ADRS_TXB0_CTRL,ctrl);

        CANSetRTS(SPI_INST_RTS_TXB0);

        check_ctrl = 0;
        check_ctrl = CANReadReg(CAN_ADRS_TXB0_CTRL);

        if((check_ctrl&0x10) == 0x10){   //送信中にバスエラーが発生
            TEC_Errer();
        }else if((check_ctrl&0x20) == 0x20){  //アービトレーションを失う
            ctrl = 0;
            pctrl->TXREQ = 0;   //メッセージ送信待ちをクリア
            CANWriteReg(CAN_ADRS_TXB0_CTRL,ctrl);
        }
    }while((check_ctrl&0x40) != 0x00);    //メッセージ送信正常完了で偽
}

void State_interrupt(void){
    unsigned char rcv_id[6];
    unsigned char rcv_dat[8];
    unsigned char rx0_data;
    unsigned char rx1_data;
    unsigned short msgid;
    unsigned char dlc;
    unsigned char intf;
    unsigned char check_rxstat;

    REC_Errer();

    intf = 0;
    intf = CANREADSTAT();

    if((intf&0x01) == 0x01){    //受信バッファ0フル割り込み

        check_rxstat = 0;
        check_rxstat = CANREADRXSTAT();

        if((check_rxstat&0x18) == 0x18){   //拡張リモートフレーム
        }else if((check_rxstat&0x10) == 0x10){ //拡張データフレーム
        }else if((check_rxstat&0x08) == 0x08){ //標準リモートフレーム
        }else if((check_rxstat&0x00) == 0x00){  //標準データフレーム

            rcv_id[0] = CANREADREG(CAN_ADRS_RXB0_CTRL);

            //SPIインストラクションにより自動的に受信バッファ０フラグクリア
            CANRXB0BUFMSGREAD(&rcv_id[1]);
            msgid = MAKE_SID(rcv_id[1],rcv_id[2]);  //識別子作成
            dlc = rcv_id[5]&0x0F;   //データ長を調べる

            CANRXB0BUFDATREAD(&rcv_dat[0],dlc);

            rx0_data = 0;
            Make_rx(&rx0_data,&rcv_dat[0],dlc);

            if(msgid == 0x0111){

            }else if(msgid == 0x0222){

            }
        }
    }else if((intf&0x02) == 0x02){  //受信バッファ1フル割り込み

        check_rxstat = 0;
        check_rxstat = CANREADRXSTAT();

        if((check_rxstat&0x18) == 0x18){   //拡張リモートフレーム

        }else if((check_rxstat&0x10) == 0x10){ //拡張データフレーム

        }else if((check_rxstat&0x08) == 0x08){ //標準リモートフレーム

        }else if((check_rxstat&0x00) == 0x00){  //標準データフレーム

            rcv_id[0] = CANREADREG(CAN_ADRS_RXB1_CTRL);

            //SPIインストラクションにより自動的に受信バッファ０フラグクリア
            CANRXB1BUFMSGREAD(&rcv_id[1]);
            msgid = MAKE_SID(rcv_id[1],rcv_id[2]);  //識別子作成
            dlc = rcv_id[5]&0x0F;   //データ長を調べる

            CANRXB1BUFDATREAD(&rcv_dat[0],dlc);

            rx1_data = 0;
            Make_data(&rx1_data,&rcv_dat[0],dlc);

            if(msgid == 0x0333){

            }else if(msgid == 0x0444){

            }else if(msgid == 0x0555){

            }
        }
    }else if((intf&0x08) == 0x08){  //送信バッファ0空割り込み
        CANbitmodcmd(CAN_ADRS_CANINTF,0x04,0x00);
    }else if((intf&0x20) == 0x20){  //送信バッファ1空割り込み
        CANbitmodcmd(CAN_ADRS_CANINTF,0x08,0x00);
    }else if((intf&0x80) == 0x80){  //送信バッファ2空割り込み
        CANbitmodcmd(CAN_ADRS_CANINTF,0x10,0x00);
    }
}
