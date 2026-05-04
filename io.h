#ifndef IO_H
#define IO_H

#include "waveform.h"

int count_csv_rows(const char *filename);
int load_csv(const char *filename, WaveformSample **samples, int *count);

int write_results(
    const char *filename,
    PhaseMetrics phase_a,
    PhaseMetrics phase_b,
    PhaseMetrics phase_c,
    int total_clipped,
    double min_frequency,
    double max_frequency,
    double min_power_factor,
    double max_power_factor,
    double min_thd,
    double max_thd);

#endif