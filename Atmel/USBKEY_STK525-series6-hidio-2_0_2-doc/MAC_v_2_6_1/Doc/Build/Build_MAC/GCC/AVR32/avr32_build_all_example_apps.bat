REM $Id: avr32_build_all_example_apps.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM **********************************************
REM AVR32 Build all Example Applications
REM **********************************************


rm -f avr32_build_all_example_apps.log
rm -f avr32_build_all_example_apps.err


REM ***********************************************
REM MAC Examples
REM ***********************************************

CALL avr32_build_mac_example_1.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_2_nobeacon_indirect_traffic.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_3_beacon_payload.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_4_beacon_bc_data.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_6_beacon_indirect_bc_data.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_basic_sensor_network.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_promiscuous_mode_demo.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_star_network.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_star_push_button.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_mac_example_star_high_rate.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err

REM ***********************************************
REM STB Examples
REM ***********************************************

CALL avr32_build_stb_example_secure_remote_control.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_stb_example_secure_sensor.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
REM CALL avr32_build_stb_example_secure_star_network.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err

REM ***********************************************
REM TAL Examples
REM ***********************************************

CALL avr32_build_tal_example_performance_test.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err

REM ***********************************************
REM Tiny-TAL Examples
REM ***********************************************
CALL avr32_build_tiny_tal_example_wireless_uart.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
CALL avr32_build_tiny_tal_example_wireless_touch.bat >> avr32_build_all_example_apps.log 2>> avr32_build_all_example_apps.err
