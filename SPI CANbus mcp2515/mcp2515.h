#ifndef MCP2515_H
#define MCP2515_H

//------------------------------------------------------------------------------
//�r�b�g���f�B�t�@�C�Ɋւ���
//�}�X�N�l��"�P"�ɐݒ肷��Ƃ��̒l�͕ύX���邱�Ƃ��o����悤�ɂȂ�
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//   SPI�|�[�g��`
//
//  #define CAN_SPI_CS PORT8.PORT.BIT.B0
//------------------------------------------------------------------------------

//SPI�|�[�g�̑����`
#define CAN_SPI_CS B0//SPI��CS(SS)�s���ɐݒ�
#define CAN_SPI_SO B1//SPI��SO�s���ɐݒ�
#define CAN_SPI_SCK B2//SPI��SCK�s���ɐݒ�

#define CAN_SPI_SI B3//SPI��SI�s���ɐݒ�




//------------------------------------------------------------------------------
//  ���x����
//
//  �������牺�̐ݒ��ύX����Ɠ����Ȃ��Ȃ�\��������܂��B
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  MCP2515 ���W�X�^DATAFLAMEB0(send_data,0x0111,4,3);
//------------------------------------------------------------------------------

//����M�o�b�t�@�����䃌�W�X�^ (n = 0,1,2)
struct TXBnCTRL_t{
    unsigned char TXP:2;
    unsigned char b2:1;
    unsigned char TXREQ:1;
    unsigned char TXERR:1;
    unsigned char MLOA:1;
    unsigned char ABTF:1;
    unsigned char b7:1;
};
//���M�o�b�t�@n�f�[�^���R�[�h (n = 0,1,2)
struct TXBnDLC_t{
    unsigned char DLC:4;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char RTR:1;
    unsigned char b7:1;
};
//��M�o�b�t�@0����
struct RXB0CTRL_t{
    unsigned char FILHIT:1;
    unsigned char BUKT1:1;
    unsigned char BUKT:1;
    unsigned char RXRTR:1;
    unsigned char RXM:2;
    unsigned char b7:1;
};
//��M�o�b�t�@1����
struct RXB1CTRL_t{
    unsigned char FILHIT:3;
    unsigned char RXRTR:1;
    unsigned char b4:1;
    unsigned char RXM:2;
    unsigned char b7:1;
};
//��M�o�b�t�@n�f�[�^���R�[�h (n = 0,1)
struct RXBnDLC_t{
    unsigned char DLC:4;
    unsigned char RB:2;
    unsigned char RTR:1;
    unsigned char b7:1;
};
//�R���t�B�M�����[�V����1
struct CNF1_t{
    unsigned char BRP:6;
    unsigned char SJW:2;
};
//�R���t�B�M�����[�V����2
struct CNF2_t{
    unsigned char PRSEG:3;
    unsigned char PHSEG1:3;
    unsigned char SAM:1;
    unsigned char BTLMODE:1;
};
//�R���t�B�M�����[�V����3
struct CNF3_t{
    unsigned char PHSEG2:3;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
    unsigned char WAKFIL:1;
    unsigned char SOF:1;
};
//CAN�R���g���[��
struct CANCTRL_t{
    unsigned char CLKPRE:2;
    unsigned char CLKEN:1;
    unsigned char OSM:1;
    unsigned char ABAT:1;
    unsigned char REQOP:3;
};
//CAN�X�e�[�^�X
struct CANSTAT_t{
    unsigned char b0:1;
    unsigned char ICOD:3;
    unsigned char b4:1;
    unsigned char OPMOD:3;
};
//�G���[�t���O
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
//���荞�݋���
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
//���荞�݃t���O
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
//  MCP2515 SPI�R�}���h �C���X�g���N�V����
//------------------------------------------------------------------------------

