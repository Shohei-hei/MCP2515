#include <stdio.h>
#include "mcp2515.h"
#include "state.h"

void main(void){
    char s_data;

        while(1){

        s_data = 12;

        DATAFLAMEB0(s_data,0x07FF,1,3);
        State_interrupt();
    }
}
