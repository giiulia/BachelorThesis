# BachelorThesis
This work contributed to the developement of a custom vacuum chaber for high sensitivity **alpha spectroscopy** measures. The project was initiated by the section of INFN at Milan Bicocca. The process is well described in the [summary](Thesis_summary.pdf) provided both in italian and english.

- My activity was focused on the characterization of 3 out of the 4 **silicon surface barrier detectors** destined to be allocated in the chamber. Each detector has been studied thoroughly by measuring both leakage current and electric capacitance in typical conditions of use and by determining its best point of work in terms of energectic resolution.
- I tested the outcome of the system operating [two of the detectors](Detectors1+2) in parallel in the same reading chain.
- The hypotesis that the larger detector-source distance is better for spectroscopy measures was supported by the execution of [Monte Carlo simulations](MC_sim) in C++  which allowed the evaluation of energy loss in the detector's dead layer.
- Spectroscopy measures of a [<sup>224</sup>Ra source](Radio_source) were profoundly interesting for my study of characterization for determining the depth of the active layer of the detector in use since the chain of the beta decay 212Bi->212Po with the alpha decay <sup>212</sup>Po-><sup>208</sup>Pb generates a continous spectrum whose shape is determined by the depth of the active layer of the detector in use.

## Sources 
The resolving performance of each detector was evaluated referring to signals produced by two types of sources:
- a **<sup>241</sup>Am source**, with special interest to its main peak at 5486 keV,
- a **function generator** used to inject a square-wave signal into the first stage of the electronics, the resolution of its peak is affecetd exclusively by electronic noise.
  
## Experimental apparatus
The aforementioned signals were individually processed by an analog chain consisting of:
- a **peamplifier**, that channels all the current into a feedback circuit so as to return a voltage signal proportional to the charge produced in the detector,
- an **amplifier**, that shapes the signal into a Gaussian,
- a **multi-channel analyzer** which collects the amplitude of each signal and converts it to a discrete-channel system.
  
The interface of the multichannel analyzer with **MAESTRO software** via PC allows to obtain a histogram in ".Chn" format.

The energy spectra (real or simulated) were analyzed using a program called **Tasso**, wich, relying on the MINUIT minimization package, executes fits of peaks with symmetric or asymmetric Gaussians by returning centroid position, FWHM resolution, area and rate at the peak. The Tasso program requires a specific type of input binary file. Therefore, it is necessary to convert ".Chn" files output from MAESTRO with the command: **_chn2tas FileIn_** where FileIn should be reported without the ".Chn" extension.
