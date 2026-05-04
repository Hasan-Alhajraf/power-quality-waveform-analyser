# Power Quality Waveform Analyser (C)

## Overview
This program reads a CSV file containing 3-phase electrical waveform data and computes key power quality metrics. The analysis is based on standard AC waveform principles and EN 50160 voltage limits.

## Functionality
The program performs the following:
- Computes RMS voltage for each phase
- Computes peak-to-peak voltage
- Computes DC offset
- Detects clipped samples (|V| ≥ 324.9 V)
- Checks voltage compliance (±10% of 230 V)
- Reports frequency, power factor, and THD ranges

## Input
- CSV file: `power_quality_log.csv`
- Contains 1000 samples over ~200 ms (50 Hz waveform)

## Output
- Text file: `results.txt`
- Contains all computed metrics per phase and overall results

## Build and Run

### Compile (GCC)
```bash
gcc -std=c99 main.c waveform.c io.c -o analyser.exe -lm
```

### Run
```bash
analyser.exe power_quality_log.csv
```

## Structure
- main.c → program control and execution
- waveform.c / waveform.h → analysis functions
- io.c / io.h → file input/output

## Author
Hasan Alhajraf