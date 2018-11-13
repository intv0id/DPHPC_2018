# Compiling instructions

``` bash
latex report.tex
bibtex report.tex
latex report.tex
latex report.tex

#Now you have a dvi file. Get a ps file as:
dvips -t letter -o report.ps -Pdf -G0 report.dvi

#Now you have a ps file. Get a pdf as:
ps2pdf report.ps
```
