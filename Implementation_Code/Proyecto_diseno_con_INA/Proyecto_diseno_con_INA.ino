/*Código creado por Sergio Oliveros con ayuda de la documentación brindada por Blynk
link de las fuentes: https://docs.blynk.io/en/getting-started/supported-boards

NOTAS: 
Cosas por hacer (en ese orden):
1- Añadir la función de deep sleep -> COMPLETADA
2- Probar si la ESP se puede comunicar con proteus -> Si se puede, COMPLETADA 
3- Implementar el código del INA a este código -> COMPLETADA
4- Hacer pruebas :) -> No es posible hacer pruebas en esta versión del código por la falta del sensor
*/
//INA219
#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;
//Deep Sleep
#define uS_TO_S_FACTOR 1000000  //Para convertir los microsegundos a segundos
#define TIME_TO_SLEEP 1800 //LA ESP32 ENTRA EN MODO DEEP SLEEP DURANTE MEDIA HORA
//Blynk
#define BLYNK_TEMPLATE_ID "TMPL2EIePkIaK"
#define BLYNK_TEMPLATE_NAME "Sensor"
#define BLYNK_AUTH_TOKEN "nR-Etdo4Yw2mPFXQaexz26yBq_eV0-se"
//Conexión a WiFi
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//Variable que se guarda cuando se duerme la ESP32
RTC_DATA_ATTR int bootCount = 0; 
//Credenciales de la red WiFi a conectar
char ssid[] = "ESP";
char pass[] = "12345678";
//Timer de la libreria de Blynk
BlynkTimer timer; //Para mandar datos periódicamente
int datos_a_enviar = 20;
int datos_enviados = 0;
//Variables a sensar 
//INA
float shuntv = 0;
float busv = 0;
float current = 0;
float voltage = 0;
float power = 0;

// Indicar que la ESP32 está conectada a la aplicación
BLYNK_CONNECTED() {Blynk.virtualWrite(V0,1);}
//Enviar datos periódicamente
void myTimerEvent()
{       
  Blynk.virtualWrite(V1, current); //Enviar datos de corriente y tensión            
  Blynk.virtualWrite(V2, voltage);
  datos_enviados++;
  if(datos_enviados >= datos_a_enviar){ 
    Serial.println("Numero de datos enviados: " + String(datos_enviados));
    Serial.println("Entrando en suspensión.");
    Serial.flush();
    esp_deep_sleep_start();
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  //Conectarse a la aplicación
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  //Definir el periodo de funcionamiento de la funcion del temporizador
  timer.setInterval(3000L, myTimerEvent); 

  ++bootCount;
  Serial.println("Inicio numero " + String(bootCount) + ".");

  //Activar el modo deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP*uS_TO_S_FACTOR); //Definir los segundos a dormir
  Serial.println("Las tramas de datos se van a enviar cada " + String(TIME_TO_SLEEP) + " segundos."); //Mensaje para indicar cuanto va a domir la ESP32
 
  //INA219
  uint32_t currentFrequency;
  if(!ina219.begin()){
    Serial.println("Error al encontrar el sensor.");
    while(1){delay(10);} //Bucle infinito
  }
  ina219.setCalibration_32V_2A(); //Librería modificada para poder medir 25v y 5A.
}

void loop()
{
  //------------------Sensado de voltaje------------------ 
  shuntv = ina219.getShuntVoltage_mV();
  busv = ina219.getBusVoltage_V();
  voltage = busv + (shuntv/1000);
  //------------------Sensado de corriente------------------ 
  current = ina219.getCurrent_mA();
  //Serial.println(int(current));
  //-----------------Envar datos por Blynk------------------
  Blynk.run();
  timer.run();
}
