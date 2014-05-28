#define DIR 12
#define STEP 11

int accion, movimiento, cifrasAngulo, angulo, aux, aux2, pasos;

void setup(){
  Serial.begin(9600);
  pinMode(DIR,OUTPUT); //Indicamos que los pines son de salida
  pinMode(STEP,OUTPUT);
}

void loop(){
  
  angulo = 0;
  aux2 = 1;
  
  while(Serial.available() <=0);
  accion = Serial.read()-48; //Convierto el numero del 0 al 9
  if(accion == 1){
    while(Serial.available() <=0);
    movimiento = Serial.read()-48;
    
    while(Serial.available() <=0);
    cifrasAngulo = Serial.read()-48;
    
    Serial.print("Cifras Angulo: ");
    Serial.println(cifrasAngulo);
    
    while(Serial.available() <=0);
    while(cifrasAngulo > 0){
      aux = Serial.read()-48;
      angulo = angulo*aux2 + aux; 
      aux2 *= 10;
      cifrasAngulo--;
    }
    
    if(movimiento == 1)
      digitalWrite(DIR, HIGH);
    else
      digitalWrite(DIR, LOW);
    
    pasos = (angulo*400) / 90; //Regla de 3
    
    for(int i=0;i<pasos;i++){
      digitalWrite(STEP,HIGH);
      delay(1);
      digitalWrite(STEP,LOW);
      delay(1);
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

