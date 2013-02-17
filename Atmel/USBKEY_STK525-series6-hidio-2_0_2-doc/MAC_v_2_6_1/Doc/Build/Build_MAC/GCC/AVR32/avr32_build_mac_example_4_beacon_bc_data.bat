REM $Id: avr32_build_mac_example_4_beacon_bc_data.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example App 4
REM ***********************************************

REM     AT86RF212_AT32UC3A3256_RZ600 Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Coordinator\AT86RF212_AT32UC3A3256_RZ600\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\
REM     AT86RF212_AT32UC3A3256_RZ600 Device

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Device\AT86RF212_AT32UC3A3256_RZ600\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\


REM       AT86RF231_AT32UC3A3256_RZ600 Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Coordinator\AT86RF231_AT32UC3A3256_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\
REM       AT86RF231_AT32UC3A3256_RZ600 Device

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Device\AT86RF231_AT32UC3A3256_RZ600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\


REM       AT86RF212_AT32UC3L064_RZ600_UC3LEK Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Coordinator\AT86RF212_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\
REM       AT86RF212_AT32UC3L064_RZ600_UC3LEK Device

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Device\AT86RF212_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\


REM       AT86RF231_AT32UC3L064_RZ600_UC3LEK Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Coordinator\AT86RF231_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\
REM       AT86RF231_AT32UC3L064_RZ600_UC3LEK Device

    cd ..\..\..\..\Applications\MAC_Examples\App_4_Beacon_Broadcast_Data\Device\AT86RF231_AT32UC3L064_RZ600_UC3LEK\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\AVR32\
