# Power Consumption Analysis of Tetouan City

## Description of the Dataset

**Dataset Chosen**: Power Consumption of Tetouan City  
**Available at**: [UCI Machine Learning Repository - Power Consumption Dataset](https://archive.ics.uci.edu/ml/datasets/Power+Consumption+of+Tetouan+City)  

**Attributes**:
- **Date**: Timestamp of the recording, with data collected every ten minutes for January 2017.
- **Features**:
  - **Temperature**: Ambient temperature in Tetouan City.
  - **Humidity**: Humidity level in the city.
  - **Wind Speed**: Wind speed measurements.
  - **General Diffuse Flow**: General diffuse solar radiation.
  - **Diffuse Flow**: Specific diffuse solar radiation.
- **Targets**:
  - **Zone 1 Power Consumption**: Power consumption in zone 1 of the city.
  - **Zone 2 Power Consumption**: Power consumption in zone 2 of the city.
  - **Zone 3 Power Consumption**: Power consumption in zone 3 of the city.

## Initial Plan for Data Exploration

- **Quantile Data and Summary Statistics**: Collect and review these for understanding the distribution of data.
- **Heatmaps**: Generate heatmaps to visualize correlations between features and targets.
- **Graphs**:
  - Temperature vs. Humidity
  - General Diffuse Flow vs. Diffuse Flow
  - Diffuse Flow vs. Power Consumption
  - Power Consumption across all zones

## Actions Taken for Data Cleaning and Feature Engineering

- **Null Entries**: Checked and handled any null values.
- **Correlation and Scatter Plots**: Analyzed data from January 2nd to explore correlations and build scatter plots.
- **Hypothesis Testing**: Used data from January 2nd and 3rd (00:00 to 23:50) for hypothesis testing related to diffuse flows.

## Key Findings and Insights

- **Power Consumption Correlation**: Power consumption across different zones is strongly correlated.
- **Temperature Effects**: Power consumption increases sharply with temperature above 12.5°C and decreases below 15°C.
- **Correlation**: Significant relationships between temperature, humidity, diffuse flows, and power consumption.

## Hypotheses

1. **Humidity Consistency**:
   - **H0**: Humidity levels are the same across different days of the week.
   - **H1**: Humidity levels differ across different days of the week.

2. **Zone 1 Power Consumption Consistency**:
   - **H0**: Power consumption in Zone 1 is consistent across different days of the week.
   - **H1**: Power consumption in Zone 1 varies across different days of the week.

3. **Effect of Temperature/Humidity on Power Consumption**:
   - **H0**: Higher temperature or humidity is not associated with higher power consumption.
   - **H1**: Higher temperature or humidity is associated with higher power consumption.

## Formal Significance Test

- **Test Conducted**: T-test for Zone 1 power consumption across January 2nd and 7th, 2017.
- **Results**: The p-value of 0.031 indicates a significant difference in power consumption levels between these two days, leading to the rejection of the null hypothesis.

## Suggestions for Next Steps

1. **T-Test for Humidity**: Implement a loop to compare t-test results for humidity values across different days of the week.
2. **PCA**: Apply Principal Component Analysis (PCA) to combine temperature, humidity, and wind speed features and test their correlation against power consumption levels.
3. **Additional Data Collection**: Acquire data for months beyond January to study power consumption trends across different seasons.

## Data Quality and Additional Data Request

The dataset is of high quality, with no null values or missing information. However, additional data covering months other than January would be valuable for analyzing seasonal variations in power consumption. The dataset contains some outliers, indicated by z-scores greater than 3:
- **General Diffuse Flows**: 23 outliers
- **Diffuse Flows**: 1361 outliers
- **Temperature**: 88 outliers
- **Zone 3 Power Consumption**: 656 outliers
- **Zone 2 Power Consumption**: 1 outlier
