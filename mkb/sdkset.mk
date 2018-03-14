#=============================================
# SDKSET CONFIG
#=============================================
RTOSDIR?=freertos_v8.1.2
LWIPDIR?=lwip_v1.4.1
# -------------------------------------------------------------------
# FLAGS
# -------------------------------------------------------------------
CFLAGS = -DCONFIG_PLATFORM_8711B -DF_CPU=125000000L
CFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g2 -Os
CFLAGS += -Wdouble-promotion -fsingle-precision-constant
CFLAGS += -std=gnu99 -Wall -Werror 
CFLAGS += -fno-common -fmessage-length=0 -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-short-enums -fsigned-char
CFLAGS += -Wno-pointer-sign

LFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g2 -Os
LFLAGS += --specs=nano.specs -nostartfiles
LFLAGS += -Wl,-Map=$(BIN_DIR)/application.map -Wl,--gc-sections -Wl,--cref -Wl,--entry=Reset_Handler -Wl,--no-enum-size-warning -Wl,--no-wchar-size-warning
LFLAGS += -Wl,-wrap,malloc -Wl,-wrap,free -Wl,-wrap,realloc

# LIBS
# -------------------------------------------------------------------

ifndef NO_SDK_FILES
all: LIBS += _platform _wlan _wps _dct _rtlstd m c nosys gcc _websocket
mp: LIBS += _platform _wlan_mp _wps _dct _rtlstd m c nosys gcc _websocket
endif
# m c nosys gcc

PATHLIBS+=sdk/component/soc/realtek/8711b/misc/bsp/lib/common/GCC

ifdef SDK_RAM
LDFILE?=sdk/component/soc/realtek/8711b/misc/bsp/lib/common/GCC/rlx8711B-symbol-v02-img1_ram.ld
else
LDFILE?=sdk/component/soc/realtek/8711b/misc/bsp/lib/common/GCC/rlx8711B-symbol-v02-img2_xip$(OTA_IDX).ld
endif

BOOTS?=sdk/component/soc/realtek/8711b/misc/bsp/image
BOOT_ALL?=sdk/component/soc/realtek/8711b/misc/bsp/image/boot_all.bin

