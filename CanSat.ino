
const String Callsign = "Hindenburg 2";

// The GY91 reads the data from the GY91 module
#include <GY91.h>

#include <Cansat_RFM96.h>

#define USE_SD              0
#define DO_CALLSIGN         1  
#define DO_MILLIS           1
#define DO_COUNTER          1
#define DO_PRESSURE         1
#define DO_TEMPERATURE      1
#define DO_ACCELERATION     1
#define DO_GYRO             1
#define DO_MAGNET           1
#define DO_VIN              1



#define printB rfm96.printToBuffer

// Declare all variables
double pressure;
uint16_t Millis;
unsigned int counter=0;
double temperature;
double ax, ay, az, gx, gy, gz, mx, my, mz;

// We need to make an instance of the two objects
GY91 gy91;
Cansat_RFM96 rfm96(433350, USE_SD);   // if DO_SD is 1 we will save to an SD card

void setup() {
  Serial.begin(9600);
  
  // Initiate the gy91 object, and stop if it could not init
  if (!gy91.init()) {
    Serial.println("Could not initiate");
    while(1);
  }
  if (!rfm96.init()) {
    Serial.println("Init of radio failed, stopping");
    while(1);
  }
  rfm96.setTxPower(5);

  analogReadResolution(12);
  analogWriteResolution(16);
}

void loop() {


#if DO_CALLSIGN == 1
      // Callsign

  // Add the data to the buffer. These functions do not initiate a transfer
  printB(Callsign);
  printB(";");
#endif

#if DO_MILLIS == 1
      // Millis

  // Update Millis
  Millis = millis();



  // Buffer

  printB(Millis);
  printB(";");

#endif

#if DO_COUNTER == 1
      // Counter

  // Buffer
  printB(counter++);
  printB(";");

#endif


#if DO_PRESSURE == 1  
      // PRESSURE
  
  
  // Update the new pressure data
  pressure = gy91.readPressure();

  
  
  // 
  printB(pressure);
  printB(";");
  
#endif

#if DO_TEMPERATURE == 1
      // TEMPERATURE


  // Update temperature
  temperature = read_temp_direct();


  //Buffer
  printB(temperature);
  printB(";");

#endif

#if DO_ACCELERATION == 1
      // Acceleration

  //Update acc vars
  ax = gy91.ax;
  ay = gy91.ay;
  az = gy91.az;

  //Buffer
  printB(ax);
  printB(";");
  printB(ay);
  printB(";");
  printB(az);
  printB(";");

#endif

#if DO_GYRO == 1
      // Gyro

  //Update Gyro vars
  gx = gy91.gx;
  gy = gy91.gy;
  gz = gy91.gz;


  //Buffer
  printB(gx);
  printB(";");
  printB(gy);
  printB(";");
  printB(gz);
  printB(";");

#endif

#if DO_MAGNET == 1
      // Magnet

  //Update mag vars
  mx = gy91.mx;
  my = gy91.my;
  mz = gy91.mz;

  //Buffer
  printB(mx);
  printB(";");
  printB(my);
  printB(";");
  printB(mz);
  printB(";");

#endif

#if DO_VIN == 1

      //VIN

  // Update Vin
  double VIN = analogRead(A11)*3.3*2/4095

  // Buffer
  printB(VIN);
  printB(";");

#endif

      // Send
  
  rfm96.sendAndWriteToFile();  
  delay(500);
}



// Function for reading and converting NTC temp

double read_temp_direct() {
  double R_NTC, log_NTC;
  uint16_t ARead = analogRead(A10);
  R_NTC = 4700*ARead/(4095.0-ARead);
  log_NTC = log(R_NTC/10000);

  // The line below is the Steinhart-Hart equation
  double i = (1/(3.354016E-3 + 2.569850E-4*log_NTC + 2.620131E-6*log_NTC*log_NTC + 6.383091E-8*log_NTC*log_NTC*log_NTC)-273.15);
  Serial.println(i);
  return i;
}
