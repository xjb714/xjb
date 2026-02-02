a simple demo for bench float/double to decimal and string algorithm.


## related algorithm

algorithm     | Description
--------------|-----------
[schubfach](https://github.com/abolz/Drachennest) | Raffaello Giulietti's algorithm.Origin source code from https://github.com/c4f7fcce9cb06515/Schubfach
schubfach_xjb  | It is improved by schubfach and has the same output result.
[ryu](https://github.com/ulfjack/ryu) | Ulf Adams's [Ryū algorithm](https://dl.acm.org/citation.cfm?id=3192369).
[dragonbox](https://github.com/jk-jeon/dragonbox) | Junekey Jeon's CPP implementation.
[fmt](https://github.com/fmtlib/fmt) | Victor Zverovich.https://github.com/fmtlib/fmt. version:12.1.0
[yy_double](https://github.com/ibireme/c_numconv_benchmark) | yy's algorithm ,  origin link : https://github.com/ibireme/c_numconv_benchmark/blob/master/vendor/yy_double/yy_double.c
[yy_json](https://github.com/ibireme/yyjson) | yy's algorithm. https://github.com/ibireme/yyjson
[teju](https://github.com/cassioneri/teju_jagua) | youtube  link : https://www.youtube.com/watch?v=fPZ1ZdA7Iwc . An academic paper will be written to provide proof of correctness.https://github.com/cassioneri/teju_jagua
[xjb](https://github.com/xjb714/xjb) | XiangJunBo's algorithm. https://github.com/xjb714/xjb
[schubfach_vitaut](https://github.com/vitaut/schubfach) | Vitaut's algorithm. https://github.com/vitaut/schubfach
[zmij](https://github.com/vitaut/zmij) | Vitaut's algorithm. https://github.com/vitaut/zmij
[jnum](https://github.com/lengjingzju/json/blob/master/jnum.c) | Jing Leng's algorithm. https://github.com/lengjingzju/json/blob/master/jnum.c


(1) run :  
makefile variable settings:
&emsp;    float/double to decimal : set variable BENCH_F_TO_STR = 0  
&emsp;    float/double to string  : set variable BENCH_F_TO_STR = 1  
&emsp;    benchmark double        : set variable BENCH_DOUBLE = 1  
&emsp;    benchmark float         : set variable BENCH_DOUBLE = 0  
compile and exec :  
&emsp;    gcc: `make g` or `make`
&emsp;    icpx:`make i`
&emsp;    clang:`make c`



<!-- (2) benchmark results :
``CPU : AMD R7 7840H , OS : ubuntu 24.04``
``compiler : (1)gcc13.3 (2)icpx 2025.0.4 (3)clang 18.1.3 ``

![alt text](image-1.png)


``CPU : Apple M1 , OS : MacOS 26.1``
``compiler : apple clang 17.0.0 ``
![alt text](image-2.png) -->


<!-- double:
![benchmark](double.png)

float:
![benchmark](float.png) -->
<!-- <div class="mw-heading mw-heading3"><h3 id="CPUs_with_AVX2">CPUs with AVX2</h3><span class="mw-editsection"><span class="mw-editsection-bracket">[</span><a href="/w/index.php?title=Advanced_Vector_Extensions&amp;action=edit&amp;section=8" title="Edit section: CPUs with AVX2"><span>edit</span></a><span class="mw-editsection-bracket">]</span></span></div>
<ul><li><a href="/wiki/Intel_Corporation" class="mw-redirect" title="Intel Corporation">Intel</a>
<ul><li><a href="/wiki/Haswell_(microarchitecture)" title="Haswell (microarchitecture)">Haswell</a> processors (Q2 2013) and newer, except models branded as Celeron and Pentium.</li>
<li>Celeron and Pentium branded processors starting with <a href="/wiki/Tiger_Lake" title="Tiger Lake">Tiger Lake</a> (Q3 2020) and newer.<sup id="cite_ref-9r8b9_10-1" class="reference"><a href="#cite_note-9r8b9-10"><span class="cite-bracket">[</span>10<span class="cite-bracket">]</span></a></sup></li></ul></li>
<li><a href="/wiki/Advanced_Micro_Devices" class="mw-redirect" title="Advanced Micro Devices">AMD</a>
<ul><li><a href="/wiki/Excavator_(microarchitecture)" title="Excavator (microarchitecture)">Excavator</a> processors (Q2 2015) and newer.</li></ul></li>
<li><a href="/wiki/VIA_Technologies" title="VIA Technologies">VIA</a>:
<ul><li>Nano QuadCore</li>
<li>Eden X4</li></ul></li></ul>

<div class="mw-heading mw-heading2"><h2 id="CPUs_with_AVX-512">CPUs with AVX-512</h2><span class="mw-editsection"><span class="mw-editsection-bracket">[</span><a href="/w/index.php?title=AVX-512&amp;action=edit&amp;section=46" title="Edit section: CPUs with AVX-512"><span>edit</span></a><span class="mw-editsection-bracket">]</span></span></div>
<ul><li><a href="/wiki/Intel_Corporation" class="mw-redirect" title="Intel Corporation">Intel</a>
<ul><li><a href="/wiki/Xeon_Phi#Knights_Landing" title="Xeon Phi">Knights Landing</a> (Xeon Phi x200):<sup id="cite_ref-reinders512_1-5" class="reference"><a href="#cite_note-reinders512-1"><span class="cite-bracket">[</span>1<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-17" class="reference"><a href="#cite_note-17"><span class="cite-bracket">[</span>17<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, ER, PF</li>
<li><a href="/wiki/Xeon_Phi#Knights_Mill" title="Xeon Phi">Knights Mill</a> (Xeon Phi x205):<sup id="cite_ref-iaiseaffpr_8-3" class="reference"><a href="#cite_note-iaiseaffpr-8"><span class="cite-bracket">[</span>8<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, ER, PF, 4FMAPS, 4VNNIW, VPOPCNTDQ</li>
<li><a href="/wiki/Skylake_(microarchitecture)#Skylake-SP_(14_nm)_Scalable_Performance" title="Skylake (microarchitecture)">Skylake-SP</a>, <a href="/wiki/Skylake_(microarchitecture)#Mainstream_desktop_processors" title="Skylake (microarchitecture)">Skylake-X</a>:<sup id="cite_ref-18" class="reference"><a href="#cite_note-18"><span class="cite-bracket">[</span>18<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-19" class="reference"><a href="#cite_note-19"><span class="cite-bracket">[</span>19<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-20" class="reference"><a href="#cite_note-20"><span class="cite-bracket">[</span>20<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW</li>
<li><a href="/wiki/Cannon_Lake_(microarchitecture)" class="mw-redirect" title="Cannon Lake (microarchitecture)">Cannon Lake</a>:<sup id="cite_ref-iaiseaffpr_8-4" class="reference"><a href="#cite_note-iaiseaffpr-8"><span class="cite-bracket">[</span>8<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI</li>
<li><a href="/wiki/Cascade_Lake_(microarchitecture)" class="mw-redirect" title="Cascade Lake (microarchitecture)">Cascade Lake</a>: AVX-512 F, CD, VL, DQ, BW, VNNI</li>
<li><a href="/wiki/Cooper_Lake_(microarchitecture)" class="mw-redirect" title="Cooper Lake (microarchitecture)">Cooper Lake</a>: AVX-512 F, CD, VL, DQ, BW, VNNI, BF16</li>
<li><a href="/wiki/Ice_Lake_(microarchitecture)" class="mw-redirect" title="Ice Lake (microarchitecture)">Ice Lake</a>,<sup id="cite_ref-iaiseaffpr_8-5" class="reference"><a href="#cite_note-iaiseaffpr-8"><span class="cite-bracket">[</span>8<span class="cite-bracket">]</span></a></sup> <a href="/wiki/Rocket_Lake" title="Rocket Lake">Rocket Lake</a>:<sup id="cite_ref-21" class="reference"><a href="#cite_note-21"><span class="cite-bracket">[</span>21<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-22" class="reference"><a href="#cite_note-22"><span class="cite-bracket">[</span>22<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI, VBMI2, VPOPCNTDQ, BITALG, VNNI, VPCLMULQDQ, GFNI, VAES</li>
<li><a href="/wiki/Tiger_Lake_(microarchitecture)" class="mw-redirect" title="Tiger Lake (microarchitecture)">Tiger Lake</a> (except Pentium and Celeron but some reviewer have the CPU-Z Screenshot of Celeron 6305 with AVX-512 support<sup id="cite_ref-23" class="reference"><a href="#cite_note-23"><span class="cite-bracket">[</span>23<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-24" class="reference"><a href="#cite_note-24"><span class="cite-bracket">[</span>24<span class="cite-bracket">]</span></a></sup>):<sup id="cite_ref-gcc_25-0" class="reference"><a href="#cite_note-gcc-25"><span class="cite-bracket">[</span>25<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI, VBMI2, VPOPCNTDQ, BITALG, VNNI, VPCLMULQDQ, GFNI, VAES, VP2INTERSECT</li>
<li><a href="/wiki/Alder_Lake" title="Alder Lake">Alder Lake</a> (never officially supported by Intel, completely removed in newer CPUs<sup class="plainlinks nourlexpansion citation" id="ref_adl-avx512-note"><a href="#endnote_adl-avx512-note">Note 1</a></sup>):<sup id="cite_ref-anandalderreview_26-0" class="reference"><a href="#cite_note-anandalderreview-26"><span class="cite-bracket">[</span>26<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-27" class="reference"><a href="#cite_note-27"><span class="cite-bracket">[</span>27<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI, VBMI2, VPOPCNTDQ, BITALG, VNNI, VPCLMULQDQ, GFNI, VAES, BF16, VP2INTERSECT, FP16</li>
<li><a href="/wiki/Sapphire_Rapids_(microprocessor)" class="mw-redirect" title="Sapphire Rapids (microprocessor)">Sapphire Rapids</a><sup id="cite_ref-28" class="reference"><a href="#cite_note-28"><span class="cite-bracket">[</span>28<span class="cite-bracket">]</span></a></sup> and later P-core-only Xeon processors: AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI, VBMI2, VPOPCNTDQ, BITALG, VNNI, VPCLMULQDQ, GFNI, VAES, BF16, FP16</li></ul></li>
<li><a href="/wiki/Centaur_Technology" title="Centaur Technology">Centaur Technology</a>
<ul><li>"CNS" core (8c/8t):<sup id="cite_ref-29" class="reference"><a href="#cite_note-29"><span class="cite-bracket">[</span>29<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-instlatx64_30-0" class="reference"><a href="#cite_note-instlatx64-30"><span class="cite-bracket">[</span>30<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI</li></ul></li>
<li><a href="/wiki/AMD" title="AMD">AMD</a>
<ul><li><a href="/wiki/Zen_4" title="Zen 4">Zen 4</a>:<sup id="cite_ref-31" class="reference"><a href="#cite_note-31"><span class="cite-bracket">[</span>31<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-Hagedoorn_32-0" class="reference"><a href="#cite_note-Hagedoorn-32"><span class="cite-bracket">[</span>32<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-33" class="reference"><a href="#cite_note-33"><span class="cite-bracket">[</span>33<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-34" class="reference"><a href="#cite_note-34"><span class="cite-bracket">[</span>34<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-35" class="reference"><a href="#cite_note-35"><span class="cite-bracket">[</span>35<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI, VBMI2, VPOPCNTDQ, BITALG, VNNI, VPCLMULQDQ, GFNI, VAES, BF16</li>
<li><a href="/wiki/Zen_5" title="Zen 5">Zen 5</a>:<sup id="cite_ref-36" class="reference"><a href="#cite_note-36"><span class="cite-bracket">[</span>36<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI, VBMI2, VPOPCNTDQ, BITALG, VNNI, VPCLMULQDQ, GFNI, VAES, BF16, VP2INTERSECT</li>
<li><a href="/wiki/Zen_6" title="Zen 6">Zen 6</a>:<sup id="cite_ref-37" class="reference"><a href="#cite_note-37"><span class="cite-bracket">[</span>37<span class="cite-bracket">]</span></a></sup> AVX-512 F, CD, VL, DQ, BW, IFMA, VBMI, VBMI2, VPOPCNTDQ, BITALG, VNNI, VPCLMULQDQ, GFNI, VAES, BF16, VP2INTERSECT, FP16, BMM</li></ul></li></ul>
<table class="wikitable">
<tbody><tr>
<th class="nowrap ts-vertical-header" style=""><div style=""><style data-mw-deduplicate="TemplateStyles:r1221560606">@supports(writing-mode:vertical-rl){.mw-parser-output .ts-vertical-header{line-height:1;max-width:1em;padding:0.4em;vertical-align:bottom;width:1em}html.client-js .mw-parser-output .sortable:not(.jquery-tablesorter) .ts-vertical-header:not(.unsortable),html.client-js .mw-parser-output .ts-vertical-header.headerSort{background-position:50%.4em;padding-right:0.4em;padding-top:21px}.mw-parser-output .ts-vertical-header.is-valign-top{vertical-align:top}.mw-parser-output .ts-vertical-header.is-valign-middle{vertical-align:middle}.mw-parser-output .ts-vertical-header.is-normal{font-weight:normal}.mw-parser-output .ts-vertical-header>*{display:inline-block;transform:rotate(180deg);writing-mode:vertical-rl}@supports(writing-mode:sideways-lr){.mw-parser-output .ts-vertical-header>*{transform:none;writing-mode:sideways-lr}}}</style><font _mstmutation="1">Subset</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">F</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">CD</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">ER</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">PF</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">4FMAPS</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">4VNNIW</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">VPOPCNTDQ</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">VL</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">DQ</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">BW</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">IFMA</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">VBMI</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">VNNI</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">BF16</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">VBMI2</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">BITALG</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">VPCLMULQDQ</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">GFNI</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">VAES</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">VP2INTERSECT</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">FP16</font></div>
</th>
<th class="nowrap ts-vertical-header" style=""><div style=""><link rel="mw-deduplicated-inline-style" href="mw-data:TemplateStyles:r1221560606"><font _mstmutation="1">BMM</font></div>
</th></tr>
<tr>
<td><a href="/wiki/Xeon_Phi#Knights_Landing" title="Xeon Phi">Knights&nbsp;Landing</a> (Xeon Phi x200, 2016)
</td>
<td colspan="2" rowspan="9" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="2" rowspan="2" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="18" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><font _mstmutation="1"><a href="/wiki/Xeon_Phi#Knights_Mill" title="Xeon Phi" _mstmutation="1">Knights Mill</a> <span class="nowrap" _mstmutation="1">(Xeon Phi x205, 2017)</span></font>
</td>
<td colspan="3" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="15" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><font _mstmutation="1"><a href="/wiki/Skylake_(microarchitecture)#Skylake-SP_(14_nm)_Scalable_Performance" title="Skylake (microarchitecture)" _mstmutation="1">Skylake-SP</a>, <span class="nowrap" _mstmutation="1"><a href="/wiki/Skylake_(microarchitecture)#Mainstream_desktop_processors" title="Skylake (microarchitecture)">Skylake-X</a> (2017)</span></font>
</td>
<td colspan="4" rowspan="12" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td>
<td rowspan="4" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td>
<td colspan="3" rowspan="4" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="12" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Cannon_Lake_(microarchitecture)" class="mw-redirect" title="Cannon Lake (microarchitecture)">Cannon Lake</a> (2018)
</td>
<td colspan="2" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="10" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><span class="nowrap"><a href="/wiki/Cascade_Lake_(microarchitecture)" class="mw-redirect" title="Cascade Lake (microarchitecture)">Cascade Lake</a> (2019)</span>
</td>
<td rowspan="2" colspan="2" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td>
<td rowspan="2" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="9" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Cooper_Lake_(microarchitecture)" class="mw-redirect" title="Cooper Lake (microarchitecture)">Cooper Lake</a> (2020)
</td>
<td style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="8" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Ice_Lake_(microarchitecture)" class="mw-redirect" title="Ice Lake (microarchitecture)">Ice Lake</a> (2019)
</td>
<td colspan="7" rowspan="3" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td rowspan="3" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td>
<td colspan="5" rowspan="3" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="3" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Tiger_Lake_(microarchitecture)" class="mw-redirect" title="Tiger Lake (microarchitecture)">Tiger Lake</a> (2020)
</td>
<td style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="2" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Rocket_Lake" title="Rocket Lake">Rocket Lake</a> (2021)
</td>
<td colspan="3" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Alder_Lake" title="Alder Lake">Alder Lake</a> (2021)
</td>
<td colspan="2" style="background:#FFB; color:black;vertical-align:middle;text-align:center;" class="table-partial"><font _mstmutation="1">Partial<sup class="plainlinks nourlexpansion citation" id="ref_adl-avx512-note" _mstmutation="1"><a href="#endnote_adl-avx512-note">Note&nbsp;1</a></sup></font>
</td>
<td colspan="15" style="background:#FFB; color:black;vertical-align:middle;text-align:center;" class="table-partial"><font _mstmutation="1">Partial<sup class="plainlinks nourlexpansion citation" id="ref_adl-avx512-note" _mstmutation="1"><a href="#endnote_adl-avx512-note">Note&nbsp;1</a></sup></font>
</td>
<td style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Zen_4" title="Zen 4">Zen 4</a> (2022)
</td>
<td colspan="2" rowspan="4" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="13" rowspan="3" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="3" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Sapphire_Rapids_(microprocessor)" class="mw-redirect" title="Sapphire Rapids (microprocessor)">Sapphire&nbsp;Rapids</a>&nbsp;(2023)
</td>
<td style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td>
<td style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Zen_5" title="Zen 5">Zen 5</a> (2024)
</td>
<td style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td>
<td colspan="2" style="background:#FFC7C7;color:black;vertical-align:middle;text-align:center;" class="table-no">No
</td></tr>
<tr>
<td><a href="/wiki/Zen_6" title="Zen 6">Zen 6</a> (TBD)
</td>
<td colspan="16" style="background:#9EFF9E;color:black;vertical-align:middle;text-align:center;" class="table-yes">Yes
</td></tr></tbody></table>
<p><style data-mw-deduplicate="TemplateStyles:r1041539562">.mw-parser-output .citation{word-wrap:break-word}.mw-parser-output .citation:target{background-color:rgba(0,127,255,0.133)}</style><font _mstmutation="1"><span class="citation wikicite" id="endnote_adl-avx512-note" _mstmutation="1"><b><a href="#ref_adl-avx512-note">^Note 1</a></b></span>&nbsp;: </font><font _mstmutation="1">Intel does not officially support AVX-512 family of instructions on the <a href="/wiki/Alder_Lake" title="Alder Lake" _mstmutation="1">Alder Lake</a> microprocessors. In early 2022, Intel began disabling in silicon (fusing off) AVX-512 in Alder Lake microprocessors to prevent customers from enabling AVX-512.<sup id="cite_ref-38" class="reference" _mstmutation="1"><a href="#cite_note-38"><span class="cite-bracket">[</span>38<span class="cite-bracket">]</span></a></sup>
In older Alder Lake family CPUs with some legacy combinations of BIOS and microcode revisions, it was possible to execute AVX-512 family instructions when disabling all the efficiency cores which do not contain the silicon for AVX-512.<sup id="cite_ref-39" class="reference" _mstmutation="1"><a href="#cite_note-39"><span class="cite-bracket">[</span>39<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-40" class="reference" _mstmutation="1"><a href="#cite_note-40"><span class="cite-bracket">[</span>40<span class="cite-bracket">]</span></a></sup><sup id="cite_ref-anandalderreview_26-1" class="reference" _mstmutation="1"><a href="#cite_note-anandalderreview-26"><span class="cite-bracket">[</span>26<span class="cite-bracket">]</span></a></sup></font>
</p> -->
<!-- author : Xiang Jun Bo (xjb) , China , email : 1302035400@qq.com -->
date : 2025.5.13