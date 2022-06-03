add_executable(main
        main.c
        )

# pull in common dependencies
target_link_libraries(main pico_stdlib pico_multicore hardware_i2c FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)

# pull in sources
target_sources(main
    PRIVATE
        dert_sense_air_task.c
        dert_sense_light_task.c
        dert_sense_soil_task.c
        dert_toggle_relays_task_test.c
        dert_i2c.c)

# enable uart output
pico_enable_stdio_uart(main 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(main)