/********************************************************************************
    * 文件名称 ：lcd.c
    * 作     者：小熊派
    * 版     本：V1.0
    * 编写日期 ：2018-4-1
    * 功     能：LCD屏幕驱动
*********************************************************************************
    * 说    明 ：本例程配套小熊派BearPi开发板使用
    *
    * 淘     宝：https://shop128001708.taobao.com/
    * 论     坛：bbs.iot-club.cn
*********************************************************************************/
#include "lcd.h"
#include "spi.h"
#include "lvgl.h"
#include "stdlib.h"
#include "stdio.h"
void LCD_Write_HalfWord(uint8_t * data,uint16_t size);
#define LCD_TOTAL_BUF_SIZE    (115200)
#define LCD_Buf_Size 1152
 uint8_t lcd_buf[LCD_Buf_Size];
 
volatile uint8_t dma_cmp_flag=0;
extern lv_disp_drv_t disp_drv;

uint16_t    POINT_COLOR = WHITE;    //画笔颜色    默认为黑色
uint16_t    BACK_COLOR = BLACK;    //背景颜色    默认为白色

extern DMA_HandleTypeDef hdma_spi2_tx;
extern SPI_HandleTypeDef hspi2;
/**
 * @brief    LCD底层SPI发送数据函数
 *
 * @param   data    数据的起始地址
 * @param   size    发送数据大小
 *
 * @return  void
 */

void LCD_SPI_Send(uint8_t *data, uint16_t size)
{
	SPI2_WriteByte(data, size);	
}
void LCD_SPI_Send_DMA(uint8_t *data, uint16_t size)
{
		HAL_SPI_Transmit_DMA(&hspi2,data,size);	
}


/**
 * @brief    写命令到LCD
 *
 * @param   cmd        需要发送的命令
 *
 * @return  void
 */
static void LCD_Write_Cmd(uint8_t cmd)
{
    LCD_DC(0);

    LCD_SPI_Send(&cmd, 1);
}

/**
 * @brief    写数据到LCD
 *
 * @param   cmd        需要发送的数据
 *
 * @return  void
 */
static void LCD_Write_Data(uint8_t data)
{
    LCD_DC(1);

    LCD_SPI_Send(&data, 1);
}

void lcd_flash()
{
	uint8_t i;
	LCD_DC(1);

  for (i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++) {
        LCD_SPI_Send(lcd_buf, LCD_Buf_Size);
    }
}
	

void LCD_Write_HalfWord_test(const uint16_t da)
{
	
		uint16_t i, j;
    uint8_t data[2] = { 0 };

    data[0] = da >> 8;
    data[1] = da;

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    for (j = 0; j < LCD_Buf_Size / 2; j++) {
        lcd_buf[j * 2] = data[0];
        lcd_buf[j * 2 + 1] = data[1];
    }
		lcd_flash();
}

/**
 * 设置数据写入LCD缓存区域
 *
 * @param   x1,y1    起点坐标
 * @param   x2,y2    终点坐标
 *
 * @return  void
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    LCD_Write_Cmd(0x2C);
}

/**
 * 打开LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOn(void)
{
    LCD_PWR(1);
}
/**
 * 关闭LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOff(void)
{
    LCD_PWR(0);
}

void LCD_Clear(uint16_t color)
{
	  uint16_t  j;
    uint8_t data[2] = { 0 };

    data[0] = color >> 8;
    data[1] = color;

    for (j = 0; j < LCD_Buf_Size / 2; j++) {
        lcd_buf[j * 2] = data[0];
        lcd_buf[j * 2 + 1] = data[1];
    }
		
		LCD_Address_Set(0, 0, LCD_Width, LCD_Height);
    LCD_DC(1);
		SPI2fast();
    for (int i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++) {
        LCD_SPI_Send_DMA(lcd_buf,LCD_Buf_Size);
    }
}


/**
 * @brief    写半个字的数据到LCD
 *
 * @param   cmd        需要发送的数据
 *
 * @return  void
 */
