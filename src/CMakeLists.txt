add_executable(${ProjectName}
    main.c
    led_buzzer.c        
    infrared_sensor.c 
    sound_sensor.c
    light_sensor.c
    servo_motor.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${ProjectName} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4 
    hardware_pwm
    hardware_i2c
    hardware_adc
    )

pico_add_extra_outputs(${ProjectName})