REM $Id: avr32_build_mac_example_star_high_rate.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example Star High Rate
REM ***********************************************

REM     AT86RF212_AT32UC3A3256_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT32UC3A3256_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF231_AT32UC3A3256_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT32UC3A3256_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\
