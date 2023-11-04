# BUILD THE PROJECT
`make`

# CLEAN THE PROJECT
`make clean`

# EXECUTABLES
In the build folder.

## XOR

NAME
    xor - play with a XOR neural network

SYNOPSIS
    ./xor [network options...] mode [mode options...] [-s] GENERAL CASE

    network options:
        -f path (IMPORT NETWORK FROM FILE)
        EXAMPLE: ./xor -f network.nw mode [mode options...] [-s]

        -lh [layer size ...] (CREATE NETWORK WITH CUSTOM DIMENSIONS)
        EXAMPLE: ./xor -lh 12 8 mode [mode options...] [-s] (2 HIDDEN LAYERS)

    mode (ONE OF THE FOLLOWING):
        test nb_tests
            - nb_tests: integer (number of tests to make)

        learn nb_tests learning_rate
            - nb_tests: integer (number of tests to make)
            - learning_rate: double (learning rate of the network)

    xor [network options...] mode [mode options...] -s [path]
        SAVE THE NETWORK AFTER EXECUTION (TO path IF SPECIFIED)

DESCRIPTION
    xor is an easy to use XOR neural network. You can dimension it as you like,
    easily perform tests and make your network learn. You can also import and
    export your network easily.

EXAMPLES
    ./xor -lh 12 learn 10000 0.9 -s
        Create a XOR neural network with a single 12 neuron hidden layer, make
        it learn with 10000 tests with a learning rate of 0.9, and save it to
        the default file: 'network.nw'
    ./xor -f network.nw test 100000
        Load a XOR neural network from the network.nw file and perform 100000
        tests on it.

