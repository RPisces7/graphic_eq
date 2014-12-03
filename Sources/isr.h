#ifndef ISR_H_
#define ISR_H_

extern volatile int adcConvComplete;

void ADC1_IRQHandler(void);
void PDB_Timer_IRQHandler(void);
void FTM0_ISR(void);

#endif /* ISR_H_ */
