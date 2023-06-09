#include <Arduino.h>
#include "FastDAC.h"

#define AUDIO_SAMPLE_RATE   44100         
#define AUDIO_PEAK_VALUE    128
#define AUDIO_BUFFER_SIZE   50
#define DAC_BUFFER_SIZE     1100
#define SINE_LUT_SIZE       1024

uint8_t sin_lut[SINE_LUT_SIZE] = {
    128, 128, 129, 130, 131, 131, 132, 133, 134, 135, 135, 136, 137, 138, 138, 139, 140, 141, 142, 142, 143, 144, 145, 
    146, 146, 147, 148, 149, 149, 150, 151, 152, 152, 153, 154, 155, 156, 156, 157, 158, 159, 159, 160, 161, 162, 162, 
    163, 164, 165, 165, 166, 167, 168, 168, 169, 170, 171, 171, 172, 173, 174, 174, 175, 176, 176, 177, 178, 179, 179, 
    180, 181, 182, 182, 183, 184, 184, 185, 186, 186, 187, 188, 189, 189, 190, 191, 191, 192, 193, 193, 194, 195, 195, 
    196, 197, 197, 198, 199, 199, 200, 201, 201, 202, 202, 203, 204, 204, 205, 206, 206, 207, 207, 208, 209, 209, 210, 
    211, 211, 212, 212, 213, 213, 214, 215, 215, 216, 216, 217, 217, 218, 219, 219, 220, 220, 221, 221, 222, 222, 223, 
    223, 224, 224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232, 232, 233, 233, 233, 234, 
    234, 235, 235, 236, 236, 236, 237, 237, 238, 238, 238, 239, 239, 240, 240, 240, 241, 241, 241, 242, 242, 243, 243, 
    243, 244, 244, 244, 245, 245, 245, 245, 246, 246, 246, 247, 247, 247, 247, 248, 248, 248, 249, 249, 249, 249, 250, 
    250, 250, 250, 250, 251, 251, 251, 251, 251, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 254, 254, 
    254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 253, 253, 253, 252, 252, 252, 252, 252, 251, 251, 
    251, 251, 251, 250, 250, 250, 250, 250, 249, 249, 249, 249, 248, 248, 248, 247, 247, 247, 247, 246, 246, 246, 245, 
    245, 245, 245, 244, 244, 244, 243, 243, 243, 242, 242, 241, 241, 241, 240, 240, 240, 239, 239, 238, 238, 238, 237, 
    237, 236, 236, 236, 235, 235, 234, 234, 233, 233, 233, 232, 232, 231, 231, 230, 230, 229, 229, 228, 228, 227, 227, 
    226, 226, 225, 225, 224, 224, 223, 223, 222, 222, 221, 221, 220, 220, 219, 219, 218, 217, 217, 216, 216, 215, 215, 
    214, 213, 213, 212, 212, 211, 211, 210, 209, 209, 208, 207, 207, 206, 206, 205, 204, 204, 203, 202, 202, 201, 201, 
    200, 199, 199, 198, 197, 197, 196, 195, 195, 194, 193, 193, 192, 191, 191, 190, 189, 189, 188, 187, 186, 186, 185, 
    184, 184, 183, 182, 182, 181, 180, 179, 179, 178, 177, 176, 176, 175, 174, 174, 173, 172, 171, 171, 170, 169, 168, 
    168, 167, 166, 165, 165, 164, 163, 162, 162, 161, 160, 159, 159, 158, 157, 156, 156, 155, 154, 153, 152, 152, 151, 
    150, 149, 149, 148, 147, 146, 146, 145, 144, 143, 142, 142, 141, 140, 139, 138, 138, 137, 136, 135, 135, 134, 133, 
    132, 131, 131, 130, 129, 128, 128, 128, 127, 126, 125, 125, 124, 123, 122, 121, 121, 120, 119, 118, 118, 117, 116, 
    115, 114, 114, 113, 112, 111, 110, 110, 109, 108, 107, 107, 106, 105, 104, 104, 103, 102, 101, 100, 100, 99, 98, 97, 
    97, 96, 95, 94, 94, 93, 92, 91, 91, 90, 89, 88, 88, 87, 86, 85, 85, 84, 83, 82, 82, 81, 80, 80, 79, 78, 77, 77, 76, 
    75, 74, 74, 73, 72, 72, 71, 70, 70, 69, 68, 67, 67, 66, 65, 65, 64, 63, 63, 62, 61, 61, 60, 59, 59, 58, 57, 57, 56,
    55, 55, 54, 54, 53, 52, 52, 51, 50, 50, 49, 49, 48, 47, 47, 46, 45, 45, 44, 44, 43, 43, 42, 41, 41, 40, 40, 39, 39, 
    38, 37, 37, 36, 36, 35, 35, 34, 34, 33, 33, 32, 32, 31, 31, 30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24, 
    23, 23, 23, 22, 22, 21, 21, 20, 20, 20, 19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 13, 13, 13, 12, 
    12, 12, 11, 11, 11, 11, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 3, 
    3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 
    5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 15, 
    15, 15, 16, 16, 16, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 22, 22, 23, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 
    28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 39, 39, 40, 40, 41, 41, 42, 43, 43, 
    44, 44, 45, 45, 46, 47, 47, 48, 49, 49, 50, 50, 51, 52, 52, 53, 54, 54, 55, 55, 56, 57, 57, 58, 59, 59, 60, 61, 61, 62, 
    63, 63, 64, 65, 65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 72, 73, 74, 74, 75, 76, 77, 77, 78, 79, 80, 80, 81, 82, 82, 83, 
    84, 85, 85, 86, 87, 88, 88, 89, 90, 91, 91, 92, 93, 94, 94, 95, 96, 97, 97, 98, 99, 100, 100, 101, 102, 103, 104, 104, 
    105, 106, 107, 107, 108, 109, 110, 110, 111, 112, 113, 114, 114, 115, 116, 117, 118, 118, 119, 120, 121, 121, 122, 123, 
    124, 125, 125, 126, 127, 128
};
    
