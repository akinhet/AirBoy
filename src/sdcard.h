#pragma once

#include <esp_log.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/gpio.h"

#define MOUNT_POINT "/sdcard"
#define MAX_CARD_FILE_SIZE 4096

void setupSDCard(void);
void setdownSDCard(void);
void getCardInfo(void);