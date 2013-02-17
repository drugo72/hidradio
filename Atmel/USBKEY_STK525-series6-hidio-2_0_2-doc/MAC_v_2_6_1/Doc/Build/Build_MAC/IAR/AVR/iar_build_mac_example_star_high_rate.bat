REM $Id: iar_build_mac_example_star_high_rate.bat 24925 2011-01-17 10:43:37Z ravikumar.vanteddu $

REM **********************************************
REM Build MAC Example Application Star High Rate
REM **********************************************

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\ATMEGA128RFA1_RCB_6_3_SENS_TERM_BOARD

    iarbuild Star_High_Rate.ewp -clean Release
    iarbuild Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF212_ATXMEGA256A3_REB_5_0_CBB

    iarbuild Star_High_Rate.ewp -clean Release
    iarbuild Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\


REM     AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    cd ..\..\..\..\Applications\MAC_Examples\Star_High_Rate\AT86RF231_ATXMEGA256A3_REB_4_1_CBB

    iarbuild Star_High_Rate.ewp -clean Release
    iarbuild Star_High_Rate.ewp -build Release

    cd ..\..\..\..\Build\Build_MAC\IAR\AVR\
