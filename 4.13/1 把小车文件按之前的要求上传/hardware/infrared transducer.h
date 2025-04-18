#ifndef __INFRARED_TRANSDUCER_h
#define __INFRARED_TRANSDUCER_h

void Infared_Transducer_Init(void);
uint8_t Infared_Transducer_GetStatus(char dv);
char Infared_Transducer_Tracking(void);

#endif
