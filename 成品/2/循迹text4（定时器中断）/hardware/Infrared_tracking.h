#ifndef __INFRAREDTRACKING_H
#define __INFRAREDTRACKING_H

//白0黑1
//从左到右依次是L1，L2，R2，R1
void InfraredTracking_Init(void);
uint16_t Get_L1Data(void);
uint16_t Get_L2Data(void);
uint16_t Get_R2Data(void);
uint16_t Get_R1Data(void);

#endif
