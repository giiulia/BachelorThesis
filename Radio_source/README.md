# Radio-224 Source
The interface of the multichannel analyzer with MAESTRO software allows a histogram in ".Chn" format to be obtained via PC; the histogram can be converted to an [.xy file](Radio_source/Data/radio_measures.xy) that stores in two columns:
- the central energy value of each bin,
- the counts related to that bin.

The histogram can be represented with ROOT libraries as executed in [hist_radio.cpp](Radio_source/hist_radio.cpp), the outcome is shown in the PDF [radio_real_spectrum.pdf](Radio_source/Graphs/radio_real_spectrum.pdf).

The acquisition of actual <sup>224</sup>Ra decays was only the beginning of my analysis, in [Radio_simulations](Radio_source/Radio_simulations) I stored all of my simulation tests used to study the reproducibility through simulations in Geant4 of the spectrum produced by the <sup>212</sup>Bi-><sup>212</sup>Po beta decay cascading with alpha decay towards stable lead. 
