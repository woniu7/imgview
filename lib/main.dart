import 'package:flutter/material.dart';
import 'package:flutter_staggered_grid_view/flutter_staggered_grid_view.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  MyAppState createState() => MyAppState();
}

class MyAppState extends State<MyApp> {
  ThemeMode _themeMode = ThemeMode.light; // 默认主题模式

  void _toggleTheme() {
    setState(() {
      _themeMode =
          _themeMode == ThemeMode.light ? ThemeMode.dark : ThemeMode.light;
    });
  }

  final TextEditingController _searchQueryController = TextEditingController();
  final FocusNode _focusNode = FocusNode();
  bool _isSearching = false;
  // String _searchQuery = "Search query";

  void _submitSearchQuery(String query) {
    setState(() {
      // _searchQuery = query;
      _isSearching = false;
    });
    // network query
  }

  // void _stopSearching() {
  //   _clearSearchQuery();

  //   setState(() {
  //     _isSearching = false;
  //   });
  // }

  // void _updateSearchQuery(String newQuery) {
  //   setState(() {
  //     _searchQuery = newQuery;
  //   });
  // }

  void _clearSearchQuery() {
    setState(() {
      _searchQueryController.clear();
    });
  }

  void _startSearch() {
    // ModalRoute.of(context)
    //     ?.addLocalHistoryEntry(LocalHistoryEntry(onRemove: _stopSearching));

    setState(() {
      _isSearching = true;
    });
    // Future.delayed(const Duration(milliseconds: 1500), () {
    //   _focusNode.requestFocus();
    // });
  }

  List<Widget> _buildActions() {
    if (_isSearching) {
      return <Widget>[
        IconButton(
          icon: const Icon(Icons.clear),
          onPressed: () {
            if (_searchQueryController.text.isEmpty) {
              setState(() {
                _isSearching = false;
              });
              return;
            }
            _clearSearchQuery();
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
      // onChanged: (query) => _updateSearchQuery(query),
      onSubmitted: (query) => _submitSearchQuery(query),
      enabled: _isSearching,
      focusNode: _focusNode,
    );
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
          const ListTile(),
          ListTile(
            leading: const Icon(Icons.dark_mode),
            title: const Text('Dark mode toggle'),
            onTap: () {
              setState(() {
                //   selectedPage = 'Messages';
                _toggleTheme();
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
    // 搜索框可以聚焦的时候(在本项目相当于搜索框enable状态)就聚焦
    WidgetsBinding.instance.addPostFrameCallback((_) {
      if (_focusNode.canRequestFocus) {
        debugPrint("focus");
        _focusNode.requestFocus();
      }
    });
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

    return MaterialApp(
        title: 'imgtag',
        theme: ThemeData(
          brightness: Brightness.light, // 明亮模式主题
        ),
        darkTheme: ThemeData(
          brightness: Brightness.dark, // 深色模式主题
        ),
        themeMode: _themeMode, // 手动控制主题模式
        home: Scaffold(
            appBar: AppBar(
              // TRY THIS: Try changing the color here to a specific color (to
              // Colors.amber, perhaps?) and trigger a hot reload to see the AppBar
              // change color while the other colors stay the same.
              // backgroundColor: Theme.of(context).colorScheme.inversePrimary,
              // Here we take the value from the MyHomePage object that was created by
              // the App.build method, and use it to set our appbar title.
              title: _buildSearchField(),
              leading: _isSearching
                  ? BackButton(onPressed: () {
                      setState(() {
                        _isSearching = false;
                      });
                    })
                  : null,
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
            )));
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
