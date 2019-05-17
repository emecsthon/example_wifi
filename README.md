# Nucleo example project with ESP8266 Wi-Fi module

## About

EMECSthon example project which is targeted on Wi-Fi functionality. Alarm-like system which is nofifies the owner of an intrusion. An accelerometer, which is secured on the window, detects and opening or closing movement and send you the email.

## Hardware Prerequisites 

1. Development Board [NUCLEO-F303RE](https://www.st.com/en/evaluation-tools/nucleo-f303re.html)
2. Wi-Fi module [ESP8266-01](https://www.espressif.com/en/products/hardware/esp8266ex/overview)
3. Accelerometer [ADXL335](https://www.analog.com/en/products/adxl335.html#product-overview)

## Getting Started

The are two parts of the system: MBED code for the MCU and Mailing webserver. MCU will detect that the window was opened and send the correcponsing POST request to the webserver, which will convert it into an email and send it to the owner.

1. PHP Server

* Create a webserver

In order to provide a reliable mailing system a dedicated webserver is required. It can be hosted locally or remotly. One of the ways to create a local host is to use [php.net](https://www.php.net/manual/en/install.unix.debian.php) or use a remote hosting platforms. Second one is preferable, because it provide lower probability of your emails getting into the spam folder

* Copy the code for the POST request to Email at `utils/form-to-emails.php`

2. MBED. 

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
    char sbuffer_window_opened[] = "POST /path/to/form-to-emails.php HTTP/1.1\r\nHost: HOST_NAME\r\nContent-Type: ...
```
```
    result = socket.connect("IP_ADDRESS", 80);
    if (result != 0) {
        pc.printf("Error! socket.connect() returned: %d\n", result);
    }
```
* Compile and upload it on the board

## Troublshooting

Several typical error sources are outlined

### Serial bus

The easiest way to troubleshoot the unexpected behaviour is to observe the output of the system by connecting it to the terminal via a serial bus. By default all AT comunication with WiFi module are mirrored into the serial connection to the PC. 

### Requests

To check the content of the request, we can redirect it to [ptsv2.com](https://ptsv2.com). This way you can see the full dump of the received request.

### TCP requests

Example:

```
POST /test HTTP/1.1
Host: foo.example
Content-Type: application/x-www-form-urlencoded
Content-Length: 27


field1=value1&field2=value2
```
