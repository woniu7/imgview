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
      debugShowCheckedModeBanner: false,
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
      darkTheme: ThemeData.dark(),
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
  final TextEditingController _searchQueryController = TextEditingController();
  bool _isSearching = false;
  String _searchQuery = "Search query";
  Widget _buildSearchField() {
    return TextField(
      controller: _searchQueryController,
      // autofocus: true,
      decoration: const InputDecoration(
        // hintText: "Search Data...",
        border: InputBorder.none,
        // hintStyle: TextStyle(color: Colors.white30),
      ),
      // style: TextStyle(color: Colors.white, fontSize: 16.0),
      // onChanged: (query) => updateSearchQuery(query),
    );
  }

  void _stopSearching() {
    _clearSearchQuery();

    setState(() {
      _isSearching = false;
    });
  }

  void _updateSearchQuery(String newQuery) {
    setState(() {
      _searchQuery = newQuery;
    });
  }

  void _clearSearchQuery() {
    setState(() {
      _searchQueryController.clear();
      _updateSearchQuery("");
    });
  }

  void _startSearch() {
    ModalRoute.of(context)
        ?.addLocalHistoryEntry(LocalHistoryEntry(onRemove: _stopSearching));

    setState(() {
      _isSearching = true;
    });
  }

  List<Widget> _buildActions() {
    if (_isSearching) {
      return <Widget>[
        IconButton(
          icon: const Icon(Icons.clear),
          onPressed: () {
            // if (searchQueryController.text.isEmpty) {
            //   Navigator.pop(context);
            //   return;
            // }
            // clearSearchQuery();
          },
        ),
      ];
    } else {
      return <Widget>[
        IconButton(
          icon: const Icon(Icons.search),
          onPressed: _startSearch,
        ),
      ];
    }
  }

  Drawer _buildDrawer() {
    return Drawer(
      child: ListView(
        padding: EdgeInsets.zero,
        children: <Widget>[
          // const DrawerHeader(
          //   decoration: BoxDecoration(
          //     color: Colors.blue,
          //   ),
          //   child: Text(
          //     'Drawer Header',
          //     style: TextStyle(
          //       color: Colors.white,
          //       fontSize: 24,
          //     ),
          //   ),
          // ),
          ListTile(
            leading: const Icon(Icons.dark_mode),
            title: const Text('Dark mode'),
            onTap: () {
              setState(() {
                MediaQuery.of(context).platformBrightness == Brightness.dark;
                //   selectedPage = 'Messages';
              });
            },
          ),
          ListTile(
            leading: const Icon(Icons.question_mark),
            title: const Text('About'),
            onTap: () {
              // setState(() {
              //   selectedPage = 'Messages';
              // });
            },
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    // 获取屏幕的三分之一宽度
    double imgWide = MediaQuery.of(context).size.width / 3;
    if (imgWide > 160) {
      imgWide = 160;
    }

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
          title: _isSearching ? _buildSearchField() : Text(widget.title),
          leading: _isSearching ? const BackButton() : null,
          actions: _buildActions(),
        ),
        drawer: _buildDrawer(),
        body: MasonryGridView.extent(
          itemCount: items.length,
          maxCrossAxisExtent: imgWide,
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