# Include folder list
# -------------------------------------------------------------------
INCLUDES = ../inc
INCLUDES += project/inc
INCLUDES += sdk/component/soc/realtek/common/bsp
INCLUDES += sdk/component/os/freertos
INCLUDES += sdk/component/os/freertos/$(RTOSDIR)/Source/include
INCLUDES += sdk/component/os/freertos/$(RTOSDIR)/Source/portable/GCC/ARM_CM4F
INCLUDES += sdk/component/os/os_dep/include
INCLUDES += sdk/component/common/api/network/include
INCLUDES += sdk/component/common/api
INCLUDES += sdk/component/common/api/at_cmd
INCLUDES += sdk/component/common/api/platform
ifdef WIFI_API_SDK
INCLUDES += sdk/component/common/api/wifi
INCLUDES += sdk/component/common/api/wifi/rtw_wpa_supplicant/src
INCLUDES += sdk/component/common/api/wifi/rtw_wowlan
INCLUDES += sdk/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant
endif
INCLUDES += sdk/component/common/drivers/wlan/realtek/include
INCLUDES += sdk/component/common/drivers/wlan/realtek/src/osdep 
INCLUDES += sdk/component/common/drivers/wlan/realtek/src/osdep/freertos 
INCLUDES += sdk/component/common/application
INCLUDES += sdk/component/common/application/mqtt/MQTTClient
INCLUDES += sdk/component/common/application/mqtt/MQTTPacket
INCLUDES += sdk/component/common/example
INCLUDES += sdk/component/common/example/wlan_fast_connect
INCLUDES += sdk/component/common/drivers/modules
INCLUDES += sdk/component/common/drivers/sdio/realtek/sdio_host/inc
INCLUDES += sdk/component/common/drivers/inic/rtl8711b
INCLUDES += sdk/component/common/drivers/usb_class/device
INCLUDES += sdk/component/common/drivers/usb_class/device/class
INCLUDES += sdk/component/common/drivers/wlan/realtek/include
INCLUDES += sdk/component/common/drivers/wlan/realtek/src/osdep
INCLUDES += sdk/component/common/drivers/wlan/realtek/src/hci
INCLUDES += sdk/component/common/drivers/wlan/realtek/src/hal
INCLUDES += sdk/component/common/drivers/wlan/realtek/src/hal/rtl8711b
INCLUDES += sdk/component/common/drivers/wlan/realtek/src/hal/OUTSRC
INCLUDES += sdk/component/common/drivers/wlan/realtek/wlan_ram_map/rom
INCLUDES += sdk/component/common/file_system
INCLUDES += sdk/component/common/network
INCLUDES += sdk/component/common/network/lwip/$(LWIPDIR)/port/realtek/freertos
INCLUDES += sdk/component/common/network/lwip/$(LWIPDIR)/src/include
INCLUDES += sdk/component/common/network/lwip/$(LWIPDIR)/src/include/lwip
INCLUDES += sdk/component/common/network/lwip/$(LWIPDIR)/src/include/ipv4
INCLUDES += sdk/component/common/network/lwip/$(LWIPDIR)/port/realtek
INCLUDES += sdk/component/common/network/ssl/polarssl-1.3.8/include
INCLUDES += sdk/component/common/network/ssl/ssl_ram_map/rom
INCLUDES += sdk/component/common/test
INCLUDES += sdk/component/common/utilities
INCLUDES += sdk/component/soc/realtek/8711b/app/monitor/include
INCLUDES += sdk/component/soc/realtek/8711b/cmsis
INCLUDES += sdk/component/soc/realtek/8711b/cmsis/device
INCLUDES += sdk/component/soc/realtek/8711b/fwlib
INCLUDES += sdk/component/soc/realtek/8711b/fwlib/include
INCLUDES += sdk/component/soc/realtek/8711b/fwlib/ram_lib/crypto
INCLUDES += sdk/component/soc/realtek/8711b/fwlib/rom_lib
INCLUDES += sdk/component/soc/realtek/8711b/swlib/os_dep/include
INCLUDES += sdk/component/soc/realtek/8711b/swlib/std_lib/include
INCLUDES += sdk/component/soc/realtek/8711b/swlib/std_lib/libc/include
INCLUDES += sdk/component/soc/realtek/8711b/swlib/std_lib/libc/rom/string
INCLUDES += sdk/component/soc/realtek/8711b/swlib/std_lib/libgcc/rtl8195a/include
INCLUDES += sdk/component/soc/realtek/8711b/swlib/rtl_lib
INCLUDES += sdk/component/soc/realtek/8711b/misc
INCLUDES += sdk/component/soc/realtek/8711b/misc/os
INCLUDES += sdk/component/common/mbed/api
INCLUDES += sdk/component/common/mbed/hal
INCLUDES += sdk/component/common/mbed/hal_ext
#INCLUDES += sdk/component/common/mbed/targets/cmsis/rtl8711b
INCLUDES += sdk/component/common/mbed/targets/hal/rtl8711b
INCLUDES += sdk/project/realtek_8195a_gen_project/rtl8195a/sw/lib/sw_lib/mbed/api
INCLUDES += sdk/component/common/application/mqtt/MQTTClient
INCLUDES += sdk/component/common/network/websocket

# Source file list
# -------------------------------------------------------------------
SRC_C ?=
DRAM_C ?=
BOOT_C ?=

ifndef NO_SDK_FILES

#bootloader
#SRC_C += sdk/component/soc/realtek/8195a/fwlib/ram_lib/rtl_bios_data.c
#BOOT_C += sdk/component/soc/realtek/8195a/fwlib/ram_lib/rtl_boot.c

#app uart_adapter
SRC_C += sdk/component/common/application/uart_adapter/uart_adapter.c

#cmsis
SRC_C += sdk/component/soc/realtek/8711b/cmsis/device/app_start.c
SRC_C += sdk/component/soc/realtek/8711b/fwlib/ram_lib/startup.c
SRC_C += sdk/component/soc/realtek/8711b/cmsis/device/system_8195a.c

ifdef USE_AT
#console
SRC_C += sdk/component/common/api/at_cmd/atcmd_lwip.c
SRC_C += sdk/component/common/api/at_cmd/atcmd_sys.c
SRC_C += sdk/component/common/api/at_cmd/atcmd_wifi.c
SRC_C += sdk/component/common/api/at_cmd/log_service.c
SRC_C += sdk/component/soc/realtek/8711b/app/monitor/ram/low_level_io.c
SRC_C += sdk/component/soc/realtek/8711b/app/monitor/ram/monitor.c
SRC_C += sdk/component/soc/realtek/8711b/app/monitor/ram/rtl_trace.c
endif

ifndef NEW_CONSOLE
SRC_C += sdk/component/soc/realtek/8711b/app/monitor/ram/rtl_consol.c
endif