#define SPI_INST_RESET 0xC0 //�������W�X�^���f�t�H���g�l�Ƀ��Z�b�g���R���t�B�M�����[�V�������[�h�ɂ���
#define SPI_INST_READ 0x03  //�I�������A�h���X���珇�Ƀ��W�X�^�̃f�[�^��ǂݍ���
#define SPI_INST_RXB0_SIDH 0x90 //��M�o�b�t�@0����f�[�^��ǂݍ��ށ@SID�n�܂�
#define SPI_INST_RXB0_D0 0x92   //��M�o�b�t�@0����f�[�^��ǂݍ��ށ@�f�[�^�n�܂�
#define SPI_INST_RXB1_SIDH 0x94 //��M�o�b�t�@1����f�[�^��ǂݍ��ށ@SID�n�܂�
#define SPI_INST_RXB1_D0 0x96   //��M�o�b�t�@1����f�[�^��ǂݍ��ށ@�f�[�^�n�܂�
#define SPI_INST_WRITE 0x02 //�I���A�h���X���珇�Ƀ��W�X�^�Ƀf�[�^����������
#define SPI_INST_TXB0_SIDH 0x40 //���M�o�b�t�@0�ɏ������ށ@SID�n�܂�
#define SPI_INST_TXB0_D0 0x41   //���M�o�b�t�@0�ɏ������ށ@�f�[�^�n�܂�
#define SPI_INST_TXB1_SIDH 0x42 //���M�o�b�t�@1�ɏ������ށ@SID�n�܂�
#define SPI_INST_TXB1_D0 0x43   //���M�o�b�t�@1�ɏ������ށ@�f�[�^�n�܂�
#define SPI_INST_TXB2_SIDH 0x44 //���M�o�b�t�@2�ɏ������ށ@SID�n�܂�
#define SPI_INST_TXB2_D0 0x45   //���M�o�b�t�@2�ɏ������ށ@�f�[�^�n�܂�
#define SPI_INST_RTS_TXB0 0x81  //TXB0�Ƀ��b�Z�[�W���M�v��
#define SPI_INST_RTS_TXB1 0x82  //TXB1�Ƀ��b�Z�[�W���M�v��
#define SPI_INST_RTS_TXB2 0x84  //TXB2�Ƀ��b�Z�[�W���M�v��
#define SPI_INST_RD_STAT 0xA0   //����M�@�\�̂������̏�ԃr�b�g��ǂݍ���
#define SPI_INST_RX_STAT 0xB0   //��M���b�Z�[�W�̃t�B���^��v�ƃ��b�Z�[�W�^�C�v��ǂݍ���
#define SPI_INST_BITMODIFY 0x05 //���[�U�[������̃r�b�g���Z�b�g�N���A�ł���悤�ɂ���

//------------------------------------------------------------------------------
//  SPI �C���X�g���N�V���� RX��ԓǂݍ��� ���W�X�^�\��
//------------------------------------------------------------------------------

//���W�X�^
struct RXSTAT_t{
    unsigned char HITFIL:3; //b2,b1,b0
    unsigned char REMO:1;   //b3
    unsigned char EXTR:1;   //b4
    unsigned char b5:1;       //b5
    unsigned char RXB0:1;   //b6
    unsigned char RXB1:1;   //b7,
}RXSTAT_t;
//�r�b�g�i���o�[
enum RXSTAT_e{
    HITRXnF = 0,
    DataREMO = 3,
    DataEXTR = 4,
    //b5 = 5;
    MsgRXB0 = 6,
    MsgRXB1 = 7
}RXSTAT_e;

//------------------------------------------------------------------------------
//   mcp2515 ���W�X�^�A�h���X
//------------------------------------------------------------------------------

