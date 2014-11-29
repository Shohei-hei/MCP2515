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

    if((eflg&0x01) == 0x01){  //エラー警告
        //ここはひとまず無視しよう
    }else if((eflg&0x02) == 0x02){    //受信エラー警告
        //ここも無視で
    }else if((eflg&0x08) == 0x08){    //受信パッシブエラー
        check_eflg = 0;
        check_eflg = CANReadEFLG();
        //RECが128未満になるまで空ループ
        while((check_eflg&0x08) != 0x00);
    }else if((eflg&0x40) == 0x40){  //受信バッファ0オーバーフローフラグ
        CANBitModCmd(CAN_ADRS_EFLG,0x40,0x00);  //フラグクリア
    }else if((eflg&0x80) == 0x80){  //受信バッファ1オーバーフローフラグ
        CANBitModCmd(CAN_ADRS_EFLG,0x80,0x00);  //フラグクリア
    }
}

void TEC_Errer(void){
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

//送信が正常送信完了するまで1回以上繰り返す
    do{ //メッセージ送信

        //バッファにデータを送信する前にTXREQを0
        CANTXB0TXREQReset();

        //SID
        MsgBuf[1] = GET_SIDH(MsgID);
        MsgBuf[2] = GET_SIDL(MsgID);
        MsgBuf[3] = 0;
        MsgBuf[4] = 0;

        //DLCの設定
        dlc = 0;
        pdlc->DLC = DLength;
        pdlc->RTR = mode;
        MsgBuf[5] = dlc;

        //データ・フィールドの設定
        print(&DatBuf[0],data,DLength);

        //TXB0にデータをロード
        CANTxB0BufMsgWrite(&MsgBuf[1]);
        CANTxB0BufDatWrite(&DatBuf[0],DLength);

        //送信開始
        ctrl = 0;
        pctrl->TXP = rk;
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
    if((intf&0x01) == 0x01){    //受信バッファ0フル割り込み

        check_rxstat = 0;
        check_rxstat = CANReadRXStat();

        if((check_rxstat&0x18) == 0x18){   //拡張リモートフレーム
        }else if((check_rxstat&0x10) == 0x10){ //拡張データフレーム
        }else if((check_rxstat&0x08) == 0x08){ //標準リモートフレーム
        }else if((check_rxstat&0x00) == 0x00){  //標準データフレーム

            rcv_ID[0] = CANReadReg(CAN_ADRS_RXB0_CTRL);

            //SPIインストラクションにより自動的に受信バッファ０フラグクリア
            CANRxB0BufMsgRead(&rcv_ID[1]);
            msgID = MAKE_SID(rcv_ID[1],rcv_ID[2]);  //識別子作成
            dlc = rcv_ID[5]&0x0F;   //データ長を調べる

            CANRxB0BufDatRead(&rcv_Dat[0],dlc);

            RX0_data = 0;
            Make_data(&RX0_data,&rcv_Dat[0],dlc);

            if(msgID == 0x0111){

            }else{

            }
        }
    }else if((intf&0x02) == 0x02){  //受信バッファ1フル割り込み

        check_rxstat = 0;
        check_rxstat = CANReadRXStat();

        if((check_rxstat&0x18) == 0x18){   //拡張リモートフレーム

        }else if((check_rxstat&0x10) == 0x10){ //拡張データフレーム

        }else if((check_rxstat&0x08) == 0x08){ //標準リモートフレーム

        }else if((check_rxstat&0x00) == 0x00){  //標準データフレーム

            rcv_ID[0] = CANReadReg(CAN_ADRS_RXB1_CTRL);

            //SPIインストラクションにより自動的に受信バッファ０フラグクリア
            CANRxB1BufMsgRead(&rcv_ID[1]);
            msgID = MAKE_SID(rcv_ID[1],rcv_ID[2]);  //識別子作成
            dlc = rcv_ID[5]&0x0F;   //データ長を調べる

            CANRxB1BufDatRead(&rcv_Dat[0],dlc);

            RX1_data = 0;
            Make_data(&RX1_data,&rcv_Dat[0],dlc);

            if(msgID == 0x0222){

            }else{

            }
        }
    }else if((intf&0x08) == 0x08){  //送信バッファ0空割り込み
        CANBitModCmd(CAN_ADRS_CANINTF,0x04,0x00);
    }else if((intf&0x20) == 0x20){  //送信バッファ1空割り込み
        CANBitModCmd(CAN_ADRS_CANINTF,0x08,0x00);
    }else if((intf&0x80) == 0x80){  //送信バッファ2空割り込み
        CANBitModCmd(CAN_ADRS_CANINTF,0x10,0x00);
    }
}
