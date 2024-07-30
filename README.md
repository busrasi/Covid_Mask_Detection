# Project Documentation: Real-Time Mask Detection on Human Faces

## Introduction
The repository contains a project focused on detecting masks on human faces in real-time using various technologies such as OpenCV, Keras, and TensorFlow. This documentation will provide an overview of the project, including its purpose, technologies used, methodology, and how to use the code.

## Project Overview
### Objective
The primary goal of the project is to develop a desktop application that can detect whether individuals in a given environment are wearing masks. This is particularly relevant during the Covid-19 pandemic to ensure compliance with health regulations.

## Technologies Used
1. **Programming Language:** Python
2. **Libraries:** 
   - OpenCV
   - Keras
   - TensorFlow
3. **Development Environment:** Visual Studio

## Methodology
### Data Collection
- The dataset consists of images of faces with and without masks.
- These images are collected from various angles and stored in a deep learning cloud system for training.

### Machine Learning Model
- The project employs deep learning techniques to create a model capable of distinguishing between masked and unmasked faces.
- The training process involves using Keras and TensorFlow to classify the images in the dataset.

### Image Processing
- OpenCV is used for capturing and processing images from the camera.
- Faces are detected and various facial features such as eyes, nose, and mouth are identified using OpenCV and NumPy.

### Application Development
- The application is designed to be a desktop application aimed at institutions.
- It uses an external camera to capture real-time images and analyze them to detect masks.
