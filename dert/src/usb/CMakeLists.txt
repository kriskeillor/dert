if (TARGET tinyusb_device)
    add_executable(dert_usb
            dert_usb.c
            )

    # pull in common dependencies
    target_link_libraries(dert_usb pico_stdlib hardware_i2c)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(dert_usb 1)
    pico_enable_stdio_uart(dert_usb 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(dert_usb)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building dert_usb because TinyUSB submodule is not initialized in the SDK")
endif()