void LCD_Write_HalfWord(uint8_t * data,uint16_t size)
{
    LCD_DC(1);
    LCD_SPI_Send_DMA(data, size);
}



//spi发送完成回调函数
#if lcd == 0 
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{    
	//printf("dma spi cmp !!!\r\n");
		//dma_cmp_flag = 1;
		lv_disp_flush_ready(&disp_drv);
}
#endif

void lcd_color_fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, lv_color_t *color)
{
	
    uint16_t width = (x2 - x1)+1;
    uint16_t height = (y2 - y1)+1;
    uint16_t buf_size = (width * height*2);
    uint8_t *buf = (uint8_t *)malloc(buf_size);
		//uint8_t buf[buf_size];
    printf("-%d-%d-%d-%d-w=%d,,h=%d,buf_size= %d\n", x1, y1, x2, y2,width,height,buf_size);
    //HAL_Delay(1);
    if (buf == NULL)
    {
        //内存分配失败
        return;
    }	
    for (uint16_t i = 0; i < buf_size/2; i++)
    {

        buf[i * 2] = ((color->full) >> 8) & 0xFF;
        buf[i * 2 + 1] = (color->full) & 0xFF;
        color++;
    }
		//dma_cmp_flag = 0;
		
		SPI2fast();
		
		LCD_Address_Set(x1, y1, x2, y2);
    LCD_DC(1);
		
    //LCD_SPI_Send_DMA(buf, buf_size);
		LCD_SPI_Send(buf, buf_size);
    free(buf);
}

 void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
		//lcd_color_fill函数未使用
		//lcd_color_fill(area->x1,area->y1,area->x2,area->y2,color_p);
		uint32_t length = (area->x2 - area->x1 +1)*(area->y2 - area->y1 +1) * (sizeof(lv_color_t));
		printf("length = %d\r\n",length);
	  LCD_Address_Set(area->x1, area->y1, area->x2, area->y2);
	  LCD_DC(1);
		SPI2fast();
		HAL_SPI_Transmit_DMA(&hspi2,(uint8_t *)color_p,length);
		//color_p 数据量最大37.5kb   最小2.34kb
}



void ST7789V_SetDir(uint8_t Dir_Mode)
{
    LCD_Write_Cmd(0x36); /*显示方向*/
    if (Dir_Mode == 0)
        LCD_Write_Data(0x00);
    else if (Dir_Mode == 1)
        LCD_Write_Data(0xC0);
    else if (Dir_Mode == 2)
        LCD_Write_Data(0x70);
    else
        LCD_Write_Data(0xA0);
}


/**
 * @brief    LCD初始化
 *
 * @param   void
 *
 * @return  void
 */
void LCD_Init(void)
{
    //LCD_Gpio_Init();    //硬件接口初始化
    LCD_PWR(0);
    LCD_RST(0);
    HAL_Delay(12);
    LCD_RST(1);

    HAL_Delay(12);
    /* Sleep Out */
    LCD_Write_Cmd(0x11);
    /* wait for power stability */
    HAL_Delay(12);

    /* Memory Data Access Control */
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0x00);

    /* RGB 5-6-5-bit  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);

    /* Porch Setting */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);


    /*  Gate Control */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);

    /* VCOM Setting */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);   //Vcom=1.625V

    /* LCM Control */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);

    /* VDV and VRH Command Enable */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);

    /* VRH Set */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);

    /* VDV Set */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);

    /* Frame Rate Control in Normal Mode */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);    //60MHZ

    /* Power Control 1 */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);

    /* Positive Voltage Gamma Control */
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2B);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x54);
    LCD_Write_Data(0x4C);
    LCD_Write_Data(0x18);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0B);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x23);

    /* Negative Voltage Gamma Control */
    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2C);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x44);
    LCD_Write_Data(0x51);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x20);
    LCD_Write_Data(0x23);

    /* Display Inversion On */
    LCD_Write_Cmd(0x21);

    LCD_Write_Cmd(0x29);

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    //LCD_Clear(BLACK);

    /* Display on */
    LCD_PWR(1);
}


