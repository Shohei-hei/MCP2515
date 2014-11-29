#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#define DATAFLAMEB0(buf,id,dlc,rnk) MsgsendB0(buf,id,0,dlc,rnk)
#define REMOTEFLAMEB0(id) MsgsendB0(0,id,1,0)

void Make_data(unsigned char *dat,unsigned char *data,unsigned char cnt);
void print(unsigned char *data,unsigned char dat,unsigned char cnt);
void REC_Errer(void);
void TEC_Errer(void);
void MsgsendB0(unsigned char data,unsigned short MsgID,unsigned char mode,unsigned char DLength,unsigned char rank);
void State_interrupt(void);

#endif // STATE_H_INCLUDED
