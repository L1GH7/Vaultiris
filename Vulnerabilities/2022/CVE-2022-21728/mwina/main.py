import os

import tensorflow as tf


@tf.function
def test():
    y = tf.raw_ops.ReverseSequence(
        input=['aaa', 'bbb'],
        seq_lengths=[1, 1, 1],
        seq_dim=-10,
        batch_dim=-10)
    return y


def test2():
    print(os.environ["Path"])


test2()
test()

