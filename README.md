# ProyectoIoT2024
GIDS4092 - Documentación proyecto IoT

## Integrantes
- José Manuel Sánchez Arredondo
- Omar Alejandro Lozada Zuñiga
- Miguel Angel Jaime García

## Objetivo general
El objetivo en general de nuestro proyecto es crear un Wearable con el proposito de reducir el estres en las personas que lo utilicen, dando asi una mejor salud para quien lo utilice.
### Objetivos específicos
- 1.- Planeación de prototipo.
- 2.- Diseñado del prototipo.
- 3.- Validación de prototipo.
- 4.- Test de prototipo.
- 5.- Realización de proyecto final.
- 6.- Test de proyecto.
- 7.-Liberación de proyecto.

## Tabla de Software utilizado
| Id | Software | Version | Tipo |
|----|----------|---------|------|
|1.1|Arduino IDE | 2.3.2 |Freeware|
|1.2|Visual Studio Code | 1.92.2 |Freeware|
|1.3|Mosquitto   | 2.0.18|Freeware|
|1.4|Node-Red   | 4.0.2 |Freeware|
|1.5|Node.js  | 22.7.0 |Freeware|
|1.6|MySQL | 8.0.39 |Freeware|
|1.7|Flutter | 3.24.1 |Freeware|
|1.8|Dart | 3.5.1|Freeware|

