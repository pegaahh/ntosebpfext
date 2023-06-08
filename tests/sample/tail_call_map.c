// Copyright (c) Microsoft Corporation
// SPDX-License-Identifier: MIT

// Whenever this sample program changes, bpf2c_tests will fail unless the
// expected files in tests\bpf2c_tests\expected are updated. The following
// script can be used to regenerate the expected files:
//     generate_expected_bpf2c_output.ps1
//
// Usage:
// .\scripts\generate_expected_bpf2c_output.ps1 <build_output_path>
// Example:
// .\scripts\generate_expected_bpf2c_output.ps1 .\x64\Debug\

#include "bpf_helpers.h"

struct
{
    __uint(type, BPF_MAP_TYPE_PROG_ARRAY);
    __type(key, uint32_t);
    __type(value, uint32_t);
    __uint(max_entries, 1);
} inner_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_ARRAY_OF_MAPS);
    __type(key, uint32_t);
    __type(value, uint32_t);
    __uint(max_entries, 1);
    __array(values, inner_map);
} outer_map SEC(".maps");

SEC("xdp_prog") int caller(struct xdp_md* ctx)
{
    uint32_t index = 0;
    struct bpf_map* inner_map = (struct bpf_map*)bpf_map_lookup_elem(&outer_map, &index);

    bpf_tail_call(ctx, inner_map, 0);

    // If we get to here it means bpf_tail_call failed.
    return 6;
}

SEC("xdp_prog/0") int callee(struct xdp_md* ctx) { return 42; }
