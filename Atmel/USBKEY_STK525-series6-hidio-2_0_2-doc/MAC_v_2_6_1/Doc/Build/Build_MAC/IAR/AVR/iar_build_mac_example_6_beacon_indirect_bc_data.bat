REM $Id: iar_build_mac_example_6_beacon_indirect_bc_data.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Application App_6
REM **********************************************

    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Coordinator_Indirect_Traffic_BC_Data.ewp -clean Release
    iarbuild Coordinator_Indirect_Traffic_BC_Data.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Device_Indirect_Traffic_BC_Data.ewp -clean Release
    iarbuild Device_Indirect_Traffic_BC_Data.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\

    iarbuild Coordinator_Indirect_Traffic_BC_Data.ewp -clean Release
    iarbuild Coordinator_Indirect_Traffic_BC_Data.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\

    iarbuild Device_Indirect_Traffic_BC_Data.ewp -clean Release
    iarbuild Device_Indirect_Traffic_BC_Data.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\



REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\

    iarbuild Coordinator_Indirect_Traffic_BC_Data.ewp -clean Release
    iarbuild Coordinator_Indirect_Traffic_BC_Data.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_6_Beacon_Indirect_Traffic_and_Broadcast_Data\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\

    iarbuild Device_Indirect_Traffic_BC_Data.ewp -clean Release
    iarbuild Device_Indirect_Traffic_BC_Data.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

