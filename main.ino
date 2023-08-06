/*
####
IEEE Project for NAAC
Smart Traffic Management System.
####
*/

//Including Libraries for LCD Diaplay
#include <LiquidCrystal.h>

//Including libraries for RFID Tag
#include <SPI.h>
#include <MFRC522.h>

//Defining Pins for LCD Display
const int rs = 43, en = 41, d4 = 39, d5 = 37, d6 = 35, d7 = 33;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Defining Pins for Ultrasonic Sensor
#define Echo_U1 27 
#define Trig_U1 29 

#define Echo_U2 13 
#define Trig_U2 12 


#define Echo_U3 7 
#define Trig_U3 6 


#define Echo_U4 11 
#define Trig_U4 10 


#define Echo_U5 8 
#define Trig_U5 9 

//Defining Variables for Ultrasonic Sensor
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

//Defining Pins for RFID Tag
#define SS_RFID1 22
#define RST_RFID1 23
MFRC522 rfid1(SS_RFID1, RST_RFID1);   // Create rfid1 instance.

#define SS_RFID2 24
#define RST_RFID2 25
MFRC522 rfid2(SS_RFID2, RST_RFID2);   // Create rfid2 instance.

//Defining Pins for Traffic Lights
#define R1 44
#define R2 42
#define R3 49
#define R4 36

#define O1 48
#define O2 40
#define O3 47
#define O4 34

#define G1 46
#define G2 38
#define G3 45
#define G4 32

//Defining Variables
int ActiveLane=1;
char Lanes[5][3] = {{R2,R3,R4},{R1,O1,G1},{R2,O2,G2},{R3,O3,G3},{R4,O4,G4}};
int Lane=0;
int ActiveTime=0;
bool Emergency1 = false;
bool Emergency2 = false;
bool Density2 = false;
bool RoundRobin = false;
bool RoundRobin2 = false;
bool Orange = true;

//Functions for LCD Display
void LCD_Scroll(String Msg, int row)
{
    lcd.setCursor(16,row);
    lcd.autoscroll();
    for(int i=0; i<Msg.length(); i++)
    {
        lcd.print(Msg[i]);
        delay(150);
    }
}
void LCD_Ready(String Msg, int row)
{
    lcd.setCursor(0, row);
    lcd.print(Msg);
}

