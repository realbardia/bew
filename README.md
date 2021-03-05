# Bew

## How to

Bew help you to convert Web apps to Desktop apps using below command:

```bash
bew "https://twitter.com" --app-name "Twitter" --app-icon "/path/to/icon.png" --font "Ubuntu" --no-scrollbar --user-agent "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:86.0) Gecko/20100101 Firefox/86.0"
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
