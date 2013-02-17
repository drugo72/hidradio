REM $Id: arm_iar_build_mac_example_basic_sensor_network.bat 25424 2011-02-08 06:36:03Z ravikumar.vanteddu $

REM ************************
REM ARM IAR Build batch file
REM ************************

REM ARM IAR Build path

set arm_iar_build="C:\Program Files\IAR Systems\Embedded Workbench 5.4_0\common\bin\Iarbuild"

REM **********************************************
REM Build MAC Example Application Basic Sensor Network
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor_Network.ewp -clean Release
    %arm_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF230B_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor_Network.ewp -clean Release
    %arm_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Basic_Sensor_Network\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Sensor_Network.ewp -clean Release
    %arm_iar_build% Sensor_Network.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\


