REM $Id: avr32_iar_build_stb_example_secure_sensor.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM AVR32 IAR Build path
set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"

REM **********************************************
REM Build STB Example App 1
REM **********************************************

REM     AT86RF212_AT32UC3A3256_RZ600 Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF212_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Data_Sink.ewp -clean Release
    %avr32_iarbuild% Data_Sink.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF212_AT32UC3A3256_RZ600 Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF212_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Sensor.ewp -clean Release
    %avr32_iarbuild% Sensor.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\


REM     AT86RF231_AT32UC3A3256_RZ600 Data_Sink

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Data_Sink\AT86RF231_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Data_Sink.ewp -clean Release
    %avr32_iarbuild% Data_Sink.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF231_AT32UC3A3256_RZ600 Sensor

    cd ..\..\..\..\Applications\STB_Examples\Secure_Sensor\Sensor\AT86RF231_AT32UC3A3256_RZ600\IAR

    %avr32_iarbuild% Sensor.ewp -clean Release
    %avr32_iarbuild% Sensor.ewp -build Release

    cd ..\..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
