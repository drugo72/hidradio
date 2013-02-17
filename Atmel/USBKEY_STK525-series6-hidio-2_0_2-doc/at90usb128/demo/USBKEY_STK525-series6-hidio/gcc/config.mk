
# Project name
PROJECT = USBKEY_STK525-series6-hidio

# CPU architecture : {avr0|...|avr6}
# Parts : {at90usb646|at90usb647|at90usb1286|at90usb1287|at90usb162|at90usb82}
MCU = at90usb1287

# Source files
CSRCS = \
  ../../../../common/lib_mcu/wdt/wdt_drv.c\
  ../usb_descriptors.c\
  ../../../modules/usb/device_chap9/usb_device_task.c\
  ../../../lib_mcu/usb/usb_drv.c\
  ../../../lib_mcu/util/start_boot.c\
  ../main.c\
  ../usb_specific_request.c\
  ../../../modules/usb/device_chap9/usb_standard_request.c\
  ../../../modules/usb/usb_task.c\
  ../../../lib_board/stk_525/stk_525.c\
  ../hid_task.c\
  ../radio_phy_task.c\
  ../../../../common/modules/scheduler/scheduler.c\

# Assembler source files
ASSRCS = \

