REM $Id: avr32_build_stb_example_secure_sensor.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM Build STB Example App 1
REM ***********************************************

REM     AT86RF212_AT32UC3A3256_RZ600 Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT32UC3A3256_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF212_AT32UC3A3256_RZ600 Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT32UC3A3256_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\


REM     AT86RF231_AT32UC3A3256_RZ600 Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT32UC3A3256_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF231_AT32UC3A3256_RZ600 Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT32UC3A3256_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

