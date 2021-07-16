#pragma once


//onboard spi flash descriptor - taken from adafruit flash_devices.h and slightly modified


#define W25Q128JV_SM                                                        \
{                                                                            \
    .total_size = (1UL << 24), /* 16 MiB */                                    \
        .start_up_time_us = 5000, .manufacturer_id = 0xef,                     \
    .memory_type = 0x70, .capacity = 0x18, .max_clock_speed_mhz = 20,         \
    .quad_enable_bit_mask = 0x02, .has_sector_protection = false,              \
    .supports_fast_read = true, .supports_qspi = true,                         \
    .supports_qspi_writes = true, .write_status_register_split = false,        \
    .single_status_byte = false, .is_fram = false,                             \
  }

  
