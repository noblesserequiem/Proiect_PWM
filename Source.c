
void adc_init() 
{
ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
ADMUX |= (1<<REFS0);
ADCSRA |= (1<<ADSC); 
}
uint16_t read_adc(uint8_t channel) 
{
ADMUX &= 0xF0;
ADMUX |= channel;
ADCSRA |= (1<<ADSC);
while (ADCSRA & (1<<ADSC));
return ADCW; 
}
/*int FOSC=16000000;
int BAUD=9600;
int MYUBRR=FOSC/16/BAUD-1; */
void setup()
{
  DDRD=0;
  adc_init();
  DDRD|=0x5C;
  UBRR0=103;
  UCSR0B=0;
  UCSR0A=(1<<UDRE0);
  UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXC0);
  UCSR0C = (1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
  //---------------------------//
  TCCR1B=0;  //super imp val/rez
  TCCR1A=0; //
  TIMSK1=0; //
  TCCR1B|=(1<<WGM12);
  TIMSK1|=(1<<OCIE1A);
  sei();
  OCR1A=15624; //comp value
  TCCR1B|=(1<<CS10)|(1<<CS12);
  //-------------------------//
   TIMSK0=0;
   TCCR0A=0;
   TCCR0B=0;
   TIMSK0|=(1<<TOIE0);
   TCCR0A|=(1<<WGM01)|(1<<WGM00);
   TCCR0A|=(1<<COM0A1);
   TCCR0B|=(1<<CS01)|(1<<CS00);
   //-----------------------//
   TIMSK2=0;
   TCCR2A=0;
   TCCR2B=0;
   TIMSK2|=(1<<TOIE2);
   TCCR2B|=(1<<WGM21);
   TCCR2B|=(1<<CS22)|(1<<CS21)|(1<<CS20);
}

void USART_send_one_character(unsigned char data)
{
  while( !( UCSR0A & (1<<UDRE0)) ) ; 
  UDR0 = data;
}
unsigned char USART_receive(void)
{
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

ISR(USART_RX_vect)  //intreruperea la receive
{
 unsigned char litera;
  litera=USART_receive();
  if(litera=='A')
    PORTD|=0x08;
  if(litera=='S')
    PORTD&=~0x08;  
   
}
int counter,i;
ISR(TIMER1_COMPA_vect)
{
   PORTD ^= (1 << 4); 
   TCNT1=0;
   
}
ISR(TIMER2_OVF_vect)
{
  counter++;
  }
float temp;
int t1,t2,t,t3,p_state;
void loop()
{
  temp=(read_adc(0)/1024.0)*500;
  temp=temp-0.5;
  if(temp>=25)
  {
    PORTD|=0x04;
  }
  else
  {
  PORTD&=~0x04;
  }
  temp=temp*10;
  t=(int)temp;
  t1=t/100;
  t2=t/10%10;
  t3=t%10;
  USART_send_one_character(t1+0x30);
  USART_send_one_character(t2+0x30);
  USART_send_one_character('.');
  USART_send_one_character(t3+0x30);
  USART_send_one_character('\n');
  while(1&&counter!=0)
  {
      if(counter>100)
       {  p_state++;
          counter=0;
       }
       if(p_state>3)
        p_state=0;

       if(p_state==0)
       {
        OCR0A=counter;
        } 
        else
        if(p_state==1)
        OCR0A=200;
        else
        if(p_state==2)
        OCR0A=200-counter*2;
        else
        if(p_state==3)
        OCR0A=0;
       }

   }
