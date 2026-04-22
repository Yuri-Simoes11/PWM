#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <pwm_z42.h>

#define PORTB_NODE DT_NODELABEL(gpiob)
#define TPM_MODULE 1000

int main(void){
    const struct device *portb = DEVICE_DT_GET(PORTB_NODE);
    const struct device *porta = DEVICE_DT_GET(DT_NODELABEL(gpioa));
    const struct device *portd = DEVICE_DT_GET(DT_NODELABEL(gpiod));
    if (!device_is_ready(portb)) {
        return 0;
    }


    pwm_tpm_Init(TPM1, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM1, 1, TPM_PWM_H, GPIOB, 1);  // ENB (PTB1)
    pwm_tpm_Ch_Init(TPM1, 0, TPM_PWM_H, GPIOE, 20); //ENA (PTE20)

    gpio_pin_configure(portb, 2, GPIO_OUTPUT); // IN1 (PTB2)
    gpio_pin_configure(portb, 3, GPIO_OUTPUT); // IN2 (PTB3)
    gpio_pin_configure(porta, 12, GPIO_OUTPUT);// IN3 (PTA12)
    gpio_pin_configure(portd, 4, GPIO_OUTPUT); // IN4 (PTD4)

    int vel1=300;
    int vel2=800;

    while (1){        
        gpio_pin_set(porta, 12, 0);
        gpio_pin_set(portd, 4, 1); 
        pwm_tpm_CnV(TPM1, 0, vel1);

        gpio_pin_set(portb, 2, 0);
        gpio_pin_set(portb, 3, 1); 
        pwm_tpm_CnV(TPM1, 1, vel2);
        k_msleep(10);
     
    }
    return 0;
}