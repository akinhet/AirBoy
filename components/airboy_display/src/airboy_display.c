#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_check.h"
#include "ili9341.h"
#include "airboy_display.h"
#include "airboy_display_framebuffer.h"

frame_buffer_t frame_buffer;
static esp_lcd_panel_handle_t panel_handle = NULL;

#define TASKNAME "display_init"
#define LCD_HOST SPI2_HOST
#define LCD_CMD_BITS    8
#define LCD_PARAM_BITS  8

bool lcd_trans_done_cb(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    BaseType_t woken = false;
    frame_buffer.write_enable = true;
    return woken;
}

typedef enum {
	LCD_PIN_DATA_0  =   4, // Data 0-7 pins
	LCD_PIN_DATA_1  =  40,
	LCD_PIN_DATA_2  =   5,
	LCD_PIN_DATA_3  =  41,
	LCD_PIN_DATA_4  =   6,
	LCD_PIN_DATA_5  =  42,
	LCD_PIN_DATA_6  =   7,
	LCD_PIN_DATA_7  =   2,
	LCD_PIN_CLK	    =  16, // WRX pin
	LCD_PIN_CS		=  17, // CS pin
	LCD_PIN_DC		=  38, // DC pin
	LCD_PIN_RESET	=  15, // RST pin
} lcd_pins_t;

static void init_spi_bus(esp_lcd_panel_io_handle_t *io_handle)
{
	// Spi bus configuration
	spi_bus_config_t buscfg = {
        .sclk_io_num  = LCD_PIN_CLK,
        .mosi_io_num  = LCD_PIN_DATA_0,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .data1_io_num = LCD_PIN_DATA_1,
        .data2_io_num = LCD_PIN_DATA_2,
        .data3_io_num = LCD_PIN_DATA_3,
        .data4_io_num = LCD_PIN_DATA_4,
        .data5_io_num = LCD_PIN_DATA_5,
        .data6_io_num = LCD_PIN_DATA_6,
        .data7_io_num = LCD_PIN_DATA_7,
		.max_transfer_sz = frame_buffer.height * frame_buffer.width * sizeof(uint16_t),
        .flags = SPICOMMON_BUSFLAG_OCTAL
    };

	ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
	
	// spi bus to lcd connection
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num       = LCD_PIN_DC,
        .cs_gpio_num       = LCD_PIN_CS,
        .pclk_hz           = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits      = LCD_CMD_BITS,
        .lcd_param_bits    = LCD_PARAM_BITS,
        .spi_mode          = 3, // fastest mode
        .trans_queue_depth = 20,
        .flags.octal_mode  = 1,
        .on_color_trans_done = lcd_trans_done_cb,
    };

	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, io_handle));
}

static void init_i80_bus(esp_lcd_panel_io_handle_t *io_handle)
{
	esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t bus_config = {
        .dc_gpio_num = 38,
        .wr_gpio_num = 16,
        .data_gpio_nums = {
            4, 40, 5, 41, 6, 42, 7, 2,
            -1, -1, -1, -1, -1, -1, -1, -1
        },
        .bus_width = 8,
        .max_transfer_bytes = frame_buffer.height * frame_buffer.width * sizeof(uint16_t),
    };

    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));

    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = 17,
        .pclk_hz = 8000000, //8MHz
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level = 1,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .on_color_trans_done = lcd_trans_done_cb,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, io_handle));
}

void init_display(frame_buffer_config_t *buffer_config)
{
    //first initialize framebuffer
    ESP_ERROR_CHECK(init_frame_buffer(buffer_config, &frame_buffer));
	esp_lcd_panel_io_handle_t io_handle = NULL;

	// initialize selected lcd bus type
	#if DISPLAY_BUS_TYPE 
	    init_spi_bus(&io_handle);
	#else
	    init_i80_bus(&io_handle);
	#endif

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_PIN_RESET,
        .color_space    = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = sizeof(uint16_t) * 8,
    };

	ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
	ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
    //ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
}

void draw_frame()
{
        #if DISPLAY_BUS_TYPE 
        // due to esp32 s3 spi bus buffer size the screen painting has to be split into smaller fragments (less than 32kb)     
            for (uint8_t y = 0; y < frame_buffer.height; y += 48)
            {
                esp_lcd_panel_draw_bitmap(panel_handle, 0, y, frame_buffer.width, y + 48, &(frame_buffer.buffer[frame_buffer.current_buffer][y * frame_buffer.width]));
                frame_buffer.write_enable = false;
            }    
                
        #else
            esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, frame_buffer.width, frame_buffer.height, &(frame_buffer.buffer[frame_buffer.current_buffer][0]));
            frame_buffer.write_enable = false;
        #endif

        if (frame_buffer.buffer_count > 1)
        {
            if (frame_buffer.current_buffer == (frame_buffer.buffer_count - 1)) frame_buffer.current_buffer = 0;
            else frame_buffer.current_buffer++;
        }else{
            while(!frame_buffer.write_enable) {}
        }
}

void clear_buffer(uint16_t color)
{
    memset(frame_buffer.buffer[frame_buffer.current_buffer], color, frame_buffer.height * frame_buffer.width * sizeof(uint16_t));
}

void set_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    frame_buffer.buffer[frame_buffer.current_buffer][frame_buffer.width * y + x] = color;
}