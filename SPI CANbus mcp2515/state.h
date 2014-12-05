#ifndef STATE_H
#define STATE_H

#define DATAFLAMEB0(buf,id,dlc,rank) Msgsendb0(buf,id,0,dlc,rank)
#define REMOTEFLAMEB0(id) MsgsendB0(0,id,1,0)

void CANset(void);
void Make_rx(unsigned char *dat,unsigned char *data,unsigned char cnt);
void Make_tx(unsigned char *data,unsigned char dat,unsigned char cnt);
void REC_errer(void);
void TEC_errer(void);
void Msgsendb0(unsigned char data,unsigned short msgid,unsigned char mode,unsigned char dlength,unsigned char rnk);
void State_interrupt(void);

#endif // STATE_H_INCLUDED
