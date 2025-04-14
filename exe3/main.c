#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/i2c.h"

// Definições para o BMP280:
#define BMP280_I2C_ADDRESS 0x76   // Endereço padrão do BMP280 (pode ser 0x77 dependendo do pino SDO)
#define BMP280_REG_ID      0xD0   // Registrador que contém o ID do chip

const int I2C_SDA_GPIO = 20;
const int I2C_SCL_GPIO = 21;

void i2c_task(void *p) {
    // Inicializa o I2C a 400 kHz
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(I2C_SDA_GPIO, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_GPIO, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_GPIO);
    gpio_pull_up(I2C_SCL_GPIO);

    // Declaração do buffer que armazenará os dados lidos (apenas 1 byte neste caso)
    uint8_t buffer[1];

    // Define o registrador de ID do BMP280 para leitura
    uint8_t reg_address = BMP280_REG_ID;
    i2c_write_blocking(i2c_default, BMP280_I2C_ADDRESS, &reg_address, 1, true);  // Envia o endereço do registrador a ser lido
    i2c_read_blocking(i2c_default, BMP280_I2C_ADDRESS, buffer, 1, false);        // Lê 1 byte do sensor

    // Imprime na serial o ID lido do BMP280
    printf("BMP280 ID: 0x%X \n", buffer[0]);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

int main() {
    stdio_init_all();

    xTaskCreate(i2c_task, "i2c task", 4095, NULL, 1, NULL);
    vTaskStartScheduler();

    while (true) { }
}