

#define cs_mini12864 2   //片选 低电平片选
#define rst_mini12864 3 //物理重置.低电平有效
#define rs_mini12864 4   //寄存器选择 1:数据寄存器 0:指令寄存器
#define sck_mini12864 5 //时钟
#define sda_mini12864 6 //数据
//上面的脚可以自己修改
#define  Display  0xAF     //显示开启
#define  Power    0x2F           //电源全开
#define  VO       0x24     //对比度调节             
#define  AllPoint 0xA4           //非全屏显示
#define  ADCset   0xA1            //负向，131-4
#define  COMset   0xc0            //com0-com63
#define  ELECTVO  0x81     //亮度调节         调节颜色         不同的模块需要不同的  ELECTSET
#define  ELECTSET 0x2a           //亮度数值         调节颜色         不同的模块需要不同的        数值
#define  BIASSET  0xA2    //占空比1/9

unsigned char  ao[]={0x00,0x00,0xFC,0x04,0x54,0xE6,0x45,0xF4,0x44,0xE4,0x54,0x04,0xFC,0x00,0x00,0x00,
0x88,0x88,0x4B,0x48,0x29,0x28,0x18,0x0D,0x18,0x28,0x29,0x48,0x4B,0x88,0x88,0x00};/*"奥",0*/

unsigned char  song[]={0x10,0x10,0xD0,0xFF,0x90,0x10,0x80,0x60,0x1E,0x00,0xE0,0x07,0x18,0x60,0x80,0x00,
0x04,0x03,0x00,0xFF,0x00,0x03,0x20,0x70,0x2C,0x23,0x20,0x20,0x24,0x38,0x60,0x00};/*"松",1*/

unsigned char  ji[]={0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,
0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F,0x00,0x00,0x00,0x3F,0x40,0x40,0x78,0x00};/*"机",2*/

unsigned char  qi[]={0x80,0x80,0x9E,0x92,0x92,0x92,0x9E,0xE0,0x80,0x9E,0xB2,0xD2,0x92,0x9E,0x80,0x00,
0x08,0x08,0xF4,0x94,0x92,0x92,0xF1,0x00,0x01,0xF2,0x92,0x94,0x94,0xF8,0x08,0x00};/*"器",3*/

unsigned char  ren[]={0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x10,0x20,0x40,0x80,0x00};/*"人",4*/

void send_8bit_mini12864(unsigned char d)  //传8位数据,高位先传
{
        for(int i=0;i<8;i++)                    //循环8次,每次发送1个最高位
        {
                digitalWrite(sck_mini12864,0);       //时钟拉低,一位数据开始
                if(d&0x80)                          //与上10000000,最高位如果是1,就数据位就高
                        digitalWrite(sda_mini12864,1);
                else                                //与上10000000,最高位如果是0,就数据位就低
                        digitalWrite(sda_mini12864,0);
                digitalWrite(sck_mini12864,1);       //一位数据发送完毕,时钟拉高
                d<<=1;                              // d|ddddddd0    抛掉最高位,其余数据左移一位,最低位填0
        }
}

void write_cmd_mini12864(unsigned char cmd)  //写命令
{
        digitalWrite(cs_mini12864,0); //片选拉低,选中芯片
        digitalWrite(rs_mini12864,0); //rs为低,写入命令
        send_8bit_mini12864(cmd);     //写入8位命令
}
 
void write_dat_mini12864(unsigned char dat)  //写数据
{
        digitalWrite(cs_mini12864,0); //片选拉低,选中芯片
        digitalWrite(rs_mini12864,1); //rs为高,写入数据
        send_8bit_mini12864(dat);          //写入8位数据
}

void mini12864_cl()         //清屏,其实就是把所有的地址全部填成0
{         
  unsigned char x,y;
  for(y=0;y<8;y++)
  {    
    write_cmd_mini12864(0xb0+y);
    write_cmd_mini12864(0x10);                
    write_cmd_mini12864(0x00);
    for(x=0;x<132;x++)  write_dat_mini12864(0); 
  }        
}

