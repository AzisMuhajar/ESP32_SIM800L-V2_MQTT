# ESP32_SIM800L-V2_MQTT

project ini memantau kondisi lokasi menggunakan modul GPS NEO 6M dan memanfaatkan SIM800L V2 untuk mengirim data melalui jaringan internet menggunkana protokol MQTT.
broker MQTT yang digunakan pada percobaan ini yaitu Broker EMQX.

### Modul yang digunakan
1. ESP32 DEV KIT
2. SIM800L V2
3. GPS NEO 6M

### Software/Tools yang digunakan
1. Visual Studio Code
2. Broker dekstop EMQX

## PINOUT
ESP32      SIM800L-V2      GPS NEO6M      Power Supply External 5VDC      
Vin                        Vcc
GND        GND             GND            GND
RX2        TXD
TX2        RXD
26                        TX
27                        RX
           VCC                           5V
