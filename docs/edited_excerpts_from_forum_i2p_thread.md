# Edited excerpts from forum.i2p thread http://forum.i2p/viewtopic.php?t=2474

## Initial Version — posted: Jan 3, 2008 10:24 pm
	
This messenger is a very early beta release.

### Features

 * Online States: like invisible, want to chat etc
 * Sound Events: like a new chat message etc
 * Chat Messages: formatted text with colors

### Security

 * Block / unblock a user
 * Block all unknown users
 * Request authorization

### Details

 * The client works with the I2P SAM protocol; default port: 7656 TCP.
 * The communication goes «direct over i2p» from client to client, no server is required.
 * […]

### Build instructions

With both Debian and Ubuntu this can be built with just qt4-qmake & libqt4-dev. Of course when compiling software you'll want to "build-essential" installed too:
```
sudo apt-get install qt4-qmake libqt4-dev build-essential
```

After these are installed, building should be possible with just
```
qmake-qt4 I2P-Messenger_release.pro
make
```
