# Nucleo example project with ESP8266 Wi-Fi module

## About

EMECSthon example project which is targeted on Wi-Fi functionality. Alarm-like system which is nofifies the owner of an intrusion. An accelerometer, which is secured on the window, detects and opening or closing movement and send you the email.

## Hardware Prerequisites 

1. Development Board [NUCLEO-F303RE](https://www.st.com/en/evaluation-tools/nucleo-f303re.html)
2. Wi-Fi module [ESP8266-01](https://www.espressif.com/en/products/hardware/esp8266ex/overview)
3. Accelerometer [ADXL335](https://www.analog.com/en/products/adxl335.html#product-overview)

## Getting Started

The are two parts of the system: MBED code for the MCU and Mailing webserver. MCU will detect that the window was opened and send the correcponsing POST request to the webserver, which will convert it into an email and send it to the owner.

### PHP Server

* Create a webserver

In order to provide a reliable mailing system a dedicated webserver is required. It can be hosted locally or remotly. Even though local webserver is not recommended for sending emails, there are ways of doing it:

1. Windows: [Pegasus Mail](http://www.pmail.com/)
2. Debian: [php.net](https://www.php.net/manual/en/install.unix.debian.php) + [PHPmailer](https://sourceforge.net/projects/phpmailer/)

or, alternatively, use a remote hosting platforms **(recommentded)**. Second one is preferable, because it provides lower probability of your emails getting into the spam folder

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
* Compile and upload it on the board

### Assembling map

* Wait until system initializes and put it in `active` state. (See *Troubleshooting* if it can't be reached) 

## Functionality

Systtem has two main states (besides sending request and initializing): `active` and `inactive`. `Inactive` state disables Wi-Fi communication to prevent the system from sending notification when they are not required (e.g. when you are at home). Oppositely `active` state allows notifying the owner, when the windows open or closes. The only user button is used to toggle between these states. **It is important to note** that the button triggers the **interrupt** which creates an **event**! which is surved in the **main loop**. The routing of sending a request may take a substantial time, thus increasing the chanses of getting another interrupt, if this interrupt stats the sending request routine while previous hasn't beed finished, the integrity will be violated. However by creating an event, incomming interrupt isn't capable to start sending a request before the end of a previous request. 

## LED

LED is conviniently used to display the current state of the system. 

### Constantly ON

The system is in the initialization state. During which a Wi-Fi connection to the router is established

### Slow blinking

*(2 seconds period)* Inactive state. Wi-Fi communication is disabled, such that controller won't send any notifications

### Fast blinking

*(less than a second period)* Active state. The controller is allowed to communicate through the wifi and ready to send notifications

### Constantly OFF

Unsupported state

## Troublshooting

Here, several typical error sources and their troubleshooting methods are outlined:

### Serial bus

First order of busyness is to check if ESP8266 is talking to the MCU. The easiest way to determine this is to observe the output of the system by connecting it to the terminal via the serial bus. By default all AT comunication with WiFi module are mirrored into the serial connection to the PC. Thus, if you can't see the responce, the link between them doesn't exist. One way to resolve this is to check the pin connection or update ESP8266 Firmware *(See below)*. If the wrong credentials were put, Wi-Fi module wouldn't return`WIFI CONNECTED`.

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

## Nucleo F303RE headers

![GPIO-left-nucleo-f303re](utils/img/gpio-left.png?raw=true "GPIO-left-nucleo-f303re")

![GPIO-right-nucleo-f303re](utils/img/gpio-right.png?raw=true "GPIO-right-nucleo-f303re")

## Full System view

![Overview](utils/img/overview.jpg?raw=true "Overview")

## Authors
[Kirill Bykov](https://github.com/kibk/)


