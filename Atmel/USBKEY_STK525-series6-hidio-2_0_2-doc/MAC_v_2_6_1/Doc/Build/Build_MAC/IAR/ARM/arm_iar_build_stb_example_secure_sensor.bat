REM $Id: arm_iar_build_stb_example_secure_sensor.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ************************
REM ARM IAR Build batch file
REM ************************

REM ARM IAR Build path

set arm_iar_build="C:\Program Files\IAR Systems\Embedded Workbench 5.4_0\common\bin\Iarbuild"

REM **********************************************
REM Build STB Example Application Secure_Sensor
REM **********************************************

REM AT86RF230B_AT91SAM7XC256_REB_2_3_REX_ARM_REV_2
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF230B_AT91SAM7XC256_REB_2_3_REX_ARM_REV_2

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF230B_AT91SAM7XC256_REB_2_3_REX_ARM_REV_2

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    REM cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF230B_AT91SAM3S4C_SAM3SEK

    REM %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    REM %arm_iar_build% Data_Sink.ewp -build Data_Sink

    REM cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    REM cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF230B_AT91SAM3S4C_SAM3SEK

    REM %arm_iar_build% Sensor.ewp -clean Sensor
    REM %arm_iar_build% Sensor.ewp -build Sensor

    REM cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\



REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04
    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    %arm_iar_build% Data_Sink.ewp -clean Data_Sink
    %arm_iar_build% Data_Sink.ewp -build Data_Sink

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    %arm_iar_build% Sensor.ewp -clean Sensor
    %arm_iar_build% Sensor.ewp -build Sensor


    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\
