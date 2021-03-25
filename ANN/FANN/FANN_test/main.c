//Copyright (c) 2018 ETH Zurich, Ferdinand von Hagen, Michele Magno, Lukas Cavigelli

#include <stdio.h>
#include "fann_conf.h"
#include "fann.h"
#include "fann_structs.h"
#include "fann_net.h"
#include <arm_math.h>



fann_type neuron_values[NUM_NEURONS];

fann_type *fann_run(fann_type * input)
{
    fann_type *neurons;
    unsigned int i, num_connections;
    fann_type neuron_sum, *output;
    fann_type *weights;
    unsigned int activation_function, layer_it, neuron_it, last_neuron;
    fann_type steepness;

    /* store some variabels local for fast access */
#ifdef FIXEDFANN
    /* values used for the stepwise linear sigmoid function */
    fann_type r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0;
    fann_type v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0, v6 = 0;

    fann_type last_steepness = 0;
    unsigned int last_activation_function = 0;
#else
    fann_type max_sum = 0;
#endif

    /* first set the input */
    arm_fill_f32(1.0f, neuron_values, NUM_NEURONS);
    arm_copy_f32(input, &neuron_values[fann_layers[0].first_neuron], NUM_INPUT);

    for(layer_it = 1; layer_it != NUM_LAYERS; ++layer_it)
    {
        last_neuron = fann_layers[layer_it].last_neuron;
        for(neuron_it = fann_layers[layer_it].first_neuron; neuron_it != last_neuron; ++neuron_it)
        {
            if(fann_neurons[neuron_it].first_connection == fann_neurons[neuron_it].last_connection)
            {
                continue;
            }

            activation_function = fann_neurons[neuron_it].activation_function;
            steepness = fann_neurons[neuron_it].activation_steepness;

            neuron_sum = 0;
            num_connections = fann_neurons[neuron_it].last_connection - fann_neurons[neuron_it].first_connection;
            weights = fann_weights + fann_neurons[neuron_it].first_connection;

            if(CONNECTION_RATE >= 1)
            {
                if(network_type == FANN_NETTYPE_SHORTCUT)
                {
                    neurons = neuron_values;
                }
                else
                {
                    neurons = neuron_values + fann_layers[layer_it - 1].first_neuron;
                }

                arm_dot_prod_f32(weights, neurons, num_connections, &neuron_sum);
            }
            else
            {
                // Not supported yet...
              exit(-1);
	    }
#ifdef FIXEDFANN
            if(activation_function != last_activation_function || steepness != last_steepness)
            {
                switch (activation_function)
                {
                    case FANN_SIGMOID:
                    case FANN_SIGMOID_STEPWISE:
                        r1 = SIGMOID_RESULTS_0;
                        r2 = SIGMOID_RESULTS_1;
                        r3 = SIGMOID_RESULTS_2;
                        r4 = SIGMOID_RESULTS_3;
                        r5 = SIGMOID_RESULTS_4;
                        r6 = SIGMOID_RESULTS_5;
                        v1 = SIGMOID_SYMMETRIC_RESULTS_0 / steepness;
                        v2 = SIGMOID_SYMMETRIC_RESULTS_1 / steepness;
                        v3 = SIGMOID_SYMMETRIC_RESULTS_2 / steepness;
                        v4 = SIGMOID_SYMMETRIC_RESULTS_3 / steepness;
                        v5 = SIGMOID_SYMMETRIC_RESULTS_4 / steepness;
                        v6 = SIGMOID_SYMMETRIC_RESULTS_5 / steepness;
                        break;
                    case FANN_SIGMOID_SYMMETRIC:
                    case FANN_SIGMOID_SYMMETRIC_STEPWISE:
                        r1 = SIGMOID_SYMMETRIC_RESULTS_0;
                        r2 = SIGMOID_SYMMETRIC_RESULTS_1;
                        r3 = SIGMOID_SYMMETRIC_RESULTS_2;
                        r4 = SIGMOID_SYMMETRIC_RESULTS_3;
                        r5 = SIGMOID_SYMMETRIC_RESULTS_4;
                        r6 = SIGMOID_SYMMETRIC_RESULTS_5;
                        v1 = SIGMOID_SYMMETRIC_VALUES_0 / steepness;
                        v2 = SIGMOID_SYMMETRIC_VALUES_1 / steepness;
                        v3 = SIGMOID_SYMMETRIC_VALUES_2 / steepness;
                        v4 = SIGMOID_SYMMETRIC_VALUES_3 / steepness;
                        v5 = SIGMOID_SYMMETRIC_VALUES_4 / steepness;
                        v6 = SIGMOID_SYMMETRIC_VALUES_5 / steepness;
                        break;
                    case FANN_THRESHOLD:
                        break;
                }
            }

            switch (activation_function)
            {
                case FANN_SIGMOID:
                case FANN_SIGMOID_STEPWISE:
                    neuron_sum =
                        (fann_type) fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6, 0,
                                                  MULTIPLIER, neuron_sum);
                    break;
                case FANN_SIGMOID_SYMMETRIC:
                case FANN_SIGMOID_SYMMETRIC_STEPWISE:
                    neuron_sum =
                        (fann_type) fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6,
                                                  -MULTIPLIER, MULTIPLIER, neuron_sum);
                    break;
                case FANN_THRESHOLD:
                    neuron_sum = (fann_type) ((neuron_sum < 0) ? 0 : MULTIPLIER);
                    break;
                case FANN_THRESHOLD_SYMMETRIC:
                    neuron_sum = (fann_type) ((neuron_sum < 0) ? -MULTIPLIER : MULTIPLIER);
                    break;
                case FANN_LINEAR:
                    neuron_sum = neuron_sum;
                    break;
                case FANN_LINEAR_PIECE:
                    neuron_sum = (fann_type)((neuron_sum < 0) ? 0 : (neuron_sum > MULTIPLIER) ? MULTIPLIER : neuron_sum);
                    break;
                case FANN_LINEAR_PIECE_SYMMETRIC:
                    neuron_sum = (fann_type)((neuron_sum < -MULTIPLIER) ? -MULTIPLIER : (neuron_sum > MULTIPLIER) ? MULTIPLIER : neuron_sum);
                    break;
                case FANN_ELLIOT:
                case FANN_ELLIOT_SYMMETRIC:
                case FANN_GAUSSIAN:
                case FANN_GAUSSIAN_SYMMETRIC:
                case FANN_GAUSSIAN_STEPWISE:
                case FANN_SIN_SYMMETRIC:
                case FANN_COS_SYMMETRIC:
                    while(1) {}
                    break;
            }
            last_steepness = steepness;
            last_activation_function = activation_function;
            neuron_values[neuron_it] = neuron_sum;
#else
            neuron_sum = fann_mult(steepness, neuron_sum);

            max_sum = 150.0f / steepness;
            if(neuron_sum > max_sum)
                neuron_sum = max_sum;
            else if(neuron_sum < -max_sum)
                neuron_sum = -max_sum;

            fann_activation_switch(activation_function, neuron_sum, neuron_values[neuron_it]);

#endif
        }
    }

    /* set the output */
    return neuron_values + fann_layers[NUM_LAYERS - 1].first_neuron;
}
