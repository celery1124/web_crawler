# web_crawler
multi-thread web crawler
compiled in x64 mode

usage:  
hw1.exe [thread number] [urlfile]  
eg: hw1.exe 5000 test_file\URL-input-100.txt  
  
trace example:  
Opened .\URL-input-1M.txt with size 66152005   
[  2]   5000 Q   49235 E   8033 H   5332 D  3795 I   599 R   246 C 11354 L   
        *** crawling @299.5 pps @30.5 Mbps   
[  4]   5000 Q   81285 E  12552 H  11882 D  8908 I  2279 R   810 C 37352 L   
        *** crawling @569.8 pps @42.8 Mbps   
[  6]   5000 Q  108408 E  16220 H  15492 D 11820 I  3682 R  1647 C 89243 L   
        *** crawling @613.7 pps @63.3 Mbps   
[  8]   5000 Q  136392 E  20354 H  19479 D 15121 I  5269 R  2699 C 157337 L   
        *** crawling @658.6 pps @86.3 Mbps   
[ 10]   5000 Q  159313 E  23937 H  22876 D 17883 I  6537 R  3701 C 237882 L   
        *** crawling @653.7 pps @96.6 Mbps   
[ 12]   5000 Q  188792 E  27566 H  26383 D 20733 I  7828 R  4716 C 309399 L   
        *** crawling @652.3 pps @103.5 Mbps   
[ 14]   5000 Q  218926 E  31736 H  30346 D 23920 I  9326 R  5856 C 378493 L   
        *** crawling @666.1 pps @110.4 Mbps   
[ 16]   5000 Q  247078 E  35771 H  34185 D 26941 I 10719 R  6837 C 445704 L   
        *** crawling @669.9 pps @113.1 Mbps   
[ 18]   5000 Q  274113 E  40330 H  38596 D 30459 I 12304 R  8136 C 521963 L   
        *** crawling @683.6 pps @118.8 Mbps   
[ 20]   5000 Q  295133 E  43973 H  42128 D 33211 I 13583 R  8998 C 579619 L   
        *** crawling @679.1 pps @118.8 Mbps   
[ 22]   5000 Q  315807 E  47438 H  45468 D 35784 I 14769 R  9967 C 655607 L   
        *** crawling @671.3 pps @120.7 Mbps   
[ 24]   5000 Q  340482 E  51597 H  49449 D 38867 I 16212 R 11030 C 727401 L   
        *** crawling @675.5 pps @122.7 Mbps   
[ 26]   5000 Q  368510 E  55615 H  53232 D 41769 I 17656 R 12005 C 788709 L   
        *** crawling @679.1 pps @122.9 Mbps   
[ 28]   5000 Q  408859 E  60455 H  57916 D 45319 I 19297 R 13217 C 863412 L   
        *** crawling @689.2 pps @124.8 Mbps   
[ 30]   5000 Q  438396 E  64334 H  61606 D 48089 I 20589 R 14084 C 924242 L   
        *** crawling @686.3 pps @124.7 Mbps   
[ 32]   5000 Q  477030 E  69543 H  66599 D 51789 I 22279 R 15348 C 1014081 L   
        *** crawling @696.2 pps @128.1 Mbps   
[ 34]   5000 Q  531264 E  75058 H  71838 D 55616 I 24086 R 16654 C 1100899 L   
        *** crawling @708.4 pps @130.6 Mbps   
[ 36]   5000 Q  555545 E  78192 H  74953 D 57815 I 24949 R 17312 C 1153644 L   
        *** crawling @693.0 pps @129.8 Mbps   
[ 38]   5000 Q  589287 E  83293 H  79812 D 60837 I 26445 R 18398 C 1225688 L   
        *** crawling @695.9 pps @129.9 Mbps   
[ 40]   5000 Q  631850 E  89147 H  84948 D 63434 I 27839 R 19344 C 1278273 L   
        *** crawling @696.0 pps @128.7 Mbps   
[ 42]   5000 Q  674354 E  95156 H  91219 D 66441 I 28981 R 20192 C 1356091 L   
        *** crawling @690.0 pps @128.9 Mbps   
