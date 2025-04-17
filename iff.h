/*
 * IFF SDK (https://mr-technologies.com/) is licensed under MRTech SK Standard License Agreement.
 *
 * Copyright (c) 2015-2025 MRTech SK, s.r.o.
 */

#ifndef _IFF_H
#define _IFF_H

#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif


#ifdef _IFF_SDK_LIB
    #ifdef _WIN32
        #define IFFAPI __declspec(dllexport)
    #else
        #define IFFAPI __attribute__((visibility ("default")))
    #endif
#else
    #ifdef _WIN32
        #define IFFAPI __declspec(dllimport)
    #else
        #define IFFAPI
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct iff_wb_params
{
    float r;
    float g1;
    float g2;
    float b;
    float r_off;
    float g1_off;
    float g2_off;
    float b_off;
} iff_wb_params;

typedef struct iff_image_metadata
{
    uint32_t width;
    uint32_t height;
    uint32_t offset_x;
    uint32_t offset_y;

    uint64_t src_ts;
    uint64_t ntp_ts;

    uint32_t exposure;
    uint32_t black_level;
    float    gain;

    iff_wb_params wb;

    unsigned char sequence_id;

    size_t padding;
} iff_image_metadata;

typedef enum iff_error_code
{
    camera_disconnected     = 0x4001,
    end_of_stream           = 0x4002,

    element_not_found       = 0x4003,
    element_not_ready       = 0x4004,

    image_queue_overflow    = 0x4005,
    invalid_input_data      = 0x4006,
    out_of_memory           = 0x4007,
    processing_failed       = 0x4008,

    parameter_not_found     = 0x4009,
    invalid_parameter_value = 0x400a,
    command_not_found       = 0x400b,

    camera_reset            = 0x400c
} iff_error_code;

#define IFF_LOG_LEVEL_DEBUG "DEBUG"
#define IFF_LOG_LEVEL_WARNING "WARNING"
#define IFF_LOG_LEVEL_ERROR "ERROR"
#define IFF_LOG_LEVEL_INFO "INFO"

typedef void* iff_chain_handle_t;

typedef void(*iff_callback_t)(const char* data, void* private_data);
typedef void(*iff_error_handler_t)(const char* element_id, int error_code, void* private_data);
typedef void(*iff_image_handler_t)(const void* data, size_t size, iff_image_metadata* metadata, void* private_data);

IFFAPI void iff_initialize(const char* config);
IFFAPI void iff_finalize();

IFFAPI void iff_log(const char* level, const char* title, const char* message);

IFFAPI iff_chain_handle_t iff_create_chain(const char* config, iff_error_handler_t error_handler, void* private_data);
IFFAPI void iff_release_chain(iff_chain_handle_t chain_handle);

IFFAPI void iff_execute(iff_chain_handle_t chain_handle, const char* command, iff_callback_t result_handler, void* private_data);
IFFAPI void iff_get_params(iff_chain_handle_t chain_handle, const char* params, iff_callback_t result_handler, void* private_data);
IFFAPI void iff_set_params(iff_chain_handle_t chain_handle, const char* params);

IFFAPI void iff_set_callback(iff_chain_handle_t chain_handle, const char* name, iff_callback_t callback, void* private_data);
IFFAPI void iff_set_export_callback(iff_chain_handle_t chain_handle, const char* exporter_id, iff_image_handler_t image_handler, void* private_data);

IFFAPI void* iff_get_import_buffer(iff_chain_handle_t chain_handle, const char* importer_id, size_t* size);
IFFAPI bool iff_push_import_buffer(iff_chain_handle_t chain_handle, const char* importer_id, void* buffer, iff_image_metadata metadata);

IFFAPI bool iff_release_buffer(iff_chain_handle_t chain_handle, const char* element_id, void* buffer);

#ifdef __cplusplus
}
#endif

#undef IFFAPI
#endif
