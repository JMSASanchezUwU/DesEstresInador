import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

class SensoresControl extends StatefulWidget {
  final MqttServerClient client;

  SensoresControl({Key? key, required this.client}) : super(key: key);

  @override
  _SensoresControlState createState() => _SensoresControlState();
}

class _SensoresControlState extends State<SensoresControl> {
  bool _ledVerdeState = true;
  bool _ledRojoState = true;
  bool _buzzerState = true;
  bool _motorState = true;

  void _publishMessage(String topic, String message) {
    final builder = MqttClientPayloadBuilder();
    builder.addString(message);
    widget.client.publishMessage(topic, MqttQos.atMostOnce, builder.payload!);
  }

  void _toggleLedVerde(bool state) {
    setState(() {
      _ledVerdeState = state;
    });

    if (state) {
      _publishMessage('actuadores/ledVerde', 'on');
    } else {
      _publishMessage('actuadores/ledVerde', 'off');
    }
  }

  void _toggleLedRojo(bool state) {
    setState(() {
      _ledRojoState = state;
    });

    if (state) {
      _publishMessage('actuadores/ledRojo', 'on');
    } else {
      _publishMessage('actuadores/ledRojo', 'off');
    }
  }

  void _toggleBuzzer(bool state) {
    setState(() {
      _buzzerState = state;
    });

    if (state) {
      _publishMessage('actuadores/buzzer', 'on');
    } else {
      _publishMessage('actuadores/buzzer', 'off');
    }
  }

  void _toggleMotor(bool state) {
    setState(() {
      _motorState = state;
    });

    if (state) {
      _publishMessage('actuadores/motor', 'on');
    } else {
      _publishMessage('actuadores/motor', 'off');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            SwitchListTile(
              title: const Text('LED Verde'),
              value: _ledVerdeState,
              onChanged: _toggleLedVerde,
            ),
            SwitchListTile(
              title: const Text('LED Rojo'),
              value: _ledRojoState,
              onChanged: _toggleLedRojo,
            ),
            SwitchListTile(
              title: const Text('Control del Buzzer'),
              value: _buzzerState,
              onChanged: _toggleBuzzer,
            ),
            SwitchListTile(
              title: const Text('Control del Motor'),
              value: _motorState,
              onChanged: _toggleMotor,
            ),
          ],
        ),
      ),
    );
  }
}