//���M�o�b�t�@0�@���W�X�^�A�h���X
#define CAN_ADRS_TXB0_CTRL 0x30
#define CAN_ADRS_TXB0_SIDH 0x31
#define CAN_ADRS_TXB0_SIDL 0x32
#define CAN_ADRS_TXB0_EID8 0x33
#define CAN_ADRS_TXB0_EID0 0x34
#define CAN_ADRS_TXB0_DLC 0x35
//���M�o�b�t�@1�@���W�X�^�A�h���X
#define CAN_ADRS_TXB1_CTRL 0x40
#define CAN_ADRS_TXB1_SIDH 0x41
#define CAN_ADRS_TXB1_SIDL 0x42
#define CAN_ADRS_TXB1_EID8 0x43
#define CAN_ADRS_TXB1_EID0 0x44
#define CAN_ADRS_TXB1_DLC 0x45
//���M�o�b�t�@2�@���W�X�^�A�h���X
#define CAN_ADRS_TXB2_CTRL 0x50
#define CAN_ADRS_TXB2_SIDH 0x51
#define CAN_ADRS_TXB2_SIDL 0x52
#define CAN_ADRS_TXB2_EID8 0x53
#define CAN_ADRS_TXB2_EID0 0x54
#define CAN_ADRS_TXB2_DLC 0x55
//��M�o�b�t�@0�@���W�X�^�A�h���X
#define CAN_ADRS_RXB0_CTRL 0x60
#define CAN_ADRS_RXB0_SIDH 0x61
#define CAN_ADRS_RXB0_SIDL 0x62
#define CAN_ADRS_RXB0_EID8 0x63
#define CAN_ADRS_RXB0_EID0 0x64
#define CAN_ADRS_RXB0_DLC 0x65
//��M�o�b�t�@1�@���W�X�^�A�h���X
#define CAN_ADRS_RXB1_CTRL 0x70
#define CAN_ADRS_RXB1_SIDH 0x71
#define CAN_ADRS_RXB1_SIDL 0x72
#define CAN_ADRS_RXB1_EID8 0x73
#define CAN_ADRS_RXB1_EID0 0x74
#define CAN_ADRS_RXB1_DLC 0x75
//��M�t�B���^0�@���W�X�^�A�h���X
#define CAN_ADRS_RXF0_SIDH 0x00
#define CAN_ADRS_RXF0_SIDL 0x01
#define CAN_ADRS_RXF0_EID8 0x02
#define CAN_ADRS_RXF0_EID0 0x03
//��M�t�B���^1�@���W�X�^�A�h���X
#define CAN_ADRS_RXF1_SIDH 0x04
#define CAN_ADRS_RXF1_SIDL 0x05
#define CAN_ADRS_RXF1_EID8 0x06
#define CAN_ADRS_RXF1_EID0 0x07
//��M�t�B���^2�@���W�X�^�A�h���X
#define CAN_ADRS_RXF2_SIDH 0x08
#define CAN_ADRS_RXF2_SIDL 0x09
#define CAN_ADRS_RXF2_EID8 0x0A
#define CAN_ADRS_RXF2_EID0 0x0B
//��M�t�B���^3�@���W�X�^�A�h���X
#define CAN_ADRS_RXF3_SIDH 0x10
#define CAN_ADRS_RXF3_SIDL 0x11
#define CAN_ADRS_RXF3_EID8 0x12
#define CAN_ADRS_RXF3_EID0 0x13
//��M�t�B���^4�@���W�X�^�A�h���X
#define CAN_ADRS_RXF4_SIDH 0x14
#define CAN_ADRS_RXF4_SIDL 0x15
#define CAN_ADRS_RXF4_EID8 0x16
#define CAN_ADRS_RXF4_EID0 0x17
//��M�t�B���^5�@���W�X�^�A�h���X
#define CAN_ADRS_RXF5_SIDH 0x18
#define CAN_ADRS_RXF5_SIDL 0x19
#define CAN_ADRS_RXF5_EID8 0x1A
#define CAN_ADRS_RXF5_EID0 0x1B
//��M�}�X�N0�@���W�X�^�A�h���X
#define CAN_ADRS_RXM0_SIDH 0x20
#define CAN_ADRS_RXM0_SIDL 0x21
#define CAN_ADRS_RXM0_EID8 0x22
#define CAN_ADRS_RXM0_EID0 0x23
//��M�}�X�N1�@���W�X�^���ǂꂷ
#define CAN_ADRS_RXM1_SIDH 0x24
#define CAN_ADRS_RXM1_SIDL 0x25
#define CAN_ADRS_RXM1_EID8 0x26
#define CAN_ADRS_RXM1_EID0 0x27
//TXnRTS �s������Ə�ԃ��W�X�^
#define CAN_ADRS_TXRTS_CTRL 0x0D
//RXnBF�s������Ə��
#define CAN_ADRS_BFP_CTRL 0x0C
//�R���t�B�M�����[�V����1
#define CAN_ADRS_CNF1 0x2A
//�R���t�B�M�����[�V����2
#define CAN_ADRS_CNF2 0x29
//�R���t�B�M�����[�V����3
#define CAN_ADRS_CNF3 0x28
//CAN�R���g���[��
#define CAN_ADRS_CANCTRL 0x0F
//CAN�X�e�[�^�X
#define CAN_ADRS_CANSTAT 0x0E
//���荞�݋���
#define CAN_ADRS_CANINTE 0x2B
//���荞�݃t���O
#define CAN_ADRS_CANINTF 0x2C
//�G���[�t���O
#define CAN_ADRS_EFLG 0x2D
//���M�G���[�J�E���^�[#include "state.h"
#define CAN_ADRS_TEC 0x1C
//��M�G���[�J�E���^�[
#define CAN_ADRS_REC 0x1D

