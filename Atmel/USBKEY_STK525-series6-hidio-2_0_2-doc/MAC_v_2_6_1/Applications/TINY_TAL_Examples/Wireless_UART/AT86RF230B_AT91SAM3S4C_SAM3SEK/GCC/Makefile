############################################################################################
# Makefile for the project Wireless_UART Release Using single source files
############################################################################################
# $Id: Makefile 24642 2010-12-31 09:46:43Z mahendran.p $

# Build specific properties
_TAL_TYPE = AT86RF230B
_PAL_TYPE = AT91SAM3S4C
_PAL_GENERIC_TYPE = SAM3
_BOARD_TYPE = RZ600_230_SAM3SEK
_HIGHEST_STACK_LAYER = TINY_TAL

# Path variables
## Path to main project directory
PATH_ROOT = ../../../../..
PATH_APP = ../..
PATH_TAL = $(PATH_ROOT)/TAL
PATH_TINY_TAL = $(PATH_ROOT)/TINY_TAL
PATH_PAL = $(PATH_ROOT)/PAL
PATH_GLOB_INC = $(PATH_ROOT)/Include
PATH_SAM3S_LIB = $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Libraries
PATH_LDSCRIPTS = ./linker_scripts
PATH_SIO_SUPPORT = $(PATH_ROOT)/Applications/Helper_Files/SIO_Support

## General Flags
PROJECT = Wireless_UART
MCU = cortex-m3
TARGET_DIR = .
TARGET = $(TARGET_DIR)/$(PROJECT).elf
CC = arm-none-eabi-gcc
SIZE = arm-none-eabi-size
STRIP = arm-none-eabi-strip
OBJCOPY = arm-none-eabi-objcopy

## Options common to compile, link and assembly rules
COMMON = -mcpu=$(MCU) -mthumb

## Compile options common for all C compilation units.
CFLAGS = $(COMMON) $(INCLUDES)
CFLAGS += -Wall -Werror -g -Wundef -std=c99 -mlong-calls -ffunction-sections -DSIO_HUB -DUSB0 -Os
CFLAGS += -DDEBUG=0
CFLAGS += -DTAL_TYPE=$(_TAL_TYPE)
CFLAGS += -DPAL_GENERIC_TYPE=$(_PAL_GENERIC_TYPE)
CFLAGS += -DPAL_TYPE=$(_PAL_TYPE)
CFLAGS += -DBOARD_TYPE=$(_BOARD_TYPE)
CFLAGS += -DHIGHEST_STACK_LAYER=$(_HIGHEST_STACK_LAYER)
CFLAGS += -DIEEE_ADDR=0x123456789ABCDEF0
CFLAGS += -DNODMA_SPI

## Assembly specific flags
ASMFLAGS = $(COMMON) $(INCLUDES)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-g

## Linker flags
LDFLAGS = $(COMMON) --gc-sections -nostartfiles -Wl,-Map=$(PROJECT).map

# Defines which are the available memory targets
MEMORIES = flash

# Output directories
OBJ = obj

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
INCLUDES += -I $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Inc/
INCLUDES += -I$(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Inc
INCLUDES += -I$(PATH_SAM3S_LIB)/cmsis
INCLUDES += -I$(PATH_SAM3S_LIB)
## USB descriptor files
INCLUDES += -I$(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)/descriptors
## Include directories for specific boards type
INCLUDES += -I $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/
INCLUDES += -I $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)
## Include directories for SIO support
INCLUDES += -I $(PATH_SIO_SUPPORT)/Inc

