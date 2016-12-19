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

```
qmake-qt4 I2P-Messenger_release.pro
qmake-qt4
make
```
