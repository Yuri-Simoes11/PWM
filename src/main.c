#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)

// Define o valor do registrador MOD do TPM para configurar o período do PWM
#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
// uint16_t duty_50  = TPM_MODULE/2;       // 50% de duty cycle (meio brilho)

int main(void)
{
    // Inicializa o módulo TPM2 com:
    // - base do TPMx
    // - fonte de clock PLL/FLL (TPM_CLK)
    // - valor do registrador MOD
    // - tipo de clock (TPM_CLK)
    // - prescaler de 1 a 128 (PS)
    // - modo de operação EDGE_PWM
    pwm_tpm_Init(TPM0, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);

    // Inicializa o canal 0 do TPM2 para gerar sinal PWM na porta GPIOB_18
    // - modo TPM_PWM_H (nível alto durante o pulso)
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 18);
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 19);
    pwm_tpm_Ch_Init(TPM0, 1, TPM_PWM_H, GPIOD, 1);

    // Define o valor do duty cycle: nesse caso, duty_100 (LED quase desligado)
    pwm_tpm_CnV(TPM2, 0, 0); // Vermelho
    pwm_tpm_CnV(TPM2, 1, 300); // Verde
    pwm_tpm_CnV(TPM0, 1, 1000); // Azul

    // Loop infinito
    for (;;){
      pwm_tpm_CnV(TPM2, 0, 0); // Vermelho
      pwm_tpm_CnV(TPM2, 1, 300); // Verde
      pwm_tpm_CnV(TPM0, 1, 10000); // Azul
      k_msleep(1000);
      pwm_tpm_CnV(TPM2, 0, 10000); // Vermelho
      pwm_tpm_CnV(TPM2, 1, 10000); // Verde
      pwm_tpm_CnV(TPM0, 1, 10000); // Azul
      k_msleep(1000);
      // O programa poderia alterar o duty cycle dinamicamente aqui se desejado
    }

    return 0;
}