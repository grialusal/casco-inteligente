/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x32|64 size display using SPI or I2C to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution

02/18/2013 	Charles-Henri Hallard (http://hallard.me)
						Modified for compiling and use on Raspberry ArduiPi Board
						LCD size and connection are now passed as arguments on 
						the command line (no more #define on compilation needed)
						ArduiPi project documentation http://hallard.me/arduipi

						
*********************************************************************/

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PRG_NAME        "oled_demo"
#define PRG_VERSION     "1.1"

// Instantiate the display
ArduiPi_OLED display;


// Config Option
struct s_opts
{
	int oled;
	int verbose;
} ;

int sleep_divisor = 1 ;
	
// default options values
s_opts opts = {
	OLED_ADAFRUIT_SPI_128x32,	// Default oled
  false										// Not verbose
};

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGOUSAL_HEIGHT 50
#define LOGOUSAL_WIDTH 96
static unsigned char logoUsal[] = 
{
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b01000000, 0b00000000, 0b00001111, 0b11111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b00000000, 0b00111111, 0b11111100, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00000000, 0b00000000, 0b01111110, 0b01111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00111000, 0b00000000, 0b00000001, 0b11111110, 0b01111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11111100, 0b00000000, 0b00011111, 0b11111111, 0b11100111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000001, 0b11000000, 0b00000000, 0b01111110, 0b00111111, 0b11100011, 0b11000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000011, 0b00000000, 0b00000000, 0b11111110, 0b00011111, 0b11100011, 0b11110000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111100, 0b00001111, 0b11100000, 0b11110000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b11110000, 0b00000111, 0b11100000, 0b01111000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000100, 0b00000001, 0b11110000, 0b00000000, 0b00000000, 0b00111100, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00001100, 0b00000001, 0b11110000, 0b00000000, 0b00000000, 0b00011110, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00001000, 0b00000001, 0b11111000, 0b00000000, 0b00000000, 0b00000101, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00000001, 0b11111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00011000, 0b00000001, 0b11111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00110000, 0b00000001, 0b11111100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b00000001, 0b11111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11100000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b01000000, 0b00000000, 0b01111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b01000000, 0b00000000, 0b00111111, 0b11100000, 0b11100000, 0b00000000, 0b00000001, 0b10000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b01000000, 0b00000000, 0b00011111, 0b11110001, 0b10010000, 0b00000000, 0b00000011, 0b10000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000000, 0b00001111, 0b11111001, 0b10000000, 0b00000000, 0b00000011, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000000, 0b00000111, 0b11111111, 0b00000000, 0b00000000, 0b00000010, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000000, 0b00000011, 0b11111111, 0b00000000, 0b00000000, 0b00000010, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000000, 0b00000000, 0b11111111, 0b10000000, 0b00000000, 0b00000110, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000000, 0b00000000, 0b00111111, 0b11000000, 0b00000000, 0b00000110, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000100, 0b00000000, 0b00111111, 0b11000000, 0b00000000, 0b00000110, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00001110, 0b00000000, 0b00011111, 0b11100000, 0b00000000, 0b00000110, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00010000, 0b00000000, 0b00001111, 0b11100000, 0b00000000, 0b00000110, 0b00100000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b11100000, 0b00010000, 0b00000000, 0b00001111, 0b11100000, 0b00000000, 0b00000110, 0b01000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b01110000, 0b00010000, 0b00000000, 0b00001111, 0b11110000, 0b00000000, 0b00000111, 0b11000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b01111000, 0b00000000, 0b00000000, 0b00000111, 0b11110000, 0b00000000, 0b00000111, 0b10000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b01111100, 0b00000000, 0b00000000, 0b00000111, 0b11110000, 0b00000010, 0b00000111, 0b10000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b01011110, 0b00000000, 0b00000000, 0b00000111, 0b11110000, 0b00000011, 0b00000111, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b00010000, 0b00000000, 0b00000011, 0b11100000, 0b00000011, 0b10000111, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11110000, 0b00000000, 0b00000011, 0b11100000, 0b00000001, 0b10001111, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11111110, 0b00000000, 0b00000011, 0b11100000, 0b00000001, 0b11110010, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000110, 0b11000000, 0b00000000, 0b00000111, 0b11100000, 0b01000000, 0b00000011, 0b10000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11110000, 0b00000000, 0b00000111, 0b11100000, 0b00000000, 0b00000001, 0b10000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00110011, 0b00000000, 0b00000111, 0b11110000, 0b00000000, 0b00000001, 0b11000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00001111, 0b10000000, 0b00000000, 0b00000000, 0b11111000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b00111111, 0b10000000, 0b00000000, 0b00000000, 0b00110000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11111110, 0b10100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
};

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static unsigned char logo16_glcd_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.print("\n");
  }    
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.height()/2-15; i+=9) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
  }
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  
  if (opts.oled == OLED_SH1106_I2C_128x64)
    display.print("No scroll\non SH1106");
  else
    display.print("scroll");
  display.display();
 
  display.startscrollright(0x00, 0x0F);
  sleep(2);
  display.stopscroll();
  sleep(1);
  display.startscrollleft(0x00, 0x0F);
  sleep(2);
  display.stopscroll();
  sleep(1);    
  display.startscrolldiagright(0x00, 0x07);
  sleep(2);
  display.startscrolldiagleft(0x00, 0x07);
  sleep(2);
  display.stopscroll();
}


