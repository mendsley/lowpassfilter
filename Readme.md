Simple low-pass filter
======================

Based on the transfer function

$$ H(s) = {1 \over { \pi s + 1 } } $$

Differential equation of the filter

$$ \tau {\Delta y \over \Delta t} + y = x $$

Solving for delta-y:

$$ \Delta y = {\Delta t \over \tau}(x - y) $$

Cuttoff frequency caluclation:

$$ \tau = {1 \over \text{cutoff\\_freq}} $$
