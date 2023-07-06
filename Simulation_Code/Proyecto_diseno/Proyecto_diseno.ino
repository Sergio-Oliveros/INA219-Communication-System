/*Código creado por Sergio Oliveros con ayuda de la documentación brindada por Blynk
link de las fuentes: https://docs.blynk.io/en/getting-started/supported-boards

NOTAS: 
Cosas por hacer (en ese orden):
1- Añadir la función de deep sleep -> COMPLETADA
2- Probar si la ESP se puede comunicar con proteus -> Si se puede, COMPLETADA 
3- Implementar el código del INA a este código -> NO ES NECESARIO PARA ESTA VERSIÓN - PENDIENTE
4- Hacer pruebas :)
*/

//Deep Sleep
#define uS_TO_S_FACTOR 1000000  //Para convertir los microsegundos a segundos
#define TIME_TO_SLEEP 10 //IMPORTANTE: CAMBIAR A MEDIA HORA PARA ENVIAR LOS DATOS
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
int current = 100;
int voltage = 25;
//Para separar la corriente del voltaje 
int delimitador;
String datos_leidos;
String dato1;
String dato2;

// Indicar que la ESP32 está conectada a la aplicación
BLYNK_CONNECTED() {Blynk.virtualWrite(V0,1);}
//Enviar datos periódicamente
void myTimerEvent()
{
  Blynk.virtualWrite(V4, bootCount); 

  if(Serial.available() != 0){

    datos_leidos = Serial.readStringUntil('\n');  // Lee los datos del puerto serial hasta el salto de línea
    delimitador = datos_leidos.indexOf(',');  // Busca el índice del delimitador

    if (delimitador != -1) {
      dato1 = datos_leidos.substring(0, delimitador);  // Extrae el primer dato
      dato2 = datos_leidos.substring(delimitador + 1);  // Extrae el segundo dato

      current = dato1.toInt();  // Convierte el primer dato a entero
      voltage = dato2.toInt();  // Convierte el segundo dato a entero
    }      
    Blynk.virtualWrite(V1, current); 
    Blynk.virtualWrite(V2, voltage);   
  }  
  
  datos_enviados++;
  Blynk.virtualWrite(V3, datos_enviados); 
  if(datos_enviados > datos_a_enviar){ 
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
}

void loop()
{
  Blynk.run();
  timer.run();
}