//Functions for Ultrasonic Sensor
int dist(int Echo_U, int Trig_U, int S)
{
    // Clears the Trig_U1 condition
    digitalWrite(Trig_U, LOW);
    delayMicroseconds(2);
    // Sets the Trig_U1 HIGH (ACTIVE) for 10 microseconds
    digitalWrite(Trig_U, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig_U, LOW);
    // Reads the Echo_U1, returns the sound wave travel time in microseconds
    duration = pulseIn(Echo_U, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    // Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm on Lane");
    Serial.println(S);

    return distance;
}

void DensityA()
{
    int U1 = dist(Echo_U1, Trig_U1, 1);
    int U2 = dist(Echo_U2, Trig_U2, 2);
    int U3 = dist(Echo_U3, Trig_U3, 3);
    int U4 = dist(Echo_U4, Trig_U4, 4);
    int U5 = dist(Echo_U5, Trig_U5, 5);
    if(U5 < 10)
    {
        RoundRobin2 = true;
        RoundRobin = true;
        Serial.println("RoundRobin2");
        return;
    }
    else if(U1 < 10 || U2 < 10 || U3 < 10 || U4 < 10)
    {
        RoundRobin = true;
        Serial.println("RoundRobin");
        return;
    }
    else{
        RoundRobin = false;
    }

}


//Functions for RFID System
bool Emergency(MFRC522 rfid)
{
    // Look for new cards
    if ( ! rfid.PICC_IsNewCardPresent()) 
    {
        Serial.println("No Card Nearby!");

        return false;
    }
    // Select one of the cards
    if ( ! rfid.PICC_ReadCardSerial()) 
    {
      Serial.println("No Card!");
        return false;
    }
    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < rfid.uid.size; i++) 
    {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
        content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(rfid.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "3C 08 8B 17") //change here the UID of the card/cards that you want to give access
    {
        Serial.println("Authorized access");
        Serial.println();
        delay(3000);
        return true;
    }
    
    else   {
        Serial.println(" Access denied");
        delay(3000);
        return false;
    }
}

//Functions for Traffic Lights
void TrafficLights()
{
    //Testing Lane 1
    lcd.clear();
    Serial.println("Testing Lane 1");
    LCD_Ready("Testing Lane 1",0);
    digitalWrite(R1, HIGH);
    delay(200);
    digitalWrite(O1, HIGH);
    delay(200);
    digitalWrite(G1, HIGH);
    delay(200);

    digitalWrite(R1, LOW);
    digitalWrite(O1, LOW);
    digitalWrite(G1, LOW);

    //Testing Lane 2
    lcd.clear();
    Serial.println("Testing Lane 2");
    LCD_Ready("Testing Lane 2",0);
    digitalWrite(R2, HIGH);
    delay(200);
    digitalWrite(O2, HIGH);
    delay(200);
    digitalWrite(G2, HIGH);
    delay(200);

    digitalWrite(R2, LOW);
    digitalWrite(O2, LOW);
    digitalWrite(G2, LOW);

    //Testing Lane 3
    lcd.clear();
    Serial.println("Testing Lane 3");
    LCD_Ready("Testing Lane 3",0);
    digitalWrite(R3, HIGH);
    delay(200);
    digitalWrite(O3, HIGH);
    delay(200);
    digitalWrite(G3, HIGH);
    delay(200);

    digitalWrite(R3, LOW);
    digitalWrite(O3, LOW);
    digitalWrite(G3, LOW);

    //Testing Lane 1
    lcd.clear();
    Serial.println("Testing Lane 4");
    LCD_Ready("Testing Lane 4",0);
    digitalWrite(R4, HIGH);
    delay(200);
    digitalWrite(O4, HIGH);
    delay(200);
    digitalWrite(G4, HIGH);
    delay(200);

    digitalWrite(R4, LOW);
    digitalWrite(O4, LOW);
    digitalWrite(G4, LOW);
}

void TurnGreen2(int R, int O)
{
    int time = 9;
        for(int k=1; k<=4; k++){
            if(k != R)
            {
                digitalWrite(Lanes[k][2], LOW);
                digitalWrite(Lanes[k][1], LOW);
                digitalWrite(Lanes[k][0], HIGH);
            }
        }
        digitalWrite(Lanes[O][0], LOW);
        digitalWrite(Lanes[O][1], HIGH);
        digitalWrite(Lanes[O][2], LOW);
        for(int t = 2;t>0;t--)
        {
            lcd.clear();
            LCD_Ready("Emergency Sit.",0);
            String S1="Time Left ";
            LCD_Ready(S1,1);
            lcd.print(t);
            delay(1000);
            
        }

        digitalWrite(Lanes[O][0], HIGH);
        digitalWrite(Lanes[O][1], LOW);
        digitalWrite(Lanes[O][2], LOW);

        digitalWrite(Lanes[R][0], LOW);
        digitalWrite(Lanes[R][1], LOW);
        digitalWrite(Lanes[R][2], HIGH);

        for(int t = 9;t>0;t--)
        {
            lcd.clear();
            LCD_Ready("Emergency Sit.",0);
            String S1="Time Left ";
            LCD_Ready(S1,1);
            lcd.print(time);
            delay(1000);
            time--;
            
        }
        ActiveTime += 90;
        RoundRobin = true;
        RoundRobin2 = false;

}

void TurnGreen(int R, int O)
{
    int time = 0;
    if ( R != O)
    {

        for(int k=1; k<=4; k++){
            if(k != R)
            {
                digitalWrite(Lanes[k][2], LOW);
                digitalWrite(Lanes[k][1], LOW);
                digitalWrite(Lanes[k][0], HIGH);
            }
        }
        digitalWrite(Lanes[O][0], LOW);
        digitalWrite(Lanes[O][1], HIGH);
        digitalWrite(Lanes[O][2], LOW);
        
        for(int t = 2;t>0;t--)
        {
            lcd.clear();
            LCD_Ready("Changing Lane...",0);
            String S1="Time Left ";
            LCD_Ready(S1,1);
            lcd.print(t);
            delay(1000);
            
        }
        

        digitalWrite(Lanes[O][0], HIGH);
        digitalWrite(Lanes[O][1], LOW);
        digitalWrite(Lanes[O][2], LOW);

        digitalWrite(Lanes[R][0], LOW);
        digitalWrite(Lanes[R][1], LOW);
        digitalWrite(Lanes[R][2], HIGH);

        Serial.print("Activating Lane ");
        Serial.println(R);
    }
    lcd.clear();
    if(RoundRobin2 == true && ActiveLane == 3)
    {
        LCD_Ready("Denser Traffic",0);
        time = 12;
    }
    else if(RoundRobin2 == true)
    {
        LCD_Ready("Denser Traffic",0);
        time = 9;
    }
    else{
        LCD_Ready("Normal Traffic",0);
        time = 9;
    }
    
    if(ActiveTime >= 90)
    {
        return;
    }
    if(Emergency(rfid1))
    {
        Emergency1 = true;
        return;
    }
    if(Emergency(rfid2))
    {
        Emergency2 = true;
        return;
    }
    for(int t = 3;t>0;t--)
        {
            
            String S1="Time Left ";
            LCD_Ready(S1,1);
            lcd.print(time);
            delay(1000);
            time--;
            
        }
    ActiveTime += 30;
    if(ActiveTime >= 90)
    {
        return;
    }
    if(Emergency(rfid1))
    {
        Emergency1 = true;
        return;
    }
    if(Emergency(rfid2))
    {
        Emergency2 = true;
        return;
    }
    for(int t = 3;t>0;t--)
        {
            String S1="Time Left ";
            LCD_Ready(S1,1);
            lcd.print(time);
            lcd.print(" ");
            delay(1000);
            time--;
            
        }
    ActiveTime += 30;
    if(ActiveTime >= 90)
    {
        return;
    }
    if(Emergency(rfid1))
    {
        Emergency1 = true;
        return;
    }
    if(Emergency(rfid2))
    {
        Emergency2 = true;
        return;
    }
    for(int t = 3;t>0;t--)
        {
            String S1="Time Left ";
            LCD_Ready(S1,1);
            lcd.print(time);
            lcd.print(" ");
            delay(1000);
            time--;
            
        }
    ActiveTime += 30;
    if(RoundRobin2 == true && ActiveLane == 3)
    {
        if(Emergency(rfid1))
        {
            Emergency1 = true;
            return;
        }
        if(Emergency(rfid2))
        {
            Emergency2 = true;
            return;
        }
        for(int t = 3;t>0;t--)
        {
            String S1="Time Left ";
            LCD_Ready(S1,1);
            lcd.print(time);
            lcd.print(" ");
            delay(1000);
            time--;
            
        }
        ActiveTime += 30;
        RoundRobin2 = false;
        RoundRobin = true;
    }
    return;
}

//Running Setup...
void setup() {
    Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed

    //Initialising LCD Display
    lcd.begin(16, 4); // set up the LCD's number of columns and rows
    Serial.println("Smart Traffic Management System");
    LCD_Ready("Smart Traffic",0);
    LCD_Ready("Management System",1);
    delay(500);
    lcd.clear();
    Serial.println("LCD Display Ready!");
    LCD_Ready("LCD Display",0);
    LCD_Ready("Ready!",1);
    delay(500);
    lcd.clear();
    

    //Defining I/O for Ultrasonic Sensors
    pinMode(Trig_U1, OUTPUT);
    pinMode(Echo_U1, INPUT);

    pinMode(Trig_U2, OUTPUT);
    pinMode(Echo_U2, INPUT);

    pinMode(Trig_U3, OUTPUT);
    pinMode(Echo_U3, INPUT);

    pinMode(Trig_U4, OUTPUT);
    pinMode(Echo_U4, INPUT);

    pinMode(Trig_U5, OUTPUT);
    pinMode(Echo_U5, INPUT);
    Serial.println("Ultrasonic Sensors Ready!");
    LCD_Ready("Ultrasonic Sensors",0);
    LCD_Ready("Ready!",1);
    delay(500);
    lcd.clear();

    //Initialising RFID System
    SPI.begin();      // Initiate  SPI bus
    rfid1.PCD_Init();   // Initiate rfid1
    rfid2.PCD_Init();   // Initiate rfid2
    Serial.println("RFID System Ready!");
    LCD_Ready("RFID System",0);
    LCD_Ready("Ready!",1);
    delay(500);
    lcd.clear();

    //Initialising Traffic Lights
    pinMode(R1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, OUTPUT);

    pinMode(O1, OUTPUT);
    pinMode(O2, OUTPUT);
    pinMode(O3, OUTPUT);
    pinMode(O4, OUTPUT);

    pinMode(G1, OUTPUT);
    pinMode(G2, OUTPUT);
    pinMode(G3, OUTPUT);
    pinMode(G4, OUTPUT);
    Serial.println("Traffic Lights Ready!");
    LCD_Ready("Traffic Lights",0);
    LCD_Ready("Ready!",1);
    delay(500);
    lcd.clear();
    Serial.println("Testing Traffic Lights...");
    LCD_Ready("Testing Traffic",0);
    LCD_Ready("Lights...",1);
    delay(500);
    lcd.clear();
    TrafficLights();
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    digitalWrite(R3, HIGH);
    digitalWrite(R4, HIGH);
    TurnGreen(1, 0);

}

//Running Loops
void loop() {
    
    if(Emergency(rfid1))
    {
        lcd.clear();
        Serial.println("Emergency Vechile Detected...");
        LCD_Ready("Emergency",0);
        LCD_Ready("Vehicle!!!",1);
        delay(3000);
    }

    if(Emergency(rfid2))
    {
        lcd.clear();
        Serial.println("Responding to Emergency Vechile...");
        LCD_Ready("Emergency",0);
        LCD_Ready("Vehicle!!!",1);
        Lane = ActiveLane;
        ActiveLane = 1;
        RoundRobin = true;
        TurnGreen2(ActiveLane, Lane);
    }

    DensityA();

    if(RoundRobin == true && ActiveTime >= 90 && RoundRobin2 == false || RoundRobin == true && RoundRobin2 == true || RoundRobin2 == true)
        {
            Serial.println("Round Robin");
            if(ActiveLane < 4)
            {
                ActiveTime = 0;
                Lane = ActiveLane;
                ActiveLane++;
                TurnGreen(ActiveLane, Lane);
            }
            else
            {
                ActiveTime = 0;
                Lane = ActiveLane;
                ActiveLane = 1;
                TurnGreen(ActiveLane, Lane);
            }
        }
        else if (RoundRobin2 == false)
        {
            lcd.clear();
            Serial.println("Pass With Caution...");
            LCD_Ready("Pass With",0);
            LCD_Ready("Caution !!!",1);
            digitalWrite(G1, LOW);
            digitalWrite(G2, LOW);
            digitalWrite(G3, LOW);
            digitalWrite(G4, LOW);
            digitalWrite(R1, LOW);
            digitalWrite(R2, LOW);
            digitalWrite(R3, LOW);
            digitalWrite(R4, LOW);
            digitalWrite(O1, HIGH);
            digitalWrite(O2, HIGH);
            digitalWrite(O3, HIGH);
            digitalWrite(O4, HIGH);
        }
}
