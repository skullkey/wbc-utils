==DEPENDENCIES==
1. To use, clone and make the excellent fft2txt utility from Michał Kazior https://github.com/kazikcz/ath9k-spectral-scan:

git clone https://github.com/kazikcz/ath9k-spectral-scan
Follow the instructions and dependencies to make

2. Install fbi
sudo apt-get install fbi

==What it does==
Generate a 2d histogram of frequency and signal strenth using the spectral scan utility for the ath9k_htc driver.

A 2d histogram is calculated using python (directly based on http://pelican.rsvs.ulaval.ca/mediawiki/index.php/Making_density_maps_using_Gnuplot)
  and a png is generated using gnuplot and displayed on the framebuffer

==Why==
Very useful for 'screening' frequencies in use before FPV



