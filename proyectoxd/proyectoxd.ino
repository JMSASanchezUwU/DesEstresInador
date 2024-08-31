#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "pitches.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

// Tópicos para los sensores
const char* topicTempAmbiente = "sensor/temperatura/ambiente";
const char* topicTempObjeto = "sensor/temperatura/cuerpo";
const char* topicFrecuencia = "sensor/frecuencia_cardiaca/frecuencia";
const char* topicSpo2 = "sensor/oxigeno/spo2";
const char* topicIR = "sensor/oxigeno/ir";
const char* topicEstadoAlarma = "sensor/estado/alarma";

// Tópicos para los actuadores
const char* topicLedRojo = "actuadores/ledRojo";
const char* topicLedVerde = "actuadores/ledVerde";
const char* topicBuzzer = "actuadores/buzzer";
const char* topicMotor = "actuadores/motor";


//Tópico para las funciones agrgadas
const char* topicCount = "funcion/count";
const char* topicSum = "funcion/sum";
const char* topicAvg = "funcion/avg";

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuración de la red Wi-Fi
const char* ssid = "jmsawifi"; 
const char* password = "linux123"; 

// Configuración del servidor NTP
const long utcOffsetInSeconds = -21600; 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Definición de los pines
#define BUZZER_PIN  2
#define MOTOR_PIN  17
#define LED_GREEN_PIN  16
#define LED_RED_PIN  4

// Instancias de los sensores
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MAX30105 particleSensor;

// Variables globales para MAX30105
#define BUFFER_SIZE 100
uint32_t irBuffer[BUFFER_SIZE];
uint32_t redBuffer[BUFFER_SIZE];
int32_t bufferLength = BUFFER_SIZE;
int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

// Variables booleanas para los actuadores
bool ledRojoState = true;
bool ledVerdeState = true;
bool buzzerState = true;
bool motorState = true;

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("No se pudo encontrar una pantalla SSD1306."));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  // Conexión a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado a la red Wi-Fi \n Ip: ");
  Serial.println(WiFi.localIP());

  // Inicializa el cliente NTP
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);

  if (!mlx.begin()) {
    Serial.println("No se pudo encontrar el sensor MLX90614.");
    while (1);
  }

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("No se pudo encontrar un sensor MAX30102.");
    while (1);
  }

  client.setServer("3.144.180.0", 1883);
  client.setCallback(callback);

  // Intentar suscribirse a los tópicos
  if (client.subscribe(topicLedRojo)) {
    Serial.println("Suscrito al tópico del LED rojo");
  } else {
    Serial.println("Error al suscribirse al tópico del LED rojo");
  }

  if (client.subscribe(topicLedVerde)) {
    Serial.println("Suscrito al tópico del LED verde");
  } else {
    Serial.println("Error al suscribirse al tópico del LED verde");
  }

  if (client.subscribe(topicBuzzer)) {
    Serial.println("Suscrito al tópico de Buzzer");
  } else {
    Serial.println("Error al suscribirse al tópico de Buzzer");
  }

  if (client.subscribe(topicMotor)) {
    Serial.println("Suscrito al tópico de Motor");
  } else {
    Serial.println("Error al suscribirse al tópico de Motor");
  }
  client.subscribe(topicCount);
  client.subscribe(topicSum);
  client.subscribe(topicAvg);

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el tópico: ");
  Serial.print(topic);
  Serial.print(". Mensaje: ");
  
  String message;  
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  
  if (String(topic) == topicLedRojo) {
    ledRojoState = (message == "on");
  } else if (String(topic) == topicLedVerde) {
    ledVerdeState = (message == "on");
  } else if (String(topic) == topicBuzzer) {
    buzzerState = (message == "on");
  } else if (String(topic) == topicMotor) {
    motorState = (message == "on");
  } else if (String(topic) == topicCount) {
    int countValue = message.toInt();
    if (countValue > 100) {
      activarMotor(3);  // Activa el motor 3 veces si el count es mayor a 100
    }
  } else if (String(topic) == topicSum) {
    int sumValue = message.toInt();
    if (sumValue > 5000) {  // Define el umbral que consideras alto para la sum
      // Aquí activas los actuadores o realizas alguna acción
      activarActuadoresSum();
    }
  } else if (String(topic) == topicAvg) {
    float avgValue = message.toFloat();
    if (avgValue < 25) {
      activarAlarmaTempBaja();
    } else if (avgValue > 30) {
      activarAlarmaTempAlta();
    }
  } else {
    Serial.println("Tópico desconocido");
  }
}

void activarMotor(int veces) {
  for (int i = 0; i < veces; i++) {
    digitalWrite(MOTOR_PIN, HIGH);
    delay(500);
    digitalWrite(MOTOR_PIN, LOW);
    delay(500);
  }
}

void activarActuadoresSum() {
  tone(BUZZER_PIN, NOTE_DS8, 500);
  digitalWrite(LED_RED_PIN, HIGH);
  delay(1000);
  noTone(BUZZER_PIN);
  digitalWrite(LED_RED_PIN, LOW);
}

void activarAlarmaTempBaja() {
  digitalWrite(LED_RED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_RED_PIN, LOW);
}

