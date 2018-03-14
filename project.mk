#=============================================
# Project Config
#=============================================
#NO_SDK_SSL = 1
#NO_SDK_NETAPP = 1
#NO_SDK_EXAMPLE = 1
#WIFI_API_SDK = 1
#NEW_CONSOLE = 1
#NEW_SNTP = 1
WIFI_API_SDK = 1
USE_AT = 1
include $(GCCMK_PATH)sdkset.mk
#=============================================
# Project Files
#=============================================
#user main
ADD_SRC_C += project/src/main.c
# components
#ADD_SRC_C += ...
#INCLUDES += ...
#CFLAGS += -w
