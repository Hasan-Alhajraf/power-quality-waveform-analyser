#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "waveform.h"
#include "io.h"

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: %s <csv_filename>\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = "results.txt";

    WaveformSample *samples = NULL;
    int sample_count = 0;

    if (!load_csv(input_filename, &samples, &sample_count))
    {
        printf("Error: could not load CSV file '%s'.\n", input_filename);
        return 1;
    }

    PhaseMetrics phase_a = analyse_phase(samples, sample_count, 'A');
    PhaseMetrics phase_b = analyse_phase(samples, sample_count, 'B');
    PhaseMetrics phase_c = analyse_phase(samples, sample_count, 'C');

    int total_clipped = phase_a.clipped_samples;

    double min_frequency = find_min_frequency(samples, sample_count);
    double max_frequency = find_max_frequency(samples, sample_count);

    double min_power_factor = find_min_power_factor(samples, sample_count);
    double max_power_factor = find_max_power_factor(samples, sample_count);

    double min_thd = find_min_thd(samples, sample_count);
    double max_thd = find_max_thd(samples, sample_count);

    if (!write_results(
            output_filename,
            phase_a,
            phase_b,
            phase_c,
            total_clipped,
            min_frequency,
            max_frequency,
            min_power_factor,
            max_power_factor,
            min_thd,
            max_thd))
    {
        printf("Error: could not write results file.\n");
        free(samples);
        return 1;
    }

    printf("Analysis completed successfully.\n");
    printf("Samples loaded: %d\n", sample_count);
    printf("Results written to: %s\n", output_filename);

    free(samples);
    samples = NULL;

    return 0;
}