/* ======================================================================
Function: usage
Purpose : display usage
Input 	: program name
Output	: -
Comments: 
====================================================================== */
void usage( char * name)
{
	printf("%s\n", name );
	printf("Usage is: %s --oled type [options]\n", name);
	printf("  --<o>led type\nOLED type are:\n");
	for (int i=0; i<OLED_LAST_OLED;i++)
		printf("  %1d %s\n", i, oled_type_str[i]);
	
	printf("Options are:\n");
	printf("  --<v>erbose  : speak more to user\n");
	printf("  --<h>elp\n");
	printf("<?> indicates the equivalent short option.\n");
	printf("Short options are prefixed by \"-\" instead of by \"--\".\n");
	printf("Example :\n");
	printf( "%s -o 1 use a %s OLED\n\n", name, oled_type_str[1]);
	printf( "%s -o 4 -v use a %s OLED being verbose\n", name, oled_type_str[4]);
}


/* ======================================================================
Function: parse_args
Purpose : parse argument passed to the program
Input 	: -
Output	: -
Comments: 
====================================================================== */
void parse_args(int argc, char *argv[])
{
	static struct option longOptions[] =
	{
		{"oled"	  , required_argument,0, 'o'},
		{"verbose", no_argument,	  	0, 'v'},
		{"help"		, no_argument, 			0, 'h'},
		{0, 0, 0, 0}
	};

	int optionIndex = 0;
	int c;

	while (1) 
	{
		/* no default error messages printed. */
		opterr = 0;

    c = getopt_long(argc, argv, "vho:", longOptions, &optionIndex);

		if (c < 0)
			break;

		switch (c) 
		{
			case 'v': opts.verbose = true	;	break;

			case 'o':
				opts.oled = (int) atoi(optarg);
				
				if (opts.oled < 0 || opts.oled >= OLED_LAST_OLED )
				{
						fprintf(stderr, "--oled %d ignored must be 0 to %d.\n", opts.oled, OLED_LAST_OLED-1);
						fprintf(stderr, "--oled set to 0 now\n");
						opts.oled = 0;
				}
			break;

			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			break;
			
			case '?':
			default:
				fprintf(stderr, "Unrecognized option.\n");
				fprintf(stderr, "Run with '--help'.\n");
				exit(EXIT_FAILURE);
		}
	} /* while */

	if (opts.verbose)
	{
		printf("%s v%s\n", PRG_NAME, PRG_VERSION);
		printf("-- OLED params -- \n");
		printf("Oled is    : %s\n", oled_type_str[opts.oled]);
		printf("-- Other Stuff -- \n");
		printf("verbose is : %s\n", opts.verbose? "yes" : "no");
		printf("\n");
	}	
}

void drawFF (void) {

  display.clearDisplay();
  display.fillRect(display.width()/2-8,20,16,30,WHITE);
  display.fillTriangle(display.width()/2,5,display.width()/2-17,25,display.width()/2+16,25,WHITE);
  display.display();
}

void drawFA (void) {
  int hTercio = display.height()/3;
  display.clearDisplay();
  display.fillRect(display.width()/2-8,5,16,30,WHITE);
  display.fillTriangle(display.width()/2,hTercio*2+10,display.width()/2-17,hTercio*2-10,display.width()/2+17,hTercio*2-10,WHITE);
  display.display();
}

