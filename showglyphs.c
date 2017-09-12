const char *fontmap[] = {//DO NOT ADD OR DELETE LINES!!!
"\\pdfminorversion=5\\pdfobjcompresslevel=1",
"\\pdfpageheight=297mm\\pdfpagewidth=210mm",
"\\hsize180mm\\vsize267mm\\hoffset-10.4mm\\voffset-10.4mm\\nopagenumbers", //A4paper
"\\font\\bigf=cmtt12 at 36pt",
"\\font\\smallf=cmtt8 at 6pt",
"\\dimen20=1cm\\font\\numf=cmtt8 at 7pt",
"\\def\\mone#1#2#3{\\vbox{\\hbox to \\dimen20{\\zvrule height14pt depth3pt\\midf\\hss#1\\hss}\\hbox{\\zvrule height6pt depth1pt}\\hbox to \\dimen20{\\zvrule height6pt depth1pt\\smallf\\hss#2\\hss}\\hbox to \\dimen20{\\zvrule height6pt depth2pt\\hss\\numf#3\\hss}}}",
"\\def\\mtwo#1#2#3{\\vbox{\\hbox to \\dimen20{\\zvrule height14pt depth3pt\\midf\\hss#1\\hss}\\hbox to \\dimen20{\\zvrule height6pt depth1pt\\smallf\\hss#2\\hss}\\hbox{\\zvrule height6pt depth1pt}\\hbox to \\dimen20{\\zvrule height6pt depth2pt\\hss\\numf#3\\hss}}}",
"\\def\\mthree#1{\\raise6pt\\hbox to 40pt{\\bigf#1\\hss}}",
"\\def\\mfour#1{\\raise6pt\\hbox to \\dimen20{\\hss\\bigf#1\\hss}}",
"\\def\\zvrule{\\vrule width0pt }",
"\\pdfmapline{+", //testfont-enc <testfont-enc.enc <testfont}
"\\font\\midf=", //testfont-enc at 14pt
"\\centerline{\\bf ", //testfont enc}
"\\bigskip",
"\\centerline{\\vbox{\\offinterlineskip\\halign{&#\\cr",
"\\mthree{}&\\mfour{0}&\\mfour{1}&\\mfour{2}&\\mfour{3}&",
"\\mfour{4}&\\mfour{5}&\\mfour{6}&\\mfour{7}&",
"\\mfour{8}&\\mfour{9}&\\mfour{A}&\\mfour{B}&",
"\\mfour{C}&\\mfour{D}&\\mfour{E}&\\mfour{F}\\cr",
"\\noalign{\\hrule}"};
