REM $Id: arm_build_mac_example_6_beacon_indirect_bc_data.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM ***********************************************
REM ARM Build MAC Example App 6
REM ***********************************************
REM AT86RF212_AT91SAM3S4C_SAM3SEK Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\
REM AT86RF212_AT91SAM3S4C_SAM3SEK Device
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF212_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF231_AT91SAM3S4C_SAM3SEK Coordinator
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\
REM AT86RF231_AT91SAM3S4C_SAM3SEK Device
    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF231_AT91SAM3S4C_SAM3SEK\GCC

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04 Coordinator
   cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF212_AT91SAM3S4B_DERF_USB_13E04\GCC

   make clean -f Makefile
   make all -f Makefile

   cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\
REM AT86RF212_AT91SAM3S4B_DERF_USB_13E04 Device
   cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF212_AT91SAM3S4B_DERF_USB_13E04\GCC

  make clean -f Makefile
  make all -f Makefile

  cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\


REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04 Coordinator
   cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF231_AT91SAM3S4B_DERF_USB_23E04\GCC

   make clean -f Makefile
   make all -f Makefile

   cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\
REM AT86RF231_AT91SAM3S4B_DERF_USB_23E04 Device
   cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF231_AT91SAM3S4B_DERF_USB_23E04\GCC

  make clean -f Makefile
  make all -f Makefile

  cd ..\..\..\..\..\..\Build\Build_MAC\GCC\ARM\
 

