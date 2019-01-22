# 🌀 Fam Chat

Anonymous private secure opensource chat using end-to-end encrypted transport.

Previously I2P Chat AKA I2P Qt Messenger.

## Current project status: Wreckage

## For plans description, see: https://github.com/drmagma/i2pchat/issues/85

## Features

 * The communication goes «directly over i2p» from client to client, no server is required. Note: The connections to the so-called «seedless» servers were removed from this project; see: https://github.com/drmagma/i2pchat/issues/40 . —hypn.

## How to run it

To make I2P Chat work over your I2P router, you need to enable SAM in your router:

 * for java i2p: at <a href="http://127.0.0.1:7657/configclients">configclients page</a>, or 
 * for i2pd: at [sam] section of i2pd.conf.

## Security measures

### Since version 0.3.1

When building this, make sure you do not define `DEBUGLOGGING`. If you do define it, the app will log debug info (which includes all chat messages) to `configdir/DebugLog.txt` if set `DebugLogging` to `true` in `General` group in the app's configuration. See `main.cpp` and `form_settingsgui.cpp` for more details on `DEBUGLOGGING`. All versions < 0.3.1 are affected by this and are less secure.
    
## Project status, news and history

### Older Project status

Now the development was resumed by Hypnosis and R4SAS, with a lot more folks as testers &amp; bugs+issues reporters.

### Current news

11 Feb, 2017: I started a great overhaul of the code, I consider the old code junior very bad quality. It's impossible to modify such a chaos. A german coder of the original I2PQtMessenger seems to have known nothing about OOP.

5 Jan, 2017: Original repo at http://git.repo.i2p/w/I2P-Messenger-QT.git was fully merged here.
  
### History

Original developer of this messenger went away.

## License

The license of this software is GPLv2.

## Build instructions

 * Install prerequisites:
```
sudo apt-get install qt5-qmake qt5-default build-essential libqt5multimedia5 qtmultimedia5-dev
```
 * Run qmake for either release:
```
qmake I2P-Messenger.pro "CONFIG += release"
```
or for debug:
```
qmake I2P-Messenger.pro "CONFIG += debug"
```
 * And after that,
```
make -j NUMBER_OF_PROCESSOR_CORES
```
or simply
```
make
```

## Old files and sites

 * http://echelon.i2p/qti2pmessenger/macos → Mac OS X versions, not tested;
 * http://echelon.i2p/qti2pmessenger/ → old eche|on's site for I2P Chat (versions up to 0.2.24);
 * https://launchpad.net/~i2p.packages/+archive/ubuntu/i2p/+sourcepub/4824794/+listing-archive-extra → Kill Your TV's PPA (v.0.2.25).
 * http://git.repo.i2p/w/I2P-Messenger-QT.git/shortlog → original repo (fully merged into this /i2pchat)

## List of relevant repositories
 
I created two repos of github:
 * one for old files of I2P Messenger: https://github.com/drmagma/I2PQtMessengerLegacy ;
 * another for development: https://github.com/drmagma/i2pchat .
