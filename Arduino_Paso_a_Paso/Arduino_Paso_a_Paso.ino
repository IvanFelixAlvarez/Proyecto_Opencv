#define DIR 11
#define STEP 10

int accion, movimiento, cifrasAngulo, angulo, aux, aux2, pasos;

void setup(){
  Serial.begin(9600);
  pinMode(DIR,OUTPUT); //Indicamos que los pines son de salida
  pinMode(STEP,OUTPUT);
}

void loop(){
  
  angulo = 0;
  aux2 = 1;
  
  //Esperamos a ue llegue informacion por el puerto Serial
  while(Serial.available() <=0);
  accion = Serial.read()-48; //Convierto el numero del 0 al 9
  if(accion == 1){ //Se desea mover la cadera
    while(Serial.available() <=0); //Se espera, de nuevo, a que exista informacion en el Serial
    movimiento = Serial.read()-48; //Convierto el numero del 0 al 9
    
    while(Serial.available() <=0); //Esperamos
    cifrasAngulo = Serial.read()-48; //Calculo cuantas cifras tiene el angulo
    
    Serial.print("Cifras Angulo: ");
    Serial.println(cifrasAngulo);
    
    while(Serial.available() <=0); //Esperamos
    while(cifrasAngulo > 0){ //Dadas el numero de cifras calculo el angulo
      aux = Serial.read()-48;
      angulo = angulo*aux2 + aux; 
      aux2 *= 10;
      cifrasAngulo--;
    }
    
    if(movimiento == 1) //Giro para la izquierda
      digitalWrite(DIR, HIGH);
    else
      digitalWrite(DIR, LOW); //Giro para la derecha
    
    pasos = (angulo*400) / 90; //Regla de 3
    
    for(int i=0;i<pasos;i++){ //Me muevo
      digitalWrite(STEP,HIGH);
      delay(100);
      digitalWrite(STEP,LOW);
      delay(100);
    }
    
    Serial.print("Accion: ");
    Serial.println(accion);
    Serial.print("Movimiento: ");
    Serial.println(movimiento);
    Serial.print("Angulo: ");
    Serial.println(angulo);
  }

  delay(1000);
}