void mini12864_Init()   //初始化
{
  pinMode(cs_mini12864,OUTPUT);   //设置各个I/O为输出
  pinMode(rst_mini12864,OUTPUT);  
  pinMode(rs_mini12864,OUTPUT);
  pinMode(sck_mini12864,OUTPUT);
  pinMode(sda_mini12864,OUTPUT);  
   
  digitalWrite(cs_mini12864,LOW);  
  digitalWrite(rst_mini12864,LOW);          //物理重置
  delay(200);
  digitalWrite(rst_mini12864,HIGH);
  delay(1000);        
  write_cmd_mini12864(0xe2);                //软件重置
  delay(200);
 
//以下为初始设置,具体请参考数据手册
 
  write_cmd_mini12864(0x24);//对比度
  write_cmd_mini12864(0x81);//SET ELECTRONIC VOLUME
  write_cmd_mini12864(0x20);//set pm: 通过改变这里的数值来改变电压 
  write_cmd_mini12864(0x2f);//set power control
  write_cmd_mini12864(0xa2);//BR=1/9
  write_cmd_mini12864(0xa0);//set seg direction
  write_cmd_mini12864(0xc8);//set com direction
  write_cmd_mini12864(0x40);//set scroll line
  
  //write_cmd(0xa6);//set inverse display           a6 off, a7 on
  //write_cmd(0xa4);//set all pixel on
  write_cmd_mini12864(0xaf);//开显示
  mini12864_cl();  //清屏
}

void mini12864_address(unsigned char page,unsigned char column)
{
      digitalWrite(cs_mini12864,0); //片选拉低,选中芯片  
      column=column-1;  
      page=page-1;
      write_cmd_mini12864(0xb0+page);
      write_cmd_mini12864(((column>>4)&0x0f)+0x10);
      write_cmd_mini12864(column&0x0f);
}

void full_dispaly()
{
    int i,j;
   for(i=0;i<8;i++)
  {
      digitalWrite(cs_mini12864,0); //片选拉低,选中芯片 
      mini12864_address(i+1,0);
      for(j=0;j<128;j++)
      {
        write_dat_mini12864(0xff); 
      }
  }
}

void disp_graphic_32X32(unsigned char page,unsigned char column,unsigned char *dp)
{
    unsigned char i,j;
    digitalWrite(cs_mini12864,0); //片选拉低,选中芯片 
    for(j=0;j<4;j++)
    {
         mini12864_address(page+1,column); 
       for(i=0;i<31;i++)
       {
             write_dat_mini12864(*dp);
            dp++;
       }  
    }
    digitalWrite(cs_mini12864,1); //片选拉低,选中芯片 
}

void display_graphic_16x16(unsigned char page,unsigned char column,unsigned char *dp)
{
      unsigned char i,j;
      digitalWrite(cs_mini12864,0); //片选拉低,选中芯片 
      for(j=0;j<2;j++)
      {
             mini12864_address(page+j,column);
            for (i=0;i<16;i++)
            {
                 write_dat_mini12864(*dp); /*写数据到LCD,每写完一个8 位的数据后列地址自动加1*/
                dp++;
            }
      }
      digitalWrite(cs_mini12864,1); //片选拉低,选中芯片 
}

void display_graphic_8x16(unsigned char page,unsigned char column,unsigned char *dp)
{
      unsigned char i,j;
       digitalWrite(cs_mini12864,0); //片选拉低,选中芯片
      for(j=0;j<2;j++)
      {
             mini12864_address(page+j,column);
            for (i=0;i<8;i++)
            {
                 write_dat_mini12864(*dp); /*写数据到LCD,每写完一个8 位的数据后列地址自动加1*/
                dp++;
            }
      }
      digitalWrite(cs_mini12864,1); //片选拉低,选中芯片
}

void setup()
{
        mini12864_Init();   //初始化液晶
}
 
void loop()
{  
full_dispaly();
delay(2000);
mini12864_cl();  //清屏
display_graphic_16x16(4,24,ao);
display_graphic_16x16(4,24+16,song);
display_graphic_16x16(4,24+32,ji);
display_graphic_16x16(4,24+48,qi);
display_graphic_16x16(4,24+64,ren);
delay(5000);
 
}