void activarAlarmaTempAlta() {
  tone(BUZZER_PIN, NOTE_DS8, 500);
  delay(500);
  noTone(BUZZER_PIN);
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Necesario para mantener la conexión MQTT

  // Actualiza la hora
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();

  // Convierte el tiempo a formato de 12 horas
  int hours = timeClient.getHours();
  String ampm = "AM";
  if (hours >= 12) {
    if (hours > 12) hours -= 12;
    ampm = "PM";
  } else if (hours == 0) {
    hours = 12;
  }

  // Formatea la hora
  String timeString = String(hours) + ":" + timeClient.getMinutes() + ":" + timeClient.getSeconds() + " " + ampm;

  // Lee las temperaturas
  double ambientTemp = mlx.readAmbientTempC();
  double objectTemp = mlx.readObjectTempC();

  // Llena los buffers y calcula los valores
  for (int i = 0; i < bufferLength; i++) {
    while (particleSensor.available() == false)
      particleSensor.check();
    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();
  }

  int32_t sumIR = 0;
  for (int i = 0; i < bufferLength; i++) {
    sumIR += irBuffer[i];
  }

  if (sumIR > 50000) {
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }

  bool valuesUnusual = heartRate > 150 || heartRate < 50 || spo2 < 75 || objectTemp > 38 || objectTemp < 29.0;

  String recommendation = ""; // Variable para almacenar la recomendación

  if (valuesUnusual) {
    // Verifica si el buzzer está activado
    if (buzzerState) {
      tone(BUZZER_PIN, NOTE_DS8, 500);
    }

    // Verifica si el motor está activado
    if (motorState) {
      digitalWrite(MOTOR_PIN, HIGH);
    } 

    // Verifica si el LED rojo está activado
    if (ledRojoState) {
      digitalWrite(LED_RED_PIN, HIGH);
    } 

    digitalWrite(LED_GREEN_PIN, LOW);
    
    delay(1000);
    noTone(BUZZER_PIN);
    digitalWrite(MOTOR_PIN, LOW);
   digitalWrite(LED_RED_PIN, LOW);

    // Publicar estado de alarma activa
    client.publish(topicEstadoAlarma, "ALARM_ACTIVE");

    // Determina la recomendación basada en el valor inusual
    if (heartRate > 120 || heartRate < 50) {
      recommendation = "Respira profundo\ny relajate :3";
    } else if (spo2 < 90) {
      recommendation = "Toma aire fresco\nlentamente";
    } else if (objectTemp > 38) {
      recommendation = "Trata de refrescarte\nbien";
    } else if (objectTemp < 29.0) {
      recommendation = "Abrígate y\nmantente cálido";
    }
  } else {
    // Verifica el estado del LED verde
    if (ledVerdeState) {
      digitalWrite(LED_GREEN_PIN, HIGH);
    }

    digitalWrite(LED_RED_PIN, LOW);
    noTone(BUZZER_PIN);
    digitalWrite(MOTOR_PIN, LOW);
    
    // Publicar estado de alarma inactiva
    client.publish(topicEstadoAlarma, "ALARM_INACTIVE");
  }

  // Publicar datos en los temas MQTT
  char tempAmbienteStr[8];
  dtostrf(ambientTemp, 1, 2, tempAmbienteStr);
  client.publish(topicTempAmbiente, tempAmbienteStr);

  char tempObjetoStr[8];
  dtostrf(objectTemp, 1, 2, tempObjetoStr);
  client.publish(topicTempObjeto, tempObjetoStr);

  if (validHeartRate) {
    char frecuenciaStr[8];
    itoa(heartRate, frecuenciaStr, 10);
    client.publish(topicFrecuencia, frecuenciaStr);
  }

  if (validSPO2) {
    char spo2Str[8];
    itoa(spo2, spo2Str, 10);
    client.publish(topicSpo2, spo2Str);
  }

  // Publicar el valor de IR
  char irStr[8];
  itoa(irBuffer[bufferLength - 1], irStr, 10);
  client.publish(topicIR, irStr);

  // Muestra la información en la pantalla OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (valuesUnusual) {
    // Muestra solo la recomendación en la pantalla OLED
    display.setCursor(0, 0);
    display.print(recommendation);
  } else {
    // Muestra la hora en la esquina superior derecha
    display.setCursor(SCREEN_WIDTH - 80, 0);
    display.print(timeString);

    display.setCursor(0, 10);
    display.print("Temp ambiente: ");
    display.print(ambientTemp, 1);
    display.println(" C");

    display.setCursor(0, 20);
    display.print("Temp corporal: ");
    display.print(objectTemp, 1);
    display.println(" C");

    display.setCursor(0, 30);
    display.print("IR: ");
    display.println(irBuffer[bufferLength - 1]);

    if (validHeartRate) {
      display.setCursor(0, 40);
      display.print("Frecuencia: ");
      display.print(heartRate);
      display.println(" bpm");
    } else {
      display.setCursor(0, 40);
      display.println("HR no válida");
    }

    if (validSPO2) {
      display.setCursor(0, 50);
      display.print("SpO2: ");
      display.print(spo2);
      display.println(" %");
    } else {
      display.setCursor(0, 50);
      display.println("SpO2 no válida");
    }
  }

  display.display();  // Actualiza la pantalla con los nuevos valores o la recomendación
}


// Reconexión al broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando reconectar al broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado.");
      client.subscribe(topicLedRojo);
      client.subscribe(topicLedVerde);
      client.subscribe(topicBuzzer);
      client.subscribe(topicMotor);
      client.subscribe(topicCount);
      client.subscribe(topicSum);
      client.subscribe(topicAvg);
    } else {
      Serial.print("Error de conexión, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando en 5 segundos...");
      delay(1000);
    }
  }
}

