# Training

## Directory Structure
    data/
        raw/
            positive.dat
            negative.dat
        transformed/
            positive_train.dat
            negative_train.dat
            positive_test.dat
            negative_test.dat
    models/
        svm_classifier.dat
        dnn_classifier.dat
    
- `data/raw/positive.dat` contains positive vectors in the format that can be read by `sampml::data:reader`
- `data/raw/negative.dat` contains negative vectors in the format that can be read by `sampml::data:reader`
- `data/transformed/positive_train.dat` contains positive training set
- `data/transformed/negative_train.dat` contains negative training set
- `data/transformed/positive_test.dat` contains positive testing set
- `data/transformed/negative_test.dat` contains negative testing set

## Programs
- transformer (`transformer.cpp`)
  - performs feature engineering
  - splits the transformed dataset into a training set and a validation/testing set
- analyzer (`analyzer.cpp`)
  - processes the transformed data and prints statistics about each feature
- svm (`svm.cpp`)
  - builds a SVM model from the transformed data
  - ranks features
  - evaluates the model using the testing set and prints the statistics
- dnn (`dnn.cpp`)
  - builds a DNN model from the transformed data
  - evaluates the model using the testing set and prints the statistics
  
The programs can be built using the `Makefile` provided. 
- `make all`, `make clean`
- `make transformer`, `make analyzer`, `make svm`, `make dnn`
- `make transformer-clean`, `make analyzer-clean`, `make svm-clean`, `make dnn-clean`

## building models

1. obtain quality dataset consists of large number of shots for both classes: aimbot and non-aimbot
   - ensure that there are approximately equal number of transformed positive and negative samples for a good model
2. run `./transformer` (should produce transformed training and testing sets in `data/transformed/`)
3. [optional] run `./analyzer` to look at the distributions formed by the features and compare between the positive and negative class
4. `./svm` will train an SVM model using the training data and saves a model at `models/svm_classifier.dat`
5. `./dnn` will train a DNN model using the training data and saves a model at `models/dnn_classifier.dat`
