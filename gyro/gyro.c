#include<iocc2540.h>
#include"gyro.h"

short *pval,*junk;
short PressureRead1[18];
short readyptr[6];

void gyro_lowp(void);
void gyro_startmeasure(void);


#define SCK             P1_5
#define MISO            P1_7
#define MOSI            P1_6

#define CS              P0_0


#define CS_DISABLED     1
#define CS_ENABLED      0

short test1,test2,test3;
int i,j;

void main()
{
  
  CLKCONCMD = 0x80; 
    
  WAIT_1_3US(3000); 
  
  GyroInit();
  
  gyro_startmeasure();
  
  gyroReadReg(0x8f, &test1);
  
  gyroRead();
  
  gyro_lowp();
  
  gyroRead();
  
  gyro_lowp();
  
  gyroRead();
  
  __asm("nop;");
  

}

void gyroRead( void )
{
 
 multiread(0xe8,(readyptr)); 
 //multiread(0x28,(PressureRead1+6)); 
}

//brief	Initialize SPI interface and gyro accelerometer


void GyroInit(void)
{
    WAIT_1_3US(1000);
    //*** Setup USART 1 SPI at alternate location 2 ***

    // USART 1 at alternate location 2
    PERCFG |= 0x02;
    // Peripheral function on SCK, MISO and MOSI (P1_5-7)
    P1SEL |= 0xE0;
    // Configure CS (P0_0) output
    P0DIR |= 0x01;
    P2DIR |= 0x01;
    P2 |=0x01;
    
    
    CS = CS_DISABLED;
    
    WAIT_1_3US(2);
    //*** Setup the SPI interface ***
    // SPI master mode
    U1CSR = 0x00;
      // MSB first
    U1GCR = 0xe0;
    // SCK frequency = 480.5kHz (max 500kHz)
    U1GCR |= 15;
    U1BAUD = 0x0;
    

    
  gyroWriteReg(0x22,0x08);
  gyroWriteReg(0x23,0x00);
 
 // while(P1_3!=1);
  for(i=0;i<5000;i++){
    for(j=0;j<50;j++){
    __asm("nop;");
    }
  }
  
}

void gyroWriteReg(short reg, short val)
{
    CS = CS_ENABLED;
    WAIT_1_3US(2);
    spiWriteByte(reg);
    spiWriteByte(val);
    CS = CS_DISABLED;
}


void gyroReadReg(short reg, short *pVal)
{
    CS = CS_ENABLED;
    WAIT_1_3US(2);
    spiWriteByte(reg);
    spiReadByte(pVal, 0xFF);
    CS = CS_DISABLED;
}


void spiWriteByte(short write)
{
        U1CSR &= ~0x02;                 // Clear TX_BYTE
        U1DBUF = write;
        while (!(U1CSR & 0x02));        // Wait for TX_BYTE to be set
}

void spiReadByte(short *read, short write)
{
        U1CSR &= ~0x02;                 // Clear TX_BYTE
        U1DBUF = write;
        while (!(U1CSR & 0x02));        // Wait for TX_BYTE to be set
        *read = U1DBUF;
}
void multiread(short reg, short *pVal) 
{ 
    CS = CS_ENABLED; 
    WAIT_1_3US(2); 
    spiWriteByte(reg); 
    spiReadByte(pVal, 0xFF); 
    spiReadByte(pVal+1, 0xFF); 
    spiReadByte(pVal+2, 0xFF); 
    spiReadByte(pVal+3, 0xFF); 
    spiReadByte(pVal+4, 0xFF); 
    spiReadByte(pVal+5, 0xFF); 
    CS = CS_DISABLED; 
}

void gyro_lowp()
{
 
    gyroWriteReg(0x20,0x00);
    gyroWriteReg(0x20,0x00);
    gyroWriteReg(0x20,0x00);
      
     gyroReadReg(0x20,&test1);
     
  //   printf("reg = %d",test1);
  
}

void gyro_startmeasure()
{

  gyroWriteReg(0x20,0x1f);
  
}