#lib
#SRC_C += sdk/component/soc/realtek/8711b/misc/bsp/lib/common/IAR/lib_platform.a
#SRC_C += sdk/component/soc/realtek/8711b/misc/bsp/lib/common/IAR/lib_rtlstd.a
#SRC_C += sdk/component/soc/realtek/8711b/misc/bsp/lib/common/IAR/lib_wlan.a
#SRC_C += sdk/component/soc/realtek/8711b/misc/bsp/lib/common/IAR/lib_wlan_mp.a
#SRC_C += sdk/component/soc/realtek/8711b/misc/bsp/lib/common/IAR/lib_wps.a

ifdef WIFI_API_SDK
#network api wifi rtw_wpa_supplicant
SRC_C += sdk/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_eap_config.c #implicit declaration of function 'eap_peer_unregister_methods' 'eap_sm_deinit'...
SRC_C += sdk/component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_wps_config.c

#network api wifi
SRC_C += sdk/component/common/api/wifi/wifi_conf.c
SRC_C += sdk/component/common/api/wifi/wifi_ind.c
SRC_C += sdk/component/common/api/wifi/wifi_promisc.c
SRC_C += sdk/component/common/api/wifi/wifi_simple_config.c
SRC_C += sdk/component/common/api/wifi/wifi_util.c
endif

#network api
SRC_C += sdk/component/common/api/lwip_netconf.c

ifndef NO_SDK_NETAPP
#network app
SRC_C += sdk/component/common/application/mqtt/MQTTClient/MQTTClient.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTConnectClient.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTConnectServer.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTDeserializePublish.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTFormat.c
SRC_C += sdk/component/common/application/mqtt/MQTTClient/MQTTFreertos.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTPacket.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTSerializePublish.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTSubscribeClient.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTSubscribeServer.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTUnsubscribeClient.c
SRC_C += sdk/component/common/application/mqtt/MQTTPacket/MQTTUnsubscribeServer.c
SRC_C += sdk/component/common/api/network/src/ping_test.c
SRC_C += sdk/component/common/utilities/ssl_client.c
SRC_C += sdk/component/common/utilities/tcptest.c
SRC_C += sdk/component/common/api/network/src/wlan_network.c
endif #NO_SDK_NETAPP

#network lwip api
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/api/api_lib.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/api/api_msg.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/api/err.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/api/netbuf.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/api/netdb.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/api/netifapi.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/api/sockets.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/api/tcpip.c

#network lwip core ipv4
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/ipv4/autoip.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/ipv4/icmp.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/ipv4/igmp.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/ipv4/inet.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/ipv4/inet_chksum.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/ipv4/ip.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/ipv4/ip_addr.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/ipv4/ip_frag.c

#network lwip core
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/def.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/dhcp.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/dns.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/init.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/lwip_timers.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/mem.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/memp.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/netif.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/pbuf.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/raw.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/stats.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/sys.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/tcp.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/tcp_in.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/tcp_out.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/core/udp.c

#network lwip netif
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/src/netif/etharp.c

#network lwip port
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/port/realtek/freertos/ethernetif.c
SRC_C += sdk/component/common/drivers/wlan/realtek/src/osdep/lwip_intf.c
SRC_C += sdk/component/common/network/lwip/$(LWIPDIR)/port/realtek/freertos/sys_arch.c

#network - wsclient
SRC_C += sdk/component/common/network/websocket/wsclient_tls.c

#network lwip
SRC_C += sdk/component/common/network/dhcp/dhcps.c
ifndef NEW_SNTP
SRC_C += sdk/component/common/network/sntp/sntp.c
endif

ifndef NO_SDK_SSL
#network polarssl polarssl
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/aesni.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/blowfish.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/camellia.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/ccm.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/certs.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/cipher.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/cipher_wrap.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/debug.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/ecp_ram.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/entropy.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/entropy_poll.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/error.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/gcm.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/havege.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/md2.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/md4.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/memory_buffer_alloc.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/net.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/padlock.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/pbkdf2.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/pkcs11.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/pkcs12.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/pkcs5.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/pkparse.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/platform.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/ripemd160.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/ssl_cache.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/ssl_ciphersuites.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/ssl_cli.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/ssl_srv.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/ssl_tls.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/threading.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/timing.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/version.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/version_features.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/x509.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/x509_create.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/x509_crl.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/x509_crt.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/x509_csr.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/x509write_crt.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/x509write_csr.c
SRC_C += sdk/component/common/network/ssl/polarssl-1.3.8/library/xtea.c
endif #NO_SDK_SSL

