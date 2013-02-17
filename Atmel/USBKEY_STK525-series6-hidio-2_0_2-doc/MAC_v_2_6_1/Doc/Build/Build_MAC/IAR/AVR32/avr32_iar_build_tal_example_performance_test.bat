REM $Id: avr32_iar_build_tal_example_performance_test.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM AVR3232 IAR Build path

set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"

REM **********************************************
REM Build TAL Example Application Performance_Test
REM **********************************************

REM     AT86RF212_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF212_AT32UC3A3256S_RZ600\IAR\

    %avr32_iarbuild% Performance_test.ewp -clean Release
    %avr32_iarbuild% Performance_test.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF230B_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF230B_AT32UC3A3256S_RZ600\IAR\

    %avr32_iarbuild% Performance_test.ewp -clean Release
    %avr32_iarbuild% Performance_test.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF231_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF231_AT32UC3A3256S_RZ600\IAR\

    %avr32_iarbuild% Performance_test.ewp -clean Release
    %avr32_iarbuild% Performance_test.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF212_AT32UC3B1128_REB_5_0_STK600

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF212_AT32UC3B1128_REB_5_0_STK600\IAR\

    %avr32_iarbuild% Performance.ewp -clean Release
    %avr32_iarbuild% Performance.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF230B_AT32UC3B1128_REB_2_3_STK600

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF230B_AT32UC3B1128_REB_2_3_STK600\IAR\

    %avr32_iarbuild% Performance.ewp -clean Release
    %avr32_iarbuild% Performance.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF231_AT32UC3B1128_REB_4_0_STK600

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF231_AT32UC3B1128_REB_4_0_STK600\IAR\

    %avr32_iarbuild% Performance.ewp -clean Release
    %avr32_iarbuild% Performance.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR\

    %avr32_iarbuild% Performance.ewp -clean Release
    %avr32_iarbuild% Performance.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF230B_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR\

    %avr32_iarbuild% Performance.ewp -clean Release
    %avr32_iarbuild% Performance.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR\

    %avr32_iarbuild% Performance.ewp -clean Release
    %avr32_iarbuild% Performance.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

