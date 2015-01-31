#include <Button.h>
#include <UnoJoy.h>
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2,3); // RX, TX
char incomingByte;
char controllerInput = '0';         // throw away previous integerdigit2ue
int controllerInputdigit2 = 0;         // throw away previous integerdigit2ue

String combinedData = "";
int byteCount = 0;
int led = 13;
int ledState = 0;

#define numberOfDigits 3
char theNumberString[numberOfDigits + 1]; // used to atoi joyVal value
int joyVal;


boolean triangleOn = false;
boolean circleOn = false;
boolean squareOn = false;
boolean crossOn = false;
boolean dpadUpOn = false;
boolean dpadDownOn = false;
boolean dpadLeftOn = false;
boolean dpadRightOn = false;
boolean l1On = false;
boolean l2On = false;
boolean l3On = false;
boolean r1On = false;
boolean r2On = false;
boolean r3On = false;
boolean selectOn = false;
boolean startOn = false;
boolean homeOn = false;
int leftStickX = 128;
int leftStickY = 128;
int rightStickX = 128;
int rightStickY = 128;
long lastSerialTime = 0;


Button ABTN(LOW);
Button BBTN(LOW);
Button CBTN(LOW);
Button DBTN(LOW);
Button EBTN(LOW);

void setup(){

  setupUnoJoy();

  pinMode(led, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  // Set what the button will be when pressed (default is HIGH)
  // and set the hold time (default is 500)
  ABTN.SetStateAndTime(LOW, 500);
  BBTN.SetStateAndTime(LOW, 500);
  CBTN.SetStateAndTime(LOW, 500);
  DBTN.SetStateAndTime(LOW, 500);
  EBTN.SetStateAndTime(LOW, 500);

  // set the data rate for the SoftwareSerial port
  bluetooth.begin(115200);
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  delay(1000);


}

void loop(){
  checkSwitches();

  while(bluetooth.available()) {

    int numBytes = bluetooth.available();

    if (numBytes > 0 && numBytes < 4 && (millis() - lastSerialTime)>2000) {
      bluetooth.flush();
      forceReset(); 
      lastSerialTime = millis();
      bluetooth.println("flushing bt");

    }

    if (numBytes >= 4) {

      int x = bluetooth.read(); //read first byte
      int digit0 = bluetooth.read(); //read second byte if available;
      int digit1 = bluetooth.read(); //read second byte if available;
      int digit2 = bluetooth.read(); //read second byte if available;
      theNumberString[0] = digit0;
      theNumberString[1] = digit1;
      theNumberString[2] = digit2;
      theNumberString[3] = 0x00;
      lastSerialTime = millis();

      //  'triangle': 'T',
      //  'circle': 'O',
      //  'square': 'S',
      //  'cross': 'X',
      //  'dpadUp': 'U',
      //  'dpadDown': 'D',
      //  'dpadLeft': 'F',
      //  'dpadRight': 'G',
      //  'l1': '[',
      //  'l2': '{',
      //  'l3': '<',
      //  'r1': ']',
      //  'r2': '}',
      //  'r3': '>',    
      //  'select': 'Z',
      //  'start': 'Y',
      //  'home': 'P',

      if (x == '*') {
        bluetooth.flush();
        forceReset(); 
      } 
      else if (x == 'T') {
        triangleOn = (digit2 == '1');  
      } 
      else if (x == 'O') {
        circleOn = (digit2 == '1');  
      } 
      else if (x == 'S') {
        squareOn = (digit2 == '1'); 
      } 
      else if (x == 'X') {
        crossOn = (digit2 == '1'); 
      } 
      else if (x == 'U') {
        dpadUpOn = (digit2 == '1'); 
      } 
      else if (x == 'D') {
        dpadDownOn = (digit2 == '1'); 
      } 
      else if (x == 'F') {
        dpadLeftOn = (digit2 == '1'); 
      } 
      else if (x == 'G') {
        dpadRightOn = (digit2 == '1'); 
      } 
      else if (x == '[') {
        l1On = (digit2 == '1'); 
      } 
      else if (x == '{') {
        l2On = (digit2 == '1'); 
      } 
      else if (x == '<') {
        l3On = (digit2 == '1'); 
      } 
      else if (x == ']') {
        r1On = (digit2 == '1'); 
      } 
      else if (x == '}') {
        r2On = (digit2 == '1'); 
      } 
      else if (x == '>') {
        r3On = (digit2 == '1'); 
      } 
      else if (x == 'Z') {
        selectOn = (digit2 == '1'); 
      } 
      else if (x == 'Y') {
        startOn = (digit2 == '1'); 
      } 
      else if (x == 'P') {
        homeOn = (digit2 == '1');
      }
      else if (x == 'L') {
        leftStickX = atoi(theNumberString);

      } 
      else if (x == 'l') {
        leftStickY = atoi(theNumberString);        
      }
      else if (x == 'R') {
        rightStickX = atoi(theNumberString);

      } 
      else if (x == 'r') {
        rightStickY = atoi(theNumberString);

      }

      ledState = (digit2 == '1');
      // bluetooth.println(digit2);



    }
    break;

  }

  // Always be getting fresh data
  dataForController_t controllerData = getControllerData();

  // Then send out the data over the USB connection
  setControllerData(controllerData);

  digitalWrite(13, ledState);
  delay(50);


}


void checkSwitches() {

  byte A_BTN = ABTN.checkButton(12);
  if (A_BTN)
  {
    switch (A_BTN)
    {
    case PRESSED:
      bluetooth.println("A1");
      break;
    case RELEASED:
      bluetooth.println("A0");
      break;
    default: 
      break;
    }
  }

  byte B_BTN = BBTN.checkButton(11);
  if (B_BTN)
  {
    switch (B_BTN)
    {
    case PRESSED:
      bluetooth.println("B1");
      break;
    case RELEASED:
      bluetooth.println("B0");
      break;
    default: 
      break;
    }
  }

  byte C_BTN = CBTN.checkButton(10);
  if (C_BTN)
  {
    switch (C_BTN)
    {
    case PRESSED:
      bluetooth.println("C1");
      break;
    case RELEASED:
      bluetooth.println("C0");
      break;
    default: 
      break;
    }
  }

  byte D_BTN = DBTN.checkButton(9);
  if (D_BTN)
  {
    switch (D_BTN)
    {
    case PRESSED:
      bluetooth.println("D1");
      break;
    case RELEASED:
      bluetooth.println("D0");
      break;
    default: 
      break;
    }
  }

  byte E_BTN = EBTN.checkButton(8);
  if (E_BTN)
  {
    switch (E_BTN)
    {
    case PRESSED:
      bluetooth.println("E1");
      break;
    case RELEASED:
      bluetooth.println("E0");
      break;
    default: 
      break;
    }
  }

}



dataForController_t getControllerData(void){  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  digit2ues that were in those memory locations before
  dataForController_t controllerData = getBlankDataForController();
  controllerData.triangleOn = triangleOn;
  controllerData.circleOn = circleOn;
  controllerData.squareOn = squareOn;
  controllerData.crossOn = crossOn;
  controllerData.dpadUpOn = dpadUpOn;
  controllerData.dpadDownOn = dpadDownOn;
  controllerData.dpadLeftOn = dpadLeftOn;
  controllerData.dpadRightOn = dpadRightOn;
  controllerData.l1On = l1On;
  controllerData.l2On = l2On;
  controllerData.l3On = l3On;
  controllerData.r1On = r1On;
  controllerData.r2On = r2On;
  controllerData.r3On = r3On;
  controllerData.selectOn = selectOn;
  controllerData.startOn = startOn;
  controllerData.homeOn = homeOn;

  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit digit2ue,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use  // analogRead(A0) >> 2;
  controllerData.leftStickX = leftStickX;
  controllerData.leftStickY = leftStickY;
  controllerData.rightStickX = rightStickX;
  controllerData.rightStickY = rightStickY;

  // And return the data!
  return controllerData;
}

void forceReset() {
  triangleOn = false;
  circleOn = false;
  squareOn = false;
  crossOn = false;
  dpadUpOn = false;
  dpadDownOn = false;
  dpadLeftOn = false;
  dpadRightOn = false;
  l1On = false;
  l2On = false;
  l3On = false;
  r1On = false;
  r2On = false;
  r3On = false;
  selectOn = false;
  startOn = false;
  homeOn = false;
  leftStickX = 128;
  leftStickY = 128;
  rightStickX = 128;
  rightStickY = 128;
}

































