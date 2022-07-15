#include <stdio.h>
#include "tcp_protocol.h"

#include "driver/twai.h"
#include <string.h>

#define MINIMAL_BUFFER_SIZE 100

esp_err_t convert_can_frame(char* buffer, int buffer_size, const twai_message_t* can_msg){
    //  check if buffer is small 
    if (buffer_size < MINIMAL_BUFFER_SIZE) {
        printf("CAN_msg converter: buffer size smaller than \"MINIMAL_BUFFER_SIZE\"");
        return ESP_FAIL;
    }
    
    memset(buffer, 0, buffer_size); //  clear buffer
    buffer[0] = '{';  //  set initial symbol
    //  set message type
    strcat(buffer, "\"msg_type\":\"can_msg\",");
    //  add ID type
    strcat(buffer, "\"id_type\":\"extended\",");
    //  add ID
    char convert_buffer[20] = "";
    // itoa(can_msg->identifier, convert_buffer, 20);
    sprintf(convert_buffer, "\"0x%x\",", can_msg->identifier);
    strcat(buffer, "\"id\":");
    strcat(buffer, convert_buffer);
    //  add message length
    memset(convert_buffer, 0, 20);  //  clear buffer
    //itoa(can_msg->data_length_code, convert_buffer, 20);
    sprintf(convert_buffer, "\"%d\",", can_msg->data_length_code);
    strcat(buffer, "\"data_size\":");
    strcat(buffer, convert_buffer);
    //  add frame data  
    strcat(buffer, "\"frame_data\":\"");
    if (can_msg->data_length_code > 0){
        memset(convert_buffer, 0, 20);  //  clear buffer
        for (int idx = 0; idx < can_msg->data_length_code; idx++){
            sprintf(convert_buffer, "%x ", can_msg->data[idx]);
            strcat(buffer, convert_buffer);
        }
    }
    strcat(buffer, "\"}");
    
    return ESP_OK;
}