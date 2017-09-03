[image1]: ./p.png "P"
[image2]: ./d.png "D"

# Describe the effect each of the P, I, D components had in your implementation. 

## P - represents the speed of turning. Larger P - faster turns, more overshooting. Let,s see on the graph of first 1000 iterationf for p, p*1.1 and p*1.3:
![Different p][image1]


## I - coefficient for integral (==sum of cte), helps if we are always on the left or on the right from center. But if P is small and we are slow in turns than after few left turns we can have problems on the right turn.

## D - parametr for derivative, helps to go smoother without overshooting. On the first 1000 steps, larger P - smoother moving
![Different D][image2]

# Describe how the final hyperparameters were chosen.

First I've chosed parametrs manualy, than used twiddle. The goal was to use twiddle and obtain coefficients for larger trottle, but I gived up because it takes to much time) For this I've made running similator using ipynb and the idea was to find coefficients for 0.3, then using in for 0.4 find there etc. For details please see train.ipynb

# Video - please see best.mov . Best p for throttle = 0.3 is 0.3224544990939262, 0.00013517627894290305, 7.069833065826603 
