#include <stdio.h>
#include "mcp2515.h"
#include "state.h"

void CANset(void);

int main(){
    char send_data;

        while(1){

        send_data = 12;

        DATAFLAMEB0((unsigned char)send_data,0x07FF,1,3);
        State_interrupt();
    }
    return 0;
}

void CANset(){
    unsigned char i = 0;
    CANSETMODE(CAN_MODE_CONFIG);  //コンフィギュレーションモード

    //ここに10ms待つ操作
    //待って！お願い！

    for(;;){
        i = 0;
        i = CANREADREG(CAN_ADRS_CANSTAT);
        if((i&0xE0) == 0x80) break;
    }
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

    CANSETSIDMASK0(MASK_SID_NOT);   //完全フィルタ一致
    CANSETSIDMASK1(MASK_SID_NOT);   //完全フィルタ一致

    //CANSETMODE(CAN_MODE_LOOPBACK);  //ループバックモード
    CANSETMODE(CAN_MODE_ACTIVE);  //通常動作モード
}
