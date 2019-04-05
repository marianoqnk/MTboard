#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <EEPROM.h>
#include "MTboard.h"

#define TIEMPO_ONOFF 500
#define LED_ON 0
#define LED_OFF 1

int passwordAdd = 0;   //Location we want the data to be put.
int mensajeAdd=12;
int crcAdd=EEPROM.length()-1;

//char password[10]="123456";
//char mensaje[32]="N40 24.345 W003 34.345";

static const uint8_t PROGMEM
smile_bmp[] =
{ B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
},
neutral_bmp[] =
{ B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10111101,
  B10000001,
  B01000010,
  B00111100
},
frown_bmp[] =
{ B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100
},
piedra_bmp[] =
{ B00000000,
  B00011000,
  B00100100,
  B00100010,
  B01000010,
  B00100010,
  B00011100,
  B00000000
},
papel_bmp[] =
{ B00000000,
  B01111110,
  B01000010,
  B01000010,
  B01000010,
  B01000010,
  B01111110,
  B00000000
},
tijera_bmp[] =
{ B00000000,
  B01000001,
  B00100010,
  B00010100,
  B00001000,
  B00110110,
  B00100010,
  B00000000
};


int numberOfVerticalDisplays = 1;
int numberOfHorizontalDisplays = 1;
int pinCS = 10;
int wait = 100; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

MTboard::MTboard()
{
  Serial.begin(57600);
  for(int n=0;n<TECLAS;n++) pinMode(puls[n],OUTPUT);
  void iniEEPROM();
}

void  MTboard::textoScroll(char *mensaje)
{
  byte i;
  if (i > width * strlen(mensaje) + matrix.width() - 1 - spacer)i = 0; else i++;
  for ( int i = 0 ; i < width * strlen(mensaje) + matrix.width() - 1 - spacer; i++ ) {
    //atiendeSerie();
    matrix.fillScreen(LOW);//borra todo

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < strlen(mensaje) ) {
        matrix.drawChar(x, y, mensaje[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(wait);
  }
}
void MTboard::drawPixel(int x, int y,int color)
{
  matrix.drawPixel(x,y,color);
  matrix.write();
}
void MTboard::drawBitmap(byte icono)
{
  matrix.fillScreen(0);
  switch(icono)
  {
    case ALEGRE:
    matrix.drawBitmap(0, 0, smile_bmp, 8, 8, 1);
    break;
    case TRISTE:
    matrix.drawBitmap(0, 0, frown_bmp, 8, 8, 1);
    break;
    case NEUTRAL:
    matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, 1);
    break; 
    case PIEDRA:
    matrix.drawBitmap(0, 0, piedra_bmp, 8, 8, 1);
    break;
    case PAPEL:
    matrix.drawBitmap(0, 0, papel_bmp, 8, 8, 1);
    break;
    case TIJERA:
    matrix.drawBitmap(0, 0, tijera_bmp, 8, 8, 1);
    break;     
  } 
  matrix.write();

}
byte MTboard::esperaPulsador(int cuanto)
{
long tiempo = millis();
for (int n = 0; n < TECLAS; n++) pinMode(puls[n], INPUT_PULLUP);

  do {
    for (int c = 0; c < TECLAS; c++) //declaro e inicio la variable c simultaneamente
    {
      if (digitalRead(puls[c]) == LOW)
      {
        //Serial.println(c);
        //Serial.println("Pulsa");
        while (digitalRead(puls[c]) == LOW); //Espera a que se suelte el pulsador
        //Serial.println("Suelta");
        for (int n = 0; n < TECLAS; n++) pinMode(puls[n], OUTPUT);
        delay(20);
        return c;
        //if (c==cual)return true; else return false;
      }
    }
  }while ((millis() - tiempo) < cuanto ); // repite por siempre, sale cuando se pulsa una tecla con return
  //Serial.println("no pulsdo");
  for (int n = 0; n < TECLAS; n++) pinMode(puls[n], OUTPUT);
  return 255;
}
byte MTboard::leePulsador()
{

  for (int n = 0; n < TECLAS; n++) pinMode(puls[n], INPUT_PULLUP);
    for (int c = 0; c < TECLAS; c++) //declaro e inicio la variable c simultaneamente
    {
      if (digitalRead(puls[c]) == LOW)
      {
        for (int n = 0; n < TECLAS; n++) pinMode(puls[n], OUTPUT);
        delay(10);
        return c;
      }
    }
     for (int n = 0; n < TECLAS; n++) pinMode(puls[n], OUTPUT);
     return 255;
}

void MTboard::borrar()
{
matrix.fillScreen(0);
matrix.write();
}
void MTboard::enciendeTodos()
{
 for(int n=0;n<TECLAS;n++) digitalWrite(puls[n],LED_ON);
}
void MTboard::apagaTodos()
{
 for(int n=0;n<TECLAS;n++) digitalWrite(puls[n],LED_OFF);
}


void MTboard::print(char *cadena)
{
    Serial.println(cadena);
    matrix.fillScreen(0);
    matrix.setCursor(0,0);
    matrix.print(cadena);
    matrix.write();
}
void MTboard::print(int x)
{
    Serial.println(x);
    matrix.fillScreen(0);
    matrix.setCursor(0,0);
    matrix.print(x);
    matrix.write();
}
void MTboard::enciendeLed(int n){
  digitalWrite(puls[n],LED_ON);
  delay(TIEMPO_ONOFF);
  digitalWrite(puls[n],LED_OFF);
  //delay(TIEMPO_ONOFF);
}
void MTboard::fijaLed(int n){
  digitalWrite(puls[n],LED_ON);
}


uint8_t MTboard::XORChecksum8(void)
{
  uint8_t value = 0;
  for (size_t i = 0; i < EEPROM.length()-1; i++)
  {
    value ^= (uint8_t)EEPROM[i];
  }
  return ~value;
}

void  MTboard::atiendeSerie(void)
{
  static bool passwordOk=false;
  if(Serial.available()>0)
  {
    char buffer[36]="";
    int l=Serial.readBytesUntil('\n', buffer, 32);
    if(passwordOk)
    {
      if(buffer[0]=='m'){
        strncpy(mensaje,buffer+2,l-1);
        Serial.print("Nuevo mensaje: ");Serial.println(mensaje);
        EEPROM.put(mensajeAdd,mensaje);
        EEPROM[crcAdd]=XORChecksum8();
        
        }
      else if(buffer[0]=='p'){
        strncpy(password,buffer+2,l-1);
        Serial.print("Nueva password: ");
        Serial.println(password);
        EEPROM.put(passwordAdd,password);
        EEPROM[crcAdd]=XORChecksum8();
      }else if(strncmp(buffer,password,strlen(password))==0){Serial.println("PASSWORD OK\n");passwordOk=true;}else Serial.println("ERROR\n");
     }else if(strncmp(buffer,password,strlen(password))==0){Serial.println("PASSWORD OK\n");passwordOk=true;}else Serial.println("PASSWORD MAL\n");
    
    
  }  
}
void MTboard::coordenadasShow(void)
{
  textoScroll(mensaje);
}
void MTboard::iniEEPROM(void)
{
  if(XORChecksum8()!=EEPROM[crcAdd])
    {
      Serial.println("Iniciando EEPROM");
      EEPROM.put(passwordAdd, password);
      EEPROM.put(mensajeAdd, mensaje);
      EEPROM[crcAdd]=XORChecksum8();
    }
    EEPROM.get(passwordAdd,password);
    EEPROM.get(mensajeAdd,mensaje);
    Serial.println(XORChecksum8());
    Serial.println(password);
    Serial.println(mensaje);
  
}
