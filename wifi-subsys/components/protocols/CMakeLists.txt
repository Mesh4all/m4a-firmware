file(GLOB_RECURSE SOURCES src/*.c)
idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS src ../../main
                                        REQUIRES storage network esp-tls esp_http_client
                                        EMBED_TXTFILES ./server_root_cert.pem
                                        )
