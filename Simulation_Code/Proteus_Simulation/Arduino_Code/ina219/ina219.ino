#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal.h> //ELIMINAR LUEGO
LiquidCrystal lcd(2,3,4,5,6,7); //ELIMINAR LUEGO

float shuntv = 0;
float busv = 0;
float current = 0;
float voltage = 0;
float power = 0; 

Adafruit_INA219 ina219;
void setup() {
  Serial.begin(9600);
  while(!Serial){
    delay(1);
  }
  //---------------ELIMINAR
  lcd.begin(20,4);
  lcd.setCursor(0,0);
  lcd.print("Vatimetro");
  lcd.setCursor(0,1);
  lcd.print("digital DC");
  delay(2000);
  lcd.clear();
  //-----------------ELIMINAR
  
  uint32_t currentFrequency;

  if(!ina219.begin()){
//--------------------------Eliminar 
    lcd.setCursor(0,0);
    lcd.print("Failed to find");
    lcd.setCursor(0,1);
    lcd.print("INA219 chip");
//-------------------------ELIMINAR 
    while(1){delay(10);} //Bucle infinito
  }

  lcd.clear();  //  Eliminar 
  ina219.setCalibration_32V_2A();
  //---------------ELIMINAR 
  lcd.setCursor(0,0);
  lcd.print("Measuring voltage");
  lcd.setCursor(0,1);
  lcd.print("and current with");
  lcd.setCursor(0,2);
  lcd.print("INA219 ...");
  delay(2000);
  lcd.clear();
  //------------------ELIMINAR
}
void loop() {
  lcd.clear();     //ELIMINAR
  //------------------Sensado de voltaje------------------ 
  shuntv = ina219.getShuntVoltage_mV();
  busv = ina219.getBusVoltage_V();
  voltage = busv + (shuntv/1000);
  //------------------Sensado de corriente------------------ 
  current = ina219.getCurrent_mA();
  Serial.print(current);
  Serial.print(",");
  Serial.print(voltage);
  Serial.println();
  //Serial.println(int(current));
  //Serial.println(int(round(voltage)));
  //power = ina219.getPower_mW();
  //int potencia = power/1000;
  
//----------------------IMPRIMIR VALORES - ELIMINAR ---------------------
  lcd.setCursor(0,0);
  lcd.print("Voltage: "); lcd.print(voltage); lcd.print("V");
  lcd.setCursor(0,1);
  lcd.print("Current: "); lcd.print(current); lcd.print("mA");
  //lcd.setCursor(0,2);
  //lcd.print("Power: "); lcd.print(power); lcd.print("mW");
  //Serial.write(potencia); //Envio serial
//----------------------IMPRIMIR VALORES - ELIMINAR ---------------------
  
  delay(1000);
}
