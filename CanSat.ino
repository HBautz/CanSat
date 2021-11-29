
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





int GyroTolerance = 10;
float AccTolerance = 0.05;


// Declare all variables
double Taxu, Taxl, Tayu, Tayl, Tazu, Tazl, Tgxu, Tgxl, Tgyu, Tgyl, Tgzu, Tgzl;
int Moff, Mcu;

double actP;
int lt = 0;

double pressure;
uint16_t Millis;
unsigned int counter = 0;
double temperature;
double ax, ay, az, gx, gy, gz, mx, my, mz;

// We need to make an instance of the two objects
GY91 gy91;
Cansat_RFM96 rfm96(433350, USE_SD);   // if DO_SD is 1 we will save to an SD card

void setup() {
  Serial.begin(19200);
  
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
  
  actP = (gy91.readPressure() - 500);
  Serial.println("Activation pressure:");
  Serial.print("\t");
  Serial.println(actP);
  
  
}

void loop() {

  
    
  #if DO_CALLSIGN == 1
      // Callsign
  
    // Add the data to the buffer. These functions do not initiate a transfer
    rfm96.printToBuffer(Callsign);
    rfm96.printToBuffer(";");
  #endif
  
  #if DO_MILLIS == 1
        // Millis
  
    // Update Millis
    Millis = millis();
  
  
  
    // Buffer
  
    rfm96.printToBuffer(Millis);
    rfm96.printToBuffer(";");
  
  #endif
  
  #if DO_COUNTER == 1
        // Counter
  
    // Buffer
    rfm96.printToBuffer(counter++);
    rfm96.printToBuffer(";");
  
  #endif
  
  
  #if DO_PRESSURE == 1  
        // PRESSURE
    
    
    // Update the new pressure data
    pressure = gy91.readPressure();
  
    
    
    // Buffer
    rfm96.printToBuffer(pressure);
    rfm96.printToBuffer(";");
    
  #endif
  
  #if DO_TEMPERATURE == 1
        // TEMPERATURE
  
  
    // Update temperature
    temperature = read_temp_direct();
  
  
    //Buffer
    rfm96.printToBuffer(temperature);
    rfm96.printToBuffer(";");
  
  #endif
  
  #if DO_ACCELERATION == 1
        // Acceleration
    gy91.read_acc();
    //Update acc vars
    ax = gy91.ax;
    ay = gy91.ay;
    az = gy91.az;
  
    //Buffer
    rfm96.printToBuffer(ax);
    rfm96.printToBuffer(";");
    rfm96.printToBuffer(ay);
    rfm96.printToBuffer(";");
    rfm96.printToBuffer(az);
    rfm96.printToBuffer(";");
  
  #endif
  
  #if DO_GYRO == 1
        // Gyro
    gy91.read_gyro();
    //Update Gyro vars
    gx = gy91.gx;
    gy = gy91.gy;
    gz = gy91.gz;
  
  
    //Buffer
    rfm96.printToBuffer(gx);
    rfm96.printToBuffer(";");
    rfm96.printToBuffer(gy);
    rfm96.printToBuffer(";");
    rfm96.printToBuffer(gz);
    rfm96.printToBuffer(";");
  
  #endif
  
  #if DO_MAGNET == 1
        // Magnet

    gy91.read_mag();
    //Update mag vars
    mx = gy91.mx;
    my = gy91.my;
    mz = gy91.mz;
  
    //Buffer
    rfm96.printToBuffer(mx);
    rfm96.printToBuffer(";");
    rfm96.printToBuffer(my);
    rfm96.printToBuffer(";");
    rfm96.printToBuffer(mz);
    rfm96.printToBuffer(";");
  
  #endif
  
  #if DO_VIN == 1
  
        //VIN
  
    // Update Vin
    double VIN = analogRead(A11)*3.3*2/4095;
  
    // Buffer
    rfm96.printToBuffer(VIN);
    rfm96.printlnToBuffer(",");
  
  #endif

  if (lt == 0) {
    if (gy91.readPressure() <= actP) {

      Serial.println("\nAbove 50m!\n");
      
      Taxu = (ax + AccTolerance);
      Tayu = (ay + AccTolerance);
      Tazu = (az + AccTolerance);
      Taxl = (ax - AccTolerance);
      Tayl = (ay - AccTolerance);
      Tazl = (az - AccTolerance);
      
      Tgxu = (gx + GyroTolerance);
      Tgyu = (gy + GyroTolerance);
      Tgzu = (gz + GyroTolerance);
      Tgxl = (gx - GyroTolerance);
      Tgyl = (gy - GyroTolerance);
      Tgzl = (gz - GyroTolerance);
      
      Moff = millis();
      lt = 1;
    } else {
      Serial.println("\nNot above 50m!\n");
    }
  }

  if (lt == 1) {
    
    Mcu = (millis() - Moff);
    if (Mcu >= 10000) {
      if (ax <= Taxu and ax >= Taxl) {
        if (ay <= Tayu and ay >= Tayl) {
          if (az <= Tazu and az >= Tazl) {
            if (gx <= Tgxu and gx >= Tgxl) {
              if (gy <= Tgyu and gy >= Tgyl) {
                if (gz <= Tgzu and gz >= Tgzl) {
                  Serial.println("\nRUNNING BUZZER!!\n");
                  runBuzzer();
                } else {
                  Serial.println("\nNot stable\n");
                }
              } else {
                Serial.println("\nNot stable\n");
              }
            } else {
              Serial.println("\nNot stable\n");
            }
          } else {
            Serial.println("\nNot stable\n");
          }
        } else {
          Serial.println("\nNot stable\n");
        }
      } else {
        Serial.println("\nNot stable\n");
      }
      
      Taxu = (ax + AccTolerance);
      Tayu = (ay + AccTolerance);
      Tazu = (az + AccTolerance);
      Taxl = (ax - AccTolerance);
      Tayl = (ay - AccTolerance);
      Tazl = (az - AccTolerance);
        
      Tgxu = (gx + GyroTolerance);
      Tgyu = (gy + GyroTolerance);
      Tgzu = (gz + GyroTolerance);
      Tgxl = (gx - GyroTolerance);
      Tgyl = (gy - GyroTolerance);
      Tgzl = (gz - GyroTolerance);
  
      Moff = millis();
    }
  }

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
  //Serial.println(i);
  return i;
}

// Function for buzzer
void runBuzzer() {
  Serial.println("\nBuzz Buzz\n");
}
