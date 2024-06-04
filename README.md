# Parallel Image Convolution

## How to run the scripts
1. Python code will be used to create a speedup graph. In *parallel-image-processing* directory, run the following to set up Python virtual environment
```terminal
python -m venv env
```
2. Activate the virtual environment with the following
```terminal
source env/bin/activate
```
3. In *parallel-image-processing* directory, run the following to install the Python libraries for generating graph. 
```terminal
pip install -r requirements.txt
```
4. Change directory to data directory and create an output directory for the processed images
```terminal
cd data
mkdir out
```
5. If you have not installed OpenCV, install OpenCV first. For example:
```terminal
brew install opencv
```
6. If you have not installed cmake, install cmake first. For example:
```terminal
brew install cmake
```
7. Make a build directory in *parallel-image-processing* directory and run the following to build the C++ code
```terminal
mkdir build
cd build
cmake ..
make
```
8. For just running the script for the image processing tasks for testing, there are different settings available. Note that size can be configured to run multiple file sizes e.g. small+mixture will run small and mixture. Mode s means running sequential version, parfiles means running multiple files in parallel, parslices means running multiple threads on each image. Images processed will be available in *data/out*. 
```terminal
cd build
./Parallel-Image-Processing editor <size: small | mixture | big> <mode: s | parfiles | parslices> <number of threads>

```
9. For measuring performance and generating the speedup graph, create the slurm folders and modify *benchmark.sh* and *graph.py* accordingly for the appropriate file paths, input and output file names. 
```terminal
cd benchmark
mkdir slurm
cd slurm
mkdir out
mkdir err
```
10. Run the job in cluster. Speedup results will be available in *benchmark/slurm/out* directory and images processed will be available in *data/out*. 
```terminal
cd benchmark
sbatch benchmark.sh
```
11. When ready to exit, run the following to deactivate the Python virtual environment 
```terminal
deactivate
```