REM $Id: avr32_iar_build_stb_example_secure_remote_control.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM AVR32 IAR Build path
set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"

REM **********************************************
REM Build STB Example App 1
REM **********************************************

REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Secure_Remote_Control.ewp -clean Release
    %avr32_iarbuild% Secure_Remote_Control.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR

    %avr32_iarbuild% Secure_Remote_Control.ewp -clean Release
    %avr32_iarbuild% Secure_Remote_Control.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
