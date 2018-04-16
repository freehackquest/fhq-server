# FreeHackQuest Backend

Backend for FreeHackQuest on Qt and WebSockets
Frontend for FreeHackQuest here: [frontend](https://github.com/freehackquest/frontend)

* [CONFIGURING](install/CONFIGURING.md)
* [HOW-TO-BUILD](install/HOW-TO-BUILD.md)
* [PRE-BUILDS](install/PRE-BUILDS.md)
* Configure autostart
	* [SYSTEMD](install/SYSTEMD.md)
	* [INITD](install/INITD.md)


## (Ubuntu 16.04) Install from ppa:

```
$ sudo add-apt-repository ppa:freehackquest/fhq-server
$ sudo apt update
$ sudo apt install fhq-server
```

After install please try [CONFIGURING](install/CONFIGURING.md)

## 3rdParty

* Contains source code to compile: [SmtpClient-for-Qt](https://github.com/bluetiger9/SmtpClient-for-Qt)
* For Json: [Json](https://github.com/nlohmann/json)
* Using libzip for a working with zip-archives [https://libzip.org/](https://libzip.org/)
