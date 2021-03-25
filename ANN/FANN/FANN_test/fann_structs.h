//Copyright (c) 2018 ETH Zurich, Ferdinand von Hagen, Michele Magno, Lukas Cavigelli
#include "fann.h"

#ifndef FANN_FANN_STRUCTS_H_
#define FANN_FANN_STRUCTS_H_

enum fann_activationfunc_enum
{
    FANN_LINEAR = 0,
    FANN_THRESHOLD,
    FANN_THRESHOLD_SYMMETRIC,
    FANN_SIGMOID,
    FANN_SIGMOID_STEPWISE,
    FANN_SIGMOID_SYMMETRIC,
    FANN_SIGMOID_SYMMETRIC_STEPWISE,
    FANN_GAUSSIAN,
    FANN_GAUSSIAN_SYMMETRIC,
    /* Stepwise linear approximation to gaussian.
    * Faster than gaussian but a bit less precise.
    * NOT implemented yet.
    */
    FANN_GAUSSIAN_STEPWISE,
    FANN_ELLIOT,
    FANN_ELLIOT_SYMMETRIC,
    FANN_LINEAR_PIECE,
    FANN_LINEAR_PIECE_SYMMETRIC,
    FANN_SIN_SYMMETRIC,
    FANN_COS_SYMMETRIC,
    FANN_SIN,
    FANN_COS
};

/* Enum: fann_train_enum
    The Training algorithms used when training on <struct fann_train_data> with functions like
    <fann_train_on_data> or <fann_train_on_file>. The incremental training looks alters the weights
    after each time it is presented an input pattern, while batch only alters the weights once after
    it has been presented to all the patterns.

    FANN_TRAIN_INCREMENTAL -  Standard backpropagation algorithm, where the weights are
        updated after each training pattern. This means that the weights are updated many
        times during a single epoch. For this reason some problems, will train very fast with
        this algorithm, while other more advanced problems will not train very well.
    FANN_TRAIN_BATCH -  Standard backpropagation algorithm, where the weights are updated after
        calculating the mean square error for the whole training set. This means that the weights
        are only updated once during a epoch. For this reason some problems, will train slower with
        this algorithm. But since the mean square error is calculated more correctly than in
        incremental training, some problems will reach a better solutions with this algorithm.
    FANN_TRAIN_RPROP - A more advanced batch training algorithm which achieves good results
        for many problems. The RPROP training algorithm is adaptive, and does therefore not
        use the learning_rate. Some other parameters can however be set to change the way the
        RPROP algorithm works, but it is only recommended for users with insight in how the RPROP
        training algorithm works. The RPROP training algorithm is described by
        [Riedmiller and Braun, 1993], but the actual learning algorithm used here is the
        iRPROP- training algorithm which is described by [Igel and Husken, 2000] which
        is an variety of the standard RPROP training algorithm.
    FANN_TRAIN_QUICKPROP - A more advanced batch training algorithm which achieves good results
        for many problems. The quickprop training algorithm uses the learning_rate parameter
        along with other more advanced parameters, but it is only recommended to change these
        advanced parameters, for users with insight in how the quickprop training algorithm works.
        The quickprop training algorithm is described by [Fahlman, 1988].

    See also:
        <fann_set_training_algorithm>, <fann_get_training_algorithm>
*/
enum fann_train_enum
{
    FANN_TRAIN_INCREMENTAL = 0,
    FANN_TRAIN_BATCH,
    FANN_TRAIN_RPROP,
    FANN_TRAIN_QUICKPROP,
    FANN_TRAIN_SARPROP
};

/* Enum: fann_stopfunc_enum
    Stop criteria used during training.

    FANN_STOPFUNC_MSE - Stop criteria is Mean Square Error (MSE) value.
    FANN_STOPFUNC_BIT - Stop criteria is number of bits that fail. The number of bits; means the
        number of output neurons which differ more than the bit fail limit
        (see <fann_get_bit_fail_limit>, <fann_set_bit_fail_limit>).
        The bits are counted in all of the training data, so this number can be higher than
        the number of training data.

    See also:
        <fann_set_train_stop_function>, <fann_get_train_stop_function>
*/
enum fann_stopfunc_enum
{
    FANN_STOPFUNC_MSE = 0,
    FANN_STOPFUNC_BIT
};