void drawFD (void) {
  int wMitad = display.width()/2;
  int hMitad = display.height()/2;
  int wTercio = display.width()/3;
  display.clearDisplay();
  display.fillRect(wMitad-wTercio+10,hMitad-10,wTercio*2-30,20,WHITE);
  //display.fillTriangle(wTercio*2-20,hMitad-5,wTercio*2-20,hMitad+5,wTercio*2-10,hMitad,WHITE);
  display.fillTriangle(wTercio*2+15,hMitad,wTercio*2-5,hMitad-20,wTercio*2-5,hMitad+20,WHITE);
  display.display();
}

void drawFI (void) {
  int wMitad = display.width()/2;
  int hMitad = display.height()/2;
  int wTercio = display.width()/3;
  display.clearDisplay();
  display.fillRect(wMitad-wTercio+10,hMitad-10,wTercio*2-30,20,WHITE);
  display.fillTriangle(wTercio-25,hMitad,wTercio-5,hMitad-20,wTercio-5,hMitad+20,WHITE);
  display.display();
}

void escribe (char* texto, int tam, int linea){

display.setTextSize(tam);
display.setTextColor(WHITE);
display.setCursor(0,linea);
display.print(texto);
display.display();

}//fin de escribe

void direccion (int distancia, char* dir){

if(strcmp(dir, "frente") == 0){

drawFF();

}else if ((strcmp(dir, "izquierda") == 0) || (strcmp(dir, "oeste") == 0)){

drawFI();

}else if ((strcmp(dir, "derecha") == 0) || (strcmp(dir, "este") == 0)){

drawFD();

}
char entrada [1000];
sprintf(entrada, "Distancia: %d m", distancia); 
escribe (entrada, 1, 55);

}//fin de direccion


int gps (){
FILE * archivo;
char linea[100];
int distancia;
int sleepParcial;
archivo = fopen("direcciones.txt", "r");
if (archivo == NULL){
escribe("Archivo fallido", 1, 20);
}else{
        while(!feof(archivo)){

                fgets(linea, sizeof(linea), archivo);
                char * sentido = strtok(linea, ",");
                char * metros = strtok(NULL, ",");
		char * siesta = strtok(NULL, ",");
                distancia = atoi(metros);
		sleepParcial = atoi(siesta);
                direccion(distancia, sentido);
                sleep(sleepParcial);
		if (sentido == "final")
			return 0;
        }
}
return 1;
}
/* ======================================================================
Function: main
Purpose : Main entry Point
Input 	: -
Output	: -
Comments: 
====================================================================== */
int main(int argc, char **argv)
{
	int i;
	
	// Oled supported display in ArduiPi_SSD1306.h
	// Get OLED type
	parse_args(argc, argv);

	// SPI
	if (display.oled_is_spi_proto(opts.oled))
	{
		// SPI change parameters to fit to your LCD
		if ( !display.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, opts.oled) )
			exit(EXIT_FAILURE);
	}
	else
	{
		// I2C change parameters to fit to your LCD
		if ( !display.init(OLED_I2C_RESET,opts.oled) )
			exit(EXIT_FAILURE);
	}

	display.begin();
	
  // init done
  display.clearDisplay();   // clears the screen  buffer
  display.display();   		// display it (clear display)

	if (opts.oled == 5)
	{
		// showing on this display is very slow (the driver need to be optimized)
		sleep_divisor = 4;

		for(char i=0; i < 12 ; i++)
		{
			display.setSeedTextXY(i,0);  //set Cursor to ith line, 0th column
			display.setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
			display.putSeedString("Hello World"); //Print Hello World
		}
		
		sleep(2);

	}
/*

int p = 149;
while(p<150) {
  drawFF();  
  sleep(2);
  drawFD();
  sleep(2);
  drawFI();
  sleep(2);
  drawFA();
p++;
}

  display.clearDisplay();   // clears the screen  buffer
  display.display();            // display it (clear display)

 display.drawBitmap(0, 0, logoUsal, LOGOUSAL_WIDTH, LOGOUSAL_HEIGHT, WHITE);
 display.display();

*/
gps();

}








