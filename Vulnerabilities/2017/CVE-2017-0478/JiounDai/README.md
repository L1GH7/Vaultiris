# CVE-2017-0478
poc of CVE-2017-0478

The file of FrameSequence_webp.cpp is not compiled  in AOSP currently,  so when you compile libframesequence.so  with webp, 
you need compile with FRAMESEQUENCE_INCLUDE_WEBP(like this:  FRAMESEQUENCE_INCLUDE_WEBP=true mm -B)
