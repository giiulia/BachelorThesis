# Radio-224 Source
This is a true example of the application of spectroscopy measurements. In this case, I acquired the spectrum of an implanted source of <sup>224</sup>Ra, assumed to have uniform distribution, and compared the real spectrum with the result of simulations based on the **Geant4 libraries**.\
Given the purpose of comparing multiple histograms I converted the histogram in [".Chn" format](/Radio_source/Data/radio_measures16-20_july.Chn) to a [.xy file](/Radio_source/Data/radio_measures.xy) that stores:
- the central energy value of each bin in the first column,
- the counts related to that bin in the second column.

By this means the histogram can be displayed with **ROOT libraries** as performed in [hist_radio.cpp](/Radio_source/hist_radio.cpp), the outcome is shown in the PDF [radio_real_spectrum.pdf](/Radio_source/Graphs/radio_real_spectrum.pdf).

Acquiring the real <sup>224</sup>Ra decays was only the beginning of my analysis, in [Radio_simulations](/Radio_source/Radio_simulations) I stored all my tests to study the **reproducibility** through simulations in Geant4 of the region of the spectrum produced by the **<sup>212</sup>Bi-><sup>212</sup>Po** beta decay cascading with alpha decay to stable lead (said region is focused in [radio_real_spectrum_zoom.pdf](/Radio_source/Graphs/radio_real_spectrum_zoom.pdf)). 
