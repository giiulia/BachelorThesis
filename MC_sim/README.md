# Monte Carlo simulations

For the purpose of determining the most convenient **detector-source distance** for spectroscopy measures, performances of Detector1 were analyzed in two opposite geometric configurations within the spatial limits of the chamber:
- [near source](../Single_detectors/Detector1/Shaping_time/Near_source)
- [faraway source](../Single_detectors/Detector1/Shaping_time/Faraway_source)
  
Monte Carlo simulations of multiple rectilinear trajectories allowed me to easily calculate the **maximum path through the detector's dead layer** given a geometric configuration. In [paths.txt](MC_sim/paths.txt) I listed the maximum paths for every distance simulated and the associated vertical inclination of the trajectory. This data allowed me to determine the medium **energy loss** of an alpha particle in the energy range of my experiments.\
Each event in [MC simulations](MC_sim/sim_distances.cpp) is represented by a [C++ class](MC_sim/alpha_class.h) wich is constructed by the definition of two angular coordinates of the trajectory's line. The oriented line is constrained to pass through the origin point of the alpha particle whose polar coordinated are generated within the source disk.\
Checking whether the particle has reached the dead layer is critical before calculating the path taken in it. In [checks.h](MC_sim/checks.h) are listed the functions that execute this operations and other random generation functions.
