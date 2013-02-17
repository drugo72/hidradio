REM $Id: build_mac_example_star_push_button.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ***********************************************
REM Build MAC Example Star Push Button
REM ***********************************************

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_Push_Button\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\


