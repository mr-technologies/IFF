/*
 * IFF SDK (https://mr-technologies.com/) is licensed under MRTech SK Standard License Agreement.
 *
 * Copyright (c) 2015-2023 MRTech SK, s.r.o.
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
    size_t   padding;

    uint32_t width;
    uint32_t height;
    uint32_t offset_x;
    uint32_t offset_y;

    uint64_t ts;
    uint64_t ntp_time;

    uint32_t black_level;
    unsigned int exposure;
    float gain;

    iff_wb_params wb;

    unsigned char sequence_id;
} iff_image_metadata;

#define IFF_LOG_LEVEL_INFO "INFO"
#define IFF_LOG_LEVEL_DEBUG "DEBUG"
#define IFF_LOG_LEVEL_ERROR "ERROR"
#define IFF_LOG_LEVEL_WARNING "WARNING"

typedef void* iff_chain_handle_t;

typedef void(*iff_error_handler_t)(const char* element_name, int error_code);
typedef void(*iff_result_handler_t)(const char* params);
typedef void(*iff_callback_t)(const char* callback_data);
typedef void(*iff_frame_export_function_t)(const void* data, size_t size, iff_image_metadata* metadata, void* private_data);

IFFAPI void iff_initialize(const char* config);
IFFAPI void iff_finalize();

IFFAPI void iff_log(const char* level, const char* message);

IFFAPI iff_chain_handle_t iff_create_chain(const char* chain_config, iff_error_handler_t on_error);
IFFAPI void iff_release_chain(iff_chain_handle_t chain_handle);

IFFAPI void iff_get_params(iff_chain_handle_t chain_handle, const char* params, iff_result_handler_t ret_func);
IFFAPI void iff_set_params(iff_chain_handle_t chain_handle, const char* params);
IFFAPI void iff_execute(iff_chain_handle_t chain_handle, const char* command);

IFFAPI void iff_set_callback(iff_chain_handle_t chain_handle, const char* name, iff_callback_t callback);
IFFAPI void iff_set_export_callback(iff_chain_handle_t chain_handle, const char* exporter_id, iff_frame_export_function_t export_func, void* private_data);

#ifdef __cplusplus
}
#endif

#undef IFFAPI
#endif
