REM $Id: avr32_iar_build_mac_example_star_high_rate.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM AVR32 IAR Build path
set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"

REM **********************************************
REM Build MAC Example Application Star High Rate
REM **********************************************

REM    AT86RF212_AT32UC3A3256_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Star_High_Rate.ewp -clean Release
    %avr32_iarbuild% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3A3256_RZ600

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Star_High_Rate.ewp -clean Release
    %avr32_iarbuild% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Star_High_Rate.ewp -clean Release
    %avr32_iarbuild% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Star_High_Rate.ewp -clean Release
    %avr32_iarbuild% Star_High_Rate.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
