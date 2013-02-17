REM $Id: arm_build_stb_example_secure_sensor.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ***********************************************
REM ARM Build STB Example App 1
REM ***********************************************

REM     AT86RF212_AT91SAM3S4C_SAM3SEK Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF212_AT91SAM3S4C_SAM3SEK Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM     AT86RF231_AT91SAM3S4C_SAM3SEK Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF231_AT91SAM3S4C_SAM3SEK Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM     AT86RF231_AT91SAM3S4B_DERF_USB_23E04 Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT91SAM3S4B_DERF_USB_23E04\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\

REM     AT86RF231_AT91SAM3S4B_DERF_USB_23E04 Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT91SAM3S4B_DERF_USB_23E04\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\



