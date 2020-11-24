sbit LCD_RS at RD4_bit;
sbit LCD_EN at RD5_bit;
sbit LCD_D7 at RD3_bit;
sbit LCD_D6 at RD2_bit;
sbit LCD_D5 at RD1_bit;
sbit LCD_D4 at RD0_bit;

// Pin direction
sbit LCD_RS_Direction at TRISD4_bit;
sbit LCD_EN_Direction at TRISD5_bit;
sbit LCD_D7_Direction at TRISD3_bit;
sbit LCD_D6_Direction at TRISD2_bit;
sbit LCD_D5_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISD0_bit;

char txtBtn[7];
char txtV[7],napon[7]="Napon:",dugme[7]="Br.pr:";
int i, tmrOldState=0;
unsigned int temp;
float voltage;
void interrupt()
{
    if(ADIF_bit)
    {
        temp=ADRESH*256+ADRESL;
        ADIF_bit=0;
    }

}
void convert(float volti, char *string)
{
     int pom = volti*1000;
     string[0]=pom/1000 + 48;
     string[1]='.';
     string[2]=(pom/100)%10 + 48;
     string[3]=(pom/10)%10 + 48;
     string[4]=pom%10 + 48;
     string[5]=' ';      //za razmak i bolji ispis na VT.
     string[6]=' ';
     string[7]='\0';
}

void ispisi(char *text)
{
    for(i=0;text[i]!='\0';)
    {
         if(TXSTA.TRMT==1)
         {
              TXREG=text[i];
              i++;
         }
    }
    TXREG=' ';
}

void main() {
     ANSEL=0b00100000;
     ANSELH=0;
     TRISA.b4=1;
     TRISE.B0=1;

     OPTION_REG.B4=1; //silazna ivica
     OPTION_REG.B5=1;
     TMR0=0;

     ADCON0=0b00010101; //an port, uklj A/D
     ADCON1=0xA0;  //desno poravnanje, VDD i Vref-(uzemljen) kao ref naponi
     GIE_bit=1;       //globalni inter
     PEIE_bit=1;      //periferalni
     ADIE_bit=1;      //A/D inter
     ADIF_bit=0;      //A/D zastavica
     
     TXSTA.TXEN=1;
     TXSTA.SYNC=0;
     RCSTA.SPEN=1;

     TXSTA.BRGH=1;
     BAUDCTL.BRG16=0;
     SPBRG = 25;



     Lcd_Init();
     Lcd_Cmd(_LCD_CURSOR_OFF);
     Lcd_Out(1,1,"Klikni na taster");

     while(1)
     {
             if(TMR0!=tmrOldState)
             {
                 Lcd_Cmd(_LCD_CLEAR);
                 Lcd_Out(1,1,dugme);
                 Lcd_Out(2,1,napon);
                 IntToStr(TMR0,txtBtn);
                 Lcd_Out(1,7,txtBtn);
                 strcpy(txtV, "");     //ocisti string
                 ispisi(dugme);
                 ispisi(txtBtn);
                 tmrOldState=TMR0;
                 if(TMR0%2==0 && TMR0!=0)
                 {
                      voltage=(temp/1024.0)*5.0;
                      convert(voltage,txtV);
                      ispisi(napon);
                      ispisi(txtV);
                      Lcd_Out(2,7,txtV);
                 }
             }
             ADCON0.B1=1;
    }
}