#network polarssl ssl_ram_map
SRC_C += sdk/component/common/network/ssl/ssl_ram_map/ssl_ram_map.c

#os freertos portable
SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/portable/MemMang/heap_5.c
SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/portable/GCC/ARM_CM4F/port.c
#SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/portable/IAR/ARM_CM4F/portasm.s

#os freertos
SRC_C += sdk/component/os/freertos/cmsis_os.c
SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/croutine.c
SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/event_groups.c
SRC_C += sdk/component/os/freertos/freertos_service.c
SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/list.c
SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/queue.c
SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/tasks.c
SRC_C += sdk/component/os/freertos/$(RTOSDIR)/Source/timers.c

#os osdep
SRC_C += sdk/component/os/os_dep/device_lock.c
SRC_C += sdk/component/os/os_dep/osdep_service.c

#peripheral api
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/analogin_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/dma_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/efuse_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/flash_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/gpio_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/gpio_irq_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/i2c_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/i2s_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/nfc_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/pinmap.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/pinmap_common.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/port_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/pwmout_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/rtc_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/serial_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/sleep.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/spi_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/sys_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/timer_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/us_ticker.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/us_ticker_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/wait_api.c
SRC_C += sdk/component/common/mbed/targets/hal/rtl8711b/wdt_api.c

#peripheral rtl8710b
SRC_C += sdk/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_dsleepcfg.c
SRC_C += sdk/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_dstandbycfg.c
SRC_C += sdk/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_intfcfg.c
SRC_C += sdk/component/soc/realtek/8711b/misc/rtl8710b_ota.c
SRC_C += sdk/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_pinmapcfg.c
SRC_C += sdk/component/soc/realtek/8711b/fwlib/ram_lib/rtl8710b_sleepcfg.c

#utilities example
ifndef NO_SDK_EXAMPLE
SRC_C += sdk/component/common/example/bcast/example_bcast.c
SRC_C += sdk/component/common/example/dct/example_dct.c
SRC_C += sdk/component/common/example/eap/example_eap.c
SRC_C += sdk/component/common/example/example_entry.c
SRC_C += sdk/component/common/example/get_beacon_frame/example_get_beacon_frame.c
SRC_C += sdk/component/common/example/high_load_memory_use/example_high_load_memory_use.c
SRC_C += sdk/component/common/example/http_client/example_http_client.c
SRC_C += sdk/component/common/example/http_download/example_http_download.c
SRC_C += sdk/component/common/example/mcast/example_mcast.c
SRC_C += sdk/component/common/example/mdns/example_mdns.c
SRC_C += sdk/component/common/example/mqtt/example_mqtt.c
SRC_C += sdk/component/common/example/nonblock_connect/example_nonblock_connect.c
SRC_C += sdk/component/common/example/rarp/example_rarp.c
SRC_C += sdk/component/common/example/sntp_showtime/example_sntp_showtime.c
SRC_C += sdk/component/common/example/socket_select/example_socket_select.c
SRC_C += sdk/component/common/example/ssl_download/example_ssl_download.c
SRC_C += sdk/component/common/example/ssl_server/example_ssl_server.c
SRC_C += sdk/component/common/example/tcp_keepalive/example_tcp_keepalive.c
SRC_C += sdk/component/common/example/uart_atcmd/example_uart_atcmd.c
SRC_C += sdk/component/common/example/wifi_mac_monitor/example_wifi_mac_monitor.c
SRC_C += sdk/component/common/example/wlan_fast_connect/example_wlan_fast_connect.c
SRC_C += sdk/component/common/example/wlan_scenario/example_wlan_scenario.c
SRC_C += sdk/component/common/example/websocket/example_wsclient.c
SRC_C += sdk/component/common/example/xml/example_xml.c

#utilities
SRC_C += sdk/component/common/utilities/cJSON.c
SRC_C += sdk/component/common/utilities/http_client.c
SRC_C += sdk/component/common/utilities/uart_socket.c
SRC_C += sdk/component/common/utilities/webserver.c
SRC_C += sdk/component/common/utilities/xml.c
endif #NO_SDK_EXAMPLE

endif #NO_SDK_FILES

# -------------------------------------------------------------------
# Add Source file list
# -------------------------------------------------------------------
ADD_SRC_C ?=
# -------------------------------------------------------------------
# SAMPLES
# -------------------------------------------------------------------

