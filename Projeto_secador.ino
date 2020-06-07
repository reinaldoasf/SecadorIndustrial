#include <avr/io.h>
#include <avr/interrupt.h>

//uint16_t over = 0; //contador de overflow 
uint8_t estados = 0;//contador de overflow que conta a cada 0,5 minutos para mudança de estado da curva 
void adc_setup() {
  ADMUX |=0b01000000; //configura referencia vcc e habilita o adc0
  ADCSRA |=0b10000111; //enable habilitado e divide o clk por 128 para fins de economia energetica
}
ISR(TIMER_OVF_vect){
  over++;
  if(over>2184 && overcurva<4)
  {
    over=0;
    estados++;
  }
}
void configura_porta(){
  DDRB |= 0b01100000; // configura as portas 12 e 11 do PWM como saída (OC1B e OC1A respectivamente)
  DDRH |= 0b01000000; //configura a porta 9 do PWM como saída(OC2B)
  DDRC |= 0b00000001; //configura a porta digital 37 como saída(notifica se o led está ligado)
}
void contador_setup(){
  //setar o modo do pwm e alterar a frequencia
  TCCR1A |= 0b10001011; //utiliza os registradores no modo fast pwm e seta no modo clear on compare utilizando 2 registradores OCR1A e OCR1B
  TCCR1B |= 0b00001011; //modo fast pwm e escalonamento do clock clk/64(economia de energia)
}
int main()
{
  adc_setup(); //inicia o conversor
  configura_porta();//configura as entradas e saídas
  contador_setup();//configura o modo e o prescale do pwm
  sei();//ativa as interrupções
  
  while (1)
  {
    ADCSRA|=0b01000000;//inicia a conversão adc
    while(!(ADCSRA & 0b00010000));//a flag de interrupção indica a saída do loop após a conversão adc
    uint16_t valorADC = ADC;
    if (estados < 1&&over<2184){
      OCR1A = valorADC*0.4*over/1023;
    }
      
  }
}

