Knapsack K-Best
===============

_1-D Knapsack K-best solution porblem solver_

Author: luca.mella@studio.unibo.it

Status: Experimental

Copyleft: [CC-BY-NC 2014](http://creativecommons.org/licenses/by-nc/4.0/)

- - - - - - - - - - - - - - - -

# Introduction

Class project of 'Metodi e Modelli per il Supporto alle Decisioni'. 

The goal of the project is to  implement the algorithm published in the paper [An Algorithm For Determining The K-Best Solutions Of The One-Dimensional Knapsack Problem](http://dx.doi.org/10.1590/S0101-74382000000100011) for retrieving the k-best solutions in a monodimensional knapsack problem (aka KKP).

- - - - - - - - - - - - - - - -

### Getting Started

The main components of the project are the following:

| Component  | Task                                                                                              |
|:-----------|:-------------------------------------------------------------------------------------------------:|
| Solver     | Solve KKP problem from text files                                                                 | 
| Converter  | Convert _Galassi/Leardini_ problem library to "text list of problem paths"                        |
| List Solver| Process a "text list of problem paths" and produce a "CSV file containing computation information"| 
| Plot Tool  | Load "CSV file containing computation information" and plot data                                  |
| Generator  | Knapsack problem generator (David Pisinger's or Galassi/Leardini)                                 |

The expected usage of the project is:

* Generation of problem library with _Galassi/Leardini_ generator in a folder (eg. in `Desktop\problem_lib\`)
* Conversion of the problem library with Converter (eg. `problemlist.exe -d Desktop\problem_lib -o Desktop\problem_list`)
* Solving the list with the List Solver script (eg. `solve_problem_list.bat Desktop\problem_list\ Desktop\problem_list_performance\`)
* Plotting performance data using Plot Tool (eg. `plot.R Desktop\problem_list_performance\filename.lis.solved.csv Desktop\plots`)

Or:

* Using Solver for printing Decision Variables of the solutions (eg. `kbest_cli.exe -k 15 problemfile.in`) 

Or also:

* Using Solver for printing paper problem solutions (eg. `kbest_cli.exe --sample -k 15`)

### Project Structure


    binaries                        <-- Contains compiled binaries
    doc                             <-- Documentation folder, doxigen class project report
      |--> report                   <-- Class project report
      |--> doxygen                  <-- Code documentation
    kbest                           <-- Project folder, contains sources
      |--> src                      <-- C++ generator and solver code
      |--> kbest.py                 <-- Python solver (protorype)
      |--> problemlist.py           <-- Problem converter, from "Galassi and Leardini" library to list of problems 
      |--> solve_problem_list.sh    <-- Script for solving a problem list (Linux) 
      |--> solve_problem_list.bat   <-- Script for solving a problem list (Windows)
      |--> plots.R                  <-- Script for plotting computation times (Require R or RStudio)

### Dependencies

* [CMake > 2.8 or CMakeGui 2.8](http://www.cmake.org/files/v2.8/cmake-2.8.12.2-win32-x86.exe)
* g++ > 4 or MSVS > 10
* Python > 2.7
* R > 3 or [RStudio](http://www.rstudio.com/ide/download/)

- - - - - - - - - - - - - - - -

# Build

Building C++ solver in _Linux_:

* The `kbest/` folder contains a __CMake project__.
* Make a build folder `mkdir /tmp/kbestbuild` and also a binary folder `mkdir /tmp/kbestbins` then `cd /tmp/kbestbuild`
* Run CMake `cmake -DCMAKE_INSTALL_PREFIX=/tmp/kbestbins -DCMAKE_BUILD_TYPE=Release /pathToKBestFolder/kbest`
* `make; make install`, now `/tmp/kbestbins` contains all the needed executables and scripts

Building C++ solver in _Windows_:

* The `kbest\` folder contains a __CMake project__.
* Make a build folder `mkdir %USERPROFILE%\Desktop\kbestbuild` and also a binary folder `mkdir %USERPROFILE%\Desktop\kbestbins`
* Run `CMakeGui`
    * Click `Browse Source` and select `C:\\pathToKBestFolder\kbest`
    * Click `Browse Build` and select `%USERPROFILE%\Desktop\kbestbuild`
    * Click `Configure` and:
        * Building via __NMake__:
            * Select `NMake Makefiles`, go ahead
            * Set `CMAKE_INSTALL_PREFIX=%USERPROFILE%\Desktop\kbestbins`
            * Set `CMAKE_BUILD_TYPE=Release`
            * Click `Generate`
            * `cd %USERPROFILE%\Desktop\kbestbuild` 
            * `nmake & nmake install`
        * Building via __Visual Studio__:
            * Select `Visual Studio X 20ZY`, go ahead and then `Generate`
            * Set `CMAKE_INSTALL_PREFIX=%USERPROFILE%\Desktop\kbestbins`
            * Set `CMAKE_BUILD_TYPE=Release`
            * Click `Generate`
            * Go to `%USERPROFILE%\Desktop\kbestbuild`
            * Open `kbest.snl`

- - - - - - - - - - - - - - - -

# Usage


### Solver

Using the `kbest.py` Python solver :

    python kbest.py [-h] [--sample] [-v] [-p] [-k K] [prob [prob ...]]

    positional arguments:
      prob                  Text file containing problem data

    optional arguments:
      -h, --help            Show this help message and exit
      --sample              Run with the sample problem of the paper
      -v                    Verbose output
      -p                    Print only performance, no k-best solutions
      -k K                  Number of best solution to retrieve


Using the `kbest_cli` C++ solver:

    kbest_cli.exe -h
     
     Usage:
     
        ./kbest_cli [-h] [-p] [--sample] [-k NUM] [PROB [PROB ...]]
     
     Options:
          -h         This help
          -p         Print only performance, no k-best solutions
          -k         Number of best solution to retrieve
          --sample   Run with the sample problem of the paper
     
     Params:
     
           PROB      File where to load a 1-D knapsack problem

### Converter

Using the `problemlist.py` converter:

    usage: problemlist.py [-h] [-d DIR] [-o OUTDIR] [-n] [-t]

    optional arguments:
      -h, --help            show this help message and exit
      -d DIR                Directory that contains a libraries generated with "Galassi-Leardini" generator 
      -o OUTDIR             Directory where to save generated problem lists
      -n                    Group problems by number of vars
      -t                    Group problems by instance type

### List Solver

Using the `solve_problem_list.bat` and `solve_problem_list.sh` scripts:


    usage: solve_problem_list.bat INDIR OUTDIR

    Params:

           INDIR      Directory where to search for *.lst files generated by the problemlist.py converter
           OUTDIR     Directory where to store CSV file containing performance data of the resolution of a problem list

### Plot Tool

Using the `plots.R` scripts:

    usage : ./plots.R INCSVFILE OUTFILE
    
    Params:

          INCSVFILE    Input CSV file generated by the solve_problem_list.bat script
          OUTFILE      Output file (3 files SVG images will be created using 
                       OUTFILE as initial part of the file name)

### Generator

Using the [problem generator](http://www.diku.dk/~pisinger/codes.html) `generator_cli`:

   Usage:
      generator_cli n r type i S
   
   Parameters:
      n: number of items
      r: range of coefficients, 
      type: 1=uncorrelated
            2=weakly corr
            3=strongly corr
            4=inverse str.corr
            5=almost str.corr
            6=subset-sum
            7=even-odd subset-sum
            8=even-odd knapsack
            9=uncorrelated, similar weights
            11=Avis subset-sum
            12=Avis knapsack
            13=collapsing KP
            14=bounded strongly corr
            15=No small weights
      i: instance no
      S: number of tests in series (typically 1000)


- - - - - - - - - - - - - - - -
