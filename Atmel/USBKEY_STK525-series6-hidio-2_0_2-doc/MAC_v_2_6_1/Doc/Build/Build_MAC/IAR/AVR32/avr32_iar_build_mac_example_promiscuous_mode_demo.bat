REM $Id: avr32_iar_build_mac_example_promiscuous_mode_demo.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM AVR323232 IAR Build path
set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"

REM **********************************************
REM Build MAC Example Application Promiscuous Mode Demo
REM **********************************************

    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF212_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF230B_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


    cd ..\..\..\..\Applications\MAC_Examples\Promiscuous_Mode_Demo\AT86RF231_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -clean Release
    %avr32_iarbuild% Promiscuous_Mode_Demo.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
