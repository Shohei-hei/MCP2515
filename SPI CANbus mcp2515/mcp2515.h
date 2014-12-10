#ifndef MCP2515_H
#define MCP2515_H

//------------------------------------------------------------------------------
//ビットモディファイに関して
//マスク値を"１"に設定するとその値は変更することが出来るようになる
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//   SPIポート定義
//
//  #define CAN_SPI_CS PORT8.PORT.BIT.B0
//------------------------------------------------------------------------------

//SPIポートの操作定義
#define CAN_SPI_CS B0//SPIのCS(SS)ピンに設定
#define CAN_SPI_SO B1//SPIのSOピンに設定
#define CAN_SPI_SCK B2//SPIのSCKピンに設定

#define CAN_SPI_SI B3//SPIのSIピンに設定




//------------------------------------------------------------------------------
//  ＜警告＞
//
//  ここから下の設定を変更すると動かなくなる可能性があります。
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  MCP2515 レジスタDATAFLAMEB0(send_data,0x0111,4,3);
//------------------------------------------------------------------------------

//送受信バッファｎ制御レジスタ (n = 0,1,2)
struct TXBnCTRL_t{
    unsigned char TXP:2;
    unsigned char b2:1;
    unsigned char TXREQ:1;
    unsigned char TXERR:1;
    unsigned char MLOA:1;
    unsigned char ABTF:1;
    unsigned char b7:1;
};
//送信バッファnデータ長コード (n = 0,1,2)
struct TXBnDLC_t{
    unsigned char DLC:4;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char RTR:1;
    unsigned char b7:1;
};
//受信バッファ0制御
struct RXB0CTRL_t{
    unsigned char FILHIT:1;
    unsigned char BUKT1:1;
    unsigned char BUKT:1;
    unsigned char RXRTR:1;
    unsigned char RXM:2;
    unsigned char b7:1;
};
//受信バッファ1制御
struct RXB1CTRL_t{
    unsigned char FILHIT:3;
    unsigned char RXRTR:1;
    unsigned char b4:1;
    unsigned char RXM:2;
    unsigned char b7:1;
};
//受信バッファnデータ長コード (n = 0,1)
struct RXBnDLC_t{
    unsigned char DLC:4;
    unsigned char RB:2;
    unsigned char RTR:1;
    unsigned char b7:1;
};
//コンフィギュレーション1
struct CNF1_t{
    unsigned char BRP:6;
    unsigned char SJW:2;
};
//コンフィギュレーション2
struct CNF2_t{
    unsigned char PRSEG:3;
    unsigned char PHSEG1:3;
    unsigned char SAM:1;
    unsigned char BTLMODE:1;
};
//コンフィギュレーション3
struct CNF3_t{
    unsigned char PHSEG2:3;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char WAKFIL:1;
    unsigned char SOF:1;
};
//CANコントロール
struct CANCTRL_t{
    unsigned char CLKPRE:2;
    unsigned char CLKEN:1;
    unsigned char OSM:1;
    unsigned char ABAT:1;
    unsigned char REQOP:3;
};
//CANステータス
struct CANSTAT_t{
    unsigned char b0:1;
    unsigned char ICOD:3;
    unsigned char b4:1;
    unsigned char OPMOD:3;
};
//エラーフラグ
struct EFLG_t{
    unsigned char EWARN:1;
    unsigned char RXWAR:1;
    unsigned char TXWAR:1;
    unsigned char RXEP:1;
    unsigned char TXEP:1;
    unsigned char TXBO:1;
    unsigned char RX0OVR:1;
    unsigned char RX1OVR:1;
};
//割り込み許可
struct CANINTE_t{
    unsigned char RX0IE:1;
    unsigned char RX1IE:1;
    unsigned char TX0IE:1;
    unsigned char TX1IE:1;
    unsigned char TX2IE:1;
    unsigned char ERRIE:1;
    unsigned char WAKIE:1;
    unsigned char MERRE:1;
};
//割り込みフラグ
struct CANINTF_t{
    unsigned char RX0IE:1;  //b1
    unsigned char RX1IE:1;  //b2
    unsigned char TX0IF:1;  //b3
    unsigned char TX1IF:1;  //b4
    unsigned char TX2IF:1;  //b5
    unsigned char ERRIF:1;  //b6
    unsigned char WAKIF:1;  //b7
    unsigned char MERRF:1;  //b8
};