//---------------------------------------------------------------------------
//  ���[�h�n�}�N��
//---------------------------------------------------------------------------

//
#define MASK_SID_ALL 0x0000 //�t�B���^�l�𖳎�
#define MASK_SID_NOT 0x07FF //�t�B���^�l�ƈ�v�̂�
//�ǂݏ���
#define WRITE 0
#define READ 1
//CAN���샂�[�h
#define CAN_MODE_ACTIVE 0x00
#define CAN_MODE_SLEEP 0x20
#define CAN_MODE_LOOPBACK 0x40
#define CAN_MODE_LISTEN 0x60
#define CAN_MODE_CONFIG 0x80
//�{�[���[�g�ݒ�@20MHz
#define CAN_BRP_20MHZ_1MBPS 0
#define CAN_BRP_20MHZ_500KBPS 1
#define CAN_BRP_20MHZ_250KBPS 3
#define CAN_BRP_20MHZ_125KBPS 7
//��M�o�b�t�@���샂�[�h
#define CAN_RXBnCTRL_MODE_ALLON 0
#define CAN_RXBnCTRL_MODE_STDON 1
#define CAN_RXBnCTRL_MODE_EXTON 2
#define CAN_RXBnCTRL_MODE_ALLOFF 3

//------------------------------------------------------------------------------
//   mcp2515 SID���W�X�^����
//------------------------------------------------------------------------------
#define GET_SIDH(msg) (unsigned char)(msg>>3)
#define GET_SIDL(msg) (unsigned char)(msg<<5)
#define MAKE_SID(sid_h, sid_l) (((unsigned short)sid_h<<3)|((unsigned short)sid_l>>5))

//------------------------------------------------------------------------------
//SPI����M�֐��@�v���g�^�C�v�錾
//------------------------------------------------------------------------------
unsigned char SPI_TxRx(unsigned char Tx_data,unsigned char cmd);

//------------------------------------------------------------------------------
//CAN�h���C���֐� �v���g�^�C�v�錾
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
//�֐��}�N������
//------------------------------------------------------------------------------

