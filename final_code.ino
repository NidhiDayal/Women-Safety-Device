#include <TinyGPS.h>
#include <SoftwareSerial.h>
SoftwareSerial sgps(4, 5);
SoftwareSerial sgsm(2, 3);
TinyGPS gps;
int state = 0;
float lt=12.8245,lon=80.04469; // create variable for latitude and longitude object 
char msg;
char call;

void setup()
{
  sgps.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  sgsm.begin(9600); // connect gps sensor
  Serial.println("GSM SIM900A BEGIN");
  Serial.println("Enter character for control option:");
  Serial.println("h : to disconnect a call");
  Serial.println("i : to receive a call");
  Serial.println("s : to send message");
  Serial.println("c : to make a call");  
  Serial.println("e : to redial");
  Serial.println();
  pinMode(9,INPUT);//BUTTON
  delay(100);
}

void loop()
{  
  if (digitalRead(9) == HIGH && state == 0) {
      SendMessage();
      delay(4000);
      MakeCall();
      state=1;
//      break;
  }
  else
  {
      state=0;
//      Serial.println(",,");
//      break;

  }
 if (sgps.available()>0)
 Serial.write(sgps.read());
}

void SendMessage()
{
  sgsm.println("AT+CMGF=1");    //Sets \ the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  sgsm.println("AT+CMGS=\"+917011997312\"\r"); // Replace x with mobile number
  delay(1000);
   while(sgps.available()){ // check for gps data
    if(gps.encode(sgps.read()))// encode gps data
    { 
    gps.f_get_position(&lt,&lon); // get latitude and longitude
   }
  }
  
  String latitude = String(lt,6);
    String longitude = String(lon,6);
   sgsm.print("Latitude :");
   
      sgsm.println(lt, 6);
      sgsm.print("Longitude:");
      sgsm.println(lon, 6);
      sgsm.print("Latitude :");
   
      Serial.println(lt, 6);
      Serial.print("Longitude:");
      Serial.println(lon, 6);
      delay(1000);
      sgsm.write(0x1A);
      Serial.println("sent");
  delay(100);
   sgsm.println((char)26);// ASCII code of CTRL+Z
   sgsm.write(0x1A);
  delay(1000);
}

void MakeCall()
{
  sgsm.println("ATD+917011997312;"); // ATDxxxxxxxxxx; -- add your number on which you wanna call
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}