# Directories where source files can be found
VPATH += $(PATH_APP)/Src
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)/descriptors
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src
VPATH += $(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src
VPATH += $(PATH_SAM3S_LIB)/
VPATH += $(PATH_SAM3S_LIB)/cmsis
VPATH += $(PATH_SAM3S_LIB)/usb/common/core
VPATH += $(PATH_SAM3S_LIB)/usb/device/core
VPATH += $(PATH_SAM3S_LIB)/usb/common/cdc
VPATH += $(PATH_SAM3S_LIB)/usb/device/cdc-serial
VPATH += $(PATH_SAM3S_LIB)/usb/usbd
VPATH += $(PATH_TINY_TAL)/$(_TAL_TYPE)/Src
VPATH += $(PATH_SIO_SUPPORT)/Src


# Objects built from C source files
## Main
C_OBJECTS += main.o

## platform dependent
C_OBJECTS += board_cstartup_gnu.o
C_OBJECTS += exceptions.o
C_OBJECTS += pal_board.o
C_OBJECTS += pal_irq.o

##SIO HUB
C_OBJECTS += pal_sio_hub.o

## SAM3S Generic PAL related
C_OBJECTS += pal_timer.o
C_OBJECTS += pal_trx_access.o
C_OBJECTS += pal_utils.o
C_OBJECTS += pal.o
C_OBJECTS += pal_usb.o

## SAM3S LIB objects
C_OBJECTS += core_cm3.o

## SAM3S USB LIB objects
C_OBJECTS += CDCDSerialDriverDescriptors.o
C_OBJECTS += USBD.o
C_OBJECTS += USBD_HAL.o
C_OBJECTS += CDCDSerialDriver.o
C_OBJECTS += CDCSetControlLineStateRequest.o
C_OBJECTS += CDCLineCoding.o
C_OBJECTS += USBDDriver.o
C_OBJECTS += USBDCallbacks_Initialized.o
C_OBJECTS += USBDCallbacks_Resumed.o
C_OBJECTS += USBDCallbacks_Suspended.o
C_OBJECTS += USBDCallbacks_Reset.o
C_OBJECTS += USBDDriverCb_CfgChanged.o
C_OBJECTS += USBDDriverCb_IfSettingChanged.o
C_OBJECTS += USBSetAddressRequest.o
C_OBJECTS += USBGenericDescriptor.o
C_OBJECTS += USBInterfaceRequest.o
C_OBJECTS += USBGenericRequest.o
C_OBJECTS += USBGetDescriptorRequest.o
C_OBJECTS += USBSetConfigurationRequest.o
C_OBJECTS += USBFeatureRequest.o
C_OBJECTS += USBEndpointDescriptor.o
C_OBJECTS += USBConfigurationDescriptor.o

## TAL/TINY_TAL objects
C_OBJECTS += tiny_tal_init.o
C_OBJECTS += tiny_tal_irq_handler.o
C_OBJECTS += tiny_tal_pib.o
C_OBJECTS += tiny_tal_pwr_mgmt.o
C_OBJECTS += tiny_tal_rx_enable.o
C_OBJECTS += tiny_tal_rx.o
C_OBJECTS += tiny_tal_tx.o
C_OBJECTS += tiny_tal.o

## SIO Handler objects
C_OBJECTS += sio_handler.o

# Append  project directory to output filename
OUTPUT := $(PROJECT)

#-------------------------------------------------------------------------------
#       Build
#-------------------------------------------------------------------------------

all: $(OBJ) $(MEMORIES)

$(OBJ):
	mkdir $@


define RULES
C_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(C_OBJECTS))
ASM_OBJECTS_$(1) = $(addprefix $(OBJ)/$(1)_, $(ASM_OBJECTS))

$(1): $$(ASM_OBJECTS_$(1)) $$(C_OBJECTS_$(1))
	$(CC) $(LDFLAGS) -T"$(PATH_LDSCRIPTS)/$$@.lds" -o $(OUTPUT).elf $$^
	$(OBJCOPY) -O binary $(OUTPUT).elf $(OUTPUT).bin
	$(SIZE) $$^ $(OUTPUT).elf


$$(C_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.c Makefile $(OBJ)
ifeq ($(SILENT),true)
	@echo compiling $$@
	@-$(CC) $(CFLAGS) -D$(1) -c -o $$@ $$<
else
	$(CC) $(CFLAGS) -D$(1) -c -o $$@ $$<
endif

$$(ASM_OBJECTS_$(1)): $(OBJ)/$(1)_%.o: %.S Makefile $(OBJ)
	$(CC) $(ASFLAGS) -D$(1) -c -o $$@ $$<

ifeq ($(SILENT),true)
	# compiling $$@
	@-$(CC) $(ASMFLAGS) -D$(1) -c -o $$@ $$<
else
	$(CC) $(ASMFLAGS) -D$(1) -c -o $$@ $$<
endif

endef

$(foreach MEMORY, $(MEMORIES), $(eval $(call RULES,$(MEMORY))))

clean:
	-rm -f $(OBJ)/*.o $(OBJ)/*.lst $(OBJ)/.d -r $(OBJ)/.dep
	-rm -f $(PROJECT).bin $(PROJECT).elf $(PROJECT).map
