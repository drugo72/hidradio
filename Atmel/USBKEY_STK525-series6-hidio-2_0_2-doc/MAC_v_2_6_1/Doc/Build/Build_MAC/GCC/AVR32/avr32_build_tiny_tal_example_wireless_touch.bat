REM $Id: avr32_build_tiny_tal_example_wireless_touch.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM Build Tiny-TAL Example Wireless Touch
REM ***********************************************


REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_Touch\AT86RF231_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\