//------------------------------------------------------------------------------
//  MCP2515 SPIコマンド インストラクション
//------------------------------------------------------------------------------

#define SPI_INST_RESET 0xC0 //内部レジスタをデフォルト値にリセットしコンフィギュレーションモードにする
#define SPI_INST_READ 0x03  //選択したアドレスから順にレジスタのデータを読み込む
#define SPI_INST_RXB0_SIDH 0x90 //受信バッファ0からデータを読み込む　SID始まり
#define SPI_INST_RXB0_D0 0x92   //受信バッファ0からデータを読み込む　データ始まり
#define SPI_INST_RXB1_SIDH 0x94 //受信バッファ1からデータを読み込む　SID始まり
#define SPI_INST_RXB1_D0 0x96   //受信バッファ1からデータを読み込む　データ始まり
#define SPI_INST_WRITE 0x02 //選択アドレスから順にレジスタにデータを書き込む
#define SPI_INST_TXB0_SIDH 0x40 //送信バッファ0に書き込む　SID始まり
#define SPI_INST_TXB0_D0 0x41   //送信バッファ0に書き込む　データ始まり
#define SPI_INST_TXB1_SIDH 0x42 //送信バッファ1に書き込む　SID始まり
#define SPI_INST_TXB1_D0 0x43   //送信バッファ1に書き込む　データ始まり
#define SPI_INST_TXB2_SIDH 0x44 //送信バッファ2に書き込む　SID始まり
#define SPI_INST_TXB2_D0 0x45   //送信バッファ2に書き込む　データ始まり
#define SPI_INST_RTS_TXB0 0x81  //TXB0にメッセージ送信要求
#define SPI_INST_RTS_TXB1 0x82  //TXB1にメッセージ送信要求
#define SPI_INST_RTS_TXB2 0x84  //TXB2にメッセージ送信要求
#define SPI_INST_RD_STAT 0xA0   //送受信機能のいくつかの状態ビットを読み込む
#define SPI_INST_RX_STAT 0xB0   //受信メッセージのフィルタ一致とメッセージタイプを読み込む
#define SPI_INST_BITMODIFY 0x05 //ユーザーが特定のビットをセットクリアできるようにする

//------------------------------------------------------------------------------
//  SPI インストラクション RX状態読み込み レジスタ構造
//------------------------------------------------------------------------------

//レジスタ
struct RXSTAT_t{
    unsigned char HITFIL:3; //b2,b1,b0
    unsigned char REMO:1;   //b3
    unsigned char EXTR:1;   //b4
    unsigned char b5:1;       //b5
    unsigned char RXB0:1;   //b6
    unsigned char RXB1:1;   //b7,
}RXSTAT_t;
//ビットナンバー
enum RXSTAT_e{
    HITRXnF = 0,
    DataREMO = 3,
    DataEXTR = 4,
    //b5 = 5;
    MsgRXB0 = 6,
    MsgRXB1 = 7
}RXSTAT_e;

//------------------------------------------------------------------------------
//   mcp2515 レジスタアドレス
//------------------------------------------------------------------------------

