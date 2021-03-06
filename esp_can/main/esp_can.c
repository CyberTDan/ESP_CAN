#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"
#include "wifi_ap.h"
#include "can_handler.h"
#include "tcp_protocol.h"


void app_main(void)
{   
    /********Print chip information**********/
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());


    /********Launch NVS Setup**********/
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /*************QUEUE for wifi and cancommunication*************/
    vTaskDelay(pdMS_TO_TICKS(4000));
    QueueHandle_t wifi_can_queue = xQueueCreate( 20, sizeof( queue_msg_t* ) );
    if (wifi_can_queue != 0){
        printf("Queue initilized succesfully\n");
    } else {
        printf("Queue initilization failed\n");
    }


    /*************START WIFI TASK*************/
    launchWifiTask(wifi_can_queue);

    /**************START CAN TASK************/
    xTaskCreatePinnedToCore(launchCanTask, "CAN_rx", 4096, wifi_can_queue, 9, NULL, tskNO_AFFINITY);
}