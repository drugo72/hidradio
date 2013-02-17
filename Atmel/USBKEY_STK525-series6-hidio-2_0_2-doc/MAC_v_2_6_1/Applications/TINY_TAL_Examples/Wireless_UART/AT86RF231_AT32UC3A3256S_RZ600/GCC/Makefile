############################################################################################
# Makefile for the project Wireless_UART
############################################################################################
# $Id: Makefile 25661 2011-02-21 13:42:59Z yogesh.bellan $

# Build specific properties
_TAL_TYPE = AT86RF231
_PAL_TYPE = AT32UC3A3256
_PAL_GENERIC_TYPE = AVR32
_BOARD_TYPE = RZ600_231
_HIGHEST_STACK_LAYER = TINY_TAL

# Path variables
## Path to main project directory
PATH_ROOT = ../../../../..
PATH_APP = ../..
PATH_TAL = $(PATH_ROOT)/TAL
PATH_TINY_TAL = $(PATH_ROOT)/TINY_TAL
PATH_PAL = $(PATH_ROOT)/PAL
PATH_GLOB_INC = $(PATH_ROOT)/Include

## General Flags
PROJECT = Wireless_UART
PART = uc3a3256s
TARGET_DIR = .
TARGET = $(TARGET_DIR)/$(PROJECT).elf
CC = avr32-gcc

## Options common to compile, link and assembly rules
COMMON = -mpart=$(PART)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -Werror -Wundef -g0 -std=gnu99 -DSIO_HUB=1 -DUSB0 -Os -ffunction-sections
CFLAGS += -DDEBUG=0
CFLAGS += -DTAL_TYPE=$(_TAL_TYPE)
CFLAGS += -DPAL_GENERIC_TYPE=$(_PAL_GENERIC_TYPE)
CFLAGS += -DPAL_TYPE=$(_PAL_TYPE)
CFLAGS += -DBOARD_TYPE=$(_BOARD_TYPE)
CFLAGS += -DHIGHEST_STACK_LAYER=$(_HIGHEST_STACK_LAYER)
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d
CFLAGS += -DPAL_USE_SPI_TRX=1 -DIEEE_ADDRESS=0xBABE -DEXTERN_EEPROM_AVAILABLE=0 -DDEF_TOUCH_QMATRIX=0

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-g

## Linker flags
LDFLAGS = $(COMMON) -Wl,--gc-sections

## Include directories for application
INCLUDES = -I $(PATH_APP)/Inc
## Include directories for general includes
INCLUDES += -I $(PATH_GLOB_INC)
## Include directories for TAL
INCLUDES += -I $(PATH_TAL)/$(_TAL_TYPE)/Inc/
## Include directories for TINY_TAL
INCLUDES += -I $(PATH_TINY_TAL)/Inc/
INCLUDES += -I $(PATH_TINY_TAL)/$(_TAL_TYPE)/Inc/
## Include directories for PAL
INCLUDES += -I $(PATH_PAL)/Inc/
INCLUDES += -I $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Inc
INCLUDES += -I $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries/USB/cdc-serial/Inc
INCLUDES += -I $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Inc/
## Include directories for specific boards type
INCLUDES += -I $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/
INCLUDES += -I $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)

## Objects that must be built in order to link
OBJECTS = $(TARGET_DIR)/main.o\
	$(TARGET_DIR)/pal_sio_hub.o\
	$(TARGET_DIR)/pal_irq.o\
	$(TARGET_DIR)/pal.o\
	$(TARGET_DIR)/pal_timer.o\
	$(TARGET_DIR)/pal_board.o\
	$(TARGET_DIR)/pal_utils.o\
	$(TARGET_DIR)/pal_trx_access.o\
	$(TARGET_DIR)/tiny_tal.o\
	$(TARGET_DIR)/tiny_tal_rx.o\
	$(TARGET_DIR)/tiny_tal_tx.o\
	$(TARGET_DIR)/tiny_tal_pib.o\
	$(TARGET_DIR)/tiny_tal_init.o\
	$(TARGET_DIR)/tiny_tal_irq_handler.o\
	$(TARGET_DIR)/tiny_tal_pwr_mgmt.o\
	$(TARGET_DIR)/tiny_tal_rx_enable.o\
	$(TARGET_DIR)/pal_usb.o\
	$(TARGET_DIR)/uart_usb_lib.o\
	$(TARGET_DIR)/usb_descriptors.o\
	$(TARGET_DIR)/usb_device_task.o\
	$(TARGET_DIR)/usb_drv.o\
	$(TARGET_DIR)/usb_specific_request.o\
	$(TARGET_DIR)/usb_standard_request.o\
	$(TARGET_DIR)/usb_task.o\
	$(TARGET_DIR)/exception.o

## Objects explicitly added by the user
LINKONLYOBJECTS =

## Build
all: $(TARGET) $(TARGET_DIR)/$(PROJECT).hex $(TARGET_DIR)/$(PROJECT).eep $(TARGET_DIR)/$(PROJECT).lss size

## Compile
$(TARGET_DIR)/main.o: $(PATH_APP)/Src/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  -o $@ $<
$(TARGET_DIR)/pal_sio_hub.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src/pal_sio_hub.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_irq.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)/pal_irq.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src/pal.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_timer.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src/pal_timer.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_board.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)/pal_board.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_utils.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src/pal_utils.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/pal_trx_access.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src/pal_trx_access.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/tiny_tal.o: $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src/tiny_tal.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/tiny_tal_rx.o: $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src/tiny_tal_rx.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/tiny_tal_tx.o: $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src/tiny_tal_tx.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/tiny_tal_pib.o: $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src/tiny_tal_pib.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/tiny_tal_init.o: $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src/tiny_tal_init.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/tiny_tal_irq_handler.o: $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src/tiny_tal_irq_handler.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/tiny_tal_pwr_mgmt.o: $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src/tiny_tal_pwr_mgmt.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/tiny_tal_rx_enable.o: $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src/tiny_tal_rx_enable.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

$(TARGET_DIR)/pal_usb.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src/pal_usb.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/uart_usb_lib.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries/USB/cdc-serial/Src/uart_usb_lib.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/usb_descriptors.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries/USB/cdc-serial/Src/usb_descriptors.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/usb_device_task.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries/USB/cdc-serial/Src/usb_device_task.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/usb_drv.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries/USB/cdc-serial/Src/usb_drv.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/usb_specific_request.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries/USB/cdc-serial/Src/usb_specific_request.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/usb_standard_request.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries/USB/cdc-serial/Src/usb_standard_request.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
$(TARGET_DIR)/usb_task.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries/USB/cdc-serial/Src/usb_task.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<

$(TARGET_DIR)/exception.o: $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src/exception.S
	avr32-gcc $(ASMFLAGS) $(INCLUDES) -c -o $@ $<

##Link
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) -o $(TARGET)

%.hex: $(TARGET)
	avr32-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	avr32-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr32-objdump -h -S $< > $@

##Size
size: ${TARGET}
	@echo
	@avr32-size -Bd $<

## Clean target
.PHONY: clean
clean:
	-rm -rf $(TARGET_DIR)/*.o $(TARGET_DIR)/$(PROJECT).elf dep/* $(TARGET_DIR)/$(PROJECT).hex $(TARGET_DIR)/$(PROJECT).eep $(TARGET_DIR)/$(PROJECT).lss $(TARGET_DIR)/$(PROJECT).map

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)