//送信バッファ0　レジスタアドレス
#define CAN_ADRS_TXB0_CTRL 0x30
#define CAN_ADRS_TXB0_SIDH 0x31
#define CAN_ADRS_TXB0_SIDL 0x32
#define CAN_ADRS_TXB0_EID8 0x33
#define CAN_ADRS_TXB0_EID0 0x34
#define CAN_ADRS_TXB0_DLC 0x35
//送信バッファ1　レジスタアドレス
#define CAN_ADRS_TXB1_CTRL 0x40
#define CAN_ADRS_TXB1_SIDH 0x41
#define CAN_ADRS_TXB1_SIDL 0x42
#define CAN_ADRS_TXB1_EID8 0x43
#define CAN_ADRS_TXB1_EID0 0x44
#define CAN_ADRS_TXB1_DLC 0x45
//送信バッファ2　レジスタアドレス
#define CAN_ADRS_TXB2_CTRL 0x50
#define CAN_ADRS_TXB2_SIDH 0x51
#define CAN_ADRS_TXB2_SIDL 0x52
#define CAN_ADRS_TXB2_EID8 0x53
#define CAN_ADRS_TXB2_EID0 0x54
#define CAN_ADRS_TXB2_DLC 0x55
//受信バッファ0　レジスタアドレス
#define CAN_ADRS_RXB0_CTRL 0x60
#define CAN_ADRS_RXB0_SIDH 0x61
#define CAN_ADRS_RXB0_SIDL 0x62
#define CAN_ADRS_RXB0_EID8 0x63
#define CAN_ADRS_RXB0_EID0 0x64
#define CAN_ADRS_RXB0_DLC 0x65
//受信バッファ1　レジスタアドレス
#define CAN_ADRS_RXB1_CTRL 0x70
#define CAN_ADRS_RXB1_SIDH 0x71
#define CAN_ADRS_RXB1_SIDL 0x72
#define CAN_ADRS_RXB1_EID8 0x73
#define CAN_ADRS_RXB1_EID0 0x74
#define CAN_ADRS_RXB1_DLC 0x75
//受信フィルタ0　レジスタアドレス
#define CAN_ADRS_RXF0_SIDH 0x00
#define CAN_ADRS_RXF0_SIDL 0x01
#define CAN_ADRS_RXF0_EID8 0x02
#define CAN_ADRS_RXF0_EID0 0x03
//受信フィルタ1　レジスタアドレス
#define CAN_ADRS_RXF1_SIDH 0x04
#define CAN_ADRS_RXF1_SIDL 0x05
#define CAN_ADRS_RXF1_EID8 0x06
#define CAN_ADRS_RXF1_EID0 0x07
//受信フィルタ2　レジスタアドレス
#define CAN_ADRS_RXF2_SIDH 0x08
#define CAN_ADRS_RXF2_SIDL 0x09
#define CAN_ADRS_RXF2_EID8 0x0A
#define CAN_ADRS_RXF2_EID0 0x0B
//受信フィルタ3　レジスタアドレス
#define CAN_ADRS_RXF3_SIDH 0x10
#define CAN_ADRS_RXF3_SIDL 0x11
#define CAN_ADRS_RXF3_EID8 0x12
#define CAN_ADRS_RXF3_EID0 0x13
//受信フィルタ4　レジスタアドレス
#define CAN_ADRS_RXF4_SIDH 0x14
#define CAN_ADRS_RXF4_SIDL 0x15
#define CAN_ADRS_RXF4_EID8 0x16
#define CAN_ADRS_RXF4_EID0 0x17
//受信フィルタ5　レジスタアドレス
#define CAN_ADRS_RXF5_SIDH 0x18
#define CAN_ADRS_RXF5_SIDL 0x19
#define CAN_ADRS_RXF5_EID8 0x1A
#define CAN_ADRS_RXF5_EID0 0x1B
//受信マスク0　レジスタアドレス
#define CAN_ADRS_RXM0_SIDH 0x20
#define CAN_ADRS_RXM0_SIDL 0x21
#define CAN_ADRS_RXM0_EID8 0x22
#define CAN_ADRS_RXM0_EID0 0x23
//受信マスク1　レジスタあどれす
#define CAN_ADRS_RXM1_SIDH 0x24
#define CAN_ADRS_RXM1_SIDL 0x25
#define CAN_ADRS_RXM1_EID8 0x26
#define CAN_ADRS_RXM1_EID0 0x27
//TXnRTS ピン制御と状態レジスタ
#define CAN_ADRS_TXRTS_CTRL 0x0D
//RXnBFピン制御と状態
#define CAN_ADRS_BFP_CTRL 0x0C
//コンフィギュレーション1
#define CAN_ADRS_CNF1 0x2A
//コンフィギュレーション2
#define CAN_ADRS_CNF2 0x29
//コンフィギュレーション3
#define CAN_ADRS_CNF3 0x28
//CANコントロール
#define CAN_ADRS_CANCTRL 0x0F
//CANステータス
#define CAN_ADRS_CANSTAT 0x0E
//割り込み許可
#define CAN_ADRS_CANINTE 0x2B
//割り込みフラグ
#define CAN_ADRS_CANINTF 0x2C
//エラーフラグ
#define CAN_ADRS_EFLG 0x2D
//送信エラーカウンター#include "state.h"
#define CAN_ADRS_TEC 0x1C
//受信エラーカウンター
#define CAN_ADRS_REC 0x1D

