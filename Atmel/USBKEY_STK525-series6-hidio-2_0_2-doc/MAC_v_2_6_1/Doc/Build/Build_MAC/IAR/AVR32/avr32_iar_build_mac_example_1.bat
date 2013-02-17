REM $Id: avr32_iar_build_mac_example_1.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM AVR3232 IAR Build path
set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"


REM **********************************************
REM Build MAC Example Application App_1
REM **********************************************

    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Coordinator\AT86RF212_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Coordinator.ewp -clean Release
    %avr32_iarbuild% Coordinator.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Device\AT86RF212_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Device.ewp -clean Release
    %avr32_iarbuild% Device.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\



    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Coordinator\AT86RF230B_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Coordinator.ewp -clean Release
    %avr32_iarbuild% Coordinator.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Device\AT86RF230B_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Device.ewp -clean Release
    %avr32_iarbuild% Device.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Coordinator\AT86RF231_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Coordinator.ewp -clean Release
    %avr32_iarbuild% Coordinator.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Device\AT86RF231_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Device.ewp -clean Release
    %avr32_iarbuild% Device.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Coordinator\AT86RF231_AT32UC3B1128_REB_4_0_STK600\IAR

    %avr32_iarbuild% Coordinator.ewp -clean Release
    %avr32_iarbuild% Coordinator.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Device\AT86RF231_AT32UC3B1128_REB_4_0_STK600\IAR

    %avr32_iarbuild% Device.ewp -clean Release
    %avr32_iarbuild% Device.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Coordinator\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Coordinator.ewp -clean Release
    %avr32_iarbuild% Coordinator.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Device\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Device.ewp -clean Release
    %avr32_iarbuild% Device.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Coordinator\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Coordinator.ewp -clean Release
    %avr32_iarbuild% Coordinator.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Device\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Device.ewp -clean Release
    %avr32_iarbuild% Device.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\




    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Coordinator\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Coordinator.ewp -clean Release
    %avr32_iarbuild% Coordinator.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

    cd ..\..\..\..\Applications\MAC_Examples\App_1_Nobeacon\Device\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Device.ewp -clean Release
    %avr32_iarbuild% Device.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


