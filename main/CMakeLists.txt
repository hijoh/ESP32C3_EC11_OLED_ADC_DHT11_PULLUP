idf_component_register(SRCS 
                "app_main.c" "encoder.c" "key.c"
                "i2c_simple_oled.c"
                "font.c" "dht11.c"

                    INCLUDE_DIRS ".")
# //set(COMPONENT_ADD_INCLUDEDIRS  ".")
# register_component()
