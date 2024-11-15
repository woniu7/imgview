import 'package:flutter/material.dart';
import 'package:flutter_staggered_grid_view/flutter_staggered_grid_view.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // TRY THIS: Try running your application with "flutter run". You'll see
        // the application has a purple toolbar. Then, without quitting the app,
        // try changing the seedColor in the colorScheme below to Colors.green
        // and then invoke "hot reload" (save your changes or press the "hot
        // reload" button in a Flutter-supported IDE, or press "r" if you used
        // the command line to start the app).
        //
        // Notice that the counter didn't reset back to zero; the application
        // state is not lost during the reload. To reset the state, use hot
        // restart instead.
        //
        // This works for code too, not just values: Most code changes can be
        // tested with just a hot reload.
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.green),
        useMaterial3: true,
      ),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  // int _counter = 0;

  // void _incrementCounter() {
  //   setState(() {
  //     // This call to setState tells the Flutter framework that something has
  //     // changed in this State, which causes it to rerun the build method below
  //     // so that the display can reflect the updated values. If we changed
  //     // _counter without calling setState(), then the build method would not be
  //     // called again, and so nothing would appear to happen.
  //     _counter++;
  //   });
  // }

  @override
  Widget build(BuildContext context) {
    // 获取屏幕的宽度
    double screenWidth = MediaQuery.of(context).size.width;

    // 计算屏幕的三分之一宽度
    double oneThirdWidth = screenWidth / 3;

    // final List<Widget> items = List.generate(
    //     20, (index) => Image.network('https://picsum.photos/250?image=9'));
    // items[1] = Image.network(
    //     'https://fastly.picsum.photos/id/314/536/354.jpg?hmac=uQME2VgtwoULPclwqxIs5OqNE84y_4z4uIGmMgynbLU');
    final List<Widget> items = [
      Image.network(
        'https://picsum.photos/160/160?random=',
        width: 160,
        // fit: BoxFit.cover,
      ),
      Image.network(
        'https://picsum.photos/160/200?random=',
        width: 160,
        // fit: BoxFit.cover,
      ),
      Image.network(
        'https://picsum.photos/160/160?random=',
        width: 160,
        // fit: BoxFit.cover,
      ),
      Image.network(
        'https://picsum.photos/160/160?random=',
        width: 160,
        // fit: BoxFit.cover,
      ),
      Image.network(
        'https://picsum.photos/160/300?random=',
        width: 160,
        // fit: BoxFit.cover,
      ),
    ];

    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.
    return Scaffold(
        appBar: AppBar(
          // TRY THIS: Try changing the color here to a specific color (to
          // Colors.amber, perhaps?) and trigger a hot reload to see the AppBar
          // change color while the other colors stay the same.
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
          // Here we take the value from the MyHomePage object that was created by
          // the App.build method, and use it to set our appbar title.
          title: Text(widget.title),
        ),
        body: MasonryGridView.extent(
          itemCount: 5,
          maxCrossAxisExtent: 160,
          // mainAxisSpacing: 4,
          // crossAxisSpacing: 4,
          itemBuilder: (context, index) {
            return items[index];
            // return ImageTile(
            //     index: index, width: 200, height: 150 + 10 * index);
          },
        ));
  }
}

class ImageTile extends StatelessWidget {
  const ImageTile({
    Key? key,
    required this.index,
    required this.width,
    required this.height,
  }) : super(key: key);

  final int index;
  final int width;
  final int height;

  @override
  Widget build(BuildContext context) {
    return Image.network(
      'https://picsum.photos/$width/$height?random=$index',
      width: width.toDouble(),
      height: height.toDouble(),
      fit: BoxFit.cover,
    );
  }
}
