REM $Id: arm_iar_build_mac_example_star_network.bat 25424 2011-02-08 06:36:03Z ravikumar.vanteddu $

REM ************************
REM ARM IAR Build batch file
REM ************************

REM ARM IAR Build path

set arm_iar_build="C:\Program Files\IAR Systems\Embedded Workbench 5.4_0\common\bin\Iarbuild"

REM **********************************************
REM Build MAC Example Application Star_Nobeacon
REM **********************************************

    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_AT91SAM7X256_REB_2_3_REX_ARM_REV_2

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\



    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT91SAM7X256_REB_4_0_2_REX_ARM_REV_3

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\



    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT91SAM7X256_REB_5_0_REX_ARM_REV_3

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    %arm_iar_build% Star.ewp -clean Star
    %arm_iar_build% Star.ewp -build Star

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