/* Enum: fann_errorfunc_enum
    Error function used during training.

    FANN_ERRORFUNC_LINEAR - Standard linear error function.
    FANN_ERRORFUNC_TANH - Tanh error function, usually better
        but can require a lower learning rate. This error function agressively targets outputs that
        differ much from the desired, while not targetting outputs that only differ a little that much.
        This activation function is not recommended for cascade training and incremental training.

    See also:
        <fann_set_train_error_function>, <fann_get_train_error_function>
*/
enum fann_errorfunc_enum
{
    FANN_ERRORFUNC_LINEAR = 0,
    FANN_ERRORFUNC_TANH
};

enum fann_nettype_enum
{
    FANN_NETTYPE_LAYER = 0, /* Each layer only has connections to the next layer */
    FANN_NETTYPE_SHORTCUT /* Each layer has connections to all following layers */
};

/* Enum: fann_errno_enum
    Used to define error events on <struct fann> and <struct fann_train_data>.

    See also:
        <fann_get_errno>, <fann_reset_errno>, <fann_get_errstr>

    FANN_E_NO_ERROR - No error
    FANN_E_CANT_OPEN_CONFIG_R - Unable to open configuration file for reading
    FANN_E_CANT_OPEN_CONFIG_W - Unable to open configuration file for writing
    FANN_E_WRONG_CONFIG_VERSION - Wrong version of configuration file
    FANN_E_CANT_READ_CONFIG - Error reading info from configuration file
    FANN_E_CANT_READ_NEURON - Error reading neuron info from configuration file
    FANN_E_CANT_READ_CONNECTIONS - Error reading connections from configuration file
    FANN_E_WRONG_NUM_CONNECTIONS - Number of connections not equal to the number expected
    FANN_E_CANT_OPEN_TD_W - Unable to open train data file for writing
    FANN_E_CANT_OPEN_TD_R - Unable to open train data file for reading
    FANN_E_CANT_READ_TD - Error reading training data from file
    FANN_E_CANT_ALLOCATE_MEM - Unable to allocate memory
    FANN_E_CANT_TRAIN_ACTIVATION - Unable to train with the selected activation function
    FANN_E_CANT_USE_ACTIVATION - Unable to use the selected activation function
    FANN_E_TRAIN_DATA_MISMATCH - Irreconcilable differences between two <struct fann_train_data> structures
    FANN_E_CANT_USE_TRAIN_ALG - Unable to use the selected training algorithm
    FANN_E_TRAIN_DATA_SUBSET - Trying to take subset which is not within the training set
    FANN_E_INDEX_OUT_OF_BOUND - Index is out of bound
    FANN_E_SCALE_NOT_PRESENT - Scaling parameters not present
    FANN_E_INPUT_NO_MATCH - The number of input neurons in the ann and data don't match
    FANN_E_OUTPUT_NO_MATCH - The number of output neurons in the ann and data don't match
*/
enum fann_errno_enum
{
    FANN_E_NO_ERROR = 0,
    FANN_E_CANT_OPEN_CONFIG_R,
    FANN_E_CANT_OPEN_CONFIG_W,
    FANN_E_WRONG_CONFIG_VERSION,
    FANN_E_CANT_READ_CONFIG,
    FANN_E_CANT_READ_NEURON,
    FANN_E_CANT_READ_CONNECTIONS,
    FANN_E_WRONG_NUM_CONNECTIONS,
    FANN_E_CANT_OPEN_TD_W,
    FANN_E_CANT_OPEN_TD_R,
    FANN_E_CANT_READ_TD,
    FANN_E_CANT_ALLOCATE_MEM,
    FANN_E_CANT_TRAIN_ACTIVATION,
    FANN_E_CANT_USE_ACTIVATION,
    FANN_E_TRAIN_DATA_MISMATCH,
    FANN_E_CANT_USE_TRAIN_ALG,
    FANN_E_TRAIN_DATA_SUBSET,
    FANN_E_INDEX_OUT_OF_BOUND,
    FANN_E_SCALE_NOT_PRESENT,
    FANN_E_INPUT_NO_MATCH,
    FANN_E_OUTPUT_NO_MATCH
};
 
typedef struct
{
    int first_connection;
    int last_connection;
    fann_type activation_steepness;
	fann_type bias;
    enum fann_activationfunc_enum activation_function;
} fann_neuron;

typedef struct
{
    int first_neuron;
    int last_neuron;
} fann_layer;

#define fann_max(x, y) (((x) > (y)) ? (x) : (y))
#define fann_min(x, y) (((x) < (y)) ? (x) : (y))
#define fann_safe_free(x) {if(x) { free(x); x = NULL; }}
#define fann_clip(x, lo, hi) (((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi) : (x)))
#define fann_exp2(x) exp(0.69314718055994530942f*(x))
/*#define fann_clip(x, lo, hi) (x)*/

