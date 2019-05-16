#include "mbed.h"
// For the Wi-Fi
//#include "ESP8266Interface.h"
// Easy connet library for the HTTP server
#include "easy-connect.h"
// For the Web Server
//#include "http_server.h"
//#include "http_response_builder.h"
// TCP libraries
//#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"

Serial pc(USBTX, USBRX);
DigitalOut led(LED1); 
//ESP8266Interface net(D8, D2, true); // WiFi driver
InterruptIn mybutton(USER_BUTTON);
TCPSocket socket; // Socket for sending requests

// LED
DigitalOut led1(LED1);
// Accelerometer
AnalogIn x(A2); 
AnalogIn y(A1); 
AnalogIn z(A0);

bool openClosed = false; // Window
bool wifiAllowed = false; // Allow communication

// Interrupt function
void pressed() {
    pc.printf("\n\r=========Button Interrupt Came=============");
    // Allow wifi send emails
    wifiAllowed = !wifiAllowed;
}

// test Function
void test()
{
//    pc.printf("\r\nAccelerometer X : %f\n\r", x.read()*3.3);
//    pc.printf("\r\nAccelerometer Y : %f\n\r", y.read()*3.3);
//    pc.printf("\r\nAccelerometer Z : %f\n\r", z.read()*3.3);
}

void send_request(NetworkInterface* network, bool openedOrClosed) {
//    int remaining;
//    int rcount;
//    char *p;
    char *buffer = new char[256];
        // Open a socket on the network interface, and create a TCP connection to ifconfig.io
    TCPSocket socket;
    // Send a simple http request
    char sbuffer_window_closed[] = "POST /scripts/form-to-email.php HTTP/1.1\r\nHost: form.tw1.ru\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 20\r\n\r\nfield1=window-closed\r\n";
    char sbuffer_window_opened[] = "POST /scripts/form-to-email.php HTTP/1.1\r\nHost: form.tw1.ru\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 20\r\n\r\nfield1=window-opened\r\n";
    nsapi_size_t size;
    if (openedOrClosed) {
        size = strlen(sbuffer_window_closed);
    } else {
        size = strlen(sbuffer_window_opened);
    }

    nsapi_size_or_error_t result;
    result = socket.open(network);
    if (result != 0) {
        pc.printf("Error! socket.open() returned: %d\n", result);
    }

    result = socket.connect("form.tw1.ru", 80);
    if (result != 0) {
        pc.printf("Error! socket.connect() returned: %d\n", result);
    }

    // Loop until whole request sent
    if (openedOrClosed) {
        while(size) {
            result = socket.send(sbuffer_window_closed+result, size);
            if (result < 0) {
                pc.printf("Error! socket.send() returned: %d\n", result);
            }
            size -= result;
            pc.printf("sent %d [%.*s]\n", result, strstr(sbuffer_window_closed, "\r\n")-sbuffer_window_closed, sbuffer_window_closed);
        }
    } else {
            result = socket.send(sbuffer_window_opened+result, size);
            if (result < 0) {
                pc.printf("Error! socket.send() returned: %d\n", result);
            }
            size -= result;
            pc.printf("sent %d [%.*s]\n", result, strstr(sbuffer_window_opened, "\r\n")-sbuffer_window_opened, sbuffer_window_opened);
    }

    // Receieve an HTTP response and print out the response line
//    remaining = 256;
//    rcount = 0;
//    p = buffer;
//    while (remaining > 0 && 0 < (result = socket.recv(p, remaining))) {
//        p += result;
//        rcount += result;
//        remaining -= result;
//    }
//    if (result < 0) {
//        pc.printf("Error! socket.recv() returned: %d\n", result);
//    }
//    // the HTTP response code
//    pc.printf("recv %d [%.*s]\n", rcount, strstr(buffer, "\r\n")-buffer, buffer);

    delete[] buffer;

    // Close the socket to return its memory and bring down the network interface
    socket.close();
}

// main() runs in its own thread in the OS
int main() {
    led1 = true;
    // Instanciate interrupts
    mybutton.fall(&pressed);
    

    pc.printf("\r\nNUCLEO-mbed Application\r\n");     
    
    // ================================================
    // Establish a connection
    // ================================================
    pc.printf("\r\nconnecting to AP\r\n");
    NetworkInterface* network = easy_connect(true);
    
    // ================================================
    // Display network parameters
    // ================================================
    const char *ip = network->get_ip_address();
    const char *mac = network->get_mac_address();
    const char *netmask = network->get_netmask();
    const char *gateway = network->get_gateway();
    
    pc.printf("\r\nIP Address is: %s\r\n", (ip) ? ip : "None");
    pc.printf("\r\nMAC Address is: %s\r\n", (mac) ? mac : "None");  
    pc.printf("\r\nNetmask is: %s\r\n", (netmask) ? netmask : "None");
    pc.printf("\r\nGateway is: %s\r\n", (gateway) ? gateway : "None");
    
    // ================================================
    // Infinite loop
    // ================================================
    while(true) {
        // Check for the interrupt on the button
        if(wifiAllowed) {
            // Check the sensor data
            if (z.read()/y.read() > 0.86) {
                if (!openClosed) {
                    // Call the function which will send the request
                    send_request(network, false);
                    pc.printf("\r\nWindow is opened\n\r");
                    pc.printf("\r\nAccelerometer X : %f\n\r", x.read());
                    pc.printf("\r\nAccelerometer Y : %f\n\r", y.read());
                    pc.printf("\r\nAccelerometer Z : %f\n\r", z.read());
                    pc.printf("\r\nRatio (Z/Y) : %f\n\r", z.read()/y.read());
                    openClosed = true;
                }
            } else {
                if (openClosed) {
                    // Call the function which will send the request
                    send_request(network, true);
                    pc.printf("\r\nWindow is closed\n\r");
                    pc.printf("\r\nAccelerometer X : %f\n\r", x.read());
                    pc.printf("\r\nAccelerometer Y : %f\n\r", y.read());
                    pc.printf("\r\nAccelerometer Z : %f\n\r", z.read());
                    pc.printf("\r\nRatio (Z/Y) : %f\n\r", z.read()/y.read());
                    openClosed = false;
                }
            }
        }
        // Blink with different frequency when wifiAllowed
        led1 = !led1;
        if (wifiAllowed) {
            wait(0.2);
        } else {
            wait(1.0);
        }
    }
    
    // ================================================
    // The end
    // ================================================
    network->disconnect();
    pc.printf("\r\nReached the end of the program");
    wait(osWaitForever);
}

