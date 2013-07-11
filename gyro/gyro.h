//***********************************************************************************
// Macros

// Wait 1+1/3*t [us]
#define WAIT_1_3US(t)                   \
    do{                                 \
        for (int i = 0; i<t; i++)     \
            asm("NOP");                 \
    }while(0)



//***********************************************************************************
// Function prototypes
void GyroInit(void);
void gyroWriteReg(short reg, short val);
void gyroReadReg(short, short *);
void spiWriteByte(short write);
void spiReadByte(short *read, short write);
void multiread(short, short *);
void gyroRead( void );