#define fann_rand(min_value, max_value) (((float)(min_value))+(((float)(max_value)-((float)(min_value)))*rand()/(RAND_MAX+1.0f)))

#ifdef FIXEDFANN

#define fann_mult(x,y) ((x*y) >> DECIMAL_POINT)
#define fann_div(x,y) (((x) << DECIMAL_POINT)/y)
#define fann_random_weight() (fann_type)(fann_rand(0,MULTIPLIER/10))
#define fann_random_bias_weight() (fann_type)(fann_rand((0-MULTIPLIER)/10,MULTIPLIER/10))

#else

#define fann_mult(x,y) (x*y)
#define fann_div(x,y) (x/y)
#define fann_random_weight() (fann_rand(-0.1f,0.1f))
#define fann_random_bias_weight() (fann_rand(-0.1f,0.1f))

#include <math.h>
//#include <arm_math.h>
/* Implementation of the activation functions
 */

/* stepwise linear functions used for some of the activation functions */

/* defines used for the stepwise linear functions */

#define fann_linear_func(v1, r1, v2, r2, sum) (((((r2)-(r1)) * ((sum)-(v1)))/((v2)-(v1))) + (r1))
#define fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6, min, max, sum) (sum < v5 ? (sum < v3 ? (sum < v2 ? (sum < v1 ? min : fann_linear_func(v1, r1, v2, r2, sum)) : fann_linear_func(v2, r2, v3, r3, sum)) : (sum < v4 ? fann_linear_func(v3, r3, v4, r4, sum) : fann_linear_func(v4, r4, v5, r5, sum))) : (sum < v6 ? fann_linear_func(v5, r5, v6, r6, sum) : max))

/* FANN_LINEAR */
/* #define fann_linear(steepness, sum) fann_mult(steepness, sum) */
#define fann_linear_derive(steepness, value) (steepness)

/* FANN_SIGMOID */
/* #define fann_sigmoid(steepness, sum) (1.0f/(1.0f + exp(-2.0f * steepness * sum))) */
#define fann_sigmoid_real(sum) (1.0f/(1.0f + exp(-2.0f * sum)))
#define fann_sigmoid_derive(steepness, value) (2.0f * steepness * value * (1.0f - value))

/* FANN_SIGMOID_SYMMETRIC */
/* #define fann_sigmoid_symmetric(steepness, sum) (2.0f/(1.0f + exp(-2.0f * steepness * sum)) - 1.0f) */
#define fann_sigmoid_symmetric_real(sum) (2.0f/(1.0f + exp(-2.0f * sum)) - 1.0f)
#define fann_sigmoid_symmetric_derive(steepness, value) steepness * (1.0f - (value*value))

/* FANN_GAUSSIAN */
/* #define fann_gaussian(steepness, sum) (exp(-sum * steepness * sum * steepness)) */
#define fann_gaussian_real(sum) (exp(-sum * sum))
#define fann_gaussian_derive(steepness, value, sum) (-2.0f * sum * value * steepness * steepness)

/* FANN_GAUSSIAN_SYMMETRIC */
/* #define fann_gaussian_symmetric(steepness, sum) ((exp(-sum * steepness * sum * steepness)*2.0)-1.0) */
#define fann_gaussian_symmetric_real(sum) ((exp(-sum * sum)*2.0f)-1.0f)
#define fann_gaussian_symmetric_derive(steepness, value, sum) (-2.0f * sum * (value+1.0f) * steepness * steepness)

/* FANN_ELLIOT */
/* #define fann_elliot(steepness, sum) (((sum * steepness) / 2.0f) / (1.0f + fann_abs(sum * steepness)) + 0.5f) */
#define fann_elliot_real(sum) (((sum) / 2.0f) / (1.0f + fann_abs(sum)) + 0.5f)
#define fann_elliot_derive(steepness, value, sum) (steepness * 1.0f / (2.0f * (1.0f + fann_abs(sum)) * (1.0f + fann_abs(sum))))

/* FANN_ELLIOT_SYMMETRIC */
/* #define fann_elliot_symmetric(steepness, sum) ((sum * steepness) / (1.0f + fann_abs(sum * steepness)))*/
#define fann_elliot_symmetric_real(sum) ((sum) / (1.0f + fann_abs(sum)))
#define fann_elliot_symmetric_derive(steepness, value, sum) (steepness * 1.0f / ((1.0f + fann_abs(sum)) * (1.0f + fann_abs(sum))))

/* FANN_SIN_SYMMETRIC */
#define fann_sin_symmetric_real(sum) (sin(sum))
#define fann_sin_symmetric_derive(steepness, sum) (steepness*cos(steepness*sum))

