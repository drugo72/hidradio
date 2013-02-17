REM $Id: arm_iar_build_tiny_tal_example_wireless_uart.bat 25424 2011-02-08 06:36:03Z ravikumar.vanteddu $

REM ************************
REM ARM IAR Build batch file
REM ************************

REM ARM IAR Build path

set arm_iar_build="C:\Program Files\IAR Systems\Embedded Workbench 5.4_0\common\bin\Iarbuild"

REM **********************************************
REM Build Tiny-TAL Example Application Wireless_UART
REM **********************************************

REM AT86RF230B_AT91SAM7X256_REB_2_3_REX_ARM_REV_2
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF230B_AT91SAM7X256_REB_2_3_REX_ARM_REV_2

    %arm_iar_build% Wireless_UART.ewp -clean Wireless_UART
    %arm_iar_build% Wireless_UART.ewp -build Wireless_UART

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Wireless_UART.ewp -clean Wireless_UART
    %arm_iar_build% Wireless_UART.ewp -build Wireless_UART

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF230B_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Wireless_UART.ewp -clean Wireless_UART
    %arm_iar_build% Wireless_UART.ewp -build Wireless_UART

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Wireless_UART.ewp -clean Wireless_UART
    %arm_iar_build% Wireless_UART.ewp -build Wireless_UART

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    %arm_iar_build% Wireless_UART.ewp -clean Wireless_UART
    %arm_iar_build% Wireless_UART.ewp -build Wireless_UART

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    %arm_iar_build% Wireless_UART.ewp -clean Wireless_UART
    %arm_iar_build% Wireless_UART.ewp -build Wireless_UART

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
