


20200904
1、电流传感器那部分代码，记得添加限流保护机制，确保电路安全



TIM8和TIM3都用作PWM输出了，不能做其他定时；
TIM1用作系统任务定时了
TIM2和TIM4做编码开关计数，也不能做其他定时了


只用TIM8输出互补PWM，单独控制其使能位
TIM3、TIM6、TIM7用作SPWM定时器











