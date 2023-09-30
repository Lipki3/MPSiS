#include msp430.h

 main.c

int fl=0;
int fint=0, fint2=0;
int count=0, count2=0,count3=0, level=1;
int delay=0;
int frequency=0;
int sost;
volatile int i=0;
unsigned int level=3;
int main(void) {
WDTCTL = WDTPW  WDTHOLD;  Stop watchdog timer
P1DIR = BIT0;
P1DIR = BIT1;
P1DIR = BIT2;
P1DIR = BIT3;
P1DIR = BIT5;
P8DIR = BIT2;
P7DIR = BIT7;
P7SEL = BIT7;
P8OUT &= ~BIT2;
P1OUT &= ~BIT1;
P1REN = BIT7;
P1OUT = BIT7;
P2REN = BIT2;
P2OUT = BIT2;
P1IE= BIT7;
P2IE= BIT2;
P1IFG &= ~BIT7;
P2IFG &= ~BIT2;
P1IES = BIT7;
P2IES = BIT2;
P8OUT &= ~BIT2;
P1OUT = BIT2;
P1OUT &= ~BIT2;
P1OUT = BIT0;
P1OUT = BIT1;
P1IFG=0;
__bis_SR_register(GIE);
TA1CTL = TASSEL_1 + MC_1 + ID_1 + TACLR;
TA1CCR0 = 1250;
TA1CCTL0 = CCIE;
UCSCTL3 = SELREF__REFOCLK; Выбор источника для FLL 010b – REFOCLK
UCSCTL4 = SELM__DCOCLK; Выбор источника для MCLK DCOCLK
20
UCSCTL1 = DCORSEL_1; источник частоты
UCSCTL2 = FLLD__4; делитель частоты D
UCSCTL2 = FLLN6;  N множитель DCO
 UCSCTL3 =FLLREFDIV__2; делитель частоты n
 UCSCTL3 = DIVM__4;  DIVM__32
__no_operation();
return 0;
}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
{
P1OUT ^= BIT5; если выждали задержку зажигаем светодиод
count++;
if(count=count2+5){
fint=0;
TA1CTL = TACLR;
TA1CCTL0 &=~ CCIE;
}
if(count=count3+5){
fint2=0;
TA1CTL = TACLR;
TA1CCTL0 &=~ CCIE;
}
}
}
#pragma vector=PORT1_VECTOR
__interrupt void P1INT()
{
if(fint==0){
count2=count;
fint=1;
if(fl==0)
{P1OUT &= ~BIT0;
P8OUT = BIT2;
fl=1;
_BIS_SR(LPM2_bits+GIE); перейти в режим энергосбережения LPM2
}
else if(fl==1){
P1OUT = BIT0;
P8OUT &= ~BIT2;
fl=0;
_BIC_SR_IRQ(LPM2_bits); выйти из режима энергосбережения LPM2
}
}
P1IFG=0;
}
#pragma vector=PORT2_VECTOR
__interrupt void P2INT()  Обработчик прерывания уменьшает делитель частоты
{
if(fint2==0){ проверка задержки для предотвращения дребезга
count3=count;
fint2=1;
if(frequency==0)
21
{P1OUT &= ~BIT1;
P1OUT = BIT2; включить светодтод сигнализирующий о высоком уровне напряжения
UCSCTL4=0;
level=2;
PMMCTL0_H = PMMPW_H;  открыть PMM регистры для доступа
SVSMHCTL = SVSHE  SVSHRVL0  level  SVMHE  SVSMHRRL0  level;  управление 
SVMSVS на входе
SVSMLCTL = SVSLE  SVMLE  SVSMLRRL0  level;  установка SVM в новый уровень
while (!(PMMIFG & SVSMLDLYIFG));  ожидание пока установится SVM
PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);  очистка ранее установленных флагов
PMMCTL0_L = PMMCOREV0  level;  установка Vcore в новый уровень
if ((PMMIFG & SVMLIFG))  ожидание пока будет достигнут новый уровень
while (!(PMMIFG & SVMLVLRIFG));
SVSMLCTL = SVSLE  SVSLRVL0  level  SVMLE  SVSMLRRL0  level;  управление 
SVMSVS на выходе
PMMCTL0_H = 0;  закрыть PMM регистры для доступа
level=3;
PMMCTL0_H = PMMPW_H;  открыть PMM регистры для доступа
SVSMHCTL = SVSHE  SVSHRVL0  level  SVMHE  SVSMHRRL0  level;  управление 
SVMSVS на входе
SVSMLCTL = SVSLE  SVMLE  SVSMLRRL0  level;  установка SVM в новый уровень
while (!(PMMIFG & SVSMLDLYIFG));  ожидание пока установится SVM
PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);  очистка ранее установленных флагов
PMMCTL0_L = PMMCOREV0  level;  установка Vcore в новый уровень
if ((PMMIFG & SVMLIFG))  ожидание пока будет достигнут новый уровень
while (!(PMMIFG & SVMLVLRIFG));
SVSMLCTL = SVSLE  SVSLRVL0  level  SVMLE  SVSMLRRL0  level;  управление 
SVMSVS на выходе
PMMCTL0_H = 0;  закрыть PMM регистры для доступа
UCSCTL4 &= ~SELS__DCOCLK; убрать источник DCO
UCSCTL4 = SELS__REFOCLK; установить источник REFO
frequency=1;
}
else if(frequency==1){
PMMCOREV1
P1OUT = BIT1;
P1OUT &= ~BIT2;
UCSCTL4=0;
level=2;
PMMCTL0_H = PMMPW_H;  открыть PMM регистры для доступа
SVSMLCTL = SVSLE  SVSLRVL0  level  SVMLE  SVSMLRRL0  level;  управление
SVMSVS на входе
while (!(PMMIFG & SVSMLDLYIFG));  ожидание пока установится SVM
PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);  очистка ранее установленных флагов
PMMCTL0_L = PMMCOREV0  level;  установка Vcore в новый уровень
PMMCTL0_H = 0;  открыть PMM регистры для доступа
22
level=1;
PMMCTL0_H = PMMPW_H;  открыть PMM регистры для доступа
SVSMLCTL = SVSLE  SVSLRVL0  level  SVMLE  SVSMLRRL0  level;  управление 
SVMSVS на входе
while (!(PMMIFG & SVSMLDLYIFG));  ожидание пока установится SVM
PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);  очистка ранее установленных флагов
PMMCTL0_L = PMMCOREV0  level;  установка Vcore в новый уровень
PMMCTL0_H = 0;  открыть PMM регистры для доступа
UCSCTL4 &= ~SELS__REFOCLK;
UCSCTL4 = SELS__DCOCLK;
UCSCTL1 = DCORSEL_1; источник частоты
UCSCTL2 = FLLD__16; делитель частоты D
UCSCTL2 = FLLN6;  N множитель DCO
UCSCTL5 = DIVS__8;
frequency=0;
}
}
P2IFG=0;
}