/* FANN_COS_SYMMETRIC */
#define fann_cos_symmetric_real(sum) (cos(sum))
#define fann_cos_symmetric_derive(steepness, sum) (steepness*-sin(steepness*sum))

/* FANN_SIN */
#define fann_sin_real(sum) (sin(sum)/2.0f+0.5f)
#define fann_sin_derive(steepness, sum) (steepness*cos(steepness*sum)/2.0f)

/* FANN_COS */
#define fann_cos_real(sum) (cos(sum)/2.0f+0.5f)
#define fann_cos_derive(steepness, sum) (steepness*-sin(steepness*sum)/2.0f)

#define fann_activation_switch(activation_function, value, result) \
switch(activation_function) \
{ \
    case FANN_LINEAR: \
        result = (fann_type)value; \
        break; \
    case FANN_LINEAR_PIECE: \
        result = (fann_type)((value < 0) ? 0 : (value > 1) ? 1 : value); \
        break; \
    case FANN_LINEAR_PIECE_SYMMETRIC: \
        result = (fann_type)((value < -1) ? -1 : (value > 1) ? 1 : value); \
        break; \
    case FANN_SIGMOID: \
        result = (fann_type)fann_sigmoid_real(value); \
        break; \
    case FANN_SIGMOID_SYMMETRIC: \
        result = (fann_type)fann_sigmoid_symmetric_real(value); \
        break; \
    case FANN_SIGMOID_SYMMETRIC_STEPWISE: \
        result = (fann_type)fann_stepwise(-2.64665293693542480469f, -1.47221934795379638672f, -0.549306154251098632812f, 0.549306154251098632812f, 1.47221934795379638672f, 2.64665293693542480469f, -0.990000009536743164062f, -0.899999976158142089844f, -0.50000000000000000000f, 0.500000000000000000000f, 0.899999976158142089844f, 0.990000009536743164062f, -1, 1, value); \
        break; \
    case FANN_SIGMOID_STEPWISE: \
        result = (fann_type)fann_stepwise(-2.64665246009826660156f, -1.47221946716308593750f, -0.549306154251098632812f, 0.549306154251098632812f, 1.47221934795379638672f, 2.64665293693542480469f, 0.00499999988824129104614f, 0.0500000007450580596924f, 0.250000000000000000000f, 0.750000000000000000000f, 0.949999988079071044922f, 0.995000004768371582031f, 0, 1, value); \
        break; \
    case FANN_THRESHOLD: \
        result = (fann_type)((value < 0) ? 0 : 1); \
        break; \
    case FANN_THRESHOLD_SYMMETRIC: \
        result = (fann_type)((value < 0) ? -1 : 1); \
        break; \
    case FANN_GAUSSIAN: \
        result = (fann_type)fann_gaussian_real(value); \
        break; \
    case FANN_GAUSSIAN_SYMMETRIC: \
        result = (fann_type)fann_gaussian_symmetric_real(value); \
        break; \
    case FANN_ELLIOT: \
        result = (fann_type)fann_elliot_real(value); \
        break; \
    case FANN_ELLIOT_SYMMETRIC: \
        result = (fann_type)fann_elliot_symmetric_real(value); \
        break; \
    case FANN_SIN_SYMMETRIC: \
        result = (fann_type)fann_sin_symmetric_real(value); \
        break; \
    case FANN_COS_SYMMETRIC: \
        result = (fann_type)fann_cos_symmetric_real(value); \
        break; \
    case FANN_SIN: \
        result = (fann_type)fann_sin_real(value); \
        break; \
    case FANN_COS: \
        result = (fann_type)fann_cos_real(value); \
        break; \
    case FANN_GAUSSIAN_STEPWISE: \
        result = 0; \
        break; \
}

#endif

#define fann_abs(value) (((value) > 0) ? (value) : -(value))

#define fann_linear_func(v1, r1, v2, r2, sum) (((((r2)-(r1)) * ((sum)-(v1)))/((v2)-(v1))) + (r1))
#define fann_stepwise(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6, min, max, sum) (sum < v5 ? (sum < v3 ? (sum < v2 ? (sum < v1 ? min : fann_linear_func(v1, r1, v2, r2, sum)) : fann_linear_func(v2, r2, v3, r3, sum)) : (sum < v4 ? fann_linear_func(v3, r3, v4, r4, sum) : fann_linear_func(v4, r4, v5, r5, sum))) : (sum < v6 ? fann_linear_func(v5, r5, v6, r6, sum) : max))
 
 
 #endif /* FANN_FANN_STRUCTS_H_ */
