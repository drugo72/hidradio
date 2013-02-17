REM $Id: arm_iar_build_mac_example_6_beacon_indirect_bc_data.bat 25424 2011-02-08 06:36:03Z ravikumar.vanteddu $

REM ************************
REM ARM IAR Build batch file
REM ************************

REM ARM IAR Build path

set arm_iar_build="C:\Program Files\IAR Systems\Embedded Workbench 5.4_0\common\bin\Iarbuild"

REM **********************************************
REM Build MAC Example Application App_6
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Coordinator_Indirect_Traffic_BC_Data.ewp -clean Coordinator
    %arm_iar_build% Coordinator_Indirect_Traffic_BC_Data.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK Device
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Device_Indirect_Traffic_BC_Data.ewp -clean Device
    %arm_iar_build% Device_Indirect_Traffic_BC_Data.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF231_AT91SAM3S4C_SAM3SEK Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Coordinator_Indirect_Traffic_BC_Data.ewp -clean Coordinator
    %arm_iar_build% Coordinator_Indirect_Traffic_BC_Data.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK Device
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Device_Indirect_Traffic_BC_Data.ewp -clean Device
    %arm_iar_build% Device_Indirect_Traffic_BC_Data.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04 Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    %arm_iar_build% Coordinator_Indirect_Traffic_BC_Data.ewp -clean Coordinator
    %arm_iar_build% Coordinator_Indirect_Traffic_BC_Data.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04 Device
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    %arm_iar_build% Device_Indirect_Traffic_BC_Data.ewp -clean Device
    %arm_iar_build% Device_Indirect_Traffic_BC_Data.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\


REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04 Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    %arm_iar_build% Coordinator_Indirect_Traffic_BC_Data.ewp -clean Coordinator
    %arm_iar_build% Coordinator_Indirect_Traffic_BC_Data.ewp -build Coordinator

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04 Device
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    %arm_iar_build% Device_Indirect_Traffic_BC_Data.ewp -clean Device
    %arm_iar_build% Device_Indirect_Traffic_BC_Data.ewp -build Device

    cd ..\..\..\..\..\Build\Build_MAC\IAR\ARM\
