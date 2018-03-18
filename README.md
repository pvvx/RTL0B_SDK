# RTL0B_SDK
MySDK RTL8710BN

WiFi modules: MJIOT-AMB-03, EMW3080

При использовании программирования через COM порт в Win10 WSL выполните:
>$ sudo visudo

и добавьте строку:

>user_name ALL=(ALL) NOPASSWD: /bin/chmod 666 /dev/ttyS[0-9]*
 
Based source [Realtek IoT/Arduino Solution](https://www.amebaiot.com/en/ameba-sdk-download)
