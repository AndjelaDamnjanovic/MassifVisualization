# Massif Visualizer

## About project

This application is made by a student of computer science at the Faculty of Mathematics, University of Belgrade as the project for the *Software verification* course.
As the name suggests, the main purpose of this project is to visualize data obtained by using Valgrind's tool Massif.

Using Massif Visualizer, user can visualize one or many output files simultaniously. Also, it is possible to choose flags with which an exe file will be compiled and to
then visualize the result. If only one file is visualized, user can see the pie chart that displays which functions are responsible for allocating memory (only for detailed and peak snapshots).
On the other hand, if multiple files are chosen, then only the peak of each process is showed on the pie chart. Some of other possibilities are: saving graph as a .jpg or .png image and changing 
colors and thickness of a graph lines and choosing between a normal graph and a scatter plot. 

## Prerequisites:
- QtCreator (version 5.15.2 was used for developing this app)
- QtCharts (`sudo apt install libqt5charts5 libqt5charts5-dev`)
- C++ (version C++17)

## Setup:

### Download QtCreator
<ul>
<li>The QtCreator can be found and downloaded [on this website](https://www.qt.io/download). <br></li><li>After opening the link, scroll to the end of the page where the <i>download for open source use</i> is and click <i>go open source</i>.<br></li><li>When the previous step is done, scroll to the end of the page and click <i>Download the Qt online Installer</i>.<br></li><li>Now, click <i>Download</i> and you have downloaded the QtInstaller.</li><li>Now, the only thing you have to do is download the desired version of Qt.</li></ul>

### Downloading the Massif Visualizer

To download project Massif Visualizer, you need to open the terminal and type `git clone https://github.com/AndjelaDamnjanovic/MassifVisualization.git
`<br>
When you type the command, the project will be cloned onto your local device. 

### Start with Qt
<ul>
<li>After downloading the QtCreator, open the application on your computer.<br></li>
<li>When you open the app, click on <i>Open project</i></li>
<li>Now, find a folder where the project is downloaded and open <i>MassifVisualization.pro</i> file.</li>
<li>To build the project, click on the <i>Build</i> icon on the bottom left side of the screen.</li>
<li>When the project is built, run the project by clicking <i>Run</i> that is also located on the bottom left side of the screen.</li>
<li>The project is now running and you can use it.</li>
</ul>

## Test examples
The tests that were used during the development of the application can be found in the *tests* directory. Tests are split in groups: valid tests, invalid tests and 
tests for executable files.

## Developers
Anđela Damnjanović, 59/2019
