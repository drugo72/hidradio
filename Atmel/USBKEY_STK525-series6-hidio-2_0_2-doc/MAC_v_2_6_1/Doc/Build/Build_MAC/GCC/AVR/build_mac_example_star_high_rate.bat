REM $Id: build_mac_example_star_high_rate.bat 24927 2011-01-17 10:49:18Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example Star High Rate
REM ***********************************************

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\
