REM $Id: arm_build_all_example_apps.bat 25610 2011-02-17 05:37:07Z ravikumar.vanteddu $

REM **********************************************
REM Build all ARM GCC Example Applications
REM **********************************************


rm -f arm_build_all_example_apps.log
rm -f arm_build_all_example_apps.err


REM ***********************************************
REM MAC Examples
REM ***********************************************

CALL arm_build_mac_example_1.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_2_nobeacon_indirect_traffic.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_3_beacon_payload.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_4_beacon_bc_data.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_6_beacon_indirect_bc_data >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_basic_sensor_network.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_promiscuous_mode_demo.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_star_network.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_mac_example_star_high_rate.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err

REM ***********************************************
REM PAL Examples
REM ***********************************************

REM CALL arm_build_pal_example_led_control.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err

REM ***********************************************
REM STB Examples
REM ***********************************************

CALL arm_build_stb_example_secure_remote_control.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
CALL arm_build_stb_example_secure_sensor.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
REM CALL arm_build_stb_example_secure_star_network.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err

REM STB Examples for Software AES are not included in this build

REM ***********************************************
REM TAL Examples
REM ***********************************************
CALL arm_build_tal_example_performance_test.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
REM CALL arm_build_tal_example_performance_test_evk.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err

REM ***********************************************
REM Tiny-TAL Examples
REM ***********************************************
CALL arm_build_tiny_tal_example_wireless_uart.bat >> arm_build_all_example_apps.log 2>> arm_build_all_example_apps.err
