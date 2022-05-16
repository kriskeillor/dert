if (TARGET tinyusb_device)
    add_executable(dert_freertos
            dert_freertos.c
            )

    # pull in common dependencies
    target_link_libraries(dert_freertos pico_stdlib hardware_i2c FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(dert_freertos 1)
    pico_enable_stdio_uart(dert_freertos 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(dert_freertos)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building dert_freertos because TinyUSB submodule is not initialized in the SDK")
endif()