/*
----------- www.electroensaimada.com ------------
 ------ Tutoriales Arduino y Raspberry Pi --------
 ------ Proyectos Electrónica y Robótica ---------
 -------------------------------------------------
 ------ Tutorial Motor Paso a Paso ---------------
 */

#define STEP 11
#define DIR 12
void setup(){

  pinMode(DIR,OUTPUT); //Indicamos que los pines son de salida
  pinMode(STEP,OUTPUT);

  digitalWrite(DIR,HIGH); //Fijamos una direción


  for(int i=0;i<400;i++){ //Mandamos pulsos al motor en concreto 2000
    digitalWrite(STEP,HIGH);
    delay(1);
    digitalWrite(STEP,LOW);
    delay(1);
  }

  delay(500); //Esperamos medio segundo
  digitalWrite(DIR,LOW); //Cambiamos la dirección de giro

  for(int i=0;i<400;i++){ //Hacemos 2000 pasos en sentido contrario
    digitalWrite(STEP,HIGH);
    delay(1);
    digitalWrite(STEP,LOW);
    delay(1);
  }
}


void loop(){


  //No hacemos nada  para ejecutar el programa de nuevo pulsamos el boton de reset.


}

