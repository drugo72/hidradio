REM $Id: arm_iar_build_stb_example_secure_remote_control.bat 25997 2011-03-23 06:14:02Z ravikumar.vanteddu $

REM ************************
REM ARM IAR Build batch file
REM ************************

REM ARM IAR Build path

set arm_iar_build="C:\Program Files\IAR Systems\Embedded Workbench 5.4_0\common\bin\Iarbuild"

REM **********************************************
REM Build STB Example Application Secure_Remote_Control
REM **********************************************

REM AT86RF230B_AT91SAM7XC256_REB_2_3_REX_ARM_REV_2
    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF230B_AT91SAM7XC256_REB_2_3_REX_ARM_REV_2

    %arm_iar_build% Secure_Remote_Control.ewp -clean Secure_Remote_Control
    %arm_iar_build% Secure_Remote_Control.ewp -build Secure_Remote_Control

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF212_AT91SAM3S4C_SAM3SEK
    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF212_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Secure_Remote_Control.ewp -clean Secure_Remote_Control
    %arm_iar_build% Secure_Remote_Control.ewp -build Secure_Remote_Control

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
	
REM AT86RF230B_AT91SAM3S4C_SAM3SEK	
    REM cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF230B_AT91SAM3S4C_SAM3SEK

    REM %arm_iar_build% Secure_Remote_Control.ewp -clean Secure_Remote_Control
    REM %arm_iar_build% Secure_Remote_Control.ewp -build Secure_Remote_Control

    REM cd ..\..\..\..\Build\Build_MAC\IAR\ARM\

REM AT86RF231_AT91SAM3S4C_SAM3SEK	
    cd ..\..\..\..\Applications\STB_Examples\Secure_Remote_Control\AT86RF231_AT91SAM3S4C_SAM3SEK

    %arm_iar_build% Secure_Remote_Control.ewp -clean Secure_Remote_Control
    %arm_iar_build% Secure_Remote_Control.ewp -build Secure_Remote_Control

    cd ..\..\..\..\Build\Build_MAC\IAR\ARM\
