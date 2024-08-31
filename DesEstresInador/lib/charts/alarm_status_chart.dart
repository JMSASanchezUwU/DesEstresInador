import 'package:flutter/material.dart';
import 'package:fl_chart/fl_chart.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';

class AlarmStatusChart extends StatefulWidget {
  final MqttServerClient client;
  final String topic;

  const AlarmStatusChart({super.key, required this.client, required this.topic});

  @override
  _AlarmStatusChartState createState() => _AlarmStatusChartState();
}

class _AlarmStatusChartState extends State<AlarmStatusChart> {
  List<FlSpot> _spots = [];

  @override
  void initState() {
    super.initState();
    widget.client.updates!.listen(_onMessage);
  }

  void _onMessage(List<MqttReceivedMessage<MqttMessage>> event) {
    final message = event[0].payload as MqttPublishMessage;
    final payload = MqttPublishPayload.bytesToStringAsString(message.payload.message);
    final value = double.tryParse(payload);

    if (event[0].topic == widget.topic && value != null) {
      setState(() {
        _spots.add(FlSpot(_spots.length.toDouble(), value));
        if (_spots.length > 20) {
          _spots.removeAt(0);
        }
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(8.0),
      child: LineChart(
        LineChartData(
          titlesData: FlTitlesData(show: true),
          borderData: FlBorderData(show: true),
          gridData: FlGridData(show: true),
          lineBarsData: [
            LineChartBarData(
              spots: _spots,
              isCurved: true,
              barWidth: 4,
              color: Colors.purple, // Definiendo el color de la línea
              belowBarData: BarAreaData(
                show: true,
                color: Colors.purple.withOpacity(0.3), // Definiendo el color del área bajo la línea
              ),
            ),
          ],
        ),
      ),
    );
  }
}
