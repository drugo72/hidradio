REM $Id: iar_build_mac_example_3_beacon_payload.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Application App_3
REM **********************************************

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Coordinator_Beacon_Payload.ewp -clean Release
    iarbuild Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Device_Beacon_Payload.ewp -clean Release
    iarbuild Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\

    iarbuild Coordinator_Beacon_Payload.ewp -clean Release
    iarbuild Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\

    iarbuild Device_Beacon_Payload.ewp -clean Release
    iarbuild Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\



REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\

    iarbuild Coordinator_Beacon_Payload.ewp -clean Release
    iarbuild Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\

    iarbuild Device_Beacon_Payload.ewp -clean Release
    iarbuild Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\



REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Coordinator

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\

    iarbuild Coordinator_Beacon_Payload.ewp -clean Release
    iarbuild Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\
REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB Device

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\

    iarbuild Device_Beacon_Payload.ewp -clean Release
    iarbuild Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR\

