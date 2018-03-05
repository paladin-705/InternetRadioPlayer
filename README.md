Internet Radio Player
=========================
Cross-platform internet radio player program 

Folder structure
------------
    .
    ├── import
    ├── lib
    │   ├── linux-arm
    │   ├── linux
    │   └── windows
    ├── playerClients                             # Client programs for this player
    │   └── InternetRadioPlayerController_QT
    ├── src
    │   └── includes
    ├── .gitignore
    ├── init_repository                           #Init repository script for linux system
    ├── InternetRadioPlayer.pro
    └── README.md

INSTALLATION
------------
### Linux:
Clone repository and execute init_repository bash script: 
```shell
git clone https://github.com/paladin-705/InternetRadioPlayer
cd InternetRadioPlayer
sudo init_repository -s <system: linux or linux-arm>
```
init_repository options:

     -s		select system (linux or linux-arm)
     -h		print help
This script install libs and make directories
### Windows:
Coming soon

REQUIREMENTS
------------
This program required QT 5 and QT 5.6 or greater for InternetRadioPlayerController_QT
