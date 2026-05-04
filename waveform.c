#include "waveform.h"
#include <math.h>

double get_phase_voltage(const WaveformSample *sample, char phase)
{
    if (phase == 'A')
        return sample->phase_A_voltage;
    if (phase == 'B')
        return sample->phase_B_voltage;
    if (phase == 'C')
        return sample->phase_C_voltage;
    return 0.0;
}

double compute_rms(const WaveformSample *samples, int count, char phase)
{
    double sum_square = 0.0;

    for (int i = 0; i < count; i++)
    {
        double voltage = get_phase_voltage(samples + i, phase);
        sum_square += voltage * voltage;
    }

    return sqrt(sum_square / count);
}

double compute_peak_to_peak(const WaveformSample *samples, int count, char phase)
{
    double min_value = get_phase_voltage(samples, phase);
    double max_value = get_phase_voltage(samples, phase);

    for (int i = 1; i < count; i++)
    {
        double voltage = get_phase_voltage(samples + i, phase);

        if (voltage < min_value)
            min_value = voltage;
        if (voltage > max_value)
            max_value = voltage;
    }

    return max_value - min_value;
}

double compute_dc_offset(const WaveformSample *samples, int count, char phase)
{
    double sum = 0.0;

    for (int i = 0; i < count; i++)
    {
        sum += get_phase_voltage(samples + i, phase);
    }

    return sum / count;
}

int count_clipped_samples(const WaveformSample *samples, int count, char phase)
{
    int clipped_count = 0;

    for (int i = 0; i < count; i++)
    {
        double voltage = fabs(get_phase_voltage(samples + i, phase));

        if (voltage >= CLIPPING_THRESHOLD)
        {
            clipped_count++;
        }
    }

    return clipped_count;
}

int check_voltage_compliance(double rms)
{
    double lower_limit = NOMINAL_VOLTAGE * (1.0 - TOLERANCE_PERCENT);
    double upper_limit = NOMINAL_VOLTAGE * (1.0 + TOLERANCE_PERCENT);

    return rms >= lower_limit && rms <= upper_limit;
}

double find_min_frequency(const WaveformSample *samples, int count)
{
    double min_value = samples->frequency;

    for (int i = 1; i < count; i++)
    {
        if ((samples + i)->frequency < min_value)
        {
            min_value = (samples + i)->frequency;
        }
    }

    return min_value;
}

double find_max_frequency(const WaveformSample *samples, int count)
{
    double max_value = samples->frequency;

    for (int i = 1; i < count; i++)
    {
        if ((samples + i)->frequency > max_value)
        {
            max_value = (samples + i)->frequency;
        }
    }

    return max_value;
}

double find_min_power_factor(const WaveformSample *samples, int count)
{
    double min_value = samples->power_factor;

    for (int i = 1; i < count; i++)
    {
        if ((samples + i)->power_factor < min_value)
        {
            min_value = (samples + i)->power_factor;
        }
    }

    return min_value;
}

double find_max_power_factor(const WaveformSample *samples, int count)
{
    double max_value = samples->power_factor;

    for (int i = 1; i < count; i++)
    {
        if ((samples + i)->power_factor > max_value)
        {
            max_value = (samples + i)->power_factor;
        }
    }

    return max_value;
}

double find_min_thd(const WaveformSample *samples, int count)
{
    double min_value = samples->thd_percent;

    for (int i = 1; i < count; i++)
    {
        if ((samples + i)->thd_percent < min_value)
        {
            min_value = (samples + i)->thd_percent;
        }
    }

    return min_value;
}

double find_max_thd(const WaveformSample *samples, int count)
{
    double max_value = samples->thd_percent;

    for (int i = 1; i < count; i++)
    {
        if ((samples + i)->thd_percent > max_value)
        {
            max_value = (samples + i)->thd_percent;
        }
    }

    return max_value;
}

PhaseMetrics analyse_phase(const WaveformSample *samples, int count, char phase)
{
    PhaseMetrics metrics;

    metrics.rms = compute_rms(samples, count, phase);
    metrics.peak_to_peak = compute_peak_to_peak(samples, count, phase);
    metrics.dc_offset = compute_dc_offset(samples, count, phase);
    metrics.clipped_samples = count_clipped_samples(samples, count, phase);
    metrics.compliant = check_voltage_compliance(metrics.rms);

    return metrics;
}