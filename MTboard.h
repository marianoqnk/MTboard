#ifndef MT_BOARD
#define MT_BOARD

#define TECLAS 4


#define ALEGRE 0
#define TRISTE 1
#define NEUTRAL 2
#define PIEDRA 3
#define PAPEL 4
#define TIJERA 5



#define ARRIBA 0
#define DERECHA  1
#define ABAJO 2
#define IZQUIERDA 3

#define VERDE ARRIBA
#define ROJO DERECHA
#define AZUL ABAJO
#define NARANJA IZQUIERDA

//char password[10]="123456";
//char mensaje[32]="N40 24.345 W003 34.345";

class MTboard 
{
  public:
  MTboard();
  ini();
  //LED PULSADORES
  void apagaTodos();
  void enciendeTodos();
  void enciendeLed(int n);
  byte esperaPulsador(int cuanto);
  byte leePulsador();
  //DISPLAY
  void textoScroll(char *cadena);
  void drawPixel(int x, int y,int color);
  void drawBitmap(unsigned char icono);
  void borrar();
  void print(char* cadena);
  void print(int x);
  void atiendeSerie(void);
  void coordenadasShow(void);
  void fijaLed(int n);
  
  private:
  const int puls[TECLAS]={9,8,7,6};
  void iniEEPROM(void);
  uint8_t XORChecksum8(void);
  char password[10]="123456";
  char mensaje[32]="N40 24.345 W003 34.345";
  
};

#endif
