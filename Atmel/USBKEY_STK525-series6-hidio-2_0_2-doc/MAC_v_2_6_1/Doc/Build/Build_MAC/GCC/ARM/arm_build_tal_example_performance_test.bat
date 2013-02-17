REM $Id: arm_build_tal_example_performance_test.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM Build TAL Example Performance test
REM ***********************************************

REM     AT86RF212_AT91SAM3S4C_SAM3SEK

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF230B_AT91SAM3S4C_SAM3SEK

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF230B_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF231_AT91SAM3S4C_SAM3SEK

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF212_AT91SAM3S4B_DERF_USB_13E04

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF212_AT91SAM3S4B_DERF_USB_13E04\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF231_AT91SAM3S4B_DERF_USB_23E04

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF231_AT91SAM3S4B_DERF_USB_23E04\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\
