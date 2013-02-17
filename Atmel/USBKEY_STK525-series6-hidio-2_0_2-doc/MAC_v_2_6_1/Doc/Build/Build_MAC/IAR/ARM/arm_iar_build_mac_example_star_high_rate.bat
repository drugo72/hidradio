REM $Id: arm_iar_build_mac_example_star_high_rate.bat 25424 2011-02-08 06:36:03Z ravikumar.vanteddu $

REM ************************
REM ARM IAR Build batch file
REM ************************

REM ARM IAR Build path

set arm_iar_build="C:\Program Files\IAR Systems\Embedded Workbench 5.4_0\common\bin\Iarbuild"

REM **********************************************
REM Build MAC Example Application Star_High_Rate
REM **********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star_High_Rate.ewp -clean Release
    %arm_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star_High_Rate.ewp -clean Release
    %arm_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    %arm_iar_build% Star_High_Rate.ewp -clean Release
    %arm_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04
    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    %arm_iar_build% Star_High_Rate.ewp -clean Release
    %arm_iar_build% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
