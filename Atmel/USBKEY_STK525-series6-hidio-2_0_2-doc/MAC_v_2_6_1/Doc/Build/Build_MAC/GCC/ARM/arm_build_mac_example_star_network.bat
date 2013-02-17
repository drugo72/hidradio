REM $Id: arm_build_mac_example_star_network.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example Star NoBeacon
REM ***********************************************

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF230B_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF230B_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF231_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF212_AT91SAM3S4B_DERF_USB_13E04\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04
    cd ..\..\..\..\Applications\MAC_Examples\Star_Nobeacon\AT86RF231_AT91SAM3S4B_DERF_USB_23E04\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\
