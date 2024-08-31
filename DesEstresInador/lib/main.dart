import 'dart:async';
import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

// Nuevas importaciones para los gráficos
import 'package:emotion_pulse/charts/temperature_ambient_chart.dart';
import 'package:emotion_pulse/charts/temperature_body_chart.dart';
import 'package:emotion_pulse/charts/heart_rate_chart.dart';
import 'package:emotion_pulse/charts/spo2_chart.dart';
import 'package:emotion_pulse/charts/ir_chart.dart';
import 'package:emotion_pulse/charts/sensores_control.dart';

void main() {
  runApp(AppInitializer());
}

class AppInitializer extends StatefulWidget {
  @override
  _AppInitializerState createState() => _AppInitializerState();
}

class _AppInitializerState extends State<AppInitializer> {
  bool _isLoading = true;
  MqttServerClient? _mqttClient;

  @override
  void initState() {
    super.initState();
    _initializeApp();
  }

  Future<void> _initializeApp() async {
    final client = MqttServerClient.withPort('3.144.180.0', '', 1883);
    client.keepAlivePeriod = 20;
    client.logging(on: true);

    final connMessage = MqttConnectMessage()
        .withClientIdentifier('emotionpulse1523')
        .startClean()
        .withWillQos(MqttQos.atMostOnce);

    client.autoReconnect = true;
    client.connectionMessage = connMessage;

    try {
      await client.connect();

      if (client.connectionStatus?.state == MqttConnectionState.connected) {
        print('MQTT Client Connected');
        setState(() {
          _mqttClient = client;
          _isLoading = false;
        });

        // Suscripciones a los tópicos
        client.subscribe('sensor/temperatura/ambiente', MqttQos.atMostOnce);
        client.subscribe('sensor/temperatura/cuerpo', MqttQos.atMostOnce);
        client.subscribe('sensor/frecuencia_cardiaca/frecuencia', MqttQos.atMostOnce);
        client.subscribe('sensor/oxigeno/spo2', MqttQos.atMostOnce);
        client.subscribe('sensor/oxigeno/ir', MqttQos.atMostOnce);
        // Eliminado: client.subscribe('sensor/estado/alarma', MqttQos.atMostOnce);
      } else {
        print('Connection failed, retrying in 5 seconds...');
        Future.delayed(const Duration(seconds: 5), _initializeApp);
      }
    } catch (e) {
      print('Connection failed: $e, retrying in 5 seconds...');
      Future.delayed(const Duration(seconds: 5), _initializeApp);
    }
  }

  @override
  Widget build(BuildContext context) {
    if (_isLoading) {
      return MaterialApp(
        debugShowCheckedModeBanner: false,
        home: Scaffold(
          body: Center(
            child: CircularProgressIndicator(),
          ),
        ),
      );
    } else {
      return MainApp(client: _mqttClient!);
    }
  }
}

class MainApp extends StatelessWidget {
  final MqttServerClient client;

  const MainApp({super.key, required this.client});

  @override
  Widget build(BuildContext context) {
    final List<Map<String, dynamic>> charts = [
      {'title': 'Temperatura Ambiente', 'widget': TemperatureAmbientChart(topic: 'sensor/temperatura/ambiente', client: client)},
      {'title': 'Temperatura Corporal', 'widget': TemperatureBodyChart(topic: 'sensor/temperatura/cuerpo', client: client)},
      {'title': 'Frecuencia Cardíaca', 'widget': HeartRateChart(topic: 'sensor/frecuencia_cardiaca/frecuencia', client: client)},
      {'title': 'SpO2', 'widget': Spo2Chart(topic: 'sensor/oxigeno/spo2', client: client)},
      {'title': 'IR', 'widget': IrChart(topic: 'sensor/oxigeno/ir', client: client)},
       {'title': 'Control de Actuadores', 'widget': SensoresControl(client: client)},
      // Eliminado: {'title': 'Estado de Alarma', 'widget': AlarmStatusChart(topic: 'sensor/estado/alarma', client: client)},
    ];

    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: Scaffold(
        appBar: AppBar(
          centerTitle: true,
          title: const Text('DesEstresInador'),
          backgroundColor: Colors.deepPurple[300],
        ),
        body: ListView.builder(
          itemCount: charts.length,
          itemBuilder: (context, index) {
            return ListTile(
              title: Text(charts[index]['title']),
              trailing: Icon(Icons.arrow_forward),
              onTap: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(
                    builder: (context) => ChartScreen(
                      title: charts[index]['title'],
                      chartWidget: charts[index]['widget'],
                    ),
                  ),
                );
              },
            );
          },
        ),
      ),
    );
  }
}

class ChartScreen extends StatelessWidget {
  final String title;
  final Widget chartWidget;

  const ChartScreen({super.key, required this.title, required this.chartWidget});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(title),
        backgroundColor: Colors.deepPurple[300],
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: chartWidget,
      ),
    );
  }
}
