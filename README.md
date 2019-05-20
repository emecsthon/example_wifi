# Nucleo example project with ESP8266 Wi-Fi module


## About


EMECSthon example project which is targeted on Wi-Fi functionality. Alarm-like system which nofifies the owner of an intrusion. An accelerometer, which is secured on the window, detects and opening or closing movement and send you the email.


## Hardware Prerequisites 


1. Development Board [NUCLEO-F303RE](https://www.st.com/en/evaluation-tools/nucleo-f303re.html)
2. Wi-Fi module [ESP8266-01](https://www.espressif.com/en/products/hardware/esp8266ex/overview)
3. Accelerometer [ADXL335](https://www.analog.com/en/products/adxl335.html#product-overview)


## Getting Started


The are two parts of the system: MBED code for the MCU and Mailing webserver. MCU will detect that the window was opened and it will send a corresponding POST request to the web server, which will convert it into an email and send it to the owner.


### PHP Server


* Create a web server


In order to provide a reliable mailing system a dedicated web server is required. It can be hosted locally or remotely. Even though local web server is not recommended for sending emails, there are ways of doing it:


1. Windows: [Pegasus Mail](http://www.pmail.com/)
2. Debian: [php.net](https://www.php.net/manual/en/install.unix.debian.php) + [PHPmailer](https://sourceforge.net/projects/phpmailer/)


or, alternatively, use a remote hosting platforms **(recommended)**. Second one is preferable, because it provides lower probability of your emails getting into the spam folder


* Copy the code for the POST request to Email at `utils/form-to-emails.php`


### MBED. 


MBED platform is used to program the MCU


* Create empty MBED project [here](https://ide.mbed.com/compiler)
* Import a project or manually add required libraries:
```

easy-connect <= for connecting ESP8266
mbed-os

```
* Import `main.cpp` (or copy it)
* Put the credentials of your Wi-Fi in `mbedos-app.jsom` and change the address of your POST request in `main.cpp`

```
    char sbuffer_window_opened[] = "POST /path/to/form-to-emails.php HTTP/1.1\r\nHost: HOST_NAME ...

```
```

    result = socket.connect("IP_ADDRESS", 80);
    if (result != 0) {

        pc.printf("Error! socket.connect() returned: %d\n", result);
    }
```

* Compile


### Assembling map


![GPIO-left-nucleo-f303re](utils/img/gpio_left.png?raw=true "GPIO-left-nucleo-f303re")


![GPIO-right-nucleo-f303re](utils/img/gpio_right.png?raw=true "GPIO-right-nucleo-f303re")


* Connect ESP8266


![ESP8266-GPIO](utils/img/ESP8266.png?raw=true "ESP8266-GPIO")


| ESP8266  | Nucleo F303 |
| ------------- | ------------- |
| VCC  | 3.3V  |
| GND  | GND  |
| TX  | D2 (RX) |
| RX  | D8 (TX)  |
| GPIO0  | 3.3V  | 
| GPIO2  | 3.3V  |
| CH_EN  | 3.3V  |
| RST  | 3.3V  |


* Connect Accelerometer


| ADXL335  | Nucleo F303 |
| ------------- | ------------- |
| VCC  | 3.3V  |
| GND  | GND  |
| X  | A0  |
| Y  | A1  |
| Z  | A2  | 


* Power up and upload it on the board 
* Wait until system is initialized and put it in `active` state. (See *Troubleshooting* if it can't be reached) 


## Functionality


System has two main states (besides sending request and initializing): `active` and `inactive`. `Inactive` state disables Wi-Fi communication to prevent the system from sending notification when they are not required (e.g. when you are at home). `Active` state allows notifying the owner, when the windows open or closes. The only user button is used to toggle between these states. **It is important to note** that the button triggers the **interrupt** which sets the **flag**! and and **main loop** checks if the flag was raised. The routing of sending a request may take a substantial time, thus increasing the chances of getting another interrupt, if this interrupt stats the sending request routine while previous hasn't been finished, the integrity will be violated. However by creating an event, incoming interrupt isn't capable to start sending a request before the end of a previous request. 


## LED


LED is conveniently used to display the current state of the system. 


### Constantly ON


The system is in the initialization state. During which a Wi-Fi connection to the router is established


### Slow blinking


*(2 seconds period)* Inactive state. Wi-Fi communication is disabled, such that controller won't send any notifications


### Fast blinking


*(less than a second period)* Active state. The controller is allowed to communicate through the wifi and ready to send notifications


### Constantly OFF


Unsupported state


## Troubleshooting

Here, several typical error sources and their troubleshooting methods are outlined:


### Serial bus


First order of business is to check if ESP8266 is talking to the MCU. The easiest way to determine this is to observe the output of the system by connecting it to the terminal via the serial bus. By default all AT communication with WiFi module are mirrored into the serial connection to the PC. Thus, if you can't see the response, the link between them doesn't exist. One way to resolve this is to check the pin connection or update ESP8266 Firmware *(See below)*. If the wrong credentials were put, Wi-Fi module wouldn't return`WIFI CONNECTED`.


### Requests


To check the content of the request, we can redirect it to [ptsv2.com](https://ptsv2.com). This way you can see the full dump of the received request. 
Many times requests are misspelled because they are raw


Example:


```
POST /test HTTP/1.1
Host: foo.example
Content-Type: application/x-www-form-urlencoded
Content-Length: 27




field1=value1&field2=value2
```


## Updating ESP8266 Firmware


* Put the ESP8266 device into UART Download mode


| ESP8266  | Nucleo F303 |
| ------------- | ------------- |
| VCC  | 3.3V  |
| GND  | GND  |
| TX  | D2 (RX) |
| RX  | D8 (TX)  |
| GPIO0  | GND  | 
| GPIO2  | 3.3V  |
| CH_EN  | 3.3V  |
| RST  | 3.3V  |


* Install mbed and esp tools on your PC


```
# pip install mbed
# pip install esptool
```


* Check if the device is recognise by the system


```
# mbedls
| platform_name | platform_name_unique | mount_point             | serial_port  | target_id | daplink_version |
|---------------|----------------------|-------------------------|--------------|-----------|-----------------|
| NUCLEO        | NUCLEO-F303          | /path/to/mount/point    | /dev/ttyACM0 | ....      | 0250            |
```


* Check the parameters of your ESP8266


```
$ esptool.py --port /dev/ttyACM0 --baud 115200 flash_id
esptool.py v2.5.2
Serial port /dev/ttyACM0
Connecting......
Detecting chip type... ESP8266
Chip is ESP8266EX
Features: WiFi
MAC: XX:XX:XX:XX:XX:XX
Uploading stub...
Running stub...
Stub running...
Manufacturer: 1c
Device: 3014
Detected flash size: 1MB  <-- In our case it is 1MByte  = 8 Mbits
Hard resetting via RTS pin...
```


* Download new Firmware from the official website [here](https://www.espressif.com/sites/default/files/ap/ESP8266_AT_Bin_V1.6.2_0.zip)


* Flash the Firmware


```
$ esptool.py --port /dev/ttyACM0 --baud 19200 write_flash --flash_size 1MB --flash_mode dio 0x00000 boot_v1.6.bin 0x01000 at/512+512/user1.1024.new.2.bin 0xFC000 esp_init_data_default_v08.bin 0x7E000 blank.bin 0xFE000 blank.bin
```


* Now you should have an updated Frimware on your ESP8266-01
<!--
## ESP8266-01 GPIO


![ESP8266-GPIO](utils/img/ESP8266.png?raw=true "ESP8266-GPIO")


## Nucleo F303RE headers


![GPIO-left-nucleo-f303re](utils/img/gpio_left.png?raw=true "GPIO-left-nucleo-f303re")


![GPIO-right-nucleo-f303re](utils/img/gpio_right.png?raw=true "GPIO-right-nucleo-f303re")
-->
## Full System view


![Overview](utils/img/overview.jpg?raw=true "Overview")


## Authors
[Kirill Bykov](https://github.com/kibk/)
