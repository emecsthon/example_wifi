# Nucleo example project with ESP8266 Wi-Fi module

## About

EMECSthon example project which is targeted on Wi-Fi functionality. Alarm-like system which is nofifies the owner of an intrusion. An accelerometer, which is secured on the window, detects and opening or closing movement and send you the email.

## Hardware Prerequisites 

1. Development Board [NUCLEO-F303RE](https://www.st.com/en/evaluation-tools/nucleo-f303re.html)
2. Wi-Fi module [ESP8266-01](https://www.espressif.com/en/products/hardware/esp8266ex/overview)
3. Accelerometer [ADXL335](https://www.analog.com/en/products/adxl335.html#product-overview)

## Getting Started

The are two parts of the system:

1. MBED. 

MBED platform is used to program the MCU

* Create empty MBED project [here](https://ide.mbed.com/compiler)
* Import a project or manually add required libraries:
```

easy-connect
mbed-os
```

2. PHP Server

The PHP capable server is required in order to translate POST request from the MCU to an email. There are many solutions to create it, Code for page PHP example is in `utils/form-to-emails.php`

### TCP requests

Example:

```
POST /test HTTP/1.1
Host: foo.example
Content-Type: application/x-www-form-urlencoded
Content-Length: 27


field1=value1&field2=value2
```
