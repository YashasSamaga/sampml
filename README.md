# sampml
Machine Learning tools for SAMP

## Requirements

**Mandatory**
- [https://github.com/davisking/dlib](http://dlib.net/) - C++ ML Toolkit

**Optional**
- [OpenBLAS](https://github.com/xianyi/OpenBLAS/) - BLAS and LAPACK (speeds things up)

## Directory Structure
    datasets\
        proaim\
    includes\
    models\
    sampml\
    
- `datasets` collection of datasets that can be used for training
- `includes` Pawn includes that facilitate data collection
- `models` trained ready-to-use models
- `sampml` sampml library (with examples)
  
## Feature Vector
A feature vector is an array of features (just a number). It is internally represented as a column matrix.
  
`sampml::feature_vector<double, 22>` represents a feature vector containing 22 `double`s.
  
## Data Reader
SAMPML comes with a basic dataset reader (`sampml/sampml/data.hpp`) which reads data from text file. It expects every line in the data file
to be a vector of `N` features. It parses the file and provides a container-like interface for manipulating the data.
 
```
constexpr int feature_size = 25;
using sample_type = sampml::feature_vector<double, feature_size>;
sampml::data::reader<sample_type> positive_dataset("dataset/something/positive_vectors");
 
std::cout << "Number of vectors: " << positive_dataset.siez() << std::endl;
for(const auto& vec : positive_dataset)
  std::cout << vec << std::endl;
```
  
##  SVM Classifier
SAMPML contains a very basic minimal SVM classifier (`sampml/sampml/svm_classifier.hpp`) that abstracts most of the technical details
and presents an intiutive interface.
  
The SVM uses a radial basis function kernel and hence can solve non-linear problems. It automatically normalizes and 
performs cross validation on the dataset.
  
### Training
To train a SVM classifier, you need to have two datasets:
1. dataset containing vectors for which the classifier must classify as positive
2. dataset containing vectors for which the classifier must classify as negative

```
constexpr int feature_size = 25;
using sample_type = sampml::feature_vector<double, feature_size>;

sampml::data::reader<sample_type> data_negative("negative_dataset.dat");
sampml::data::reader<sample_type> data_positive("positive_dataset.dat");

sampml::trainer::svm_classifier<sample_type> classifier;
classifier.set_samples(data_positive, data_negative); /* provide the datasets to the classifier */
classifier.train(); /* train the classifier */
    
classifier.serialize("classifier.dat"); /* save the classifier for reuse */
```
  
### Testing
```
 constexpr int feature_size = 25;
 using sample_type = sampml::feature_vector<double, feature_size>;

 sampml::trainer::svm_classifier<sample_type> classifier;
 classifier.deserialize("classifier.dat");

 std::cout << "positive probability: " << classifier.test(test_vector) << std::endl;
```
