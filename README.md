# I2P Qt Chat AKA Messenger

## Features

The communication goes «directly over i2p» from client to client, no server is required.[1]

## How to run it

You need to enable SAM in your router on <a href="http://127.0.0.1:7657/configclients">java i2p configclients page</a> or i2pd's i2pd.conf [sam] section to make I2P Chat work!

## Project News and History

Now the development was resumed by Hypnosis-i2p and r4sas, with a lot more folks as testers &amp; bugs+issues reporters.

Original developer of this messenger went away.
  
## License

The license of this software is GPLv2.

## Old Files and Sites

 * http://echelon.i2p/qti2pmessenger/macos → Mac OS X versions, not tested;
 * http://echelon.i2p/qti2pmessenger/ → old eche|on's site for I2P Chat
 * https://launchpad.net/~i2p.packages/+archive/ubuntu/i2p/+sourcepub/4824794/+listing-archive-extra → Kill Your TV's PPA (v.0.2.25)

## Forum thread (English)

http://forum.i2p/viewtopic.php?t=2474

## Notes (in Russian)
 
я два репа на гитхабе создал
 * один для антиквариата по части и2п массажника https://github.com/hypnosis-i2p/I2PQtMessengerLegacy
 * другой для разработки, я заплоадил туда 0.2.25 https://github.com/hypnosis-i2p/i2pqtchat

## Build Requirements (in Russian)

Q: 0.2.25 на пятом QT?
A: нет
   нужно качать Qt 4.8.6 https://download.qt.io/archive/qt/4.8/4.8.6/ и собирать его
   и отдельно качать Qt Creator https://www.qt.io/download/ - если креатор у тебя есть, просто добавишь ему QT SDK 4.8.6
   
Q: а разве apt-get install qt4-qmake не хватит?
A: не пробовал, не знаю

## Footnotes

[1] http://forum.i2p/viewtopic.php?p=11071#11071