//CAN�I�y���[�V�������[�h�ݒ�
#define CANSETMODE(mode) CANbitmodcmd(CAN_ADRS_CANCTRL,0xE0,mode)
//�{�[���[�g�ݒ�
#define CANSETBRP(mode) CANbitmodcmd(CAN_ADRS_CNF1,0x3F,mode)
//SJW�l��ݒ�
#define CANSETSJW(mode) CANbitmodcmd(CAN_ADRS_CNF1,0xC0,mode<<5)
//PHSEG2���蓮�ݒ肷��
#define CANSETBTLMODE() CANbitmodcmd(CAN_ADRS_CNF2,0xC0,0x80)
//PHSEG�l��ݒ�
#define CANSETPHSEG(point) CANbitmodcmd(CAN_ADRS_CNF2,0x07,point)
//PHSEG1�l��ݒ�
#define CANSETPHSEG1(point) CANbitmodcmd(CAN_ADRS_CNF2,0x38,point<<3)
//PHSEG2�l��ݒ�
#define CANSETPHSEG2(point) CANbitmodcmd(CAN_ADRS_CNF3,0x07,point)
//���ׂĂ̑��M�o�b�t�@���M��~
#define CANTXBUFOFF() CANbitmodcmd(CAN_ADRS_CANCTRL,0x10,0x10)
//�S���M��~�̏I��
#define CANTXBUFON() CANbitmodcmd(CAN_ADRS_CANCTRL,0x10,0x00)
//���W�X�^ �o�C�g���[�h
#define CANREADREG(adrs) CANregread2b(SPI_INST_READ,adrs)
//�X�e�[�^�X�E���[�h
#define CANREADSTAT() CANregread2b(SPI_INST_RD_STAT,0)
//��M�X�e�[�^�X�E���[�h
#define CANREADRXSTAT() CANregread2b(SPI_INST_RX_STAT,0)
//�G���[�t���O�E���[�h
#define CANREADEFLG() CANregread2b(SPI_INST_READ,CAN_ADRS_EFLG)
//���b�Z�[�W���M�v��
#define CANTXB0RTS() CANregread2b(SPI_INST_RTS_TXB0,0)
//TXREQ���Z�b�g�i�f�[�^���M�O��TXREQ��0�ɂ���j
#define CANTXB0TXREQRESET() CANbitmodcmd(CAN_ADRS_TXB0_CTRL,0x04,0x00)
//���ꂼ��̃o�b�t�@���M�v���s���Ƃ��Ďg���i������G�b�W�Łj�i�v���A�b�v�K�{�j
#define CANB0RTSM_ON() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x01,0x01)
#define CANB0RTSM_OFF() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x01,0x00)
#define CANB1RTSM_ON() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x02,0x02)
#define CANB1RTSM_OFF() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x02,0x00)
#define CANB2RTSM_ON() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x04,0x02)
#define CANB2RTSM_OFF() CANbitmodcmd(CAN_ADRS_BFP_CTRL,0x04,0x00)
//���荞�݋���
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
//���荞�݃t���O�N���A
#define CANRX0IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x01,0x00)
#define CANRX1IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x02,0x00)
#define CANTX0IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x04,0x00)
#define CANTX1IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x08,0x00)
#define CANTX2IF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x10,0x00)
#define CANERRIF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x20,0x00)
#define CANWAKIF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x40,0x00)
#define CANMERRF_CLEAR() CANbitmodcmd(CAN_ADRS_CANINTF,0x80,0x00)
//�t�B���^���[�h�̐ݒ�
#define CANSETFILTERRXB0(mode) CANbitmodcmd(CAN_ADRS_RXB0_CTRL,3<<5,mode<<5)
#define CANSETFILTERRXB1(mode) CANbitmodcmd(CAN_ADRS_RXB1_CTRL,3<<5,mode<<5)
//��M�o�b�t�@�@�V�[�P���V�������[�h
#define CANRXB0BUFMSGREAD(buf) CANtxrxbufrw(SPI_INST_RXB0_SIDH,READ,buf,5)
#define CANRXB1BUFMSGREAD(buf) CANtxrxbufrw(SPI_INST_RXB1_SIDH,READ,buf,5)
#define CANRXB0BUFDATREAD(buf,dlc) CANtxrxbufrw(SPI_INST_RXB0_D0,READ,buf,dlc)
#define CANRXB1BUFDATREAD(buf,dlc) CANtxrxbufrw(SPI_INST_RXB1_D0,READ,buf,dlc)
//���M�o�b�t�@�@�V�[�P���V�������C�g
#define CANTXB0BUFMSGWRITE(buf) CANtxrxbufrw(SPI_INST_TXB0_SIDH,WRITE,buf,5)
#define CANTXB1BUFMSGWRITE(buf) CANtxrxbufrw(SPI_INST_TXB1_SIDH,WRITE,buf,5)
#define CANTXB2BUFMSGWRITE(buf) CANtxrxbufrw(SPI_INST_TXB2_SIDH,WRITE,buf,5)
#define CANTXB0BUFDATWRITE(buf,dlc) CANtxrxbufrw(SPI_INST_TXB0_D0,WRITE,buf,dlc)
#define CANTXB1BUFDATWRITE(buf,dlc) CANtxrxbufrw(SPI_INST_TXB1_D0,WRITE,buf,dlc)
#define CANTXB2BUFDATWRITE(buf,dlc) CANtxrxbufrw(SPI_INST_TXB2_D0,WRITE,buf,dlc)
//�t�B���^SID�ݒ�
#define CANSETSIDFILTER0(sid) CANsetsidfiltermask(CAN_ADRS_RXF0_SIDH,sid)
#define CANSETSIDFILTER1(sid) CANsetsidfiltermask(CAN_ADRS_RXF1_SIDH,sid)
#define CANSETSIDFILTER2(sid) CANsetsidfiltermask(CAN_ADRS_RXF2_SIDH,sid)
#define CANSETSIDFILTER3(sid) CANsetsidfiltermask(CAN_ADRS_RXF3_SIDH,sid)
#define CANSETSIDFILTER4(sid) CANsetsidfiltermask(CAN_ADRS_RXF4_SIDH,sid)
#define CANSETSIDFILTER5(sid) CANsetsidfiltermask(CAN_ADRS_RXF5_SIDH,sid)
//�}�X�NSID�ݒ�
#define CANSETSIDMASK0(sid) CANsetsidfiltermask(CAN_ADRS_RXM0_SIDH,sid)
#define CANSETSIDMASK1(sid) CANsetsidfiltermask(CAN_ADRS_RXM1_SIDH,sid)

#endif // MCP2515_H_INCLUDED
