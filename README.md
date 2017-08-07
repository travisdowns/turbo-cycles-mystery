Code the question of why there appear to be missed REF_TSC cycles when TurboBoost is enabled, 
as described on this [Stack Overflow question](https://stackoverflow.com/q/45472147/149138).

# Clone, Build and Install Kernel Module

```
git clone --recursive https://github.com/travisdowns/turbo-cycles-mystery
cd turbo-cycles-mystery
make
sudo insmod libpfc/pfc.ko
```

# Run
```
make run
```

This will prompt your for root credentials to install the `pfc.ko` kernel module, as well as enable userland
`rdpmc` calls. You can inpsect the makefile for details.

# Typical Output

## TurboBoost OFF

```
Pinned to CPU 0
CPU# REF_TSC   rdtsc Eff Mhz
   0 2589.52 2591.93 2589.07
   0 2590.80 2591.94 2590.81
   0 2590.38 2591.94 2590.10
   0 2590.80 2591.94 2590.66
   0 2590.57 2591.94 2590.61
   0 2590.70 2591.94 2590.23
   0 2590.66 2591.94 2591.07
   0 2590.33 2591.94 2590.38
   0 2590.24 2591.94 2590.05
   0 2590.83 2591.94 2591.12
   0 2590.63 2591.94 2590.22
   0 2590.56 2591.94 2590.29
   0 2590.88 2591.94 2590.82
   0 2590.35 2591.94 2588.98
   0 2590.71 2591.94 2590.67
   ...
```

## TurboBoost ON

```
Pinned to CPU 0
CPU# REF_TSC   rdtsc Eff Mhz
   0 2451.80 2591.93 3238.33
   0 2452.46 2591.94 3240.19
   0 2478.53 2591.94 3303.37
   0 2482.48 2591.94 3299.19
   0 2461.07 2591.93 3274.17
   0 2445.60 2591.94 3247.31
   0 2452.25 2591.94 3259.14
   0 2460.98 2591.94 3252.05
   0 2491.09 2591.94 3221.34
   0 2347.04 2591.94 3110.03
   0 2296.06 2591.94 2976.21
   0 2289.50 2591.94 3015.82
   0 2289.61 2591.94 3032.19
   0 2278.37 2591.94 3014.59
   0 2285.70 2591.94 3032.06
```


