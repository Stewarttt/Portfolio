# Prediction Analysis of the Iris Dataset

## Objective of the Analysis

The primary objective of this analysis is to perform prediction using the Iris dataset. The focus is on creating models that can accurately predict the type of Iris plant based on its sepal and petal measurements.

## Description of the Dataset

**Dataset Chosen**: Iris dataset from Fisher, 1936  
**Available at**: [UCI Machine Learning Repository - Iris Dataset](https://archive.ics.uci.edu/ml/datasets/iris)  
**Attributes**:
- **Classes**: 3 (each representing a different Iris plant species)  
- **Instances**: 50 per class, totaling 150 instances  
- **Features**: 4 quantitative attributes - sepal length, sepal width, petal length, and petal width  
- **Class**: Categorical variable indicating the type of Iris plant

## Data Exploration and Cleaning

**Exploration Activities**:
- **Variables Analysis**: Reviewed data variables and quantile information.
- **Summary Statistics**: Generated summary statistics for all features.
- **Null Value Checks**: Verified that there are no missing values in the dataset.
- **Visualizations**:
  - **Plots**: Displayed sepal length vs. sepal width and petal length vs. petal width, categorized by flower class.
  - **Heatmap**: Depicted correlations between different columns of the dataset.
  - **Histogram**: Confirmed that each of the 150 samples is equally represented and converted to integer representations for linear regression.

## Linear Regression Models

### Model Variations
1. **Simple Linear Regression**: Used as a baseline.
2. **Polynomial Regression**:
   - **Degree 1**: R² score = 0.9321
   - **Degree 2**: R² score = 0.9362
   - **Degree 3**: R² score = 0.8433 (without bias), 0.5339 (with bias)

### Training Details
- **Random State**: 72018
- **Testing Size**: 0.3
- **Bias**: Tested with and without bias

## Recommended Model

**Polynomial Regression (Degree 2)** is recommended as the final model. It achieved the highest R² score of 0.9362, indicating the best fit. The model's performance suggests that the polynomial degree of 2 effectively captures the complexity of the data. This degree of polynomial regression can differentiate between the three classes of Iris plants, with one degree focusing on separating one class from the others and another degree distinguishing the remaining two classes.

## Key Findings and Insights

- **Petal Length and Width**: Strongly correlated with the Iris plant class.
- **Sepal Width**: Exhibits a negative correlation with plant class.
- **Model Performance**: Polynomial regression with degree 2 provides the best fit for predicting Iris plant species.
- **Correlation**: Length and width measurements are heavily correlated with the class, enabling accurate predictions.

## Suggestions for Next Steps

1. **Explore Other Models**: Consider using alternative methods such as polynomial regression, ridge regression, or lasso regression to address potential bias and improve predictions.
2. **Expand Dataset**: Increase the number of entries and include a wider diversity of samples or species to enhance model generalizability.
3. **Additional Features**: Collect and incorporate more measurements for the Iris plants to improve the model's explanatory power.

By following these steps, we can further enhance our analysis and achieve better predictive accuracy for Iris plant classification.
