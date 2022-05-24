#include "sdcard.h"

#define TAG "sdcard"

enum sdpins{
	SD_PIN_MISO	= 12,
	SD_PIN_MOSI	= 13,
	SD_PIN_SCLK	= 14,
	SD_PIN_CS	= 15,
};

sdmmc_card_t *card;
sdmmc_host_t host = SDSPI_HOST_DEFAULT();

void setupSDCard()
{
	esp_err_t err;
	gpio_set_pull_mode(SD_PIN_MISO, GPIO_PULLUP_ONLY);

    esp_vfs_fat_sdmmc_mount_config_t mount_config = 
	{

        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
		
    };

	const char mount_point[] = MOUNT_POINT;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_PIN_MOSI,
        .miso_io_num = SD_PIN_MISO,
        .sclk_io_num = SD_PIN_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = MAX_CARD_FILE_SIZE,
    };


	err = spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CH_AUTO);
	if (err != ESP_OK)
	{

#ifdef INPUT_LOGGING
		ESP_LOGE(TAG, "SPI bus for sdcard initialization error");
#endif

		return;
	}

	sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_PIN_CS;
    slot_config.host_id = host.slot;

	err = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);
	if (err != ESP_OK) 
	{

#ifdef INPUT_LOGGING
        if (err == ESP_FAIL) 
		{
            ESP_LOGE(TAG, "Failed to mount filesystem. SD card is propably not formated");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). ", esp_err_to_name(err));
        }
#endif

        return;
    }

#ifdef INPUT_LOGGING
	ESP_LOGI(TAG, "SD card is working correctly");
#endif

}

void setdownSDCard()
{
	const char mount_point[] = MOUNT_POINT;

	ESP_ERROR_CHECK(esp_vfs_fat_sdcard_unmount(mount_point, card));
	ESP_ERROR_CHECK(spi_bus_free(host.slot));
}

void getCardInfo()
{
	sdmmc_card_print_info(stdout, card);
}