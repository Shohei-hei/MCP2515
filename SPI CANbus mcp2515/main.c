#include <stdio.h>
#include "mcp2515.h"
#include "state.h"

unsigned int g_send_data;

void main(){

    CANset();
    //CANSETMODE(CAN_MODE_SLEEP);  //スリープモード
    //CANSETMODE(CAN_MODE_LISTEN);  //リッスンオンリーモード
    CANSETMODE(CAN_MODE_LOOPBACK);  //ループバックモード
    //CANSETMODE(CAN_MODE_ACTIVE);  //通常動作モード

        while(1){

        g_send_data = 12;

        REMOTEFLAMEB0(0x0111);
        State_interrupt();
    }
}

