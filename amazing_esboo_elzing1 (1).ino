#include <LiquidCrystal.h>
#define temp A5
#define buzzer 6
#define Button1 10
#define Button2 9
#define Button3 8
#define Button4 7
//variables...............
int Second = 0;
int minutes = 0;
int hours = 0;
unsigned int AlarmHours = 0;
unsigned int AlarmMins = 0;
bool clockFromat = true;
bool alarmSet = false;
bool degree = false;
bool stopAlarm = false;
unsigned int stopTimer = 0;
unsigned int counter1 = 0;
unsigned int counter2 = 0;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned int initialTime = millis();
//........................................
void setup() {
lcd.begin(16, 2);
Serial.begin(9600);
lcd.print("TIME");
lcd.setCursor(8,0);
lcd.print("||");
lcd.setCursor(8,1);
lcd.print("||");
pinMode(temp,INPUT);
pinMode(buzzer,OUTPUT);
pinMode(Button1,INPUT_PULLUP);
pinMode(Button2,INPUT_PULLUP);
pinMode(Button3,INPUT_PULLUP);
pinMode(Button4,INPUT_PULLUP);
Second = 0;
minutes = 0;
hours = 0;
}

void loop() {

//call Display function
showTime();
showTemp();
showAlarm();
changeTime();
initialTime = millis();
//handling buttons
handleButtons();
}

void handleButtons() {
//tested
if(digitalRead(Button1) == LOW) {
counter1++; //3sec
return;
}
// for handling Button1
if(counter1 >= 30) {
setTime();
counter1 = 0;
} else if (counter1 > 0){
clockFromat = !clockFromat;
counter1 = 0;
}
if(digitalRead(Button2) == LOW) {
counter2++; //3sec
return;
}
//for handling Button2
if(counter2 >= 30) {
setAlarm();
counter2 = 0;
}
else if(counter2 > 0 ) {
if(AlarmHours == -1) {
setAlarm();
}
alarmSet = !alarmSet;
counter2 = 0;
}
if(digitalRead(Button3) == LOW) {
  
degree = !degree;
}
  
if(alarmSet && stopTimer < 1) {
playAlarm();
stopAlarm = false;
stopTimer = 0;
}
  
if(digitalRead(Button4) == LOW) {
stopTimer = 5;
stopAlarm = true;
stop();
}
}


void showTime() {

String hrs,mins;
minutes = minutes % 60;
if(clockFromat) {
if(hours < 12) {
lcd.setCursor(6,1);
lcd.print("AM");
} else {
lcd.setCursor(6,1);
lcd.print("PM");
}
hours = hours % 12;
} else {
hours = hours % 24;
lcd.setCursor(6,1);
lcd.print(" ");
}
//setting for hours
if(hours < 9) {
hrs = "0" + String(hours);
} else {
hrs = String(hours);
}
  
//setting for minutes
if(minutes < 9) {
mins = "0" + String(minutes);
} else {
mins = String(minutes);
}

//set cursor to col 0, line 0
lcd.setCursor(0,1);
lcd.print(hrs + ":" + mins);
}


float temperature() {
int temperature = analogRead(temp);

// converting that temperature to voltage, for 3.3v arduino use 3.3
float voltage = temperature * 5.0;
voltage /= 1024.0;

// now print out the temp
float temperatureC = (voltage - 0.5) * 100 ; //converting from 10 mv per degree wit 500 mV offset
//to degrees ((voltage - 500mV) times 100)
// now convert to Fahrenheit
float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
if(degree)
return temperatureC;
return temperatureF;
}

void showTemp() {
float value = temperature();
lcd.setCursor(10, 1);
if(degree) {
lcd.print(String(value)+ "C");
return;
}
lcd.print(String(value)+ "F");
}

void showAlarm() {
lcd.setCursor(10, 0);
if(!alarmSet) {
lcd.print("NO AL");
return ;
}
String hrs,mins;
if(AlarmHours <= 9) {
hrs = "0" + String(AlarmHours);
} else {
hrs = String(AlarmHours);
}
  
//setting for minutes
if(AlarmMins <= 9) {
mins = "0" + String(AlarmMins);
} else {
mins = String(AlarmMins);
}
String active;
//set active = S if set else set it N
if(alarmSet) {
active = "AL";
} else {
active = "NAL";
}
lcd.print(hrs + ":" + mins + active);
}


void playAlarm() {
if(AlarmHours == hours && AlarmHours == minutes)
digitalWrite(buzzer,HIGH);
}

void stop() {
digitalWrite(buzzer,LOW);
}

void changeTime() {
// count seconds
unsigned int currentMillis = millis();
Second += currentMillis - initialTime;
if (Second < 300) {
return;
}
Second = 0;
// count minutes
minutes++;
if(stopAlarm)
stopTimer--;
if (minutes < 60) {
return;
}
minutes = 0;
// count hours
hours++;
if (hours < 24) {
return;
}
hours = 0;
}

void setTime() {
//first set time
int i = 3;
int counter = 0;
int mins = 0;
int hrs = 0;
lcd.setCursor(0,1);
lcd.print(" ");
while(counter < 2 ) {
lcd.setCursor(i,1);
lcd.blink();
if(mins > 59)
mins = 0;
if(hrs > 12)
hrs = 0;
if(digitalRead(Button3) == LOW && i == 3) {
mins++;
lcd.print(mins);
}
if(digitalRead(Button3) == LOW && i == 0) {
hrs++;
lcd.print(hrs);
}
if(digitalRead(Button1) == LOW) {
counter++;
i = 0;
}
}
hours = hrs;
minutes = mins;
lcd.noBlink();
}

void setAlarm() {
int i = 13;
int counter = 0;
int mins = 0;
int hrs = 0;
lcd.setCursor(10,0);
lcd.print(" ");
while(counter < 2 ) {
lcd.setCursor(i,0);
lcd.blink();
Serial.println(mins);
if(mins > 59) {
mins = 0;
}
if(hrs > 12) {
hrs = 0;
}
if(digitalRead(Button3) == LOW && i == 13) {
mins++;
lcd.print(mins);
}
if(digitalRead(Button3) == LOW && i == 10) {
hrs++;
lcd.print(hrs);
}
if(digitalRead(Button2) == LOW) {
counter++;
i = 10;
}
}
AlarmHours = hrs;
AlarmMins = mins;
alarmSet = true;
lcd.noBlink();
}