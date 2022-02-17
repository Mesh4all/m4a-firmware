idf_component_register(SRC_DIRS "."
                       INCLUDE_DIRS "."
                       REQUIRES network storage unity esp-tls esp_http_client
                       EMBED_TXTFILES ../server_root_cert.pem)