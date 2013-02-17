REM $Id: build_mac_example_3_beacon_payload.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example App 3
REM ***********************************************

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD Device

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\
REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR\

