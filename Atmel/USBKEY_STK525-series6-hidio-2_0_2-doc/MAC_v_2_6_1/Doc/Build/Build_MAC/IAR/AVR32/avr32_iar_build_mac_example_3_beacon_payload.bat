REM $Id: avr32_iar_build_mac_example_3_beacon_payload.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM AVR3232 IAR Build path
set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"


REM **********************************************
REM Build MAC Example Application App_3
REM **********************************************

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF212_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF212_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Device_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\



    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF230B_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF230B_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Device_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF231_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF231_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Device_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Device_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Device_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Coordinator\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Coordinator_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_3_Beacon_Payload\Device\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Device_Beacon_Payload.ewp -clean Release
    %avr32_iarbuild% Device_Beacon_Payload.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