//---------------------------------------------------------------------------
//  モード系マクロ
//---------------------------------------------------------------------------

//
#define MASK_SID_ALL 0x0000 //フィルタ値を無視
#define MASK_SID_NOT 0x07FF //フィルタ値と一致のみ
//読み書き
#define WRITE 0
#define READ 1
//CAN動作モード
#define CAN_MODE_ACTIVE 0x00
#define CAN_MODE_SLEEP 0x20
#define CAN_MODE_LOOPBACK 0x40
#define CAN_MODE_LISTEN 0x60
#define CAN_MODE_CONFIG 0x80
//ボーレート設定　20MHz
#define CAN_BRP_20MHZ_1MBPS 0
#define CAN_BRP_20MHZ_500KBPS 1
#define CAN_BRP_20MHZ_250KBPS 3
#define CAN_BRP_20MHZ_125KBPS 7
//受信バッファ動作モード
#define CAN_RXBnCTRL_MODE_ALLON 0
#define CAN_RXBnCTRL_MODE_STDON 1
#define CAN_RXBnCTRL_MODE_EXTON 2
#define CAN_RXBnCTRL_MODE_ALLOFF 3

//------------------------------------------------------------------------------
//   mcp2515 SIDレジスタ操作
//------------------------------------------------------------------------------
#define GET_SIDH(msg) (unsigned char)(msg>>3)
#define GET_SIDL(msg) (unsigned char)(msg<<5)
#define MAKE_SID(sid_h, sid_l) (((unsigned short)sid_h<<3)|((unsigned short)sid_l>>5))

//------------------------------------------------------------------------------
//SPI送受信関数　プロトタイプ宣言
//------------------------------------------------------------------------------
unsigned char SPI_TxRx(unsigned char Tx_data,unsigned char cmd);

//------------------------------------------------------------------------------
//CANドライヴ関数 プロトタイプ宣言
//------------------------------------------------------------------------------
void CANwritereg(unsigned char adrs,unsigned char data);
void CANtxrxbufrw(unsigned char inst,unsigned char readop,unsigned char *data,unsigned char cnt);
void CANbitmodcmd(unsigned char adrs,unsigned char mask,unsigned char data);
void CANsetsidfiltermask(unsigned char adrs,unsigned short sid);
unsigned char CANregread2b(unsigned char inst,unsigned char adrs);
void CANreset(void);
void CANsetrts(unsigned char inst);
unsigned char CANrxcheck(unsigned char rxbnum);
void CANinit(unsigned char brp);

//------------------------------------------------------------------------------
//関数マクロたち
//------------------------------------------------------------------------------

