#include <stdio.h>
#include "mcp2515.h"
#include "state.h"

void main(){
    unsigned int send_data;

    //CANSETMODE(CAN_MODE_SLEEP);  //スリープモード
    //CANSETMODE(CAN_MODE_LISTEN);  //リッスンオンリーモード
    //CANSETMODE(CAN_MODE_LOOPBACK);  //ループバックモード
    CANSETMODE(CAN_MODE_ACTIVE);  //通常動作モード

        while(1){

        send_data = 12;

        DATAFLAMEB0(send_data,0x07FF,4,3);
        State_interrupt();
    }
}

