REM $Id: arm_iar_build_tal_example_performance_test.bat 25424 2011-02-08 06:36:03Z ravikumar.vanteddu $

REM ************************
REM ARM IAR Build batch file
REM ************************

REM ARM IAR Build path

set arm_iar_build="C:\Program Files\IAR Systems\Embedded Workbench 5.4_0\common\bin\Iarbuild"

REM **********************************************
REM Build TAL Example Application Performance_Test
REM **********************************************

REM AT86RF230B_AT91SAM7X256_REB_2_3_REX_ARM_REV_2
	cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF230B_AT91SAM7X256_REB_2_3_REX_ARM_REV_2

    %arm_iar_build% Performance.ewp -clean Performance
    %arm_iar_build% Performance.ewp -build Performance

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Performance.ewp -clean Performance
    %arm_iar_build% Performance.ewp -build Performance

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF230B_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Performance.ewp -clean Performance
    %arm_iar_build% Performance.ewp -build Performance

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Performance.ewp -clean Performance
    %arm_iar_build% Performance.ewp -build Performance

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04
    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    %arm_iar_build% Performance.ewp -clean Performance
    %arm_iar_build% Performance.ewp -build Performance

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04
    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    %arm_iar_build% Performance.ewp -clean Performance
    %arm_iar_build% Performance.ewp -build Performance

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