## Tabla con el hardware utilizado
| Id | Componente | Descripción | Imagen | Cantidad | Costo total |
|----|------------|-------------|--------|----------|-------------|
|2.1|  Esp32  | Es la denominación de una familia de chips SoC de bajo coste y consumo de energía, con tecnología Wi-Fi y Bluetooth de modo dual integrada.            |![image](https://user-images.githubusercontent.com/106613839/217649811-9335a9c7-5a4c-4813-8f17-e03488f52967.png)| 1         | $150            |
|2.2|  Cables          | Clabes por los cuales se harán conecciones entre sensores, actuadores y la esp32|![image](https://user-images.githubusercontent.com/106613839/217650692-88574d94-022c-44fe-a183-58dd5121c26a.png)| 20         |  $80           |
|2.| Leds  | Leds para indicar el estado de la alarma.   |![image](https://github.com/user-attachments/assets/81d25d8e-f3ad-4d6b-b065-9b8e5e479829)| 2    | $5       |
|2.4| Sensor MAX30102  | Sensor infrarojo para detectar pulso y oxigenación. |![image](https://github.com/user-attachments/assets/cbfc41ed-f6f5-4ca3-9d19-2e82c2b877c3)| 1   |  $70      |
|2.5| Sensor GY-906  | Sensor para detectar la temperatura corporal y ambiental |![image](https://github.com/user-attachments/assets/b71ed642-a357-467e-b429-a453621cb4ec)| 1 | $100  |
|2.6| MiniMotor vibrador  | Minimotor para dar vibraciones al usuario y este pueda detectar que se ha activado la alarma|![image](https://github.com/user-attachments/assets/aeca266d-fd31-41b6-90b6-5354de503be8)|1| $2750            |
|2.7| Buzzer  | Es un pequeño duspositivo capaz de convertir energía eléctrica en sonido. |![image](https://user-images.githubusercontent.com/106613839/217650467-74e67b77-a1b1-4a00-9268-57ff7921a149.png)|  1   | $20   |
|2.8| Display OLED  | Pantalla con tecnología OLED para mostrar los datos y alertas en pantalla. |![image](https://user-images.githubusercontent.com/106613839/233921614-5501b5f8-bc25-4e67-9ae6-b3f3069594ad.png)| 1 | $95 |
|2.9| Placa fenólica | Placa hecha de papel impregnado con resina fenólica endurecida. Es un circuito impreso. |![image](https://user-images.githubusercontent.com/106613839/233930710-8fb3ca33-d93f-4d8d-ac8f-adf434ff0cbd.png)| 1 | $22 |


## Epicas del proyecto (Minimo debe de haber una épica por integrante de equipo)
- Yo como usuario quiero saber el nivel de oxigeno en la sangre.
- Yo como usuario quiero saber los latidos por minutos en mi cuerpo.
- Yo como usuario quiero saber la temperatura de mi cuerpo.
- Yo como usuario quiero saber la temperatura ambienta.
- Yo como usuario quiero ver los datos de los sensores en pantalla.
- Yo como usuario quiero ver los datos en un dashboard de una app mobile.
- Yo como usuario quiero ver los datos en un dashboard de una app para tv.
- Yo como usuario quiero ver los datos en un dashboard en node-red.

## Tabla de historias de usuario
## Tabla de historias de usuario

| Id  | Historia de usuario                                                                               | Prioridad | Estimación | Como probarlo                                                      | Responsable                          |
|-----|---------------------------------------------------------------------------------------------------|-----------|------------|--------------------------------------------------------------------|--------------------------------------|
| 1.1 | Yo como usuario quiero saber el nivel de oxígeno en la sangre.                                     | Alta      | 2 semanas  | Verificar que el sensor de oxígeno muestre valores correctos.      | José Manuel Sánchez Arredondo        |
| 1.2 | Yo como usuario quiero saber los latidos por minuto en mi cuerpo.                                  | Alta      | 2 semanas  | Comprobar que el sensor de frecuencia cardíaca muestre datos.      | Miguel Ángel Jaime García            |
| 1.3 | Yo como usuario quiero saber la temperatura de mi cuerpo.                                          | Alta      | 2 semanas  | Confirmar que el sensor de temperatura corporal funcione bien.     | Omar Alejandro Lozada Zúñiga         |
| 1.4 | Yo como usuario quiero saber la temperatura ambiental.                                             | Media     | 2 semanas  | Verificar que el sensor de temperatura ambiente funcione.          | José Manuel Sánchez Arredondo        |
| 1.5 | Yo como usuario quiero ver los datos de los sensores en pantalla.                                  | Alta      | 3 semanas  | Revisar si los datos se muestran correctamente en la pantalla.     | Miguel Ángel Jaime García            |
| 1.6 | Yo como usuario quiero ver los datos en un dashboard de una app móvil.                             | Alta      | 1 mes      | Confirmar que los datos aparezcan en la app móvil correctamente.   | Omar Alejandro Lozada Zúñiga         |
| 1.7 | Yo como usuario quiero ver los datos en un dashboard de una app para TV.                           | Baja      | 1.5 meses  | Verificar la visualización en la app para TV.                      | José Manuel Sánchez Arredondo        |
| 1.8 | Yo como usuario quiero ver los datos en un dashboard en Node-RED.                                  | Alta      | 3 semanas  | Chequear que los datos se muestren correctamente en Node-RED.      | Miguel Ángel Jaime García            |

## Proceso de desarrollo 
  ### Desarrollo de codigo ESP32
  ![image](https://github.com/user-attachments/assets/b03b06f5-ad07-4fb0-b996-2b865f48fd3a)
  ### Desarrollo de la app mobile/tv
  ![image](https://github.com/user-attachments/assets/6d7e8334-6649-4931-bf16-c1c5a348d6c3)
  ![image](https://github.com/user-attachments/assets/dd8e52c9-8a48-42f5-a442-2dfaeb1b372b)
  ![image](https://github.com/user-attachments/assets/9b93862d-7dd6-44b8-b6e7-4e1392d13483)
  ![image](https://github.com/user-attachments/assets/a84656e5-432d-4b0d-ac6d-70f01175ab53)
  
  ### Tablero de Node-Red
  ![image](https://github.com/user-attachments/assets/6e57020c-bb55-4146-a102-82ded2a0a0f3)
  ![image](https://github.com/user-attachments/assets/35c518f7-d449-4bc7-9afe-c081f95cf3c8)

  ### Prototipo en dibujo
  - Coloca la fotografia de tu prototipo dibujado a lapiz
  ![Imagen de WhatsApp 2024-08-30 a las 16 47 31_c26cc589](https://github.com/user-attachments/assets/5a56d0d7-6e9d-4bde-b527-a0cd2d62929e)

  ### Desarrollo del dispositivo fisico
  ![Imagen de WhatsApp 2024-08-30 a las 16 20 31_7554c5db](https://github.com/user-attachments/assets/8ead3ed7-76ba-4a65-bde6-9af14c80cb7f)
  ![Imagen de WhatsApp 2024-08-30 a las 16 20 29_98fb98ba](https://github.com/user-attachments/assets/c5703bb6-ffdc-4444-a4e7-7845bcc245e1)
  ![Imagen de WhatsApp 2024-08-30 a las 16 20 31_216f99dc](https://github.com/user-attachments/assets/999f9201-d187-4d3e-b2ec-822209f421ed)

## Esquemas
![Imagen de WhatsApp 2024-08-30 a las 17 15 22_d6b0a007](https://github.com/user-attachments/assets/ecb14ffe-ed0a-4a59-8401-9c6d9092c889)

## Video de demostración
[https://drive.google.com/drive/folders/1F5SofnbHtd9WaBUGriVulZW0koL4Zd6w](https://drive.google.com/file/d/1DfGaWIkpq-8bFCmUOfnM2PVuXYo9Nh7a/view?usp=sharing)

## Video con el patrocinador
[https://drive.google.com/file/d/1RpZLSwNEm3pmz2LmaiI-hLeFGBg71rWJ/view?usp=drivesdk](https://drive.google.com/drive/folders/1Rl7_3s5UMmy3nJY9KpyqqC6Uk-CIPy-J) 

## Cartda de agradecimiento
![Imagen de WhatsApp 2024-08-30 a las 22 27 38_42bc0332](https://github.com/user-attachments/assets/27957ea2-c14d-482b-9b9e-77acebb3ec0e)
