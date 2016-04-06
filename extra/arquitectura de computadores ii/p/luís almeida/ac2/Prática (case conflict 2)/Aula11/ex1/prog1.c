#include <detpic32.h>

#define RDSR          0x05
#define WRITE         0x02
#define READ          0x03
#define WREN          0x06
#define WRDIS         0x04
#define EEPROM_CLOCK  500

// declaration of functions
void spi2_setClock(unsigned int);
void spi2_init(void);
char eeprom_readStatus(void);
void eeprom_writeStatusCommand(char);
void eeprom_writeData(int, char);
char eeprom_readData(int);

// declaration of variables
volatile char trash;

// main
int main(void){
  char caracter;
  int address, value;

  spi2_init();
  spi2_setClock(EEPROM_CLOCK);
  while(1){
    // leitura de uma tecla
    caracter = inkey();

    // mode de escrita
    if (caracter == 'W' || caracter == 'w'){
      printStr("\n");
      printStr("Modo de escrita: \n");
      printStr("Insira um endereço: ");
      do{
        address = readInt10();
      }while(address < 0 || address > 511);
      printStr("\nInsira um valor: ");
      value = readInt10();
      eeprom_writeData(address, value);
      printStr("\n");
    
    // modo de leitura
    }else if (caracter == 'R' || caracter == 'r'){
      printStr("\n");
      printStr("Modo de Leitura: \n");
      printStr("Insira um endereço: ");
      do{
        address = readInt10();
      }while(address < 0 || address > 511);
      
      printStr("\nValor em memoria: ");
      printInt10(eeprom_readData(address));
      printStr("\n");
    }
  }
}

void spi2_setClock(unsigned int clock_freq){
  SPI2BRG = (PBCLK + clock_freq)/(2 * clock_freq) - 1;
}

void spi2_init(void){
  SPI2CONbits.ON = 0;                     // Disable SPI module
  SPI2CONbits.CKP = 0;                    // Configure clock idle state as logic level 0
  SPI2CONbits.CKE = 1;                    // Configure the clock active transition: from active state to idle state
  SPI2CONbits.SMP = 0;                    // Configure SPI data input sample phase bit (middle of data output time)
  SPI2CONbits.MODE32 = 0;                 // Configure word length (8 bits)
  SPI2CONbits.MODE16 = 0;
  SPI2CONbits.ENHBUF = 1;                 // Enable Enhanced Buffer mode (this allows the usage of FIFOs RX, TX)
  SPI2CONbits.MSSEN = 1;                  // Enable slave select support (Master Mode Slave Select) 
  SPI2CONbits.MSTEN = 1;                  // Enable Master Mode

  // Clear RX FIFO
  while (SPI2STATbits.SPIRBE == 0){       // while RX FIFO not empty
    trash = SPI2BUF;                      // FIFO and discard read character
  }

  SPI2STATbits.SPIROV = 0;                // clear overflow error flag
  SPI2CONbits.ON = 1;                     // Enable SPI module
}

char eeprom_readStatus(void){
  // Clear RX FIFO
  while (SPI2STATbits.SPIRBE == 0){       // while RX FIFO not empty
    trash = SPI2BUF;                      // FIFO and discard read character
  }
  SPI2STATbits.SPIROV = 0;                // Clear overflow error flag bit

  SPI2BUF = RDSR;                         // Send RDSR command
  SPI2BUF = 0;                            // Send anything so that EEPROM clocks that into SO

  while (SPI2STATbits.SPIBUSY);           // wait while SPI module is working
  trash = SPI2BUF;                        // First char received is garbage (received while sending command)
  return SPI2BUF;                         // Second received character is the STATUS value
}

void eeprom_writeStatusCommand(char command){
  while(eeprom_readStatus() & 0x01);          // Wait while WIP is true (write in progress)
  SPI2BUF = command;                          // Copy "command" value to SPI2BUF (TX FIFO)
  while(SPI2STATbits.SPIBUSY);                // Wait while SPI module is working (SPIBUSY set)
}

void eeprom_writeData(int address, char value){
  
  // apply mask to limit address to 9 bits
  address = address & 0x01FF;

  while ((eeprom_readStatus() & 0x01));   // read STATUS and wait while SPI module is working
  
  eeprom_writeStatusCommand(WREN);        // enable write operations (activate WREN bit STATUS register, using
                                          // eeprom_writeStatusCommand() function)

  // Copy WRITE command and A8 address bit to TX FIFO:
  SPI2BUF = WRITE | ((address & 0x100) >> 5);

  SPI2BUF = (address & 0x0FF);            // copy address (8 LSbits) to the TX FIFO
  SPI2BUF = value;                        // copy value to the TX FIFO
  while (SPI2STATbits.SPIBUSY);           // wait while SPI module is working
}

char eeprom_readData(int address){
  // Clear RX FIFO
  while (SPI2STATbits.SPIRBE == 0){       // while RX FIFO not empty
    trash = SPI2BUF;                      // FIFO and discard read character
  }
  SPI2STATbits.SPIROV = 0;                // Clear overflow error flag bit
  
  // apply mask to limit address to 9 bits
  address = (address & 0x01FF);
  
  while ((eeprom_readStatus() & 0x01));   // read STATUS and wait while SPI module is working
   
  // Copy READ command and A8 address bit to TX FIFO:
  SPI2BUF = READ | ((address & 0x100) >> 5);

  SPI2BUF = (address & 0x0FF);            // copy address (8 LSbits) to the TX FIFO
  SPI2BUF = 0x00;                         // copy any value (e.g.0x00) TX FIFO
  
  while (SPI2STATbits.SPIBUSY);           // wait while SPI module is working
  
  // Read and Discard 2 characters
  int counter = 0;
  while (counter < 2){                    
    trash = SPI2BUF;                      // discard read character
    counter++;
  }

  return SPI2BUF;
}
