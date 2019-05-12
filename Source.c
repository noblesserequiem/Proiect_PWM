
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
int FOSC=16000000;// Clock Speed
int BAUD=9600;
int MYUBRR=FOSC/16/BAUD-1;
void USART_Init( unsigned int ubrr )
{
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
void setup()
{
  DDRD=0;
  adc_init();
  DDRD|=0x04;
  USART_Init(MYUBRR);
}
void USART_send_one_character(unsigned char data){
  /*wait for empty transmit buffer*/
  while( !( UCSR0A & (1<<UDRE0)) ) ; 
  //just wait /*put buffer into buffer, sends the data */
UDR0 = data;
}
float temp;
int t;


void loop()
{
  temp=read_adc(0x00);
  temp=(temp*5.0)/1023.0;
  temp=(temp-0.5)*100;
  if(temp>=35)
  {
    PORTD|=0x04;
  }
  else
  {
  PORTD&=~0x04;
  }
  t=(int)temp%10;
  USART_send_one_character(temp/10+0x30);
	USART_send_one_character(t+0x30);
  USART_send_one_character('\n');
}
