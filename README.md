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

## How to use Massif Visualizer?
As mentioned in the first section, Massif Visualizer is primarily used for the visualization of Massif's output files. When the application is run, the window will look like this: 

![Screenshot from 2024-08-23 21-18-03](https://github.com/user-attachments/assets/d71ad3db-af7c-4e89-99f3-41666f109093)

All of the possible options are shown on the left side of the screen: In the *File* section user can choose whether he wants to open one (or multiple) massif file(s), save visualized data (as .jpg, .png, or just a regular file) or exit the application. 

![Screenshot from 2024-08-23 21-08-37](https://github.com/user-attachments/assets/b233d11f-a3e2-4a63-9e98-4364280bc67a)

The rest of the options can be found in the tab widget. First tub lets user choose how he wants his graphs shown -- background colour and graph colour can be changed, as well as the thickness of lines shown on graph and the type of graph shown: normal or scattered plot. The last option in this tab determins whether time-unit or snapshot number will be displayed on x-axis. If the field is not checked, snapshot numbers will be displayed.

![Screenshot from 2024-08-23 21-09-02](https://github.com/user-attachments/assets/de890d73-bdc9-4766-88d1-800db2998ead)

If user wants to choose options with which the executable file will be compiled he fills in the information in the second tab. After the selection is done, the program executes command with selected options and shows output the same way as when opening a regular massif file(s).

![Screenshot from 2024-08-23 21-09-12](https://github.com/user-attachments/assets/7c1a54b3-e0a0-4960-94d9-4bb180da85c5)

Opening one massif file with default options results in the following graph:
![Screenshot from 2024-08-23 21-10-24](https://github.com/user-attachments/assets/2314f71f-52a8-476e-b64c-ae556e0521ed)

If different colors are chosen, and scatter plot, the graph looks like this:

![Screenshot from 2024-08-23 21-10-47](https://github.com/user-attachments/assets/5802685c-fb93-463a-982f-6dd404f60409)

Finally, the graph that shows results of multiple massif files can be seen below:

![slika](https://github.com/user-attachments/assets/cbee5c56-fb15-4278-915b-46a579d2a456)


Lastly, in the *Pie charts* tab pie charts of detailed snapshots are shown (if only one massif file is visualized). If multiple massif files are opened, only the peak snapshot of each output file is shown as the pie chart.

![Screenshot from 2024-08-23 21-09-59](https://github.com/user-attachments/assets/e87b7d7e-b4f2-498c-8749-7be88cd01d7e)

## Test examples
The tests that were used during the development of the application can be found in the *tests* directory. Tests are split in groups: valid tests, invalid tests and 
tests for executable files.

## Developers
Anđela Damnjanović, 59/2019
