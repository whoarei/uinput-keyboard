#
# Makefile for virtual non-volatile ram
#

ifeq ($(obj),)
	obj=.
endif

ifeq ($(KDIR),)
KDIR := $(KERNEL_OUT_ROOT)
endif

include $(KDIR)/.config

TARGET_MODULE := uinput

obj-m = $(TARGET_MODULE).o

MODULE_BUILD_DIR := $(KERNEL_MODULE_BUILD_DIR)/$(TARGET_MODULE)
MODULE_TARGET_DIR := $(KERNEL_MODULE_INSTAL_DIR)
DEPENDS_SYMVERS_DIR := ${PUBLIC_SYMVERS}/epay_pinentry

all: 
	mkdir -p $(MODULE_BUILD_DIR)
	lndir $(shell pwd) $(MODULE_BUILD_DIR)
	cp -f ${DEPENDS_SYMVERS_DIR}/Module.symvers $(MODULE_BUILD_DIR)
	$(MAKE) -C $(KDIR) M=$(MODULE_BUILD_DIR) modules
	cp $(MODULE_BUILD_DIR)/$(TARGET_MODULE).ko $(MODULE_TARGET_DIR)


clean:
	$(MAKE) -C $(KDIR) M=$(MODULE_BUILD_DIR) clean
	rm -f $(MODULE_TARGET_DIR)/$(TARGET_MODULE).ko
