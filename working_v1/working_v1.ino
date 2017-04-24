#include<SoftwareSerial.h>

#define DEBUG true

SoftwareSerial esp8266(2,3);

int s1=4,s2=5,s3=6,s4=7;

void setup()
{
  Serial.begin(9600);    ///////For Serial monitor 
  esp8266.begin(115200); ///////ESP Baud rate
  pinMode(s1,INPUT);    
  pinMode(s2,INPUT);
  pinMode(s3,INPUT);
  pinMode(s4,INPUT);
 
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}

int connectionId;

void loop()
{
  if(esp8266.available())
  {
    /////////////////////Recieving from web browser to toggle led
    if(esp8266.find("+IPD,"))
    {
     delay(5);
     connectionId = esp8266.read()-48;
     if(esp8266.find("pin="))
     { 
     Serial.println("recieving data from web browser");
     int pinNumber = (esp8266.read()-48)*10; 
     pinNumber += (esp8266.read()-48); 
     digitalWrite(pinNumber, !digitalRead(pinNumber));
     }
   
    /////////////////////Sending data to browser
    else
    {
      String webpage = "<h1>Parking Slots</h1>";
      espsend(webpage);
     }

    int p1 = digitalRead(s1);
    int p2 = digitalRead(s2);
    int p3 = digitalRead(s3);
    int p4 = digitalRead(s4);

    String resu = "<h4>Slot 1 is: </h4>";
      espsend(resu);
  
    if(p1 == 1)
    {
      
      String ans = " Occupied ";
      espsend(ans);
    }
    else
    {
      String ans = "<h3> Vacant </h3>";
      espsend(ans);
    }

    /*String res = " \n"; // for additional new line
    espsend(res);*/

    resu = "<h4>Slot 2 is: </h4>";
      espsend(resu);

        if(p2 == 1)
    {
      String ans = " Occupied ";
      espsend(ans);
    }
    else
    {
      String ans = "<h3> Vacant </h3>";
      espsend(ans);
    }

    //String res = " ";
//    espsend(res);

  resu = "<h4>Slot 3 is: </h4>";
      espsend(resu);
      
    if(p3 == 1)
    {
      String ans = " Occupied ";
      espsend(ans);
    }
    else
    {
      String ans = "<h3> Vacant </h3>";
      espsend(ans);
    }

    //String ans = " ";
//    espsend(res);
    resu = "<h4>Slot 4 is: </h4>";
      espsend(resu);
    if(p4 == 3)
    {
     String ans = " Occupied ";
      espsend(ans);
    }
    else
    {
      String ans = "<h3> Vacant </h3>";
      espsend(ans);
    }
       
     String closeCommand = "AT+CIPCLOSE=";  ////////////////close the socket connection////esp command 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     sendData(closeCommand,300,DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug)
            {
                String response = "";
                esp8266.print(command);
                long int time = millis();
                while( (time+timeout) > millis())
                {
                   while(esp8266.available())
                      {
                         char c = esp8266.read(); // read the next character.
                         response+=c;
                      }  
                }
                
                if(debug)
                     {
                     Serial.print(response); //displays the esp response messages in arduino Serial monitor
                     }
                return response;
            }

void espsend(String d)
         {
             String cipSend = " AT+CIPSEND=";
             cipSend += connectionId; 
             cipSend += ",";
             cipSend +=d.length();
             cipSend +="\r\n";
             sendData(cipSend,100,DEBUG);
             sendData(d,100,DEBUG); 
         }
