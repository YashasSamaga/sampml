# AntiAimbot

## Overview
The architecture consists of two independent and separate components: training and detection. The training code is used to analyze the data collected and train different models which can be saved into files. The detection code is in the plugin source which loads the models (trained using the training component) and uses them to classify samples given at runtime.

## Directory Structure
    filterscripts\
        DCS.pwn
        AntiAimbot.pwn
    includes\
        aimbot_dc.inc
    plugin\
        ...
    training\
        ...
    
- `filterscripts\DCS.pwn` example script showing how data is collected
- `filterscripts\AntiAimbot.pwn` example script showing how to detect players using aimbot
- `includes\aimbot_dc.inc` include containing data collection structures and encoding functions
- `plugin` plugin source
- `training` code for training models
