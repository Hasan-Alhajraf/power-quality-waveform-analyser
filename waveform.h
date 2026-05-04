#ifndef WAVEFORM_H
#define WAVEFORM_H

#define NOMINAL_VOLTAGE 230.0
#define TOLERANCE_PERCENT 0.10
#define CLIPPING_THRESHOLD 324.9

typedef struct
{
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample;

typedef struct
{
    double rms;
    double peak_to_peak;
    double dc_offset;
    int clipped_samples;
    int compliant;
} PhaseMetrics;

double get_phase_voltage(const WaveformSample *sample, char phase);

double compute_rms(const WaveformSample *samples, int count, char phase);
double compute_peak_to_peak(const WaveformSample *samples, int count, char phase);
double compute_dc_offset(const WaveformSample *samples, int count, char phase);
int count_clipped_samples(const WaveformSample *samples, int count, char phase);
int check_voltage_compliance(double rms);

double find_min_frequency(const WaveformSample *samples, int count);
double find_max_frequency(const WaveformSample *samples, int count);
double find_min_power_factor(const WaveformSample *samples, int count);
double find_max_power_factor(const WaveformSample *samples, int count);
double find_min_thd(const WaveformSample *samples, int count);
double find_max_thd(const WaveformSample *samples, int count);

PhaseMetrics analyse_phase(const WaveformSample *samples, int count, char phase);

#endif