#include "driver/can.h"


typedef struct{
    char* message;
    int length;
} queue_msg_t;

esp_err_t convert_can_frame(char* buffer, int buffer_size, const can_message_t* can_msg);
