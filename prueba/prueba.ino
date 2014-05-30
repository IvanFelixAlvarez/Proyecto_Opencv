#define DIR 11
#define STEP 10

void setup(){
  Serial.begin(9600);
  pinMode(DIR,OUTPUT); //Indicamos que los pines son de salida
  pinMode(STEP,OUTPUT);
}

void loop(){

    digitalWrite(DIR, HIGH);
  
    for(int i=0;i<400;i++){
      digitalWrite(STEP,HIGH);
      delay(4);
      digitalWrite(STEP,LOW);
      delay(4);
    }
    
    digitalWrite(DIR, LOW);
    for(int i=0; i<400; i++){
      digitalWrite(STEP,HIGH);
      delay(4);
      digitalWrite(STEP,LOW);
      delay(4);
    }
   
}

