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
    xor - play with a XOR neural network<br>
<br>
SYNOPSIS<br>
    ./xor [network options...] mode [mode options...] [-s] GENERAL CASE<br>
<br>
    network options:<br>
        -f path (IMPORT NETWORK FROM FILE)<br>
        EXAMPLE: ./xor -f network.nw mode [mode options...] [-s]<br>
<br>
        -lh [layer size ...] (CREATE NETWORK WITH CUSTOM DIMENSIONS)<br>
        EXAMPLE: ./xor -lh 12 8 mode [mode options...] [-s] (2 HIDDEN LAYERS)<br>
<br>
    mode (ONE OF THE FOLLOWING):<br>
        test nb_tests<br>
            - nb_tests: integer (number of tests to make)<br>
<br>
        learn nb_tests learning_rate<br>
            - nb_tests: integer (number of tests to make)<br>
            - learning_rate: double (learning rate of the network)<br>
<br>
    xor [network options...] mode [mode options...] -s [path]<br>
        SAVE THE NETWORK AFTER EXECUTION (TO path IF SPECIFIED)<br>
<br>
DESCRIPTION<br>
    xor is an easy to use XOR neural network. You can dimension it as you like,<br>
    easily perform tests and make your network learn. You can also import and<br>
    export your network easily.<br>
<br>
EXAMPLES<br>
    ./xor -lh 12 learn 10000 0.9 -s<br>
        Create a XOR neural network with a single 12 neuron hidden layer, make<br>
        it learn with 10000 tests with a learning rate of 0.9, and save it to<br>
        the default file: 'network.nw'<br>
    ./xor -f network.nw test 100000<br>
        Load a XOR neural network from the network.nw file and perform 100000<br>
        tests on it.<br>
<br>
