// link del tp2 SISTEMA DE LUCES - AKATSUKI
//  https://www.tinkercad.com/things/52L2KN5jsgM-grupo-akatsuki-sistema-de-luces/editel?sharecode=NjwwePZPc8MVvOgCJ8jZ3K08LXPjKa4TdAhjuuerunU



#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

//DEFINO LEDS CON SUS RESPECTIVOS PINES
#define LED_POSICION_DELANTERA 8
#define LED_NARANJA_DER 9
#define LED_NARANJA_IZQ 10
#define LED_FRENO 11
#define LED_POSICION_TRASERA 12

//DEFINO LOS BOTONES
#define noBOTON 0
#define GUINIO_IZQ  1
#define GUINIO_DER 2
#define BALIZAS 3
#define FRENO 4
#define LUCES_POSICION 5


// DEFINO UNA CONSTANTE PARA CADA ESTADOS
// DE LAS LUCES DE GUIÑO
#define LUCES_APAGADAS 0
#define LUCES_IZQ_ENCENDIDAS 1
#define LUCES_DER_ENCENDIDAS 2
#define LUCES_BALIZAS_ENCENDIDAS 3

void setup()
{
  pinMode(LED_POSICION_DELANTERA,OUTPUT);
  pinMode(LED_NARANJA_DER,OUTPUT);
  pinMode(LED_NARANJA_IZQ,OUTPUT);
  pinMode(LED_FRENO,OUTPUT);
  pinMode(LED_POSICION_TRASERA,OUTPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,1);
  lcd.print("LUCES:");

}

int botonAntes  = 0;
int millisAntes = 0;
int estadoGuinio          = LUCES_APAGADAS;
int estadoLucesDePosicion = LOW;
int estadoParpadeoGuinio  = LOW;

void loop()
{
  int botonAhora;
  int lecturaFotoresistor;
  lecturaFotoresistor = analogRead(A1); // leo el pin analogico del fotoresistor
  //Serial.println(lecturaFotoresistor);
  botonAhora = leerBotones();

  if(botonAhora != noBOTON && botonAhora != botonAntes)
  {
      switch(botonAhora)
      {
        case GUINIO_IZQ:
        estadoGuinio = estadoLucesGuinio(botonAhora);
        break;

        case GUINIO_DER:
        estadoGuinio = estadoLucesGuinio(botonAhora);
        break;

        case BALIZAS:
        estadoGuinio = estadoLucesGuinio(botonAhora);
        break;

        case LUCES_POSICION:
        estadoLucesDePosicion=!estadoLucesDePosicion;
        break;
      }
  }
  control_lucesGuinio(estadoGuinio);
  control_lucesDePosicion(lecturaFotoresistor, estadoLucesDePosicion);
  control_lucesDeFreno(botonAhora);

  botonAntes = botonAhora;

  delay(50);
}

//*************** FUNCIONES ************

// ESTA FUNCION ES PARA LEER LOS BOTONES
int leerBotones()
{
  int valorA0 = analogRead(A0);//hago la lectura del pin analogico de los botones
  int retorno;
  Serial.println(valorA0);

  if(valorA0 > 500 && valorA0 < 520)
  {
    retorno = GUINIO_IZQ;
  }
   if(valorA0 > 680 && valorA0 < 700)
  {
    retorno = GUINIO_DER;
  }
   if(valorA0 > 760 && valorA0 < 780)
  {
    retorno = BALIZAS;
  }
   if(valorA0 > 810 && valorA0 < 830)
  {
    retorno = FRENO;
  }
   if(valorA0 > 850 && valorA0 < 860)
  {
    retorno = LUCES_POSICION;
  }

  if(valorA0 == 1023)
  {
    retorno = noBOTON;
  }

  return retorno;
}



//ESTA FUNCION ES PARA EL CONTROL DE LUCES DEL FRENO

