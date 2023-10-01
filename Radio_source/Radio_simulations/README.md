# Radio simulations
The original purpose of running the simulations was to determine the most suitable depth of the detector's active layer to reproduce the spectrum shape of the beta+alpha coincidence. A rough range of depths between 250 &mu;m and 350 &mu;m was considered based on the capacity measures of the detector in use.\
Results of the [initial attempt](/Radio_source/Radio_simulations/Graphs/hist_radio_total.cpp) of reproducing the real interaction are shown in [radio_total_spectrum.pdf](/Radio_source/Radio_simulations/Graphs/radio_total_spectrum.pdf). Functions from the ROOT library [TH1.h](https://root.cern.ch/doc/master/classTH1.html) such as [TH1::Scale()](https://root-forum.cern.ch/t/th1-scale/7086) and [TH1::Rebin()](https://root.cern.ch/doc/master/classTH1.html#rebinning) were critical in normalizing the MC spectra to the real source activity, however, the alpha peak was not faithfully reproduced. Reasons for this included: 
- an improper way of describing the geomeitric configuration of the experiment setup and/or the deposition of radioactive material,
- inconsistencies in the models underlying the code simulating beta interactions with the detector material,
- an overestimation of the capacitance value, and thus an incorrect assessment of the depth range.

Being the issue related to each peak of the spectrum ([test1](/Radio_source/Radio_simulations/Graphs/peak_at_6288keV.pdf)) and each depth considerated, different geometric distributions of the source were taken in consideration:
- exponential distribution ([test2](/Radio_source/Radio_simulations/Graphs/exponential.pdf)),
- contamination of the detector's dead layer due to recoil following the decay of unstable nuclei, in this test the dead layer is supposed to have 30nm of absorbent material al 20nm of source ([test3](/Radio_source/Radio_simulations/Graphs/source_on_detector_20+30.pdf)).
