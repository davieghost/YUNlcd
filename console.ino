// ssh root@arduino.local 'telnet localhost 6571'

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Console.h>
#include <LiquidCrystal.h>

YunServer server;

const int numRows = 2;
const int numCols = 16;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String consoleInput;
boolean printed = false;

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW); 
  
  Bridge.begin();
  Console.begin();
  Serial.begin(9600); 

//  server.listenOnLocalhost();
  server.begin();

  Serial.println(F("Server is listening."));
  
  lcd.autoscroll();
  lcd.begin(numCols, 1);
  
  lcd.print("waiting");
  Serial.println("waiting");
  
  while (!Console){
    for(int i = 0; i < 3; i++) {
     lcd.setCursor(7 + i, 1);
     lcd.write(".");
     delay(500);
    }
    delay(500);
  }
  
  lcd.clear();
  lcd.print("connecting");
  Serial.println("connecting");
   for(int i = 0; i < 3; i++){
    lcd.setCursor(10 + i, 1);
    lcd.write(".");
    delay(500);
   }
 lcd.clear();
}

void loop() {

  YunClient client = server.accept();

    if(client) {
      process(client);

      client.stop();
    }
  
  
  if (Console.connected() >= 0) {
    if (printed == false) {
      Serial.println("console ready");
      lcd.print("console ready");  
      printed = true;
      delay(1000);
      lcd.clear();
      
      Console.println("type something for the LCD");
      Serial.println("type something for the LCD");
    }
  }
  if (Console.available() > 0) {
    char c = Console.read();
    if (c == '\n') {
      lcd.print(consoleInput);
      Serial.println(consoleInput);
    }
    else {
      consoleInput += c;
    }
    if (c == '@') {
      lcd.clear();
    }
  }
}

void process(YunClient client) {
  String command = client.readStringUntil('/');
  
  lcd.print(command);
  Serial.println();
  Serial.print(F("New command: "));
  Serial.println(command);
}

