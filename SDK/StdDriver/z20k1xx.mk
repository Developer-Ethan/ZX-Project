# Component Makefile
#
ifneq ($(CONFIG_ENABLE_LIB), 1)
## These include paths would be exported to project level
COMPONENT_ADD_INCLUDEDIRS += Inc

## not be exported to project level
COMPONENT_PRIV_INCLUDEDIRS :=

## This component's src
COMPONENT_SRCS := Src/Z20K11xM_adc.c \
                  Src/Z20K11xM_can.c \
                  Src/Z20K11xM_clock.c \
                  Src/Z20K11xM_cmp.c \
                  Src/Z20K11xM_crc.c \
                  Src/Z20K11xM_dma.c \
                  Src/Z20K11xM_drv.c \
                  Src/Z20K11xM_ewdt.c \
                  Src/Z20K11xM_flash.c \
                  Src/Z20K11xM_gpio.c \
                  Src/Z20K11xM_hwdiv.c \
                  Src/Z20K11xM_i2c.c \
                  Src/Z20K11xM_pmu.c \
                  Src/Z20K11xM_regfile.c \
                  Src/Z20K11xM_rtc.c \
                  Src/Z20K11xM_spi.c \
                  Src/Z20K11xM_srmc.c \
                  Src/Z20K11xM_stim.c \
                  Src/Z20K11xM_sysctrl.c \
                  Src/Z20K11xM_tdg.c \
                  Src/Z20K11xM_tim.c \
                  Src/Z20K11xM_tmu.c \
                  Src/Z20K11xM_uart.c \
                  Src/Z20K11xM_wdog.c 


COMPONENT_OBJS := $(patsubst %.c,%.o, $(COMPONENT_SRCS))
COMPONENT_OBJS := $(patsubst %.cpp,%.o, $(COMPONENT_OBJS))

COMPONENT_SRCDIRS := Src
else

COMPONENT_LIB_ONLY := 1
COMPONENT_ADD_INCLUDEDIRS += Inc
LIBS ?= StdDriver
COMPONENT_ADD_LDFLAGS += -L$(COMPONENT_PATH)/Lib/GCC $(addprefix -l,$(LIBS))
ALL_LIB_FILES := $(patsubst %,$(COMPONENT_PATH)/Lib/GCC/lib%.a,$(LIBS))
COMPONENT_ADD_LINKER_DEPS := $(ALL_LIB_FILES)

endif

##
#CPPFLAGS +=
