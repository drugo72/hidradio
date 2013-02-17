REM $Id: avr32_iar_build_tiny_tal_example_wireless_touch.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM AVR32 IAR Build path

set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"

REM ***********************************************
REM IAR Build Tiny-TAL Example Wireless Touch
REM ***********************************************


REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_Touch\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR\

    %avr32_iarbuild% Wireless_Touch.ewp -clean Release
    %avr32_iarbuild% Wireless_Touch.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
