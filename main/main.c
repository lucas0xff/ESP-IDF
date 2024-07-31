// Código de teste e consulta de um gerador PWM com LEDC - Led Controller
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include <driver/gpio.h>
#include <unistd.h>
#define LEDV GPIO_NUM_18
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (5) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4096) // Está definindo o duty cycle (largura do pulso), como metade da capacidade (2^13 = 4096)
#define LEDC_FREQUENCY          (4) // COntrola a frequencia do sinal
/* For ESP32, ESP32S2, ESP32S3, ESP32C3, ESP32C2, ESP32C6, ESP32H2, ESP32P4 targets,
 * when LEDC_DUTY_RES selects the maximum duty resolution (i.e. value equal to SOC_LEDC_TIMER_BIT_WIDTH),
 * 100% duty cycle is not reachable (duty cannot be set to (2 ** SOC_LEDC_TIMER_BIT_WIDTH)).
 */
static void ledc_config(void){
    //Configuração do LEDC PWM
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE, //desativa as interrupções
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // inicia o duty cycle em 0 
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void app_main(void){
    // Set the LEDC peripheral configuration
    ledc_config();
    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
    
    gpio_set_direction(LEDV, GPIO_MODE_OUTPUT);
    
    while(true){
		for(int i = 0; i<15;i++){
		gpio_set_level(LEDV, 1);
        usleep(100000);

        gpio_set_level(LEDV, 0);
        usleep(100000);
		}	
	}
    
    
}