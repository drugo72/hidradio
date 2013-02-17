REM $Id: build_tal_example_performance_test_evk.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ***********************************************
REM Build TAL Examples Performance test evk
REM ***********************************************

REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF212_ATXMEGA256A3_REB_5_0_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF230B_ATXMEGA256A3_REB_2_3_CBB

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF230B_ATXMEGA256A3_REB_2_3_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\TAL_Examples\Performance_Test_EVK\AT86RF231_ATXMEGA256A3_REB_4_1_CBB\GCC\

    make clean -f Makefile
    make all -f Makefile

    cd ..\..\..\..\..\Build\Build_MAC\GCC\AVR\