void control_lucesDeFreno(int botonAhora)
{
  lcd.setCursor(7,0);
  if(botonAhora == FRENO)
  {
      digitalWrite(LED_FRENO, HIGH);
      lcd.print("!! ");
  }
  else
  {
      digitalWrite(LED_FRENO, LOW);
      lcd.print("  ");
  }

}
//ESTA FUNCION ES PARA EL CONTROL DE LUCES DE POSICION
void control_lucesDePosicion(int valorFotoresistor, int estadoLucesDePosicion)
{
  lcd.setCursor(7,1);
  if(valorFotoresistor > 500)
  {
    digitalWrite(LED_POSICION_DELANTERA, !estadoLucesDePosicion);
    digitalWrite(LED_POSICION_TRASERA, !estadoLucesDePosicion);

    if(estadoLucesDePosicion == LOW)
    {
      	lcd.print("ON ");
    }
    else
    {
      	lcd.print("OFF");
    }
  }
  else
  {
    digitalWrite(LED_POSICION_DELANTERA, estadoLucesDePosicion);
    digitalWrite(LED_POSICION_TRASERA, estadoLucesDePosicion);

    if(estadoLucesDePosicion == HIGH)
    {
      	lcd.print("ON ");
    }
    else
    {
      	lcd.print("OFF");
    }
  }
}

//***** CONTROL GUIÑOS  ******

//ESTADOS DE LUCES GUIÑO
int estadoLucesGuinio(int boton)
{
  switch(boton)
  {
    case GUINIO_IZQ:
    if(estadoGuinio != LUCES_IZQ_ENCENDIDAS)
    {
      estadoGuinio = LUCES_IZQ_ENCENDIDAS;
    }
    else
    {
      estadoGuinio = LUCES_APAGADAS;
    }
    break;

    case GUINIO_DER:
    if(estadoGuinio != LUCES_DER_ENCENDIDAS)
    {
      estadoGuinio = LUCES_DER_ENCENDIDAS;
    }
    else
    {
      estadoGuinio = LUCES_APAGADAS;
    }
    break;
    case BALIZAS:
    if(estadoGuinio != LUCES_BALIZAS_ENCENDIDAS)
    {
      estadoGuinio = LUCES_BALIZAS_ENCENDIDAS;
    }
    else
    {
      estadoGuinio = LUCES_APAGADAS;
    }
    break;
  }
  return estadoGuinio;
}

//FUNCION PARA EL CONTROL DE LUCES  GUIÑO POR MEDIO DE LOS BOTONES
void control_lucesGuinio(int estadoGuinio)
{

  unsigned long millisAhora = millis();
  switch(estadoGuinio)
  {
    case LUCES_APAGADAS:
    digitalWrite(LED_NARANJA_IZQ, LOW);
    digitalWrite(LED_NARANJA_DER, LOW);
    lcd.setCursor(0,0);
    lcd.print(' ');
    lcd.setCursor(15,0);
    lcd.print(' ');
    break;

    case GUINIO_IZQ:
    if(millisAhora - millisAntes > 500)
    {
      millisAntes = millisAhora;
      estadoParpadeoGuinio = !estadoParpadeoGuinio;
    }
    digitalWrite(LED_NARANJA_IZQ, estadoParpadeoGuinio); //escribo el estado del pin
    digitalWrite(LED_NARANJA_DER, LOW);
    lcd.setCursor(0,0);
    if(estadoParpadeoGuinio == HIGH)
    {
      lcd.print('<');
    }
    else
    {
    	lcd.print(' ');
    }
    lcd.setCursor(15,0);
    lcd.print(' ');
    break;

    case GUINIO_DER:
    if(millisAhora - millisAntes > 500)
    {
      millisAntes = millisAhora;
      estadoParpadeoGuinio = !estadoParpadeoGuinio;
    }

    digitalWrite(LED_NARANJA_IZQ, LOW);
    digitalWrite(LED_NARANJA_DER, estadoParpadeoGuinio);
    lcd.setCursor(0,0);
    lcd.print(' ');
    lcd.setCursor(15,0);
    if(estadoParpadeoGuinio == HIGH)
    {
      lcd.print('>');
    }
    else
    {
    	lcd.print(' ');
    }
    lcd.setCursor(0,0);
    lcd.print(' ');
    break;

    case BALIZAS:
    if(millisAhora - millisAntes>100)
    {
      millisAntes = millisAhora;
      estadoParpadeoGuinio = !estadoParpadeoGuinio;
    }
    digitalWrite(LED_NARANJA_IZQ, estadoParpadeoGuinio);
    digitalWrite(LED_NARANJA_DER, estadoParpadeoGuinio);
    lcd.setCursor(0,0);
    if(estadoParpadeoGuinio==HIGH)
    {
      lcd.print('<');
    }
    else
    {
    	lcd.print(' ');
    }
    lcd.setCursor(15,0);
    if(estadoParpadeoGuinio == HIGH)
    {
      lcd.print('>');
    }
    else
    {
    	lcd.print(' ');
    }
    break;
  }

}
