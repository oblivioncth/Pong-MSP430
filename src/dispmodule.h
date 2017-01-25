#ifndef dispmodule   /* Include guard */
#define dispmodule

extern unsigned int iPad1Y;
extern unsigned int iPad2Y;
extern unsigned int iBallX;
extern unsigned int iBallY;

extern unsigned int iPad1YL;
extern unsigned int iPad2YL;
extern unsigned int iBallXL;
extern unsigned int iBallYL;

extern unsigned int iP1Score;
extern unsigned int iP2Score;

void comm_out(char c);
void data_out(unsigned char d);
void clearA1Z(void);
void fnClearElements(void);
void fnRedrawScreen(void);
void fnScoreDisplay(void);
void fnInitializeDisplay(void);

#endif // dispmodule.h
