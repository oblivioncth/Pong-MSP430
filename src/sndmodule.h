#ifndef sndmodule   /* Include guard */
#define sndmodule

void fnPlaySound(int iSound, int iGmeSpd);
void fnInitializeSound();
__interrupt void Timer_A2(void);

#endif // sndmodule.h