uint8_t audio[AUDIO_BUFFER_SIZE] = {
    128, 144, 159, 175, 189, 203, 215, 226, 236, 243, 249, 253, 255, 255, 253, 249, 243, 236, 226, 215, 203, 189, 175, 159,
    144, 128, 112, 97, 81, 67, 53, 41, 30, 20, 13, 7, 3, 1, 1, 3, 7, 13, 20, 30, 41, 53, 67, 81, 97, 112
};

uint8_t dac_buffer[DAC_BUFFER_SIZE];
volatile uint32_t dac1_sample_cnt = 0;
uint32_t num_over_sample;

float fm = AUDIO_SAMPLE_RATE;
float fs = 1000000;
float fc = 100000;
float fd = 50000;

float k1, k2;

hw_timer_t *dac_timer = NULL;

void IRAM_ATTR ISR_Timer(){
   DAC1Write(dac_buffer[dac1_sample_cnt]);
   DAC2Write(audio[dac1_sample_cnt/num_over_sample]);
   if(dac1_sample_cnt == DAC_BUFFER_SIZE-1) dac1_sample_cnt=0;
   else dac1_sample_cnt++;
}

void computeFmParams(){
    k1 = fc/fs;
    k2 = (fd/fs)/AUDIO_PEAK_VALUE;
    num_over_sample = (uint32_t)fs/AUDIO_SAMPLE_RATE;
}

void bufferInit(){
    int32_t audio_sum = 0;
    for(int i=0; i<AUDIO_BUFFER_SIZE; i++){
        int16_t msg_sample = (int16_t)audio[i] - 128;
        uint32_t buffer_shift = num_over_sample*i;
        for(int j=0; j<num_over_sample; j++){
            float time_instant = k1*(buffer_shift+j) + k2*audio_sum;
            int lut_index = (int)SINE_LUT_SIZE*(time_instant-(int)time_instant);
            dac_buffer[buffer_shift+j] = sin_lut[lut_index];
            audio_sum += msg_sample;
        } 
    }
}

// Use Channel1 for FM and Channel2 for message
void dacInit(){
    setCpuFrequencyMhz(RTC_CPU_FREQ_240M);
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);
    DACPrepare(true);
    DACUnprepare(true);
}

// timer interrupts every 1 us to write sample from buffer to DAC => sample rate is 1MHz
void timerInit(){
    dac_timer = timerBegin(0, 2, true); 
    timerAttachInterrupt(dac_timer, &ISR_Timer, true);
    timerAlarmWrite(dac_timer, 39, true);
    timerAlarmEnable(dac_timer);
}

void setup(){
    computeFmParams();
    bufferInit();
    dacInit();
    timerInit();
}

void loop(){
    ;
}