[ 44]   5000 Q  716762 E 101400 H  96840 D 69134 I 30324 R 21206 C 1416685 L   
        *** crawling @689.2 pps @129.2 Mbps    
[ 46]   5000 Q  771067 E 109070 H 103949 D 72187 I 31756 R 22308 C 1487844 L   
        *** crawling @690.3 pps @130.0 Mbps   
[ 48]   5000 Q  820731 E 116400 H 111024 D 75214 I 33113 R 23304 C 1551426 L   
        *** crawling @689.9 pps @130.7 Mbps [ 50]   5000 Q  865488 E 122512 H 117459 D 77944 I 34149 R 23931 C 1609113 L   
        *** crawling @683.0 pps @130.2 Mbps   
[ 52]   5000 Q  904282 E 127895 H 122612 D 80250 I 35233 R 24619 C 1655717 L   
        *** crawling @677.6 pps @129.1 Mbps   
[ 54]   5000 Q  953787 E 134967 H 128285 D 82703 I 36832 R 25903 C 1735614 L   
        *** crawling @682.1 pps @131.1 Mbps   
[ 56]   4123 Q 1000004 E 139300 H 133567 D 85104 I 37841 R 26432 C 1782159 L   
        *** crawling @675.7 pps @129.4 Mbps   
[ 58]   2605 Q 1000004 E 139300 H 133576 D 85110 I 38234 R 27060 C 1829900 L   
        *** crawling @659.2 pps @128.0 Mbps   
[ 60]   1806 Q 1000004 E 139300 H 133577 D 85111 I 38425 R 27326 C 1853016 L   
        *** crawling @640.4 pps @125.3 Mbps   
[ 62]   1447 Q 1000004 E 139300 H 133578 D 85112 I 38457 R 27432 C 1859623 L   
        *** crawling @620.3 pps @121.8 Mbps   
[ 64]   1076 Q 1000004 E 139300 H 133580 D 85113 I 38480 R 27517 C 1867171 L   
        *** crawling @601.3 pps @118.5 Mbps   
[ 66]    859 Q 1000004 E 139300 H 133580 D 85113 I 38497 R 27553 C 1871309 L   
        *** crawling @583.3 pps @115.2 Mbps   
[ 68]    620 Q 1000004 E 139300 H 133580 D 85113 I 38501 R 27578 C 1872204 L   
        *** crawling @566.2 pps @111.9 Mbps   
[ 70]    464 Q 1000004 E 139300 H 133580 D 85113 I 38507 R 27601 C 1874097 L   
        *** crawling @550.1 pps @108.9 Mbps   
[ 72]    369 Q 1000004 E 139300 H 133580 D 85113 I 38507 R 27604 C 1874172 L   
        *** crawling @534.8 pps @105.9 Mbps   
[ 74]    212 Q 1000004 E 139300 H 133580 D 85113 I 38508 R 27607 C 1874607 L   
        *** crawling @520.4 pps @103.1 Mbps   
[ 76]     73 Q 1000004 E 139300 H 133580 D 85113 I 38508 R 27611 C 1875077 L   
        *** crawling @506.7 pps @100.4 Mbps   
[ 78]      8 Q 1000004 E 139300 H 133580 D 85113 I 38508 R 27613 C 1875213 L   
        *** crawling @493.7 pps @97.8 Mbps   
[ 80]      3 Q 1000004 E 139300 H 133580 D 85113 I 38508 R 27613 C 1875213 L   
        *** crawling @481.4 pps @95.4 Mbps   
[ 82]      1 Q 1000004 E 139300 H 133580 D 85113 I 38508 R 27613 C 1875213 L   
        *** crawling @469.6 pps @93.0 Mbps   
   
Extracted 1000004 URLs @ 12195/s   
Looked up 133580 DNS names @ 1629/s   
Downloaded 85113 robots @ 1037/s  
Crawled 38508 pages @ 469/s (953.00 MB)  
Parsed 1875213 links @ 22868/s  
HTTP codes: 2xx = 27613, 3xx = 4505, 4xx = 5727, 5xx = 205, other = 1  
