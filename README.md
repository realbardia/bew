# Bew

## How to

Bew help you to convert Web apps to Desktop apps using below command:

```bash
bew "https://twitter.com" --app-name "Twitter" --app-icon "/path/to/icon.png" --font "Ubuntu" --no-scrollbar 
```

## Build

Bew needs QtWebEngine to build. So you must install it first:

```bash
sudo apt install git g++ qtbase5-dev qtwebengine5-dev
```

and then just build it

```bash
git clone git@github.com:realbardia/bew.git
cd bew
qmake
make
sudo make install
```
