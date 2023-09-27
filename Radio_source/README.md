# Radio-224 Source
This is a proper example of applications for spectroscopy measures. In this case I acquired the spectrum of a <sup>224</sup>Ra implanted source.\
Here the [data](Radio_source/Data) are collected differently due to the need for comparisons between histograms. The histogram in ".Chn" format can be converted to a [.xy file](Radio_source/Data/radio_measures.xy) that stores:
- the central energy value of each bin in the first column,
- the counts related to that bin in the second column.

The histogram can be displayed with ROOT libraries as performed in [hist_radio.cpp](Radio_source/hist_radio.cpp), the outcome is shown in the PDF [radio_real_spectrum.pdf](Radio_source/Graphs/radio_real_spectrum.pdf).

The acquisition of actual <sup>224</sup>Ra decays was only the beginning of my analysis, in [Radio_simulations](Radio_source/Radio_simulations) I stored all of my simulation tests used to study the reproducibility through simulations in Geant4 of the region of the spectrum produced by the <sup>212</sup>Bi-><sup>212</sup>Po beta decay cascading with alpha decay towards stable lead (said region is focused in [radio_real_spectrum_zoom.pdf](Radio_source/Graphs/radio_real_spectrum_zoom.pdf)). 
