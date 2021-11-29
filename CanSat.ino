
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
int buzzerPin = 29;

double pressure;
uint16_t Millis;
unsigned int counter = 0;
double temperature;
double ax, ay, az, gx, gy, gz, mx, my, mz;


int noteDurations[] = {
  8, 4, 6, 16, 4, 8, 8, 
  4, 6, 16, 4, 8, 8, 
  4, 8, 8, 4, 8, 8, 4, 8, 8, 2,
  4, 6, 16, 4, 8, 8, 
  4, 6, 16, 4, 8, 8, 
  4, 6, 16, 4, 6, 16, 
  4, 6, 16, 8, 8, 8, 8, 
  2, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8,
  2, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8,
  4, 6, 16, 4, 6, 16, 4, 8, 8, 2,
  2, 8, 8, 8, 8, 3, 8, 2,
  2, 8, 8, 8, 8, 3, 8, 2,
  4, 6, 16, 4, 4, 2, 4, 4, 1
};

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int melody[] = {
  NOTE_G4, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, 
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_C4, 
  NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, 
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_G4, 
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, 
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_C4, 
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_B4, NOTE_C5, NOTE_D5, 
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_G4, NOTE_B4, NOTE_C5, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4,
  NOTE_C5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_A4,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_C4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
};

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
  
  actP = (gy91.readPressure());
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
  while (true) {
    for (int thisNote = 0; thisNote < sizeof(melody) / 2; thisNote++) {
      int noteDuration = 2000 / noteDurations[thisNote];
      tone(buzzerPin, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(buzzerPin);
    }
  }
}
