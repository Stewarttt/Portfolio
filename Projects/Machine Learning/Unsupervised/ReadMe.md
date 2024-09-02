# Dimensionality Reduction Analysis of Breast Cancer Coimbra Dataset

## Objective of the Analysis

The main objective of this analysis is to perform dimensionality reduction on the Breast Cancer Coimbra dataset. This will enable us to visualize the data in a 2D plot, which can help stakeholders identify trends and patterns related to the presence or absence of breast cancer. The benefits of this analysis include simplifying complex data for better interpretability and aiding in the identification of key features that distinguish between breast cancer patients and healthy controls.

## Description of the Dataset

**Dataset Chosen**: Breast Cancer Coimbra  
**Available at**: [Breast Cancer Coimbra Dataset](https://archive.ics.uci.edu/dataset/451/breast+cancer+coimbra)

**Attributes**:
- **Predictors**: 10 quantitative variables, including anthropometric data and routine blood analysis parameters.
- **Dependent Variable**: Binary outcome indicating the presence or absence of breast cancer.

The goal is to use dimensionality reduction techniques to project the data into a 2D space, allowing us to identify trends and classify participants as either breast cancer patients or healthy controls.

## Data Exploration and Cleaning

**Exploration Activities**:
- **Summary Statistics**: Analyzed the distribution and range of predictor variables.
- **Percentile Analysis**: Assessed the distribution of data points.
- **Null Values**: Checked for and handled any missing values.
- **Pair-Plots and Heatmaps**: Visualized relationships between predictors and overall data distribution.

**Data Cleaning and Feature Engineering**: Ensured data quality by addressing missing values and standardized features if necessary.

## Clustering Model Variations

**1. KMeans Clustering**:
- **Number of Clusters**: 4
- **Results**: Generated a diagram with four distinct sections. Three of these sections effectively separate the data points, while one section includes outliers.

**2. Agglomerative Clustering**:
- **Results**: Produced clusters similar to KMeans, but the data points were segmented differently, with outliers in a separate category.

**3. Birch Clustering**:
- **Results**: Similar to Agglomerative clustering, with a distinction between outliers and central clusters.

## Recommended Model

**KMeans Clustering** is recommended as the final model. It provides a clearer separation of data into distinct clusters compared to Agglomerative and Birch clustering, particularly along the left-hand side of the 2D plot. This model effectively partitions the data, making it easier to interpret and visualize differences related to breast cancer diagnosis.

## Key Findings and Insights

- The analysis demonstrates that dimensionality reduction can be successfully applied to cluster and visualize complex breast cancer data in 2D.
- KMeans clustering reveals distinct sections related to breast cancer, providing a useful way to analyze and interpret the data.
- This method facilitates easier identification of patterns and trends associated with breast cancer patients versus healthy controls.

## Next Steps

1. **Enhance Clustering**: Explore additional clustering techniques or adjust parameters to refine groupings.
2. **Feature Analysis**: Investigate adding or modifying features to improve clustering results.
3. **Dimensionality Reduction**: Consider using Principal Component Analysis (PCA) with more than two dimensions to capture additional variance in the data.
4. **Validation**: Compare clustering results with actual patient diagnoses to validate and potentially improve clustering accuracy.

By following these steps, we can further enhance the analysis and gain deeper insights into the data.
