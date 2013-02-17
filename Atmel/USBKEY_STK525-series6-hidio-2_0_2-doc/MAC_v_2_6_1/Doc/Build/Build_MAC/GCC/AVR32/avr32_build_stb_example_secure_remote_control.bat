REM $Id: avr32_build_stb_example_secure_remote_control.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM Build STB Example App Secure Remote Control
REM ***********************************************

REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF212_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF231_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\
