//timer testing for final project


#include <msp430.h> 


const unsigned int offDelay0 =1;                // off time in minutes for valve 1
const int onDelay0 = 10;                        // on time in seconds for valve 1
const unsigned int offDelay1 = 1;               // off time in minutes for valve 2
const int onDelay1 = 20;                        // on time in seconds for valve 2


int count0 = 0;                                 // initialize counts which will be used in loop
int count1 = 0;
int count2 = 0;
int count3 = 0;
int state0 = 0;                                 // initialize states which will be used in loop
int state1 = 0;

void gpioInit();
void timerInit();

int main(void)
{
    gpioInit();
    timerInit();

    __bis_SR_register(LPM3_bits | GIE);         // run clk
}


void gpioInit(){
    WDTCTL = WDTPW | WDTHOLD;               // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // turn on gpio

    P1OUT &= ~BIT0;                         // configure red LED as output
    P1DIR |= BIT0;
    P6OUT &= ~BIT6;                         // configure green LED as output
    P6DIR |= BIT6;

    P3OUT &= ~BIT7;                         // initialize PUMP (3.7)
    P3DIR |= BIT7;
    P2OUT &= ~BIT4;                         // initialize VALVE1 (2.4)
    P2DIR |= BIT4;
    P3OUT &= ~BIT2;                         // INITIALIZE VALVE2 (3.2)
    P3DIR |= BIT2;

}

void timerInit(){
    TB0CTL = ID_3 | TBSSEL_1 | MC_1;                     // timer B0 runs every 1 second
    TB0CCR0 = 4096;
    TB0CCTL0 |= CCIE;
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
    switch(state0)                                      // timer loop for valve 1
    {
        case 0:{                                        // counts up to the user set value for off time for valve 1
            count0++;
            if(count0 == ((offDelay0 * 60) - 1))        // once vale for off time is hit move to state 1
                state0 = 1;
            break;
        }

        case 1:{                                        // start counting up to user set value for on time for valve 1
            count1++;
            P1OUT |= BIT0;                              // turn on red LED (for testing)
            P3OUT |= BIT7;                              // turn on water pump and open valve 1
            P2OUT |= BIT4;
            if(count1 == onDelay0)                      // once value for on time is hit move to state 2
                state0 = 2;
            break;
        }

        case 2:{                                        // reset the counters and turn all outputs off
            count0 = 0;
            count1 = 0;
            P1OUT &= ~BIT0;
            P3OUT &= ~BIT7;
            P2OUT &= ~BIT4;
            state0 = 0;                                 // return back to state 0
            break;
        }
    }

    switch(state1)                                      // timer loop for valve 2
    {
        case 0:{                                        // counts up to the user set value for off time for valve 2
            count2++;
            if(count2 == ((offDelay1 * 60) - 1))        // once vale for off time is hit move to state 1
                state1 = 1;
            break;
        }

        case 1:{                                        // start counting up to user set value for on time for valve 2
            count3++;
            P6OUT |= BIT6;                              // turn on green LED (for testing)
            P3OUT |= BIT7;                              // turn on water pump and open valve 2
            P3OUT |= BIT2;
            if(count3 == onDelay1)                      // once value for on time is hit move to state 2
                state1 = 2;
            break;
        }

        case 2:{                                        // reset the counters and turn all outputs off
            count2 = 0;
            count3 = 0;
            P6OUT &= ~BIT6;
            P3OUT &= ~BIT7;
            P3OUT &= ~BIT2;
            state1 = 0;                                 // return back to state 0
            break;
        }
    }
}

