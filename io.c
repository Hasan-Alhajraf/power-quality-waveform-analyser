#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 512

int count_csv_rows(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return -1;
    }

    char line[LINE_BUFFER_SIZE];
    int rows = 0;

    fgets(line, LINE_BUFFER_SIZE, file);

    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL)
    {
        if (strlen(line) > 1)
        {
            rows++;
        }
    }

    fclose(file);
    return rows;
}

int load_csv(const char *filename, WaveformSample **samples, int *count)
{
    int rows = count_csv_rows(filename);

    if (rows <= 0)
    {
        return 0;
    }

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return 0;
    }

    *samples = malloc(rows * sizeof(WaveformSample));

    if (*samples == NULL)
    {
        fclose(file);
        return 0;
    }

    char line[LINE_BUFFER_SIZE];

    fgets(line, LINE_BUFFER_SIZE, file);

    int index = 0;

    while (fgets(line, LINE_BUFFER_SIZE, file) != NULL && index < rows)
    {
        WaveformSample *current = *samples + index;

        int fields_read = sscanf(
            line,
            "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            &current->timestamp,
            &current->phase_A_voltage,
            &current->phase_B_voltage,
            &current->phase_C_voltage,
            &current->line_current,
            &current->frequency,
            &current->power_factor,
            &current->thd_percent);

        if (fields_read == 8)
        {
            index++;
        }
    }

    fclose(file);

    *count = index;
    return index > 0;
}

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
    double max_thd)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        return 0;
    }

    fprintf(file, "Power Quality Waveform Analyser Results\n");
    fprintf(file, "======================================\n\n");

    fprintf(file, "Phase A\n");
    fprintf(file, "RMS Voltage: %.2f V\n", phase_a.rms);
    fprintf(file, "Peak-to-Peak Voltage: %.2f V\n", phase_a.peak_to_peak);
    fprintf(file, "DC Offset: %.6f V\n", phase_a.dc_offset);
    fprintf(file, "Clipped Samples: %d\n", phase_a.clipped_samples);
    fprintf(file, "Compliance: %s\n\n", phase_a.compliant ? "COMPLIANT" : "NON-COMPLIANT");

    fprintf(file, "Phase B\n");
    fprintf(file, "RMS Voltage: %.2f V\n", phase_b.rms);
    fprintf(file, "Peak-to-Peak Voltage: %.2f V\n", phase_b.peak_to_peak);
    fprintf(file, "DC Offset: %.6f V\n", phase_b.dc_offset);
    fprintf(file, "Clipped Samples: %d\n", phase_b.clipped_samples);
    fprintf(file, "Compliance: %s\n\n", phase_b.compliant ? "COMPLIANT" : "NON-COMPLIANT");

    fprintf(file, "Phase C\n");
    fprintf(file, "RMS Voltage: %.2f V\n", phase_c.rms);
    fprintf(file, "Peak-to-Peak Voltage: %.2f V\n", phase_c.peak_to_peak);
    fprintf(file, "DC Offset: %.6f V\n", phase_c.dc_offset);
    fprintf(file, "Clipped Samples: %d\n", phase_c.clipped_samples);
    fprintf(file, "Compliance: %s\n\n", phase_c.compliant ? "COMPLIANT" : "NON-COMPLIANT");

    fprintf(file, "Overall Results\n");
    fprintf(file, "Total Clipped Samples: %d\n", total_clipped);
    fprintf(file, "Frequency Range: %.3f Hz to %.3f Hz\n", min_frequency, max_frequency);
    fprintf(file, "Power Factor Range: %.3f to %.3f\n", min_power_factor, max_power_factor);
    fprintf(file, "THD Range: %.2f%% to %.2f%%\n", min_thd, max_thd);

    fclose(file);
    return 1;
}