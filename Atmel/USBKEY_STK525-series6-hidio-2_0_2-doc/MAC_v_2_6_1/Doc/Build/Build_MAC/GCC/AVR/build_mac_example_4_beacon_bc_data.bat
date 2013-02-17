REM $Id: build_mac_example_4_beacon_bc_data.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example App 4
REM ***********************************************

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Coordinator\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD Device

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Device\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

