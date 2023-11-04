# OCR SUDOCUL

A program that solves sudokus with a photo of the grid.

## BUILD THE PROJECT
`git clone https://github.com/Michael-Rousseau/OCR_sudocul.git`
`cd OCR_sudocul`
Use the command `make` at the root of the project.

## CLEAN THE PROJECT
`make clean` at the root of the project.

## EXECUTABLES
All executables can be found in the build folder (at the root).

## DOCMENTATION

### XOR

NAME<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;xor &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;play with a XOR neural network<br>
<br>
SYNOPSIS<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;./xor [network options...] mode [mode options...] [-s] GENERAL CASE<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;network options:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-f path (IMPORT NETWORK FROM FILE)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EXAMPLE: ./xor -f network.nw mode [mode options...] [-s]<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-lh [layer size ...] (CREATE NETWORK WITH CUSTOM DIMENSIONS)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EXAMPLE: ./xor -lh 12 8 mode [mode options...] [-s] (2 HIDDEN LAYERS)<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;mode (ONE OF THE FOLLOWING):<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;test nb_tests<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nb_tests: integer (number of tests to make)<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;learn nb_tests learning_rate<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;nb_tests: integer (number of tests to make)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;learning_rate: double (learning rate of the network)<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;xor [network options...] mode [mode options...] -s [path]<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SAVE THE NETWORK AFTER EXECUTION (TO path IF SPECIFIED)<br>
<br>
DESCRIPTION<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;xor is an easy to use XOR neural network. You can dimension it as you like,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;easily perform tests and make your network learn. You can also import and<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;export your network easily.<br>
<br>
EXAMPLES<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;./xor -lh 12 learn 10000 0.9 -s<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Create a XOR neural network with a single 12 neuron hidden layer, make<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;it learn with 10000 tests with a learning rate of 0.9, and save it to<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;the default file: 'network.nw'<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;./xor -f network.nw test 100000<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Load a XOR neural network from the network.nw file and perform 100000<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;tests on it.<br>
<br>
