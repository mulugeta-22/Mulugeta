
/*************************************************** 
  Name mulugeta Weldegebriel Id 850169962
  Mid Semester Project
  this code is to run the SHTC3 temp and humdity sensor  and SCD 40 CO2 sensor

 ****************************************************/

#include "Adafruit_SHTC3.h" //include the library adafruit SHTC3
#include <Wire.h>// to communicate microcontroller with all I2c devices
#include <SparkFun_SCD4x_Arduino_Library.h>// libray of the Adafruit SCD 40 CO2 sensor
#include <SD.h>// include library of the SD card
#include <SPI.h>// libray for communcation between the Arduino microcontroller and SD card

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3(); // adfrutit SHTC3 is declared as a vraible shtc3
SCD4x SCD4;// the adfaruit SCD 40 is declared as a variable SCd4
int led= 7;// led port 7 is declared as a varibale led
const int chip= 10;// pin 10 will be connected to the chip select (CS) of the SD card
File muller;// the fille in the SD card will be saved Under the folder "muller"

void setup() {
  pinMode(led, OUTPUT); // call the pinmode function
  Serial.begin(115200);// the communication between the board and IDE begins
  Wire.begin();
  // SHTC3 Intialiazation
while (!Serial)  // if the serial does not begin it will wait 10 milli second
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  if (! shtc3.begin()) {    // if the sensor does not working or not connected it will print out "coudn't find SHTC3" 
    Serial.println("Couldn't find SHTC3");      
    while(1) delay(1);
  } else{
  Serial.println("Found SHTC3 sensor");
  } // if sensor connected it will print"found SHTC3 sensor"
  //SCDC 40 CO2 sensor Intialization
 if (!SCD4.begin()){
  Serial.println(" coudn't find SCD4 sensorr");
   while(1) delay(1);// check wiring
} else {
Serial.println("Found SCD4 Sensor");
}
SCD4.startPeriodicMeasurement();
delay(5000);

// SD intialization
if(!SD.begin(chip)){
  Serial.println("SD not conneted");
  while(1);
} else{
Serial.println("SD intialized");
}
// writng the file name SHTC3.TXT on the SD card
muller= SD.open("SHTC3.Txt", FILE_WRITE);
if(muller) {
muller.println("millis, SHTC3Temp, SHT3RH(%)");
muller.close();
} else{
Serial.println("error opening SHTC3 text");
}
// Writing the file nmae of SDR.Txt in the SD card
muller=SD.open("SCD4.Txt", FILE_WRITE);
if(muller){
 muller.println("mills, scdTEMP, SCD RH(%), CO2(ppm)");
 muller.close();
} else{
Serial.println("error SCD4 opening datalog text");
}
}
void loop() {
 if (millis()< 259200000 ){  // this will iterate untill 100 seconds 
   digitalWrite(led, HIGH);  // turn the LED on (HIGH is the voltage level)
   delay(5000);// wait for 5 seconds
   //read temprature and humidity from SHTC3 sensor
  sensors_event_t humidity, temp;
  shtc3.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
// Read T/RH and CO2 form SCD 40 CO2 sensor
  if(SCD4.readMeasurement()){
    Serial.println("data ready");
    // then T/RH and CO2 will be printed in the serial monitor for both sensors
  Serial.print("time:"); Serial.println(millis()/1000);
  Serial.print("SHTC3Temp: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("ScdTemp: ");Serial.print(SCD4.getTemperature(),1);Serial.println(" dgree c");
  Serial.print("SHTC3RH: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  Serial.print("ScdRH: ");Serial.print(SCD4.getHumidity(),1);Serial.println("%RH");
  Serial.print("Scdco2: ");Serial.print(SCD4.getCO2(),1);Serial.println("ppm");
  // log data to SD card from shtc3 sensor
muller= SD.open("SHTC3.Txt", FILE_WRITE);
if(muller){
 muller.print(millis()/1000); muller.print(", ");
  muller.print(temp.temperature); muller.print(", ");
  muller.println(humidity.relative_humidity);
  muller.close();
} else{
Serial.println("error logging data from SHTC3");
}
//log data to SD card from SCD 40sensor
  muller=SD.open("SCD4.Txt", FILE_WRITE);
   if(muller){
   muller.print(millis()/1000); muller.print(", ");
   muller.print(SCD4.getTemperature()); muller.print(", ");
   muller.print(SCD4.getHumidity()); muller.print(", ");
   muller.println(SCD4.getCO2());
   muller.close();
   } else{
Serial.println(" erorr logging data from SCD 40 sensor");
   }
 }
 digitalWrite(led, LOW);   // turn the LED off by making the voltage LOW
  delay(5000); // wait 5 seconds while the led is off 
}  
}
