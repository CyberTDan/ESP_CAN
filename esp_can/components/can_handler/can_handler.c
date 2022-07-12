#include <stdio.h>
#include "can_handler.h"

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/can.h"

#include "tcp_protocol.h"


#define NO_OF_ITERS                     3
#define RX_TASK_PRIO                    9
#define TX_GPIO_NUM                     21
#define RX_GPIO_NUM                     22
#define EXAMPLE_TAG                     "CAN Listen Only"

#define ID_MASTER_STOP_CMD              0x0A0
#define ID_MASTER_START_CMD             0x0A1
#define ID_MASTER_PING                  0x0A2
#define ID_SLAVE_STOP_RESP              0x0B0
#define ID_SLAVE_DATA                   0x0B1
#define ID_SLAVE_PING_RESP              0x0B2


static const can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
static const can_timing_config_t t_config = CAN_TIMING_CONFIG_25KBITS();
//Set TX queue length to 0 due to listen only mode
static const can_general_config_t g_config = {.mode = CAN_MODE_LISTEN_ONLY,
                                              .tx_io = TX_GPIO_NUM, .rx_io = RX_GPIO_NUM,
                                              .clkout_io = CAN_IO_UNUSED, .bus_off_io = CAN_IO_UNUSED,
                                              .tx_queue_len = 0, .rx_queue_len = 5,
                                              .alerts_enabled = CAN_ALERT_NONE,
                                              .clkout_divider = 0};


static SemaphoreHandle_t rx_sem;


static void can_receive_task(QueueHandle_t wifi_can_queue)
{

    while (1) {
        can_message_t rx_msg;
        // can_receive(&rx_msg, portMAX_DELAY);
        // printf("Reseived Can message:\n");
        // printf("    Msg ID: %d\n", rx_msg.identifier);
        // printf("    Message length: %d\n", rx_msg.data_length_code);
        // printf("    ");
        // for(int i = 0; i < rx_msg.data_length_code; i++){
        //     printf("%d  ", rx_msg.data[i]);
        // }
        // printf("\n");

        //  fill can msg
        rx_msg.identifier = 10001;
        rx_msg.data_length_code = 8;
        for(int i = 0; i < 8; i++) rx_msg.data[i] = i+6;
        //  create json str
        char* data = (char*) malloc(100*sizeof(char));
        convert_can_frame(data, 100, &rx_msg);

        queue_msg_t* queue_msg = (queue_msg_t*) malloc( sizeof(queue_msg_t) );
        queue_msg->length = 100;
        queue_msg->message = data;
        if( xQueueSendToBack( wifi_can_queue, ( void * ) &queue_msg, ( TickType_t ) 10 ) != pdPASS )
        {
            printf("transmition failed\n");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));

        // if (rx_msg.identifier == ID_MASTER_PING) {
        //     ESP_LOGI(EXAMPLE_TAG, "Received master ping");
        // } else if (rx_msg.identifier == ID_SLAVE_PING_RESP) {
        //     ESP_LOGI(EXAMPLE_TAG, "Received slave ping response");
        // } else if (rx_msg.identifier == ID_MASTER_START_CMD) {
        //     ESP_LOGI(EXAMPLE_TAG, "Received master start command");
        //     start_cmd = true;
        // } else if (rx_msg.identifier == ID_SLAVE_DATA) {
        //     uint32_t data = 0;
        //     for (int i = 0; i < rx_msg.data_length_code; i++) {
        //         data |= (rx_msg.data[i] << (i * 8));
        //     }
        //     ESP_LOGI(EXAMPLE_TAG, "Received data value %d", data);
        // } else if (rx_msg.identifier == ID_MASTER_STOP_CMD) {
        //     ESP_LOGI(EXAMPLE_TAG, "Received master stop command");
        // } else if (rx_msg.identifier == ID_SLAVE_STOP_RESP) {
        //     ESP_LOGI(EXAMPLE_TAG, "Received slave stop response");
        //     stop_resp = true;
        // }
        // if (start_cmd && stop_resp) {
        //     //Each iteration is complete after a start command and stop response is received
        //     iterations++;
        //     start_cmd = 0;
        //     stop_resp = 0;
        // }
    }

    
}



void launchCanTask(void * pvParameters)
{
    QueueHandle_t wifi_can_queue = (QueueHandle_t) pvParameters;
    //Install and start CAN driver
    ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(EXAMPLE_TAG, "Driver installed");
    ESP_ERROR_CHECK(can_start());
    ESP_LOGI(EXAMPLE_TAG, "Driver started");

    


    can_receive_task(wifi_can_queue);
    //vTaskDelay(pdMS_TO_TICKS(100));

    //Stop and uninstall CAN driver
    ESP_ERROR_CHECK(can_stop());
    ESP_LOGI(EXAMPLE_TAG, "Driver stopped");
    ESP_ERROR_CHECK(can_driver_uninstall());
    ESP_LOGI(EXAMPLE_TAG, "Driver uninstalled");

    vTaskDelete(NULL);
}
