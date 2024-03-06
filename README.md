# BEW Store

## How to

BEW is a store and tool, help you to convert Web apps to Desktop web apps (bew) using a graphical User interface. Just run:

```bash
bew
```

But you can also run it using command line:

```bash
bew install trello
bew --help
```

## Build

BEW needs QtWebEngine to build. So you must install it first:

```bash
sudo apt install git g++ qtbase5-dev qtwebengine5-dev
```

and then just build it

```bash
git clone git@github.com:realbardia/bew.git
cd bew
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr ..
make -j4
sudo make install
```

