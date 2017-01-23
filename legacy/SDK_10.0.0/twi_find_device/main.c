/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup tw_sensor_example main.c
 * @{
 * @ingroup nrf_twi_example
 * @brief TWI Sensor Example main file.
 *
 * This file contains the source code for a sample application using TWI.
 *
 */

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_uart.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

/*UART buffer size. */
#define UART_TX_BUF_SIZE 256
#define UART_RX_BUF_SIZE 1

#warning "!**** ARE YOU ABSOLUTELY SURE YOU HAVE CHOSEN THE CORRECT SCL AND SDA PINS? ****!"
#define DEVICE_SCL_PIN 1
#define DEVICE_SDA_PIN 2

nrf_drv_twi_t twi_instance = NRF_DRV_TWI_INSTANCE(0);

uint8_t device_address = 0; // Address used to temporarily store the current address being checked
bool device_found = false; 


/**
 * @brief UART events handler.
 */
static void uart_events_handler(app_uart_evt_t * p_event)
{
    switch (p_event->evt_type)
    {
        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}


/**
 * @brief UART initialization.
 */
static void uart_config(void)
{
    uint32_t                     err_code;
    const app_uart_comm_params_t comm_params =
    {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud38400
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_events_handler,
                       APP_IRQ_PRIORITY_LOW,
                       err_code);

    APP_ERROR_CHECK(err_code);
}

/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{   
    switch(p_event->type)
    {
        case NRF_DRV_TWI_TX_DONE:
            // If TX_DONE (tranfer done) is received a device is found and responding on that particular address
            printf("\r\n!****************************!\r\nDevice found at 7-bit address: %#x!\r\n!****************************!\r\n\r\n", device_address);
            device_found = true;
            break;
        case NRF_DRV_TWI_ERROR:
            printf("No device responding on address: %#x!\r\n", device_address);
            break;
        default:
            break;        
    }   
}

/**
 * @brief UART initialization.
 */
void twi_init (void)
{
    ret_code_t err_code;
    
    const nrf_drv_twi_config_t twi_config = {
       .scl                = DEVICE_SCL_PIN,
       .sda                = DEVICE_SDA_PIN,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH
    };
    
    err_code = nrf_drv_twi_init(&twi_instance, &twi_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_twi_enable(&twi_instance);
}

/**
 * @brief Function for main application entry.
 */
int main(void)
{
    nrf_gpio_cfg_output(LED_1);
    nrf_gpio_pin_set(LED_1);
    uart_config();
    printf("\033[2J\033[;H\r\n* Example to find TWI devices connected to \r\n* the TWI bus and their addresses\r\n\r\n");
    twi_init();
    
    uint8_t dummy_data = 0x55;
    // Itterate through all possible 7-bit TWI addresses
    for(uint8_t i = 0; i <= 0x7F; i++)
    {
        device_address = i;
        // Send dummy data. If a device is present on this particular address a TWI EVT_DONE event is 
        // received in the twi event handler and a message is printed to UART
        nrf_drv_twi_tx(&twi_instance, i, &dummy_data, 1, false);
        // Delay 10 ms to allow TWI transfer to complete and UART to print messages before starting new transfer
        nrf_delay_ms(10);
    }
    if(device_found)
    {
        // Blinke LED_1 rapidly if device is found
        while(true)
        {
            nrf_gpio_pin_toggle(LED_1);
            nrf_delay_ms(100);
        }
    }
    else
    {
        // Disable LED_1 if device is NOT found
        nrf_gpio_cfg_default(LED_1);
        printf("*****************\n\rNO DEVICES FOUND!\r\n*****************\n\r");
        while(true)
        {
            ;
        }      
    }
}

/** @} */
