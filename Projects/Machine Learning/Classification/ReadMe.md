# HeartPredict: Advanced Classification for Early Heart Disease Diagnosis

## Main Objective of the Analysis

The primary objective of this analysis was to build a predictive model for diagnosing heart disease based on a set of features. The goal was to use this model to predict the likelihood of heart disease diagnosis in patients, enhancing early detection and potentially guiding treatment decisions. By focusing on prediction, the model can provide actionable insights to healthcare professionals and stakeholders, leading to better patient outcomes and more efficient use of medical resources.

## Brief Description of the Data Set

The dataset used contains thirteen features that are relevant to heart disease diagnosis, with the target variable indicating the presence of heart disease. This target variable is an integer ranging from 0 (no presence) to 4 (various degrees of heart disease presence). For simplicity, experiments concentrated on distinguishing between the presence (values 1 to 4) and absence (value 0) of heart disease. The analysis aims to implement a classification tool that predicts whether a patient has heart disease based on these features.

## Data Exploration and Cleaning

- **Data Rounding and Normalization:** Data values were rounded to two decimal places and normalized to a range between 0 and 1 across all columns to ensure consistency and comparability.
- **Null Values:** The dataset was checked for null values, which were absent.
- **Correlation Analysis:** Pair plots and heatmaps were generated to analyze the relationships between features. These visualizations revealed both positive and negative correlations among the attributes.

## Model Training and Evaluation

1. **Linear Regression:**
   - Polynomial features (degree 1)
   - Training/testing split: 20%
   - R² value: 0.705

2. **Logistic Regression (newton-cg solver):**
   - Training/testing split: 20%
   - R² value: 0.868

3. **Logistic Regression (saga solver):**
   - Training/testing split: 20%
   - R² value: 0.802

4. **Lasso Regression:**
   - Alpha value: 0.001
   - Training/testing split: 10%
   - R² value: 0.806

5. **Support Vector Machine (SVM):**
   - C value: 0.3
   - Training/testing split: 20%
   - R² value: 0.967

6. **K-Nearest Neighbors (KNN):**
   - Number of neighbors: 15
   - Training/testing split: 20%
   - R² value: 0.967

7. **Decision Tree Classifier:**
   - Criterion: entropy
   - Maximum depth: 7
   - Minimum weight fraction leaf: 0.001
   - Training/testing split: 10%
   - R² value: 1.0

8. **Random Forest Classifier:**
   - Number of estimators: 24
   - Training/testing split: 20%
   - R² value: 0.951

## Recommended Model

The **Decision Tree Classifier** emerged as the best model due to its perfect accuracy (R² value of 1.0). It also provides the advantage of explainability, allowing for a detailed understanding of the decision-making process and feature importance. While other models like SVM, KNN, and Random Forest also showed high accuracy, the Decision Tree offers a clear, interpretable structure that can be crucial for understanding the underlying factors influencing heart disease diagnosis.

## Key Findings and Insights

- **Model Performance:** Different models demonstrated varying levels of accuracy, with the Decision Tree Classifier, SVM, KNN, and Random Forest performing exceptionally well.
- **Predictive Capability:** The models confirmed that it is feasible to train a classification tool to predict heart disease diagnoses effectively.
- **Feature Relationships:** Correlation analyses helped identify relationships between features, which can be useful for understanding and refining the model.

## Suggestions for Next Steps

1. **Cross-Validation:** Implement cross-validation to ensure the model's robustness and generalizability beyond the training and testing splits.
2. **Feature Expansion:** Integrate additional features related to heart disease to potentially improve the model's performance.
3. **Data Augmentation:** Increase the dataset size with more rows or entries to enhance training and testing accuracy.
4. **Data Visualization:** Further visualize results to strengthen understanding of how features correlate with heart disease.
5. **Outlier Detection:** Examine the dataset for outliers within an acceptable margin of error to ensure the quality and reliability of the predictions.

These steps will help refine the model, improve prediction accuracy, and provide deeper insights into the factors influencing heart disease diagnosis.
