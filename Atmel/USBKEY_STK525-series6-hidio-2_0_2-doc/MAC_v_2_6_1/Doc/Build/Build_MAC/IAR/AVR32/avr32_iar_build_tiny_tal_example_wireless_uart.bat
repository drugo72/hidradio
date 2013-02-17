REM $Id: avr32_iar_build_tiny_tal_example_wireless_uart.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM AVR32 IAR Build path

set avr32_iarbuild="c:\Program Files\IAR Systems\Embedded Workbench 5.6\common\bin\iarbuild"

REM ***********************************************
REM IAR Build Tiny-TAL Example Wireless Uart
REM ***********************************************

REM     AT86RF212_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF212_AT32UC3A3256S_RZ600\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF230B_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF230B_AT32UC3A3256S_RZ600\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF231_AT32UC3A3256S_RZ600

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF231_AT32UC3A3256S_RZ600\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF212_AT32UC3B1128_REB_5_0_STK600

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF212_AT32UC3B1128_REB_5_0_STK600\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF230B_AT32UC3B1128_REB_2_3_STK600

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF230B_AT32UC3B1128_REB_2_3_STK600\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF231_AT32UC3B1128_REB_4_0_STK600

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF231_AT32UC3B1128_REB_4_0_STK600\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF212_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF212_AT32UC3L064_RZ600_UC3LEK\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF230B_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF230B_AT32UC3L064_RZ600_UC3LEK\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\

REM     AT86RF231_AT32UC3L064_RZ600_UC3LEK

    cd ..\..\..\..\Applications\TINY_TAL_Examples\Wireless_UART\AT86RF231_AT32UC3L064_RZ600_UC3LEK\IAR\

    %avr32_iarbuild% Wireless_UART.ewp -clean Release
    %avr32_iarbuild% Wireless_UART.ewp -build Release

    cd ..\..\..\..\..\Build\Build_MAC\IAR\AVR32\
