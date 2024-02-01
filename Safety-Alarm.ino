#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

#define GAS A0
#define iIR 4
#define TEMP A2

#define laser 3


#define WARNING 2
#define buzzer 7

void setup() {
 
  Serial.begin(19200);
  while (!Serial)
    ;
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(GAS, INPUT);
  pinMode(iIR,INPUT);
  pinMode(TEMP, INPUT);
  pinMode(WARNING, INPUT);

  
  digitalWrite(WARNING, 0);
}

const int d_gas = 100;
const int d_temp = 30;


void loop() {
  int gas = analogRead(GAS);
  int pass = 0;
  int ID = 0;
  int temp = (analogRead(TEMP) * 0.0432) - 10;
  int IR = digitalRead(iIR);
  
  
  digitalWrite(laser,1);
  if (gas > d_gas + 900) {
    digitalWrite(WARNING, 1);
    digitalWrite(buzzer,1);
    Serial.println("BAD AIR");
  } 
  else {
    digitalWrite(WARNING, 0);
    digitalWrite(buzzer,0);
  }

  if (temp > d_temp + 25) {
    digitalWrite(WARNING, 1);
     digitalWrite(buzzer,1);
     Serial.println("HIGH temperature");
  } else {
    digitalWrite(WARNING, 0);
    digitalWrite(buzzer,0);
  }
  
  if(IR == 1)
  {
    digitalWrite(WARNING, 1);
    digitalWrite(buzzer,1);
    Serial.println("SEcurity breach");
    delay(1000);

  }
  else if (IR == 0)
  {
    digitalWrite(WARNING, 0);
    digitalWrite(buzzer,0);

  }

  
   if(ID == 1 )
  {
    digitalWrite(WARNING, 0);
    digitalWrite(buzzer,0);
    Serial.println("Security system OFF");
    Serial.println("SEcurity system off for 10 seconds");
    digitalWrite(laser,0);
    pass = 0;
    while(pass != 10)
    {
      pass = pass+1;
      Serial.println(pass);
      ID = 0;
    }

  }
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("User with UID: ");
    ID = 1;
    return;
  }

  for (int i = 0; i < 4; ++i) {
    Serial.print(mfrc522.uid.uidByte[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("SEcurity system off for 10 seconds");
  Serial.print("has entered\n");

  delay(10000);
}
