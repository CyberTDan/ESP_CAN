#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"


#include "driver/gpio.h"
#include "driver/twai.h"

#include "tcp_protocol.h"

twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_19, GPIO_NUM_18, TWAI_MODE_NORMAL);
twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();


static void can_receive_task(QueueHandle_t wifi_can_queue)
{
    printf("CAN REader launched\n");

    while (1) {
        twai_message_t canrxmsg;

        for(;;){

            while(twai_receive(&canrxmsg, 0) == ESP_OK){
                printf("RX can message: ID %x, DLC %d\n", canrxmsg.identifier, canrxmsg.data_length_code);
                char* data = (char*) malloc(160*sizeof(char));
                memset(data, 0, 160);
                convert_can_frame(data, 160, &canrxmsg);
                queue_msg_t* queue_msg = (queue_msg_t*) malloc( sizeof(queue_msg_t) );
                queue_msg->length = strlen(data);
                queue_msg->message = data;
                if( xQueueSendToBack( wifi_can_queue, ( void * ) &queue_msg, ( TickType_t ) 10 ) != pdPASS )
                {
                    printf("transmition failed\n");
                }
            }

            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}



void launchCanTask(void * pvParameters)
{
    QueueHandle_t wifi_can_queue = (QueueHandle_t) pvParameters;
    if(twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK){
    	puts("TWAI Driver installed");
    }else{
    	puts("TWAI Driver install failed");
    }

    if(twai_start() == ESP_OK){
    	puts("TWAI Driver started");
    }else{
    	puts("TWAI Driver start failed");
    }

    can_receive_task(wifi_can_queue);


    

    vTaskDelete(NULL);
}
