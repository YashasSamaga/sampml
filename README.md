# sampml
| Travis CI | AppVeyor Linux | AppVeyor Windows |
| --------- | -------------- | ---------------- |
| [![Build Status](https://travis-ci.org/YashasSamaga/sampml.svg?branch=master)](https://travis-ci.org/YashasSamaga/sampml) | [![Build status](https://ci.appveyor.com/api/projects/status/us7n7fx75qlblyg3?svg=true)](https://ci.appveyor.com/project/YashasSamaga/sampml) | [![Build status](https://ci.appveyor.com/api/projects/status/gjj8krqn9uqmgmv9?svg=true)](https://ci.appveyor.com/project/YashasSamaga/sampml-mjxk0) |

---

simplified machine learning toolkit and associated SAMP projects 

### Projects:
 - Anti Aimbot (`examples/anti-aimbot`)

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
