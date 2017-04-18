ifdef CONFIG_HAS_CC3200SDK
ZEPHYRINCLUDE +=-I$(srctree)/ext/hal/ti/cc3200sdk
ZEPHYRINCLUDE +=-I$(srctree)/ext/hal/ti/cc3200sdk/inc
ZEPHYRINCLUDE +=-I$(srctree)/ext/hal/ti/cc3200sdk/driverlib
endif # CONFIG_HAS_CC3200SDK

ifdef CONFIG_CC3200SDK_SIMPLELINK
ZEPHYRINCLUDE +=-I$(srctree)/ext/hal/ti/cc3200sdk/simplelink
ZEPHYRINCLUDE +=-I$(srctree)/ext/hal/ti/cc3200sdk/simplelink/include
endif

ifdef CONFIG_CC3200SDK_ROM_DRIVERLIB
KBUILD_CFLAGS += -DTARGET_IS_CC3200
endif  # CONFIG_CC3200SDK_ROM_DRIVERLIB
