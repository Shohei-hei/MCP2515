#include <stdio.h>
#include "mcp2515.h"
#include "state.h"

unsigned int g_send_data;

void main(){

    CANset();
    //CANSETMODE(CAN_MODE_SLEEP);  //�X���[�v���[�h
    //CANSETMODE(CAN_MODE_LISTEN);  //���b�X���I�����[���[�h
    CANSETMODE(CAN_MODE_LOOPBACK);  //���[�v�o�b�N���[�h
    //CANSETMODE(CAN_MODE_ACTIVE);  //�ʏ퓮�샂�[�h

        while(1){

        g_send_data = 12;

        REMOTEFLAMEB0(0x0111);
        State_interrupt();
    }
}

