#include <SoftwareSerial.h>
SoftwareSerial A_Serial(5,6);

#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SPI.h>

#define RST_PIN 9
#define SS_PIN 10

volatile byte state = LOW;

const int duration = 100;
unsigned long pre_time = 0;
unsigned long cur_time =0;

int sw= 2;
int bell_pin = 4;

int count=0;
String myStr="";

MFRC522 mfrc(SS_PIN, RST_PIN);

void isr(){
  cur_time = millis();
  if(cur_time - pre_time >= duration)
  {
    digitalWrite(bell_pin,state);
    state = !state;
    Serial.println(state);
    pre_time= cur_time;
    
  }
  
}


 void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  A_Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();
  attachInterrupt(0, isr, RISING);
  pinMode(sw, INPUT);
  pinMode(bell_pin, OUTPUT);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!mfrc.PICC_IsNewCardPresent())
      return;
  if(!mfrc.PICC_ReadCardSerial())
      return;
 
  ++count;
  
  if(mfrc.uid.uidByte[0] == 147 &&
     mfrc.uid.uidByte[1] == 68 &&
     mfrc.uid.uidByte[2] == 47 &&
     mfrc.uid.uidByte[3] == 19 &&
     count ==1)
     {
      Serial.println("Normal Mode");
      A_Serial.println("Normal Mode");

      myStr="Normal Mode";

      if(Serial.available())
      {
        A_Serial.println(myStr);
      }
      delay(300);
     }
  else if(mfrc.uid.uidByte[0] == 147 &&
     mfrc.uid.uidByte[1] == 68 &&
     mfrc.uid.uidByte[2] == 47 &&
     mfrc.uid.uidByte[3] == 19 &&
     count ==2)
     {
      Serial.println("Sleep Mode");
      A_Serial.println("Sleep Mode");
      
      myStr="Sleep Mode";
      if(Serial.available())
      {
        A_Serial.println(myStr);
      }
      delay(300);
     }
     

  else if(mfrc.uid.uidByte[0] == 147 &&
     mfrc.uid.uidByte[1] == 68 &&
     mfrc.uid.uidByte[2] == 47 &&
     mfrc.uid.uidByte[3] == 19 &&
     count ==3)
     {
      Serial.println("Defensive Mode");
      A_Serial.print("Defensive Mode");
      
      myStr="Defensive Mode";
      
      if(Serial.available())
      {
        A_Serial.println(myStr);
      }
      delay(300);
     }
     

  else{
    Serial.println("Access denied");
    A_Serial.print("Access denied");
    
    myStr="Access denied";
    
    if(Serial.available())
      {
        A_Serial.println(myStr);
      }
      delay(300);
     }
  
  
  
  if(count == 3)
  {
    count = 0;
  } 
}
    

  //카드키 읽기
  /*
  Serial.print("Card UID:");
  for(byte i = 0; i<4; i++)
  {
    Serial.print(mfrc.uid.uidByte[i]);
    Serial.print(" ");
  }
  Serial.println();*/
