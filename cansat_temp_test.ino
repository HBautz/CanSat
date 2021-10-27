void setup(){
  Serial.begin(9600);
  while(!Serial);
  
  analogReadResolution(12);
}

void loop() {
 double celsius_temp;
 
 celsius_temp = read_temp();

 Serial.println(celsius_temp, "°C");

 delay(100);
}


double read_temp(){
  double r_ntc, log_ntc;
  uint16_t ARead = analogRead(A10);
  r_ntc = 4700*ARead/(4095.0-ARead);
  log_ntc = log(r_ntc/10000);

 return 1 / (3.354016E-3 + 2.569850E-4*log_ntc + 2.620131E-6*log_ntc*log_ntc + 6.383091E-8*log_ntc*log_ntc*log_ntc)-273.15;
}
