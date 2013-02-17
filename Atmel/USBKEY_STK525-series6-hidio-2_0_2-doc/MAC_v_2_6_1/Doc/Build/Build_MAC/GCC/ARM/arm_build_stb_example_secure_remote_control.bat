REM $Id: arm_build_stb_example_secure_remote_control.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ***********************************************
REM ARM Build STB Example App Secure Remote Control
REM ***********************************************

REM     AT86RF212_AT91SAM3S4C_SAM3SEK

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM     AT86RF231_AT91SAM3S4C_SAM3SEK

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\ARM\
