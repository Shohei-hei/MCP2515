#include <stdio.h>
#include "mcp2515.h"
#include "state.h"

void main(){
    unsigned char send_data;

    //CANSETMODE(CAN_MODE_SLEEP);  //�X���[�v���[�h
    //CANSETMODE(CAN_MODE_LISTEN);  //���b�X���I�����[���[�h
    //CANSETMODE(CAN_MODE_LOOPBACK);  //���[�v�o�b�N���[�h
    CANSETMODE(CAN_MODE_ACTIVE);  //�ʏ퓮�샂�[�h

        while(1){

        send_data = 12;

        DATAFLAMEB0(send_data,0x07FF,1,3);
        State_interrupt();
    }
}

