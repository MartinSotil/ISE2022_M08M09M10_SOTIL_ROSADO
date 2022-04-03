#include "LCD.h"
#include "Arial12x12.h"


extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI * SPIdrv =& Driver_SPI1;

unsigned char buffer[128];

uint8_t posicion=0;
int r=0;

char frase[25];
char frase2[25];

int i;
int j;

void retardoMicro(void){  //retardo 1us
	for(i=0;i<6;i++); 
}

void retardoMili(void){  //returndo 1ms
	for(j=0;j<6250;j++);
}

//Resetear LCD
void resetDisplay(){
	GPIO_PinWrite(0,8,0); //Reset (activo a nivel bajo)
	retardoMicro();
	GPIO_PinWrite(0,8,1); //Reset
	retardoMili();
}

//Inicializar display
void init(void){

	GPIO_SetDir(0,8,GPIO_DIR_OUTPUT);
	GPIO_SetDir(0,6,GPIO_DIR_OUTPUT);
	GPIO_SetDir(0,18,GPIO_DIR_OUTPUT);
	GPIO_PinWrite(0,8,1);  //reset (deja de hacer reset)
	GPIO_PinWrite(0,6,1);  //A0
	GPIO_PinWrite(0,18,1);  //Cs

	
	//Inicializamos SPI Driver
	SPIdrv->Initialize(NULL);
	
  //Encendemos el periferico SPI a full use
	SPIdrv->PowerControl(ARM_POWER_FULL);
	
	//COnfiguramos el SPI con Master, 8 bits y 20MHz de frecuencia
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB |  ARM_SPI_DATA_BITS(8),20000000); //8 bits, 20MHz
	
	resetDisplay();
	
	LCD_reset();
}     

//Para escribir en el LCD datos
void wr_data(unsigned char data){
	uint32_t numDatos=sizeof(data); //Mide la longitud del dato a introducir
  // Seleccionar CS = 0;
  // Seleccionar A0 = 1;
    GPIO_PinWrite(0,18,0);//Cs a 0
    GPIO_PinWrite(0,6,1);//A0 a 1	
  // Escribir un dato (data) usando la función SPIDrv->Send(…);
    SPIdrv->Send(&data,numDatos); 
	// Seleccionar CS = 1;
    GPIO_PinWrite(0,18,1);//Cs a 1
}

//Para escribir en el LCD comandos
void wr_cmd(unsigned char cmd){
	uint32_t numDatos=sizeof(cmd); //Mide la longitud del clomando a introducir
  // Seleccionar CS = 0; para activar el chip puesto que es activo a niver bajo
	   GPIO_PinWrite(0,18,0);//Cs a 0
  // Seleccionar A0 = 0;
     GPIO_PinWrite(0,6,0);//A0 a 0
  // Escribir un comando (cmd) usando la función SPIDrv->Send(…);
     SPIdrv->Send(&cmd,numDatos);
  // Seleccionar CS = 1;
	   GPIO_PinWrite(0,18,1);//Cs a 1
}

//resetea y configura el lcd contraste etc
void  LCD_reset(void){
		wr_cmd(0xAE);   //Display off
		wr_cmd(0xA2);   //Fija el valor de la relación de la tensión de polarización del LCD a 1/9 
		wr_cmd(0xA0);   //El direccionamiento de la RAM de datos del display es la normal
		wr_cmd(0xC8);   //El scan en las salidas COM es el normal
		wr_cmd(0x22);   //Fija la relación de resistencias interna a 2
		wr_cmd(0x2F);   //Power on
		wr_cmd(0x40);   //Display empieza en la línea 0
		wr_cmd(0xAF);   //Display ON
		wr_cmd(0x81);   //Contraste
		wr_cmd(0x17);   //Valor Contraste
		wr_cmd(0xA4);   //Display all points normal
		wr_cmd(0xA6);   //LCD Display normal
}


//escribe en el lcd

