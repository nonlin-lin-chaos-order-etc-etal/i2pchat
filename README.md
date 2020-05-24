# I2P Chat AKA I2P Qt Messenger

## Updates now go here

https://github.com/i2pchat/i2pchat has new important fixes

Note: this repo is being eventually migrated to https://github.com/i2pchat/i2pchat which should considered as the main. This repo is deprecated.

## Features

 * The communication goes «directly over i2p» from client to client, no server is required.[1] Note: The connections to the so-called «seedless» servers were removed from this project ; see : https://github.com/nonlinear-chaos-order-etc-etal/i2pchat/issues/40 . —hypn.

## How to run it

You need to enable SAM in your router on <a href="http://127.0.0.1:7657/configclients">java i2p configclients page</a> or i2pd's i2pd.conf [sam] section to make I2P Chat work over your I2P router.

## Security measures

### Since version 0.3.1

When building this, make sure you do not define `DEBUGLOGGING`. If you do define it, the app will log debug info (which includes all chat messages) to `configdir/DebugLog.txt` if set `DebugLogging` to `true` in `General` group in the app's configuration. See `main.cpp` and `form_settingsgui.cpp` for more details on `DEBUGLOGGING`. All versions < 0.3.1 are affected by this and are less secure.
    
## Project status, news and history

### Project status

Now the development was resumed by Hypnosis-i2p and R4SAS, with a lot more folks as testers &amp; bugs+issues reporters.

### Current news

5 Jan, 2017: Original repo at http://git.repo.i2p/w/I2P-Messenger-QT.git was fully merged here.
  
### History

Past. Original developer of this messenger went away.

Present. Hypnosis is continuing his work and the work of I2P folks (of Echelon, KYTV et al); the work is being done in a very low priority mode.

## License

The license of this software is GPLv2.

### Build instructions

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
make -j$(nproc)
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
 * Forum thread (English) → http://forum.i2p/viewtopic.php?t=2474 (link is obsolete; forum is down since long ago)

## List of relevant repositories
 
I created two repos:

 * legacy i2p messenger files https://github.com/nonlinear-chaos-order-etc-etal/I2PQtMessengerLegacy
 * for development https://github.com/nonlinear-chaos-order-etc-etal/i2pchat .

## Footnotes

[1] http://forum.i2p/viewtopic.php?p=11071#11071 (link is obsolete; forum is down since long ago)
