#include "stm32f1xx.h"
#include <stdint.h>

extern int32_t rust_main();
int32_t i;

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    //RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    GPIOC->CRH=0x33333333;
    //GPIOB->CRL=0x33333333;

	i = rust_main();
	
    while(1)
    {
        volatile int i = 0;

        for(i=0;i<0x4000;i++){
        }

        GPIOC->ODR ^= (1 << 13);
        //GPIOB->ODR ^= (1 << 1);
    }
}
