REM $Id: avr32_build_mac_example_star_push_button.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example Star Push Button
REM ***********************************************

REM     AT86RF212_AT32UC3B1128_REB_5_0_STK600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF212_AT32UC3B1128_REB_5_0_STK600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF230B_AT32UC3B1128_REB_2_3_STK600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF230B_AT32UC3B1128_REB_2_3_STK600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\

REM     AT86RF231_AT32UC3B1128_REB_4_0_STK600

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF231_AT32UC3B1128_REB_4_0_STK600\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR32\


