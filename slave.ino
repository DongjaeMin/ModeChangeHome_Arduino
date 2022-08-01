#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

volatile byte state = LOW;

SoftwareSerial A_Serial(5,6);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int duration = 100;
unsigned long pre_time = 0;
unsigned long cur_time =0;

int sw= 2;
String myStr = "";
int pir_pin = 7;
int led_pin = 11;
int siren = 13;
char mychar;
int Buzzer = 8;
int bell_pin = 12;

char *send_str[20];
char *asdf = NULL;

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
  A_Serial.begin(9600);
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(pir_pin, INPUT);
  pinMode(sw, INPUT);
  pinMode(siren, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(12, OUTPUT);
  lcd.init();
  lcd.backlight();
  attachInterrupt(0, isr, RISING);
}

void loop(){
  int i=0;
  int j=0;
  char charArr[50];
  String abc="";

  
  while(A_Serial.available()) 
  {
    mychar = A_Serial.read();
    myStr = myStr+mychar;
    delay(5);
  }
  if(!myStr.equals(""))
    {
      Serial.println("input value:"+ myStr);
      
      myStr.toCharArray(charArr, 50); //myStr=Normal Mode
      //string문자를 제공되는 버퍼에 복사
      asdf = strtok(charArr, " ");
      while(asdf != NULL)
      {
        send_str[j++] = asdf;
        asdf = strtok(NULL, " "); 
      }
      j=0;
      abc = send_str[0]; //Normal
      
      while(abc =="Normal")
      {
        analogWrite(led_pin, 255);
        digitalWrite(Buzzer, LOW);
        analogWrite(siren, 0);
        
        while(A_Serial.available()) 
        {
          mychar = A_Serial.read();
          myStr += mychar;
          delay(5);
        }
        if(!myStr.equals(""))
        {
          if(myStr != "Normal Mode")
          {
            myStr="";
            myStr="Sleep Mode";
            abc="";
          }
        }
      }
      
      if(myStr == "Sleep Mode")
      {
        lcd.clear();
        lcd.print("Sleep Mode");
      }
      
      Serial.println("input value:"+ myStr);
      //string문자를 제공되는 버퍼에 복사
      myStr.toCharArray(charArr, 50);
      asdf = strtok(charArr, " ");
      while(asdf != NULL)
      {
        send_str[j++] = asdf;
        asdf = strtok(NULL, " "); 
      }
      j=0;   
      abc = send_str[0];
      
      while(abc == "Sleep")
      {
        int val = digitalRead(pir_pin);
        if(val == HIGH){
          Serial.println("감지 ON");
          analogWrite(led_pin, 35); 
          digitalWrite(Buzzer, LOW);
          analogWrite(siren, 0); 
          delay(3000);
        }
        else if(val == LOW)
        {
          Serial.println("감지 OFF");
          analogWrite(led_pin, 0); 
          digitalWrite(Buzzer, LOW);
          analogWrite(siren, 0);
        }
        while(A_Serial.available()) 
        {
          mychar = A_Serial.read();
          myStr += mychar;
          delay(5);
        }
        if(!myStr.equals(""))
        {
          if(myStr != "Sleep Mode")
          {
            myStr="";
            myStr="Defensive Mode";
            send_str[20]="";
            abc = "";
          }
        } 
      }

      if(myStr =="Defensive Mode")
      {
        lcd.clear();
        lcd.print("Defensive Mode");
      }

      
      Serial.println("input value:"+ myStr);
      myStr.toCharArray(charArr, 50);
      asdf = strtok(charArr, " ");
      while(asdf != NULL)
      {
        send_str[j++] = asdf;
        asdf = strtok(NULL, " "); 
      }
      j=0;
      abc = send_str[0];
      while(abc == "Defensive")
       {
        
        int val = digitalRead(pir_pin);
        if(val == LOW)
        {
          Serial.println("감지 OFF");
          analogWrite(siren, 0); 
          analogWrite(led_pin, 0);  
          digitalWrite(Buzzer, LOW);
        }
        else if(val == HIGH)
        {
          Serial.println("감지 ON");
          for(int i = 0;i<3;i++){
            tone(Buzzer, 3136, 500);
            analogWrite(siren, 255);
            delay(100);   
            analogWrite(siren, 0);
            digitalWrite(Buzzer, LOW);
          }
        }
        
        while(A_Serial.available()) 
        {
          mychar = A_Serial.read();
          myStr += mychar;
          delay(5);
        }
        if(!myStr.equals(""))
        {
          if(myStr != "Defensive Mode")
          {
            myStr="";
            myStr="Normal Mode";
            send_str[20] = "";
            abc="";
          }
        }
      }//defensive
      
      if(myStr =="Normal Mode")
      {
        lcd.clear();
        lcd.print("Normal Mode");
      }
      
    
    }//!mySrt
} //loop  
  