//CANオペレーションモード設定
#define CANSETMODE(mode) CANbitmodcmd(CAN_ADRS_CANCTRL,0xE0,mode)
//ボーレート設定
#define CANSETBRP(mode) CANbitmodcmd(CAN_ADRS_CNF1,0x3F,mode)
//SJW値を設定
#define CANSETSJW(mode) CANbitmodcmd(CAN_ADRS_CNF1,0xC0,mode<<5)
//PHSEG2を手動設定する
#define CANSETBTLMODE() CANbitmodcmd(CAN_ADRS_CNF2,0xC0,0x80)
//PHSEG値を設定
#define CANSETPHSEG(point) CANbitmodcmd(CAN_ADRS_CNF2,0x07,point)
//PHSEG1値を設定
#define CANSETPHSEG1(point) CANbitmodcmd(CAN_ADRS_CNF2,0x38,point<<3)
//PHSEG2値を設定
#define CANSETPHSEG2(point) CANbitmodcmd(CAN_ADRS_CNF3,0x07,point)
//すべての送信バッファ送信停止
#define CANTXBUFOFF() CANbitmodcmd(CAN_ADRS_CANCTRL,0x10,0x10)
//全送信停止の終了
#define CANTXBUFON() CANbitmodcmd(CAN_ADRS_CANCTRL,0x10,0x00)
//レジスタ バイトリード
#define CANREADREG(adrs) CANregread2b(SPI_INST_READ,adrs)
//ステータス・リード
#define CANREADSTAT() CANregread2b(SPI_INST_RD_STAT,0)
//受信ステータス・リード
#define CANREADRXSTAT() CANregread2b(SPI_INST_RX_STAT,0)
//エラーフラグ・リード
#define CANREADEFLG() CANregread2b(SPI_INST_READ,CAN_ADRS_EFLG)
//メッセージ送信要求
#define CANTXB0RTS() CANregread2b(SPI_INST_RTS_TXB0,0)
//TXREQリセット（データ送信前にTXREQを0にする）
#define CANTXB0TXREQRESET() CANbitmodcmd(CAN_ADRS_TXB0_CTRL,0x04,0x00)
//それぞれのバッファ送信要求ピンとして使う（立下りエッジで）（プルアップ必須）
#define CANB0RTSM_ON() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x01,0x01)
#define CANB0RTSM_OFF() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x01,0x00)
#define CANB1RTSM_ON() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x02,0x02)
#define CANB1RTSM_OFF() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x02,0x00)
#define CANB2RTSM_ON() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x04,0x02)
#define CANB2RTSM_OFF() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x04,0x00)
//割り込み許可
#define CANRX0IE_PERMIT() CANbitmodcmd(CAN_ADRS_CANINTE,0x01,0x01)
#define CANRX0IE_FORBID() CANbitmodcmd(CAN_ADRS_CANINTE,0x01,0x00)
#define CANRX1IE_PERMIT() CANbitmodcmd(CAN_ADRS_CANINTE,0x02,0x02)
#define CANRX1IE_FORBID() CANbitmodcmd(CAN_ADRS_CANINTE,0x02,0x00)
#define CANTX0IE_PERMIT() CANbitmodcmd(CAN_ADRS_CANINTE,0x04,0x04)
#define CANTX0IE_FORBID() CANbitmodcmd(CAN_ADRS_CANINTE,0x04,0x00)
#define CANTX1IE_PERMIT() CANbitmodcmd(CAN_ADRS_CANINTE,0x08,0x08)
#define CANTX1IE_FORBID() CANbitmodcmd(CAN_ADRS_CANINTE,0x08,0x00)
#define CANTX2IE_PERMIT() CANbitmodcmd(CAN_ADRS_CANINTE,0x10,0x10)
#define CANTX2IE_FORBID() CANbitmodcmd(CAN_ADRS_CANINTE,0x10,0x00)
#define CANERRIE_PERMIT() CANbitmodcmd(CAN_ADRS_CANINTE,0x20,0x20)
#define CANERRIE_FORBID() CANbitmodcmd(CAN_ADRS_CANINTE,0x20,0x00)
#define CANWAKIE_PERMIT() CANbitmodcmd(CAN_ADRS_CANINTE,0x40,0x40)
#define CANWAKIE_FORBID() CANbitmodcmd(CAN_ADRS_CANINTE,0x40,0x00)
#define CANMERRE_PERMIT() CANbitmodcmd(CAN_ADRS_CANINTE,0x80,0x80)
#define CANMERRE_FORBID() CANbitmodcmd(CAN_ADRS_CANINTE,0x80,0x00)
//割り込みフラグクリア
#define CANRX0IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x01,0x00)
#define CANRX1IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x02,0x00)
#define CANTX0IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x04,0x00)
#define CANTX1IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x08,0x00)
#define CANTX2IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x10,0x00)
#define CANERRIF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x20,0x00)
#define CANWAKIF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x40,0x00)
#define CANMERRF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x80,0x00)
//フィルタモードの設定
#define CANSETFILTERRXB0(mode) CANbitmodcmd(CAN_ADRS_RXB0_CTRL,3<<5,mode<<5)
#define CANSETFILTERRXB1(mode) CANbitmodcmd(CAN_ADRS_RXB1_CTRL,3<<5,mode<<5)
//受信バッファ　シーケンシャルリード
#define CANRXB0BUFMSGREAD(buf) CANtxrxbufrw(SPI_INST_RXB0_SIDH,READ,buf,5)
#define CANRXB1BUFMSGREAD(buf) CANtxrxbufrw(SPI_INST_RXB1_SIDH,READ,buf,5)
#define CANRXB0BUFDATREAD(buf,dlc) CANtxrxbufrw(SPI_INST_RXB0_D0,READ,buf,dlc)
#define CANRXB1BUFDATREAD(buf,dlc) CANtxrxbufrw(SPI_INST_RXB1_D0,READ,buf,dlc)
//送信バッファ　シーケンシャルライト
#define CANTXB0BUFMSGWRITE(buf) CANtxrxbufrw(SPI_INST_TXB0_SIDH,WRITE,buf,5)
#define CANTXB1BUFMSGWRITE(buf) CANtxrxbufrw(SPI_INST_TXB1_SIDH,WRITE,buf,5)
#define CANTXB2BUFMSGWRITE(buf) CANtxrxbufrw(SPI_INST_TXB2_SIDH,WRITE,buf,5)
#define CANTXB0BUFDATWRITE(buf,dlc) CANtxrxbufrw(SPI_INST_TXB0_D0,WRITE,buf,dlc)
#define CANTXB1BUFDATWRITE(buf,dlc) CANtxrxbufrw(SPI_INST_TXB1_D0,WRITE,buf,dlc)
#define CANTXB2BUFDATWRITE(buf,dlc) CANtxrxbufrw(SPI_INST_TXB2_D0,WRITE,buf,dlc)
//フィルタSID設定
#define CANSETSIDFILTER0(sid) CANsetsidfiltermask(CAN_ADRS_RXF0_SIDH,sid)
#define CANSETSIDFILTER1(sid) CANsetsidfiltermask(CAN_ADRS_RXF1_SIDH,sid)
#define CANSETSIDFILTER2(sid) CANsetsidfiltermask(CAN_ADRS_RXF2_SIDH,sid)
#define CANSETSIDFILTER3(sid) CANsetsidfiltermask(CAN_ADRS_RXF3_SIDH,sid)
#define CANSETSIDFILTER4(sid) CANsetsidfiltermask(CAN_ADRS_RXF4_SIDH,sid)
#define CANSETSIDFILTER5(sid) CANsetsidfiltermask(CAN_ADRS_RXF5_SIDH,sid)
//マスクSID設定
#define CANSETSIDMASK0(sid) CANsetsidfiltermask(CAN_ADRS_RXM0_SIDH,sid)
#define CANSETSIDMASK1(sid) CANsetsidfiltermask(CAN_ADRS_RXM1_SIDH,sid)

#endif // MCP2515_H_INCLUDED
