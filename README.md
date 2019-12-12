# avr_rtos_orgel_project
electirc orgel based on UCOS-II ported Atmega128

 
 Dependancies
 -----
 You need winAVR-20100110 and cmake to build.
 
 Downloading dependancies 
 WinAVR-20100110 [here](https://ko.osdn.net/projects/sfnet_winavr/downloads/WinAVR/20100110/WinAVR-20100110-install.exe/)
 and 
 msys-1.0.dll [here](https://t1.daumcdn.net/cfile/tistory/2173C943559AA7A222?download)


> * ***waring***
winavr-20100110 installer has bug on PATH including, you need to uncheck "Add directories to PATH" option and add maually.



After install, you need to  **replace** msys-1.0.dll file in WinAVR-20100110\bin folder to given file(724,784 byte)


 
 Build guide
 -----
 make WinAVR-RTOS folder on C drive.
clone this repository in the folder
run make on **avr_rtos_orgel_project\app** folder