void copy_to_lcd( uint8_t pagina ){
  uint8_t i; 
	wr_cmd(0x00);
	wr_cmd(0x10);
	wr_cmd(0xB0+pagina); //Siendo pagina, las paginas 0,1,2 y 3
	
	for(i=0;i<128;i++){
		wr_data(buffer[i]);
	}
}

int EscribeLetra (uint8_t letra,uint8_t pagina){
	
	uint8_t i, valor,Par_impar;
	uint16_t comienzo = 0;
	// detectamos en que pagina estamos para escribir el valor correcto de la pagina
	if(pagina%2== 0){
	Par_impar=1;
	}else Par_impar=2;
	
	comienzo = 25*(letra - ' ');//Es dónde empezamos a leer la tabla
															//(letra - ' ') Es para que el código ascii de los char comience en 0 y se multiplica por 25 porque cadadato son 25 bytes
	for (i=0; i < 12; i++){ 
		
		valor = Arial12x12[comienzo + i*2 + Par_impar]; //Impares (pagina 0,2) o pares paginas 1,3
		buffer[i + posicion] = valor;
	
	}
	posicion = posicion + Arial12x12[comienzo];
	return 0;
}

void EscribeFrase( char Linea1[] ){
	  int i;
    posicion = 0;
	
		// limpiamos el buffer 
	  for(i=0; i<sizeof(buffer); i++){
		buffer[i]=NULL;  
    
  }
  for(i=0; i<strlen(Linea1); i++){
		EscribeLetra(Linea1[i],0);
  }
	//copio ña pagina al lcd y reinicio la posicion para la siguiente pagina 
	copy_to_lcd(0);
	posicion = 0;
	for(i=0; i<strlen(Linea1); i++){
		EscribeLetra(Linea1[i],1);
  }
		copy_to_lcd(1);
}

void EscribeFrase2( char Linea2[] ){
	int i;
	posicion = 0;
	// limpiamos el buffer 
	 for(i=0; i<sizeof(buffer); i++){
		buffer[i]=NULL;  
    
  }
  for(i= 0; i<strlen(Linea2); i++){
		EscribeLetra(Linea2[i],2);
  }
	//copio la pagina al lcd y reinicio la posicion para la siguiente pagina 
	copy_to_lcd(2);
	posicion = 0;
	for(i= 0; i<strlen(Linea2); i++){
		EscribeLetra(Linea2[i],3);
  }
		copy_to_lcd(3);
}

void pintarHoraPrimeraLinea( int hora, int min, int seg ){
	refrescoPantalla(1);
	for (r = 0; r < 25; r++){
		frase[r]=NULL;
	}
	sprintf( frase, "Hora: %d :%d :%d ",hora,min,seg); //Concatenamos la hora en un unico array de caracteres
	EscribeFrase( frase );
}

void pintarFechaLinea( int dia, int mes, int ano ){
	refrescoPantalla(2);
	for (r = 0; r < 25; r++){
		frase2[r]=NULL;
	}
	sprintf( frase2, "Fecha: %d :%d :%d ",dia,mes,ano); //Concatenamos la hora en un unico array de caracteres
	EscribeFrase2( frase2 );
}


//Practica 4, apartado 4 ** Escribe en el LCD un entero y un numero decimal respectivamente **
void EscribeEntero(int entero){											//Funcion que escribe un nº entero en el LCD{
  char str[24];
	
	sprintf(str, "Prueba valor1: %d", entero);
	 EscribeFrase(str);
}

void EscribeDecimal(float decimal){										//Funcion que escribe un nº del tipo float en el LCD{
	char str2[24];
	sprintf(str2, "Prueba valor2: %f", decimal);
	EscribeFrase2(str2);
}


void refrescoPantalla( int linea ){
    for(i=0;i<128;i++){
      buffer[i]=0x00;
		}
		if(linea==1){
			copy_to_lcd(0);
			copy_to_lcd(1);
		}else if(linea==2){
			copy_to_lcd(2);
			copy_to_lcd(3);
		}
}

