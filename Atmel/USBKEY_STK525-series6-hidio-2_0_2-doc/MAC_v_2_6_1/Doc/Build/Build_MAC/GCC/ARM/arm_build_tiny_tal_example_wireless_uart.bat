REM $Id: arm_build_tiny_tal_example_wireless_uart.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM Build Tiny-TAL Example Wireless UART
REM ***********************************************


REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF230B_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF212_AT91SAM3S4B_DERF_USB_13E04\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04
    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF231_AT91SAM3S4B_DERF_USB_23E04\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\
