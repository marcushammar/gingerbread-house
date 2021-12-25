import 'package:flutter/material.dart';
import 'package:http/http.dart';
import 'credentials.dart';

void main() => runApp(GingerbreadHouseRemote());

void sendCommand(String feed, String value) async {
  print('Sending value ' + value + ' to feed ' + feed);

  var endpoint = baseUrl + '/' + feed + '/data';
  var headers = {
    "Content-Type": "application/json",
    "X-AIO-Key": apiKey,
  };
  var body = '{"value": ' + value + '}';

  var response = await post(Uri.parse(endpoint), headers: headers, body: body);

  print('Server status code: ' + response.statusCode.toString());
  print('Server response: ' + response.body);
}

class GingerbreadHouseRemote extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Text('Gingerbread House Remote'),
        ),
        body: SingleChildScrollView(
          child: Column(
            children: [
              GridView.count(
                shrinkWrap: true,
                padding: const EdgeInsets.only(
                  top: 20,
                  right: 20,
                  bottom: 10,
                  left: 20,
                ),
                crossAxisCount: 2,
                mainAxisSpacing: 10,
                crossAxisSpacing: 10,
                childAspectRatio: 2,
                physics: const NeverScrollableScrollPhysics(),
                children: [
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Colors.red,
                    ),
                    icon: const Icon(Icons.lightbulb),
                    label: const Text('Red'),
                    onPressed: () => sendCommand(feedLights, "1"),
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Colors.green,
                    ),
                    icon: const Icon(Icons.lightbulb),
                    label: const Text('Green'),
                    onPressed: () => sendCommand(feedLights, "2"),
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Colors.blue,
                    ),
                    icon: const Icon(Icons.lightbulb),
                    label: const Text('Blue'),
                    onPressed: () => sendCommand(feedLights, "3"),
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Colors.yellow,
                    ),
                    icon: const Icon(Icons.lightbulb),
                    label: const Text('Yellow'),
                    onPressed: () => sendCommand(feedLights, "4"),
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Colors.pink,
                    ),
                    icon: const Icon(Icons.lightbulb),
                    label: const Text('Pink'),
                    onPressed: () => sendCommand(feedLights, "5"),
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Color.fromRGBO(0, 255, 255, 1.0),
                    ),
                    icon: const Icon(Icons.lightbulb),
                    label: const Text('Turquoise'),
                    onPressed: () => sendCommand(feedLights, "6"),
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Colors.grey,
                    ),
                    icon: const Icon(Icons.lightbulb_outline),
                    label: const Text('No light'),
                    onPressed: () => sendCommand(feedLights, "0"),
                  ),
                ],
              ),
              GridView.count(
                shrinkWrap: true,
                padding: const EdgeInsets.only(
                  top: 10,
                  right: 20,
                  bottom: 20,
                  left: 20,
                ),
                crossAxisCount: 2,
                mainAxisSpacing: 10,
                crossAxisSpacing: 10,
                childAspectRatio: 2,
                physics: const NeverScrollableScrollPhysics(),
                children: [
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Colors.grey,
                    ),
                    icon: const Icon(Icons.directions_run),
                    label: const Text('Start dancing'),
                    onPressed: () => sendCommand(feedDance, "1"),
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(
                      primary: Colors.grey,
                    ),
                    icon: const Icon(Icons.block),
                    label: const Text('Stop dancing'),
                    onPressed: () => sendCommand(feedDance, "0"),
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}
