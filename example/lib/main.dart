import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:miso/miso.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text('Miso Example (${View.of(context).viewId})'),
        ),
      ),
    );
  }
}
