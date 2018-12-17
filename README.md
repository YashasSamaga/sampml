# sampml
Machine learning tools for SAMP

## Requirements

**Mandatory**
- [https://github.com/davisking/dlib](http://dlib.net/) - C++ ML Toolkit

**Optional**
- any BLAS/LAPACK library
  - [OpenBLAS](https://github.com/xianyi/OpenBLAS/)

## Directory Structure
    examples\
        anti-aimbot\
        anti-proaim\
    sampml\
    
- `examples\anti-proaim\` simple example showing how to use `sampml::trainer::svm_classifier`
- `examples\anti-aimbot\` contains code to train models and a plugin that can be used for detection
- `sampml` SAMPML library

More information can be found in `README.md` in the subdirectories.
