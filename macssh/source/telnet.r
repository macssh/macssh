resource 'ALRT' (200, "Low Memory Alert", preload) {
	{40, 40, 147, 450},
	200,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, visible, sound1,
		/* [3] */
		OK, visible, sound1,
		/* [4] */
		OK, visible, sound1
	},
	101
};

resource 'ALRT' (259, "My IP Number ALRT") {
	{40, 40, 150, 300},
	259,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, silent,
		/* [2] */
		OK, visible, silent,
		/* [3] */
		OK, visible, silent,
		/* [4] */
		OK, visible, silent
	},
	alertPositionMainScreen
};

resource 'BNDL' (128) {
	'Ssh2',
	0,
	{	/* array TypeArray: 2 elements */
		/* [1] */
		'FREF',
		{	/* array IDArray: 5 elements */
			/* [1] */
			0, 128,
			/* [2] */
			1, 129,
			/* [3] */
			2, 130,
			/* [4] */
			3, 131,
			/* [5] */
			4, 132
		},
		/* [2] */
		'ICN#',
		{	/* array IDArray: 5 elements */
			/* [1] */
			0, 128,
			/* [2] */
			1, 130,
			/* [3] */
			2, 131,
			/* [4] */
			3, 132,
			/* [5] */
			4, 133
		}
	}
};

data 'CDEF' (1109, "Tab Panel mw68", purgeable) {
	$"600A 0000 4344 4546 0455 0000 4EFA 0016"            /* `¬..CDEF.U..N˙.. */
	$"41FA FFEE D1FC 0000 151C 2008 A055 C18C"            /* A˙ˇÓ—¸.... .†U¡å */
	$"4E75 4E75 4E56 FFF6 48E7 1F30 382E 0012"            /* NuNuNVˇˆHÁ.08... */
	$"246E 000E 262E 0008 7E00 4EBA FFD4 2640"            /* $n..&...~.N∫ˇ‘&@ */
	$"204A A069 1D40 FFF6 204A A029 2052 4AA8"            /*  J†i.@ˇˆ J†) RJ® */
	$"001C 6714 2052 2068 001C A069 1D40 FFF7"            /* ..g. R h..†i.@ˇ˜ */
	$"2052 2068 001C A029 486E FFF8 A874 206E"            /*  R h..†)Hnˇ¯®t n */
	$"FFF8 3A28 0044 3C28 004A 3004 0240 0001"            /* ˇ¯:(.D<(.J0..@.. */
	$"671A 554F 3EB8 0BA6 301F 3F00 A887 554F"            /* g.UO>∏.¶0.?.®áUO */
	$"3EB8 0BA8 301F 3F00 A88A 6014 3004 0240"            /* >∏.®0.?.®ä`.0..@ */
	$"0008 660C 3F3C 0003 A887 3F3C 0009 A88A"            /* ..f.?<..®á?<.∆®ä */
	$"302E 000C 0C40 000B 6200 00E6 D040 303B"            /* 0....@..b..Ê–@0; */
	$"0006 4EFB 0002 006C 008C 00BA 0018 004A"            /* ..N˚...l.å.∫...J */
	$"00DA 00DA 00DA 00DA 00DA 00D0 00D0 3F06"            /* .⁄.⁄.⁄.⁄.⁄.–.–?. */
	$"3F05 3F04 2F0A 4EBA 0102 2052 4AA8 001C"            /* ?.?./¬N∫.. RJ®.. */
	$"4FEF 000A 6700 00AA 2052 2068 001C A069"            /* OÔ.¬g..™ R h..†i */
	$"1D40 FFF7 2052 2068 001C A029 6000 0092"            /* .@ˇ˜ R h..†)`..í */
	$"2052 4AA8 001C 6700 0088 2052 2068 001C"            /*  RJ®..g..à R h.. */
	$"A02A 2052 2068 001C A023 2052 42A8 001C"            /* †* R h..†# RB®.. */
	$"606E 2052 4A28 0010 6766 2052 2068 0004"            /* `n RJ(..gf R h.. */
	$"4A28 006E 675A 3F04 2F0A 4EBA 0350 5C4F"            /* J(.ngZ?./¬N∫.P\O */
	$"604E 2003 7210 E2A0 3D40 FFFC 3D43 FFFE"            /* `N .r.‚†=@ˇ¸=Cˇ˛ */
	$"554F 2F2E FFFC 2052 4868 0008 A8AD 101F"            /* UO/.ˇ¸ RHh..®≠.. */
	$"672E 2F03 2F0A 4EBA 01E2 2E00 504F 6020"            /* g././¬N∫.‚..PO`  */
	$"2003 0280 7FFF FFFF 2040 4850 2052 4868"            /*  ..Ä.ˇˇˇ @HP RHh */
	$"0008 A8DF 600A 2F03 2052 4868 0008 A8DF"            /* ..®ﬂ`¬/. RHh..®ﬂ */
	$"3004 0240 0008 6608 3F05 A887 3F06 A88A"            /* 0..@..f.?.®á?.®ä */
	$"2052 4AA8 001C 670C 2052 2068 001C 102E"            /*  RJ®..g. R h.... */
	$"FFF7 A06A 204A 102E FFF6 A06A 200B C18C"            /* ˇ˜†j J..ˇˆ†j .¡å */
	$"2F47 003A 4CDF 0CF8 4E5E 205F 4FEF 000C"            /* /G.:Lﬂ.¯N^ _OÔ.. */
	$"4ED0 846D 6169 6E00 0000 4E56 FFF8 48E7"            /* N–Ñmain...NVˇ¯HÁ */
	$"1F30 266E 0008 701C A122 2448 200A 6700"            /* .0&n..p.°"$H ¬g. */
	$"0148 7601 600E 3003 5340 2052 D0C0 3183"            /* .Hv.`.0.S@ R–¿1É */
	$"0010 5243 0C43 0005 6DEC 082E 0001 000D"            /* ..RC.C..mÏ...... */
	$"6708 2053 3828 0016 6014 2053 3828 0026"            /* g. S8(..`. S8(.& */
	$"0C44 0002 6D06 0C44 0008 6F02 7804 7605"            /* .D..m..D..o.x.v. */
	$"C7C4 2053 4A68 0016 6E08 2053 3144 0016"            /* «ƒ SJh..n. S1D.. */
	$"600E 2053 B668 0016 6C06 2053 3143 0016"            /* `. S∂h..l. S1C.. */
	$"2053 B668 0012 6C08 2053 317C 0001 0012"            /*  S∂h..l. S1|.... */
	$"2053 3268 0016 2A09 2605 87C4 2005 81C4"            /*  S2h..*∆&.áƒ .Åƒ */
	$"4840 4A40 6702 5243 2053 2D68 0008 FFF8"            /* H@J@g.RC S-h..ˇ¯ */
	$"2D68 000C FFFC 3E2E FFFE 9E6E FFFA 3C2E"            /* -h..ˇ¸>.ˇ˛ûnˇ˙<. */
	$"FFFC 9C6E FFF8 2053 2A28 0024 7210 E2A5"            /* ˇ¸únˇ¯ S*(.$r.‚• */
	$"4A45 6D0C 3047 2008 81FC 0002 BA40 6F02"            /* JEm.0G .Å¸..∫@o. */
	$"7A00 3007 9045 3203 5341 C3FC 000A 9041"            /* z.0.êE2.SA√¸.¬êA */
	$"2052 3140 0006 2053 2252 3368 0014 0004"            /*  R1@.. S"R3h.... */
	$"2052 BC68 0004 6F0A 3006 D040 2052 3140"            /*  Rºh..o¬0.–@ R1@ */
	$"0004 3046 2008 81C3 2052 3140 0008 082E"            /* ..0F .Å√ R1@.... */
	$"0001 000D 6714 2052 317C 0001 000C 2053"            /* ....g. R1|.... S */
	$"2252 3368 0016 000E 600C 2052 3143 000C"            /* "R3h....`. R1C.. */
	$"2052 3144 000E 2053 317C 0001 0014 2052"            /*  R1D.. S1|.... R */
	$"4268 000A 2052 30AE 000E 2052 316E 0010"            /* Bh.¬ R0Æ.. R1n.. */
	$"0002 2053 214A 001C 4CDF 0CF8 4E5E 4E75"            /* .. S!J..Lﬂ.¯N^Nu */
	$"8664 6F49 6E69 7400 0000 4E56 FFE4 48E7"            /* ÜdoInit...NVˇ‰HÁ */
	$"1F30 266E 0008 262E 000C 2053 2468 001C"            /* .0&n..&... S$h.. */
	$"200A 6606 7000 6000 0112 2003 7210 E2A0"            /*  ¬f.p.`... .r.‚† */
	$"3D40 FFEC 3D43 FFEE 2052 3E28 0008 2052"            /* =@ˇÏ=CˇÓ R>(.. R */
	$"3D68 000C FFE8 2053 3D68 0016 FFE4 2053"            /* =h..ˇË S=h..ˇ‰ S */
	$"2D68 0008 FFF0 2D68 000C FFF4 2D68 0008"            /* -h..ˇ-h..ˇÙ-h.. */
	$"FFF8 2D68 000C FFFC 7801 6000 00C4 3004"            /* ˇ¯-h..ˇ¸x.`..ƒ0. */
	$"5340 2052 D0C0 3630 0010 3F03 2F0B 4EBA"            /* S@ R–¿60..?./.N∫ */
	$"0B7E 3D40 FFEA 3F03 2F0B 4EBA 0B22 3D40"            /* .~=@ˇÍ?./.N∫."=@ */
	$"FFE6 302E FFFA D06E FFEA 3D40 FFFE 302E"            /* ˇÊ0.ˇ˙–nˇÍ=@ˇ˛0. */
	$"FFFC 9047 3D40 FFF8 2D6E FFF8 FFF0 2D6E"            /* ˇ¸êG=@ˇ¯-nˇ¯ˇ-n */
	$"FFFC FFF4 7A01 4FEF 000C 605C 2052 3C03"            /* ˇ¸ˇÙz.OÔ..`\ R<. */
	$"5346 CDE8 000E DC45 BC6E FFE4 6E50 554F"            /* SFÕË..‹Eºnˇ‰nPUO */
	$"2F2E FFEC 486E FFF0 A8AD 101F 6728 554F"            /* /.ˇÏHnˇ®≠..g(UO */
	$"A973 101F 66F8 486E FFEC A972 554F 2F2E"            /* ©s..f¯HnˇÏ©rUO/. */
	$"FFEC 486E FFF0 A8AD 101F 6706 3046 2008"            /* ˇÏHnˇ®≠..g.0F . */
	$"6038 7000 6034 486E FFF0 302E FFEA 5340"            /* `8p.`4Hnˇ0.ˇÍS@ */
	$"3F00 4267 A8A8 5245 BA6E FFE6 6F9E 486E"            /* ?.Bg®®RE∫nˇÊoûHn */
	$"FFF8 3F3C 000A 3007 4440 3F00 A8A8 5244"            /* ˇ¯?<.¬0.D@?.®®RD */
	$"B86E FFE8 6F00 FF38 7000 4CDF 0CF8 4E5E"            /* ∏nˇËo.ˇ8p.Lﬂ.¯N^ */
	$"4E75 8664 6F54 6573 7400 0000 4E56 FFF0"            /* NuÜdoTest...NVˇ */
	$"48E7 1F30 266E 0008 2053 2468 001C 200A"            /* HÁ.0&n.. S$h.. ¬ */
	$"6700 0146 2053 0C28 00FF 0011 6700 013A"            /* g..F S.(.ˇ..g..: */
	$"2053 4A28 0011 6712 2053 7000 1028 0011"            /*  SJ(..g. Sp..(.. */
	$"2053 3140 0012 6000 0120 700E A122 2608"            /*  S1@..`.. p.°"&. */
	$"4A83 6700 0114 41FA 0122 2D48 FFF2 2043"            /* JÉg...A˙."-HˇÚ C */
	$"2050 208B 2043 2050 316E 000C 0004 2053"            /*  P ã C P1n.... S */
	$"2D68 0008 FFF8 2D68 000C FFFC 2052 302E"            /* -h..ˇ¯-h..ˇ¸ R0. */
	$"FFF8 D068 0004 3D40 FFFC 2043 2050 216E"            /* ˇ¯–h..=@ˇ¸ C P!n */
	$"FFF8 0006 216E FFFC 000A 594F A8D8 205F"            /* ˇ¯..!nˇ¸.¬YO®ÿ _ */
	$"2C08 2F06 A87A 594F A8D8 205F 2808 2F04"            /* ,./.®zYO®ÿ _(./. */
	$"486E FFF8 A8DF 2F06 2F04 2F04 A8E4 2052"            /* Hnˇ¯®ﬂ/././.®‰ R */
	$"2253 3E29 0012 5347 48C7 8FE8 000E 5247"            /* "S>)..SGH«èË..RG */
	$"2052 BE68 0010 6746 2052 317C 0001 001A"            /*  Ræh..gF R1|.... */
	$"2052 3147 0010 2052 3D68 000C FFF0 7A00"            /*  R1G.. R=h..ˇz. */
	$"3D7C 0001 FFF6 6018 5245 BA47 6602 5245"            /* =|..ˇˆ`.RE∫Gf.RE */
	$"2052 302E FFF6 D0C0 3185 0010 526E FFF6"            /*  R0.ˇˆ–¿1Ö..Rnˇˆ */
	$"302E FFF0 5340 B06E FFF6 6CDC 6006 2052"            /* 0.ˇS@∞nˇˆl‹`. R */
	$"4268 001A 4EBA 0C1E 0C40 0700 6D0E 2F04"            /* Bh..N∫...@..m./. */
	$"2F2E FFF2 2F03 42A7 ABCA 6012 42A7 2F03"            /* /.ˇÚ/.Bß´ `.Bß/. */
	$"2F2E FFF2 2F04 4EBA 0B06 4FEF 0010 2F06"            /* /.ˇÚ/.N∫..OÔ../. */
	$"A879 2F04 A8D9 2F06 A8D9 2043 A023 2053"            /* ®y/.®Ÿ/.®Ÿ C†# S */
	$"2252 3368 0012 000A 4CDF 0CF8 4E5E 4E75"            /* "R3h...¬Lﬂ.¯N^Nu */
	$"8664 6F44 7261 7700 0000 4E56 FF84 48E7"            /* ÜdoDraw...NVˇÑHÁ */
	$"1F30 362E 0012 7A00 422E FFD1 422E FFAF"            /* .06...z.B.ˇ—B.ˇØ */
	$"266E 0008 200B 670E 2053 2450 2053 3D68"            /* &n.. .g. S$P S=h */
	$"0004 FFA8 6004 6000 0716 2052 2668 001C"            /* ..ˇ®`.`... R&h.. */
	$"200B 6700 070A 486E FFAA A874 0C43 0001"            /*  .g..¬Hnˇ™®t.C.. */
	$"6F0E 206E FFAA 0828 0007 0006 6602 7601"            /* o. nˇ™.(....f.v. */
	$"2053 3D68 000A FFA6 2052 3D68 0012 FFD8"            /*  S=h.¬ˇ¶ R=h..ˇÿ */
	$"2052 3D68 0016 FFB2 2053 3D68 0008 FFB4"            /*  R=h..ˇ≤ S=h..ˇ¥ */
	$"2053 3D68 000C FFC8 2053 326E FFB2 2009"            /*  S=h..ˇ» S2nˇ≤ ∆ */
	$"81E8 000E B06E FFC8 6706 1D7C 0001 FFAF"            /* ÅË..∞nˇ»g..|..ˇØ */
	$"486E FFB8 A88B 302E FFBA D06E FFBE 3D40"            /* Hnˇ∏®ã0.ˇ∫–nˇæ=@ */
	$"FFB0 302E FFB8 D06E FFB0 3D40 FFA4 486E"            /* ˇ∞0.ˇ∏–nˇ∞=@ˇ§Hn */
	$"FF92 A898 2F3C 0001 0001 A89B 486C 0000"            /* ˇí®ò/<....®õHl.. */
	$"A89D 2052 2D68 0008 FFF8 2D68 000C FFFC"            /* ®ù R-h..ˇ¯-h..ˇ¸ */
	$"2D68 0008 FFF0 2D68 000C FFF4 2053 302E"            /* -h..ˇ-h..ˇÙ S0. */
	$"FFF0 D068 0004 3D40 FFF4 2D6E FFF0 FF84"            /* ˇ–h..=@ˇÙ-nˇˇÑ */
	$"2D6E FFF4 FF88 302E FFC8 5340 C1FC 000A"            /* -nˇÙˇà0.ˇ»S@¡¸.¬ */
	$"322E FFF6 9240 3D41 FFF6 0C43 0002 6F00"            /* 2.ˇˆí@=Aˇˆ.C..o. */
	$"0084 486E FFD2 486E FF8C 4EBA 091C 1F3C"            /* .ÑHnˇ“HnˇåN∫∆..< */
	$"0001 4267 2F0A 4EBA 0888 486E FFE2 4EBA"            /* ..Bg/¬N∫.àHnˇ‚N∫ */
	$"094E 4A00 4FEF 0014 6712 486E FFE2 AA14"            /* ∆NJ.OÔ..g.Hnˇ‚™. */
	$"486E FFDC 4EBA 0938 584F 6024 3D7C 8888"            /* Hnˇ‹N∫∆8XO`$=|àà */
	$"FFE6 3D7C 8888 FFE4 3D7C 8888 FFE2 3D7C"            /* ˇÊ=|ààˇ‰=|ààˇ‚=| */
	$"CCCC FFE0 3D7C CCCC FFDE 3D7C CCCC FFDC"            /* ÃÃˇ‡=|ÃÃˇﬁ=|ÃÃˇ‹ */
	$"7A01 1D7C 0001 FFD1 0C6E FFFF FFD2 6614"            /* z..|..ˇ—.nˇˇˇ“f. */
	$"0C6E FFFF FFD4 660C 0C6E FFFF FFD6 6604"            /* .nˇˇˇ‘f..nˇˇˇ÷f. */
	$"422E FFD1 7601 6000 0586 3003 5340 2053"            /* B.ˇ—v.`..Ü0.S@ S */
	$"D0C0 3D70 0010 FFCA 3F2E FFCA 2F0A 4EBA"            /* –¿=p..ˇ ?.ˇ /¬N∫ */
	$"078E 3D40 FFB6 302E FFFA D06E FFB6 3D40"            /* .é=@ˇ∂0.ˇ˙–nˇ∂=@ */
	$"FFFE 302E FFFC 906E FFB4 3D40 FFF8 3D6E"            /* ˇ˛0.ˇ¸ênˇ¥=@ˇ¯=n */
	$"FFFC FFDA 2053 302E FFF2 D068 0006 3D40"            /* ˇ¸ˇ⁄ S0.ˇÚ–h..=@ */
	$"FFCC 3E2E FFF6 9E40 2053 4A68 001A 5C4F"            /* ˇÃ>.ˇˆû@ SJh..\O */
	$"6722 4A2E FFAF 671C 2D6E FFF8 FFC0 2D6E"            /* g"J.ˇØg.-nˇ¯ˇ¿-n */
	$"FFFC FFC4 5940 3D40 FFC6 586E FFC2 486E"            /* ˇ¸ˇƒY@=@ˇ∆Xnˇ¬Hn */
	$"FFC0 A8A3 3F2E FFCA 2F0A 4EBA 06D2 3D40"            /* ˇ¿®£?.ˇ /¬N∫.“=@ */
	$"FFCE 7801 5C4F 6000 0306 3C2E FFCA 5346"            /* ˇŒx.\O`...<.ˇ SF */
	$"2053 CDE8 000E DC44 BC6E FFB2 6E00 02F8"            /*  SÕË..‹Dºnˇ≤n..¯ */
	$"B86E FFCE 6606 3D6E FFCC FFFE 4A05 6710"            /* ∏nˇŒf.=nˇÃˇ˛J.g. */
	$"1F3C 0001 3F3C 0002 2F0A 4EBA 0754 504F"            /* .<..?<../¬N∫.TPO */
	$"3F2E FFB0 3F2E FFA4 3F2E FFA6 3F06 2053"            /* ?.ˇ∞?.ˇ§?.ˇ¶?. S */
	$"1F28 001B 1F2E FFD1 486E FFF8 3F2E FFA8"            /* .(....ˇ—Hnˇ¯?.ˇ® */
	$"2F0A 4EBA 04D6 4A05 4FEF 0016 6700 01C4"            /* /¬N∫.÷J.OÔ..g..ƒ */
	$"4878 001E A862 302E FFFA 5240 3F00 302E"            /* Hx..®b0.ˇ˙R@?.0. */
	$"FFFC 5340 3F00 A893 BC6E FFD8 6608 2F3C"            /* ˇ¸S@?.®ìºnˇÿf./< */
	$"0001 0000 A894 302E FFFA 5240 3F00 302E"            /* ....®î0.ˇ˙R@?.0. */
	$"FFF8 5640 3F00 A891 302E FFFA 5640 3F00"            /* ˇ¯V@?.®ë0.ˇ˙V@?. */
	$"302E FFF8 5240 3F00 A891 302E FFFE 5B40"            /* 0.ˇ¯R@?.®ë0.ˇ˛[@ */
	$"3F00 302E FFF8 5240 3F00 A891 BC6E FFD8"            /* ?.0.ˇ¯R@?.®ëºnˇÿ */
	$"662C 2F3C 0001 0001 A894 302E FFFA 5440"            /* f,/<....®î0.ˇ˙T@ */
	$"3F00 302E FFF8 5440 3F00 A891 302E FFFA"            /* ?.0.ˇ¯T@?.®ë0.ˇ˙ */
	$"5440 3F00 302E FFFC 5240 3F00 A891 0C43"            /* T@?.0.ˇ¸R@?.®ë.C */
	$"0001 6600 0098 BC6E FFD8 6700 0090 4878"            /* ..f..òºnˇÿg..êHx */
	$"001E A862 3F2E FFFA 302E FFFC 5240 3F00"            /* ..®b?.ˇ˙0.ˇ¸R@?. */
	$"A893 0C44 0001 6608 4878 0003 A894 6008"            /* ®ì.D..f.Hx..®î`. */
	$"2F3C 0000 FFFF A894 B86E FFCE 661C 302E"            /* /<..ˇˇ®î∏nˇŒf.0. */
	$"FFFE 5740 3F00 302E FFFC 5240 3F00 A891"            /* ˇ˛W@?.0.ˇ¸R@?.®ë */
	$"2F3C 0000 FFFF A894 600E 3F2E FFFE 302E"            /* /<..ˇˇ®î`.?.ˇ˛0. */
	$"FFFC 5240 3F00 A891 2F3C 0001 0000 A894"            /* ˇ¸R@?.®ë/<....®î */
	$"0C44 0001 6614 302E FFFA 5640 3F00 302E"            /* .D..f.0.ˇ˙V@?.0. */
	$"FFFC 5440 3F00 A891 6012 302E FFFA 5540"            /* ˇ¸T@?.®ë`.0.ˇ˙U@ */
	$"3F00 302E FFFC 5440 3F00 A891 486E FFE2"            /* ?.0.ˇ¸T@?.®ëHnˇ‚ */
	$"AA14 302E FFFE 5940 3F00 302E FFF8 5240"            /* ™.0.ˇ˛Y@?.0.ˇ¯R@ */
	$"3F00 A893 302E FFFE 5540 3F00 302E FFF8"            /* ?.®ì0.ˇ˛U@?.0.ˇ¯ */
	$"5640 3F00 A891 BC6E FFD8 6640 302E FFFE"            /* V@?.®ëºnˇÿf@0.ˇ˛ */
	$"5540 3F00 3F2E FFFC A891 4A2E FFD1 670A"            /* U@?.?.ˇ¸®ëJ.ˇ—g¬ */
	$"2F3C 0001 FFFF A894 6008 2F3C 0000 FFFF"            /* /<..ˇˇ®î`./<..ˇˇ */
	$"A894 486E FFDC AA14 302E FFFE 5740 3F00"            /* ®îHnˇ‹™.0.ˇ˛W@?. */
	$"302E FFF8 5640 3F00 A891 6012 302E FFFE"            /* 0.ˇ¯V@?.®ë`.0.ˇ˛ */
	$"5540 3F00 302E FFFC 5340 3F00 A891 B86E"            /* U@?.0.ˇ¸S@?.®ë∏n */
	$"FFCE 660E 0C43 0001 6708 2F3C 0002 0000"            /* ˇŒf..C..g./<.... */
	$"A892 4A05 670E 1F3C 0001 4267 2F0A 4EBA"            /* ®íJ.g..<..Bg/¬N∫ */
	$"0550 504F 3F2E FFFA 3F2E FFFC A893 3F2E"            /* .PPO?.ˇ˙?.ˇ¸®ì?. */
	$"FFFA 302E FFF8 5640 3F00 A891 302E FFFA"            /* ˇ˙0.ˇ¯V@?.®ë0.ˇ˙ */
	$"5640 3F00 3F2E FFF8 A891 302E FFFE 5940"            /* V@?.?.ˇ¯®ë0.ˇ˛Y@ */
	$"3F00 3F2E FFF8 A891 302E FFFE 5340 3F00"            /* ?.?.ˇ¯®ë0.ˇ˛S@?. */
	$"302E FFF8 5640 3F00 A891 302E FFFE 5340"            /* 0.ˇ¯V@?.®ë0.ˇ˛S@ */
	$"3F00 3F2E FFFC A891 302E FFFE 5540 3F00"            /* ?.?.ˇ¸®ë0.ˇ˛U@?. */
	$"3F2E FFFC A893 BC6E FFD8 6712 0C43 0001"            /* ?.ˇ¸®ìºnˇÿg..C.. */
	$"660C 3F2E FFFA 3F2E FFFC A891 603E 0C43"            /* f.?.ˇ˙?.ˇ¸®ë`>.C */
	$"0001 6638 2D6E FFF8 FFE8 2D6E FFFC FFEC"            /* ..f8-nˇ¯ˇË-nˇ¸ˇÏ */
	$"526E FFEA 536E FFEE 302E FFEC 526E FFEC"            /* RnˇÍSnˇÓ0.ˇÏRnˇÏ */
	$"3D40 FFE8 4A05 670E 486E FFD2 AA14 546E"            /* =@ˇËJ.g.Hnˇ“™.Tn */
	$"FFEA 556E FFEE 486E FFE8 A8A3 486E FFF8"            /* ˇÍUnˇÓHnˇË®£Hnˇ¯ */
	$"302E FFB6 5340 3F00 4267 A8A8 5244 B86E"            /* 0.ˇ∂S@?.Bg®®RD∏n */
	$"FFCE 6F00 FCF6 4A05 670E 1F3C 0001 4267"            /* ˇŒo.¸ˆJ.g..<..Bg */
	$"2F0A 4EBA 046C 504F 0C43 0001 661A 382E"            /* /¬N∫.lPO.C..f.8. */
	$"FFF2 3F04 3F2E FFDA A893 3F04 302E FFF4"            /* ˇÚ?.?.ˇ⁄®ì?.0.ˇÙ */
	$"5340 3F00 A891 6006 78F6 D86E FFF6 3F04"            /* S@?.®ë`.xˆÿnˇˆ?. */
	$"302E FFF4 5340 3F00 A893 302E FFF6 5340"            /* 0.ˇÙS@?.®ì0.ˇˆS@ */
	$"3F00 302E FFF4 5340 3F00 A891 302E FFF6"            /* ?.0.ˇÙS@?.®ë0.ˇˆ */
	$"5340 3F00 3F2E FFDA A891 4A47 670A 3007"            /* S@?.?.ˇ⁄®ëJGg¬0. */
	$"4440 3F00 4267 A892 4A05 6700 014A 4878"            /* D@?.Bg®íJ.g..JHx */
	$"001E A862 2F3C 0002 0002 A89B 0C43 0001"            /* ..®b/<....®õ.C.. */
	$"6620 3004 5240 3F00 302E FFDA 5240 3F00"            /* f 0.R@?.0.ˇ⁄R@?. */
	$"A893 3004 5240 3F00 302E FFF4 5740 3F00"            /* ®ì0.R@?.0.ˇÙW@?. */
	$"A891 2F3C 0001 0001 A89B 4A47 6736 302E"            /* ®ë/<....®õJGg60. */
	$"FFF6 5740 3F00 302E FFDA 5240 3F00 A893"            /* ˇˆW@?.0.ˇ⁄R@?.®ì */
	$"3007 4440 5240 3F00 4267 A892 0C43 0001"            /* 0.D@R@?.Bg®í.C.. */
	$"6612 2F3C 0001 FFFF A894 3007 5340 3F00"            /* f./<..ˇˇ®î0.S@?. */
	$"4267 A892 486E FFDC AA14 2F3C 0001 0001"            /* Bg®íHnˇ‹™./<.... */
	$"A89B 302E FFF6 5740 3F00 302E FFF4 5740"            /* ®õ0.ˇˆW@?.0.ˇÙW@ */
	$"3F00 A893 0C43 0001 661A 3004 5640 3F00"            /* ?.®ì.C..f.0.V@?. */
	$"302E FFF4 5740 3F00 A891 2F3C 0001 FFFF"            /* 0.ˇÙW@?.®ë/<..ˇˇ */
	$"A894 600C 3F04 302E FFF4 5540 3F00 A893"            /* ®î`.?.0.ˇÙU@?.®ì */
	$"486E FFE2 AA14 302E FFF6 5540 3F00 302E"            /* Hnˇ‚™.0.ˇˆU@?.0. */
	$"FFF4 5540 3F00 A891 0C43 0001 6704 4A47"            /* ˇÙU@?.®ë.C..g.JG */
	$"672C 302E FFF6 5540 3F00 302E FFDA 5240"            /* g,0.ˇˆU@?.0.ˇ⁄R@ */
	$"3F00 A891 4A2E FFD1 670A 2F3C 0001 FFFF"            /* ?.®ëJ.ˇ—g¬/<..ˇˇ */
	$"A894 6024 2F3C 0000 FFFF A894 601A 302E"            /* ®î`$/<..ˇˇ®î`.0. */
	$"FFF6 5540 3F00 302E FFF8 5640 3F00 A891"            /* ˇˆU@?.0.ˇ¯V@?.®ë */
	$"2F3C 0000 FFFF A894 486E FFDC AA14 0C43"            /* /<..ˇˇ®îHnˇ‹™..C */
	$"0001 6612 302E FFF6 5740 3F00 302E FFF4"            /* ..f.0.ˇˆW@?.0.ˇÙ */
	$"5740 3F00 A891 486E FFF0 4878 000A A8A8"            /* W@?.®ëHnˇHx.¬®® */
	$"046E 000A FFF4 486E FFF8 4267 302E FFB4"            /* .n.¬ˇÙHnˇ¯Bg0.ˇ¥ */
	$"4440 3F00 A8A8 3D6E FFF2 FFFA 5243 B66E"            /* D@?.®®=nˇÚˇ˙RC∂n */
	$"FFC8 6F00 FA76 4A05 670E 486E FFD2 486E"            /* ˇ»o.˙vJ.g.Hnˇ“Hn */
	$"FF8C 4EBA 0326 504F 486E FF92 A899 4CDF"            /* ˇåN∫.&POHnˇí®ôLﬂ */
	$"0CF8 4E5E 205F 4FEF 000C 4ED0 8B44 5241"            /* .¯N^ _OÔ..N–ãDRA */
	$"5743 4F4E 5452 4F4C 0000 4E56 FEF0 48E7"            /* WCONTROL..NV˛HÁ */
	$"1C30 282E 0008 246E 000E 1A2E 0012 362E"            /* .0(...$n......6. */
	$"0016 3D7C 0001 FEF0 B66E 0018 6710 2044"            /* ..=|..˛∂n..g. D */
	$"2050 B668 0012 6706 4A2E 0014 6730 2D52"            /*  P∂h..g.J...g0-R */
	$"FEF8 2D6A 0004 FEFC 486E FEF8 2F3C 0002"            /* ˛¯-j..˛¸Hn˛¯/<.. */
	$"0002 A8A9 2044 2050 B668 0012 6606 5A6E"            /* ..®© D P∂h..f.Zn */
	$"FEFC 6004 546E FEFC 486E FEF8 A8A3 3F03"            /* ˛¸`.Tn˛¸Hn˛¯®£?. */
	$"486E FF00 2044 2050 4868 0028 4EBA 00D4"            /* Hnˇ. D PHh.(N∫.‘ */
	$"4A2E FF00 4FEF 000A 6700 00B4 594F A8D8"            /* J.ˇ.OÔ.¬g..¥YO®ÿ */
	$"205F 2648 2F0B A87A 4A05 6706 486E FEF2"            /*  _&H/.®zJ.g.Hn˛Ú */
	$"AA19 2044 2050 B668 0012 6616 082E 0000"            /* ™. D P∂h..f..... */
	$"000D 6708 3F3C 0004 A888 6006 3F3C 0001"            /* ..g.?<..®à`.?<.. */
	$"A888 362A 0004 382A 0004 9852 3004 906E"            /* ®à6*..8*..òR0.ên */
	$"001A 48C0 81FC 0002 9640 966E 001C 554F"            /* ..H¿Å¸..ñ@ñn..UO */
	$"486E FF00 A88C 301F 322A 0006 926A 0002"            /* Hnˇ.®å0.2*..íj.. */
	$"9240 48C1 83FC 0002 D26A 0002 3801 2F0A"            /* í@H¡É¸..“j..8./¬ */
	$"A87B 4A05 6720 4878 001E A862 3004 5240"            /* ®{J.g Hx..®b0.R@ */
	$"3F00 3003 5240 3F00 A893 486E FF00 A884"            /* ?.0.R@?.®ìHnˇ.®Ñ */
	$"486E FEF2 AA14 3F04 3F03 A893 486E FF00"            /* Hn˛Ú™.?.?.®ìHnˇ. */
	$"A884 4267 A888 2F0B A879 2F0B A8D9 4CDF"            /* ®ÑBg®à/.®y/.®ŸLﬂ */
	$"0C38 4E5E 4E75 8964 7261 7754 6974 6C65"            /* .8N^NuâdrawTitle */
	$"0000 2F05 226F 0008 206F 000C 7201 7400"            /* ../."o.. o..r.t. */
	$"7A01 4210 0C31 000D 1000 6604 5245 6010"            /* z.B..1....f.RE`. */
	$"BA6F 0010 660A 5242 1082 11B1 1000 2000"            /* ∫o..f¬RB.Ç.±.. . */
	$"5241 7000 1011 B240 6FDA 2A1F 4E75 8C67"            /* RAp...≤@o⁄*.Nuåg */
	$"6574 4E65 7874 5469 746C 6500 0000 2F0B"            /* etNextTitle.../. */
	$"266F 0008 342F 000C 2053 2268 001C 2009"            /* &o..4/.. S"h.. ∆ */
	$"6606 7000 265F 4E75 2051 B468 000C 6614"            /* f.p.&_Nu Q¥h..f. */
	$"3002 5340 2051 C1E8 000E 2053 3228 0016"            /* 0.S@ Q¡Ë.. S2(.. */
	$"9240 6006 2051 3228 000E 3001 265F 4E75"            /* í@`. Q2(..0.&_Nu */
	$"8A67 6574 4D61 7843 6F6C 7300 0000 48E7"            /* ägetMaxCols...HÁ */
	$"0610 266F 0010 322F 0014 2053 2268 001C"            /* ..&o..2/.. S"h.. */
	$"2009 6604 7000 6044 2051 B268 000C 6614"            /*  ∆f.p.`D Q≤h..f. */
	$"3001 5340 2051 C1E8 000E 2053 3428 0016"            /* 0.S@ Q¡Ë.. S4(.. */
	$"9440 6006 2051 3428 000E 2051 3A28 0006"            /* î@`. Q4(.. Q:(.. */
	$"3045 2208 83C2 3001 5340 C1C2 3C05 9C40"            /* 0E".É¬0.S@¡¬<.ú@ */
	$"3046 2008 81C2 D041 3200 3001 4CDF 0860"            /* 0F .Å¬–A2.0.Lﬂ.` */
	$"4E75 8B67 6574 5461 6257 6964 7468 0000"            /* NuãgetTabWidth.. */
	$"4E56 FFFC 48E7 1020 554F 2F2E 0008 486E"            /* NVˇ¸HÁ. UO/...Hn */
	$"FFFC AA44 101F 4AAE FFFC 6754 206E FFFC"            /* ˇ¸™D..JÆˇ¸gT nˇ¸ */
	$"2050 2468 0008 7600 603E 3043 2008 E788"            /*  P$h..v.`>0C .Áà */
	$"2052 3030 0808 B06E 000C 662A 4A2E 000E"            /*  R00..∞n..f*J... */
	$"6712 3043 2008 E788 2052 D1C0 4868 000A"            /* g.0C .Áà R—¿Hh.¬ */
	$"AA14 601C 3043 2008 E788 2052 D1C0 4868"            /* ™.`.0C .Áà R—¿Hh */
	$"000A AA15 600A 5243 2052 B668 0006 6FBA"            /* .¬™.`¬RC R∂h..o∫ */
	$"4CDF 0408 4E5E 4E75 8C73 6574 5061 7274"            /* Lﬂ..N^NuåsetPart */
	$"436F 6C6F 7200 0000 4E56 0000 2F2E 0008"            /* Color...NV../... */
	$"AA19 2F2E 000C AA1A 4E5E 4E75 8A73 6176"            /* ™./...™.N^Nuäsav */
	$"6543 6F6C 6F72 7300 0000 4E56 0000 2F2E"            /* eColors...NV../. */
	$"0008 AA14 2F2E 000C AA15 4E5E 4E75 8D72"            /* ..™./...™.N^Nuçr */
	$"6573 746F 7265 436F 6C6F 7273 0000 4E56"            /* estoreColors..NV */
	$"FFF6 2F2E 0008 AA19 4EBA 013A 0C40 0700"            /* ˇˆ/...™.N∫.:.@.. */
	$"6D2C 486E FFFA AA1A 594F AA32 205F 2D48"            /* m,Hnˇ˙™.YO™2 _-H */
	$"FFF6 554F 2F2E FFF6 486E FFFA 2F2E 0008"            /* ˇˆUO/.ˇˆHnˇ˙/... */
	$"303C 0C19 AAA2 101F 6704 7001 6002 7000"            /* 0<..™¢..g.p.`.p. */
	$"4E5E 4E75 8767 6574 4772 6179 0000 4E56"            /* N^NuágetGray..NV */
	$"FFF0 48E7 1C30 2A2E 000C 262E 0010 95CA"            /* ˇHÁ.0*...&...ï  */
	$"2803 594F A8D8 205F 2648 2F0B A87A 3F3C"            /* (.YO®ÿ _&H/.®z?< */
	$"AA29 4EBA 011C 4A00 544F 6700 0094 2044"            /* ™)N∫..J.TOg..î D */
	$"2050 2D68 0006 FFF8 2D68 000A FFFC 486E"            /*  P-h..ˇ¯-h.¬ˇ¸Hn */
	$"FFF8 A870 486E FFFC A870 594F AA29 205F"            /* ˇ¯®pHnˇ¸®pYO™) _ */
	$"2448 6066 554F 486E FFF8 2052 4868 0022"            /* $H`fUOHnˇ¯ RHh." */
	$"486E FFF0 A8AA 101F 6746 554F 2F0A 3F3C"            /* Hnˇ®™..gFUO/¬?< */
	$"000D AA2C 101F 6738 554F 2F0A 3F3C 000F"            /* ..™,..g8UO/¬?<.. */
	$"AA2C 101F 672A 486E FFF0 A871 486E FFF4"            /* ™,..g*Hnˇ®qHnˇÙ */
	$"A871 486E FFF0 A87B 2052 2068 0016 2050"            /* ®qHnˇ®{ R h.. P */
	$"3828 0020 3F04 4267 2F0A 2F03 2045 4E90"            /* 8(. ?.Bg/¬/. ENê */
	$"594F 2F0A AA2B 205F 2448 200A 6696 6012"            /* YO/¬™+ _$H ¬fñ`. */
	$"2F2E 0008 A879 4878 0001 2F0A 2F03 2045"            /* /...®yHx../¬/. E */
	$"4E90 2F0B A879 2F0B A8D9 4CDF 0C38 4E5E"            /* Nê/.®y/.®ŸLﬂ.8N^ */
	$"4E75 8E73 7973 3644 6576 6963 654C 6F6F"            /* Nuésys6DeviceLoo */
	$"7000 0000 4E56 FFFA 2F03 363C 0600 3F3C"            /* p...NVˇ˙/.6<..?< */
	$"A1AD 4EBA 003C 4A00 544F 6720 554F 2F3C"            /* °≠N∫.<J.TOg UO/< */
	$"7379 7376 486E FFFC 4EBA 0100 301F 3D40"            /* sysvHnˇ¸N∫..0.=@ */
	$"FFFA 4A6E FFFA 6604 362E FFFE 3003 261F"            /* ˇ˙Jnˇ˙f.6.ˇ˛0.&. */
	$"4E5E 4E75 8967 6574 4F53 5665 7273 0000"            /* N^NuâgetOSVers.. */
	$"4E56 FFFC 48E7 1800 362E 0008 3F03 4EBA"            /* NVˇ¸HÁ..6...?.N∫ */
	$"005E 1800 0C04 0001 544F 6610 0243 07FF"            /* .^......TOf..C.ˇ */
	$"4EBA 006A B640 6D04 363C A89F 594F 3F3C"            /* N∫.j∂@m.6<®üYO?< */
	$"A89F 1F3C 0001 4EBA 02B0 205F 2D48 FFFC"            /* ®ü.<..N∫.∞ _-Hˇ¸ */
	$"594F 3F03 1F04 4EBA 02A0 205F B1EE FFFC"            /* YO?...N∫.† _±Óˇ¸ */
	$"56C0 4400 4880 4CDF 0018 4E5E 4E75 8D74"            /* V¿D.HÄLﬂ..N^Nuçt */
	$"7261 7041 7661 696C 6162 6C65 0000 026F"            /* rapAvailable...o */
	$"0800 0004 6704 7001 4E75 7000 4E75 8B67"            /* ....g.p.Nup.Nuãg */
	$"6574 5472 6170 5479 7065 0000 4E56 FFFC"            /* etTrapType..NVˇ¸ */
	$"594F 3F3C AA6E 1F3C 0001 4EBA 024C 205F"            /* YO?<™n.<..N∫.L _ */
	$"2D48 FFFC 594F 3F3C A86E 1F3C 0001 4EBA"            /* -Hˇ¸YO?<®n.<..N∫ */
	$"0238 205F B1EE FFFC 6606 303C 0200 6004"            /* .8 _±Óˇ¸f.0<..`. */
	$"303C 0400 4E5E 4E75 8F6E 756D 546F 6F6C"            /* 0<..N^NuènumTool */
	$"426F 7854 7261 7073 0000 4E56 0000 203C"            /* BoxTraps..NV.. < */
	$"0000 A89F A746 2F08 203C 0000 A0AD A346"            /* ..®üßF/. <..†≠£F */
	$"B1DF 670E 202E 000C A1AD 226E 0008 2288"            /* ±ﬂg. ...°≠"n.."à */
	$"6026 41FA 0036 303C EA51 222E 000C B298"            /* `&A˙.60<ÍQ"...≤ò */
	$"6706 4A98 6712 60F6 43FA 0020 D3D0 4ED1"            /* g.Jòg.`ˆC˙. ”–N— */
	$"226E 0008 2280 4240 3D40 0010 4E5E 205F"            /* "n.."ÄB@=@..N^ _ */
	$"508F 4ED0 303C EA52 60EE 7665 7273 0000"            /* PèN–0<ÍR`Óvers.. */
	$"0060 6D61 6368 0000 0064 7379 7376 0000"            /* .`mach...dsysv.. */
	$"0088 7072 6F63 0000 0092 6670 7520 0000"            /* .àproc...ífpu .. */
	$"009E 7164 2020 0000 00E8 6B62 6420 0000"            /* .ûqd  ...Ëkbd .. */
	$"011A 6174 6C6B 0000 0142 6D6D 7520 0000"            /* ..atlk...Bmmu .. */
	$"0164 7261 6D20 0000 0188 6C72 616D 0000"            /* .dram ...àlram.. */
	$"0188 0000 0000 0000 0000 7001 6082 2278"            /* .à........p.`Ç"x */
	$"02AE 7004 0C69 0075 0008 6712 0C69 0276"            /* .Æp..i.u..g..i.v */
	$"0008 6604 5240 6006 1038 0CB3 5C80 6000"            /* ..f.R@`..8.≥\Ä`. */
	$"FF60 7000 3038 015A 6000 FF56 7000 1038"            /* ˇ`p.08.Z`.ˇVp..8 */
	$"012F 5240 6000 FF4A 0C38 0004 012F 6738"            /* ./R@`.ˇJ.8.../g8 */
	$"0838 0004 0B22 6734 204F F280 0000 F327"            /* .8..."g4 OÚÄ..Û' */
	$"3017 2E48 0C40 1F18 6716 0C40 3F18 6710"            /* 0..H.@..g..@?.g. */
	$"0C40 3F38 670E 0C40 1F38 6708 7000 600E"            /* .@?8g..@.8g.p.`. */
	$"7001 600A 7002 6006 7003 6002 7000 6000"            /* p.`¬p.`.p.`.p.`. */
	$"FF00 0C78 3FFF 028E 6E1C 303C A89F A746"            /* ˇ..x?ˇ.én.0<®üßF */
	$"2408 203C 0000 AB03 A746 203C 0000 0100"            /* $. <..´.ßF <.... */
	$"B488 6606 600A 7000 6006 203C 0000 0200"            /* ¥àf.`¬p.`. <.... */
	$"6000 FECE 1038 021E 41FA 0016 2248 1218"            /* `.˛Œ.8..A˙.."H.. */
	$"6700 FED2 B200 66F6 91C9 2008 6000 FEB2"            /* g.˛“≤.fˆë… .`.˛≤ */
	$"0313 0B02 0106 0704 0508 0900 7000 4A38"            /* ..........∆.p.J8 */
	$"0291 6B16 1238 01FB 0201 000F 0C01 0001"            /* .ëk..8.˚........ */
	$"6608 2078 02DC 1028 0007 6000 FE84 0C38"            /* f. x.‹.(..`.˛Ñ.8 */
	$"0002 012F 6D16 7000 1038 0CB1 0C00 0001"            /* .../m.p..8.±.... */
	$"670C 0C00 0003 6D04 5340 6002 7000 6000"            /* g.....m.S@`.p.`. */
	$"FE60 303C A89F A746 2408 203C 0000 A88F"            /* ˛`0<®üßF$. <..®è */
	$"A746 2038 0108 B488 670A 598F 3F3C 0016"            /* ßF 8..¥àg¬Yè?<.. */
	$"A88F 201F 6000 FE3A 225F 121F 301F 4A01"            /* ®è .`.˛:"_..0.J. */
	$"6704 A746 6002 A346 2E88 4ED1 FFFF FFFF"            /* g.ßF`.£F.àN—ˇˇˇˇ */
	$"FFFF FFFF 0000 0067 8110 0000"                      /* ˇˇˇˇ...gÅ... */
};

resource 'CNTL' (130, "Small WFONT button") {
	{129, 306, 145, 381},
	0,
	visible,
	1,
	0,
	radioButProcUseWFont,
	1991,
	""
};

resource 'CNTL' (131, "Large WFONT Checkbox") {
	{254, 96, 270, 346},
	0,
	visible,
	1,
	0,
	checkBoxProcUseWFont,
	1991,
	""
};

resource 'CNTL' (132, "Small WFONT Checkbox") {
	{150, 95, 166, 239},
	0,
	visible,
	1,
	0,
	checkBoxProcUseWFont,
	1991,
	""
};

resource 'CNTL' (133, "Large WFONT button") {
	{254, 96, 270, 320},
	0,
	visible,
	1,
	0,
	radioButProcUseWFont,
	1991,
	""
};

resource 'CNTL' (134, "Small WFONT Checkbox Short") {
	{150, 95, 166, 175},
	0,
	visible,
	1,
	0,
	checkBoxProcUseWFont,
	1991,
	""
};

resource 'CNTL' (135, "Medium WFONT Checkbox") {
	{254, 96, 270, 246},
	0,
	visible,
	1,
	0,
	checkBoxProcUseWFont,
	1991,
	""
};

resource 'CNTL' (1000, "Tabs (Terminals)") {
	{4, 4, 20, 320},
	1,
	visible,
	4,
	210,
	17744,
	0,
	"Terminal\nKeyboard\nFont\nColor"
};

resource 'CNTL' (1001, "Tabs (Favorites)") {
	{4, 4, 20, 364},
	1,
	visible,
	7,
	214,
	17744,
	7,
	"General\nNetwork\nTerminal\nSecurity\nOTP\nSS"
	"H2\nFirewall"
};

resource 'CNTL' (2000, "Protocol Pop-up (Favorites)") {
	{0, 0, 20, 300},
	0,
	visible,
	100,
	2000,
	1017,
	0,
	"Protocol:"
};

resource 'CNTL' (2001, "SSH Pop-up (Favorites)") {
	{0, 0, 20, 232},
	0,
	visible,
	75,
	2001,
	1017,
	0,
	"Encryption:"
};

resource 'CNTL' (2002, "Keymappings Pop-Up") {
	{0, 0, 20, 300},
	0,
	visible,
	75,
	2002,
	1017,
	0,
	"Key Set:"
};

resource 'CNTL' (2003, "SSH Pop-up (Authentication)") {
	{0, 0, 20, 232},
	0,
	visible,
	75,
	2003,
	1017,
	0,
	"Authentication:"
};

resource 'CNTL' (2004, "SSH Pop-up (Compression)") {
	{0, 0, 20, 232},
	0,
	visible,
	75,
	2004,
	1017,
	0,
	"Compression:"
};

resource 'CNTL' (2005, "SSH Pop-up (Method)") {
	{0, 0, 20, 232},
	0,
	visible,
	75,
	2005,
	1017,
	0,
	"Method:"
};

resource 'CNTL' (2006, "Initialize SSH button") {
	{129, 306, 145, 402},
	0,
	visible,
	1,
	0,
	pushButProcUseWFont,
	1991,
	"Initialize SSH…"
};

resource 'CNTL' (2007, "Export public key") {
	{129, 306, 145, 402},
	0,
	visible,
	1,
	0,
	pushButProcUseWFont,
	1991,
	"Export public key…"
};

resource 'CNTL' (2008, "Port") {
	{0, 0, 20, 18},
	0,
	visible,
	0,
	2008,
	1017,
	0,
	""
};

resource 'CNTL' (2010, "SSH Pop-up (NIST)") {
	{76, 100, 96, 340},
	0,
	visible,
	80,
	2010,
	1017,
	0,
	"NIST Level:"
};

resource 'CNTL' (2011, "Emulation Type") {
	{76, 100, 96, 340},
	0,
	visible,
	80,
	2011,
	1017,
	0,
	"Emulation:"
};

resource 'CURS' (256, "CPos", purgeable, preload) {
	$"0000 0000 0FE0 0FE0 0FE0 0FE0 0FE0 0FE0"
	$"0FE0 0FE0 0FE0 0FE0 0FE0 0FE0",
	$"0000 0000 0FE0 0820 0820 0820 0820 0820"
	$"0820 0820 0820 0820 0820 0FE0",
	{7, 10}
};

resource 'CURS' (257, "Graphics", purgeable, preload) {
	$"0000 0080 0000 0080 0000 0080 0000 2AAA"
	$"0000 0080 0000 0080 0000 0080",
	$"0000 0100 0000 0100 0000 0100 0000 5554"
	$"0000 0100 0000 0100 0000 01",
	{7, 8}
};

resource 'CURS' (268, "Debug", purgeable, preload) {
	$"0000 0000 0000 0188 13D0 0BD0 07E0 0420"
	$"1E78 0660 0660 0C30 0BD0 1BD8 1008",
	$"0000 0000 0000 0188 13D0 0BD0 07E0 07E0"
	$"1FF8 07E0 07E0 0FF0 0BD0 1BD8 1008",
	{3, 7}
};

resource 'CURS' (290, "Xfer in progress", purgeable, preload) {
	$"0000 0000 0FC0 0820 0B90 0808 0B68 0808"
	$"0AC8 0808 0BA8 0808 0808 0FF8",
	$"0000 0000 0FC0 0FE0 0FF0 0FF8 0FF8 0FF8"
	$"0FF8 0FF8 0FF8 0FF8 0FF8 0FF8",
	{8, 6}
};

resource 'CURS' (10010) {
	$"07C0 1F30 3F08 7F04 7F04 FF02 FF02 FFFE"
	$"81FE 81FE 41FC 41FC 21F8 19F0 07C0",
	$"07C0 1FF0 3FF8 7FFC 7FFC FFFE FFFE FFFE"
	$"FFFE FFFE 7FFC 7FFC 3FF8 1FF0 07C0",
	{7, 7}
};

resource 'CURS' (10011) {
	$"07C0 1FF0 3FF8 5FF4 4FE4 87C2 8382 8102"
	$"8382 87C2 4FE4 5FF4 3FF8 1FF0 07C0",
	$"07C0 1FF0 3FF8 7FFC 7FFC FFFE FFFE FFFE"
	$"FFFE FFFE 7FFC 7FFC 3FF8 1FF0 07C0",
	{7, 7}
};

resource 'CURS' (10012) {
	$"07C0 19F0 21F8 41FC 41FC 81FE 81FE FFFE"
	$"FF02 FF02 7F04 7F04 3F08 1F30 07C0",
	$"07C0 1FF0 3FF8 7FFC 7FFC FFFE FFFE FFFE"
	$"FFFE FFFE 7FFC 7FFC 3FF8 1FF0 07C0",
	{7, 7}
};

resource 'CURS' (10013) {
	$"07C0 1830 2008 701C 783C FC7E FEFE FFFE"
	$"FEFE FC7E 783C 701C 2008 1830 07C0",
	$"07C0 1FF0 3FF8 7FFC 7FFC FFFE FFFE FFFE"
	$"FFFE FFFE 7FFC 7FFC 3FF8 1FF0 07C0",
	{7, 7}
};

resource 'DITL' (128, "Resolver Error", purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{118, 281, 138, 339},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{19, 35, 67, 330},
		StaticText {
			disabled,
			"Unable to open MacTCP's DNS resolver.  D"
			"omain names will not work."
		}
	}
};

resource 'DITL' (129, "FTP Info", purgeable) {
	{	/* array DITLarray: 7 elements */
		/* [1] */
		{109, 38, 129, 96},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{37, 77, 56, 277},
		EditText {
			disabled,
			"Edit Text"
		},
		/* [3] */
		{39, 5, 56, 58},
		StaticText {
			disabled,
			"name:"
		},
		/* [4] */
		{76, 81, 97, 282},
		EditText {
			disabled,
			"Edit Text"
		},
		/* [5] */
		{77, 3, 93, 78},
		StaticText {
			disabled,
			"password:"
		},
		/* [6] */
		{7, 28, 27, 283},
		StaticText {
			disabled,
			"Please Enter Name and Password"
		},
		/* [7] */
		{110, 150, 130, 208},
		Button {
			enabled,
			"Cancel"
		}
	}
};

resource 'DITL' (130, purgeable) {
	{	/* array DITLarray: 1 elements */
		/* [1] */
		{17, 44, 40, 180},
		StaticText {
			disabled,
			"Building font list..."
		}
	}
};

resource 'DITL' (132, purgeable) {
	{	/* array DITLarray: 0 elements */
	}
};

resource 'DITL' (150, "Edit Config Type", purgeable) {
	{	/* array DITLarray: 6 elements */
		/* [1] */
		{180, 218, 200, 298},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{68, 212, 88, 304},
		Button {
			enabled,
			"Remove \0x11R"
		},
		/* [3] */
		{40, 212, 60, 304},
		Button {
			enabled,
			"Edit \0x11E"
		},
		/* [4] */
		{12, 212, 32, 304},
		Button {
			enabled,
			"New \0x11N"
		},
		/* [5] */
		{12, 12, 203, 198},
		UserItem {
			disabled
		},
		/* [6] */
		{96, 212, 116, 304},
		Button {
			enabled,
			"Duplicate \0x11D"
		}
	}
};

resource 'DITL' (151, "Duplicate Name Dialog", purgeable) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{68, 256, 88, 324},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{12, 4, 44, 36},
		Icon {
			disabled,
			2
		},
		/* [3] */
		{12, 52, 48, 320},
		StaticText {
			disabled,
			"The alias name for the latest selection "
			"conflicts with an existing name."
		}
	}
};

resource 'DITL' (160, "FatalCancelAlert", purgeable) {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{116, 288, 136, 346},
		Button {
			enabled,
			""
		},
		/* [2] */
		{116, 222, 136, 280},
		Button {
			enabled,
			"Quit"
		},
		/* [3] */
		{4, 10, 36, 42},
		Icon {
			disabled,
			0
		},
		/* [4] */
		{4, 56, 108, 346},
		StaticText {
			disabled,
			"^0\nInternal error #^1\nMacOS error #^2"
		}
	}
};

resource 'DITL' (170, "FatalAlert", purgeable) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{116, 288, 136, 346},
		Button {
			enabled,
			"Quit"
		},
		/* [2] */
		{4, 10, 36, 42},
		Icon {
			disabled,
			0
		},
		/* [3] */
		{4, 56, 108, 346},
		StaticText {
			disabled,
			"^0\nInternal error #^1\nMacOS error #^2"
		}
	}
};

resource 'DITL' (175, "Operation Failed Alert", purgeable) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{116, 288, 136, 346},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{4, 10, 36, 42},
		Icon {
			disabled,
			0
		},
		/* [3] */
		{4, 56, 108, 346},
		StaticText {
			disabled,
			"That operation cannot be completed becau"
			"se ^0\nInternal error #^1\nMacOS error #^2"
		}
	}
};

resource 'DITL' (180, "AlertAskUser", purgeable) {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{94, 248, 114, 306},
		Button {
			enabled,
			"Cancel"
		},
		/* [2] */
		{94, 165, 114, 223},
		Button {
			enabled,
			"OK"
		},
		/* [3] */
		{11, 14, 43, 46},
		Icon {
			disabled,
			0
		},
		/* [4] */
		{11, 56, 86, 306},
		StaticText {
			disabled,
			"^0"
		}
	}
};

resource 'DITL' (190, "Change Window Title", purgeable) {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{46, 206, 66, 264},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{46, 135, 66, 193},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{16, 109, 31, 260},
		EditText {
			enabled,
			""
		},
		/* [4] */
		{15, 16, 32, 104},
		StaticText {
			disabled,
			"New Window Name"
		}
	}
};

resource 'DITL' (195, "DNR Error Alert", purgeable) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{116, 288, 136, 346},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{4, 10, 36, 42},
		Icon {
			disabled,
			0
		},
		/* [3] */
		{4, 56, 108, 346},
		StaticText {
			disabled,
			"The connection to ^0 cannot be opened be"
			"cause ^1\nInternal error #^2\nMacOS error "
			"#^3"
		}
	}
};

resource 'DITL' (200, "Low Memory Alert", preload) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{79, 339, 99, 397},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{16, 46, 72, 396},
		StaticText {
			disabled,
			"Yikes!  MacSSH is running out of memory."
			"  I suggest you quit now and increase Ma"
			"cSSH's default memory partition."
		},
		/* [3] */
		{15, 6, 47, 38},
		Icon {
			disabled,
			2
		}
	}
};

resource 'DITL' (259, "My IP Number", purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{10, 72, 60, 260},
		StaticText {
			disabled,
			"IP Address:\n^1"
		},
		/* [2] */
		{10, 20, 42, 52},
		Icon {
			disabled,
			1
		}
	}
};

resource 'DITL' (270, "Connection Status", purgeable) {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{70, 250, 90, 310},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{70, 180, 90, 240},
		Button {
			enabled,
			"Abort"
		},
		/* [3] */
		{10, 72, 60, 304},
		StaticText {
			enabled,
			"“^0” is currently ^1."
		},
		/* [4] */
		{10, 20, 42, 52},
		Icon {
			disabled,
			2
		}
	}
};

resource 'DITL' (273, "setup keys", purgeable) {
	{	/* array DITLarray: 9 elements */
		/* [1] */
		{142, 216, 162, 276},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{143, 124, 163, 184},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{14, 11, 36, 326},
		StaticText {
			enabled,
			"Enter keys to use.  Blank entries are di"
			"sabled. "
		},
		/* [4] */
		{100, 42, 118, 72},
		EditText {
			enabled,
			""
		},
		/* [5] */
		{100, 142, 118, 172},
		EditText {
			enabled,
			""
		},
		/* [6] */
		{100, 242, 118, 272},
		EditText {
			enabled,
			""
		},
		/* [7] */
		{45, 26, 80, 96},
		StaticText {
			enabled,
			"Interrupt\nProcess  "
		},
		/* [8] */
		{45, 128, 80, 198},
		StaticText {
			enabled,
			"Suspend\nOutput"
		},
		/* [9] */
		{45, 230, 80, 300},
		StaticText {
			enabled,
			"Resume\nOutput"
		}
	}
};

resource 'DITL' (280, "New Open Connection", purgeable, preload) {
	{	/* array DITLarray: 13 elements */
		/* [1] */
		{152, 260, 172, 320},
		Button {
			enabled,
			"Connect"
		},
		/* [2] */
		{152, 188, 172, 248},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{4, 12, 20, 88},
		StaticText {
			enabled,
			"Host Name:"
		},
		/* [4] */
		{28, 12, 44, 288},
		EditText {
			enabled,
			""
		},
		/* [5] */
		{56, 12, 72, 197},
		StaticText {
			enabled,
			"Window Name (optional):"
		},
		/* [6] */
		{80, 12, 96, 288},
		EditText {
			enabled,
			""
		},
		/* [7] */
		{26, 295, 46, 318},
		Picture {
			enabled,
			128
		},
		/* [8] */
		{103, 16, 123, 220},
		CheckBox {
			enabled,
			"Kerberos Authenticate (\0x11A)"
		},
		/* [9] */
		{122, 16, 142, 188},
		CheckBox {
			enabled,
			"Kerberos Encrypt (\0x11E)"
		},
		/* [10] */
		{4, 92, 20, 324},
		StaticText {
			enabled,
			""
		},
		/* [11] */
		{160, 16, 180, 168},
		CheckBox {
			enabled,
			"Secure Shell v2 (\0x11S)"
		},
		/* [12] */
		{141, 16, 161, 185},
		CheckBox {
			enabled,
			"Forward v5 Tickets(\0x11F)"
		},
		/* [13] */
		{0, 0, 0, 0},
		HelpItem {
			enabled,
			HMScanhdlg {
				280
			}
		}
	}
};

resource 'DITL' (281, "New Open Connection COPY", purgeable) {
	{	/* array DITLarray: 11 elements */
		/* [1] */
		{152, 231, 172, 291},
		Button {
			enabled,
			"Connect"
		},
		/* [2] */
		{152, 149, 172, 209},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{13, 7, 29, 111},
		StaticText {
			enabled,
			"Host name"
		},
		/* [4] */
		{13, 115, 29, 283},
		EditText {
			enabled,
			""
		},
		/* [5] */
		{45, 7, 61, 111},
		StaticText {
			enabled,
			"Window Name"
		},
		/* [6] */
		{45, 115, 61, 283},
		EditText {
			enabled,
			""
		},
		/* [7] */
		{69, 112, 89, 254},
		CheckBox {
			enabled,
			"FTP session (\0x11F)"
		},
		/* [8] */
		{96, 109, 111, 206},
		UserItem {
			disabled
		},
		/* [9] */
		{122, 109, 137, 206},
		UserItem {
			disabled
		},
		/* [10] */
		{96, 214, 116, 274},
		Button {
			enabled,
			"Edit"
		},
		/* [11] */
		{122, 214, 142, 274},
		Button {
			enabled,
			"Edit"
		}
	}
};

resource 'DITL' (290, "Other Font Size", purgeable) {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{68, 116, 88, 184},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{68, 32, 88, 96},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{20, 28, 36, 96},
		StaticText {
			disabled,
			"Font Size:"
		},
		/* [4] */
		{20, 108, 36, 148},
		EditText {
			enabled,
			""
		}
	}
};

resource 'DITL' (303, "Couldnt Connect", purgeable) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{5, 46, 25, 187},
		StaticText {
			enabled,
			"Couldn't connect to:"
		},
		/* [2] */
		{29, 46, 49, 188},
		StaticText {
			enabled,
			"^0"
		},
		/* [3] */
		{53, 2, 91, 231},
		StaticText {
			enabled,
			"^3"
		}
	}
};

resource 'DITL' (500, "Macros", purgeable) {
	{	/* array DITLarray: 27 elements */
		/* [1] */
		{260, 456, 280, 524},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{260, 200, 280, 264},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{54, 5, 72, 85},
		StaticText {
			enabled,
			""
		},
		/* [4] */
		{94, 5, 112, 85},
		StaticText {
			enabled,
			""
		},
		/* [5] */
		{134, 5, 152, 85},
		StaticText {
			enabled,
			""
		},
		/* [6] */
		{174, 5, 192, 85},
		StaticText {
			enabled,
			""
		},
		/* [7] */
		{214, 5, 232, 85},
		StaticText {
			enabled,
			""
		},
		/* [8] */
		{54, 260, 72, 340},
		StaticText {
			enabled,
			""
		},
		/* [9] */
		{94, 260, 112, 340},
		StaticText {
			enabled,
			""
		},
		/* [10] */
		{134, 260, 152, 340},
		StaticText {
			enabled,
			""
		},
		/* [11] */
		{174, 260, 192, 340},
		StaticText {
			enabled,
			""
		},
		/* [12] */
		{214, 260, 232, 340},
		StaticText {
			enabled,
			""
		},
		/* [13] */
		{44, 77, 76, 257},
		EditText {
			enabled,
			""
		},
		/* [14] */
		{84, 77, 116, 257},
		EditText {
			enabled,
			""
		},
		/* [15] */
		{124, 77, 156, 257},
		EditText {
			enabled,
			""
		},
		/* [16] */
		{164, 77, 196, 257},
		EditText {
			enabled,
			""
		},
		/* [17] */
		{204, 77, 236, 257},
		EditText {
			enabled,
			""
		},
		/* [18] */
		{44, 340, 76, 520},
		EditText {
			enabled,
			""
		},
		/* [19] */
		{84, 340, 116, 520},
		EditText {
			enabled,
			""
		},
		/* [20] */
		{124, 340, 156, 520},
		EditText {
			enabled,
			""
		},
		/* [21] */
		{164, 340, 196, 520},
		EditText {
			enabled,
			""
		},
		/* [22] */
		{204, 340, 236, 520},
		EditText {
			enabled,
			""
		},
		/* [23] */
		{260, 360, 280, 440},
		Button {
			enabled,
			"Import..."
		},
		/* [24] */
		{260, 272, 280, 352},
		Button {
			enabled,
			"Export..."
		},
		/* [25] */
		{260, 12, 280, 132},
		Button {
			enabled,
			"Save as Default"
		},
		/* [26] */
		{0, 0, 0, 0},
		HelpItem {
			enabled,
			HMScanhdlg {
				500
			}
		},
		/* [27] */
		{10, 5, 30, 305},
		Control {
			enabled,
			2002
		}
	}
};

resource 'DITL' (501, "flash.about", purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{0, 0, 250, 400},
		Picture {
			enabled,
			1025
		},
		/* [2] */
		{-2, 9, 18, 186},
		UserItem {
			enabled
		}
	}
};

resource 'DITL' (640, "Generic Errors", preload) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{80, 220, 100, 280},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{10, 72, 76, 287},
		StaticText {
			disabled,
			"^0\n^1"
		},
		/* [3] */
		{10, 20, 42, 52},
		Icon {
			disabled,
			0
		}
	}
};

resource 'DITL' (1001, "Color Selection", purgeable) {
	{	/* array DITLarray: 10 elements */
		/* [1] */
		{149, 154, 169, 214},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{149, 35, 169, 95},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{17, 134, 37, 206},
		UserItem {
			enabled
		},
		/* [4] */
		{49, 134, 69, 207},
		UserItem {
			enabled
		},
		/* [5] */
		{81, 134, 101, 207},
		UserItem {
			enabled
		},
		/* [6] */
		{113, 134, 133, 207},
		UserItem {
			enabled
		},
		/* [7] */
		{18, 29, 34, 132},
		StaticText {
			enabled,
			"Normal Text "
		},
		/* [8] */
		{51, 29, 67, 132},
		StaticText {
			enabled,
			"Normal Background"
		},
		/* [9] */
		{85, 29, 98, 132},
		StaticText {
			enabled,
			"Blinking Text"
		},
		/* [10] */
		{114, 29, 131, 132},
		StaticText {
			enabled,
			"Blinking Background"
		}
	}
};

resource 'DITL' (1002, "ANSI Color Selection", purgeable) {
	{	/* array DITLarray: 48 elements */
		/* [1] */
		{264, 174, 284, 234},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{264, 104, 284, 164},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{8, 108, 23, 140},
		StaticText {
			disabled,
			"Plain"
		},
		/* [4] */
		{8, 183, 23, 215},
		StaticText {
			disabled,
			"Bold"
		},
		/* [5] */
		{28, 8, 45, 71},
		StaticText {
			disabled,
			"Black"
		},
		/* [6] */
		{52, 8, 69, 71},
		StaticText {
			disabled,
			"Red"
		},
		/* [7] */
		{76, 8, 93, 71},
		StaticText {
			disabled,
			"Green"
		},
		/* [8] */
		{100, 8, 117, 71},
		StaticText {
			disabled,
			"Yellow"
		},
		/* [9] */
		{124, 8, 141, 71},
		StaticText {
			disabled,
			"Blue"
		},
		/* [10] */
		{148, 8, 165, 71},
		StaticText {
			disabled,
			"Magenta"
		},
		/* [11] */
		{172, 8, 189, 71},
		StaticText {
			disabled,
			"Cyan"
		},
		/* [12] */
		{196, 8, 213, 71},
		StaticText {
			disabled,
			"White"
		},
		/* [13] */
		{216, 12, 240, 251},
		StaticText {
			disabled,
			"Use the radio buttons to select color fo"
			"r boldfaced text. select default to use "
			"foreground color."
		},
		/* [14] */
		{243, 89, 258, 249},
		StaticText {
			disabled,
			"Default"
		},
		/* [15] */
		{26, 87, 46, 160},
		UserItem {
			enabled
		},
		/* [16] */
		{50, 87, 70, 160},
		UserItem {
			enabled
		},
		/* [17] */
		{74, 87, 94, 160},
		UserItem {
			enabled
		},
		/* [18] */
		{98, 87, 118, 160},
		UserItem {
			enabled
		},
		/* [19] */
		{122, 87, 142, 160},
		UserItem {
			enabled
		},
		/* [20] */
		{146, 87, 166, 160},
		UserItem {
			enabled
		},
		/* [21] */
		{170, 87, 190, 160},
		UserItem {
			enabled
		},
		/* [22] */
		{194, 87, 214, 160},
		UserItem {
			enabled
		},
		/* [23] */
		{26, 163, 46, 235},
		UserItem {
			enabled
		},
		/* [24] */
		{50, 163, 70, 235},
		UserItem {
			enabled
		},
		/* [25] */
		{74, 163, 94, 235},
		UserItem {
			enabled
		},
		/* [26] */
		{98, 163, 118, 235},
		UserItem {
			enabled
		},
		/* [27] */
		{122, 163, 142, 235},
		UserItem {
			enabled
		},
		/* [28] */
		{146, 163, 166, 235},
		UserItem {
			enabled
		},
		/* [29] */
		{170, 163, 190, 235},
		UserItem {
			enabled
		},
		/* [30] */
		{194, 163, 214, 235},
		UserItem {
			enabled
		},
		/* [31] */
		{28, 69, 46, 87},
		RadioButton {
			enabled,
			""
		},
		/* [32] */
		{52, 69, 70, 87},
		RadioButton {
			enabled,
			""
		},
		/* [33] */
		{76, 69, 94, 87},
		RadioButton {
			enabled,
			""
		},
		/* [34] */
		{100, 69, 118, 87},
		RadioButton {
			enabled,
			""
		},
		/* [35] */
		{124, 69, 142, 87},
		RadioButton {
			enabled,
			""
		},
		/* [36] */
		{148, 69, 166, 87},
		RadioButton {
			enabled,
			""
		},
		/* [37] */
		{172, 69, 190, 87},
		RadioButton {
			enabled,
			""
		},
		/* [38] */
		{196, 69, 214, 87},
		RadioButton {
			enabled,
			""
		},
		/* [39] */
		{28, 235, 46, 253},
		RadioButton {
			enabled,
			""
		},
		/* [40] */
		{52, 235, 70, 253},
		RadioButton {
			enabled,
			""
		},
		/* [41] */
		{76, 235, 94, 253},
		RadioButton {
			enabled,
			""
		},
		/* [42] */
		{100, 235, 118, 253},
		RadioButton {
			enabled,
			""
		},
		/* [43] */
		{124, 235, 142, 253},
		RadioButton {
			enabled,
			""
		},
		/* [44] */
		{148, 235, 166, 253},
		RadioButton {
			enabled,
			""
		},
		/* [45] */
		{172, 235, 190, 253},
		RadioButton {
			enabled,
			""
		},
		/* [46] */
		{196, 235, 214, 253},
		RadioButton {
			enabled,
			""
		},
		/* [47] */
		{240, 69, 258, 87},
		RadioButton {
			enabled,
			""
		},
		/* [48] */
		{0, 0, 0, 0},
		HelpItem {
			disabled,
			HMScanhdlg {
				1002
			}
		}
	}
};

resource 'DITL' (1023, "Opening Dialog", purgeable, preload) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{0, 0, 250, 400},
		Picture {
			enabled,
			1025
		},
		/* [2] */
		{204, 116, 224, 293},
		UserItem {
			enabled
		},
		/* [3] */
		{301, 1, 315, 182},
		StaticText {
			disabled,
			""
		}
	}
};

resource 'DITL' (2001, "Close Connection", purgeable, preload) {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{85, 320, 105, 380},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{85, 250, 105, 310},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{10, 72, 60, 380},
		StaticText {
			enabled,
			"Are you certain you want to close the co"
			"nnection to “^0”?"
		},
		/* [4] */
		{10, 20, 42, 52},
		Icon {
			disabled,
			2
		}
	}
};

resource 'DITL' (4001, "Set Dir", purgeable) {
	{	/* array DITLarray: 12 elements */
		/* [1] */
		{138, 257, 157, 335},
		Button {
			enabled,
			"Open"
		},
		/* [2] */
		{0, 699, 80, 717},
		Button {
			enabled,
			"Hidden"
		},
		/* [3] */
		{163, 256, 181, 336},
		Button {
			enabled,
			"Cancel"
		},
		/* [4] */
		{39, 232, 59, 347},
		UserItem {
			disabled
		},
		/* [5] */
		{68, 256, 86, 336},
		Button {
			enabled,
			"Eject"
		},
		/* [6] */
		{93, 256, 111, 336},
		Button {
			enabled,
			"Drive"
		},
		/* [7] */
		{39, 12, 185, 230},
		UserItem {
			enabled
		},
		/* [8] */
		{39, 229, 185, 246},
		UserItem {
			enabled
		},
		/* [9] */
		{124, 252, 125, 340},
		UserItem {
			disabled
		},
		/* [10] */
		{0, 660, 101, 756},
		StaticText {
			disabled,
			""
		},
		/* [11] */
		{193, 250, 211, 346},
		Button {
			enabled,
			"Set Directory"
		},
		/* [12] */
		{193, 13, 244, 232},
		StaticText {
			enabled,
			"“Set Directory” selects the directory in"
			"dicated in the popup menu above."
		}
	}
};

resource 'DITL' (5500, "Screen Size", purgeable) {
	{	/* array DITLarray: 7 elements */
		/* [1] */
		{112, 180, 132, 240},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{112, 100, 132, 160},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{71, 36, 91, 173},
		StaticText {
			enabled,
			"Number of Columns:"
		},
		/* [4] */
		{71, 176, 87, 205},
		EditText {
			enabled,
			"132"
		},
		/* [5] */
		{44, 36, 59, 170},
		StaticText {
			disabled,
			"Number of Lines:"
		},
		/* [6] */
		{44, 176, 60, 205},
		EditText {
			enabled,
			"24"
		},
		/* [7] */
		{12, 44, 29, 238},
		StaticText {
			disabled,
			"Select Screen dimensions:"
		}
	}
};

resource 'DITL' (7002, "FTP Config", purgeable) {
	{	/* array DITLarray: 20 elements */
		/* [1] */
		{237, 283, 257, 343},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{237, 14, 257, 74},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{43, 97, 59, 172},
		Control {
			enabled,
			130
		},
		/* [4] */
		{58, 97, 74, 172},
		Control {
			enabled,
			133
		},
		/* [5] */
		{73, 97, 89, 172},
		Control {
			enabled,
			133
		},
		/* [6] */
		{105, 36, 121, 317},
		Control {
			enabled,
			131
		},
		/* [7] */
		{90, 36, 106, 317},
		Control {
			enabled,
			131
		},
		/* [8] */
		{120, 36, 136, 317},
		Control {
			enabled,
			131
		},
		/* [9] */
		{135, 36, 151, 317},
		Control {
			enabled,
			131
		},
		/* [10] */
		{150, 36, 166, 317},
		Control {
			enabled,
			131
		},
		/* [11] */
		{174, 109, 190, 144},
		EditText {
			enabled,
			"WWWW"
		},
		/* [12] */
		{174, 198, 190, 233},
		EditText {
			enabled,
			"WWWW"
		},
		/* [13] */
		{172, 253, 192, 313},
		Button {
			enabled,
			"Example"
		},
		/* [14] */
		{206, 149, 222, 184},
		EditText {
			enabled,
			"WWWW"
		},
		/* [15] */
		{205, 196, 225, 322},
		Button {
			enabled,
			"Select Application"
		},
		/* [16] */
		{6, 134, 22, 218},
		StaticText {
			disabled,
			"FTP Server Prefs"
		},
		/* [17] */
		{44, 31, 60, 98},
		StaticText {
			disabled,
			"Server Mode"
		},
		/* [18] */
		{174, 34, 190, 106},
		StaticText {
			disabled,
			"Binary Filetype"
		},
		/* [19] */
		{174, 158, 190, 195},
		StaticText {
			disabled,
			"Creator"
		},
		/* [20] */
		{206, 81, 222, 146},
		StaticText {
			disabled,
			"Text Creator"
		}
	}
};

resource 'DITL' (7005, "FTP User Config", purgeable) {
	{	/* array DITLarray: 10 elements */
		/* [1] */
		{161, 170, 181, 228},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{161, 97, 181, 155},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{120, 13, 136, 294},
		Control {
			enabled,
			131
		},
		/* [4] */
		{10, 76, 25, 206},
		EditText {
			enabled,
			""
		},
		/* [5] */
		{39, 76, 55, 206},
		EditText {
			enabled,
			""
		},
		/* [6] */
		{62, 72, 93, 232},
		StaticText {
			disabled,
			""
		},
		/* [7] */
		{96, 36, 115, 213},
		Button {
			enabled,
			"Change Default Directory"
		},
		/* [8] */
		{10, 21, 26, 73},
		StaticText {
			disabled,
			"Username"
		},
		/* [9] */
		{39, 21, 55, 73},
		StaticText {
			disabled,
			"Password"
		},
		/* [10] */
		{62, 20, 87, 72},
		StaticText {
			disabled,
			"Default Directory"
		}
	}
};

resource 'DITL' (8000, "OTP Password Entry", purgeable) {
	{	/* array DITLarray: 12 elements */
		/* [1] */
		{167, 225, 187, 350},
		Button {
			enabled,
			"Compute & Send"
		},
		/* [2] */
		{167, 125, 187, 215},
		Button {
			enabled,
			"Compute"
		},
		/* [3] */
		{167, 45, 187, 115},
		Button {
			enabled,
			"Cancel"
		},
		/* [4] */
		{27, 15, 43, 315},
		EditText {
			enabled,
			""
		},
		/* [5] */
		{73, 15, 89, 315},
		EditText {
			enabled,
			""
		},
		/* [6] */
		{120, 15, 136, 315},
		EditText {
			enabled,
			""
		},
		/* [7] */
		{141, 106, 159, 212},
		RadioButton {
			enabled,
			"Words"
		},
		/* [8] */
		{141, 212, 159, 318},
		RadioButton {
			enabled,
			"Hex"
		},
		/* [9] */
		{5, 12, 21, 114},
		StaticText {
			disabled,
			"OTP Prompt:"
		},
		/* [10] */
		{51, 12, 67, 114},
		StaticText {
			disabled,
			"Password:"
		},
		/* [11] */
		{98, 12, 114, 114},
		StaticText {
			disabled,
			"Result:"
		},
		/* [12] */
		{142, 12, 158, 101},
		StaticText {
			disabled,
			"Send As:"
		}
	}
};

resource 'DITL' (10000, "Randomization", purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{5, 5, 71, 323},
		StaticText {
			enabled,
			"Please type 512 random characters now. T"
			"he characters and the intervals between "
			"them will be used to initialize the rand"
			"om number generator."
		},
		/* [2] */
		{93, 5, 111, 83},
		StaticText {
			disabled,
			"0"
		}
	}
};

resource 'DITL' (10001, "Password Prompt", purgeable) {
	{	/* array DITLarray: 6 elements */
		/* [1] */
		{100, 260, 120, 318},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{100, 184, 120, 242},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{60, 82, 76, 318},
		EditText {
			enabled,
			""
		},
		/* [4] */
		{13, 23, 45, 55},
		Icon {
			disabled,
			129
		},
		/* [5] */
		{13, 78, 45, 321},
		StaticText {
			disabled,
			"Please enter the password for this servi"
			"ce:"
		},
		/* [6] */
		{102, 48, 118, 172},
		CheckBox {
			enabled,
			"Add to Keychain"
		}
	}
};

resource 'DITL' (10002, "Host Key Unknown", purgeable) {
	{	/* array DITLarray: 7 elements */
		/* [1] */
		{140, 212, 160, 322},
		Button {
			enabled,
			"Accept & Save"
		},
		/* [2] */
		{140, 112, 160, 203},
		Button {
			enabled,
			"Accept Once"
		},
		/* [3] */
		{140, 16, 160, 74},
		Button {
			enabled,
			"Cancel"
		},
		/* [4] */
		{13, 23, 45, 55},
		Icon {
			disabled,
			2
		},
		/* [5] */
		{12, 80, 68, 328},
		StaticText {
			disabled,
			"The key for this host has never been see"
			"n before. Are you sure you want to trust"
			" it?"
		},
		/* [6] */
		{88, 80, 128, 324},
		StaticText {
			disabled,
			""
		},
		/* [7] */
		{68, 80, 84, 220},
		StaticText {
			disabled,
			"Host key fingerprint:"
		}
	}
};

resource 'DITL' (10003, "Host Key Changed", purgeable) {
	{	/* array DITLarray: 7 elements */
		/* [1] */
		{164, 212, 184, 322},
		Button {
			enabled,
			"Accept & Save"
		},
		/* [2] */
		{164, 112, 184, 203},
		Button {
			enabled,
			"Accept Once"
		},
		/* [3] */
		{164, 24, 184, 82},
		Button {
			enabled,
			"Cancel"
		},
		/* [4] */
		{13, 23, 45, 55},
		Icon {
			disabled,
			2
		},
		/* [5] */
		{8, 80, 92, 336},
		StaticText {
			disabled,
			"The key for this host does not match any"
			" of the keys saved from this host. YOU C"
			"OULD BE THE SUBJECT OF A MAN-IN-THE-MIDD"
			"LE ATTACK RIGHT NOW. Are you sure you wa"
			"nt to trust this key?"
		},
		/* [6] */
		{116, 80, 152, 324},
		StaticText {
			disabled,
			""
		},
		/* [7] */
		{96, 80, 112, 228},
		StaticText {
			disabled,
			"Host key fingerprint:"
		}
	}
};

resource 'DITL' (10004, "Login Prompt", purgeable) {
	{	/* array DITLarray: 9 elements */
		/* [1] */
		{108, 267, 128, 325},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{108, 192, 128, 250},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{72, 148, 88, 344},
		EditText {
			enabled,
			""
		},
		/* [4] */
		{40, 148, 56, 344},
		EditText {
			enabled,
			""
		},
		/* [5] */
		{12, 148, 32, 344},
		StaticText {
			disabled,
			"..."
		},
		/* [6] */
		{13, 23, 45, 55},
		Icon {
			disabled,
			129
		},
		/* [7] */
		{40, 60, 60, 140},
		StaticText {
			disabled,
			"User Name :"
		},
		/* [8] */
		{72, 66, 92, 138},
		StaticText {
			disabled,
			"Password :"
		},
		/* [9] */
		{12, 100, 32, 136},
		StaticText {
			disabled,
			"Host :"
		}
	}
};

resource 'DITL' (10010, "Generate Keys", purgeable) {
	{	/* array DITLarray: 13 elements */
		/* [1] */
		{164, 204, 184, 262},
		Button {
			enabled,
			"Create"
		},
		/* [2] */
		{164, 128, 184, 186},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{67, 20, 87, 220},
		Control {
			enabled,
			2010
		},
		/* [4] */
		{68, 20, 84, 172},
		StaticText {
			disabled,
			"Key Length (4096 max) :"
		},
		/* [5] */
		{13, 23, 45, 55},
		Icon {
			disabled,
			129
		},
		/* [6] */
		{12, 160, 28, 320},
		RadioButton {
			enabled,
			"DSA"
		},
		/* [7] */
		{32, 160, 48, 320},
		RadioButton {
			enabled,
			"RSA"
		},
		/* [8] */
		{12, 88, 32, 156},
		StaticText {
			disabled,
			"Key Type:"
		},
		/* [9] */
		{68, 176, 84, 216},
		EditText {
			enabled,
			"1024"
		},
		/* [10] */
		{68, 224, 84, 256},
		StaticText {
			disabled,
			"bits"
		},
		/* [11] */
		{128, 72, 144, 232},
		CheckBox {
			enabled,
			"Encrypt private key"
		},
		/* [12] */
		{100, 72, 116, 276},
		EditText {
			enabled,
			""
		},
		/* [13] */
		{100, 20, 120, 64},
		StaticText {
			disabled,
			"Label:"
		}
	}
};

resource 'DITL' (10011, "Generic Errors", preload) {
	{	/* array DITLarray: 3 elements */
		/* [1] */
		{104, 248, 124, 308},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{10, 72, 88, 348},
		StaticText {
			disabled,
			"^0\n^1"
		},
		/* [3] */
		{10, 20, 42, 52},
		Icon {
			disabled,
			0
		}
	}
};

resource 'DITL' (10012, "Generate Keys", purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{56, 84, 76, 142},
		Button {
			enabled,
			"Cancel"
		},
		/* [2] */
		{16, 16, 36, 220},
		StaticText {
			disabled,
			"Generating keys. Please wait…"
		}
	}
};

resource 'DITL' (7003, "Terminal Configuration", purgeable) {
	{	/* array DITLarray: 51 elements */
		/* [1] */
		{220, 262, 240, 320},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{220, 192, 240, 250},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{53, 161, 69, 220},
		UserItem {
			enabled
		},
		/* [4] */
		{74, 161, 90, 220},
		UserItem {
			enabled
		},
		/* [5] */
		{95, 161, 111, 220},
		UserItem {
			enabled
		},
		/* [6] */
		{117, 161, 133, 220},
		UserItem {
			enabled
		},
		/* [7] */
		{55, 11, 71, 161},
		StaticText {
			disabled,
			"Normal foreground color:"
		},
		/* [8] */
		{76, 11, 92, 161},
		StaticText {
			disabled,
			"Normal background color:"
		},
		/* [9] */
		{97, 11, 113, 161},
		StaticText {
			disabled,
			"Blinking foreground color:"
		},
		/* [10] */
		{119, 11, 135, 161},
		StaticText {
			disabled,
			"Blinking background color:"
		},
		/* [11] */
		{173, 11, 189, 116},
		StaticText {
			disabled,
			"Answerback Message:"
		},
		/* [12] */
		{173, 127, 186, 258},
		EditText {
			enabled,
			""
		},
		/* [13] */
		{80, 11, 96, 95},
		StaticText {
			disabled,
			"Screen Width:"
		},
		/* [14] */
		{80, 105, 93, 140},
		EditText {
			enabled,
			""
		},
		/* [15] */
		{105, 11, 124, 87},
		StaticText {
			disabled,
			"Screen Height:"
		},
		/* [16] */
		{105, 105, 118, 140},
		EditText {
			enabled,
			""
		},
		/* [17] */
		{30, 11, 46, 100},
		StaticText {
			disabled,
			"Terminal Name:"
		},
		/* [18] */
		{30, 105, 43, 310},
		EditText {
			enabled,
			""
		},
		/* [19] */
		{55, 11, 72, 67},
		StaticText {
			disabled,
			"Font:"
		},
		/* [20] */
		{55, 68, 72, 157},
		UserItem {
			enabled
		},
		/* [21] */
		{56, 167, 69, 193},
		EditText {
			enabled,
			""
		},
		/* [22] */
		{130, 11, 146, 86},
		StaticText {
			disabled,
			"Scrollback:"
		},
		/* [23] */
		{129, 105, 142, 150},
		EditText {
			enabled,
			""
		},
		/* [24] */
		{156, 12, 172, 134},
		Control {
			enabled,
			132
		},
		/* [25] */
		{75, 170, 91, 314},
		Control {
			enabled,
			132
		},
		/* [26] */
		{90, 170, 106, 314},
		Control {
			enabled,
			132
		},
		/* [27] */
		{55, 170, 71, 314},
		Control {
			enabled,
			132
		},
		/* [28] */
		{85, 170, 101, 314},
		Control {
			enabled,
			132
		},
		/* [29] */
		{105, 170, 121, 314},
		Control {
			enabled,
			132
		},
		/* [30] */
		{120, 170, 136, 314},
		Control {
			enabled,
			132
		},
		/* [31] */
		{70, 170, 86, 314},
		Control {
			enabled,
			132
		},
		/* [32] */
		{52, 20, 68, 256},
		Control {
			enabled,
			2011
		},
		/* [33] */
		{72, 71, 88, 146},
		Control {
			enabled,
			130
		},
		/* [34] */
		{91, 71, 107, 146},
		Control {
			enabled,
			130
		},
		/* [35] */
		{110, 71, 126, 146},
		Control {
			enabled,
			130
		},
		/* [36] */
		{55, 11, 73, 96},
		StaticText {
			disabled,
			"Emacs Meta Key:"
		},
		/* [37] */
		{100, 170, 116, 314},
		Control {
			enabled,
			132
		},
		/* [38] */
		{104, 12, 120, 156},
		Control {
			enabled,
			132
		},
		/* [39] */
		{120, 12, 136, 156},
		Control {
			enabled,
			132
		},
		/* [40] */
		{83, 11, 100, 67},
		StaticText {
			disabled,
			"Bold Font:"
		},
		/* [41] */
		{82, 68, 99, 157},
		UserItem {
			enabled
		},
		/* [42] */
		{136, 12, 152, 156},
		Control {
			enabled,
			132
		},
		/* [43] */
		{4, 4, 20, 320},
		Control {
			enabled,
			1000
		},
		/* [44] */
		{82, 166, 98, 310},
		Control {
			enabled,
			132
		},
		/* [45] */
		{192, 12, 208, 293},
		Control {
			enabled,
			131
		},
		/* [46] */
		{135, 170, 151, 314},
		Control {
			enabled,
			132
		},
		/* [47] */
		{176, 12, 192, 156},
		Control {
			enabled,
			132
		},
		/* [48] */
		{151, 170, 167, 314},
		Control {
			enabled,
			132
		},
		/* [49] */
		{129, 71, 145, 146},
		Control {
			enabled,
			130
		},
		/* [50] */
		{151, 12, 167, 156},
		Control {
			enabled,
			132
		},
		/* [51] */
		{0, 0, 0, 0},
		HelpItem {
			disabled,
			HMScanhdlg {
				7003
			}
		}
	}
};

resource 'DITL' (7001, "Main Prefs", purgeable) {
	{	/* array DITLarray: 43 elements */
		/* [1] */
		{314, 310, 334, 370},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{314, 240, 334, 300},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{141, 16, 157, 138},
		Control {
			enabled,
			132
		},
		/* [4] */
		{157, 16, 173, 138},
		Control {
			enabled,
			132
		},
		/* [5] */
		{115, 192, 131, 384},
		Control {
			enabled,
			131
		},
		/* [6] */
		{131, 192, 147, 384},
		Control {
			enabled,
			131
		},
		/* [7] */
		{14, 192, 30, 314},
		Control {
			enabled,
			132
		},
		/* [8] */
		{30, 192, 46, 314},
		Control {
			enabled,
			132
		},
		/* [9] */
		{46, 192, 62, 267},
		Control {
			enabled,
			130
		},
		/* [10] */
		{62, 192, 78, 267},
		Control {
			enabled,
			130
		},
		/* [11] */
		{78, 192, 94, 267},
		Control {
			enabled,
			130
		},
		/* [12] */
		{147, 192, 163, 384},
		Control {
			enabled,
			131
		},
		/* [13] */
		{99, 192, 115, 384},
		Control {
			enabled,
			131
		},
		/* [14] */
		{17, 129, 30, 162},
		EditText {
			enabled,
			""
		},
		/* [15] */
		{41, 129, 54, 162},
		EditText {
			enabled,
			""
		},
		/* [16] */
		{284, 188, 304, 325},
		Button {
			enabled,
			"Select Application"
		},
		/* [17] */
		{288, 12, 304, 133},
		StaticText {
			disabled,
			"Capture File Creator:"
		},
		/* [18] */
		{65, 129, 78, 162},
		EditText {
			enabled,
			""
		},
		/* [19] */
		{65, 16, 81, 117},
		StaticText {
			disabled,
			"Open Timeout:"
		},
		/* [20] */
		{89, 129, 102, 162},
		EditText {
			enabled,
			""
		},
		/* [21] */
		{158, 145, 171, 169},
		EditText {
			enabled,
			""
		},
		/* [22] */
		{41, 16, 58, 115},
		StaticText {
			disabled,
			"Send Timeout:"
		},
		/* [23] */
		{314, 12, 334, 110},
		Button {
			enabled,
			"ANSI Colors"
		},
		/* [24] */
		{239, 148, 252, 181},
		EditText {
			enabled,
			""
		},
		/* [25] */
		{159, 124, 174, 138},
		StaticText {
			disabled,
			"by"
		},
		/* [26] */
		{17, 16, 33, 121},
		StaticText {
			enabled,
			"Copy Table threshold:"
		},
		/* [27] */
		{89, 16, 105, 113},
		StaticText {
			disabled,
			"Timeslice:"
		},
		/* [28] */
		{-4, 378, 8, 455},
		StaticText {
			disabled,
			""
		},
		/* [29] */
		{342, 286, 357, 344},
		StaticText {
			disabled,
			""
		},
		/* [30] */
		{163, 192, 179, 384},
		Control {
			enabled,
			131
		},
		/* [31] */
		{109, 16, 125, 138},
		Control {
			enabled,
			132
		},
		/* [32] */
		{173, 16, 189, 138},
		Control {
			enabled,
			132
		},
		/* [33] */
		{189, 16, 205, 138},
		Control {
			enabled,
			132
		},
		/* [34] */
		{205, 16, 221, 138},
		Control {
			enabled,
			132
		},
		/* [35] */
		{179, 192, 195, 384},
		Control {
			enabled,
			131
		},
		/* [36] */
		{195, 192, 211, 384},
		Control {
			enabled,
			131
		},
		/* [37] */
		{221, 16, 237, 138},
		Control {
			enabled,
			132
		},
		/* [38] */
		{237, 16, 253, 138},
		Control {
			enabled,
			132
		},
		/* [39] */
		{289, 140, 302, 175},
		EditText {
			enabled,
			""
		},
		/* [40] */
		{239, 190, 255, 376},
		StaticText {
			enabled,
			"seconds (O = until application exit)"
		},
		/* [41] */
		{253, 16, 269, 138},
		Control {
			enabled,
			132
		},
		/* [42] */
		{125, 16, 141, 138},
		Control {
			enabled,
			132
		},
		/* [43] */
		{0, 0, 0, 0},
		HelpItem {
			disabled,
			HMScanhdlg {
				7001
			}
		}
	}
};

data 'DITL' (7004, "Session Config", purgeable) {
	$"005F 0000 0000 00E4 012E 00F8 0168 0402"            /* ._.....‰...¯.h.. */
	$"4F4B 0000 0000 00E4 00E8 00F8 0122 0406"            /* OK.....‰.Ë.¯.".. */
	$"4361 6E63 656C 0000 0000 004C 006B 005C"            /* Cancel.....L.k.\ */
	$"00B6 0702 0082 0000 0000 004C 00B5 005C"            /* .∂...Ç.....L.µ.\ */
	$"0100 0702 0082 0000 0000 004C 0103 005C"            /* .....Ç.....L...\ */
	$"014E 0702 0082 0000 0000 0036 006B 0046"            /* .N...Ç.....6.k.F */
	$"00B6 0702 0082 0000 0000 0036 00B5 0046"            /* .∂...Ç.....6.µ.F */
	$"0100 0702 0082 0000 0000 0036 006B 0046"            /* .....Ç.....6.k.F */
	$"00B6 0702 0082 0000 0000 0036 00B5 0046"            /* .∂...Ç.....6.µ.F */
	$"0100 0702 0082 0000 0000 0062 0020 0072"            /* .....Ç.....b. .r */
	$"009A 0702 0084 0000 0000 0071 0020 0081"            /* .ö...Ñ.....q. .Å */
	$"009A 0702 0084 0000 0000 0062 0020 0072"            /* .ö...Ñ.....b. .r */
	$"009A 0702 0084 0000 0000 008F 0020 009F"            /* .ö...Ñ.....è. .ü */
	$"009A 0702 0084 0000 0000 0071 0020 0081"            /* .ö...Ñ.....q. .Å */
	$"009A 0702 0084 0000 0000 0035 0020 0045"            /* .ö...Ñ.....5. .E */
	$"00B6 0702 0087 0000 0000 0046 0020 0056"            /* .∂...á.....F. .V */
	$"00B6 0702 0087 0000 0000 0080 0020 0090"            /* .∂...á.....Ä. .ê */
	$"009A 0702 0084 0000 0000 0020 0060 002C"            /* .ö...Ñ..... .`., */
	$"010B 1000 0000 0000 0038 0060 0044 010B"            /* .........8.`.D.. */
	$"1000 0000 0000 0050 0060 005C 0080 1000"            /* .......P.`.\.Ä.. */
	$"0000 0000 0039 0108 0045 012D 1000 0000"            /* .....9...E.-.... */
	$"0000 0053 0116 005F 012D 1000 0000 0000"            /* ...S..._.-...... */
	$"006E 0116 007A 012D 1000 0000 0000 0089"            /* .n...z.-.......â */
	$"0116 0095 012D 1000 0000 0000 0069 007C"            /* ...ï.-.......i.| */
	$"0079 00D9 0000 0000 0000 0081 007C 0091"            /* .y.Ÿ.......Å.|.ë */
	$"00D9 0000 0000 0000 0037 0020 0047 005A"            /* .Ÿ.......7. .G.Z */
	$"880A 486F 7374 204E 616D 653A 0000 0000"            /* à¬Host Name:.... */
	$"001F 0020 002F 005A 8806 416C 6961 733A"            /* ... ./.Zà.Alias: */
	$"0000 0000 004F 0020 005F 005A 8805 506F"            /* .....O. ._.Zà.Po */
	$"7274 3A02 0000 0000 004C 0020 005C 0048"            /* rt:......L. .\.H */
	$"8804 5445 4B3A 0000 0000 0038 0020 0048"            /* à.TEK:.....8. .H */
	$"0069 880D 5061 7374 6520 4D65 7468 6F64"            /* .ià.Paste Method */
	$"3A02 0000 0000 0038 0020 0048 0069 880D"            /* :......8. .H.ià. */
	$"4465 6C65 7465 2053 656E 6473 3A02 0000"            /* Delete Sends:... */
	$"0000 0053 00DC 0063 010E 880A 496E 7465"            /* ...S.‹.c..à¬Inte */
	$"7272 7570 743A 0000 0000 006E 00DC 007E"            /* rrupt:.....n.‹.~ */
	$"010E 8808 5375 7370 656E 643A 0000 0000"            /* ..à.Suspend:.... */
	$"0089 00DC 0099 010E 8807 5265 7375 6D65"            /* .â.‹.ô..à.Resume */
	$"3A02 0000 0000 0068 0020 0078 005C 8809"            /* :......h. .x.\à∆ */
	$"5465 726D 696E 616C 3A02 0000 0000 0080"            /* Terminal:......Ä */
	$"0020 008E 0077 8812 5472 616E 736C 6174"            /* . .é.wà.Translat */
	$"696F 6E20 5461 626C 653A 0000 0000 004D"            /* ion Table:.....M */
	$"0020 005D 0084 8813 4E65 7477 6F72 6B20"            /* . .].Ñà.Network  */
	$"426C 6F63 6B20 5369 7A65 3A02 0000 0000"            /* Block Size:..... */
	$"004D 008A 0059 00B2 1000 0000 0000 007E"            /* .M.ä.Y.≤.......~ */
	$"00E4 0092 0152 040E 5361 7665 2061 7320"            /* .‰.í.R..Save as  */
	$"5365 742E 2E2E 0000 0000 0066 00E4 007A"            /* Set........f.‰.z */
	$"0152 040D 4164 6420 746F 2053 6574 2E2E"            /* .R..Add to Set.. */
	$"2E02 0000 0000 0080 0020 0090 009A 0702"            /* .......Ä. .ê.ö.. */
	$"0084 0000 0000 0062 00BB 0072 0135 0702"            /* .Ñ.....b.ª.r.5.. */
	$"0084 0000 0000 0071 00BB 0081 0135 0702"            /* .Ñ.....q.ª.Å.5.. */
	$"0084 0000 0000 0004 0004 0014 016C 0702"            /* .Ñ...........l.. */
	$"03E9 0000 0000 0054 0020 0064 0139 0702"            /* .È.....T. .d.9.. */
	$"0083 0000 0000 0063 0020 0073 0139 0702"            /* .É.....c. .s.9.. */
	$"0083 0000 0000 0072 0020 0082 0139 0702"            /* .É.....r. .Ç.9.. */
	$"0083 0000 0000 0081 0020 0091 0139 0702"            /* .É.....Å. .ë.9.. */
	$"0083 0000 0000 0094 0020 00A4 006B 880D"            /* .É.....î. .§.kà. */
	$"4F54 5020 5061 7373 776F 7264 3A02 0000"            /* OTP Password:... */
	$"0000 0096 0073 00A2 00DE 1000 0000 0000"            /* ...ñ.s.¢.ﬁ...... */
	$"00A8 0020 00B8 0139 0702 0083 0000 0000"            /* .®. .∏.9...É.... */
	$"0055 0020 0069 014C 0702 07D0 0000 0000"            /* .U. .i.L...–.... */
	$"0070 0020 0080 006B 8809 5573 6572 6E61"            /* .p. .Ä.kà∆Userna */
	$"6D65 3A02 0000 0000 0088 0020 0098 006B"            /* me:......à. .ò.k */
	$"8809 5061 7373 776F 7264 3A02 0000 0000"            /* à∆Password:..... */
	$"00A0 0020 00B0 0072 8810 436C 6965 6E74"            /* .†. .∞.rà.Client */
	$"2075 7365 726E 616D 653A 0000 0000 00B8"            /*  username:.....∏ */
	$"0020 00C8 006B 8808 436F 6D6D 616E 643A"            /* . .».kà.Command: */
	$"0000 0000 0070 0078 007C 0150 1000 0000"            /* .....p.x.|.P.... */
	$"0000 0088 0078 0094 0150 1000 0000 0000"            /* ...à.x.î.P...... */
	$"00A0 0078 00AC 0150 1000 0000 0000 00B8"            /* .†.x.¨.P.......∏ */
	$"0078 00C4 0150 1000 0000 0000 0038 000C"            /* .x.ƒ.P.......8.. */
	$"004C 00F4 0702 07D1 0000 0000 003A 0100"            /* .L.Ù...—.....:.. */
	$"004A 0160 0702 07D6 0000 0000 0035 0020"            /* .J.`...÷.....5.  */
	$"0045 00B0 0702 0084 0000 0000 0045 002F"            /* .E.∞...Ñ.....E./ */
	$"0055 0148 0702 0083 0000 0000 005C 0066"            /* .U.H...É.....\.f */
	$"0068 00FC 1000 0000 0000 005C 0124 0068"            /* .h.¸.......\.$.h */
	$"0144 1000 0000 0000 005B 0020 0069 005E"            /* .D.......[. .i.^ */
	$"880B 534F 434B 5320 486F 7374 3A02 0000"            /* à.SOCKS Host:... */
	$"0000 005B 0102 006B 011D 8805 506F 7274"            /* ...[...k..à.Port */
	$"3A02 0000 0000 0074 0020 0082 005E 8809"            /* :......t. .Ç.^à∆ */
	$"5573 6572 6E61 6D65 3A02 0000 0000 0075"            /* Username:......u */
	$"0066 0082 00FC 1000 0000 0000 009C 0060"            /* .f.Ç.¸.......ú.` */
	$"00B0 011E 0416 4564 6974 2053 6573 7369"            /* .∞....Edit Sessi */
	$"6F6E 204D 6163 726F 732E 2E2E 0000 0000"            /* on Macros....... */
	$"0050 000C 0064 00F4 0702 07D3 0000 0000"            /* .P...d.Ù...”.... */
	$"0068 000C 007C 00F4 0702 07D4 0000 0000"            /* .h...|.Ù...‘.... */
	$"0064 010C 0074 015C 0702 0086 0000 0000"            /* .d...t.\...Ü.... */
	$"0074 010C 0084 015C 0702 0086 0000 0000"            /* .t...Ñ.\...Ü.... */
	$"0084 010C 0094 015C 0702 0086 0000 0000"            /* .Ñ...î.\...Ü.... */
	$"0080 0064 0090 0104 0702 0083 0000 0000"            /* .Ä.d.ê.....É.... */
	$"0052 0100 0062 0160 0702 07D7 0000 0000"            /* .R...b.`...◊.... */
	$"0098 000C 00AC 00F4 0702 07D5 0000 0000"            /* .ò...¨.Ù...’.... */
	$"00C0 0024 00CC 0044 1000 0000 0000 00C0"            /* .¿.$.Ã.D.......¿ */
	$"0064 00CC 010F 1000 0000 0000 00C0 011C"            /* .d.Ã.........¿.. */
	$"00CC 013C 1000 0000 0000 00AC 0020 00BC"            /* .Ã.<.......¨. .º */
	$"005A 880B 4C6F 6361 6C20 706F 7274 3A02"            /* .Zà.Local port:. */
	$"0000 0000 00AC 0060 00BC 00D8 880C 5265"            /* .....¨.`.º.ÿà.Re */
	$"6D6F 7465 2068 6F73 743A 0000 0000 00AC"            /* mote host:.....¨ */
	$"0118 00BC 015E 880C 5265 6D6F 7465 2070"            /* ...º.^à.Remote p */
	$"6F72 743A 0000 0000 009A 010C 00AA 015C"            /* ort:.....ö...™.\ */
	$"0702 0086 0000 0000 00BC 004A 00D0 005C"            /* ...Ü.....º.J.–.\ */
	$"0702 07D8 0000 0000 00BC 0142 00D0 0154"            /* ...ÿ.....º.B.–.T */
	$"0702 07D8 0000 0000 004C 0088 0060 009A"            /* ...ÿ.....L.à.`.ö */
	$"0702 07D8 0000 0000 009E 0020 00AE 011A"            /* ...ÿ.....û. .Æ.. */
	$"0702 0083 0000 0000 0046 00BC 0056 0152"            /* ...É.....F.º.V.R */
	$"0702 0087 0000 0000 0080 0010 0090 0060"            /* ...á.....Ä...ê.` */
	$"0702 0086 0000 0000 00AE 0020 00BE 011A"            /* ...Ü.....Æ. .æ.. */
	$"0702 0083 0000 0000 0000 0000 0000 0000"            /* ...É............ */
	$"8104 0000 0000 0000 0000 0000 0000 0000"            /* Å............... */
	$"0000 8104 0001 1B5C"                                /* ..Å....\ */
};

data 'DLGX' (7004) {
	$"0843 6861 7263 6F61 6C00 0000 0000 0000"            /* .Charcoal....... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"000C 0000 0000 0000 0004 0004 0000 0000"            /* ................ */
	$"005F 0000 0000 0000 0000 0000 0000 0001"            /* ._.............. */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0007"            /* ................ */
	$"0000 0000 0000 0000 0000 0007 0000 0000"            /* ................ */
	$"0000 0000 0000 0007 0000 0000 0000 0000"            /* ................ */
	$"0000 0007 0000 0000 0000 0000 0000 0007"            /* ................ */
	$"0000 0000 0000 0000 0000 0007 0000 0000"            /* ................ */
	$"0000 0000 0000 0007 0000 0000 0000 0000"            /* ................ */
	$"0000 000A 0000 0000 0000 0000 0000 000A"            /* ...¬...........¬ */
	$"0000 0000 0000 0000 0000 0006 0000 0000"            /* ................ */
	$"0000 0000 0000 0006 0000 0000 0000 0000"            /* ................ */
	$"0000 0006 0000 0000 0000 0000 0000 0006"            /* ................ */
	$"0000 0000 0000 0000 0000 0006 0000 0000"            /* ................ */
	$"0000 0000 0000 0006 0000 0000 0000 0000"            /* ................ */
	$"0000 0006 0000 0000 0000 0000 0000 0006"            /* ................ */
	$"0000 0000 0000 0000 0000 0006 0000 0000"            /* ................ */
	$"0000 0000 0000 0006 0000 0000 0000 0000"            /* ................ */
	$"0000 0006 0000 0000 0000 0000 0000 0006"            /* ................ */
	$"0000 0000 0000 0000 0000 0007 0000 0000"            /* ................ */
	$"0000 0000 0000 0002 0000 0000 0000 0000"            /* ................ */
	$"0000 0002 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0006"            /* ................ */
	$"0000 0000 0000 0000 0000 0007 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0006"            /* ................ */
	$"0000 0000 0000 0000 0000 0006 0000 0000"            /* ................ */
	$"0000 0000 0000 0006 0000 0000 0000 0000"            /* ................ */
	$"0000 0006 0000 0000 0000 0000 0000 0007"            /* ................ */
	$"0000 0000 0000 0000 0000 0007 0000 0000"            /* ................ */
	$"0000 0000 0000 0007 0000 0000 0000 0000"            /* ................ */
	$"0000 0007 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0007"            /* ................ */
	$"0000 0000 0000 0000 0000 0007 0000 0000"            /* ................ */
	$"0000 0000 0000 0006 0000 0000 0000 0000"            /* ................ */
	$"0000 0006 0000 0000 0000 0000 0000 0006"            /* ................ */
	$"0000 0000 0000 0000 0000 0007 0000 0000"            /* ................ */
	$"0000 0000 0000 0002 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0007 0000 0000 0000 0000 0000 0007"            /* ................ */
	$"0000 0000 0000 0000 0000 0007 0000 0000"            /* ................ */
	$"0000 0000 0000 0006 0000 0000 0000 0000"            /* ................ */
	$"0000 0006 0000 0000 0000 0000 0000 0006"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 0000 0000"            /* ................ */
	$"0000 0000 0000 0005 0000 0000 0000 0000"            /* ................ */
	$"0000 0005 0000 0000 0000 0000 0000 696E"            /* ..............in */
	$"7320 1120 7265 736F 7572 6365 7320 7573"            /* s . resources us */
	$"0C10 BC60 1000"                                     /* ..º`.. */
};

resource 'DLOG' (128, "Resolver Error", purgeable) {
	{82, 88, 230, 433},
	dBoxProc,
	visible,
	noGoAway,
	0x0,
	128,
	"New Dialog",
	alertPositionMainScreen
};

resource 'DLOG' (129, "FTP info", purgeable) {
	{74, 136, 219, 437},
	dBoxProc,
	visible,
	goAway,
	0x0,
	129,
	"",
	centerMainScreen
};

resource 'DLOG' (130, purgeable) {
	{528, 464, 578, 685},
	dBoxProc,
	visible,
	goAway,
	0x0,
	130,
	"Building font list",
	alertPositionMainScreen
};

resource 'DLOG' (150, "Edit Shortcuts", purgeable) {
	{233, 285, 449, 601},
	movableDBoxProc,
	invisible,
	goAway,
	0x0,
	150,
	"Edit Favorites",
	centerMainScreen
};

resource 'DLOG' (151, "Duplicate Session Alert", purgeable) {
	{310, 401, 410, 737},
	dBoxProc,
	visible,
	noGoAway,
	0x0,
	151,
	"New DITL 130 from mw/68k•--Telnet.rsrc",
	alertPositionParentWindow
};

resource 'DLOG' (160, "FatalCancelAlert", purgeable) {
	{108, 76, 257, 441},
	dBoxProc,
	invisible,
	noGoAway,
	0x0,
	160,
	"",
	alertPositionMainScreen
};

resource 'DLOG' (170, "FatalAlert", purgeable) {
	{108, 76, 257, 441},
	dBoxProc,
	invisible,
	noGoAway,
	0x0,
	170,
	"",
	alertPositionMainScreen
};

resource 'DLOG' (175, "OperationFailedAlert", purgeable) {
	{108, 76, 257, 441},
	dBoxProc,
	invisible,
	noGoAway,
	0x0,
	175,
	"",
	alertPositionMainScreen
};

resource 'DLOG' (180, "AlertAskUser", purgeable) {
	{106, 92, 231, 418},
	dBoxProc,
	visible,
	noGoAway,
	0x0,
	180,
	"",
	alertPositionMainScreen
};

resource 'DLOG' (190, "Change Window Title", purgeable) {
	{136, 108, 217, 387},
	movableDBoxProc,
	visible,
	goAway,
	0x0,
	190,
	"Set Window Title",
	alertPositionParentWindow
};

resource 'DLOG' (195, "DNR Error Alert", purgeable) {
	{108, 76, 257, 441},
	dBoxProc,
	invisible,
	noGoAway,
	0x0,
	195,
	"",
	alertPositionMainScreen
};

resource 'DLOG' (259, "My IP Number", purgeable) {
	{98, 127, 154, 392},
	documentProc,
	visible,
	goAway,
	0x0,
	259,
	"IP Address",
	alertPositionMainScreen
};

resource 'DLOG' (270, "Status Dialog", purgeable) {
	{96, 122, 196, 442},
	dBoxProc,
	visible,
	goAway,
	0x0,
	270,
	"New Dialog",
	alertPositionMainScreen
};

resource 'DLOG' (273, "setup keys", purgeable) {
	{146, 112, 326, 442},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	273,
	"Setup Keys",
	centerParentWindow
};

resource 'DLOG' (280, "New Open Connection", purgeable, preload) {
	{94, 92, 278, 424},
	movableDBoxProc,
	invisible,
	noGoAway,
	0x0,
	280,
	"Open Connection...",
	centerMainScreen
};

data 'DLOG' (281, "New Open Connection COPY", purgeable) {
	$"0046 005E 00F7 018C 0001 0100 0000 0000"            /* .F.^.˜.å........ */
	$"0000 0119 0A4E 6577 2044 6961 6C6F 67"              /* ....¬New Dialog */
};

resource 'DLOG' (290, "Other Font Size", purgeable) {
	{200, 402, 300, 622},
	movableDBoxProc,
	visible,
	goAway,
	0x0,
	290,
	"Other Font Size",
	centerMainScreen
};

resource 'DLOG' (303, "Couldn't Connect", purgeable) {
	{76, 142, 180, 384},
	dBoxProc,
	visible,
	goAway,
	0x0,
	303,
	"New Dialog",
	alertPositionMainScreen
};

resource 'DLOG' (500, "Macros", purgeable) {
	{60, 10, 350, 545},
	noGrowDocProc,
	visible,
	goAway,
	0x0,
	500,
	"Set Macros...",
	centerMainScreen
};

resource 'DLOG' (501, "flash.about", purgeable) {
	{30, 30, 274, 422},
	plainDBox,
	visible,
	goAway,
	0x0,
	501,
	"New Dialog",
	alertPositionMainScreen
};

resource 'DLOG' (502, "Macros Modal", purgeable) {
	{60, 10, 350, 545},
	movableDBoxProc,
	visible,
	goAway,
	0x0,
	500,
	"Set Macros...",
	centerMainScreen
};

resource 'DLOG' (640, "Generic Errors", preload) {
	{76, 120, 186, 415},
	dBoxProc,
	visible,
	goAway,
	0x0,
	640,
	"New Dialog",
	alertPositionMainScreen
};

resource 'DLOG' (1001, "Color Selection", purgeable) {
	{121, 100, 300, 340},
	movableDBoxProc,
	visible,
	goAway,
	0x0,
	1001,
	"Terminal Colors",
	centerParentWindow
};

resource 'DLOG' (1002, "ANSI Color Selection", purgeable) {
	{113, 350, 405, 610},
	movableDBoxProc,
	visible,
	goAway,
	0x0,
	1002,
	"ANSI Colors",
	centerParentWindow
};

resource 'DLOG' (1023, "Opening Dialog", purgeable, preload) {
	{46, 66, 294, 462},
	plainDBox,
	visible,
	goAway,
	0x0,
	1023,
	"New Dialog",
	alertPositionMainScreen
};

resource 'DLOG' (2001, "Close Connection Dialog", purgeable, preload) {
	{90, 70, 205, 460},
	dBoxProc,
	visible,
	goAway,
	0x0,
	2001,
	"New Dialog",
	alertPositionParentWindow
};

resource 'DLOG' (4001, "set dir", purgeable) {
	{0, 0, 254, 352},
	dBoxProc,
	invisible,
	noGoAway,
	0x0,
	4001,
	"",
	centerMainScreen
};

resource 'DLOG' (5500, "Screen Size", purgeable) {
	{75, 112, 223, 368},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	5500,
	"Terminal Size",
	centerParentWindow
};

resource 'DLOG' (7002, "FTP config", purgeable) {
	{34, 60, 304, 417},
	dBoxProc,
	invisible,
	noGoAway,
	0x0,
	7002,
	"",
	centerMainScreen
};

resource 'DLOG' (7005, "FTP User Config", purgeable) {
	{76, 130, 262, 369},
	dBoxProc,
	visible,
	goAway,
	0x0,
	7005,
	"",
	centerParentWindow
};

resource 'DLOG' (8000, "OTP Password Entry") {
	{46, 40, 243, 409},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	8000,
	"One-Time Passwords",
	centerMainScreen
};

resource 'DLOG' (10000, "Randomization", purgeable) {
	{48, 40, 162, 372},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	10000,
	"Randomization",
	alertPositionMainScreen
};

resource 'DLOG' (10001, "Password Prompt", purgeable) {
	{136, 196, 272, 528},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	10001,
	"Enter Password",
	alertPositionParentWindow
};

resource 'DLOG' (10002, "Host Key Unknown", purgeable) {
	{130, 130, 302, 466},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	10002,
	"Host Key Unknown",
	alertPositionParentWindow
};

resource 'DLOG' (10003, "Host Key Changed", purgeable) {
	{130, 130, 326, 470},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	10003,
	"Host Key Changed",
	alertPositionParentWindow
};

resource 'DLOG' (10004, "Login Prompt", purgeable) {
	{177, 137, 321, 501},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	10004,
	"SSH2 Login",
	centerMainScreen
};

resource 'DLOG' (10010, "Create Keys", purgeable) {
	{123, 72, 323, 356},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	10010,
	"Create SSH2 Keys",
	centerParentWindow
};

resource 'DLOG' (10011, "Generic Errors", purgeable) {
	{76, 120, 216, 476},
	dBoxProc,
	visible,
	goAway,
	0x0,
	10011,
	"New Dialog",
	alertPositionMainScreen
};

resource 'DLOG' (10012, "Generate Keys", purgeable) {
	{218, 181, 314, 405},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	10012,
	"",
	centerParentWindow
};

resource 'DLOG' (7003, "Terminal Config", purgeable) {
	{115, 112, 360, 442},
	movableDBoxProc,
	invisible,
	goAway,
	0x0,
	7003,
	"Edit Terminal...",
	centerMainScreen
};

resource 'DLOG' (7001, "Main Prefs", purgeable) {
	{111, 122, 459, 514},
	movableDBoxProc,
	invisible,
	noGoAway,
	0x0,
	7001,
	"Preferences...",
	centerMainScreen
};

resource 'DLOG' (7004, "Session Config", purgeable) {
	{124, 289, 380, 657},
	movableDBoxProc,
	invisible,
	goAway,
	0x0,
	7004,
	"Edit Favorite...",
	centerMainScreen
};

data 'FOND' (74, "%NCSA VT", purgeable) {
	$"0000 004A 0000 0000 0000 0000 0000 0000"            /* ...J............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0005 0009 0000 2509 0009 0001"            /* .......∆..%∆.∆.. */
	$"2509 000A 0000 250A 000A 0000 250A 000C"            /* %∆.¬..%¬.¬..%¬.. */
	$"0000 250C 000C 0001 250C"                           /* ..%.....%. */
};

data 'FOND' (75, "NCSA VT Bold", purgeable) {
	$"0000 004B 0000 0000 0000 0000 0000 0000"            /* ...K............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0009 0000 2589"                      /* .......∆..%â */
};

data 'FOND' (6001, "ANSI/PC", preload) {
	$"B000 1771 0000 0000 0000 0000 0000 0000"            /* ∞..q............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0001 0001 0009 0000 4B40 000A 0000"            /* .......∆..K@.¬.. */
	$"4B41"                                               /* KA */
};

data 'FOND' (16784, "ASLFont+e") {
	$"6000 4190 0000 00FF 0000 0000 0000 0000"            /* `.Aê...ˇ........ */
	$"0000 004E 0000 0000 0000 0000 0000 0000"            /* ...N............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0002 0002 0009 0000 4809 000A 0000"            /* .......∆..H∆.¬.. */
	$"480A 000C 0000 480C 0000 0000 0AAB 0AAB"            /* H¬....H.....¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0000 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´..¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 01C7"            /* ¬´¬´¬´¬´¬´¬´¬´.« */
	$"1555 1555 1555 1555 1555 1555 1555 1555"            /* .U.U.U.U.U.U.U.U */
	$"1555 1555 1555 1555 1555 1555 1555 1555"            /* .U.U.U.U.U.U.U.U */
	$"1555 1555 1555 1555 1555 1555 1555 1555"            /* .U.U.U.U.U.U.U.U */
	$"1555 1555 1555 1555 1555 1555 0AAB 0AAB"            /* .U.U.U.U.U.U¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 1000 1000"            /* ¬´¬´¬´¬´¬´¬´.... */
};

data 'FOND' (16800, "ASLFont+") {
	$"6000 41A0 0000 00FF 0000 0000 0000 0000"            /* `.A†...ˇ........ */
	$"0000 004E 0000 0000 0000 0000 0000 0000"            /* ...N............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0002 0002 0009 0000 5009 000A 0000"            /* .......∆..P∆.¬.. */
	$"500A 000C 0000 500C 0000 0000 0AAB 0AAB"            /* P¬....P.....¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0000 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´..¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 01C7"            /* ¬´¬´¬´¬´¬´¬´¬´.« */
	$"1555 1555 1555 1555 1555 1555 1555 1555"            /* .U.U.U.U.U.U.U.U */
	$"1555 1555 1555 1555 1555 1555 1555 1555"            /* .U.U.U.U.U.U.U.U */
	$"1555 1555 1555 1555 1555 1555 1555 1555"            /* .U.U.U.U.U.U.U.U */
	$"1555 1555 1555 1555 1555 1555 0AAB 0AAB"            /* .U.U.U.U.U.U¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 0AAB"            /* ¬´¬´¬´¬´¬´¬´¬´¬´ */
	$"0AAB 0AAB 0AAB 0AAB 0AAB 0AAB 1000 1000"            /* ¬´¬´¬´¬´¬´¬´.... */
};

resource 'FREF' (128) {
	'APPL',
	0,
	""
};

resource 'FREF' (129) {
	'DBUG',
	1,
	""
};

resource 'FREF' (130) {
	'CONF',
	2,
	""
};

resource 'FREF' (131) {
	'IPNO',
	3,
	""
};

resource 'FREF' (132) {
	'TEXT',
	4,
	""
};

data 'FTPs' (1990, "FTP Server Prefs Master Copy") {
	$"0000 0000 3F3F 3F3F 4249 4E41 7474 7874"            /* ....????BINAttxt */
	$"0001 0100 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
};

resource 'ICN#' (128, "Application") {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 0000 0000 0000 0000 0000 0000"
		$"3FFF FFFC 4000 0002 4000 0002 4FFF FFF2"
		$"4FFF FFF2 4D63 FFF2 4823 FFF2 4D63 FFF2"
		$"4823 FFF2 4D63 FFF2 4FFF FFF2 4FFF FFF2"
		$"4FFF FFF2 4FFF FFF2 4FFF FFF2 4FFF FFF2"
		$"4FFF FFF2 4FFF FFF2 4FFF FFF2 4FFF FFF2"
		$"4FFF FFF2 4FFF FFF2 4000 0002 4000 0002"
		$"3FFF FFFC",
		/* [2] */
		$"0000 0000 0000 0000 0000 0000 0000 0000"
		$"0000 0000 3FFF FFFC 7FFF FFFE 7FFF FFFE"
		$"7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE"
		$"7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE"
		$"7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE"
		$"7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE"
		$"7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE"
		$"3FFF FFFC"
	}
};

resource 'ICN#' (129, "Application") {
	{	/* array: 2 elements */
		/* [1] */
		$"3FC0 7F80 2040 4080 2F40 5E80 2840 5080"
		$"2840 5080 2040 4080 FFF1 FFFE 801F 0022"
		$"8011 0022 FFF1 FFE2 0000 0002 007F F002"
		$"0055 5002 0055 5002 0055 5002 0055 5002"
		$"0055 5002 0055 5002 03FF FFFC 0200 0202"
		$"0200 0202 03FF FE02 0000 0002 FE1F C3FA"
		$"8210 420A B216 42CA A214 428E A3F4 7E88"
		$"8210 4208 9A13 4268 8210 4208 7C0F 81F0",
		/* [2] */
		$"3FC0 7F80 3FC0 7F80 3FC0 7F80 3FC0 7F80"
		$"3FC0 7F80 3FC0 7F80 FFF1 FFFE FFFF FFE2"
		$"FFF1 FFE2 FFF1 FFE2 0000 0002 007F F002"
		$"007F F002 007F F002 007F F002 007F F002"
		$"007F F002 007F F002 03FF FFFC 03FF FE02"
		$"03FF FE02 03FF FE02 0000 0002 FE1F C3FA"
		$"FE1F C3FA FE1F C3FA FE1F C3FE FFFF FFF8"
		$"FE1F C3F8 FE1F C3F8 FE1F C3F8 7C0F 81F0"
	}
};

resource 'ICN#' (130, "DEBUG") {
	{	/* array: 2 elements */
		/* [1] */
		$"3FC0 7F80 2040 4080 2F40 5E80 2F40 5E80"
		$"2040 4080 FFF1 FFE0 B01F 603E 80F1 03E2"
		$"FFF1 FFE2 0000 0002 0003 2AA2 0003 1542"
		$"000F CAA2 0010 2542 0010 22A2 0013 2142"
		$"0017 A0A2 0013 2042 0017 A022 0013 2002"
		$"0017 A002 0010 2002 001F E002 FE1F C3FA"
		$"8210 420A BA17 42EA BA17 42EE BBF7 7EE8"
		$"8210 4208 FE1F C3F8 8610 4208 FE1F C3F8",
		/* [2] */
		$"3FC0 7F80 3FC0 7F80 3FC0 7F80 3FC0 7F80"
		$"3FC0 7F80 FFF1 FFE0 FFFF FFFE FFF1 FFE2"
		$"FFF1 FFE2 0000 0002 0003 2AA2 0003 1FC2"
		$"000F CFE2 001F E7C2 001F E3E2 001F E1C2"
		$"001F E0E2 001F E042 001F E022 001F E002"
		$"001F E002 001F E002 001F E002 FE1F C3FA"
		$"FE1F C3FA FE1F C3FA FE1F C3FE FFFF FFF8"
		$"FE1F C3F8 FE1F C3F8 FE1F C3F8 FE1F C3F8"
	}
};

resource 'ICN#' (131, "Document") {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 0000 0000 0000 0000 FFFF FFC0"
		$"8000 0060 8000 0050 8000 0048 87FF C044"
		$"8800 2042 89FF 207F 8A00 A001 8A00 A081"
		$"8A00 A041 8A00 AFE1 8A00 A041 8A00 A281"
		$"89FF 2401 8800 2FE1 880F A401 8800 2201"
		$"8800 2001 8FFF E001 8400 4001 87FF C001"
		$"8010 0001 8AEE A001 8000 0001 FFFF FFFF",
		/* [2] */
		$"0000 0000 0000 0000 0000 0000 FFFF FFC0"
		$"FFFF FFE0 FFFF FFF0 FFFF FFF8 FFFF FFFC"
		$"FFFF FFFE FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	}
};

resource 'ICN#' (132, "IP Number Document") {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 0000 0000 0000 0000 0000 0000"
		$"0000 0000 0000 0000 0000 0000 3FFE 0000"
		$"4001 0420 4FF9 0630 5005 0630 5765 7FFC"
		$"5255 3FFE 5265 0630 5745 0630 5005 0630"
		$"4FF9 7FFC 4001 3FFE 407D 0630 4001 0630"
		$"4001 0630 7FFF 0210 2002 0000 3FFE 0000"
		$"0080 0000 0140 0000 0140 0000 0140 0000"
		$"FE3F 80",
		/* [2] */
		$"0000 0000 0000 0000 0000 0000 0000 0000"
		$"0000 0000 0000 0000 0000 0000 3FFE 0000"
		$"7FFF 0420 7FFF 0630 7FFF 0630 7FFF 7FFC"
		$"7FFF 3FFE 7FFF 0630 7FFF 0630 7FFF 0630"
		$"7FFF 7FFC 7FFF 3FFE 7FFF 0630 7FFF 0630"
		$"7FFF 0630 7FFF 0210 3FFE 0000 3FFE 0000"
		$"0080 0000 0140 0000 0140 0000 0140 0000"
		$"FE3F 80"
	}
};

resource 'ICN#' (133) {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 0000 0000 0000 0000 0000 0000"
		$"0000 0000 0000 0000 0000 0000 3FFE 0000"
		$"4001 0000 4FF9 0000 5005 18C0 5005 2520"
		$"5005 2520 5005 1FC0 5005 0500 5005 1FC0"
		$"4FF9 2520 4001 2520 407D 18C0 4001 0000"
		$"4001 0000 7FFF 0000 2002 0000 3FFE 0000"
		$"0080 0000 0140 0000 0140 0000 0140 0000"
		$"FE3F 80",
		/* [2] */
		$"0000 0000 0000 0000 0000 0000 0000 0000"
		$"0000 0000 0000 0000 0000 0000 3FFE 0000"
		$"7FFF 0000 7FFF 0000 7FFF 18C0 7FFF 3DE0"
		$"7FFF 3DE0 7FFF 1FC0 7FFF 0700 7FFF 1FC0"
		$"7FFF 3DE0 7FFF 3DE0 7FFF 18C0 7FFF 0000"
		$"7FFF 0000 7FFF 0000 3FFE 0000 3FFE 0000"
		$"0080 0000 0140 0000 0140 0000 0140 0000"
		$"FE3F 80"
	}
};

resource 'ICN#' (134, "Application") {
	{	/* array: 2 elements */
		/* [1] */
		$"03F0 00F0 0FF8 03FC 3FF8 07FE 7E00 007F"
		$"781D E01F F83F FC0F F87F FE0F F07F FF0F"
		$"F1FF FF8F FFFF FFFF 7FFF FFFF 7FFF FFFE"
		$"3FFF FFFC 3FFF FFFC 1FFF FFF8 07FF FFF0"
		$"03FF FFF0 07FF FF80 0FFF FF80 0FFF FF80"
		$"01FF FF80 01FF FF00 01FF FF00 01FF FF00"
		$"00FF FF00 00FF FE00 00FF FC00 003F FC00"
		$"0001 F800 0000 F000 0000 E0",
		/* [2] */
		$"03F0 00F0 0FF8 03FC 3FF8 07FE 7E00 007F"
		$"781D E01F F83F FC0F F87F FE0F F07F FF0F"
		$"F1FF FF8F FFFF FFFF 7FFF FFFF 7FFF FFFE"
		$"3FFF FFFC 3FFF FFFC 1FFF FFF8 07FF FFF0"
		$"03FF FFF0 07FF FF80 0FFF FF80 0FFF FF80"
		$"01FF FF80 01FF FF00 01FF FF00 01FF FF00"
		$"00FF FF00 00FF FE00 00FF FC00 003F FC00"
		$"0001 F800 0000 F000 0000 E0"
	}
};

resource 'ICN#' (135) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'ICON' (129) {
	$"0000 0000 0000 000C 0000 0012 0000 0022"
	$"0000 0044 0000 0088 0000 0114 0000 0222"
	$"0000 0441 0000 088A 0000 1114 0000 22A0"
	$"0000 4450 0000 8820 0001 1000 0002 2000"
	$"0004 4000 0008 8000 0011 0000 0022 0000"
	$"0E44 0000 3188 0000 4010 0000 8E20 0000"
	$"9120 0000 9090 0000 9090 0000 4890 0000"
	$"4720 0000 2020 0000 1840 0000 0780"
};

resource 'MBAR' (256, "New Menus", purgeable, preload) {
	{	/* array MenuArray: 5 elements */
		/* [1] */
		1,
		/* [2] */
		512,
		/* [3] */
		513,
		/* [4] */
		514,
		/* [5] */
		515
	}
};

data 'MDEF' (128, "Picture MDEF") {
	$"600A 0000 4D44 4546 0080 0000 48E7 E0C8"            /* `¬..MDEF.Ä..HÁ‡» */
	$"4EBA 00F2 41FA FFEA 2008 A055 4EBA 0090"            /* N∫.ÚA˙ˇÍ .†UN∫.ê */
	$"4CDF 1307 4EFA 00F2 41FA FFD6 D1FC 0000"            /* Lﬂ..N˙.ÚA˙ˇ÷—¸.. */
	$"04FB 2008 A055 4E75 48E7 0600 594F 206F"            /* .˚ .†UNuHÁ..YO o */
	$"0010 226F 0014 2C2F 0018 1E98 1F58 0001"            /* .."o..,/...ò.X.. */
	$"1F58 0002 1F58 0003 2A17 7400 6044 1218"            /* .X...X..*.t.`D.. */
	$"1001 0240 0080 670C D201 1001 4880 48C0"            /* ...@.Äg.“...HÄH¿ */
	$"D480 6028 1E81 1F58 0001 1001 0240 0040"            /* ‘Ä`(.Å.X.....@.@ */
	$"670C 3017 E548 E240 48C0 D480 600E 1F58"            /* g.0.ÂH‚@H¿‘Ä`..X */
	$"0002 1F58 0003 2417 E58A E282 DDB1 2800"            /* ...X..$.Âä‚Ç›±(. */
	$"5385 4A85 6EB8 584F 4CDF 0060 4E75 48E7"            /* SÖJÖn∏XOLﬂ.`NuHÁ */
	$"1020 2440 202C 0000 260A 9680 6740 4A80"            /* . $@ ,..&¬ñÄg@JÄ */
	$"661C 422C 0004 303C A89F A746 2F08 303C"            /* f.B,..0<®üßF/.0< */
	$"A198 A346 B1DF 56C0 4400 1940 0004 2F03"            /* °ò£F±ﬂV¿D..@../. */
	$"2F0A 4EBA FF44 2F00 4EBA FF4E 4FEF 000C"            /* /¬N∫ˇD/.N∫ˇNOÔ.. */
	$"294A 0000 4A2C 0004 6704 7001 A198 4CDF"            /* )J..J,..g.p.°òLﬂ */
	$"0408 4E75 41FA FEFA D1FC 0000 04F6 2008"            /* ..NuA˙˛˙—¸...ˆ . */
	$"A055 C18C 4E75 4E75 4E56 0000 48E7 0030"            /* †U¡åNuNuNV..HÁ.0 */
	$"266E 0014 246E 0010 302E 0018 670A 5340"            /* &n..$n..0...g¬S@ */
	$"6714 5340 6728 6032 2F0A 2F0B 4EB9 0000"            /* g.S@g(`2/¬/.Nπ.. */
	$"01E2 504F 6024 2F2E 0008 2F2E 000C 2F0A"            /* .‚PO`$/.../.../¬ */
	$"2F0B 4EB9 0000 033A 4FEF 0010 600C 2F0A"            /* /.Nπ...:OÔ..`./¬ */
	$"2F0B 4EB9 0000 0180 504F 4CDF 0C00 4E5E"            /* /.Nπ...ÄPOLﬂ..N^ */
	$"205F 4FEF 0012 4ED0 846D 6169 6E00 0000"            /*  _OÔ..N–Ñmain... */
	$"4E56 FFF8 48E7 1030 266E 0008 95CA 2053"            /* NVˇ¯HÁ.0&n..ï  S */
	$"3610 594F 3F03 A9BC 205F 2448 200A 672A"            /* 6.YO?.©º _$H ¬g* */
	$"2052 2D68 0002 FFF8 2D68 0006 FFFC 302E"            /*  R-h..ˇ¯-h..ˇ¸0. */
	$"FFFE 906E FFFA 2053 3140 0002 302E FFFC"            /* ˇ˛ênˇ˙ S1@..0.ˇ¸ */
	$"906E FFF8 2053 3140 0004 4CDF 0C08 4E5E"            /* ênˇ¯ S1@..Lﬂ..N^ */
	$"4E75 8D44 6F53 697A 654D 6573 7361 6765"            /* NuçDoSizeMessage */
	$"0000 4E56 FFF0 48E7 1830 282E 0008 95CA"            /* ..NVˇHÁ.0(...ï  */
	$"97CB 2044 2050 3610 594F 3F03 A9BC 205F"            /* óÀ D P6.YO?.©º _ */
	$"2448 200A 6736 47F9 0000 0254 200B 672C"            /* $H ¬g6G˘...T .g, */
	$"2D4A FFF4 2D6E 000C FFF8 2044 2050 2D68"            /* -JˇÙ-n..ˇ¯ D P-h */
	$"000A FFFC 486E FFF0 A874 206E FFF0 2F28"            /* .¬ˇ¸Hnˇ®t nˇ/( */
	$"0018 2F0B 486E FFF4 42A7 ABCA 4CDF 0C18"            /* ../.HnˇÙBß´ Lﬂ.. */
	$"4E5E 4E75 8D44 6F44 7261 774D 6573 7361"            /* N^NuçDoDrawMessa */
	$"6765 0000 4E56 FFD0 2F0A 246E 0008 486E"            /* ge..NVˇ–/¬$n..Hn */
	$"FFF0 A874 200A 6700 00B4 4A92 6706 4AAA"            /* ˇ®t ¬g..¥Jíg.J™ */
	$"0004 6604 6000 00A6 2F12 2F2A 0004 A8F6"            /* ..f.`..¶/./*..®ˆ */
	$"70FF B0AA 0008 6700 0094 4AAE 000C 675E"            /* pˇ∞™..g..îJÆ..g^ */
	$"486E FFFA AA19 486E FFEA AA1A 2D6E FFFA"            /* Hnˇ˙™.HnˇÍ™.-nˇ˙ */
	$"FFF4 3D6E FFFE FFF8 554F 2F2E 000C 486E"            /* ˇÙ=nˇ˛ˇ¯UO/...Hn */
	$"FFEA 486E FFF4 303C 0C19 AAA2 101F 672E"            /* ˇÍHnˇÙ0<..™¢..g. */
	$"486E FFF4 AA14 206E FFF0 4868 0002 206E"            /* HnˇÙ™. nˇHh.. n */
	$"FFF0 4868 0002 2F2A 0004 2F2A 0004 3F3C"            /* ˇHh../*...*..?< */
	$"0009 42A7 A8EC 486E FFFA AA14 602E 486E"            /* .∆Bß®ÏHnˇ˙™.`.Hn */
	$"FFD8 A898 3F3C 000F A89C 486E FFD0 2F3C"            /* ˇÿ®ò?<..®úHnˇ–/< */
	$"0004 0000 4EB9 0000 04B8 486E FFD0 A89D"            /* ....Nπ...∏Hnˇ–®ù */
	$"2F2A 0004 A8A2 486E FFD8 A899 245F 4E5E"            /* /*..®¢Hnˇÿ®ô$_N^ */
	$"205F 4FEF 000C 4ED0 8F44 5241 5754 4845"            /*  _OÔ..N–èDRAWTHE */
	$"4D45 4E55 5052 4F43 0000 4E56 FFF4 48E7"            /* MENUPROC..NVˇÙHÁ */
	$"1F30 2C2E 000C 266E 0014 7E00 95CA 7A00"            /* .0,...&n..~.ï z. */
	$"2D6E 0010 FFFC 7800 206E 0008 2050 70FF"            /* -n..ˇ¸x. n.. Ppˇ */
	$"B0A8 000A 6600 012A 206E 0008 2050 3610"            /* ∞®.¬f..* n.. P6. */
	$"594F 2F3C 5052 4354 3F03 A9A0 205F 2E08"            /* YO/<PRCT?.©† _.. */
	$"4A87 6606 4253 6000 0108 2047 A029 594F"            /* Jáf.BS`... G†)YO */
	$"2F07 4EB9 0000 04AA 201F E688 3A00 2047"            /* /.Nπ...™ .Êà:. G */
	$"2450 554F 2F2E FFFC 2F06 A8AD 101F 6700"            /* $PUO/.ˇ¸/.®≠..g. */
	$"00AA 2046 3010 916E FFFC 2046 3028 0002"            /* .™ F0.ënˇ¸ F0(.. */
	$"916E FFFE 7600 601E 554F 2F2E FFFC 3043"            /* ënˇ˛v.`.UO/.ˇ¸0C */
	$"2008 E788 204A D1C0 4850 A8AD 101F 6704"            /*  .Áà J—¿HP®≠..g. */
	$"3803 5244 5243 B645 6C04 4A44 67DA 4A53"            /* 8.RDRC∂El.JDg⁄JS */
	$"6F30 B853 672C 3013 5340 48C0 E788 204A"            /* o0∏Sg,0.S@H¿Áà J */
	$"D1C0 2D50 FFF4 2D68 0004 FFF8 486E FFF4"            /* —¿-PˇÙ-h..ˇ¯HnˇÙ */
	$"2046 3F28 0002 2046 3F10 A8A8 486E FFF4"            /*  F?(.. F?.®®HnˇÙ */
	$"A8A4 B853 6766 3684 4A53 6760 3013 5340"            /* ®§∏Sgf6ÑJSg`0.S@ */
	$"48C0 E788 204A D1C0 2D50 FFF4 2D68 0004"            /* H¿Áà J—¿-PˇÙ-h.. */
	$"FFF8 486E FFF4 2046 3F28 0002 2046 3F10"            /* ˇ¯HnˇÙ F?(.. F?. */
	$"A8A8 486E FFF4 A8A4 6032 4A53 6F2E 3013"            /* ®®HnˇÙ®§`2JSo.0. */
	$"5340 48C0 E788 204A D1C0 2D50 FFF4 2D68"            /* S@H¿Áà J—¿-PˇÙ-h */
	$"0004 FFF8 486E FFF4 2046 3F28 0002 2046"            /* ..ˇ¯HnˇÙ F?(.. F */
	$"3F10 A8A8 486E FFF4 A8A4 4253 2047 A02A"            /* ?.®®HnˇÙ®§BS G†* */
	$"4CDF 0CF8 4E5E 4E75 8F44 6F43 686F 6F73"            /* Lﬂ.¯N^NuèDoChoos */
	$"654D 6573 7361 6765 0000 225F 205F A025"            /* eMessage.."_ _†% */
	$"2E80 6A02 4297 4ED1 4E56 0000 594F 2F3C"            /* .Äj.BóN—NV..YO/< */
	$"5041 5423 3F2E 000A A9A0 226E 000C 201F"            /* PAT#?..¬©†"n.. . */
	$"671C 2040 2050 3018 322E 0008 6710 B240"            /* g. @ P0.2...g.≤@ */
	$"620C 5341 6704 5048 60F8 22D8 2290 4E5E"            /* b.SAg.PH`¯"ÿ"êN^ */
	$"205F 508F 4ED0 0000 0000 0000 0000 0640"            /*  _PèN–.........@ */
	$"9F8B 8840 5240 7F40 47"                             /* üãà@R@.@G */
};

resource 'MENU' (1, "Apple Menu", preload) {
	1,
	textMenuProc,
	0x7FFFFFFD,
	enabled,
	apple,
	{	/* array: 2 elements */
		/* [1] */
		"About MacSSH...", noIcon, noKey, noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (128, "Font") {
	128,
	textMenuProc,
	allEnabled,
	enabled,
	"Font",
	{	/* array: 0 elements */
	}
};

resource 'MENU' (129, "Size", preload) {
	129,
	textMenuProc,
	0x7FFFFF7F,
	enabled,
	"Size",
	{	/* array: 9 elements */
		/* [1] */
		"4", noIcon, noKey, noMark, plain,
		/* [2] */
		"9", noIcon, noKey, noMark, plain,
		/* [3] */
		"10", noIcon, noKey, noMark, plain,
		/* [4] */
		"11", noIcon, noKey, noMark, plain,
		/* [5] */
		"12", noIcon, noKey, noMark, plain,
		/* [6] */
		"14", noIcon, noKey, noMark, plain,
		/* [7] */
		"18", noIcon, noKey, noMark, plain,
		/* [8] */
		"-", noIcon, noKey, noMark, plain,
		/* [9] */
		"Other...", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (130, "Open Special Sub-Menu", preload) {
	130,
	textMenuProc,
	0x7FFFFFFD,
	enabled,
	"Favorites",
	{	/* array: 2 elements */
		/* [1] */
		"Edit Favorites...", noIcon, noKey, noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (131, "Preferences SubMenu", preload) {
	131,
	textMenuProc,
	allEnabled,
	enabled,
	"\0x00",
	{	/* array: 5 elements */
		/* [1] */
		"Global", noIcon, noKey, noMark, plain,
		/* [2] */
		"Terminals", noIcon, noKey, noMark, plain,
		/* [3] */
		"Sessions", noIcon, noKey, noMark, plain,
		/* [4] */
		"FTP Server", noIcon, noKey, noMark, plain,
		/* [5] */
		"FTP Users", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (132, "Translation SubMenu", preload) {
	132,
	textMenuProc,
	allEnabled,
	enabled,
	"\0x00",
	{	/* array: 0 elements */
	}
};

resource 'MENU' (133, "Font") {
	133,
	textMenuProc,
	allEnabled,
	enabled,
	"Font",
	{	/* array: 0 elements */
	}
};

resource 'MENU' (256, "File") {
	256,
	textMenuProc,
	0x7FFBED71,
	enabled,
	"File",
	{	/* array: 20 elements */
		/* [1] */
		"Open Connection…", noIcon, noKey, noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain,
		/* [3] */
		"Close", noIcon, noKey, noMark, plain,
		/* [4] */
		"-", noIcon, noKey, noMark, plain,
		/* [5] */
		"Load Set…", noIcon, noKey, noMark, plain,
		/* [6] */
		"Save Set…", noIcon, noKey, noMark, plain,
		/* [7] */
		"Save Set with Macros...", noIcon, noKey, noMark, plain,
		/* [8] */
		"-", noIcon, noKey, noMark, plain,
		/* [9] */
		"MacBinary II Enabled", noIcon, noKey, noMark, plain,
		/* [10] */
		"-", noIcon, noKey, noMark, plain,
		/* [11] */
		"Show FTP Log", noIcon, noKey, noMark, plain,
		/* [12] */
		"One-Time Passwords...", noIcon, noKey, noMark, plain,
		/* [13] */
		"-", noIcon, noKey, noMark, plain,
		/* [14] */
		"Print Selection...", noIcon, noKey, noMark, plain,
		/* [15] */
		"Print Screen…", noIcon, noKey, noMark, plain,
		/* [16] */
		"Page Setup...", noIcon, noKey, noMark, plain,
		/* [17] */
		"Save Selection to File...", noIcon, noKey, noMark, plain,
		/* [18] */
		"Upload File...", noIcon, noKey, noMark, plain,
		/* [19] */
		"-", noIcon, noKey, noMark, plain,
		/* [20] */
		"Quit", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (257, "Edit") {
	257,
	textMenuProc,
	0x7FFFDAFD,
	enabled,
	"Edit",
	{	/* array: 16 elements */
		/* [1] */
		"Undo", noIcon, noKey, noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain,
		/* [3] */
		"Cut", noIcon, noKey, noMark, plain,
		/* [4] */
		"Copy", noIcon, noKey, noMark, plain,
		/* [5] */
		"Paste", noIcon, noKey, noMark, plain,
		/* [6] */
		"Clear", noIcon, noKey, noMark, plain,
		/* [7] */
		"Copy Table", noIcon, noKey, noMark, plain,
		/* [8] */
		"Type Selection", noIcon, noKey, noMark, plain,
		/* [9] */
		"-", noIcon, noKey, noMark, plain,
		/* [10] */
		"Set Macros…", noIcon, noKey, noMark, plain,
		/* [11] */
		"-", noIcon, noKey, noMark, plain,
		/* [12] */
		"Preferences...", noIcon, noKey, noMark, plain,
		/* [13] */
		"Terminals...", noIcon, noKey, noMark, plain,
		/* [14] */
		"-", noIcon, noKey, noMark, plain,
		/* [15] */
		"FTP Server...", noIcon, noKey, noMark, plain,
		/* [16] */
		"FTP Users...", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (258, "Session", preload) {
	258,
	textMenuProc,
	0x7FFDF7EB,
	enabled,
	"Session",
	{	/* array: 39 elements */
		/* [1] */
		"Backspace", noIcon, noKey, noMark, plain,
		/* [2] */
		"Delete", noIcon, noKey, noMark, plain,
		/* [3] */
		"-", noIcon, noKey, noMark, plain,
		/* [4] */
		"Set Screen Size…", noIcon, noKey, noMark, plain,
		/* [5] */
		"-", noIcon, noKey, noMark, plain,
		/* [6] */
		"Setup Keys…", noIcon, noKey, noMark, plain,
		/* [7] */
		"Font", noIcon, hierarchicalMenu, "Ä", plain,
		/* [8] */
		"Bold Font", noIcon, hierarchicalMenu, "Ö", plain,
		/* [9] */
		"Size", noIcon, hierarchicalMenu, "Å", plain,
		/* [10] */
		"Color…", noIcon, noKey, noMark, plain,
		/* [11] */
		"ANSI Color...", noIcon, noKey, noMark, plain,
		/* [12] */
		"-", noIcon, noKey, noMark, plain,
		/* [13] */
		"Capture Session to File\0x00", noIcon, noKey, noMark, plain,
		/* [14] */
		"Queue Print Jobs", noIcon, noKey, noMark, plain,
		/* [15] */
		"Ignore Form Feeds", noIcon, noKey, noMark, plain,
		/* [16] */
		"Suspend Session", noIcon, noKey, noMark, plain,
		/* [17] */
		"Clear Saved Lines", noIcon, noKey, noMark, plain,
		/* [18] */
		"-", noIcon, noKey, noMark, plain,
		/* [19] */
		"Local Echo", noIcon, noKey, noMark, plain,
		/* [20] */
		"Wrap Mode", noIcon, noKey, noMark, plain,
		/* [21] */
		"BSD 4.3 CR mapping", noIcon, noKey, noMark, plain,
		/* [22] */
		"ANSI color sequences", noIcon, noKey, noMark, plain,
		/* [23] */
		"Xterm sequences", noIcon, noKey, noMark, plain,
		/* [24] */
		"8-bit connections", noIcon, noKey, noMark, plain,
		/* [25] */
		"Remap Keypad", noIcon, noKey, noMark, plain,
		/* [26] */
		"EMACS arrow mapping", noIcon, noKey, noMark, plain,
		/* [27] */
		"Map PgUp/PgDown/Home/End", noIcon, noKey, noMark, plain,
		/* [28] */
		"Translation", noIcon, hierarchicalMenu, "Ñ", plain,
		/* [29] */
		"Clear Screen Saves Lines", noIcon, noKey, noMark, plain,
		/* [30] */
		"Reset Terminal", noIcon, noKey, noMark, plain,
		/* [31] */
		"Jump Scroll", noIcon, noKey, noMark, plain,
		/* [32] */
		"TEK Page...", noIcon, noKey, noMark, plain,
		/* [33] */
		"TEK form feed clears screen", noIcon, noKey, noMark, plain,
		/* [34] */
		"Remap Del to ^D", noIcon, noKey, noMark, plain,
		/* [35] */
		"Allow boldfacing", noIcon, noKey, noMark, plain,
		/* [36] */
		"Use color for bold", noIcon, noKey, noMark, plain,
		/* [37] */
		"Use inverse for bold", noIcon, noKey, noMark, plain,
		/* [38] */
		"Ignore beeps", noIcon, noKey, noMark, plain,
		/* [39] */
		"Use Real Blinking", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (259, "Connections") {
	259,
	textMenuProc,
	0x7FFFFFF6,
	disabled,
	"Window",
	{	/* array: 4 elements */
		/* [1] */
		"Next Session", noIcon, "N", noMark, plain,
		/* [2] */
		"Change Window Title…", noIcon, noKey, noMark, plain,
		/* [3] */
		"Show Log", noIcon, "L", noMark, plain,
		/* [4] */
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (260, "Network", preload) {
	260,
	textMenuProc,
	0x7FFEBFFB,
	enabled,
	"Net",
	{	/* array: 18 elements */
		/* [1] */
		"Send FTP Command", noIcon, noKey, noMark, plain,
		/* [2] */
		"Send IP Address", noIcon, noKey, noMark, plain,
		/* [3] */
		"-", noIcon, noKey, noMark, plain,
		/* [4] */
		"Send \"Are You There?\"", noIcon, noKey, noMark, plain,
		/* [5] */
		"Send \"Abort Output\"", noIcon, noKey, noMark, plain,
		/* [6] */
		"Send \"Interrupt Process\"", noIcon, noKey, noMark, plain,
		/* [7] */
		"Send \"Erase Character\"", noIcon, noKey, noMark, plain,
		/* [8] */
		"Send \"Erase Line\"", noIcon, noKey, noMark, plain,
		/* [9] */
		"Send \"SYNC\"", noIcon, noKey, noMark, plain,
		/* [10] */
		"Send \"BRK (Break)\"", noIcon, noKey, noMark, plain,
		/* [11] */
		"Send IP + SYNC", noIcon, noKey, noMark, plain,
		/* [12] */
		"Send \"End of File\"", noIcon, noKey, noMark, plain,
		/* [13] */
		"Send \"SUSPend\"", noIcon, noKey, noMark, plain,
		/* [14] */
		"Send \"ABORT\"", noIcon, noKey, noMark, plain,
		/* [15] */
		"-", noIcon, noKey, noMark, plain,
		/* [16] */
		"Suspend Network", noIcon, noKey, noMark, plain,
		/* [17] */
		"-", noIcon, noKey, noMark, plain,
		/* [18] */
		"Show IP Address...", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (512, "nFile", preload) {
	512,
	textMenuProc,
	0x7FFBED71,
	enabled,
	"File",
	{	/* array: 20 elements */
		/* [1] */
		"Open Connection…", noIcon, "O", noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain,
		/* [3] */
		"Close", noIcon, "W", noMark, plain,
		/* [4] */
		"-", noIcon, noKey, noMark, plain,
		/* [5] */
		"Load Set…", noIcon, noKey, noMark, plain,
		/* [6] */
		"Save Set…", noIcon, noKey, noMark, plain,
		/* [7] */
		"Save Set with Macros...", noIcon, noKey, noMark, plain,
		/* [8] */
		"-", noIcon, noKey, noMark, plain,
		/* [9] */
		"MacBinary II Enabled", noIcon, noKey, noMark, plain,
		/* [10] */
		"-", noIcon, noKey, noMark, plain,
		/* [11] */
		"Show FTP Log", noIcon, noKey, noMark, plain,
		/* [12] */
		"One-Time Passwords...", noIcon, noKey, noMark, plain,
		/* [13] */
		"-", noIcon, noKey, noMark, plain,
		/* [14] */
		"Print Selection...", noIcon, "P", noMark, plain,
		/* [15] */
		"Print Screen…", noIcon, noKey, noMark, plain,
		/* [16] */
		"Page Setup...", noIcon, noKey, noMark, plain,
		/* [17] */
		"Save Selection to File...", noIcon, noKey, noMark, plain,
		/* [18] */
		"Upload File...", noIcon, noKey, noMark, plain,
		/* [19] */
		"-", noIcon, noKey, noMark, plain,
		/* [20] */
		"Quit", noIcon, "Q", noMark, plain
	}
};

resource 'MENU' (513, "nEdit", preload) {
	513,
	textMenuProc,
	0x7FFFDAFD,
	enabled,
	"Edit",
	{	/* array: 16 elements */
		/* [1] */
		"Undo", noIcon, "Z", noMark, plain,
		/* [2] */
		"-", noIcon, noKey, noMark, plain,
		/* [3] */
		"Cut", noIcon, "X", noMark, plain,
		/* [4] */
		"Copy", noIcon, "C", noMark, plain,
		/* [5] */
		"Paste", noIcon, "V", noMark, plain,
		/* [6] */
		"Clear", noIcon, noKey, noMark, plain,
		/* [7] */
		"Copy Table", noIcon, "B", noMark, plain,
		/* [8] */
		"Type Selection", noIcon, "D", noMark, plain,
		/* [9] */
		"-", noIcon, noKey, noMark, plain,
		/* [10] */
		"Set Macros…", noIcon, "M", noMark, plain,
		/* [11] */
		"-", noIcon, noKey, noMark, plain,
		/* [12] */
		"Preferences...", noIcon, noKey, noMark, plain,
		/* [13] */
		"Terminals...", noIcon, noKey, noMark, plain,
		/* [14] */
		"-", noIcon, noKey, noMark, plain,
		/* [15] */
		"FTP Server...", noIcon, noKey, noMark, plain,
		/* [16] */
		"FTP Users...", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (514, "Session", preload) {
	514,
	textMenuProc,
	0x7FFDF7EB,
	enabled,
	"Session",
	{	/* array: 39 elements */
		/* [1] */
		"Backspace", noIcon, noKey, noMark, plain,
		/* [2] */
		"Delete", noIcon, noKey, noMark, plain,
		/* [3] */
		"-", noIcon, noKey, noMark, plain,
		/* [4] */
		"Set Screen Size…", noIcon, noKey, noMark, plain,
		/* [5] */
		"-", noIcon, noKey, noMark, plain,
		/* [6] */
		"Setup Keys…", noIcon, "S", noMark, plain,
		/* [7] */
		"Font", noIcon, hierarchicalMenu, "Ä", plain,
		/* [8] */
		"Bold Font", noIcon, hierarchicalMenu, "Ö", plain,
		/* [9] */
		"Size", noIcon, hierarchicalMenu, "Å", plain,
		/* [10] */
		"Color…", noIcon, noKey, noMark, plain,
		/* [11] */
		"ANSI Color...", noIcon, noKey, noMark, plain,
		/* [12] */
		"-", noIcon, noKey, noMark, plain,
		/* [13] */
		"Capture Session to File\0x00", noIcon, "K", noMark, plain,
		/* [14] */
		"Queue Print Jobs", noIcon, noKey, noMark, plain,
		/* [15] */
		"Ignore Form Feeds", noIcon, noKey, noMark, plain,
		/* [16] */
		"Suspend Session", noIcon, noKey, noMark, plain,
		/* [17] */
		"Clear Saved Lines", noIcon, noKey, noMark, plain,
		/* [18] */
		"-", noIcon, noKey, noMark, plain,
		/* [19] */
		"Local Echo", noIcon, noKey, noMark, plain,
		/* [20] */
		"Wrap Mode", noIcon, noKey, noMark, plain,
		/* [21] */
		"BSD 4.3 CR mapping", noIcon, noKey, noMark, plain,
		/* [22] */
		"ANSI color sequences", noIcon, noKey, noMark, plain,
		/* [23] */
		"Xterm sequences", noIcon, noKey, noMark, plain,
		/* [24] */
		"8-bit connections", noIcon, noKey, noMark, plain,
		/* [25] */
		"Remap Keypad", noIcon, noKey, noMark, plain,
		/* [26] */
		"EMACS arrow mapping", noIcon, noKey, noMark, plain,
		/* [27] */
		"Map PgUp/PgDown/Home/End", noIcon, noKey, noMark, plain,
		/* [28] */
		"Translation", noIcon, hierarchicalMenu, "Ñ", plain,
		/* [29] */
		"Clear Screen Saves Lines", noIcon, noKey, noMark, plain,
		/* [30] */
		"Reset Terminal", noIcon, "R", noMark, plain,
		/* [31] */
		"Jump Scroll", noIcon, "J", noMark, plain,
		/* [32] */
		"TEK Page...", noIcon, noKey, noMark, plain,
		/* [33] */
		"TEK form feed clears screen", noIcon, noKey, noMark, plain,
		/* [34] */
		"Remap Del to ^D", noIcon, noKey, noMark, plain,
		/* [35] */
		"Allow boldfacing", noIcon, noKey, noMark, plain,
		/* [36] */
		"Use color for bold", noIcon, noKey, noMark, plain,
		/* [37] */
		"Use inverse for bold", noIcon, noKey, noMark, plain,
		/* [38] */
		"Ignore beeps", noIcon, noKey, noMark, plain,
		/* [39] */
		"Use Real Blinking", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (515, "nConnections", preload) {
	515,
	textMenuProc,
	0x7FFFFFF6,
	disabled,
	"Window",
	{	/* array: 4 elements */
		/* [1] */
		"Next Session", noIcon, noKey, noMark, plain,
		/* [2] */
		"Change Window Title…", noIcon, noKey, noMark, plain,
		/* [3] */
		"Show Log", noIcon, noKey, noMark, plain,
		/* [4] */
		"-", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (516, "nNetwork", preload) {
	516,
	textMenuProc,
	0x7FFEBFFB,
	enabled,
	"Net",
	{	/* array: 18 elements */
		/* [1] */
		"Send FTP Command", noIcon, noKey, noMark, plain,
		/* [2] */
		"Send IP Address", noIcon, noKey, noMark, plain,
		/* [3] */
		"-", noIcon, noKey, noMark, plain,
		/* [4] */
		"Send \"Are You There?\"", noIcon, noKey, noMark, plain,
		/* [5] */
		"Send \"Abort Output\"", noIcon, noKey, noMark, plain,
		/* [6] */
		"Send \"Interrupt Process\"", noIcon, noKey, noMark, plain,
		/* [7] */
		"Send \"Erase Character\"", noIcon, noKey, noMark, plain,
		/* [8] */
		"Send \"Erase Line\"", noIcon, noKey, noMark, plain,
		/* [9] */
		"Send \"SYNC\"", noIcon, noKey, noMark, plain,
		/* [10] */
		"Send \"BRK (Break)\"", noIcon, noKey, noMark, plain,
		/* [11] */
		"Send IP + SYNC", noIcon, noKey, noMark, plain,
		/* [12] */
		"Send \"End of File\"", noIcon, noKey, noMark, plain,
		/* [13] */
		"Send \"SUSPend\"", noIcon, noKey, noMark, plain,
		/* [14] */
		"Send \"ABORT\"", noIcon, noKey, noMark, plain,
		/* [15] */
		"-", noIcon, noKey, noMark, plain,
		/* [16] */
		"Suspend Network", noIcon, noKey, noMark, plain,
		/* [17] */
		"-", noIcon, noKey, noMark, plain,
		/* [18] */
		"Show IP Address…", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (1000, "Keypad") {
	1000,
	128,
	allEnabled,
	enabled,
	"Key",
	{	/* array: 0 elements */
	}
};

resource 'MENU' (1002, "Function") {
	1002,
	128,
	allEnabled,
	enabled,
	"Func",
	{	/* array: 0 elements */
	}
};

resource 'MENU' (1004) {
	1004,
	textMenuProc,
	allEnabled,
	enabled,
	"\0x00",
	{	/* array: 0 elements */
	}
};

resource 'MENU' (1005) {
	1005,
	textMenuProc,
	allEnabled,
	enabled,
	"\0x00",
	{	/* array: 0 elements */
	}
};

resource 'MENU' (2000) {
	2000,
	textMenuProc,
	allEnabled,
	enabled,
	"Protocol:",
	{	/* array: 6 elements */
		/* [1] */
		"Telnet", noIcon, noKey, noMark, plain,
		/* [2] */
		"rlogin", noIcon, noKey, noMark, plain,
		/* [3] */
		"rsh", noIcon, noKey, noMark, plain,
		/* [4] */
		"rexec", noIcon, noKey, noMark, plain,
		/* [5] */
		"ssh2 (Secure Shell)", noIcon, noKey, noMark, plain,
		/* [6] */
		"raw TCP", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2001) {
	2001,
	textMenuProc,
	allEnabled,
	enabled,
	"Encryption:",
	{	/* array: 9 elements */
		/* [1] */
		"All in order - minus <none>", noIcon, noKey, noMark, plain,
		/* [2] */
		"3DES", noIcon, noKey, noMark, plain,
		/* [3] */
		"Twofish", noIcon, noKey, noMark, plain,
		/* [4] */
		"Cast128", noIcon, noKey, noMark, plain,
		/* [5] */
		"Serpent", noIcon, noKey, noMark, plain,
		/* [6] */
		"AES (Rijndael 256)", noIcon, noKey, noMark, plain,
		/* [7] */
		"Blowfish", noIcon, noKey, noMark, plain,
		/* [8] */
		"Arcfour", noIcon, noKey, noMark, plain,
		/* [9] */
		"<none>", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2002) {
	2002,
	textMenuProc,
	allEnabled,
	enabled,
	"Key Set:",
	{	/* array: 12 elements */
		/* [1] */
		"Command+[0-9]", noIcon, noKey, noMark, plain,
		/* [2] */
		"Command+Shift+[0-9]", noIcon, noKey, noMark, plain,
		/* [3] */
		"F1-F10 (VT-220)", noIcon, noKey, noMark, plain,
		/* [4] */
		"Shift+F1-F10", noIcon, noKey, noMark, plain,
		/* [5] */
		"F11-F15(+Shift)", noIcon, noKey, noMark, plain,
		/* [6] */
		"PF1-4, PgUp, etc.", noIcon, noKey, noMark, plain,
		/* [7] */
		"Shift+\0x00PF1-4, PgUp, etc.", noIcon, noKey, noMark, plain,
		/* [8] */
		"Keypad 0-9 (app mode)", noIcon, noKey, noMark, plain,
		/* [9] */
		"Keypad symbols", noIcon, noKey, noMark, plain,
		/* [10] */
		"Arrow Keys", noIcon, noKey, noMark, plain,
		/* [11] */
		"F1-F10 (ANSI)", noIcon, noKey, noMark, plain,
		/* [12] */
		"F1-F10 (Linux)", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2003) {
	2003,
	textMenuProc,
	allEnabled,
	enabled,
	"Authentication:",
	{	/* array: 4 elements */
		/* [1] */
		"All in order - minus <none>", noIcon, noKey, noMark, plain,
		/* [2] */
		"SHA1", noIcon, noKey, noMark, plain,
		/* [3] */
		"MD5", noIcon, noKey, noMark, plain,
		/* [4] */
		"<none>", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2004) {
	2004,
	textMenuProc,
	allEnabled,
	enabled,
	"Compression:",
	{	/* array: 3 elements */
		/* [1] */
		"All in order", noIcon, noKey, noMark, plain,
		/* [2] */
		"<none>", noIcon, noKey, noMark, plain,
		/* [3] */
		"zlib", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2005) {
	2005,
	textMenuProc,
	allEnabled,
	enabled,
	"Method:",
	{	/* array: 4 elements */
		/* [1] */
		"Request pty (default)", noIcon, noKey, noMark, plain,
		/* [2] */
		"Local TCP port forward", noIcon, noKey, noMark, plain,
		/* [3] */
		"Remote TCP port forward", noIcon, noKey, noMark, plain,
		/* [4] */
		"CVS port forward", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2008) {
	2008,
	textMenuProc,
	allEnabled,
	enabled,
	"Port",
	{	/* array: 13 elements */
		/* [1] */
		"SSH", noIcon, noKey, noMark, plain,
		/* [2] */
		"Telnet", noIcon, noKey, noMark, plain,
		/* [3] */
		"SMTP", noIcon, noKey, noMark, plain,
		/* [4] */
		"HTTP", noIcon, noKey, noMark, plain,
		/* [5] */
		"POP2", noIcon, noKey, noMark, plain,
		/* [6] */
		"POP3", noIcon, noKey, noMark, plain,
		/* [7] */
		"NNTP", noIcon, noKey, noMark, plain,
		/* [8] */
		"IMAP2", noIcon, noKey, noMark, plain,
		/* [9] */
		"IRC", noIcon, noKey, noMark, plain,
		/* [10] */
		"IMAP3", noIcon, noKey, noMark, plain,
		/* [11] */
		"Timbuktu", noIcon, noKey, noMark, plain,
		/* [12] */
		"CVS", noIcon, noKey, noMark, plain,
		/* [13] */
		"other", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2010) {
	2010,
	textMenuProc,
	allEnabled,
	enabled,
	"NIST Level",
	{	/* array: 9 elements */
		/* [1] */
		"0 - 512 bit primes", noIcon, noKey, noMark, plain,
		/* [2] */
		"1", noIcon, noKey, noMark, plain,
		/* [3] */
		"2", noIcon, noKey, noMark, plain,
		/* [4] */
		"3", noIcon, noKey, noMark, plain,
		/* [5] */
		"4", noIcon, noKey, noMark, plain,
		/* [6] */
		"5", noIcon, noKey, noMark, plain,
		/* [7] */
		"6", noIcon, noKey, noMark, plain,
		/* [8] */
		"7", noIcon, noKey, noMark, plain,
		/* [9] */
		"8 - 1024 bit primes", noIcon, noKey, noMark, plain
	}
};

resource 'MENU' (2011, "Emulation") {
	2011,
	textMenuProc,
	allEnabled,
	enabled,
	"Emulation",
	{	/* array: 4 elements */
		/* [1] */
		"VT100", noIcon, noKey, noMark, plain,
		/* [2] */
		"VT220", noIcon, noKey, noMark, plain,
		/* [3] */
		"ANSI", noIcon, noKey, noMark, plain,
		/* [4] */
		"Linux", noIcon, noKey, noMark, plain
	}
};

data 'Mcmd' (1) {
	$"0000"                                               /* .. */
};

data 'Mcmd' (259) {
	$"0000"                                               /* .. */
};

data 'Mcmd' (515) {
	$"0000"                                               /* .. */
};

data 'Mcmd' (2001) {
	$"0009 0000 0000 0000 0000 0000 0000 0000"            /* .∆.............. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000"                                     /* ...... */
};

data 'Mcmd' (2002) {
	$"0000"                                               /* .. */
};

data 'Mcmd' (2003) {
	$"0004 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000"                                               /* .. */
};

data 'Mcmd' (2004) {
	$"0003 0000 0000 0000 0000 0000 0000"                 /* .............. */
};

data 'Mcmd' (2005) {
	$"0004 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000"                                               /* .. */
};

data 'Mcmd' (2006) {
	$"0006 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000"                           /* .......... */
};

data 'Mcmd' (2008) {
	$"000B 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000"                 /* .............. */
};

data 'Mcmd' (2010) {
	$"0009 0000 0000 0000 0000 0000 0000 0000"            /* .∆.............. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000"                                     /* ...... */
};

data 'Mcmd' (2011) {
	$"0004 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000"                                               /* .. */
};

data 'Mcro' (128) {
	$"210D 0D0D 0D0D 0D0D 0D0D 0D0D 0D0D 0D0D"            /* !............... */
	$"0D0D 0D0D 0D0D 5C30 3333 4F50 0D5C 3033"            /* ......\033OP.\03 */
	$"334F 510D 5C30 3333 4F52 0D5C 3033 334F"            /* 3OQ.\033OR.\033O */
	$"530D 5C30 3333 5B31 377E 0D5C 3033 335B"            /* S.\033[17~.\033[ */
	$"3138 7E0D 5C30 3333 5B31 397E 0D5C 3033"            /* 18~.\033[19~.\03 */
	$"335B 3230 7E0D 5C30 3333 5B32 317E 0D5C"            /* 3[20~.\033[21~.\ */
	$"3033 335B 3239 7E0D 0D0D 0D0D 0D5C 3033"            /* 033[29~......\03 */
	$"335B 3239 7E0D 5C30 3333 5B33 317E 0D5C"            /* 3[29~.\033[31~.\ */
	$"3033 335B 3332 7E0D 5C30 3333 5B33 337E"            /* 033[32~.\033[33~ */
	$"0D5C 3033 335B 3334 7E0D 5C30 3333 5B32"            /* .\033[34~.\033[2 */
	$"337E 0D5C 3033 335B 3234 7E0D 5C30 3333"            /* 3~.\033[24~.\033 */
	$"5B32 357E 0D5C 3033 335B 3236 7E0D 5C30"            /* [25~.\033[26~.\0 */
	$"3333 5B32 387E 0D0D 0D0D 0D0D 5C30 3333"            /* 33[28~......\033 */
	$"4F50 0D5C 3033 334F 510D 5C30 3333 4F52"            /* OP.\033OQ.\033OR */
	$"0D5C 3033 334F 530D 5C30 3333 5B35 7E0D"            /* .\033OS.\033[5~. */
	$"5C30 3333 5B36 7E0D 5C30 3333 5B31 7E0D"            /* \033[6~.\033[1~. */
	$"5C30 3333 5B34 7E0D 5C30 3333 5B32 7E0D"            /* \033[4~.\033[2~. */
	$"5C30 3333 5B33 7E0D 5C30 3333 4F50 0D5C"            /* \033[3~.\033OP.\ */
	$"3033 334F 510D 5C30 3333 4F52 0D5C 3033"            /* 033OQ.\033OR.\03 */
	$"334F 530D 5C30 3333 5B33 327E 0D5C 3033"            /* 3OS.\033[32~.\03 */
	$"335B 3331 7E0D 5C30 3333 5B33 337E 0D5C"            /* 3[31~.\033[33~.\ */
	$"3033 335B 3334 7E0D 5C30 3333 4F50 0D5C"            /* 033[34~.\033OP.\ */
	$"3033 335B 347E 0D5C 3033 334F 700D 5C30"            /* 033[4~.\033Op.\0 */
	$"3333 4F71 0D5C 3033 334F 720D 5C30 3333"            /* 33Oq.\033Or.\033 */
	$"4F73 0D5C 3033 334F 740D 5C30 3333 4F75"            /* Os.\033Ot.\033Ou */
	$"0D5C 3033 334F 760D 5C30 3333 4F77 0D5C"            /* .\033Ov.\033Ow.\ */
	$"3033 334F 780D 5C30 3333 4F79 0D5C 3033"            /* 033Ox.\033Oy.\03 */
	$"334F 6D0D 5C30 3333 4F6C 0D5C 3033 334F"            /* 3Om.\033Ol.\033O */
	$"6E0D 5C30 3333 4F4D 0D5C 3031 355C 3031"            /* n.\033OM.\015\01 */
	$"320D 0D0D 0D0D 0D5C 3033 335B 410D 5C30"            /* 2......\033[A.\0 */
	$"3333 5B42 0D5C 3033 335B 440D 5C30 3333"            /* 33[B.\033[D.\033 */
	$"5B43 0D0D 5C30 3333 4F41 0D5C 3033 334F"            /* [C..\033OA.\033O */
	$"420D 5C30 3333 4F44 0D5C 3033 334F 430D"            /* B.\033OD.\033OC. */
	$"0D5C 3033 3331 0D5C 3033 3332 0D5C 3033"            /* .\0331.\0332.\03 */
	$"3333 0D5C 3033 3334 0D5C 3033 3335 0D5C"            /* 33.\0334.\0335.\ */
	$"3033 3336 0D5C 3033 3337 0D5C 3033 3338"            /* 0336.\0337.\0338 */
	$"0D5C 3033 3339 0D5C 3033 3330 0D5C 3033"            /* .\0339.\0330.\03 */
	$"335B 5B41 0D5C 3033 335B 5B42 0D5C 3033"            /* 3[[A.\033[[B.\03 */
	$"335B 5B43 0D5C 3033 335B 5B44 0D5C 3033"            /* 3[[C.\033[[D.\03 */
	$"335B 5B45 0D5C 3033 335B 3137 7E0D 5C30"            /* 3[[E.\033[17~.\0 */
	$"3333 5B31 387E 0D5C 3033 335B 3139 7E0D"            /* 33[18~.\033[19~. */
	$"5C30 3333 5B32 307E 0D5C 3033 335B 3231"            /* \033[20~.\033[21 */
	$"7E0D"                                               /* ~. */
};

data 'NFNT' (9481, "%NCSA VT 9") {
	$"9000 0000 00D9 0006 FFFF FFF7 0006 000B"            /* ê....Ÿ..ˇˇˇ˜.... */
	$"031D 0009 0002 0000 0034 0000 0000 0000"            /* ...∆.....4...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 002B 4E22 0000"            /* ...........+N".. */
	$"0101 047E 0000 0104 8020 0000 0510 026A"            /* ...~....Ä .....j */
	$"9400 0000 0000 0000 0000 0000 0000 0000"            /* î............... */
	$"000E 0000 0000 0000 0000 0000 0000 1800"            /* ................ */
	$"0000 0001 8000 0010 D680 0000 0000 0000"            /* ....Ä...÷Ä...... */
	$"0000 0010 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0001 4852 0812 9101 0400 0000 0104"            /* ....HR..ë....... */
	$"8020 0000 0028 04B0 0022 100D 3004 8201"            /* Ä ...(.∞."..0.Ç. */
	$"9068 9040 3448 2003 0191 03CC 0000 0000"            /* êhê@4H ..ë.Ã.... */
	$"0000 0000 0000 2000 0000 0002 0000 0009"            /* ...... ........∆ */
	$"6B00 0000 0000 0000 0000 D539 EC98 4000"            /* k.........’9Ïò@. */
	$"0B97 385F 77DC E000 1CE7 79DE FFDD 1863"            /* .ó8_w‹‡..Áyﬁˇ›.c */
	$"08C5 DE77 9DF8 C631 8FF8 D115 CC42 149A"            /* .≈ﬁwù¯∆1è¯—.ÃB.ö */
	$"9101 0400 0000 0104 8020 0000 C739 DF8B"            /* ë.......Ä ..«9ﬂã */
	$"A241 2956 4808 452A 6DB1 08A5 5884 5294"            /* ¢A)VH.E*m±.•XÑRî */
	$"8249 0553 CE7A D03D C001 9102 7FE0 23B3"            /* ÇI.SŒz–=¿.ë..‡#≥ */
	$"8002 4002 0000 001C E73C 0005 5700 1403"            /* Ä.@.....Á<..W... */
	$"C000 DFD6 B2A5 5200 0C78 C4D0 8063 1041"            /* ¿.ﬂ÷≤•R..xƒ–Äc.A */
	$"2318 C631 8423 1865 0DE6 318C 6248 C62A"            /* #.∆1Ñ#.e.Ê1åbH∆* */
	$"8868 6B81 4853 8896 A101 0401 F800 0104"            /* àhkÅHSàñ°...¯... */
	$"8024 8885 28C6 30CC 6200 0000 3000 0000"            /* Ä$àÖ(∆0Ãb...0... */
	$"0000 0000 0000 003C 9214 7562 212F 0152"            /* .......<í.ub!/.R */
	$"2022 4A01 2540 24CC 4000 0032 0480 0023"            /*  "J.%@$Ã@..2.Ä.# */
	$"18D0 000A 5921 01FA 4000 8552 D442 E200"            /* .–.¬Y!.˙@.ÖR‘B‚. */
	$"1450 855E F063 1ABE 83D8 C611 8421 1869"            /* .PÖ^c.æÉÿ∆.Ñ!.i */
	$"0AD6 318C 6048 C624 88A4 47EB 4BB8 E3F2"            /* ¬÷1å`H∆$à§GÎK∏„Ú */
	$"4101 0400 0000 0104 8028 5709 08C6 10AC"            /* A.......Ä(W∆.∆.¨ */
	$"62F7 BDEF 7B9C E73A 52B3 9CE7 3A31 8C53"            /* b˜ΩÔ{úÁ:R≥úÁ:1åS */
	$"3B92 FD65 96AE 0FD2 7B24 3F91 915F 24CC"            /* ;í˝eñÆ.“{$?ëë_$Ã */
	$"5CE2 4027 3880 0023 18D1 400A AE02 A202"            /* \‚@'8Ä.#.—@¬Æ.¢. */
	$"4000 8FB9 4843 5F9F 1451 1A41 889D 1100"            /* @.èπHC_ü.Q.Aàù.. */
	$"455F FA11 F7A7 F871 08CE 3E8F 9C48 C624"            /* E_˙.˜ß¯q.Œ>èúH∆$ */
	$"5124 4380 7214 8084 3FFF FF80 07E0 01FF"            /* Q$CÄr.ÄÑ?ˇˇÄ.‡.ˇ */
	$"FFF0 357D 1FFE 1E9C 6318 C631 8C63 18C6"            /* ˇ5}.˛.úc.∆1åc.∆ */
	$"52CC 6318 C631 8C50 5609 FBE5 5880 027A"            /* RÃc.∆1åPV∆˚ÂXÄ.z */
	$"B5FA 4492 894A 23B4 6B34 7E42 4549 903F"            /* µ˙DíâJ#¥k4~BEIê? */
	$"F8DA BFF0 00FC 63FA 4000 8515 B542 4200"            /* ¯⁄ø.¸c˙@.Ö.µBB. */
	$"2452 07E1 8922 F0BE 89E8 C611 8423 1C69"            /* $R.·â"æâË∆.Ñ#.i */
	$"08C6 308C 4248 C6A4 2222 4114 2318 C084"            /* .∆0åBH∆§""A.#.¿Ñ */
	$"1030 0400 0000 0104 0228 4513 88C6 108C"            /* .0.......(E.à∆.å */
	$"6318 C631 8C3F FFFE 528C 6318 C631 8C50"            /* c.∆1å?ˇ˛Råc.∆1åP */
	$"5205 F963 9680 0FD3 3B21 9F94 914A 2004"            /* R.˘cñÄ.”;!üîëJ . */
	$"6F58 4342 3952 4823 18D2 E000 0002 A202"            /* oXCB9RH#.“‡...¢. */
	$"4000 0056 B224 0200 2454 4451 8922 1041"            /* @..V≤$..$TDQâ".A */
	$"0108 C631 8423 1C65 08C6 308C 6248 AB6A"            /* ..∆1Ñ#.e.∆0åbH´j */
	$"2422 4000 2214 83E4 1030 0400 0000 0104"            /* $"@.".É‰.0...... */
	$"0224 857D 08C6 308C 6339 CE73 9C21 0842"            /* .$Ö}.∆0åc9Œsú!.B */
	$"528C 6318 C673 9CC0 5632 7163 6100 0452"            /* Råc.∆sú¿V2qca..R */
	$"2020 0494 A54A 27FA A998 C282 4232 4823"            /*   .î•J'˙©ò¬ÇB2H# */
	$"18D2 8000 0021 23FA 4000 803A 4D18 0020"            /* .“Ä..!#˙@.Ä:M..  */
	$"C39F B84E 711C EA00 08E8 F9DE FC1D 1BA3"            /* √ü∏Nq.Í..Ë˘ﬁ¸..£ */
	$"F8C5 D074 5C47 1231 27F1 C02A 2214 8007"            /* ¯≈–t\G.1'Ò¿*".Ä. */
	$"1030 0400 001F 8104 0220 0021 08C5 DF8B"            /* .0....Å.. .!.≈ﬂã */
	$"9CD6 B5AD 6BDE F7BE 528B 9CE7 39AD 6B40"            /* ú÷µ≠kﬁ˜æRãúÁ9≠k@ */
	$"3BD1 016C 0E00 005D C0FF E4EB 7D4A 2006"            /* ;—.l...]¿ˇ‰Î}J . */
	$"DEE7 4082 03E9 9563 173D 6000 0000 1E03"            /* ﬁÁ@Ç.Èïc.=`..... */
	$"C000 0010 0000 0020 4000 0000 0000 0200"            /* ¿...... @....... */
	$"0000 0000 0000 0000 0000 0008 0000 0000"            /* ................ */
	$"0001 0000 0000 0000 0030 0400 0000 0104"            /* .........0...... */
	$"023F E043 C000 0000 0000 0000 0100 0000"            /* .?‡C¿........... */
	$"0000 0000 0000 0000 100E 0020 0000 0000"            /* ........... .... */
	$"0000 0080 0000 2000 0000 0002 0000 0000"            /* ...Ä.. ......... */
	$"0000 0000 0000 0200 0000 0000 0000 0040"            /* ...............@ */
	$"0000 0000 0000 0400 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0014 0000 0000"            /* ................ */
	$"0030 0400 0000 7F04 0220 0001 2000 0000"            /* .0....... .. ... */
	$"0000 0000 0200 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0080 0000 C000"            /* ...........Ä..¿. */
	$"0000 000C 0000 0000 0000 0000 0000 1C00"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0001 0004 0009 000E 0013"            /* ...........∆.... */
	$"0018 0019 001C 001F 0024 0029 002B 0030"            /* .........$.).+.0 */
	$"0031 0035 003A 003C 0041 0046 004B 0050"            /* .1.5.:.<.A.F.K.P */
	$"0055 005A 005F 0064 0065 0067 006A 006F"            /* .U.Z._.d.e.g.j.o */
	$"0072 0077 007C 0081 0086 008B 0090 0095"            /* .r.w.|.Å.Ü.ã.ê.ï */
	$"009A 009F 00A4 00A5 00AA 00AF 00B4 00B9"            /* .ö.ü.§.•.™.Ø.¥.π */
	$"00BE 00C3 00C8 00CD 00D2 00D7 00DC 00E1"            /* .æ.√.».Õ.“.◊.‹.· */
	$"00E6 00EB 00F0 00F5 00FA 00FC 0100 0102"            /* .Ê.Î..ı.˙.¸.... */
	$"0105 0105 010A 010F 0114 0119 011E 0123"            /* .....¬.........# */
	$"0126 012B 0130 0135 0138 013B 013F 0143"            /* .&.+.0.5.8.;.?.C */
	$"0149 014F 0155 015B 0161 0167 016B 016E"            /* .I.O.U.[.a.g.k.n */
	$"0174 017A 017B 017F 0183 0189 018E 0193"            /* .t.z.{...É.â.é.ì */
	$"0194 0194 0199 019E 01A3 01A8 01AD 01B2"            /* .î.î.ô.û.£.®.≠.≤ */
	$"01B7 01BC 01C1 01C6 01CB 01D0 01D5 01DA"            /* .∑.º.¡.∆.À.–.’.⁄ */
	$"01DF 01E4 01E9 01EE 01F0 01F2 01F5 01F8"            /* .ﬂ.‰.È.Ó..Ú.ı.¯ */
	$"01FD 0202 0207 020C 0211 0216 021B 0220"            /* .˝.............  */
	$"0225 022A 022D 0231 0236 023B 0240 0245"            /* .%.*.-.1.6.;.@.E */
	$"024A 024F 0254 0259 025F 0261 0264 0269"            /* .J.O.T.Y._.a.d.i */
	$"026E 0273 0278 027D 0280 0283 0288 028D"            /* .n.s.x.}.Ä.É.à.ç */
	$"0291 0296 029B 02A0 02A5 02A9 02AD 02B2"            /* .ë.ñ.õ.†.•.©.≠.≤ */
	$"02B7 02BC 02C1 02C2 02C7 02CC 02D1 02D6"            /* .∑.º.¡.¬.«.Ã.—.÷ */
	$"02DB 02E0 02E5 02EA 02EA 02EF 02F4 02F9"            /* .€.‡.Â.Í.Í.Ô.Ù.˘ */
	$"02FE 0303 0306 030C 0310 0314 0316 0318"            /* .˛.............. */
	$"031D 0322 0327 032D 0333 0100 0100 0100"            /* ...".'.-.3...... */
	$"0100 0100 0100 FFFF FFFF FFFF 0706 FFFF"            /* ......ˇˇˇˇˇˇ..ˇˇ */
	$"FFFF FFFF 0100 FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇ..ˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF 0706 0306 0206"            /* ˇˇˇˇˇˇˇˇˇˇ...... */
	$"0106 0106 0106 0106 0306 0206 0306 0106"            /* ................ */
	$"0106 0206 0106 0306 0206 0106 0306 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0306"            /* ................ */
	$"0206 0206 0106 0206 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0306 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0306 0206 0306 0206 0806 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0206 0106 0106 0106 0106"            /* ................ */
	$"0106 0306 0306 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0306 0106 0106 0106 0306 0106 0106"            /* ................ */
	$"0106 0006 0106 0306 0100 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0306"            /* ................ */
	$"0206 0206 0206 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0206 0206 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0306 0206 0106 0106 0106 0106 0106 0206"            /* ................ */
	$"0306 0106 0106 0206 0106 0106 0106 0106"            /* ................ */
	$"0206 0206 0106 0106 0106 0106 0406 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0706"            /* ................ */
	$"0106 0106 0106 0106 0106 0206 0106 0206"            /* ................ */
	$"0206 0306 0306 0106 0106 0106 0106 0106"            /* ................ */
	$"000B"                                               /* .. */
};

data 'NFNT' (9482, "%NCSA VT 10") {
	$"9000 0000 00FF 0006 FFFF FFFD 0006 000D"            /* ê....ˇ..ˇˇˇ˝.... */
	$"0447 000A 0003 0000 0040 0000 0000 0000"            /* .G.¬.....@...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0A80"            /* ..............¬Ä */
	$"0000 0000 2020 8FC0 0000 2090 0400 0000"            /* ....  è¿.. ê.... */
	$"A200 4552 8000 0004 0000 000C 0000 0000"            /* ¢.ERÄ........... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 020A A000"            /* .............¬†. */
	$"0000 0000 0A00 0000 0000 0042 0948 2AC1"            /* ....¬......B∆H*¡ */
	$"1008 1110 0000 0000 0000 0008 0001 8000"            /* ..............Ä. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0029"            /* ...............) */
	$"C440 0000 2020 8000 0000 2090 0400 0000"            /* ƒ@..  Ä... ê.... */
	$"0500 8A00 0442 00AA 0090 4012 2051 2080"            /* ..ä..B.™.ê@. Q Ä */
	$"2890 4000 100F 0000 0000 0000 0200 0000"            /* (ê@............. */
	$"0000 0004 0000 0000 0000 0000 0115 4000"            /* ..............@. */
	$"0000 0000 0004 0000 0000 00A5 1004 5422"            /* ...........•..T" */
	$"2804 2288 0000 2280 0000 D29C 8B12 4200"            /* (."à.."Ä..“úã.B. */
	$"0017 11CE 17CD F738 0000 0E31 3CEE 7FEE"            /* ...Œ.Õ˜8...1<Ó.Ó */
	$"8F7C 6118 BBCE 79DF 8C63 18FF C720 0569"            /* è|a.ªŒyﬂåc.ˇ« .i */
	$"0A41 0252 2020 8000 0000 2090 0400 0000"            /* ¬A.R  Ä... ê.... */
	$"423B F174 4825 2944 0108 A521 5AA2 114A"            /* B;ÒtH%)D..•!Z¢.J */
	$"5108 A511 10D0 001C 30CF 5007 BC00 0011"            /* Q.•..–..0œP.º... */
	$"033F F80A D380 0000 0C60 2000 0000 E780"            /* .?¯¬”Ä...` ...ÁÄ */
	$"013B 7011 5104 001C E400 000F 83FF FFF7"            /* .;p.Q...‰...Éˇˇ˜ */
	$"384E 8C62 128B D297 FF80 D2AB 4C94 2A80"            /* 8Nåb.ã“óˇÄ“´Lî*Ä */
	$"0018 B231 3411 18C4 0000 1149 2319 4211"            /* ..≤14..ƒ...I#.B. */
	$"8A0C A1BC C631 4624 8C63 1886 4120 2039"            /* ä.°º∆1F$åc.ÜA  9 */
	$"8842 9352 2020 8000 0000 2090 0400 0018"            /* àBìR  Ä... ê.... */
	$"4246 198C 4000 0000 0000 0000 0000 0000"            /* BF.å@........... */
	$"0000 0012 B910 07E2 4925 6A06 4C02 0611"            /* ....π..‚I%j.L... */
	$"00A0 9009 6EC0 0800 0880 2000 0109 1A00"            /* .†ê∆n¿...Ä ..∆.. */
	$"025B B010 1108 E021 2400 7048 1210 4928"            /* .[∞...‡!$.pH..I( */
	$"C491 8C62 2D75 A28A 5380 DFE9 5414 2700"            /* ƒëåb-u¢äSÄﬂÈT.'. */
	$"0029 D021 5420 18C7 C000 0199 2308 C210"            /* .)–!T .«¿..ô#.¬. */
	$"8A0D 21BC C631 4604 8C6A A50A 2150 1AA9"            /* ä.!º∆1F.åj•¬!P.© */
	$"0A71 12D4 2020 803F 0000 2090 0491 10A4"            /* ¬q.‘  Ä?.. ê.ë.§ */
	$"4242 198C 5EF7 BDEF 739C E763 6D67 39CE"            /* BB.å^˜ΩÔsúÁcmg9Œ */
	$"7463 18FD 550E 6EA4 B6D5 C006 4C02 198A"            /* tc.˝U.n§∂’¿.L..ä */
	$"89D0 97C9 6C5C F120 09E0 5000 0109 1A38"            /* â–ó…l\Ò ∆‡P..∆.8 */
	$"0369 D02A 2A09 0021 3F00 5448 1210 4928"            /* .i–**∆.!?.TH..I( */
	$"C551 8C63 8000 0002 5380 8298 A208 1710"            /* ≈QåcÄ...SÄÇò¢... */
	$"002A 9046 97BC 2747 C306 02AA BD08 FBD0"            /* .*êFóº'G√..™Ω.˚– */
	$"FA0E 215A C631 45C4 8C6A 4512 2150 0029"            /* ˙.!Z∆1EƒåjE.!P.) */
	$"771C 7E48 2020 8000 0000 2090 050A E120"            /* w.~H  Ä... ê.¬·  */
	$"A543 D58C 6318 C631 8C63 18A1 2598 C631"            /* •C’åc.∆1åc.°%ò∆1 */
	$"8C63 1890 53D1 FEA8 B695 4017 D40F E064"            /* åc.êS—˛®∂ï@.‘.‡d */
	$"8A50 9288 D46B 3000 088C 50B4 0295 1BD4"            /* äPíà‘k0..åP¥.ï.‘ */
	$"0372 E22A 2A13 C67F A400 78AF 2BDE 4928"            /* .r‚**.∆.§.xØ+ﬁI( */
	$"C7F1 8C62 8000 0002 5380 828C 5508 1A90"            /* «ÒåbÄ...SÄÇåU..ê */
	$"004C 9081 FC62 28BC 0CF9 84AA A308 C213"            /* .LêÅ¸b(º.˘Ñ™£.¬. */
	$"8A0E 211A C7D1 7824 8AAA 4212 1188 054E"            /* ä.!.«—x$ä™B..à.N */
	$"4290 1087 FFFF F000 FC00 3FFF FE06 AFA2"            /* Bê.áˇˇ.¸.?ˇ˛.Ø¢ */
	$"A542 158C 6318 C631 87FF FFA1 2518 C631"            /* •B.åc.∆1áˇˇ°%.∆1 */
	$"8C63 1890 5111 F6A6 A690 01FE 55B2 199F"            /* åc.êQ.ˆ¶¶ê.˛U≤.ü */
	$"8C48 9288 046F 5120 1093 494A 0295 1A5C"            /* åHíà.oQ .ìIJ.ï.\ */
	$"0000 0045 4421 0925 24C0 10A8 2A10 4928"            /* ...ED!∆%$¿.®*.I( */
	$"C7D1 8C62 8000 0002 5380 8FCA ACA8 127C"            /* «—åbÄ...SÄè ¨®.| */
	$"F888 9101 1062 4884 3000 649F E308 C211"            /* ¯àë..bHÑ0.dü„.¬. */
	$"8A8D 2119 C611 5024 8A94 A222 1100 0004"            /* äç!.∆.P$äî¢".... */
	$"6318 1082 0600 8000 0000 2080 4508 A271"            /* c..Ç..Ä... ÄE.¢q */
	$"FFC2 138C 6318 C631 8421 0821 2518 C631"            /* ˇ¬.åc.∆1Ñ!.!%.∆1 */
	$"8C63 1890 550E 62A1 B6D0 004A 6552 0604"            /* åc.êU.b°∂–.JeR.. */
	$"8C48 9289 FC69 513F D08C 8A85 07FF 1A53"            /* åHíâ¸iQ?–åäÖ.ˇ.S */
	$"FC00 0FA9 4443 D0A5 24C0 3FF8 7E10 4928"            /* ¸..©DC–•$¿?¯~.I( */
	$"C7D1 8C62 8000 0002 5380 052B 2C48 1013"            /* «—åbÄ...SÄ.+,H.. */
	$"0688 9211 1062 488B CCF9 8044 6319 4211"            /* .àí..bHãÃ˘ÄDc.B. */
	$"8A8C A119 C611 4A24 8915 1242 0900 0A84"            /* äå°.∆.J$â..B∆.¬Ñ */
	$"4290 7C82 0600 8000 0000 2080 4490 AFA1"            /* Bê|Ç..Ä... ÄDêØ° */
	$"18C6 138C 6739 CE73 8C63 18A1 2518 C631"            /* .∆.åg9Œsåc.°%.∆1 */
	$"8CE7 3990 3A21 02A1 4920 01FA 65B0 001F"            /* åÁ9ê:!.°I .˙e∞.. */
	$"DC90 9288 02A9 9221 5093 854A 0463 1A54"            /* ‹êíà.©í!PìÖJ.c.T */
	$"0000 0028 8441 0925 243D EB18 4610 4928"            /* ...(ÑA∆%$=Î.F.I( */
	$"C7F1 8C62 8000 0002 5380 851D 13A4 2013"            /* «ÒåbÄ...SÄÖ..§ . */
	$"0707 7FEE 179C 4733 C306 043C 7CEE 7E0E"            /* ...Ó.úG3√..<|Ó~. */
	$"8F74 7F18 BA0E 45C4 7115 127E 0900 0004"            /* èt..∫.Eƒq..~∆... */
	$"4290 00E2 0600 8000 03F0 2080 4400 0421"            /* Bê.‚..Ä.. ÄD..! */
	$"18BB F173 9AD6 B5AD 739C E731 B717 39CE"            /* .ªÒsö÷µ≠súÁ1∑.9Œ */
	$"735A D690 13E1 02AE 30C0 010B F80F FFE4"            /* sZ÷ê.·.Æ0¿..¯.ˇ‰ */
	$"AB10 9288 06DF E421 5080 FCB4 AC62 E7B8"            /* ´.íà.ﬂ‰!PÄ¸¥¨bÁ∏ */
	$"0000 0210 8480 E625 3F3D BF1F C7FF FFF7"            /* ....ÑÄÊ%?=ø.«ˇˇ˜ */
	$"39CE 739C C000 0003 DF80 0008 0004 2001"            /* 9Œsú¿...ﬂÄ.... . */
	$"0100 0000 0000 0000 4000 0000 0000 0000"            /* ........@....... */
	$"0000 0000 0002 0000 0000 0002 0107 C540"            /* ..............≈@ */
	$"0000 0000 0600 8000 0000 2080 47FC 0878"            /* ......Ä... ÄG¸.x */
	$"0010 0000 0000 0000 2000 0000 0000 0000"            /* ........ ....... */
	$"0000 0010 101E 0280 0000 0000 4000 0000"            /* .......Ä....@... */
	$"8020 9028 0001 0460 2080 0000 0000 0000"            /* Ä ê(...` Ä...... */
	$"0000 0011 0080 0000 0414 8000 0000 0000"            /* .....Ä....Ä..... */
	$"0000 0000 0000 0820 0000 0000 0002 4002"            /* ....... ......@. */
	$"0000 0000 0000 0000 8000 0000 0000 0000"            /* ........Ä....... */
	$"0000 0000 0001 8000 0000 0002 0100 0000"            /* ......Ä......... */
	$"0000 0000 0600 8000 0000 2080 4400 0024"            /* ......Ä... ÄD..$ */
	$"0008 0000 0000 0000 1000 0000 0000 0000"            /* ................ */
	$"0000 0010 0000 0280 0000 0000 0000 0000"            /* .......Ä........ */
	$"803E 9010 0000 03A0 2280 0000 0000 0000"            /* Ä>ê....†"Ä...... */
	$"0000 0002 0000 0000 0429 0000 0000 0000"            /* .........)...... */
	$"0000 0000 0000 0440 0000 0000 0001 8000"            /* .......@......Ä. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0003 8700 0A80"            /* ............á.¬Ä */
	$"0000 0000 0600 8000 000F E080 4400 0000"            /* ......Ä...‡ÄD... */
	$"0010 0000 0000 0000 2000 0000 0000 0000"            /* ........ ....... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0100 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0820 0000 0000 0000 0000"            /* ....... ........ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0001"            /* ................ */
	$"0004 000A 000F 0015 001B 001C 0020 0024"            /* ...¬......... .$ */
	$"0029 002E 0030 0035 0037 003C 0041 0046"            /* .)...0.5.7.<.A.F */
	$"004B 0050 0055 005A 005F 0064 0069 006E"            /* .K.P.U.Z._.d.i.n */
	$"0070 0072 0078 007D 0083 0088 008D 0092"            /* .p.r.x.}.É.à.ç.í */
	$"0097 009C 00A1 00A6 00AB 00B0 00B5 00B8"            /* .ó.ú.°.¶.´.∞.µ.∏ */
	$"00BD 00C2 00C7 00CC 00D1 00D6 00DB 00E1"            /* .Ω.¬.«.Ã.—.÷.€.· */
	$"00E6 00EB 00F0 00F5 00FA 00FF 0104 0109"            /* .Ê.Î..ı.˙.ˇ...∆ */
	$"010E 0111 0115 0118 011D 0122 0124 012A"            /* ...........".$.* */
	$"012F 0134 0139 013E 0141 0146 014B 0150"            /* ./.4.9.>.A.F.K.P */
	$"0153 0156 015A 015E 0164 016A 0170 0176"            /* .S.V.Z.^.d.j.p.v */
	$"017C 0182 0186 0189 018F 0195 0196 019A"            /* .|.Ç.Ü.â.è.ï.ñ.ö */
	$"019E 01A4 01A9 01AE 01AF 01AF 01B4 01B9"            /* .û.§.©.Æ.Ø.Ø.¥.π */
	$"01BE 01C3 01C8 01CD 01D2 01D7 01DC 01E1"            /* .æ.√.».Õ.“.◊.‹.· */
	$"01E6 01EB 01F0 01F5 01FA 01FF 0204 0209"            /* .Ê.Î..ı.˙.ˇ...∆ */
	$"020D 0211 0214 0217 021C 0221 0226 022B"            /* ...........!.&.+ */
	$"0230 0235 023A 023F 0244 0249 024E 0251"            /* .0.5.:.?.D.I.N.Q */
	$"0256 025B 0260 0264 026A 0270 0276 027C"            /* .V.[.`.d.j.p.v.| */
	$"0282 0284 0287 028C 0291 0297 029C 02A1"            /* .Ç.Ñ.á.å.ë.ó.ú.° */
	$"02A6 02AB 02B0 02B5 02BA 02BF 02C5 02CA"            /* .¶.´.∞.µ.∫.ø.≈.  */
	$"02CF 02D2 02D5 02DA 02DF 02E5 02EA 02EB"            /* .œ.“.’.⁄.ﬂ.Â.Í.Î */
	$"02F0 02F6 02FB 0300 0306 030B 0310 0315"            /* ..ˆ.˚.......... */
	$"0315 031A 031F 0324 0329 032E 0331 0336"            /* .......$.)...1.6 */
	$"033B 0340 0342 0344 0349 034E 0353 0358"            /* .;.@.B.D.I.N.S.X */
	$"035E 0363 0366 0369 036E 0373 0378 037A"            /* .^.c.f.i.n.s.x.z */
	$"037C 0381 0387 038C 0391 0396 039B 03A0"            /* .|.Å.á.å.ë.ñ.õ.† */
	$"03A3 03A6 03A9 03AC 03B1 03B6 03BB 03C0"            /* .£.¶.©.¨.±.∂.ª.¿ */
	$"03C5 03CA 03CF 03D2 03D5 03D9 03DC 03DF"            /* .≈. .œ.“.’.Ÿ.‹.ﬂ */
	$"03E1 03E4 03E6 03E9 03EB 03EE 03F2 0100"            /* .·.‰.Ê.È.Î.Ó.Ú.. */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"0106 FFFF FFFF FFFF 0106 FFFF FFFF FFFF"            /* ..ˇˇˇˇˇˇ..ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF 0106"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ.. */
	$"0306 0206 0106 0106 0106 0106 0406 0206"            /* ................ */
	$"0206 0106 0106 0306 0106 0306 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0306 0306 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0206 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0306 0206 0206 0106 0106 0306"            /* ................ */
	$"0106 0106 0106 0106 0106 0206 0106 0106"            /* ................ */
	$"0106 0106 0106 0306 0306 0006 0006 0106"            /* ................ */
	$"0106 0106 0106 0306 0106 0106 0106 0306"            /* ................ */
	$"0206 0106 0106 0106 0106 0306 0100 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0206 0106 0206 0206 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0206 0106 0106 0106 0206 0106 0106 0106"            /* ................ */
	$"0106 0106 0306 0206 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0206 0206 0106 0106 0106 0106"            /* ................ */
	$"0406 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0306 0306 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0306 0206 0106 0106 0106"            /* ................ */
	$"0306 0306 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0206 0206 0206 0206 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0206 0206 0206 0206"            /* ................ */
	$"0206 0306 0206 0206 0306 0206 0106 0206"            /* ................ */
	$"FFFF"                                               /* ˇˇ */
};

data 'NFNT' (9484, "%NCSA VT 12") {
	$"9000 0000 00FF 0007 0000 FFF4 0008 000F"            /* ê....ˇ....ˇÙ.... */
	$"04F4 000C 0003 0000 0043 0000 0000 0000"            /* .Ù.......C...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0559"            /* ...............Y */
	$"0000 0000 0100 811F C000 0002 0440 0800"            /* ......Å.¿....@.. */
	$"0000 0400 1A00 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0003 5A00 0000 0000 0014 0000 0000 0000"            /* ..Z............. */
	$"0841 2909 2C22 0081 1100 0000 0000 0000"            /* .A)∆,".Å........ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 02A9 0000 0000 0100 8100 0000"            /* .....©......Å... */
	$"0002 0440 0800 0001 4A00 ACA5 0000 0000"            /* ...@....J.¨•.... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 1800 0000"            /* ................ */
	$"0000 0180 0000 0045 AC00 0000 0000 0000"            /* ...Ä...E¨....... */
	$"0000 0000 0000 14A2 0092 8245 0042 2880"            /* .......¢.íÇE.B(Ä */
	$"0000 0052 2000 0144 0000 C000 0008 0000"            /* ...R ..D..¿..... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 003C 1C00 2559 0000 0000"            /* .......<..%Y.... */
	$"0100 8100 0000 0002 0440 0800 0000 0401"            /* ..Å......@...... */
	$"0000 0000 0008 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 00E0 0003 8700 3B00 0000 0000 8800"            /* ...‡..á.;.....à. */
	$"0000 2000 0000 0000 3200 0000 0020 0000"            /* .. .....2.... .. */
	$"0000 0000 0000 0000 0000 0000 0000 0004"            /* ................ */
	$"4000 0000 0002 3411 D851 4000 D14E 44C5"            /* @.....4.ÿQ@.—ND≈ */
	$"2200 000B 939C 27CD F738 0000 E01D E77B"            /* "...ìú'Õ˜8..‡.Á{ */
	$"FF74 618C 2317 79DE 77E3 0631 8FE4 0480"            /* ˇtaå#.yﬁw„.1è‰.Ä */
	$"12AF 3888 0000 0100 8100 0000 0002 0440"            /* .Ø8à....Å......@ */
	$"0800 0001 C077 E2E8 8884 0354 0120 8064"            /* ....¿w‚ËàÑ.T. Äd */
	$"1A24 100D 1208 0230 0D10 3EE4 48BD 5B03"            /* .$.....0..>‰HΩ[. */
	$"E700 0048 8801 FFE0 23DC E000 0900 2200"            /* Á..Hà.ˇ‡#‹‡.∆.". */
	$"0000 0073 9CF8 0004 EDC0 0022 13C0 18F1"            /* ...sú¯..Ì¿.".¿.Ò */
	$"0008 1DF7 7FE9 12E7 04E8 C625 5BEE E4A0"            /* ...˜.È.Á.Ë∆%[Ó‰† */
	$"9FF0 D7F5 A525 2A80 0014 7462 6410 18C4"            /* ü◊ı•%*Ä..tbd..ƒ */
	$"0411 17A3 18C6 108C 6194 3718 C631 8923"            /* ...£.∆.åaî7.∆1â# */
	$"0631 8862 0540 0D59 2148 3025 2100 8100"            /* .1àb.@.Y!H0%!.Å. */
	$"0000 0002 0440 0800 0002 2E8C 2318 904A"            /* .....@.....å#.êJ */
	$"5588 0211 4A9B 6C42 2956 2114 A248 1090"            /* Uà..JõlB)V!.¢H.ê */
	$"751B 3257 8004 88C0 0084 5019 1440 2463"            /* u.2WÄ.à¿.ÑP..@$c */
	$"1000 0900 2200 1000 008C 6320 0009 264C"            /* ..∆."....åc .∆&L */
	$"02A2 1420 2111 0014 A308 C209 1318 8918"            /* .¢. !...£.¬∆..â. */
	$"C628 8000 1800 1110 D295 A926 1710 0014"            /* ∆(Ä.....“ï©&.... */
	$"5042 A420 18C4 0808 1863 18C6 1084 61A4"            /* PB§ .ƒ...c.∆.Ña§ */
	$"2B98 C631 8122 8A31 8862 0620 02A9 3108"            /* +ò∆1Å"ä1àb. .©1. */
	$"4935 2100 8100 3F80 0002 0440 0800 0032"            /* I5!.Å.?Ä...@...2 */
	$"318C 3318 8000 0000 0000 0000 0000 0000"            /* 1å3.Ä........... */
	$"0000 0FC9 1140 751A B557 0004 8880 1102"            /* ...….@u.µW..àÄ.. */
	$"5004 8440 2463 1000 4000 2200 1000 008C"            /* P.Ñ@$c..@."....å */
	$"6320 000D CB8C 2022 2400 2117 C015 2308"            /* c ..Àå "$.!.¿.#. */
	$"C209 1318 8918 C620 0000 0000 1110 8FCC"            /* ¬∆..â.∆ ......èÃ */
	$"4942 1710 0024 D04D 27BC 2747 D3E4 2B63"            /* IB...$–M'º'G”‰+c */
	$"E846 1084 61C4 2358 C631 8122 8A2A 88A1"            /* ËF.Ñaƒ#X∆1Å"ä*à° */
	$"0400 0559 E14E 492D 4100 8100 0000 0002"            /* ...Y·NI-A.Å..... */
	$"0440 0922 214A 3184 2B18 BDEF 7BDE E739"            /* .@∆"!J1Ñ+.ΩÔ{ﬁÁ9 */
	$"CE94 ACE7 39CE 8C63 1233 9127 752A B455"            /* Œî¨Á9Œåc.3ë'u*¥U */
	$"0014 8980 1201 244E 445F 24E3 17C7 8900"            /* ..âÄ..$ND_$„.«â. */
	$"2700 2800 008C 6323 6000 0000 2462 2FC0"            /* '.(..åc#`...$b/¿ */
	$"7FD1 000A 2308 C209 1318 B718 C630 0000"            /* .—.¬#.¬∆..∑.∆0.. */
	$"0000 1110 8506 1082 1AFC F825 5083 F862"            /* ....Ö..Ç.¸¯%PÉ¯b */
	$"48C7 E002 4D7F 1847 DE9F E184 2338 FA3E"            /* H«‡.M..Gﬁü·Ñ#8˙> */
	$"7122 8A24 5121 0400 02A0 AEE3 B7E4 8FFF"            /* q"ä$Q!...†Æ„∑‰èˇ */
	$"FFE0 007F 0003 FFFF FA15 C246 3187 A718"            /* ˇ‡....ˇˇ˙.¬F1áß. */
	$"C631 8C63 18C6 3194 B318 C631 8C63 1205"            /* ∆1åc.∆1î≥.∆1åc.. */
	$"789F F52B 3440 00FF EA9B 7D02 FC52 244A"            /* xüı+4@.ˇÍõ}.¸R$J */
	$"235D 1929 89F8 4231 284C 808C 633C 9000"            /* #].)â¯B1(LÄåc<ê. */
	$"003F 5454 4406 2511 3004 3FEF FBC9 1318"            /* .?TTD.%.0.?Ô˚….. */
	$"FD18 C630 0000 0000 1110 8505 2542 1210"            /* ˝.∆0......Ö.%B.. */
	$"0046 5102 2062 48BC 13E4 4D63 1846 108C"            /* .FQ. bHº.‰Mc.F.å */
	$"61C4 2318 C231 0922 52AA 2220 8400 0550"            /* aƒ#.¬1∆"R™" Ñ..P */
	$"8852 0108 7018 0100 0000 0002 0400 8C0D"            /* àR..p.........å. */
	$"5F43 FF84 2318 C631 8C63 0FFF FF94 A318"            /* _CˇÑ#.∆1åc.ˇˇî£. */
	$"C631 8C63 1205 105F BD1A B540 0024 8CA4"            /* ∆1åc..._Ω.µ@.$å§ */
	$"9084 2462 444A 2001 19EA 910E 424E 4492"            /* êÑ$bDJ ..Íë.BNDí */
	$"40FF E324 FFF0 0000 8C48 4FC9 2511 300A"            /* @ˇ„$ˇ..åHO…%.0¬ */
	$"A308 C209 1318 FD18 C630 0000 0000 1110"            /* £.¬∆..˝.∆0...... */
	$"0015 2B2A 1010 0044 5202 2062 4884 0808"            /* ..+*...DR. bHÑ.. */
	$"0BA3 1846 108C 71A4 2318 C231 0922 5371"            /* .£.F.åq§#.¬1∆"Sq */
	$"2420 8400 02A0 8C63 0108 2018 0100 0000"            /* $ Ñ..†åc.. ..... */
	$"0002 0400 8A11 44E2 3184 2318 C631 8C63"            /* ....ä.D‚1Ñ#.∆1åc */
	$"0842 1094 A318 C631 8C63 1205 132F 951A"            /* .B.î£.∆1åc.../ï. */
	$"B240 00FC 88A4 9048 FC62 844A 27FE A90C"            /* ≤@.¸à§êH¸bÑJ'˛©. */
	$"A10A 4231 4521 208C 6324 8000 000C 5448"            /* °¬B1E! åc$Ä...TH */
	$"8410 A511 0015 6308 C209 1318 FD18 C630"            /* Ñ.•...c.¬∆..˝.∆0 */
	$"0000 0000 1110 8015 4B11 2003 0684 5422"            /* ......Ä.K. ..ÑT" */
	$"2462 488B C410 4823 18C6 108C 7194 2318"            /* $bHãƒ.H#.∆.åqî#. */
	$"C231 8922 2231 2420 4400 0550 8852 07C8"            /* ¬1â""1$ D..PàR.» */
	$"2018 0100 0000 0002 0400 8921 5F42 318C"            /*  .........â!_B1å */
	$"2318 CE73 9CE7 18C6 3194 A318 C631 9CE7"            /* #.ŒsúÁ.∆1î£.∆1úÁ */
	$"3005 5317 1514 4880 0044 989B 0000 24E5"            /* 0.S...HÄ.Dòõ..$Â */
	$"144A 2000 AB28 A301 424E 8292 408C 6324"            /* .J .´(£.BNÇí@åc$ */
	$"9000 000C 2448 8429 2511 0F65 6308 C209"            /* ê...$HÑ)%..ec.¬∆ */
	$"1318 FF18 C630 0000 0000 1110 800E 44E9"            /* ..ˇ.∆0......Ä.DÈ */
	$"2003 0683 BFDC 239C 4733 C000 47A3 E77B"            /*  ..Éø‹#úG3¿.G£Á{ */
	$"F074 6E8F E317 41D1 711C 2231 27E0 441F"            /* tnè„.A—q."1'‡D. */
	$"C2A0 8852 000E 2018 0100 0000 FE02 0400"            /* ¬†àR.. .....˛... */
	$"8800 0842 3177 E2E7 35AD 6B5A E739 CE94"            /* à..B1w‚Á5≠kZÁ9Œî */
	$"A2E7 39CE 6B5A D003 ACE0 1763 8700 0004"            /* ¢Á9ŒkZ–.¨‡.cá... */
	$"E700 7FFF 2759 F44A 2001 B5D7 1D01 4200"            /* Á..ˇ'YÙJ .µ◊..B. */
	$"FE4C 958C 5CFB 6000 0000 23C9 03C6 251F"            /* ˛Lïå\˚`...#….∆%. */
	$"CF62 A3F8 FFE9 12E7 34E7 39D0 0000 0000"            /* œb£¯ˇÈ.Á4Á9–.... */
	$"1FF0 0004 0000 C001 0100 0000 0000 0000"            /* .....¿......... */
	$"4000 0000 0000 0000 0000 0000 0080 0000"            /* @............Ä.. */
	$"0000 0038 3C00 0550 0000 0000 0018 0100"            /* ...8<..P........ */
	$"0000 0002 0400 8FF8 10F0 0020 0000 0000"            /* ......è¯.. .... */
	$"0000 4000 0000 0000 0000 0000 0001 0000"            /* ..@............. */
	$"0100 0000 0000 0000 0000 0400 0000 2000"            /* .............. . */
	$"0000 0000 8200 0000 0000 0000 0000 0000"            /* ....Ç........... */
	$"0041 0000 0001 0520 0000 0000 0000 0000"            /* .A..... ........ */
	$"0000 0000 0204 0000 0000 0000 0002 0100"            /* ................ */
	$"0000 0000 0000 8000 0000 0000 0000 0000"            /* ......Ä......... */
	$"0000 0040 0000 0000 0000 2000 02A0 0000"            /* ...@...... ..†.. */
	$"0000 0018 0100 0000 0002 0400 8800 0000"            /* ............à... */
	$"0020 0000 0000 0000 4000 0000 0000 0000"            /* . ......@....... */
	$"0000 0000 0000 0100 0000 0000 0000 0000"            /* ................ */
	$"0400 0000 C000 0000 0000 8C00 0000 0000"            /* ....¿.....å..... */
	$"0000 0000 0000 0440 0000 0001 0A40 0000"            /* .......@....¬@.. */
	$"0000 0000 0000 0000 0000 0108 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0550 0000 0000 0018 0100 0000 01FE"            /* ...P...........˛ */
	$"0400 8800 0000 0040 0000 0000 0000 8000"            /* ..à....@......Ä. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0400 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0380 0000"            /* .............Ä.. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0204 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0001 0004 000B 0010"            /* ................ */
	$"0017 001D 001E 0021 0024 0029 002E 0030"            /* .......!.$.)...0 */
	$"0035 0037 003D 0042 0045 004A 004F 0055"            /* .5.7.=.B.E.J.O.U */
	$"005A 005F 0064 0069 006E 0070 0072 0076"            /* .Z._.d.i.n.p.r.v */
	$"007B 007F 0084 008A 008F 0094 0099 009E"            /* .{...Ñ.ä.è.î.ô.û */
	$"00A3 00A8 00AD 00B2 00B3 00B8 00BD 00C2"            /* .£.®.≠.≤.≥.∏.Ω.¬ */
	$"00C7 00CC 00D1 00D6 00DB 00E0 00E5 00EA"            /* .«.Ã.—.÷.€.‡.Â.Í */
	$"00EF 00F6 00FB 0100 0105 010A 010D 0113"            /* .Ô.ˆ.˚.....¬.... */
	$"0116 011B 0122 0125 012C 0132 0137 013C"            /* .....".%.,.2.7.< */
	$"0141 0145 014A 014F 0154 0158 015C 0160"            /* .A.E.J.O.T.X.\.` */
	$"0164 016B 0172 0179 0180 0187 018E 0192"            /* .d.k.r.y.Ä.á.é.í */
	$"0196 019D 01A4 01A5 01A9 01AD 01B3 01B8"            /* .ñ.ù.§.•.©.≠.≥.∏ */
	$"01BD 01BE 01BE 01C3 01C8 01CD 01D2 01D7"            /* .Ω.æ.æ.√.».Õ.“.◊ */
	$"01DC 01E1 01E6 01EB 01F0 01F5 01FA 01FF"            /* .‹.·.Ê.Î..ı.˙.ˇ */
	$"0204 0209 020E 0213 0218 021A 021C 021F"            /* ...∆............ */
	$"0222 0227 022C 0231 0236 023B 0240 0245"            /* .".'.,.1.6.;.@.E */
	$"024A 024F 0254 0259 025D 0262 0267 026C"            /* .J.O.T.Y.].b.g.l */
	$"0271 0277 027C 0283 028A 0290 0293 0298"            /* .q.w.|.É.ä.ê.ì.ò */
	$"029D 02A3 02AA 02B1 02B6 02BB 02C0 02C5"            /* .ù.£.™.±.∂.ª.¿.≈ */
	$"02CA 02CF 02D4 02DB 02E0 02E5 02EA 02EF"            /* . .œ.‘.€.‡.Â.Í.Ô */
	$"02F4 02FB 0302 0307 0308 030D 0314 0319"            /* .Ù.˚............ */
	$"0320 0327 032D 0333 0338 0338 033D 0342"            /* . .'.-.3.8.8.=.B */
	$"0347 034D 0354 0357 035C 0361 0366 0368"            /* .G.M.T.W.\.a.f.h */
	$"036A 0370 0375 037A 037F 0384 038B 038E"            /* .j.p.u.z...Ñ.ã.é */
	$"0391 0396 039D 03A2 03A4 03A6 03AB 03B2"            /* .ë.ñ.ù.¢.§.¶.´.≤ */
	$"03B7 03BC 03C1 03C6 03CB 03CD 03D2 03D5"            /* .∑.º.¡.∆.À.Õ.“.’ */
	$"03D7 03DC 03E1 03E7 03EC 03F1 03F6 03FB"            /* .◊.‹.·.Á.Ï.Ò.ˆ.˚ */
	$"03FC 0401 0406 040B 0410 0412 0416 0418"            /* .¸.............. */
	$"041C 041E 0423 0428 0000 FFFF FFFF FFFF"            /* .....#.(..ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF 0007 FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇ..ˇˇˇˇ */
	$"FFFF 0007 FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇ..ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF 0707 0307 0207 0007"            /* ˇˇˇˇˇˇˇˇ........ */
	$"0107 0007 0007 0307 0207 0207 0107 0107"            /* ................ */
	$"0207 0107 0207 0007 0107 0207 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0207 0207"            /* ................ */
	$"0107 0107 0207 0107 0007 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0307 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0007 0107 0107 0107 0107 0207"            /* ................ */
	$"0107 0207 0107 0007 0207 0007 0007 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0007 0007"            /* ................ */
	$"0307 0307 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0307 0007 0007 0007 0307 0107 0107 0007"            /* ................ */
	$"0107 0107 0307 0000 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0307 0207"            /* ................ */
	$"0207 0207 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0007 0007 0007 0307"            /* ................ */
	$"0107 0107 0007 0007 0007 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0007 0107 0107 0107"            /* ................ */
	$"0107 0107 0007 0007 0107 0307 0107 0007"            /* ................ */
	$"0107 0007 0007 0107 0007 0107 0707 0107"            /* ................ */
	$"0107 0107 0007 0007 0207 0107 0107 0107"            /* ................ */
	$"0307 0307 0107 0107 0107 0107 0107 0007"            /* ................ */
	$"0207 0207 0107 0007 0107 0207 0207 0107"            /* ................ */
	$"0007 0107 0107 0107 0107 0107 0207 0107"            /* ................ */
	$"0207 0207 0107 0107 0007 0107 0107 0107"            /* ................ */
	$"0107 0307 0107 0107 0107 0107 0207 0107"            /* ................ */
	$"0207 0207 0207 0107 0107 FFFF"                      /* ..........ˇˇ */
};

data 'NFNT' (9609, "%NCSA Special Bold") {
	$"9000 0000 00D9 0006 0000 FFF7 0006 000B"            /* ê....Ÿ....ˇ˜.... */
	$"0328 0009 0002 0000 0035 0000 0000 0000"            /* .(.∆.....5...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0A20"            /* ..............¬  */
	$"04D5 2800 0000 0000 0000 0000 0000 0000"            /* .’(............. */
	$"0000 001C 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"3000 0000 0003 0000 0021 AD00 0000 0000"            /* 0........!≠..... */
	$"0000 0000 0008 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 03F0 0050 0960 0044 201A"            /* ........P∆`.D . */
	$"6009 0403 20D1 2080 6890 4006 0322 0798"            /* `∆.. — Ähê@..".ò */
	$"0000 0000 0000 0000 0000 4000 0000 0004"            /* ..........@..... */
	$"0000 0012 D600 0000 0000 0000 0000 FA9C"            /* ....÷.........˙ú */
	$"F667 8400 0037 3738 7F77 DCE0 0001 CE77"            /* ˆgÑ..778.w‹‡..Œw */
	$"9DEF FDDB C79C 462E F3BC EFDE F71D FFE7"            /* ùÔ˝€«úF.ÛºÔﬁ˜.ˇÁ */
	$"4040 C006 0306 378C 0000 0000 1800 0000"            /* @@¿...7å........ */
	$"06D9 AE73 BF17 4482 52AC 9010 8A54 DB62"            /* .ŸÆsø.DÇR¨ê.äT€b */
	$"114A B108 A529 0492 0AA7 9CF5 A07B 8003"            /* .J±.•).í¬ßúı†{Ä. */
	$"2204 FFC0 4767 0004 8004 0000 0039 CE78"            /* ".ˇ¿Gg..Ä....9Œx */
	$"000A AE00 2807 B000 FFFB 5D6C DF20 003F"            /* .¬Æ.(.∞.ˇ˚]lﬂ .? */
	$"FFFC FFFF FFF0 1FF3 FFFF FFFF FFFB C7BC"            /* ˇ¸ˇˇˇ.Ûˇˇˇˇˇ˚«º */
	$"6F3F DFFF 9FDE F7BD FFE7 E060 C006 0706"            /* o?ﬂˇüﬁ˜ΩˇÁ‡`¿... */
	$"019C 0000 0000 1800 0000 06DB F18C 6198"            /* .ú.........€Òåaò */
	$"C400 0000 6000 0000 0000 0000 0000 0079"            /* ƒ...`..........y */
	$"2428 EAC4 425E 02A4 4044 9402 4A80 4998"            /* $(ÍƒB^.§@Dî.JÄIò */
	$"8000 0064 0900 0046 31A0 0014 B242 03F4"            /* Ä..d∆..F1†..≤B.Ù */
	$"9000 C2B9 6E18 6E20 0069 B9CD FEC0 F7BD"            /* ê.¬πn.n .iπÕ˛¿˜Ω */
	$"B7DA 7FDE 73BC 631F C7EC 7FBB FEF7 86DE"            /* ∑⁄.ﬁsºc.«Ï.ª˛˜Üﬁ */
	$"F6ED 8F33 A02F F39E E67F B7BD EB3B CFB3"            /* ˆÌè3†/ÛûÊ.∑ΩÎ;œ≥ */
	$"FF7B AC77 F6DA D18C 2158 C5EF 7BDE F739"            /* ˇ{¨wˆ⁄—å!X≈Ô{ﬁ˜9 */
	$"CE74 A567 39CE 7463 18A6 7725 FACB 2D5C"            /* Œt•g9Œtc.¶w%˙À-\ */
	$"1FA4 F648 7F23 22BE 4998 B9C4 804E 7100"            /* .§ˆH.#"æIòπƒÄNq. */
	$"0046 31A2 8015 5C05 4404 9000 C7DC A418"            /* .F1¢Ä.\.D.ê.«‹§. */
	$"7FF8 F86A B31B 7FF1 9DF0 600C DFFF B1BF"            /* .¯¯j≥..Òù`.ﬂˇ±ø */
	$"7B7F C7CC 7EFB F6FC E6DE F647 1B33 001F"            /* {.«Ã~˚ˆ¸ÊﬁˆG.3.. */
	$"FFFF BFFF F7ED FFFF FFFE 1B7B AEF7 ECDC"            /* ˇˇøˇ˜Ìˇˇˇ˛.{Æ˜Ï‹ */
	$"1FFC 3D38 C631 8C63 18C6 318C A598 C631"            /* .¸=8∆1åc.∆1å•ò∆1 */
	$"8C63 18A0 AC13 F7CA B100 04F5 6BF4 8925"            /* åc.†¨.˜ ±..ıkÙâ% */
	$"1294 4768 D668 FC84 8A93 207F F1B5 7FE0"            /* .îGh÷h¸Ñäì .Òµ.‡ */
	$"01F8 C7F4 9000 C28E DE98 6420 F8CC B64F"            /* .¯«Ùê.¬éﬁòd ¯Ã∂O */
	$"E3DB 3EF0 37D8 DEDF F3BC 7B7B F7EC 6E7B"            /* „€>7ÿﬁﬂÛº{{˜Ïn{ */
	$"C6F6 36DE FEE2 331B 0019 DE37 F6DE F7ED"            /* ∆ˆ6ﬁ˛‚3...ﬁ7ˆﬁ˜Ì */
	$"5DEF 7BCB 9B7B FBB6 46D8 118C 2118 C631"            /* ]Ô{Àõ{˚∂Fÿ.å!.∆1 */
	$"8C63 187F FFFC A518 C631 8C63 18A0 A40B"            /* åc..ˇ¸•.∆1åc.†§. */
	$"F2C7 2D00 1FA6 7643 3F29 2294 4008 DEB0"            /* Ú«-..¶vC?)"î@.ﬁ∞ */
	$"8684 72A4 9046 31A5 C000 0005 4404 9000"            /* ÜÑr§êF1•¿...D.ê. */
	$"002F 5D0C C020 00CF BFFF FFFB 3630 1FF0"            /* ./].¿ .œøˇˇ˚60. */
	$"18DE 7FFF E3FB FFBF EE3F C7F7 36FB B7B2"            /* .ﬁ.ˇ„˚ˇøÓ?«˜6˚∑≤ */
	$"7F9F 1F9F FFFF 86FE F7BD 5DFF FFC0 DFEE"            /* .ü.üˇˇÜ˛˜Ω]ˇˇ¿ﬂÓ */
	$"FEFE F6D8 118C 6118 C673 9CE7 3842 1084"            /* ˛˛ˆÿ.åa.∆súÁ8B.Ñ */
	$"A518 C631 8CE7 3980 AC64 E2C6 C200 08A4"            /* •.∆1åÁ9Ä¨d‚∆¬..§ */
	$"4040 0929 4A94 4FF5 5331 8504 8464 9046"            /* @@∆)JîOıS1Ö.ÑdêF */
	$"31A5 0000 0042 47F4 9000 C01D 2687 8003"            /* 1•...BGÙê.¿.&áÄ. */
	$"0787 3FB8 6E73 1CED 8000 CFDF 9DEF E1DB"            /* .á?∏ns.ÌÄ.œﬂùÔ·€ */
	$"DD9F EE2E C3B6 E671 2312 7F8F 1F8D F3DE"            /* ›üÓ.√∂Êq#..è.çÛﬁ */
	$"F67E F797 5DBB CFC7 8DA4 545F F6D8 118B"            /* ˆ~˜ó]ªœ«ç§T_ˆÿ.ã */
	$"BF17 39AD 6B5A D7BD EF7C A517 39CE 735A"            /* ø.9≠kZ◊ΩÔ|•.9ŒsZ */
	$"D680 77A2 02D8 1C00 00BB 81FF C9D6 FA94"            /* ÷Äw¢.ÿ...ªÅˇ…÷˙î */
	$"400D BDCE 8104 07D3 2AC6 2E7A C000 0000"            /* @.ΩŒÅ..”*∆.z¿... */
	$"3C07 B000 0008 0000 0003 0180 0000 0000"            /* <.∞........Ä.... */
	$"0001 8000 0000 0000 0000 0000 0000 00C0"            /* ..Ä............¿ */
	$"0000 0000 0008 0000 0000 0018 0E00 0003"            /* ................ */
	$"0300 0000 0006 03F0 0000 0000 0000 0000"            /* ............... */
	$"0200 0000 0000 0000 0000 0000 201C 0040"            /* ............ ..@ */
	$"0000 0000 0000 0100 0000 4000 0000 0004"            /* ..........@..... */
	$"0000 0000 0000 0000 0000 0400 0000 0000"            /* ................ */
	$"0000 0006 0000 0000 0000 0003 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0070 0C00 0003 0300 0000 003C"            /* .....p.........< */
	$"0000 0000 0000 0000 0000 0400 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0100 0001 8000 0000 0018 0000 0000 0000"            /* ....Ä........... */
	$"0000 0000 3800 0000 0000 0000 0000 0000"            /* ....8........... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0002 0005"            /* ................ */
	$"000A 000F 0014 0019 001B 001F 0023 0028"            /* .¬...........#.( */
	$"002D 0030 0035 0037 003C 0041 0044 0049"            /* .-.0.5.7.<.A.D.I */
	$"004E 0053 0058 005D 0062 0067 006C 006E"            /* .N.S.X.].b.g.l.n */
	$"0071 0075 007A 007E 0083 0088 008D 0092"            /* .q.u.z.~.É.à.ç.í */
	$"0097 009C 00A1 00A6 00AB 00B0 00B2 00B7"            /* .ó.ú.°.¶.´.∞.≤.∑ */
	$"00BC 00C1 00C6 00CB 00D0 00D5 00DA 00DF"            /* .º.¡.∆.À.–.’.⁄.ﬂ */
	$"00E4 00E8 00ED 00F2 00F7 00FC 0101 0106"            /* .‰.Ë.Ì.Ú.˜.¸.... */
	$"0109 010D 0110 0113 0119 011B 0120 0125"            /* .∆........... .% */
	$"012A 012F 0134 0138 013D 0142 0144 0147"            /* .*...4.8.=.B.D.G */
	$"014C 014F 0154 0159 015E 0163 0168 016D"            /* .L.O.T.Y.^.c.h.m */
	$"0172 0176 017B 0180 0185 018A 018F 0194"            /* .r.v.{.Ä.Ö.ä.è.î */
	$"0198 019A 019E 01A3 01A3 01A8 01AD 01B2"            /* .ò.ö.û.£.£.®.≠.≤ */
	$"01B7 01BC 01C1 01C6 01CB 01D0 01D5 01DA"            /* .∑.º.¡.∆.À.–.’.⁄ */
	$"01DF 01E4 01E9 01EE 01F3 01F8 01FD 01FF"            /* .ﬂ.‰.È.Ó.Û.¯.˝.ˇ */
	$"0201 0204 0207 020C 0211 0216 021B 0220"            /* ...............  */
	$"0225 022A 022F 0234 0239 023C 0240 0245"            /* .%.*...4.9.<.@.E */
	$"024A 024F 0254 0259 025E 0263 0268 026E"            /* .J.O.T.Y.^.c.h.n */
	$"0270 0273 0278 027D 0282 0287 028C 028F"            /* .p.s.x.}.Ç.á.å.è */
	$"0292 0297 029C 02A0 02A5 02AA 02AF 02B4"            /* .í.ó.ú.†.•.™.Ø.¥ */
	$"02B8 02BC 02C1 02C6 02CB 02D0 02D1 02D6"            /* .∏.º.¡.∆.À.–.—.÷ */
	$"02DB 02E0 02E5 02EA 02EF 02F4 02F9 02F9"            /* .€.‡.Â.Í.Ô.Ù.˘.˘ */
	$"02FE 0303 0308 030D 0312 0315 031B 031F"            /* .˛.............. */
	$"0323 0325 0327 032C 0331 0336 033C 0342"            /* .#.%.'.,.1.6.<.B */
	$"0000 0100 0100 0100 0100 0100 FFFF FFFF"            /* ............ˇˇˇˇ */
	$"FFFF 0706 FFFF FFFF FFFF 0100 FFFF FFFF"            /* ˇˇ..ˇˇˇˇˇˇ..ˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"0706 0306 0206 0106 0106 0106 0106 0306"            /* ................ */
	$"0206 0306 0106 0106 0106 0106 0206 0206"            /* ................ */
	$"0106 0306 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0206 0106 0206 0106 0206 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0306 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0206 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0306 0206 0306 0206 0106"            /* ................ */
	$"0306 0106 0106 0106 0106 0106 0206 0106"            /* ................ */
	$"0106 0306 0106 0106 0306 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0206 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0306 0306 0106 0106 0100"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0306 0206 0206 0206 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0206 0206 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0306 0206 0106 0106 0106"            /* ................ */
	$"0106 0106 0206 0306 0106 0106 0206 0106"            /* ................ */
	$"0106 0106 0106 0206 0206 0106 0106 0106"            /* ................ */
	$"0106 0406 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0706 0106 0106 0106 0106 0106"            /* ................ */
	$"0206 0106 0206 0206 0306 0306 0106 0106"            /* ................ */
	$"0106 0106 0106 0000"                                /* ........ */
};

data 'NFNT' (18441, "ASLFont+e 9") {
	$"F800 0000 00FF 000C 0000 FFFE 000C 000C"            /* ¯....ˇ....ˇ˛.... */
	$"04A3 000A 0002 0001 004D 0000 0000 0000"            /* .£.¬.....M...... */
	$"0080 0000 0020 9004 0000 0000 0000 0000"            /* .Ä... ê......... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 003F"            /* ...............? */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FC00 0000 0000"            /* ˇˇˇˇˇˇˇˇˇˇ¸..... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0003 0000 0000 0000 0000 008F C000 0020"            /* ...........è¿..  */
	$"9004 0000 0000 0000 0000 0000 0000 0000"            /* ê............... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 003F FFFF FFFF FFFF"            /* .........?ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FC00 0000 0000 0000 0000 0000"            /* ˇˇˇˇ¸........... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0003 0000 0029"            /* ...............) */
	$"C640 0280 0080 0000 0020 9004 0000 0000"            /* ∆@.Ä.Ä... ê..... */
	$"0000 0000 0004 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0100"            /* ................ */
	$"0000 0000 0000 0080 0000 0000 0000 0001"            /* .......Ä........ */
	$"C044 003F FFFF FFFF FFFF FFFF FFFF FFFF"            /* ¿D.?ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FC00"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ¸. */
	$"F000 0000 0000 0000 0004 0090 4020 0A00"            /* ..........ê@ ¬. */
	$"1000 0400 8400 0802 8000 2000 0002 0000"            /* ....Ñ...Ä. ..... */
	$"0000 0000 4D43 1E00 0569 0841 139E 0080"            /* ....MC...i.A.û.Ä */
	$"3F00 0020 9004 C001 8768 0896 4C00 0005"            /* ?.. ê.¿.áh.ñL... */
	$"CB9C 2FBB EE70 000E 73BC FF7F EE8F 0C61"            /* Àú/ªÓp..sºˇ.Óè.a */
	$"18BB CEF3 FF18 C631 FF1A 0204 0010 1821"            /* .ªŒÛˇ.∆1ˇ......! */
	$"3080 0000 0008 0000 0002 A6C4 003F FFFF"            /* 0Ä........¶ƒ.?ˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FC20 800F 8000 0000"            /* ˇˇˇˇˇˇˇˇ¸ Ä.Ä... */
	$"0000 7C24 7090 47A0 0AC7 D117 8AAE 8477"            /* ..|$pêG†¬«—.äÆÑw */
	$"FE21 87C0 2030 41FA 1DFF CA90 07C1 3343"            /* ˛!á¿ 0A˙.ˇ ê.¡3C */
	$"1200 22B9 8842 9392 0080 0000 0020 9005"            /* .."πàBìí.Ä... ê. */
	$"208A 58EA 9F59 5221 000A 3C62 6840 3188"            /*  äXÍüYR!.¬<bh@1à */
	$"2091 8C63 08C2 118A 0CA1 BCC6 318C 0918"            /*  ëåc.¬.ä.°º∆1å∆. */
	$"C631 0C8D 0104 0010 2020 1080 0000 0008"            /* ∆1.ç....  .Ä.... */
	$"0000 0002 AB28 003F FFFF FFFF FFFF FFFF"            /* ....´(.?ˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FC40 8000 FC90 0240 0000 045F 27FF"            /* ˇˇ¸@Ä.¸ê.@..._'ˇ */
	$"F4BF FF00 7F14 B2A0 9F00 4229 8050 FFCC"            /* Ùøˇ...≤†ü.B)ÄPˇÃ */
	$"8147 C410 4A90 FC70 9343 1200 7569 775D"            /* ÅGƒ.Jê¸pìC..uiw] */
	$"7E93 FFF0 00FC 003F FFFE 1FFE 0847 E96A"            /* ~ìˇ.¸.?ˇ˛.˛.GÈj */
	$"21A9 000A 6842 AF78 318D 5F41 EC63 08C2"            /* !©.¬hBØx1ç_AÏc.¬ */
	$"108A 0D21 5AC6 318C 0918 C551 1488 007F"            /* .ä.!Z∆1å∆.≈Q.à.. */
	$"9EF7 73FD 32FA CEF3 EDFD 18D6 31FA A07C"            /* û˜s˝2˙ŒÛÌ˝.÷1˙†| */
	$"003F FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* .?ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FDB0 8100"            /* ˇˇˇˇˇˇˇˇˇˇˇˇ˝∞Å. */
	$"857D DFFB BF50 1491 2092 48A8 2A00 929A"            /* Ö}ﬂ˚øP.ë íH®*.íö */
	$"92BF C400 4429 F868 2061 0A42 441F CA90"            /* íøƒ.D)¯h a¬BD. ê */
	$"8C42 0C8F F200 FAAF 42F0 1092 6080 0000"            /* åB.èÚ.˙ØB.í`Ä.. */
	$"0020 8045 2513 8442 9CA4 2177 CF92 A88D"            /* . ÄE%.ÑBú§!wœí®ç */
	$"20C4 4E88 8022 AFFD 08FB D3FA 0E21 59C7"            /*  ƒNàÄ"Ø˝.˚”˙.!Y« */
	$"D1F3 8918 D48A 2448 008C 6118 A463 34D7"            /* —Ûâ.‘ä$H.åa.§c4◊ */
	$"318C 7209 18D5 5114 1010 003F FFFF FFFF"            /* 1år∆.’Q....?ˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFE6 813F 9B44 8248 8350"            /* ˇˇˇˇˇˇˇÊÅ?õDÇHÉP */
	$"1991 2193 F0C8 2AC9 1412 FC24 A402 DC29"            /* .ë!ì»*…..¸$§.‹) */
	$"8064 A85A 46FA 85F0 CA91 8842 00F0 1200"            /* Äd®ZF˙Ö ëàB... */
	$"7544 6298 1092 6080 0003 F020 8044 C57E"            /* uDbò.í`Ä.. ÄD≈~ */
	$"0247 CADA A1A9 0013 2903 F0C4 9178 5F44"            /* .G ⁄°©..).ƒëx_D */
	$"F463 08C2 118A 8D21 18C6 1188 4918 D544"            /* Ùc.¬.äç!.∆.àI.’D */
	$"4448 008C 611F A463 3CD6 318C 61C9 18D4"            /* DH.åa.§c<÷1åa….‘ */
	$"9122 A07C 003F FFFF FFFF FFFF FFFF FFFF"            /* ë"†|.?ˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFF9 0100 9104 8650 9E1F 9281 2292 4108"            /* ˇ˘..ë.ÜPû.íÅ"íA. */
	$"220A 9021 1044 8401 2A49 8082 AA82 2542"            /* "¬ê!.DÑ.*IÄÇ™Ç%B */
	$"0410 8AB1 8842 0020 1200 2284 4310 0092"            /* ..ä±àB. .."ÑC..í */
	$"6080 0000 0020 8044 0522 2002 BD59 1221"            /* `Ä... ÄD." .ΩY.! */
	$"0022 2A22 28C4 9108 2080 8463 08C2 118A"            /* ."*"(ƒë. ÄÑc.¬.ä */
	$"8CA1 18C6 1188 4915 6E24 8428 009C 6118"            /* å°.∆.àI.n$Ñ(.úa. */
	$"2463 32D6 318C 6029 3555 5142 A010 003F"            /* $c2÷1å`)5UQB†..? */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFF9 0181 1108"            /* ˇˇˇˇˇˇˇˇˇˇˇ˘.Å.. */
	$"8A40 8210 1082 2492 4208 2414 5042 1084"            /* ä@Ç..Ç$íB.$.PB.Ñ */
	$"8802 8889 8101 A933 E842 0411 12B2 8884"            /* à.àâÅ.©3ËB...≤àÑ */
	$"0020 1200 0544 4290 7CFE 6080 0000 0FE0"            /* . ...DBê|˛`Ä...‡ */
	$"8047 F503 C240 0926 8C00 1061 CFDC 2738"            /* ÄGı.¬@∆&å..aœ‹'8 */
	$"8E75 0004 747C FF7E 0E8F 747F 18BA 0E8F"            /* éu..t|ˇ~.èt..∫.è */
	$"88E2 4624 FE38 FC6F 9EF7 A3E3 31D6 2EF3"            /* à‚F$˛8¸oû˜£„1÷.Û */
	$"E3C6 D22A 2FFA A013 FFFF FFFF FFFF FFFF"            /* „∆“*.˙†.ˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FDA6 0F46 2113 F247 FEE0 208C"            /* ˇˇˇˇ˝¶.F!.ÚG˛‡ å */
	$"F8A4 4413 E8E4 4E84 2108 90FC 0911 7A01"            /* ¯§D.Ë‰NÑ!.ê¸∆.z. */
	$"208C 303A 3FF6 24DC FB38 0020 1E00 0000"            /*  å0:?ˆ$‹˚8. .... */
	$"0000 0000 6080 0000 0020 8044 0000 0000"            /* ....`Ä... ÄD.... */
	$"0000 0000 1000 0000 0000 0002 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0100 0000 0000 0000"            /* ................ */
	$"0000 0000 0040 2000 0080 4000 0000 0101"            /* .....@ ..Ä@..... */
	$"C000 003F FFFF FFFF FFFF FFFF FFFF FFFF"            /* ¿..?ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FC00"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ¸. */
	$"0040 0000 0000 0000 0000 0000 0000 0000"            /* .@.............. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 6080"            /* ..............`Ä */
	$"0000 0020 8044 0000 0000 0000 0000 2000"            /* ... ÄD........ . */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0380"            /* ...............Ä */
	$"C000 0080 4000 0000 0E00 0000 003F FFFF"            /* ¿..Ä@........?ˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FC00 0000 0000 0000"            /* ˇˇˇˇˇˇˇˇ¸....... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0005 000A 000F 0014 0019 001E"            /* .......¬........ */
	$"0021 0026 002B 002B 002F 0032 0036 0036"            /* .!.&.+.+./.2.6.6 */
	$"003C 0042 0048 004E 0054 005A 005E 0061"            /* .<.B.H.N.T.Z.^.a */
	$"0067 006D 006E 0071 0074 0079 007E 0083"            /* .g.m.n.q.t.y.~.É */
	$"0084 0089 0089 008A 008D 0092 0097 009C"            /* .Ñ.â.â.ä.ç.í.ó.ú */
	$"00A1 00A2 00A5 00A8 00AD 00B2 00B4 00B9"            /* .°.¢.•.®.≠.≤.¥.π */
	$"00BA 00BE 00C3 00C5 00CA 00CF 00D4 00D9"            /* .∫.æ.√.≈. .œ.‘.Ÿ */
	$"00DE 00E3 00E8 00ED 00EE 00F0 00F3 00F8"            /* .ﬁ.„.Ë.Ì.Ó..Û.¯ */
	$"00FB 0100 0105 010A 010F 0114 0119 011E"            /* .˚.....¬........ */
	$"0123 0128 012D 0130 0135 013A 013F 0144"            /* .#.(.-.0.5.:.?.D */
	$"0149 014E 0153 0158 015D 0162 0167 016C"            /* .I.N.S.X.].b.g.l */
	$"0171 0176 017B 0180 0185 0187 018B 018D"            /* .q.v.{.Ä.Ö.á.ã.ç */
	$"0190 0196 0198 019D 01A2 01A7 01AC 01B1"            /* .ê.ñ.ò.ù.¢.ß.¨.± */
	$"01B5 01BA 01BF 01C0 01C3 01C8 01C9 01CE"            /* .µ.∫.ø.¿.√.».….Œ */
	$"01D3 01D8 01DD 01E2 01E6 01EB 01EF 01F4"            /* .”.ÿ.›.‚.Ê.Î.Ô.Ù */
	$"01F9 01FE 0203 0208 020D 0210 0211 0214"            /* .˘.˛............ */
	$"0219 0219 021E 021E 022A 0236 0242 024E"            /* .........*.6.B.N */
	$"025A 0266 0272 027E 028A 0296 02A2 02AE"            /* .Z.f.r.~.ä.ñ.¢.Æ */
	$"02BA 02C6 02D2 02DE 02EA 02F6 0302 030E"            /* .∫.∆.“.ﬁ.Í.ˆ.... */
	$"031A 0326 0332 033E 034A 0356 0362 036E"            /* ...&.2.>.J.V.b.n */
	$"037A 0386 038C 0390 0394 0398 039A 039C"            /* .z.Ü.å.ê.î.ò.ö.ú */
	$"03A1 03A6 03A9 03AE 03B3 03B8 03BD 03C2"            /* .°.¶.©.Æ.≥.∏.Ω.¬ */
	$"03C7 03CC 03D1 03D6 03DB 03E0 03E5 03EA"            /* .«.Ã.—.÷.€.‡.Â.Í */
	$"03EF 03F4 03F9 03FE 0403 0408 040D 0412"            /* .Ô.Ù.˘.˛........ */
	$"0417 041C 0421 0426 042B 0430 0433 0438"            /* .....!.&.+.0.3.8 */
	$"043D 0442 0447 044B 0450 0455 045A 0460"            /* .=.B.G.K.P.U.Z.` */
	$"0465 046A 046E 0473 0478 047D 0482 0487"            /* .e.j.n.s.x.}.Ç.á */
	$"048C 0491 0495 049B 04A0 04A5 04AA 04AF"            /* .å.ë.ï.õ.†.•.™.Ø */
	$"04B3 04B7 04B7 04B7 04B7 04B7 04B7 04B7"            /* .≥.∑.∑.∑.∑.∑.∑.∑ */
	$"04B7 04B7 04B7 04B7 04B7 04B7 04B7 04B7"            /* .∑.∑.∑.∑.∑.∑.∑.∑ */
	$"04B7 04B7 04B7 04B7 04B7 04B7 04B7 04B7"            /* .∑.∑.∑.∑.∑.∑.∑.∑ */
	$"04B7 04B7 04B7 04B7 04B7 04B7 04B7 04BC"            /* .∑.∑.∑.∑.∑.∑.∑.º */
	$"04BF 04C3 04C7 0006 0006 0006 0006 0006"            /* .ø.√.«.......... */
	$"0006 0106 0006 0006 0006 0106 0006 0206"            /* ................ */
	$"0000 0006 0006 0006 0006 0006 0006 0206"            /* ................ */
	$"0006 0006 0006 0206 0106 0106 0006 0006"            /* ................ */
	$"0006 0206 0006 0006 0206 0106 0006 0006"            /* ................ */
	$"0006 0006 0206 0106 0206 0006 0006 0106"            /* ................ */
	$"0006 0206 0106 0006 0106 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0206 0106 0106"            /* ................ */
	$"0006 0106 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0106 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0206 0106"            /* ................ */
	$"0106 0106 0006 0206 0006 0006 0006 0006"            /* ................ */
	$"0006 0106 0006 0006 0206 0106 0006 0206"            /* ................ */
	$"0006 0006 0006 0006 0006 0106 0006 0106"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0106 0206"            /* ................ */
	$"0106 0006 0001 0006 0001 000C 000C 000C"            /* ................ */
	$"000C 000C 000C 000C 000C 000C 000C 000C"            /* ................ */
	$"000C 000C 000C 000C 000C 000C 000C 000C"            /* ................ */
	$"000C 000C 000C 000C 000C 000C 000C 000C"            /* ................ */
	$"000C 000C 000C 0006 0006 0106 0106 0106"            /* ................ */
	$"0006 0006 0006 0106 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0106"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0106 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0206 0106 FFFF"                           /* ........ˇˇ */
};

data 'NFNT' (18442, "ASLFont+e 10") {
	$"F800 0000 00FF 000E 0000 FFFD 000B 000F"            /* ¯....ˇ....ˇ˝.... */
	$"05A8 000C 0003 0000 004F 0000 0000 0000"            /* .®.......O...... */
	$"0010 0000 0000 2044 0080 0000 0000 0000"            /* ...... D.Ä...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0001 8000 0000 0000 0000 0011"            /* ......Ä......... */
	$"FC00 0000 2044 0080 0000 0000 0000 0000"            /* ¸... D.Ä........ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0001 8000 0000 0000 0000 0010 0000"            /* ....Ä........... */
	$"0000 2044 0080 0000 0000 0000 C000 0000"            /* .. D.Ä......¿... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0383 8000 0000 0000"            /* .........ÉÄ..... */
	$"0000 0000 0000 0000 0000 0380 887F FFFF"            /* ...........Äà.ˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FF00 1E00 0000 0000 0000 0000"            /* ˇˇˇˇˇ........... */
	$"0900 0841 0040 0480 0100 0008 0020 4000"            /* ∆..A.@.Ä..... @. */
	$"2000 4000 0401 8000 0200 0000 0000 0001"            /*  .@...Ä......... */
	$"2C01 8000 0569 C641 029F 0010 0000 0000"            /* ,.Ä..i∆A.ü...... */
	$"2044 0080 0030 ED51 12C5 2000 002E 4E70"            /*  D.Ä.0ÌQ.≈ ...Np */
	$"BE6F B9C0 000E 01DE 7FBF F747 8630 8C5D"            /* æoπ¿...ﬁ.ø˜GÜ0å] */
	$"E779 FF8C 6318 FE40 9004 0800 2030 4261"            /* Áyˇåc.˛@ê... 0Ba */
	$"0000 0000 0000 0000 054D 887F FFFF FFFF"            /* .........Mà.ˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FF08 1000 FC00 0000 0000 003F 091F"            /* ˇˇˇ...¸......?∆. */
	$"0841 1F41 F4B8 F908 5F72 97A0 47BE F8C2"            /* .A.AÙ∏˘._ró†Gæ¯¬ */
	$"43F2 047E 640D FA0E 7DE8 A907 FFF1 3341"            /* CÚ.~d.˙.}Ë©.ˇÒ3A */
	$"87C0 02A9 0842 9391 0010 03F8 0000 2044"            /* á¿.©.Bìë...¯.. D */
	$"0080 0049 1D53 EB25 2200 0031 D189 A080"            /* .Ä.I.SÎ%"..1—â†Ä */
	$"C620 4111 7A31 8461 08C5 0650 DC63 18C6"            /* ∆ A.z1Ña.≈.P‹c.∆ */
	$"048C 6318 8640 A802 0800 2040 4261 0000"            /* .åc.Ü@®... @Ba.. */
	$"0000 1000 0000 0556 507F FFFF FFFF FFFF"            /* .......VP.ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FF10 1000 07E2 4008 8001 5001 17E4 FFFF"            /* ˇ....‚@.Ä.P..‰ˇˇ */
	$"D17E 1FC0 0BF8 5112 9023 F002 0841 7C15"            /* —~.¿.¯Q.ê#..A|. */
	$"3F82 0404 47E2 0408 A904 3081 3341 8440"            /* ?Ç..G‚..©.0Å3AÑ@ */
	$"2579 8841 1391 0010 0000 0000 2044 0098"            /* %yàA.ë...... D.ò */
	$"0041 0DFD 2D26 1A90 0053 410A A100 C634"            /* .A.˝-&.ê.SA¬°.∆4 */
	$"8081 8631 8461 0845 0690 AE63 18C6 048C"            /* ÄÅÜ1Ña.E.êÆc.∆.å */
	$"6318 8620 C400 0800 2040 4021 0000 0000"            /* c.Ü ƒ... @@!.... */
	$"1000 0000 0540 207F FFFF FFFF FFFF FFFF"            /* .....@ .ˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FF6C"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇl */
	$"1020 0423 FFFF F73F 5001 3424 0849 1148"            /* . .#ˇˇ˜?P.4$.I.H */
	$"1481 0914 5112 9030 4002 1051 4015 0402"            /* .Å∆.Q.ê0@..Q@... */
	$"0404 4122 0408 A904 3083 2D41 8440 72A9"            /* ..A"..©.0É-AÑ@r© */
	$"0640 7E91 0010 0000 0000 2044 00A4 1140"            /* .@~ë...... D.§.@ */
	$"8855 1D42 1710 0053 410C BD01 4635 3E42"            /* àU.B...SA.Ω.F5>B */
	$"B631 8461 0845 0710 AD63 18C6 048C 62A8"            /* ∂1Ña.E..≠c.∆.åb® */
	$"8A20 8000 FF3D EEE7 FA65 F79D E7DB FA31"            /* ä Ä.ˇ=ÓÁ˙e˜ùÁ€˙1 */
	$"AC63 F540 F87F FFFF FFFF FFFF FFFF FFFF"            /* ¨cı@¯.ˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFE 1023"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇ˛.# */
	$"FC25 1208 9103 5FCA 5424 1849 2148 14B9"            /* ¸%..ë._ T$.I!H.π */
	$"1110 6DFE 9FE8 4012 2050 C018 8425 8825"            /* ..m˛üË@. P¿.Ñ%à% */
	$"F922 7FF8 A904 3082 0141 8440 FD69 735C"            /* ˘".¯©.0Ç.AÑ@˝is\ */
	$"1291 FFFE 0007 F000 3FFF FFC3 FFF2 4853"            /* .ëˇ˛...?ˇˇ√ˇÚHS */
	$"8882 177C 0095 4234 83E2 39E2 0024 B7FE"            /* àÇ.|.ïB4É‚9‚.$∑˛ */
	$"847D E9FD 0610 ACE3 E8F9 C48C 6A45 1210"            /* Ñ}È˝..¨„Ë˘ƒåjE.. */
	$"8001 18C2 3148 C669 AC63 18E4 1231 AAA2"            /* Ä..¬1H∆i¨c.‰.1™¢ */
	$"2820 207F FFFF FFFF FFFF FFFF FFFF FFFF"            /* (  .ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFC 1023 04CD"            /* ˇˇˇˇˇˇˇˇˇˇˇ¸.#.Õ */
	$"1218 9103 5008 9024 284F C188 1481 1120"            /* ..ë.P.ê$(O¡à.Å.  */
	$"4210 1124 400A 7090 C018 9514 6818 4142"            /* B..$@¬pê¿.ï.h.AB */
	$"0418 AB0C 2082 008F F440 728E 42F0 1091"            /* ..´. Ç.èÙ@réB.ë */
	$"1810 0000 0000 2040 08A4 A240 48F9 5542"            /* ...... @.§¢@H˘UB */
	$"1A90 F899 440F C312 4421 3E44 BE31 8461"            /* .ê¯ôD.√.D!>Dæ1Ña */
	$"08C5 0710 8C63 08C4 248C 6AA2 2210 8001"            /* .≈..åc.ƒ$åj¢".Ä. */
	$"18C2 3F48 C671 AC63 18C3 9231 A922 4540"            /* .¬?H∆q¨c.√í1©"E@ */
	$"F87F FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ¯.ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFC 1020 0494 1228"            /* ˇˇˇˇˇˇˇˇˇ¸. .î.( */
	$"A13E 1008 1024 4849 0108 1081 2900 4210"            /* °>...$HI...Å).B. */
	$"1120 4004 A890 C010 5488 090C 4102 0410"            /* . @.®ê¿.Tà∆.A... */
	$"AB0C 2082 0070 0440 2544 6298 7C91 1810"            /* ´. Ç.p.@%Dbò|ë.. */
	$"0000 0000 2040 0898 AFC0 0051 5B2A 1210"            /* .... @.òØ¿.Q[*.. */
	$"0119 4808 8312 4434 8080 8231 8461 08C5"            /* ..H.É.D4ÄÄÇ1Ña.≈ */
	$"4690 8C63 08C4 248C 6B12 4208 8001 18C2"            /* Fêåc.ƒ$åk.B.Ä..¬ */
	$"3048 C669 AC63 18C0 5231 A922 8540 207F"            /* 0H∆i¨c.¿R1©"Ö@ . */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFE C030 0884 1248 8102"            /* ˇˇˇˇˇˇˇ˛¿0.Ñ.HÅ. */
	$"1008 1044 8889 0208 1082 4900 8410 2120"            /* ...Dàâ...ÇI.Ñ.!  */
	$"400A 2110 C020 6488 1092 4102 0420 AB14"            /* @¬!.¿ dà.íA.. ´. */
	$"2104 0020 0440 0284 4310 0091 1810 0000"            /* !.. .@.ÑC..ë.... */
	$"0FE0 2040 0880 A444 4857 AB11 2001 0511"            /* .‡ @.Ä§DHW´. ... */
	$"5088 A312 4454 4104 7A31 8461 08C5 4650"            /* Pà£.DTA.z1Ña.≈FP */
	$"8C63 08C4 248A B712 4208 8001 38C2 3048"            /* åc.ƒ$ä∑.B.Ä.8¬0H */
	$"C665 AC63 18C0 526A AAA3 0540 207F FFFF"            /* ∆e¨c.¿Rj™£.@ .ˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FF7D 2028 1104 2208 8102 2010"            /* ˇˇˇˇˇ} (..".Å. . */
	$"1084 0889 0410 1124 8501 0820 4220 8011"            /* .Ñ.â...$Ö.. B Ä. */
	$"2210 C040 4405 90A0 4102 0441 2B24 2248"            /* ".¿@D.ê†A..A+$"H */
	$"0020 0440 0544 4290 00FF 1810 0000 0000"            /* . .@.DBê.ˇ...... */
	$"2040 08FE A078 4801 24E9 2001 060E FF70"            /*  @.˛†xH.$È ...ˇp */
	$"9CE2 3984 0004 023E 7FBF 0747 BA3F 8C5D"            /* ú‚9Ñ...>.ø.G∫?å] */
	$"0747 C471 2312 7E04 83F8 DF3D EF47 C663"            /* .Gƒq#.~.É¯ﬂ=ÔG∆c */
	$"AC5D E7C7 8DA4 545F F540 27FF FFFF FFFF"            /* ¨]Á«ç§T_ı@'ˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FF29 21E4 2204 4F88 8FFE 4020 111F"            /* ˇˇˇ)!‰".Oàè˛@ .. */
	$"0919 0821 F238 84F2 1040 8421 0FE0 2420"            /* ∆..!Ú8ÑÚ.@Ñ!.‡$  */
	$"BC80 4404 7FC0 391F FC82 4DC7 E470 0020"            /* ºÄD..¿9.¸ÇM«‰p.  */
	$"07C0 0000 0000 0000 1810 0000 0000 2040"            /* .¿............ @ */
	$"0880 0000 0000 0000 C001 0200 0000 0000"            /* .Ä......¿....... */
	$"0008 0000 0000 0000 0000 0000 0000 0200"            /* ................ */
	$"0000 0000 0387 8000 0000 0000 8040 0001"            /* .....áÄ.....Ä@.. */
	$"0080 0000 0002 0380 007F FFFF FFFF FFFF"            /* .Ä.....Ä..ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FF00 C004 0000 0000 0000 0000 0000 0000"            /* ˇ.¿............. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 1810 0000 001F E040 0880"            /* ............‡@.Ä */
	$"0000 0000 0000 0002 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0100 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 8040 0001 0080"            /* ..........Ä@...Ä */
	$"0000 0002 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 1810 0000 0000 2040 0880 0000"            /* .......... @.Ä.. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0007 0180 0001 0080 0000"            /* .........Ä...Ä.. */
	$"001C 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0005"            /* ................ */
	$"000A 000F 0014 0019 001E 0021 0026 002B"            /* .¬.........!.&.+ */
	$"002B 0030 0034 0038 0038 003F 0046 004D"            /* .+.0.4.8.8.?.F.M */
	$"0054 005B 0062 0066 006A 0071 0078 0079"            /* .T.[.b.f.j.q.x.y */
	$"007C 007F 0084 0089 008E 008F 0094 0094"            /* .|...Ñ.â.é.è.î.î */
	$"0095 0098 009D 00A2 00A7 00AD 00AE 00B1"            /* .ï.ò.ù.¢.ß.≠.Æ.± */
	$"00B4 00B9 00BE 00C0 00C5 00C6 00CB 00D0"            /* .¥.π.æ.¿.≈.∆.À.– */
	$"00D3 00D8 00DD 00E2 00E7 00EC 00F1 00F6"            /* .”.ÿ.›.‚.Á.Ï.Ò.ˆ */
	$"00FB 00FC 00FE 0102 0107 010B 0110 0116"            /* .˚.¸.˛.......... */
	$"011B 0120 0125 012A 012F 0134 0139 013E"            /* ... .%.*...4.9.> */
	$"0141 0146 014B 0150 0155 015A 015F 0164"            /* .A.F.K.P.U.Z._.d */
	$"0169 016E 0173 0178 017D 0182 0187 018C"            /* .i.n.s.x.}.Ç.á.å */
	$"0191 0196 0199 019E 01A1 01A6 01AD 01AF"            /* .ë.ñ.ô.û.°.¶.≠.Ø */
	$"01B4 01B9 01BE 01C3 01C8 01CC 01D1 01D6"            /* .¥.π.æ.√.».Ã.—.÷ */
	$"01D7 01DA 01DF 01E0 01E5 01EA 01EF 01F4"            /* .◊.⁄.ﬂ.‡.Â.Í.Ô.Ù */
	$"01F9 01FD 0202 0206 020B 0210 0215 021A"            /* .˘.˝............ */
	$"021F 0224 0227 0228 022B 0230 0230 0235"            /* ...$.'.(.+.0.0.5 */
	$"0235 0239 0244 024F 025A 0265 0270 027B"            /* .5.9.D.O.Z.e.p.{ */
	$"0286 0291 029C 02A7 02B2 02BD 02C8 02D3"            /* .Ü.ë.ú.ß.≤.Ω.».” */
	$"02DE 02E9 02F4 02FF 030A 0315 0320 032B"            /* .ﬁ.È.Ù.ˇ.¬... .+ */
	$"0336 0341 034C 0357 0362 036D 0378 037F"            /* .6.A.L.W.b.m.x.. */
	$"0383 0387 038B 038E 0390 0396 039B 039F"            /* .É.á.ã.é.ê.ñ.õ.ü */
	$"03A4 03A9 03AE 03B4 03BA 03BF 03C4 03CA"            /* .§.©.Æ.¥.∫.ø.ƒ.  */
	$"03D0 03D5 03DB 03E0 03E7 03ED 03F2 03F8"            /* .–.’.€.‡.Á.Ì.Ú.¯ */
	$"03FF 0404 040A 0410 0416 041C 0422 0428"            /* .ˇ...¬.......".( */
	$"042E 0434 043A 043E 0444 044A 0450 0455"            /* ...4.:.>.D.J.P.U */
	$"045A 0461 0466 046C 0472 0479 047F 0483"            /* .Z.a.f.l.r.y...É */
	$"0489 048F 0495 049B 04A1 04A6 04AC 04B1"            /* .â.è.ï.õ.°.¶.¨.± */
	$"04B7 04BD 04C3 04C9 04CF 04D3 04D7 04D7"            /* .∑.Ω.√.….œ.”.◊.◊ */
	$"04D7 04D7 04D7 04D7 04D7 04D7 04D7 04D7"            /* .◊.◊.◊.◊.◊.◊.◊.◊ */
	$"04D7 04D7 04D7 04D7 04D7 04D7 04D7 04D7"            /* .◊.◊.◊.◊.◊.◊.◊.◊ */
	$"04D7 04D7 04D7 04D7 04D7 04D7 04D7 04D7"            /* .◊.◊.◊.◊.◊.◊.◊.◊ */
	$"04D7 04D7 04D7 04D7 04DC 04E0 04E4 04EB"            /* .◊.◊.◊.◊.‹.‡.‰.Î */
	$"0107 0107 0107 0107 0107 0107 0207 0107"            /* ................ */
	$"0107 0007 0107 0007 0307 0000 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0307 0007 0007 0007"            /* ................ */
	$"0307 0207 0207 0107 0107 0107 0307 0107"            /* ................ */
	$"0707 0307 0207 0107 0107 0107 0007 0307"            /* ................ */
	$"0207 0207 0107 0107 0207 0107 0307 0107"            /* ................ */
	$"0107 0207 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0307 0207 0107 0107 0207 0107"            /* ................ */
	$"0007 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0207 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0207 0107 0207 0107 0007"            /* ................ */
	$"0307 0107 0107 0107 0107 0107 0207 0107"            /* ................ */
	$"0107 0307 0107 0107 0307 0107 0107 0107"            /* ................ */
	$"0107 0107 0207 0107 0207 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0207 0307 0207 0107 0001"            /* ................ */
	$"0107 0001 0004 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"0007 0007 0107 0207 0007 0107 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0107 0007 0307"            /* ................ */
	$"0007 FFFF"                                          /* ..ˇˇ */
};

data 'NFNT' (18444, "ASLFont+e 12") {
	$"F800 0000 00FF 000E 0000 FFFD 000C 000F"            /* ¯....ˇ....ˇ˝.... */
	$"05C6 000C 0003 0000 0051 0000 0000 0000"            /* .∆.......Q...... */
	$"0010 0000 0000 2044 0080 0000 0000 0000"            /* ...... D.Ä...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 000C 0000 0000 0000"            /* ................ */
	$"0000 0011 FC00 0000 2044 0080 0000 0000"            /* ....¸... D.Ä.... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 007F FFFF FFFF FFFF FFFF FFFF FFFF"            /* ....ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF F800"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ¯. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 000C 0000 0000"            /* ................ */
	$"0000 0000 0010 0000 0000 2044 0080 0000"            /* .......... D.Ä.. */
	$"0000 0000 C000 0000 0000 0000 0000 0000"            /* ....¿........... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0383 8000 0000 0000 0000 0000 0000 0000"            /* .ÉÄ............. */
	$"0000 0380 887F FFFF FFFF FFFF FFFF FFFF"            /* ...Äà.ˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"F800 F000 0000 0000 0000 0000 4800 4208"            /* ¯..........H.B. */
	$"0200 2400 0800 0040 0102 0001 0002 0000"            /* ..$....@........ */
	$"200C 0000 1000 0000 0000 0009 600C 0000"            /*  ..........∆`... */
	$"0569 C641 029F 0010 0000 0000 2044 0080"            /* .i∆A.ü...... D.Ä */
	$"0030 ED51 12C5 2000 002E 4E70 BE6F B9C0"            /* .0ÌQ.≈ ...Npæoπ¿ */
	$"000E 01DE 7FBF F747 8630 8C5D E779 FF8C"            /* ...ﬁ.ø˜GÜ0å]Áyˇå */
	$"6318 FE40 9004 0800 2030 4261 0000 0000"            /* c.˛@ê... 0Ba.... */
	$"0000 0000 054D 887F FFFF FFFF FFFF FFFF"            /* .....Mà.ˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF F840 8007 E000 0000 0000 01F8 48F8"            /* ˇˇ¯@Ä.‡......¯H¯ */
	$"4208 FA0F A5C7 C842 FB94 BD02 3DF7 C612"            /* B.˙.•«»B˚îΩ.=˜∆. */
	$"1F90 23F3 206F D073 EF45 483F FF89 9A0C"            /* .ê#Û o–sÔEH?ˇâö. */
	$"3E00 02A9 0842 9391 0010 03F8 0000 2044"            /* >..©.Bìë...¯.. D */
	$"0080 0049 1D53 EB25 2200 0031 D189 A080"            /* .Ä.I.SÎ%"..1—â†Ä */
	$"C620 4111 7A31 8461 08C5 0650 DC63 18C6"            /* ∆ A.z1Ña.≈.P‹c.∆ */
	$"048C 6318 8640 A802 0800 2040 4261 0000"            /* .åc.Ü@®... @Ba.. */
	$"0000 1000 0000 0556 507F FFFF FFFF FFFF"            /* .......VP.ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF F880 8000 3F12 0044 000A 8008"            /* ˇˇˇˇ¯ÄÄ.?..D.¬Ä. */
	$"BF27 FFFE 8BF0 FE00 5FC2 8894 811F 8010"            /* ø'ˇ˛ã˛._¬àîÅ.Ä. */
	$"420B E0A9 FC10 2022 3F10 2045 4821 8409"            /* B.‡©¸. "?. EH!Ñ∆ */
	$"9A0C 2200 2579 8841 1391 0010 0000 0000"            /* ö.".%yàA.ë...... */
	$"2044 0098 0041 0DFD 2D26 1A90 0053 410A"            /*  D.ò.A.˝-&.ê.SA¬ */
	$"A100 C634 8081 8631 8461 0845 0690 AE63"            /* °.∆4ÄÅÜ1Ña.E.êÆc */
	$"18C6 048C 6318 8620 C400 0800 2040 4021"            /* .∆.åc.Ü ƒ... @@! */
	$"0000 0000 1000 0000 0540 207F FFFF FFFF"            /* .........@ .ˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FB60 8100 211F FFFF B9FA"            /* ˇˇˇˇˇˇ˚`Å.!.ˇˇπ˙ */
	$"8009 A120 4248 8A40 A408 48A2 8894 8182"            /* Ä∆° BHä@§.H¢àîÅÇ */
	$"0010 828A 00A8 2010 2022 0910 2045 4821"            /* ..Çä.® . "∆. EH! */
	$"8419 6A0C 2200 72A9 0640 7E91 0010 0000"            /* Ñ.j.".r©.@~ë.... */
	$"0000 2044 00A4 1140 8855 1D42 1710 0053"            /* .. D.§.@àU.B...S */
	$"410C BD01 4635 3E42 B631 8461 0845 0710"            /* A.Ω.F5>B∂1Ña.E.. */
	$"AD63 18C6 048C 62A8 8A20 8000 FF3D EEE7"            /* ≠c.∆.åb®ä Ä.ˇ=ÓÁ */
	$"FA65 F79D E7DB FA31 AC63 F540 F87F FFFF"            /* ˙e˜ùÁ€˙1¨cı@¯.ˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFF0 811F E128 9044"            /* ˇˇˇˇˇˇˇˇˇÅ.·(êD */
	$"881A FE52 A120 C249 0A40 A5C8 8883 6FF4"            /* à.˛R° ¬I¬@•»àÉoÙ */
	$"FF42 0091 0286 00C4 212C 412F C913 FFC5"            /* ˇB.ë.Ü.ƒ!,A/….ˇ≈ */
	$"4821 8410 0A0C 2200 FD69 735C 1291 FFFE"            /* H!Ñ.¬.".˝is\.ëˇ˛ */
	$"0007 F000 3FFF FFC3 FFF2 4853 8882 177C"            /* ...?ˇˇ√ˇÚHSàÇ.| */
	$"0095 4234 83E2 39E2 0024 B7FE 847D E9FD"            /* .ïB4É‚9‚.$∑˛Ñ}È˝ */
	$"0610 ACE3 E8F9 C48C 6A45 1210 8001 18C2"            /* ..¨„Ë˘ƒåjE..Ä..¬ */
	$"3148 C669 AC63 18E4 1231 AAA2 2820 207F"            /* 1H∆i¨c.‰.1™¢(  . */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFE0 8118 2668"            /* ˇˇˇˇˇˇˇˇˇˇˇ‡Å.&h */
	$"90C4 881A 8044 8121 427E 0C40 A408 8902"            /* êƒà.ÄDÅ!B~.@§.â. */
	$"1080 8922 0053 8486 00C4 A8A3 40C2 0A10"            /* .Äâ".SÑÜ.ƒ®£@¬¬. */
	$"20C5 5861 0410 047F A200 728E 42F0 1091"            /*  ≈Xa....¢.réB.ë */
	$"1810 0000 0000 2040 08A4 A240 48F9 5542"            /* ...... @.§¢@H˘UB */
	$"1A90 F899 440F C312 4421 3E44 BE31 8461"            /* .ê¯ôD.√.D!>Dæ1Ña */
	$"08C5 0710 8C63 08C4 248C 6AA2 2210 8001"            /* .≈..åc.ƒ$åj¢".Ä. */
	$"18C2 3F48 C671 AC63 18C3 9231 A922 4540"            /* .¬?H∆q¨c.√í1©"E@ */
	$"F87F FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ¯.ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFE0 8100"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇ‡Å. */
	$"24A0 9145 09F0 8040 8122 4248 0840 8409"            /* $†ëE∆Ä@Å"BH.@Ñ∆ */
	$"4802 1080 8902 0025 4486 0082 A440 4862"            /* H..Äâ..%DÜ.Ç§@Hb */
	$"0810 2085 5861 0410 0380 2200 2544 6298"            /* .. ÖXa...Ä".%Dbò */
	$"7C91 1810 0000 0000 2040 0898 AFC0 0051"            /* |ë...... @.òØ¿.Q */
	$"5B2A 1210 0119 4808 8312 4434 8080 8231"            /* [*....H.É.D4ÄÄÇ1 */
	$"8461 08C5 4690 8C63 08C4 248C 6B12 4208"            /* Ña.≈Fêåc.ƒ$åk.B. */
	$"8001 18C2 3048 C669 AC63 18C0 5231 A922"            /* Ä..¬0H∆i¨c.¿R1©" */
	$"8540 207F FFFF FFFF FFFF FFFF FFFF FFFF"            /* Ö@ .ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFF6"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˆ */
	$"0180 4420 9244 0810 8040 8224 4448 1040"            /* .ÄD íD..Ä@Ç$DH.@ */
	$"8412 4804 2081 0902 0051 0886 0103 2440"            /* Ñ.H. Å∆..Q.Ü..$@ */
	$"8492 0810 2105 58A1 0820 0100 2200 0284"            /* Ñí..!.X°. .."..Ñ */
	$"4310 0091 1810 0000 0FE0 2040 0880 A444"            /* C..ë.....‡ @.Ä§D */
	$"4857 AB11 2001 0511 5088 A312 4454 4104"            /* HW´. ...Pà£.DTA. */
	$"7A31 8461 08C5 4650 8C63 08C4 248A B712"            /* z1Ña.≈FPåc.ƒ$ä∑. */
	$"4208 8001 38C2 3048 C665 AC63 18C0 526A"            /* B.Ä.8¬0H∆e¨c.¿Rj */
	$"AAA3 0540 207F FFFF FFFF FFFF FFFF FFFF"            /* ™£.@ .ˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FBE9 0140 8821 1044 0811 0080 8420 4448"            /* ˚È.@à!.D...ÄÑ DH */
	$"2080 8924 2808 4102 1104 0089 1086 0202"            /*  Äâ$(.A....â.Ü.. */
	$"202C 8502 0810 2209 5921 1240 0100 2200"            /*  ,Ö..."∆Y!.@..". */
	$"0544 4290 00FF 1810 0000 0000 2040 08FE"            /* .DBê.ˇ...... @.˛ */
	$"A078 4801 24E9 2001 060E FF70 9CE2 3984"            /* †xH.$È ...ˇpú‚9Ñ */
	$"0004 023E 7FBF 0747 BA3F 8C5D 0747 C471"            /* ...>.ø.G∫?å].Gƒq */
	$"2312 7E04 83F8 DF3D EF47 C663 AC5D E7C7"            /* #.~.É¯ﬂ=ÔG∆c¨]Á« */
	$"8DA4 545F F540 27FF FFFF FFFF FFFF FFFF"            /* ç§T_ı@'ˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF F949 0F21 1022 7C44 7FF2 0100 88F8"            /* ˇˇ˘I.!."|D.Ú..à¯ */
	$"48C8 410F 91C4 2790 8204 2108 7F01 2105"            /* H»A.ëƒ'êÇ.!...!. */
	$"E402 2023 FE01 C8FF E412 6E3F 2380 0100"            /* ‰. #˛.»ˇ‰.n?#Ä.. */
	$"3E00 0000 0000 0000 1810 0000 0000 2040"            /* >............. @ */
	$"0880 0000 0000 0000 C001 0200 0000 0000"            /* .Ä......¿....... */
	$"0008 0000 0000 0000 0000 0000 0000 0200"            /* ................ */
	$"0000 0000 0387 8000 0000 0000 8040 0001"            /* .....áÄ.....Ä@.. */
	$"0080 0000 0002 0380 007F FFFF FFFF FFFF"            /* .Ä.....Ä..ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF F806 0020 0000 0000 0000 0000"            /* ˇˇˇˇ¯.. ........ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 1810 0000 001F"            /* ................ */
	$"E040 0880 0000 0000 0000 0002 0000 0000"            /* ‡@.Ä............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0100 0000 0000 0000 0000 0000 0000 8040"            /* ..............Ä@ */
	$"0001 0080 0000 0002 0000 0000 0000 0000"            /* ...Ä............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 1810 0000"            /* ................ */
	$"0000 2040 0880 0000 0000 0000 0000 0000"            /* .. @.Ä.......... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0007"            /* ................ */
	$"0180 0001 0080 0000 001C 0000 0000 0000"            /* .Ä...Ä.......... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 000A 000F"            /* .............¬.. */
	$"0014 0019 001E 0021 0026 002B 002B 0030"            /* .......!.&.+.+.0 */
	$"0034 0038 0038 003F 0046 004D 0054 005B"            /* .4.8.8.?.F.M.T.[ */
	$"0062 0066 006A 0071 0078 0079 007C 007F"            /* .b.f.j.q.x.y.|.. */
	$"0084 0089 008E 008F 0094 0094 0095 0098"            /* .Ñ.â.é.è.î.î.ï.ò */
	$"009D 00A2 00A7 00AD 00AE 00B1 00B4 00B9"            /* .ù.¢.ß.≠.Æ.±.¥.π */
	$"00BE 00C0 00C5 00C6 00CB 00D0 00D3 00D8"            /* .æ.¿.≈.∆.À.–.”.ÿ */
	$"00DD 00E2 00E7 00EC 00F1 00F6 00FB 00FC"            /* .›.‚.Á.Ï.Ò.ˆ.˚.¸ */
	$"00FE 0102 0107 010B 0110 0116 011B 0120"            /* .˛.............  */
	$"0125 012A 012F 0134 0139 013E 0141 0146"            /* .%.*...4.9.>.A.F */
	$"014B 0150 0155 015A 015F 0164 0169 016E"            /* .K.P.U.Z._.d.i.n */
	$"0173 0178 017D 0182 0187 018C 0191 0196"            /* .s.x.}.Ç.á.å.ë.ñ */
	$"0199 019E 01A1 01A6 01AD 01AF 01B4 01B9"            /* .ô.û.°.¶.≠.Ø.¥.π */
	$"01BE 01C3 01C8 01CC 01D1 01D6 01D7 01DA"            /* .æ.√.».Ã.—.÷.◊.⁄ */
	$"01DF 01E0 01E5 01EA 01EF 01F4 01F9 01FD"            /* .ﬂ.‡.Â.Í.Ô.Ù.˘.˝ */
	$"0202 0206 020B 0210 0215 021A 021F 0224"            /* ...............$ */
	$"0227 0228 022B 0230 0230 0235 0235 0239"            /* .'.(.+.0.0.5.5.9 */
	$"0245 0251 025D 0269 0275 0281 028D 0299"            /* .E.Q.].i.u.Å.ç.ô */
	$"02A5 02B1 02BD 02C9 02D5 02E1 02ED 02F9"            /* .•.±.Ω.….’.·.Ì.˘ */
	$"0305 0311 031D 0329 0335 0341 034D 0359"            /* .......).5.A.M.Y */
	$"0365 0371 037D 0389 0395 039C 03A0 03A4"            /* .e.q.}.â.ï.ú.†.§ */
	$"03A8 03AB 03AD 03B3 03B8 03BC 03C1 03C6"            /* .®.´.≠.≥.∏.º.¡.∆ */
	$"03CB 03D1 03D7 03DC 03E1 03E7 03ED 03F2"            /* .À.—.◊.‹.·.Á.Ì.Ú */
	$"03F8 03FD 0404 040A 040F 0415 041C 0421"            /* .¯.˝...¬.......! */
	$"0427 042D 0433 0439 043F 0445 044B 0451"            /* .'.-.3.9.?.E.K.Q */
	$"0457 045B 0461 0467 046D 0472 0477 047E"            /* .W.[.a.g.m.r.w.~ */
	$"0483 0489 048F 0496 049C 04A0 04A6 04AC"            /* .É.â.è.ñ.ú.†.¶.¨ */
	$"04B2 04B8 04BE 04C3 04C9 04CE 04D4 04DA"            /* .≤.∏.æ.√.….Œ.‘.⁄ */
	$"04E0 04E6 04EC 04F0 04F4 04F4 04F4 04F4"            /* .‡.Ê.Ï..Ù.Ù.Ù.Ù */
	$"04F4 04F4 04F4 04F4 04F4 04F4 04F4 04F4"            /* .Ù.Ù.Ù.Ù.Ù.Ù.Ù.Ù */
	$"04F4 04F4 04F4 04F4 04F4 04F4 04F4 04F4"            /* .Ù.Ù.Ù.Ù.Ù.Ù.Ù.Ù */
	$"04F4 04F4 04F4 04F4 04F4 04F4 04F4 04F4"            /* .Ù.Ù.Ù.Ù.Ù.Ù.Ù.Ù */
	$"04F4 04F4 04F9 04FD 0501 0508 0107 0107"            /* .Ù.Ù.˘.˝........ */
	$"0107 0107 0107 0107 0207 0107 0107 0007"            /* ................ */
	$"0107 0007 0307 0000 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0307 0007 0007 0007 0307 0207"            /* ................ */
	$"0207 0107 0107 0107 0307 0107 0707 0307"            /* ................ */
	$"0207 0107 0107 0107 0007 0307 0207 0207"            /* ................ */
	$"0107 0107 0207 0107 0307 0107 0107 0207"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0307 0207 0107 0107 0207 0107 0007 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0207"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0207 0107 0207 0107 0007 0307 0107"            /* ................ */
	$"0107 0107 0107 0107 0207 0107 0107 0307"            /* ................ */
	$"0107 0107 0307 0107 0107 0107 0107 0107"            /* ................ */
	$"0207 0107 0207 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0207 0307 0207 0107 0001 0107 0001"            /* ................ */
	$"0004 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 0007 0007"            /* ................ */
	$"0107 0207 0007 0107 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0107 0007 0307 0007 FFFF"            /* ..............ˇˇ */
};

data 'NFNT' (19264, "ANSI/PC 9") {
	$"B000 0000 00FF 0006 0000 FFFE 0009 000B"            /* ∞....ˇ....ˇ˛.∆.. */
	$"042A 0009 0002 0000 0049 0000 0000 3F0F"            /* .*.∆.....I....?. */
	$"C000 0000 0000 0002 0000 0000 0000 0000"            /* ¿............... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0288 6000 0000 0000"            /* .........à`..... */
	$"2800 0000 0000 00AE C000 9240 008A ED25"            /* (......Æ¿.í@.äÌ% */
	$"402D 0552 2080 2002 2294 0280 5005 1450"            /* @-.R Ä ."î.ÄP..P */
	$"0029 0005 0821 F81F FE00 0000 0000 0000"            /* .)...!¯.˛....... */
	$"0000 0000 0008 00C0 01CC 0680 0000 0000"            /* .......¿.Ã.Ä.... */
	$"3F0F C700 C000 0100 0007 0000 0000 0000"            /* ?.«.¿........... */
	$"0800 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0380 0000 6201 8001 00C0 B014 8000 8061"            /* .Ä..b.Ä..¿∞.Ä.Äa */
	$"1805 000C 0004 6663 2953 2001 9640 0235"            /* ......fc)S .ñ@.5 */
	$"3725 402D 0552 2080 2002 2294 0280 5005"            /* 7%@-.R Ä ."î.ÄP. */
	$"1450 0029 0005 0821 F81F FE00 0000 0000"            /* .P.)...!¯.˛..... */
	$"0000 6000 0000 0088 0120 0132 0680 7380"            /* ..`....à. .2.ÄsÄ */
	$"0200 3F0F C300 AF24 0BAB CC0F 9080 0400"            /* ..?.√.Ø$.´Ã.êÄ.. */
	$"0068 1CF6 2600 0002 E4E7 0BEE FB9C 0003"            /* .h.ˆ&...‰Á.Ó˚ú.. */
	$"9CEF 3BDF FBA3 C318 462E F3BC EFC6 318C"            /* úÔ;ﬂ˚£√.F.ÛºÔ∆1å */
	$"7FC6 8081 0004 0608 230C 0000 0000 1000"            /* .∆ÄÅ....#....... */
	$"0000 054D 03A2 8528 4402 942B 4B9D F03D"            /* ...M.¢Ö(D.î+Kù= */
	$"4A12 84A0 4412 8F8A 8884 522E C400 A290"            /* J.Ñ†D.èäàÑR.ƒ.¢ê */
	$"008A ED25 402D 0552 2080 2002 2294 0280"            /* .äÌ%@-.R Ä ."î.Ä */
	$"5005 1450 0029 0005 0821 F81F FE06 7C1F"            /* P..P.)...!¯.˛.|. */
	$"0001 F738 8001 DC02 4348 0121 8F24 0680"            /* ..˜8Ä.‹.CH.!è$.Ä */
	$"8FD4 4711 B36C C570 89AE 1FED 5002 3884"            /* è‘G.≥l≈pâÆ.ÌP.8Ñ */
	$"2400 006A AB59 4910 8003 1D18 9A10 0C62"            /* $..j´YI.Ä...ö..b */
	$"0824 6318 C630 8462 8328 6F31 8C63 1246"            /* .$c.∆0ÑbÉ(o1åc.F */
	$"318C 4343 4041 0004 0808 0104 0000 0000"            /* 1åCC@A.......... */
	$"1000 0000 0556 0440 0000 0000 0000 0463"            /* .....V.@.......c */
	$"0050 0000 0007 4490 5448 0000 0320 0000"            /* .P....DêTH... .. */
	$"A282 D235 3725 402D 0552 2080 2002 2294"            /* ¢Ç“57%@-.R Ä ."î */
	$"0280 5005 1450 0029 0005 0821 F81F FE09"            /* .ÄP..P.)...!¯.˛∆ */
	$"4409 0000 48C4 400A 2202 2549 1B3F C928"            /* D∆..Hƒ@¬".%I.?…( */
	$"0680 DD7E EABB E19B 5C88 8F57 392D 5DF2"            /* .Ä›~Íª·õ\àèW9-]Ú */
	$"7C86 6415 F247 E96A 10D4 8005 5410 ABDE"            /* |Üd.ÚGÈj.‘Ä.T.´ﬁ */
	$"0C63 57D0 6F18 C230 8422 8348 56B1 8C63"            /* .cW–o.¬0Ñ"ÉHV±åc */
	$"0246 3154 4522 001F E73D DCFF 6725 EF3B"            /* .F1TE"..Á=‹ˇg%Ô; */
	$"CFB3 FE31 AC63 F540 2422 E7BD EF73 9CED"            /* œ≥˛1¨cı@$"ÁΩÔsúÌ */
	$"B463 0D51 CE74 6318 C5DC 2788 FCE8 FABF"            /* ¥c.QŒtc.≈‹'à¸Ë˙ø */
	$"E400 4115 288A ED3D 43ED FD5E 2080 2002"            /* ‰.A.(äÌ=CÌ˝^ Ä . */
	$"3E97 FEFF DFFD F753 F029 FE05 3F21 F81F"            /* >ó˛ˇﬂ˝˜S)˛.?!¯. */
	$"FED9 43E4 6A56 A8C5 CA54 23FF 9908 2CDF"            /* ˛ŸC‰jV®≈ T#ˇô.,ﬂ */
	$"C93F E680 8FFF FDFF E19B 6288 89DF F92B"            /* …?ÊÄèˇ˝ˇ·õbàâﬂ˘+ */
	$"63FF 909F FFFE E742 9CA4 10BB E7C5 5423"            /* cˇêüˇ˛ÁBú§.ªÁ≈T# */
	$"4831 13A2 2008 ABFF 423E F4FE 8388 4671"            /* H1.¢ .´ˇB>Ù˛ÉàFq */
	$"F47C E246 3122 8922 0023 18C6 2918 A345"            /* Ù|‚F1"â".#.∆).£E */
	$"58C6 31CC 1231 AAA2 2820 5423 18C6 318C"            /* X∆1Ã.1™¢( T#.∆1å */
	$"6314 97FF E2FE 318C 6318 C630 FC9D 1518"            /* c.óˇ‚˛1åc.∆0¸ù.. */
	$"C660 08FE 411A 1635 37E7 7C8D 11F3 FFFF"            /* ∆`.˛A..57Á|ç.Ûˇˇ */
	$"FFFF E2F0 8000 4000 00FC 0FFF 11FD C0FF"            /* ˇˇ‚Ä.@..¸.ˇ.˝¿ˇ */
	$"FFFF 812B 4142 9269 1FC6 35AF E202 250F"            /* ˇˇÅ+ABíi.∆5Ø‚.%. */
	$"C01D 8801 E680 AEFF FAFF E19B 628B 8B57"            /* ¿.à.ÊÄÆˇ˙ˇ·õbããW */
	$"3FE9 63F7 13E6 6014 4FC7 CADA 90D4 8009"            /* ?Èc˜.Ê`.O« ⁄ê‘Ä∆ */
	$"5440 FC31 245E 17D1 2D18 C230 8462 A348"            /* T@¸1$^.—-.¬0Ñb£H */
	$"4631 8462 1246 3551 1112 0023 1847 E918"            /* F1Ñb.F5Q...#.GÈ. */
	$"A3C5 58C6 3183 9231 A922 4540 8C23 F8C6"            /* £≈X∆1Éí1©"E@å#¯∆ */
	$"3187 FFF4 9463 07D2 318C 6318 C610 25C9"            /* 1áˇÙîc.“1åc.∆.%… */
	$"1518 C620 1082 B2B5 288A ED3D 57ED DF0E"            /* ..∆ .Ç≤µ(äÌ=WÌﬂ. */
	$"4002 2002 3E9F BFFB DFFD FF03 F501 FF45"            /* @. .>üø˚ﬂ˝ˇ.ı.ˇE */
	$"3F11 FFFF 8128 C144 9248 A8AA 35AC 23F2"            /* ?.ˇˇÅ(¡DíH®™5¨#Ú */
	$"4328 1A00 0801 E680 8FDC E211 B36C E277"            /* C(....ÊÄè‹‚.≥l‚w */
	$"9BAE 1B81 6202 11C4 2000 0002 AB59 0910"            /* õÆ.Åb..ƒ ...´Y∆. */
	$"8009 1488 8A31 2442 0820 2118 C630 8462"            /* Ä∆.àä1$B. !.∆0Ñb */
	$"A328 4631 8463 1245 5B89 2112 0027 1846"            /* £(F1Ñc.E[â!..'.F */
	$"0918 A325 58C6 3180 522A AAA2 8540 8C63"            /* ∆.£%X∆1ÄR*™¢Ö@åc */
	$"09CE 7384 2104 9463 0A52 318C 6318 C611"            /* ∆ŒsÑ!.îc¬R1åc.∆. */
	$"FC89 3518 C620 1182 8AB2 D235 3725 54AD"            /* ¸â5.∆ .Çä≤“57%T≠ */
	$"5000 4002 2002 2280 A00A 5005 0000 8500"            /* P.@. ."Ä†¬P...Ö. */
	$"1145 0811 FFFF 8128 C149 9248 48AA 35AA"            /* .E..ˇˇÅ(¡IíHH™5™ */
	$"2200 0129 2C00 2800 0680 7388 4738 3F0F"            /* "..),.(..ÄsàG8?. */
	$"DC23 1824 0929 5C0F 9080 0000 0040 1D26"            /* ‹#.$∆)\.êÄ...@.& */
	$"8600 0830 EFF7 09CE 239D 4001 1D1F 3BDF"            /* Ü..0Ô˜∆Œ#ù@...;ﬂ */
	$"83A3 DD1F C62E 83A2 E238 9189 3F8E 3F1B"            /* É£›.∆.É¢‚8ëâ?é?. */
	$"E7BD E8F8 F31F 58BB CF87 8DE4 545F F540"            /* ÁΩË¯Û.Xªœáç‰T_ı@ */
	$"FB9E F6B5 AD7B DEFF FC63 F5DD CE73 DEF7"            /* ˚ûˆµ≠{ﬁˇ¸cı›Œsﬁ˜ */
	$"39FE 24E8 DEE7 C620 0E83 14F0 008A ED25"            /* 9˛$ËﬁÁ∆ .É..äÌ% */
	$"54AD 5000 4002 2002 2280 A00A 5005 0000"            /* T≠P.@. ."Ä†¬P... */
	$"8500 1145 0811 FFFF 80DB 415F 61C9 F76D"            /* Ö..E..ˇˇÄ€A_a…˜m */
	$"CA51 E3FF FF10 0000 1800 0680 0000 0000"            /*  Q„ˇˇ......Ä.... */
	$"3F0F C070 0000 0000 0400 0000 0000 0000"            /* ?.¿p............ */
	$"0800 0000 0810 0000 0000 0000 4000 0000"            /* ............@... */
	$"0000 0000 0000 0000 0040 0000 0000 0008"            /* .........@...... */
	$"0000 0000 0010 0200 0002 0100 0000 0002"            /* ................ */
	$"0380 0080 0000 0010 0000 0000 0000 0000"            /* .Ä.Ä............ */
	$"0010 0080 0010 0000 0000 0001 3C20 0235"            /* ...Ä........< .5 */
	$"3725 54AD 5000 4002 2002 2280 A00A 5005"            /* 7%T≠P.@. ."Ä†¬P. */
	$"0000 8500 1145 0811 FFFF 8000 0000 0100"            /* ..Ö..E..ˇˇÄ..... */
	$"0000 0080 0000 0100 0000 0000 0680 0000"            /* ...Ä.........Ä.. */
	$"0000 3F0F C020 0000 0000 1800 0000 0000"            /* ..?.¿ .......... */
	$"0000 0000 0000 1000 0000 0000 0000 8000"            /* ..............Ä. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 00E0 0C00 0002 0100 0000"            /* .......‡........ */
	$"001C 0000 0300 0000 0060 0000 0000 0000"            /* .........`...... */
	$"0000 00E0 0000 0000 0000 0000 0000 0000"            /* ...‡............ */
	$"008A ED25 54AD 5000 4002 2002 2280 A00A"            /* .äÌ%T≠P.@. ."Ä†¬ */
	$"5005 0000 8500 1145 0811 FFFF 8000 0000"            /* P...Ö..E..ˇˇÄ... */
	$"0600 0000 0000 0000 0100 0000 0000 0680"            /* ...............Ä */
	$"0000 0000 0005 000A 000F 0014 0019 001E"            /* .......¬........ */
	$"0022 0028 002C 0032 0038 003D 0043 0048"            /* .".(.,.2.8.=.C.H */
	$"004D 0051 0055 005A 005D 0062 0067 006C"            /* .M.Q.U.Z.].b.g.l */
	$"0071 0076 007B 0080 0085 008A 008F 0094"            /* .q.v.{.Ä.Ö.ä.è.î */
	$"0099 0099 009A 009D 00A2 00A7 00AC 00B1"            /* .ô.ô.ö.ù.¢.ß.¨.± */
	$"00B3 00B6 00B9 00BE 00C3 00C5 00CA 00CB"            /* .≥.∂.π.æ.√.≈. .À */
	$"00CF 00D4 00D7 00DC 00E1 00E6 00EB 00F0"            /* .œ.‘.◊.‹.·.Ê.Î. */
	$"00F5 00FA 00FF 0100 0102 0105 010A 010D"            /* .ı.˙.ˇ.......¬.. */
	$"0112 0117 011C 0121 0126 012B 0130 0135"            /* .......!.&.+.0.5 */
	$"013A 013F 0142 0147 014C 0151 0156 015B"            /* .:.?.B.G.L.Q.V.[ */
	$"0160 0165 016A 016F 0174 0179 017E 0183"            /* .`.e.j.o.t.y.~.É */
	$"0188 018D 0192 0197 0199 019D 019F 01A2"            /* .à.ç.í.ó.ô.ù.ü.¢ */
	$"01A8 01AA 01AF 01B4 01B9 01BE 01C3 01C7"            /* .®.™.Ø.¥.π.æ.√.« */
	$"01CC 01D1 01D4 01D7 01DC 01DF 01E4 01E9"            /* .Ã.—.‘.◊.‹.ﬂ.‰.È */
	$"01EE 01F3 01F8 01FD 0202 0206 020B 0210"            /* .Ó.Û.¯.˝........ */
	$"0215 021A 021F 0224 0227 0228 022B 0230"            /* .......$.'.(.+.0 */
	$"0235 023A 023F 0244 0249 024E 0253 0258"            /* .5.:.?.D.I.N.S.X */
	$"025D 0262 0267 026C 026F 0272 0275 027A"            /* .].b.g.l.o.r.u.z */
	$"027F 0284 0289 028E 0293 0298 029D 02A2"            /* ...Ñ.â.é.ì.ò.ù.¢ */
	$"02A7 02AC 02B1 02B6 02BB 02C0 02C5 02CA"            /* .ß.¨.±.∂.ª.¿.≈.  */
	$"02CF 02D4 02D7 02DC 02E1 02E6 02EB 02EF"            /* .œ.‘.◊.‹.·.Ê.Î.Ô */
	$"02F3 02F8 02FC 02FF 0305 030B 030C 0311"            /* .Û.¯.¸.ˇ........ */
	$"0316 031B 0321 0327 0328 032B 032E 0332"            /* .....!.'.(.+...2 */
	$"0336 0339 033D 0340 0344 0348 034C 034F"            /* .6.9.=.@.D.H.L.O */
	$"0352 0356 035C 0362 0366 036C 0372 0376"            /* .R.V.\.b.f.l.r.v */
	$"037B 0380 0385 038B 0391 0396 039C 03A2"            /* .{.Ä.Ö.ã.ë.ñ.ú.¢ */
	$"03A8 03AE 03B4 03BA 03BF 03C3 03C7 03CC"            /* .®.Æ.¥.∫.ø.√.«.Ã */
	$"03D2 03D8 03DB 03DF 03E5 03EB 03EE 03F1"            /* .“.ÿ.€.ﬂ.Â.Î.Ó.Ò */
	$"03F7 03FC 0401 0406 040B 0410 0415 041A"            /* .˜.¸............ */
	$"041F 0424 0429 042E 0433 0438 043D 0442"            /* ...$.)...3.8.=.B */
	$"0447 044C 0451 0454 0457 045A 045D 0462"            /* .G.L.Q.T.W.Z.].b */
	$"0467 046B 046E 0472 0477 047B 047F 0482"            /* .g.k.n.r.w.{...Ç */
	$"0482 0482 0006 0006 0006 0006 0006 0006"            /* .Ç.Ç............ */
	$"0006 0106 0006 0106 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0106 0106 0006 0106 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0206 0106 0006 0006 0006"            /* ................ */
	$"0006 0106 0106 0106 0006 0006 0106 0006"            /* ................ */
	$"0206 0106 0006 0106 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0206 0106 0106 0006"            /* ................ */
	$"0106 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0106 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0206 0106 0106"            /* ................ */
	$"0106 0006 0206 0006 0006 0006 0006 0006"            /* ................ */
	$"0106 0006 0006 0106 0006 0006 0106 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0106 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0106 0206 0106"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0106 0106 0106"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0106 0006 0006 0006 0006"            /* ................ */
	$"0106 0106 0006 0206 0006 0006 0006 0206"            /* ................ */
	$"0006 0006 0006 0006 0006 0206 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0106 0006 0006 0006"            /* ................ */
	$"0006 0006 0206 0006 0006 0206 0006 0006"            /* ................ */
	$"0206 0106 0106 0106 0006 0006 0106 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0106 0206 0206"            /* ................ */
	$"0106 0006 0006 0006 0206 0006 0006 0006"            /* ................ */
	$"0306 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0106 0106 0206 0006"            /* ................ */
	$"0006 0006 0106 0106 0106 0006 0106 0106"            /* ................ */
	$"0106 0006 0006 FFFF"                                /* ......ˇˇ */
};

data 'NFNT' (19265, "ANSI/PC 10") {
	$"9000 0000 00FF 0007 FFFF FFFD 000C 000D"            /* ê....ˇ..ˇˇˇ˝.... */
	$"04D6 000A 0003 0000 004B 0000 0000 3F0F"            /* .÷.¬.....K....?. */
	$"C000 0000 0000 0001 0000 0000 0000 0000"            /* ¿............... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 4000 0000 2882 0000"            /* ........@...(Ç.. */
	$"0000 0002 8000 0000 0000 0A00 0024 9000"            /* ....Ä.....¬..$ê. */
	$"8D4D C950 0B41 5488 2008 0088 A500 A014"            /* çM…P.ATà ..à•.†. */
	$"0145 1400 0520 00A1 043F 03FF C000 01F0"            /* .E... .°.?.ˇ¿.. */
	$"000F 8000 0000 0000 0044 0060 0099 0000"            /* ..Ä......D.`.ô.. */
	$"0000 0000 3F0F C000 0000 0100 0003 8000"            /* ....?.¿.......Ä. */
	$"0000 0000 0200 0018 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 8000 0002 2008 A010"            /* ........Ä... .†. */
	$"080A 0144 0008 0411 0000 0000 0042 2211"            /* .¬.D.........B". */
	$"54D0 0065 9000 22BB 4950 0B41 5488 2008"            /* T–.eê."ªIP.ATà . */
	$"0088 A500 A014 0145 1400 0520 00A1 043F"            /* .à•.†..E... .°.? */
	$"03FF C007 C100 0002 3803 0000 0000 00A4"            /* .ˇ¿.¡...8......§ */
	$"0090 C792 0000 0000 0000 3F0F C700 C000"            /* .ê«í......?.«.¿. */
	$"03A8 0007 C840 0000 0034 A722 C124 2000"            /* .®..»@...4ß"¡$ . */
	$"0171 1CE1 7CDF 7380 0000 E313 CEE7 FEE8"            /* .q.·|ﬂsÄ..„.ŒÁ˛Ë */
	$"F7C6 118B BCE7 9DF8 C631 8FFC 7200 0400"            /* ˜∆.ãºÁù¯∆1è¸r... */
	$"1018 2086 1000 0000 0040 0000 0003 E000"            /* .. Ü.....@....‡. */
	$"3944 5284 4029 44B5 109F 03D4 A228 8A54"            /* 9DRÑ@)Dµ.ü.‘¢(äT */
	$"4868 80A4 4422 A369 0028 A400 8D4D C950"            /* HhÄ§D"£i.(§.çM…P */
	$"0B41 5488 2008 0088 A500 A014 0145 1400"            /* .ATà ..à•.†..E.. */
	$"0520 00A1 043F 03FF C0E4 4080 0002 45C4"            /* . .°.?.ˇ¿‰@Ä..Eƒ */
	$"0000 E000 00A4 009F E494 1FF0 7380 0200"            /* ..‡..§.ü‰î.sÄ.. */
	$"3F0F C300 AF24 0FEB CC01 1C40 0200 0034"            /* ?.√.Ø$.ÎÃ..@...4 */
	$"AAD3 2242 A800 018B 2313 4111 8C40 0001"            /* ™”"B®..ã#.A.å@.. */
	$"1492 3194 2118 A0CA 1BCC 6314 6248 C631"            /* .í1î!.† .Ãc.bH∆1 */
	$"8864 1202 0400 1020 2002 1000 0000 0040"            /* àd.....  ......@ */
	$"0000 0004 9000 4400 0000 0000 0000 1090"            /* ....ê.D........ê */
	$"0300 0000 0004 4888 F880 0000 3368 0028"            /* ......Hà¯Ä..3h.( */
	$"A000 22BB 4950 0B41 5488 2008 0088 A500"            /* †."ªIP.ATà ..à•. */
	$"A014 0145 1400 0520 00A1 043F 03FF C114"            /* †..E... .°.?.ˇ¡. */
	$"0080 22B7 4762 000F 1012 0184 009F E49F"            /* .Ä"∑Gb.....Ñ.ü‰ü */
	$"1330 8FD4 4711 B36C C570 89AE 192D 5001"            /* .0è‘G.≥l≈pâÆ.-P. */
	$"3E42 1200 0037 FA55 0442 7000 02AD 0215"            /* >B...7˙U.Bp..≠.. */
	$"4201 8C7C 0000 1992 308C 2108 A0D2 1BCC"            /* B.å|...í0å!.†“.Ã */
	$"6314 6048 C6AA 50A2 1501 7D9C F77B ED9E"            /* c.`H∆™P¢..}ú˜{Ìû */
	$"56AC EB3F 6EF4 6318 C7E4 9000 422E 7BDE"            /* V¨Î?nÙc.«‰ê.B.{ﬁ */
	$"F739 CEDB 1090 731C E746 3174 5C85 448F"            /* ˜9Œ€.ês.ÁF1t\ÖDè */
	$"CE8D B2D1 0010 44B4 8D4D C950 0B41 5488"            /* Œç≤—..D¥çM…P.ATà */
	$"2008 0088 A500 A014 0145 1400 0520 00A1"            /*  ..à•.†..E... .° */
	$"043F 03FF DB24 3E47 6348 C62E 0051 1011"            /* .?.ˇ€$>GcH∆..Q.. */
	$"8684 8D6E C490 1330 DD7E EABB E19B 5C88"            /* ÜÑçnƒê.0›~Íª·õ\à */
	$"8F57 392D 5DF9 0843 320A F920 A628 8081"            /* èW9-]˘.C2¬˘ ¶(ÄÅ */
	$"7100 02A9 0469 7BC2 747C 3060 2AAB D08F"            /* q..©.i{¬t|0`*´–è */
	$"BD0F A0E2 15AC 6314 5C48 C6A4 5122 1500"            /* Ω.†‚.¨c.\H∆§Q".. */
	$"8E63 18A4 7286 9573 1CC5 9144 6315 4444"            /* éc.§rÜïs.≈ëDc.DD */
	$"9192 4231 8C63 18C6 3149 295E ABE3 18C6"            /* ëíB1åc.∆1I)^´„.∆ */
	$"318C 6BE2 7891 518E 6A01 0010 454A 22BB"            /* 1åk‚xëQéj...EJ"ª */
	$"4F50 FB7F 5788 2008 008F A5FF BFF7 FF7D"            /* OP˚.Wà ..è•ˇø˜ˇ} */
	$"D4FC 053F C0A7 E43F 03FF E544 1448 A248"            /* ‘¸.?¿ß‰?.ˇÂD.H¢H */
	$"FE31 52A1 1FFC 7884 1600 0400 F330 8FFF"            /* ˛1R°.¸xÑ....Û0èˇ */
	$"FDFF E19B 6288 89DF F92B 63FF C84F FFFF"            /* ˝ˇ·õbàâﬂ˘+cˇ»Oˇˇ */
	$"73A0 A315 4081 A900 04A9 081F C622 8BC0"            /* s†£.@Å©..©..∆"ã¿ */
	$"CF98 4AAA 308C 2138 A0E2 11AC 7D17 8248"            /* œòJ™0å!8†‚.¨}.ÇH */
	$"AAA4 2121 1880 8C61 1FA4 6287 1563 18C5"            /* ™§!!.Äåa.§bá.c.≈ */
	$"0C44 5552 2884 9265 423F 8C63 187F FF49"            /* .DUR(ÑíeB?åc..ˇI */
	$"2950 BF23 18C6 2A8C 688F C9D1 518C 6BFA"            /* )Pø#.∆*åhè…—Qåk˙ */
	$"3FAC AE85 8D4D F9DF 2344 7CFF FFFF FFF8"            /* ?¨ÆÖçM˘ﬂ#D|ˇˇˇˇ¯ */
	$"BC20 0010 0000 3F03 FFE2 3FB8 1FFF FFF0"            /* º ....?.ˇ‚?∏.ˇˇ */
	$"2534 1488 A247 4631 AD7F 1011 8687 E000"            /* %4.à¢GF1≠...Üá‡. */
	$"0400 F330 AEFF FAFF E19B 628B 8B57 3FE9"            /* ..Û0Æˇ˙ˇ·õbããW?È */
	$"63FB 89F3 300A 27E3 F2AB 2881 27CF 88A9"            /* c˚âÛ0¬'„Ú´(Å'œà© */
	$"1011 0624 8843 0006 49FE 308C 2118 A8D2"            /* ...$àC..I˛0å!.®“ */
	$"119C 6115 0248 A94A 2221 1000 8C61 1824"            /* .úa..H©J"!..åa.$ */
	$"6286 9563 18C5 0244 5552 2918 8C08 C230"            /* bÜïc.≈.DUR).å.¬0 */
	$"8C63 1842 1049 7FF0 A523 18C6 2A8C 6882"            /* åc.B.I.•#.∆*åhÇ */
	$"5C91 518C 6604 20A2 AD4A 22BB 4F55 FB77"            /* \ëQåf. ¢≠J"ªOU˚w */
	$"C390 0088 008F A7EF FEF7 FF7F C0FD 403F"            /* √ê.à.èßÔ˛˜ˇ.¿˝@? */
	$"E8A7 E23F FFF0 250C 1488 B642 4631 AD61"            /* Ëß‚?ˇ%..à∂BF1≠a */
	$"1F92 0184 0D00 0400 F330 8FDC E211 B36C"            /* .í.Ñ....Û0è‹‚.≥l */
	$"E277 9BAE 1B81 6201 08E2 1000 0001 4ACB"            /* ‚wõÆ.Åb..‚....JÀ */
	$"1081 0130 6889 2111 0624 88BC CF98 0446"            /* .Å.0hâ!..$àºœò.F */
	$"3194 2118 A8CA 119C 6114 A248 9151 2420"            /* 1î!.® .úa.¢HëQ$  */
	$"9000 9C63 38A4 E286 5563 18CD 114C C8A5"            /* ê.úc8§‚ÜUc.Õ.L»• */
	$"1204 9008 C671 9CE7 38C6 3149 4630 A523"            /* ..ê.∆qúÁ8∆1IF0•# */
	$"18CE 648C 6B1F C893 519C 6604 60C5 3CB4"            /* .Œdåk.»ìQúf.`≈<¥ */
	$"8D4D C955 2B54 0010 0088 0088 A028 0294"            /* çM…U+T...à.à†(.î */
	$"0140 0021 4002 28A1 023F FFF0 250C 1508"            /* .@.!@.(°.?ˇ%... */
	$"AA42 4551 AD51 1000 0084 9600 1400 1330"            /* ™BEQ≠Q...Ññ....0 */
	$"7388 4738 3F0F DC23 1824 0929 5C07 C840"            /* sàG8?.‹#.$∆)\.»@ */
	$"0000 0021 4744 E842 0130 7077 FEE1 79C4"            /* ...!GDËB.0pw˛·yƒ */
	$"733C 3060 43C7 CEE7 E0E8 F747 F18B A0E4"            /* s<0`C«ŒÁ‡Ë˜GÒã†‰ */
	$"5C47 1151 27E0 9000 6F9C D723 62C6 3D62"            /* \G.Q'‡ê.oú◊#b∆=b */
	$"EF37 8E33 48A8 93E4 900F B9AE 6B5A D739"            /* Ô7é3H®ì‰ê.πÆkZ◊9 */
	$"CE6D C63F 7DDC E735 A473 9DF2 4C8D 6E6C"            /* Œm∆?}‹Á5§sùÚLçnl */
	$"6203 A0CF 0C00 22BB 4955 2B54 0010 0088"            /* b.†œ.."ªIU+T...à */
	$"0088 A028 0294 0140 0021 4002 28A1 023F"            /* .à†(.î.@.!@.(°.? */
	$"FFF0 1B74 15F7 200F BB6E 528F 1FFF FF94"            /* ˇ.t.˜ .ªnRè.ˇˇî */
	$"0000 0C00 1FF0 0000 0000 3F0F C070 0000"            /* .........?.¿p.. */
	$"0000 0400 0000 0000 0000 0200 0042 0010"            /* .............B.. */
	$"1000 0000 0000 0004 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0020 0000 0000 0020 107C 0000"            /* ..... ..... .|.. */
	$"0000 4004 0000 0804 0000 0000 2004 9000"            /* ..@......... .ê. */
	$"1000 0000 0200 0000 0000 0000 0000 0800"            /* ................ */
	$"0800 0280 0000 0000 0000 0000 8D4D C955"            /* ...Ä........çM…U */
	$"2B54 0010 0088 0088 A028 0294 0140 0021"            /* +T...à.à†(.î.@.! */
	$"4002 28A1 023F FFF0 0000 0000 2000 0000"            /* @.(°.?ˇ.... ... */
	$"0400 0000 0094 0000 0000 0000 0000 0000"            /* .....î.......... */
	$"3F0F C020 0000 0000 1800 0000 0000 0000"            /* ?.¿ ............ */
	$"0000 0024 0020 0000 0000 0000 0008 0000"            /* ...$. .......... */
	$"0000 0000 0000 0000 0000 0018 0000 0000"            /* ................ */
	$"0020 1000 0000 0003 8038 0000 0804 0000"            /* . ......Ä8...... */
	$"0000 4004 9000 0800 0000 0100 0000 0000"            /* ..@.ê........... */
	$"0000 0000 1000 0800 0100 0000 0000 0000"            /* ................ */
	$"0000 22BB 4955 2B54 0010 0088 0088 A028"            /* .."ªIU+T...à.à†( */
	$"0294 0140 0021 4002 28A1 023F FFF0 0000"            /* .î.@.!@.(°.?ˇ.. */
	$"0000 0000 0000 0000 0000 0088 0000 0000"            /* ...........à.... */
	$"0000 0000 0000 3F0F C000 0000 0000 0000"            /* ......?.¿....... */
	$"0000 0000 0000 0000 0018 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0038 7000 0000 0000 0000"            /* .......8p....... */
	$"0000 0000 0000 0000 0003 6000 1000 0000"            /* ..........`..... */
	$"0200 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 8D4D C955 2B54 0010"            /* ........çM…U+T.. */
	$"0088 0088 A028 0294 0140 0021 4002 28A1"            /* .à.à†(.î.@.!@.(° */
	$"023F FFF0 0000 0000 0000 0000 0000 0000"            /* .?ˇ............ */
	$"0000 0000 0000 0000 0000 0000 0005 000A"            /* ...............¬ */
	$"000F 0014 0019 001E 0022 0028 002C 0032"            /* .........".(.,.2 */
	$"0038 003D 0043 0048 004D 0051 0055 005A"            /* .8.=.C.H.M.Q.U.Z */
	$"005D 0062 0067 006D 0072 0077 007C 0081"            /* .].b.g.m.r.w.|.Å */
	$"0086 008B 0090 0095 009A 009A 009B 009E"            /* .Ü.ã.ê.ï.ö.ö.õ.û */
	$"00A4 00A9 00AF 00B5 00B8 00BC 00C0 00C5"            /* .§.©.Ø.µ.∏.º.¿.≈ */
	$"00CA 00CC 00D1 00D3 00D8 00DD 00E2 00E7"            /* . .Ã.—.”.ÿ.›.‚.Á */
	$"00EC 00F1 00F6 00FB 0100 0105 010A 010C"            /* .Ï.Ò.ˆ.˚.....¬.. */
	$"010E 0114 0119 011F 0124 0129 012E 0133"            /* .........$.)...3 */
	$"0138 013D 0142 0147 014C 0151 0154 0159"            /* .8.=.B.G.L.Q.T.Y */
	$"015E 0163 0168 016D 0172 0177 017D 0182"            /* .^.c.h.m.r.w.}.Ç */
	$"0187 018C 0191 0196 019B 01A0 01A5 01AA"            /* .á.å.ë.ñ.õ.†.•.™ */
	$"01AD 01B1 01B4 01B9 01BE 01C0 01C5 01CA"            /* .≠.±.¥.π.æ.¿.≈.  */
	$"01CF 01D4 01D9 01DD 01E2 01E7 01EA 01EE"            /* .œ.‘.Ÿ.›.‚.Á.Í.Ó */
	$"01F3 01F5 01FA 01FF 0204 0209 020E 0213"            /* .Û.ı.˙.ˇ...∆.... */
	$"0218 021D 0222 0227 022C 0231 0236 023B"            /* .....".'.,.1.6.; */
	$"0240 0241 0246 024C 0251 0256 025B 0260"            /* .@.A.F.L.Q.V.[.` */
	$"0265 026A 026F 0274 0279 027E 0283 0288"            /* .e.j.o.t.y.~.É.à */
	$"028B 028E 0291 0296 029B 02A0 02A5 02AA"            /* .ã.é.ë.ñ.õ.†.•.™ */
	$"02AF 02B4 02B9 02BE 02C3 02C8 02CD 02D2"            /* .Ø.¥.π.æ.√.».Õ.“ */
	$"02D7 02DC 02E1 02E6 02EB 02F0 02F3 02F8"            /* .◊.‹.·.Ê.Î..Û.¯ */
	$"02FD 0302 0307 030A 030D 0312 0316 0319"            /* .˝.....¬........ */
	$"031F 0325 0326 032B 0330 0335 033B 0341"            /* ...%.&.+.0.5.;.A */
	$"0342 0345 0348 034C 0350 0353 0357 035A"            /* .B.E.H.L.P.S.W.Z */
	$"035E 0362 0366 0369 036C 0370 0376 037C"            /* .^.b.f.i.l.p.v.| */
	$"0380 0386 038C 0390 0395 039A 039F 03A5"            /* .Ä.Ü.å.ê.ï.ö.ü.• */
	$"03AB 03B0 03B6 03BC 03C2 03C8 03CE 03D4"            /* .´.∞.∂.º.¬.».Œ.‘ */
	$"03DA 03DE 03E2 03E7 03ED 03F3 03F6 03FA"            /* .⁄.ﬁ.‚.Á.Ì.Û.ˆ.˙ */
	$"0400 0406 0409 040C 0412 0417 041D 0422"            /* .....∆........." */
	$"0427 042C 0432 0437 043C 0441 0446 044B"            /* .'.,.2.7.<.A.F.K */
	$"0450 0455 045A 045F 0464 0469 046E 0473"            /* .P.U.Z._.d.i.n.s */
	$"0478 047B 047E 0483 0488 048C 048F 0493"            /* .x.{.~.É.à.å.è.ì */
	$"0498 049C 04A0 04A3 04A3 04A7 0100 0106"            /* .ò.ú.†.£.£.ß.... */
	$"0106 0106 0106 0106 0106 0206 0106 0206"            /* ................ */
	$"0106 0106 0106 0106 0106 0207 0206 0206"            /* ................ */
	$"0106 0206 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0306"            /* ................ */
	$"0206 0106 0106 0106 0106 0206 0206 0206"            /* ................ */
	$"0106 0106 0306 0106 0306 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0306 0306 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0206"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0306 0206 0206 0106 0106 0306 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0206"            /* ................ */
	$"0106 0106 0306 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0306 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0206 0206 0206 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0206"            /* ................ */
	$"0106 0106 0106 0106 0206 0206 0106 0306"            /* ................ */
	$"0106 0106 0106 0306 0106 0106 0106 0106"            /* ................ */
	$"0106 0306 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0206 0106 0106 0106 0106 0106 0306 0106"            /* ................ */
	$"0106 0306 0106 0106 0306 0206 0206 0206"            /* ................ */
	$"0106 0106 0206 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0306 0306 0206 0106 0106 0106"            /* ................ */
	$"0306 0106 0106 0106 0406 0106 0106 0106"            /* ................ */
	$"0206 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0106 0106 0106 0106 0106 0106"            /* ................ */
	$"0106 0106 0306 0106 0106 0206 0206 0206"            /* ................ */
	$"0206 0106 0206 0206 0206 0006 0206 FFFF"            /* ..............ˇˇ */
};

data 'NFNT' (20489, "ASLFont+ 9") {
	$"F800 0000 00FF 000C 0000 FFFE 000C 000C"            /* ¯....ˇ....ˇ˛.... */
	$"04A3 000A 0002 0001 004D 0000 0000 0000"            /* .£.¬.....M...... */
	$"0080 0000 0020 9004 0000 0000 0000 0000"            /* .Ä... ê......... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFF0 0000 0000"            /* ˇˇˇˇˇˇˇˇˇˇˇ.... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0C00 0000 0000 0000 008F C000 0020"            /* ...........è¿..  */
	$"9004 0000 0000 0000 0000 0000 0000 0000"            /* ê............... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 FFFF FFFF FFFF"            /* ..........ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFF0 0000 0000 0000 0000 0000"            /* ˇˇˇˇˇ.......... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0C00 0029"            /* ...............) */
	$"C640 0280 0080 0000 0020 9004 0000 0000"            /* ∆@.Ä.Ä... ê..... */
	$"0000 0000 0004 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0110"            /* ................ */
	$"0000 0000 0000 0040 0000 0000 0000 0000"            /* .......@........ */
	$"E7E0 8000 FFFF FFFF FFFF FFFF FFFF FFFF"            /* Á‡Ä.ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFF0"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"03C0 0000 0000 0000 0000 1002 4100 8028"            /* .¿..........A.Ä( */
	$"0040 0010 0210 0020 0A00 0080 0000 0800"            /* .@..... ¬..Ä.... */
	$"0000 0000 0135 0C78 0569 0841 139E 0080"            /* .....5.x.i.A.û.Ä */
	$"3F00 0020 9004 C001 8768 0896 4C00 0005"            /* ?.. ê.¿.áh.ñL... */
	$"CB9C 2FBB EE70 000E 73BC FF7F EE8F 0C61"            /* Àú/ªÓp..sºˇ.Óè.a */
	$"18BB CEF3 FF18 C631 FF1D 0102 0008 0C10"            /* .ªŒÛˇ.∆1ˇ....... */
	$"9840 0000 0004 0000 0001 500D 8000 FFFF"            /* ò@........P.Ä.ˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFF0 8200 3E00 0000"            /* ˇˇˇˇˇˇˇˇˇÇ.>... */
	$"0000 01F0 91C2 411E 802B 1F44 5E2A BA11"            /* ...ë¬A.Ä+.D^*∫. */
	$"DFF8 861F 0080 C107 E877 FF2A 401F 04CD"            /* ﬂ¯Ü..Ä¡.Ëwˇ*@..Õ */
	$"0C48 22B9 8842 9392 0080 0000 0020 9005"            /* .H"πàBìí.Ä... ê. */
	$"208A 58EA 9F59 5221 000A 3C62 6840 3188"            /*  äXÍüYR!.¬<bh@1à */
	$"2091 8C63 08C2 118A 0CA1 BCC6 318C 0918"            /*  ëåc.¬.ä.°º∆1å∆. */
	$"C631 0CA6 8082 0008 1010 0840 0000 0004"            /* ∆1.¶ÄÇ.....@.... */
	$"0000 0001 5016 4000 FFFF FFFF FFFF FFFF"            /* ....P.@.ˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFF1 0200 03F2 4009 0000 0011 7C9F"            /* ˇˇˇÒ...Ú@∆....|ü */
	$"FFD2 FFFC 01FC 52CA 827C 0108 A601 43FF"            /* ˇ“ˇ¸.¸R Ç|..¶.Cˇ */
	$"3205 1F10 412A 43F1 C24D 0C48 7569 775D"            /* 2...A*CÒ¬M.Huiw] */
	$"7E93 FFF0 00FC 003F FFFE 1FFE 0847 E96A"            /* ~ìˇ.¸.?ˇ˛.˛.GÈj */
	$"21A9 000A 6842 AF78 318D 5F41 EC63 08C2"            /* !©.¬hBØx1ç_AÏc.¬ */
	$"108A 0D21 5AC6 318C 0918 C551 15F4 003F"            /* .ä.!Z∆1å∆.≈Q.Ù.? */
	$"CF7B B9FE 997D 6779 F6FE 8C6B 18FD 5000"            /* œ{π˛ô}gyˆ˛åk.˝P. */
	$"4000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* @.ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFF6 C204"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˆ¬. */
	$"0215 F77F EEFD 4052 4482 4922 A0A8 024A"            /* ..˜.Ó˝@RDÇI"†®.J */
	$"6A4A FF10 0110 A7E1 A081 8429 0910 7F2A"            /* jJˇ...ß·†ÅÑ)∆..* */
	$"4231 0832 3FC8 FAAF 42F0 1092 6080 0000"            /* B1.2?»˙ØB.í`Ä.. */
	$"0020 8045 2513 8442 9CA4 2177 CF92 A88D"            /* . ÄE%.ÑBú§!wœí®ç */
	$"20C4 4E88 8022 AFFD 08FB D3FA 0E21 59C7"            /*  ƒNàÄ"Ø˝.˚”˙.!Y« */
	$"D1F3 8918 D48A 2444 0046 308C 5231 9A6B"            /* —Ûâ.‘ä$D.F0åR1ök */
	$"98C6 3904 8C6A A88A 0800 2000 FFFF FFFF"            /* ò∆9.åj®ä.. .ˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF 9A04 FE6D 1209 220D"            /* ˇˇˇˇˇˇˇˇö.˛m.∆". */
	$"4066 4486 4FC3 20AB 2450 4BF0 9290 0B70"            /* @fDÜO√ ´$PKíê.p */
	$"A601 92A1 691B EA17 C32A 4621 0803 C048"            /* ¶.í°i.Í.√*F!..¿H */
	$"7544 6298 1092 6080 0003 F020 8044 C57E"            /* uDbò.í`Ä.. ÄD≈~ */
	$"0247 CADA A1A9 0013 2903 F0C4 9178 5F44"            /* .G ⁄°©..).ƒëx_D */
	$"F463 08C2 118A 8D21 18C6 1188 4918 D544"            /* Ùc.¬.äç!.∆.àI.’D */
	$"45F4 0046 308F D231 9E6B 18C6 30E4 8C6A"            /* EÙ.F0è“1ûk.∆0‰åj */
	$"4891 5000 2000 FFFF FFFF FFFF FFFF FFFF"            /* HëP. .ˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF E404 0244 1219 4278 7E4A 048A 4904"            /* ˇˇ‰..D..Bx~J.äI. */
	$"2088 2A40 8441 1210 04A9 2602 0AAA 0895"            /*  à*@ÑA...©&.¬™.ï */
	$"0810 422A C621 0800 8048 2284 4310 0092"            /* ..B*∆!..ÄH"ÑC..í */
	$"6080 0000 0020 8044 0522 2002 BD59 1221"            /* `Ä... ÄD." .ΩY.! */
	$"0022 2A22 28C4 9108 2080 8463 08C2 118A"            /* ."*"(ƒë. ÄÑc.¬.ä */
	$"8CA1 18C6 1188 4915 6E24 8444 004E 308C"            /* å°.∆.àI.n$ÑD.N0å */
	$"1231 996B 18C6 3014 9AAA A8A1 5000 1000"            /* .1ôk.∆0.ö™®°P... */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF E406 0444"            /* ˇˇˇˇˇˇˇˇˇˇˇˇ‰..D */
	$"2229 0208 4042 0892 4908 2090 5141 0842"            /* ")..@B.íI. êQA.B */
	$"1220 0A22 2604 06A4 CFA1 0810 444A CA22"            /* . ¬"&..§œ°..DJ " */
	$"1000 8048 0544 4290 7CFE 6080 0000 0FE0"            /* ..ÄH.DBê|˛`Ä...‡ */
	$"8047 F503 C240 0926 8C00 1061 CFDC 2738"            /* ÄGı.¬@∆&å..aœ‹'8 */
	$"8E75 0004 747C FF7E 0E8F 747F 18BA 0E8F"            /* éu..t|ˇ~.èt..∫.è */
	$"88E2 4624 FE4C 7E37 CF7B D1F1 98EB 1779"            /* à‚F$˛L~7œ{—ÒòÎ.y */
	$"F1E3 6915 17FD 5000 1FFF FFFF FFFF FFFF"            /* Ò„i..˝P..ˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFF6 983D 1884 4FC9 1FFB 8082"            /* ˇˇˇˇˇˆò=.ÑO….˚ÄÇ */
	$"33E2 9110 4FA3 913A 1084 2243 F024 45E8"            /* 3‚ë.O£ë:.Ñ"C$EË */
	$"0482 30C0 E8FF D893 73EC E000 8078 0000"            /* .Ç0¿ËˇÿìsÏ‡.Äx.. */
	$"0000 0000 6080 0000 0020 8044 0000 0000"            /* ....`Ä... ÄD.... */
	$"0000 0000 1000 0000 0000 0002 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0100 0000 0000 0000"            /* ................ */
	$"0000 0000 0020 1000 0040 2000 0000 0080"            /* ..... ...@ ....Ä */
	$"E000 0000 FFFF FFFF FFFF FFFF FFFF FFFF"            /* ‡...ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFF0"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"0001 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 6080"            /* ..............`Ä */
	$"0000 0020 8044 0000 0000 0000 0000 2000"            /* ... ÄD........ . */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 01C0"            /* ...............¿ */
	$"6000 0040 2000 0000 0700 0000 0000 FFFF"            /* `..@ .........ˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFF0 0000 0000 0000"            /* ˇˇˇˇˇˇˇˇˇ...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0005 000A 000F 0014 0019 001E"            /* .......¬........ */
	$"0021 0026 002B 002B 002F 0032 0036 0036"            /* .!.&.+.+./.2.6.6 */
	$"003C 0042 0048 004E 0054 005A 005E 0061"            /* .<.B.H.N.T.Z.^.a */
	$"0067 006D 006E 0071 0074 0079 007E 0083"            /* .g.m.n.q.t.y.~.É */
	$"0084 0089 0089 008A 008D 0092 0097 009C"            /* .Ñ.â.â.ä.ç.í.ó.ú */
	$"00A1 00A2 00A5 00A8 00AD 00B2 00B4 00B9"            /* .°.¢.•.®.≠.≤.¥.π */
	$"00BA 00BE 00C3 00C5 00CA 00CF 00D4 00D9"            /* .∫.æ.√.≈. .œ.‘.Ÿ */
	$"00DE 00E3 00E8 00ED 00EE 00F0 00F3 00F8"            /* .ﬁ.„.Ë.Ì.Ó..Û.¯ */
	$"00FB 0100 0105 010A 010F 0114 0119 011E"            /* .˚.....¬........ */
	$"0123 0128 012D 0130 0135 013A 013F 0144"            /* .#.(.-.0.5.:.?.D */
	$"0149 014E 0153 0158 015D 0162 0167 016C"            /* .I.N.S.X.].b.g.l */
	$"0171 0176 017B 0180 0185 0187 018C 018E"            /* .q.v.{.Ä.Ö.á.å.é */
	$"0191 0197 0199 019E 01A3 01A8 01AD 01B2"            /* .ë.ó.ô.û.£.®.≠.≤ */
	$"01B6 01BB 01C0 01C1 01C4 01C9 01CA 01CF"            /* .∂.ª.¿.¡.ƒ.…. .œ */
	$"01D4 01D9 01DE 01E3 01E7 01EC 01F0 01F5"            /* .‘.Ÿ.ﬁ.„.Á.Ï..ı */
	$"01FA 01FF 0204 0209 020E 0211 0212 0215"            /* .˙.ˇ...∆........ */
	$"021B 0220 0224 0224 0230 023C 0248 0254"            /* ... .$.$.0.<.H.T */
	$"0260 026C 0278 0284 0290 029C 02A8 02B4"            /* .`.l.x.Ñ.ê.ú.®.¥ */
	$"02C0 02CC 02D8 02E4 02F0 02FC 0308 0314"            /* .¿.Ã.ÿ.‰..¸.... */
	$"0320 032C 0338 0344 0350 035C 0368 0374"            /* . .,.8.D.P.\.h.t */
	$"0380 038C 0392 0396 039A 039E 03A0 03A2"            /* .Ä.å.í.ñ.ö.û.†.¢ */
	$"03A7 03AC 03AF 03B4 03B9 03BE 03C3 03C8"            /* .ß.¨.Ø.¥.π.æ.√.» */
	$"03CD 03D2 03D7 03DC 03E1 03E6 03EB 03F0"            /* .Õ.“.◊.‹.·.Ê.Î. */
	$"03F5 03FA 03FF 0404 0409 040E 0413 0418"            /* .ı.˙.ˇ...∆...... */
	$"041D 0422 0427 042C 0431 0436 0439 043E"            /* ...".'.,.1.6.9.> */
	$"0443 0448 044D 0451 0456 045B 0460 0466"            /* .C.H.M.Q.V.[.`.f */
	$"046B 0470 0474 0479 047E 0483 0488 048D"            /* .k.p.t.y.~.É.à.ç */
	$"0492 0497 049B 04A1 04A6 04AB 04B0 04B5"            /* .í.ó.õ.°.¶.´.∞.µ */
	$"04B9 04BD 04BD 04BD 04BD 04BD 04BD 04BD"            /* .π.Ω.Ω.Ω.Ω.Ω.Ω.Ω */
	$"04BD 04BD 04BD 04BD 04BD 04BD 04BD 04BD"            /* .Ω.Ω.Ω.Ω.Ω.Ω.Ω.Ω */
	$"04BD 04BD 04BD 04BD 04BD 04BD 04BD 04BD"            /* .Ω.Ω.Ω.Ω.Ω.Ω.Ω.Ω */
	$"04BD 04BD 04BD 04BD 04BD 04BD 04BD 04C2"            /* .Ω.Ω.Ω.Ω.Ω.Ω.Ω.¬ */
	$"04C5 04C9 04CD 0006 0006 0006 0006 0006"            /* .≈.….Õ.......... */
	$"0006 0106 0006 0006 0006 0106 0006 0206"            /* ................ */
	$"0000 0006 0006 0006 0006 0006 0006 0206"            /* ................ */
	$"0006 0006 0006 0206 0106 0106 0006 0006"            /* ................ */
	$"0006 0206 0006 0006 0206 0106 0006 0006"            /* ................ */
	$"0006 0006 0206 0106 0206 0006 0006 0106"            /* ................ */
	$"0006 0206 0106 0006 0106 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0206 0106 0106"            /* ................ */
	$"0006 0106 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0106 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0206 0006"            /* ................ */
	$"0106 0106 0006 0206 0006 0006 0006 0006"            /* ................ */
	$"0006 0106 0006 0006 0206 0106 0006 0206"            /* ................ */
	$"0006 0006 0006 0006 0006 0106 0006 0106"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0106 0206"            /* ................ */
	$"0106 0006 0006 0106 0001 000C 000C 000C"            /* ................ */
	$"000C 000C 000C 000C 000C 000C 000C 000C"            /* ................ */
	$"000C 000C 000C 000C 000C 000C 000C 000C"            /* ................ */
	$"000C 000C 000C 000C 000C 000C 000C 000C"            /* ................ */
	$"000C 000C 000C 0006 0006 0106 0106 0106"            /* ................ */
	$"0006 0006 0006 0106 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0106"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0106 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0006 0006 0006 0006 0006 0006"            /* ................ */
	$"0006 0006 0206 0106 FFFF"                           /* ........ˇˇ */
};

data 'NFNT' (20490, "ASLFont+ 10") {
	$"F800 0000 00FF 000E 0000 FFFD 000B 000F"            /* ¯....ˇ....ˇ˝.... */
	$"05B7 000C 0003 0000 0050 0000 0000 0000"            /* .∑.......P...... */
	$"0010 0000 0000 2044 0080 0000 0000 0000"            /* ...... D.Ä...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0300 0000 0000 0000 0000"            /* ................ */
	$"0011 FC00 0000 2044 0080 0000 0000 0000"            /* ..¸... D.Ä...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 001F"            /* ................ */
	$"C000 0000 0000 0000 0000 0000 0000 0000"            /* ¿............... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0300 0000 0000 0000 0000"            /* ................ */
	$"0010 0000 0000 2044 0080 0000 0000 0000"            /* ...... D.Ä...... */
	$"C000 0000 0000 0000 0000 0000 0000 0000"            /* ¿............... */
	$"0000 0000 0000 0000 0000 0000 03C7 8000"            /* .............«Ä. */
	$"0000 0000 0000 0000 0000 0000 0000 0380"            /* ...............Ä */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ..ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFE 003C 0000 0000"            /* ˇˇˇˇˇˇˇˇˇ˛.<.... */
	$"0000 0000 0012 0010 8200 8009 0002 0000"            /* ........Ç.Ä∆.... */
	$"1000 4080 0040 0080 0008 0300 0004 0000"            /* ..@Ä.@.Ä........ */
	$"0000 0000 0258 0300 0000 0569 C641 029F"            /* .....X.....i∆A.ü */
	$"0010 0000 0000 2044 0080 0030 ED51 12C5"            /* ...... D.Ä.0ÌQ.≈ */
	$"2000 002E 4E70 BE6F B9C0 000E 01DE 7FBF"            /*  ...Npæoπ¿...ﬁ.ø */
	$"F747 8630 8C5D E779 FF8C 6318 FE44 9004"            /* ˜GÜ0å]Áyˇåc.˛Dê. */
	$"0800 2030 4261 0000 0000 0000 0000 0540"            /* .. 0Ba.........@ */
	$"1B00 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ..ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFE 1020 01F8 0000"            /* ˇˇˇˇˇˇˇˇˇ˛. .¯.. */
	$"0000 0000 7E12 3E10 823E 83E9 71F2 10BE"            /* ....~.>.Ç>ÉÈqÚ.æ */
	$"E52F 408F 7DF1 8487 E408 FCC8 1BF4 1CFB"            /* Â/@è}ÒÑá‰.¸».Ù.˚ */
	$"D152 0FFF E266 830F 8000 02A9 0842 9391"            /* —R.ˇ‚fÉ.Ä..©.Bìë */
	$"0010 03F8 0000 2044 0080 0049 1D53 EB25"            /* ...¯.. D.Ä.I.SÎ% */
	$"2200 0031 D189 A080 C620 4111 7A31 8461"            /* "..1—â†Ä∆ A.z1Ña */
	$"08C5 0650 DC63 18C6 048C 6318 8628 A802"            /* .≈.P‹c.∆.åc.Ü(®. */
	$"0800 2040 4261 0000 0000 1000 0000 0540"            /* .. @Ba.........@ */
	$"2D00 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* -.ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFE 2020 000F C480"            /* ˇˇˇˇˇˇˇˇˇ˛  ..ƒÄ */
	$"1100 02A0 022F C9FF FFA2 FC3F 8017 F0A2"            /* ...†./…ˇˇ¢¸?Ä.¢ */
	$"2520 47E0 0410 82F8 2A7F 0408 088F C408"            /* % G‡..Ç¯*....èƒ. */
	$"1152 0861 0266 8308 8000 2579 8841 1391"            /* .R.a.fÉ.Ä.%yàA.ë */
	$"0010 0000 0000 2044 0098 0041 0DFD 2D26"            /* ...... D.ò.A.˝-& */
	$"1A90 0053 410A A100 C634 8081 8631 8461"            /* .ê.SA¬°.∆4ÄÅÜ1Ña */
	$"0845 0690 AE63 18C6 048C 6318 8610 C400"            /* .E.êÆc.∆.åc.Ü.ƒ. */
	$"0800 2040 4021 0000 0000 1000 0000 0540"            /* .. @@!.........@ */
	$"0080 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* .Äˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFE D820 4008 47FF"            /* ˇˇˇˇˇˇˇˇˇ˛ÿ @.Gˇ */
	$"FFEE 7EA0 0268 4810 9222 9029 0212 28A2"            /* ˇÓ~†.hH.í"ê)..(¢ */
	$"2520 6080 0420 A280 2A08 0408 0882 4408"            /* % `Ä. ¢Ä*....ÇD. */
	$"1152 0861 065A 8308 8000 72A9 0640 7E91"            /* .R.a.ZÉ.Ä.r©.@~ë */
	$"0010 0000 0000 2044 00A4 1140 8855 1D42"            /* ...... D.§.@àU.B */
	$"1710 0053 410C BD01 4635 3E42 B631 8461"            /* ...SA.Ω.F5>B∂1Ña */
	$"0845 0710 AD63 18C6 048C 62A8 8A7C 8000"            /* .E..≠c.∆.åb®ä|Ä. */
	$"FF3D EEE7 FA65 F79D E7DB FA31 AC63 F540"            /* ˇ=ÓÁ˙e˜ùÁ€˙1¨cı@ */
	$"0080 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* .Äˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FC20 47F8 4A24"            /* ˇˇˇˇˇˇˇˇˇˇ¸ G¯J$ */
	$"1122 06BF 94A8 4830 9242 9029 7222 20DB"            /* .".øî®H0íBê)r" € */
	$"FD3F D080 2440 A180 3108 4B10 4BF2 44FF"            /* ˝?–Ä$@°Ä1.K.KÚDˇ */
	$"F152 0861 0402 8308 8000 FD69 735C 1291"            /* ÒR.a..É.Ä.˝is\.ë */
	$"FFFE 0007 F000 3FFF FFC3 FFF2 4853 8882"            /* ˇ˛...?ˇˇ√ˇÚHSàÇ */
	$"177C 0095 4234 83E2 39E2 0024 B7FE 847D"            /* .|.ïB4É‚9‚.$∑˛Ñ} */
	$"E9FD 0610 ACE3 E8F9 C48C 6A45 1210 8001"            /* È˝..¨„Ë˘ƒåjE..Ä. */
	$"18C2 3148 C669 AC63 18E4 1231 AAA2 2820"            /* .¬1H∆i¨c.‰.1™¢(  */
	$"0040 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* .@ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF F820 4609 9A24"            /* ˇˇˇˇˇˇˇˇˇˇ¯ F∆ö$ */
	$"3122 06A0 1120 4850 9F83 1029 0222 4084"            /* 1".†. HPüÉ.)."@Ñ */
	$"2022 4880 14E1 2180 312A 28D0 3082 8408"            /*  "HÄ.·!Ä1*(–0ÇÑ. */
	$"3156 1841 0401 1FE8 8000 728E 42F0 1091"            /* 1V.A...ËÄ.réB.ë */
	$"1810 0000 0000 2040 08A4 A240 48F9 5542"            /* ...... @.§¢@H˘UB */
	$"1A90 F899 440F C312 4421 3E44 BE31 8461"            /* .ê¯ôD.√.D!>Dæ1Ña */
	$"08C5 0710 8C63 08C4 248C 6AA2 227C 8001"            /* .≈..åc.ƒ$åj¢"|Ä. */
	$"18C2 3F48 C671 AC63 18C3 9231 A922 4540"            /* .¬?H∆q¨c.√í1©"E@ */
	$"0040 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* .@ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF F820 4009 2824"            /* ˇˇˇˇˇˇˇˇˇˇ¯ @∆($ */
	$"5142 7C20 1020 4890 9202 1021 0252 0084"            /* QB| . Hêí..!.R.Ñ */
	$"2022 4080 0951 2180 20A9 1012 1882 0408"            /*  "@Ä∆Q!Ä ©...Ç.. */
	$"2156 1841 0400 E008 8000 2544 6298 7C91"            /* !V.A..‡.Ä.%Dbò|ë */
	$"1810 0000 0000 2040 0898 AFC0 0051 5B2A"            /* ...... @.òØ¿.Q[* */
	$"1210 0119 4808 8312 4434 8080 8231 8461"            /* ....H.É.D4ÄÄÇ1Ña */
	$"08C5 4690 8C63 08C4 248C 6B12 4210 8001"            /* .≈Fêåc.ƒ$åk.B.Ä. */
	$"18C2 3048 C669 AC63 18C0 5231 A922 8540"            /* .¬0H∆i¨c.¿R1©"Ö@ */
	$"0020 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* . ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FD80 6011 0824"            /* ˇˇˇˇˇˇˇˇˇˇ˝Ä`..$ */
	$"9102 0420 1020 8911 1204 1021 0492 0108"            /* ë.. . â....!.í.. */
	$"2042 4080 1442 2180 40C9 1021 2482 0408"            /*  B@Ä.B!Ä@….!$Ç.. */
	$"4156 2842 0800 4008 8000 0284 4310 0091"            /* AV(B..@.Ä..ÑC..ë */
	$"1810 0000 0FE0 2040 0880 A444 4857 AB11"            /* .....‡ @.Ä§DHW´. */
	$"2001 0511 5088 A312 4454 4104 7A31 8461"            /*  ...Pà£.DTA.z1Ña */
	$"08C5 4650 8C63 08C4 248A B712 4210 8001"            /* .≈FPåc.ƒ$ä∑.B.Ä. */
	$"38C2 3048 C665 AC63 18C0 526A AAA3 0540"            /* 8¬0H∆e¨c.¿Rj™£.@ */
	$"0020 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* . ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFE FA40 5022 0844"            /* ˇˇˇˇˇˇˇˇˇ˛˙@P".D */
	$"1102 0440 2021 0811 1208 2022 490A 0210"            /* ...@ !.... "I¬.. */
	$"4084 4100 2244 2180 8088 0B21 4082 0408"            /* @ÑA."D!ÄÄà.!@Ç.. */
	$"8256 4844 9000 4008 8000 0544 4290 00FF"            /* ÇVHDê.@.Ä..DBê.ˇ */
	$"1810 0000 0000 2040 08FE A078 4801 24E9"            /* ...... @.˛†xH.$È */
	$"2001 060E FF70 9CE2 3984 0004 023E 7FBF"            /*  ...ˇpú‚9Ñ...>.ø */
	$"0747 BA3F 8C5D 0747 C471 2312 7E10 83F8"            /* .G∫?å].Gƒq#.~.É¯ */
	$"DF3D EF47 C663 AC5D E7C7 8DA4 545F F540"            /* ﬂ=ÔG∆c¨]Á«ç§T_ı@ */
	$"001F FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ..ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFE 5243 C844 089F"            /* ˇˇˇˇˇˇˇˇˇ˛RC»D.ü */
	$"111F FC80 4022 3E12 3210 43E4 7109 E420"            /* ..¸Ä@">.2.C‰q∆‰  */
	$"8108 421F C048 4179 0088 08FF 8072 3FF9"            /* Å.B.¿HAy.à.ˇÄr?˘ */
	$"049B 8FC8 E000 400F 8000 0000 0000 0000"            /* .õè»‡.@.Ä....... */
	$"1810 0000 0000 2040 0880 0000 0000 0000"            /* ...... @.Ä...... */
	$"C001 0200 0000 0000 0008 0000 0000 0000"            /* ¿............... */
	$"0000 0000 0000 0200 0000 0000 0383 8000"            /* .............ÉÄ. */
	$"0000 0000 8040 0001 0080 0000 0002 0380"            /* ....Ä@...Ä.....Ä */
	$"0010 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ..ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFE 0180 0800 0000"            /* ˇˇˇˇˇˇˇˇˇ˛.Ä.... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"1810 0000 001F E040 0880 0000 0000 0000"            /* ......‡@.Ä...... */
	$"0002 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0100 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 8040 0001 0080 0000 0002 0000"            /* ....Ä@...Ä...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"1810 0000 0000 2040 0880 0000 0000 0000"            /* ...... @.Ä...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0007 0180 0001 0080 0000 001C 0000"            /* .....Ä...Ä...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0005 000A"            /* ...............¬ */
	$"000F 0014 0019 001E 0021 0026 002B 002B"            /* .........!.&.+.+ */
	$"0030 0034 0038 0038 003F 0046 004D 0054"            /* .0.4.8.8.?.F.M.T */
	$"005B 0062 0066 006A 0071 0078 0079 007C"            /* .[.b.f.j.q.x.y.| */
	$"007F 0084 0089 008E 008F 0094 0094 0095"            /* ...Ñ.â.é.è.î.î.ï */
	$"0098 009D 00A2 00A7 00AD 00AE 00B1 00B4"            /* .ò.ù.¢.ß.≠.Æ.±.¥ */
	$"00B9 00BE 00C0 00C5 00C6 00CB 00D0 00D3"            /* .π.æ.¿.≈.∆.À.–.” */
	$"00D8 00DD 00E2 00E7 00EC 00F1 00F6 00FB"            /* .ÿ.›.‚.Á.Ï.Ò.ˆ.˚ */
	$"00FC 00FE 0102 0107 010B 0110 0116 011B"            /* .¸.˛............ */
	$"0120 0125 012A 012F 0134 0139 013E 0141"            /* . .%.*...4.9.>.A */
	$"0146 014B 0150 0155 015A 015F 0164 0169"            /* .F.K.P.U.Z._.d.i */
	$"016E 0173 0178 017D 0182 0187 018C 0191"            /* .n.s.x.}.Ç.á.å.ë */
	$"0196 0199 019E 01A1 01A6 01AD 01AF 01B4"            /* .ñ.ô.û.°.¶.≠.Ø.¥ */
	$"01B9 01BE 01C3 01C8 01CC 01D1 01D6 01D7"            /* .π.æ.√.».Ã.—.÷.◊ */
	$"01DA 01DF 01E0 01E5 01EA 01EF 01F4 01F9"            /* .⁄.ﬂ.‡.Â.Í.Ô.Ù.˘ */
	$"01FD 0202 0206 020B 0210 0215 021A 021F"            /* .˝.............. */
	$"0224 0227 0228 022B 0232 0237 023C 023C"            /* .$.'.(.+.2.7.<.< */
	$"0240 024B 0256 0261 026C 0277 0282 028D"            /* .@.K.V.a.l.w.Ç.ç */
	$"0298 02A3 02AE 02B9 02C4 02CF 02DA 02E5"            /* .ò.£.Æ.π.ƒ.œ.⁄.Â */
	$"02F0 02FB 0306 0311 031C 0327 0332 033D"            /* ..˚.......'.2.= */
	$"0348 0353 035E 0369 0374 037F 0386 038A"            /* .H.S.^.i.t...Ü.ä */
	$"038E 0392 0395 0397 039D 03A2 03A6 03AB"            /* .é.í.ï.ó.ù.¢.¶.´ */
	$"03B0 03B5 03BB 03C1 03C6 03CB 03D1 03D7"            /* .∞.µ.ª.¡.∆.À.—.◊ */
	$"03DC 03E2 03E7 03EE 03F4 03F9 03FF 0406"            /* .‹.‚.Á.Ó.Ù.˘.ˇ.. */
	$"040B 0411 0417 041D 0423 0429 042F 0435"            /* .........#.)./.5 */
	$"043B 0441 0445 044B 0451 0457 045C 0461"            /* .;.A.E.K.Q.W.\.a */
	$"0468 046D 0473 0479 0480 0486 048A 0490"            /* .h.m.s.y.Ä.Ü.ä.ê */
	$"0496 049C 04A2 04A8 04AD 04B3 04B8 04BE"            /* .ñ.ú.¢.®.≠.≥.∏.æ */
	$"04C4 04CA 04D0 04D6 04DA 04DE 04DE 04DE"            /* .ƒ. .–.÷.⁄.ﬁ.ﬁ.ﬁ */
	$"04DE 04DE 04DE 04DE 04DE 04DE 04DE 04DE"            /* .ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ */
	$"04DE 04DE 04DE 04DE 04DE 04DE 04DE 04DE"            /* .ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ */
	$"04DE 04DE 04DE 04DE 04DE 04DE 04DE 04DE"            /* .ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ.ﬁ */
	$"04DE 04DE 04DE 04E3 04E7 04EB 04F2 0107"            /* .ﬁ.ﬁ.ﬁ.„.Á.Î.Ú.. */
	$"0107 0107 0107 0107 0107 0207 0107 0107"            /* ................ */
	$"0007 0107 0007 0307 0000 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0307 0007 0007 0007 0307"            /* ................ */
	$"0207 0207 0107 0107 0107 0307 0107 0707"            /* ................ */
	$"0307 0207 0107 0107 0107 0007 0307 0207"            /* ................ */
	$"0207 0107 0107 0207 0107 0307 0107 0107"            /* ................ */
	$"0207 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0307 0207 0107 0107 0207 0107 0007"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0207 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0207 0107 0207 0107 0007 0307"            /* ................ */
	$"0107 0107 0107 0107 0107 0207 0107 0107"            /* ................ */
	$"0307 0107 0107 0307 0107 0107 0107 0107"            /* ................ */
	$"0107 0207 0107 0207 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0207 0307 0207 0007 0107 0107"            /* ................ */
	$"0001 0004 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 0007"            /* ................ */
	$"0007 0107 0207 0007 0107 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0107 0007 0307 0007"            /* ................ */
	$"FFFF"                                               /* ˇˇ */
};

data 'NFNT' (20492, "ASLFont+ 12") {
	$"F800 0000 00FF 000E 0000 FFFD 000C 000F"            /* ¯....ˇ....ˇ˝.... */
	$"05C6 000C 0003 0000 0051 0000 0000 0000"            /* .∆.......Q...... */
	$"0010 0000 0000 2044 0080 0000 0000 0000"            /* ...... D.Ä...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 1800 0000 0000"            /* ................ */
	$"0000 0011 FC00 0000 2044 0080 0000 0000"            /* ....¸... D.Ä.... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"001F C000 FFFF FFFF FFFF FFFF FFFF FFFF"            /* ..¿.ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFF0"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 1800 0000"            /* ................ */
	$"0000 0000 0010 0000 0000 2044 0080 0000"            /* .......... D.Ä.. */
	$"0000 0000 C000 0000 0000 0000 0000 0000"            /* ....¿........... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"03C7 8000 0000 0000 0000 0000 0000 0000"            /* .«Ä............. */
	$"0000 0380 0000 FFFF FFFF FFFF FFFF FFFF"            /* ...Ä..ˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFF0 01E0 0000 0000 0000 0000 0090 0084"            /* ˇ.‡.........ê.Ñ */
	$"1004 0048 0010 0000 8002 0400 0200 0400"            /* ...H....Ä....... */
	$"0040 1800 0020 0000 0000 0000 12C0 1800"            /* .@... .......¿.. */
	$"0569 C641 029F 0010 0000 0000 2044 0080"            /* .i∆A.ü...... D.Ä */
	$"0030 ED51 12C5 2000 002E 4E70 BE6F B9C0"            /* .0ÌQ.≈ ...Npæoπ¿ */
	$"000E 01DE 7FBF F747 8630 8C5D E779 FF8C"            /* ...ﬁ.ø˜GÜ0å]Áyˇå */
	$"6318 FE44 9004 0800 2030 4261 0000 0000"            /* c.˛Dê... 0Ba.... */
	$"0000 0000 0540 1B00 FFFF FFFF FFFF FFFF"            /* .....@..ˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFF0 8100 0FC0 0000 0000 0003 F091"            /* ˇˇˇÅ..¿......ë */
	$"F084 11F4 1F4B 8F90 85F7 297A 047B EF8C"            /* Ñ.Ù.KèêÖ˜)z.{Ôå */
	$"243F 2047 E640 DFA0 E7DE 8A90 7FFF 1334"            /* $? GÊ@ﬂ†Áﬁäê.ˇ.4 */
	$"187D 02A9 0842 9391 0010 03F8 0000 2044"            /* .}.©.Bìë...¯.. D */
	$"0080 0049 1D53 EB25 2200 0031 D189 A080"            /* .Ä.I.SÎ%"..1—â†Ä */
	$"C620 4111 7A31 8461 08C5 0650 DC63 18C6"            /* ∆ A.z1Ña.≈.P‹c.∆ */
	$"048C 6318 8628 A802 0800 2040 4261 0000"            /* .åc.Ü(®... @Ba.. */
	$"0000 1000 0000 0540 2D00 FFFF FFFF FFFF"            /* .......@-.ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFF1 0100 007E 2400 8800 1500"            /* ˇˇˇˇˇÒ...~$.à... */
	$"117E 4FFF FD17 E1FC 00BF 8511 2902 3F00"            /* .~Oˇ˝.·¸.øÖ.).?. */
	$"2084 17C1 53F8 2040 447E 2040 8A90 4308"            /*  Ñ.¡S¯ @D~ @äêC. */
	$"1334 1844 2579 8841 1391 0010 0000 0000"            /* .4.D%yàA.ë...... */
	$"2044 0098 0041 0DFD 2D26 1A90 0053 410A"            /*  D.ò.A.˝-&.ê.SA¬ */
	$"A100 C634 8081 8631 8461 0845 0690 AE63"            /* °.∆4ÄÅÜ1Ña.E.êÆc */
	$"18C6 048C 6318 8610 C400 0800 2040 4021"            /* .∆.åc.Ü.ƒ... @@! */
	$"0000 0000 1000 0000 0540 0080 FFFF FFFF"            /* .........@.Äˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFF6 C102 0042 3FFF FF73"            /* ˇˇˇˇˇˇˇˆ¡..B?ˇˇs */
	$"F500 1342 4084 9114 8148 1091 4511 2903"            /* ı..B@Ñë.ÅH.ëE.). */
	$"0400 2105 1401 5040 2040 4412 2040 8A90"            /* ..!...P@ @D. @äê */
	$"4308 32D4 1844 72A9 0640 7E91 0010 0000"            /* C.2‘.Dr©.@~ë.... */
	$"0000 2044 00A4 1140 8855 1D42 1710 0053"            /* .. D.§.@àU.B...S */
	$"410C BD01 4635 3E42 B631 8461 0845 0710"            /* A.Ω.F5>B∂1Ña.E.. */
	$"AD63 18C6 048C 62A8 8A7C 8000 FF3D EEE7"            /* ≠c.∆.åb®ä|Ä.ˇ=ÓÁ */
	$"FA65 F79D E7DB FA31 AC63 F540 0080 FFFF"            /* ˙e˜ùÁ€˙1¨cı@.Äˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF E102 3FC2 5120"            /* ˇˇˇˇˇˇˇˇˇˇ·.?¬Q  */
	$"8910 35FC A542 4184 9214 814B 9111 06DF"            /* â.5¸•BAÑí.ÅKë..ﬂ */
	$"E9FE 8401 2205 0C01 8842 5882 5F92 27FF"            /* È˛Ñ."...àBXÇ_í'ˇ */
	$"8A90 4308 2014 1844 FD69 735C 1291 FFFE"            /* äêC. ..D˝is\.ëˇ˛ */
	$"0007 F000 3FFF FFC3 FFF2 4853 8882 177C"            /* ...?ˇˇ√ˇÚHSàÇ.| */
	$"0095 4234 83E2 39E2 0024 B7FE 847D E9FD"            /* .ïB4É‚9‚.$∑˛Ñ}È˝ */
	$"0610 ACE3 E8F9 C48C 6A45 1210 8001 18C2"            /* ..¨„Ë˘ƒåjE..Ä..¬ */
	$"3148 C669 AC63 18E4 1231 AAA2 2820 0040"            /* 1H∆i¨c.‰.1™¢( .@ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF C102 304C"            /* ˇˇˇˇˇˇˇˇˇˇˇˇ¡.0L */
	$"D121 8910 3500 8902 4284 FC18 8148 1112"            /* —!â.5.â.BÑ¸.ÅH.. */
	$"0421 0112 4400 A709 0C01 8951 4681 8414"            /* .!..D.ß∆..âQFÅÑ. */
	$"2041 8AB0 C208 2008 FF44 728E 42F0 1091"            /*  Aä∞¬. .ˇDréB.ë */
	$"1810 0000 0000 2040 08A4 A240 48F9 5542"            /* ...... @.§¢@H˘UB */
	$"1A90 F899 440F C312 4421 3E44 BE31 8461"            /* .ê¯ôD.√.D!>Dæ1Ña */
	$"08C5 0710 8C63 08C4 248C 6AA2 227C 8001"            /* .≈..åc.ƒ$åj¢"|Ä. */
	$"18C2 3F48 C671 AC63 18C3 9231 A922 4540"            /* .¬?H∆q¨c.√í1©"E@ */
	$"0040 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* .@ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF C102"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇ¡. */
	$"0049 4122 8A13 E100 8102 4484 9010 8108"            /* .IA"ä.·.Å.DÑê.Å. */
	$"1290 0421 0112 0400 4A89 0C01 0548 8090"            /* .ê.!....Jâ...HÄê */
	$"C410 2041 0AB0 C208 2007 0044 2544 6298"            /* ƒ. A¬∞¬. ..D%Dbò */
	$"7C91 1810 0000 0000 2040 0898 AFC0 0051"            /* |ë...... @.òØ¿.Q */
	$"5B2A 1210 0119 4808 8312 4434 8080 8231"            /* [*....H.É.D4ÄÄÇ1 */
	$"8461 08C5 4690 8C63 08C4 248C 6B12 4210"            /* Ña.≈Fêåc.ƒ$åk.B. */
	$"8001 18C2 3048 C669 AC63 18C0 5231 A922"            /* Ä..¬0H∆i¨c.¿R1©" */
	$"8540 0020 FFFF FFFF FFFF FFFF FFFF FFFF"            /* Ö@. ˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"EC03 0088 4124 8810 2100 8104 4888 9020"            /* Ï..àA$à.!.Å.Hàê  */
	$"8108 2490 0841 0212 0400 A211 0C02 0648"            /* Å.$ê.A....¢....H */
	$"8109 2410 2042 0AB1 4210 4002 0044 0284"            /* Å∆$. B¬±B.@..D.Ñ */
	$"4310 0091 1810 0000 0FE0 2040 0880 A444"            /* C..ë.....‡ @.Ä§D */
	$"4857 AB11 2001 0511 5088 A312 4454 4104"            /* HW´. ...Pà£.DTA. */
	$"7A31 8461 08C5 4650 8C63 08C4 248A B712"            /* z1Ña.≈FPåc.ƒ$ä∑. */
	$"4210 8001 38C2 3048 C665 AC63 18C0 526A"            /* B.Ä.8¬0H∆e¨c.¿Rj */
	$"AAA3 0540 0020 FFFF FFFF FFFF FFFF FFFF"            /* ™£.@. ˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFF7 D202 8110 4220 8810 2201 0108 4088"            /* ˇ˜“.Å.B à."...@à */
	$"9041 0112 4850 1082 0422 0801 1221 0C04"            /* êA..HP.Ç."...!.. */
	$"0440 590A 0410 2044 12B2 4224 8002 0044"            /* .@Y¬.. D.≤B$Ä..D */
	$"0544 4290 00FF 1810 0000 0000 2040 08FE"            /* .DBê.ˇ...... @.˛ */
	$"A078 4801 24E9 2001 060E FF70 9CE2 3984"            /* †xH.$È ...ˇpú‚9Ñ */
	$"0004 023E 7FBF 0747 BA3F 8C5D 0747 C471"            /* ...>.ø.G∫?å].Gƒq */
	$"2312 7E10 83F8 DF3D EF47 C663 AC5D E7C7"            /* #.~.É¯ﬂ=ÔG∆c¨]Á« */
	$"8DA4 545F F540 001F FFFF FFFF FFFF FFFF"            /* ç§T_ı@..ˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFF2 921E 4220 44F8 88FF E402 0111"            /* ˇˇˇÚí.B D¯àˇ‰... */
	$"F091 9082 1F23 884F 2104 0842 10FE 0242"            /* ëêÇ.#àO!..B.˛.B */
	$"0BC8 0440 47FC 0391 FFC8 24DC 7E47 0002"            /* .».@G¸.ëˇ»$‹~G.. */
	$"007D 0000 0000 0000 1810 0000 0000 2040"            /* .}............ @ */
	$"0880 0000 0000 0000 C001 0200 0000 0000"            /* .Ä......¿....... */
	$"0008 0000 0000 0000 0000 0000 0000 0200"            /* ................ */
	$"0000 0000 0383 8000 0000 0000 8040 0001"            /* .....ÉÄ.....Ä@.. */
	$"0080 0000 0002 0380 0010 FFFF FFFF FFFF"            /* .Ä.....Ä..ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFF0 0C00 4000 0000 0000 0000"            /* ˇˇˇˇˇ..@....... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 1810 0000 001F"            /* ................ */
	$"E040 0880 0000 0000 0000 0002 0000 0000"            /* ‡@.Ä............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0100 0000 0000 0000 0000 0000 0000 8040"            /* ..............Ä@ */
	$"0001 0080 0000 0002 0000 0000 0000 0000"            /* ...Ä............ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 1810 0000"            /* ................ */
	$"0000 2040 0880 0000 0000 0000 0000 0000"            /* .. @.Ä.......... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0007"            /* ................ */
	$"0180 0001 0080 0000 001C 0000 0000 0000"            /* .Ä...Ä.......... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0005 000A 000F"            /* .............¬.. */
	$"0014 0019 001E 0021 0026 002B 002B 0030"            /* .......!.&.+.+.0 */
	$"0034 0038 0038 003F 0046 004D 0054 005B"            /* .4.8.8.?.F.M.T.[ */
	$"0062 0066 006A 0071 0078 0079 007C 007F"            /* .b.f.j.q.x.y.|.. */
	$"0084 0089 008E 008F 0094 0094 0095 0098"            /* .Ñ.â.é.è.î.î.ï.ò */
	$"009D 00A2 00A7 00AD 00AE 00B1 00B4 00B9"            /* .ù.¢.ß.≠.Æ.±.¥.π */
	$"00BE 00C0 00C5 00C6 00CB 00D0 00D3 00D8"            /* .æ.¿.≈.∆.À.–.”.ÿ */
	$"00DD 00E2 00E7 00EC 00F1 00F6 00FB 00FC"            /* .›.‚.Á.Ï.Ò.ˆ.˚.¸ */
	$"00FE 0102 0107 010B 0110 0116 011B 0120"            /* .˛.............  */
	$"0125 012A 012F 0134 0139 013E 0141 0146"            /* .%.*...4.9.>.A.F */
	$"014B 0150 0155 015A 015F 0164 0169 016E"            /* .K.P.U.Z._.d.i.n */
	$"0173 0178 017D 0182 0187 018C 0191 0196"            /* .s.x.}.Ç.á.å.ë.ñ */
	$"0199 019E 01A1 01A6 01AD 01AF 01B4 01B9"            /* .ô.û.°.¶.≠.Ø.¥.π */
	$"01BE 01C3 01C8 01CC 01D1 01D6 01D7 01DA"            /* .æ.√.».Ã.—.÷.◊.⁄ */
	$"01DF 01E0 01E5 01EA 01EF 01F4 01F9 01FD"            /* .ﬂ.‡.Â.Í.Ô.Ù.˘.˝ */
	$"0202 0206 020B 0210 0215 021A 021F 0224"            /* ...............$ */
	$"0227 0228 022B 0232 0237 023C 023C 0240"            /* .'.(.+.2.7.<.<.@ */
	$"024C 0258 0264 0270 027C 0288 0294 02A0"            /* .L.X.d.p.|.à.î.† */
	$"02AC 02B8 02C4 02D0 02DC 02E8 02F4 0300"            /* .¨.∏.ƒ.–.‹.Ë.Ù.. */
	$"030C 0318 0324 0330 033C 0348 0354 0360"            /* .....$.0.<.H.T.` */
	$"036C 0378 0384 0390 039C 03A3 03A7 03AB"            /* .l.x.Ñ.ê.ú.£.ß.´ */
	$"03AF 03B2 03B4 03BA 03BF 03C3 03C8 03CD"            /* .Ø.≤.¥.∫.ø.√.».Õ */
	$"03D2 03D8 03DE 03E3 03E8 03EE 03F4 03F9"            /* .“.ÿ.ﬁ.„.Ë.Ó.Ù.˘ */
	$"03FF 0404 040B 0411 0416 041C 0423 0428"            /* .ˇ...........#.( */
	$"042E 0434 043A 0440 0446 044C 0452 0458"            /* ...4.:.@.F.L.R.X */
	$"045E 0462 0468 046E 0474 0479 047E 0485"            /* .^.b.h.n.t.y.~.Ö */
	$"048A 0490 0496 049D 04A3 04A7 04AD 04B3"            /* .ä.ê.ñ.ù.£.ß.≠.≥ */
	$"04B9 04BF 04C5 04CA 04D0 04D5 04DB 04E1"            /* .π.ø.≈. .–.’.€.· */
	$"04E7 04ED 04F3 04F7 04FB 04FB 04FB 04FB"            /* .Á.Ì.Û.˜.˚.˚.˚.˚ */
	$"04FB 04FB 04FB 04FB 04FB 04FB 04FB 04FB"            /* .˚.˚.˚.˚.˚.˚.˚.˚ */
	$"04FB 04FB 04FB 04FB 04FB 04FB 04FB 04FB"            /* .˚.˚.˚.˚.˚.˚.˚.˚ */
	$"04FB 04FB 04FB 04FB 04FB 04FB 04FB 04FB"            /* .˚.˚.˚.˚.˚.˚.˚.˚ */
	$"04FB 04FB 0500 0504 0508 050F 0107 0107"            /* .˚.˚............ */
	$"0107 0107 0107 0107 0207 0107 0107 0007"            /* ................ */
	$"0107 0007 0307 0000 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0307 0007 0007 0007 0307 0207"            /* ................ */
	$"0207 0107 0107 0107 0307 0107 0707 0307"            /* ................ */
	$"0207 0107 0107 0107 0007 0307 0207 0207"            /* ................ */
	$"0107 0107 0207 0107 0307 0107 0107 0207"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0307 0207 0107 0107 0207 0107 0007 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0207"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0107 0107 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0207 0107 0207 0107 0007 0307 0107"            /* ................ */
	$"0107 0107 0107 0107 0207 0107 0107 0307"            /* ................ */
	$"0107 0107 0307 0107 0107 0107 0107 0107"            /* ................ */
	$"0207 0107 0207 0107 0107 0107 0107 0107"            /* ................ */
	$"0107 0207 0307 0207 0007 0107 0107 0001"            /* ................ */
	$"0004 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 010E 010E"            /* ................ */
	$"010E 010E 010E 010E 010E 010E 0007 0007"            /* ................ */
	$"0107 0207 0007 0107 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0007 0007 0007 0007 0007"            /* ................ */
	$"0007 0007 0007 0107 0007 0307 0007 FFFF"            /* ..............ˇˇ */
};

resource 'PICT' (128, purgeable) {
	131,
	{160, 242, 178, 262},
	$"1101 A000 8201 000A 0000 0000 02D0 0240"
	$"9000 0400 A000 F000 B201 0800 A000 F200"
	$"B201 0600 A000 F200 B201 0600 003F FFF8"
	$"0020 0008 0020 000C 0020 000C 0020 000C"
	$"0020 000C 0023 FF8C 0021 FF0C 0020 FE0C"
	$"0020 7C0C 0020 380C 0020 100C 0020 000C"
	$"0020 000C 0020 000C 0020 000C 003F FFFC"
	$"000F FFFC 00A0 0083 FF"
};

resource 'PICT' (256) {
	2532,
	{129, 153, 141, 230},
	$"0011 02FF 0C00 FFFF FFFF 0099 0000 0081"
	$"0000 00E6 0000 008D 0000 0000 0000 001F"
	$"7FFF 7FFF 7FFF 0001 000A 0081 0099 008D"
	$"00E6 0099 804E 0081 0099 008D 00E6 0000"
	$"0000 0000 0000 0048 0000 0048 0000 0000"
	$"0008 0001 0008 0000 0000 00EB 80C4 0000"
	$"0000 0000 5C53 0000 00FF 0000 FFFF FFFF"
	$"FFFF 0001 FFFF FFFF CCCC 0002 FFFF FFFF"
	$"9999 0003 FFFF FFFF 6666 0004 FFFF FFFF"
	$"3333 0005 FFFF FFFF 0000 0006 FFFF CCCC"
	$"FFFF 0007 FFFF CCCC CCCC 0008 FFFF CCCC"
	$"9999 0009 FFFF CCCC 6666 000A FFFF CCCC"
	$"3333 000B FFFF CCCC 0000 000C FFFF 9999"
	$"FFFF 000D FFFF 9999 CCCC 000E FFFF 9999"
	$"9999 000F FFFF 9999 6666 0010 FFFF 9999"
	$"3333 0011 FFFF 9999 0000 0012 FFFF 6666"
	$"FFFF 0013 FFFF 6666 CCCC 0014 FFFF 6666"
	$"9999 0015 FFFF 6666 6666 0016 FFFF 6666"
	$"3333 0017 FFFF 6666 0000 0018 FFFF 3333"
	$"FFFF 0019 FFFF 3333 CCCC 001A FFFF 3333"
	$"9999 001B FFFF 3333 6666 001C FFFF 3333"
	$"3333 001D FFFF 3333 0000 001E FFFF 0000"
	$"FFFF 001F FFFF 0000 CCCC 0020 FFFF 0000"
	$"9999 0021 FFFF 0000 6666 0022 FFFF 0000"
	$"3333 0023 FFFF 0000 0000 0024 CCCC FFFF"
	$"FFFF 0025 CCCC FFFF CCCC 0026 CCCC FFFF"
	$"9999 0027 CCCC FFFF 6666 0028 CCCC FFFF"
	$"3333 0029 CCCC FFFF 0000 002A CCCC CCCC"
	$"FFFF 002B CCCC CCCC CCCC 002C CCCC CCCC"
	$"9999 002D CCCC CCCC 6666 002E CCCC CCCC"
	$"3333 002F CCCC CCCC 0000 0030 CCCC 9999"
	$"FFFF 0031 CCCC 9999 CCCC 0032 CCCC 9999"
	$"9999 0033 CCCC 9999 6666 0034 CCCC 9999"
	$"3333 0035 CCCC 9999 0000 0036 CCCC 6666"
	$"FFFF 0037 CCCC 6666 CCCC 0038 CCCC 6666"
	$"9999 0039 CCCC 6666 6666 003A CCCC 6666"
	$"3333 003B CCCC 6666 0000 003C CCCC 3333"
	$"FFFF 003D CCCC 3333 CCCC 003E CCCC 3333"
	$"9999 003F CCCC 3333 6666 0040 CCCC 3333"
	$"3333 0041 CCCC 3333 0000 0042 CCCC 0000"
	$"FFFF 0043 CCCC 0000 CCCC 0044 CCCC 0000"
	$"9999 0045 CCCC 0000 6666 0046 CCCC 0000"
	$"3333 0047 CCCC 0000 0000 0048 9999 FFFF"
	$"FFFF 0049 9999 FFFF CCCC 004A 9999 FFFF"
	$"9999 004B 9999 FFFF 6666 004C 9999 FFFF"
	$"3333 004D 9999 FFFF 0000 004E 9999 CCCC"
	$"FFFF 004F 9999 CCCC CCCC 0050 9999 CCCC"
	$"9999 0051 9999 CCCC 6666 0052 9999 CCCC"
	$"3333 0053 9999 CCCC 0000 0054 9999 9999"
	$"FFFF 0055 9999 9999 CCCC 0056 9999 9999"
	$"9999 0057 9999 9999 6666 0058 9999 9999"
	$"3333 0059 9999 9999 0000 005A 9999 6666"
	$"FFFF 005B 9999 6666 CCCC 005C 9999 6666"
	$"9999 005D 9999 6666 6666 005E 9999 6666"
	$"3333 005F 9999 6666 0000 0060 9999 3333"
	$"FFFF 0061 9999 3333 CCCC 0062 9999 3333"
	$"9999 0063 9999 3333 6666 0064 9999 3333"
	$"3333 0065 9999 3333 0000 0066 9999 0000"
	$"FFFF 0067 9999 0000 CCCC 0068 9999 0000"
	$"9999 0069 9999 0000 6666 006A 9999 0000"
	$"3333 006B 9999 0000 0000 006C 6666 FFFF"
	$"FFFF 006D 6666 FFFF CCCC 006E 6666 FFFF"
	$"9999 006F 6666 FFFF 6666 0070 6666 FFFF"
	$"3333 0071 6666 FFFF 0000 0072 6666 CCCC"
	$"FFFF 0073 6666 CCCC CCCC 0074 6666 CCCC"
	$"9999 0075 6666 CCCC 6666 0076 6666 CCCC"
	$"3333 0077 6666 CCCC 0000 0078 6666 9999"
	$"FFFF 0079 6666 9999 CCCC 007A 6666 9999"
	$"9999 007B 6666 9999 6666 007C 6666 9999"
	$"3333 007D 6666 9999 0000 007E 6666 6666"
	$"FFFF 007F 6666 6666 CCCC 0080 6666 6666"
	$"9999 0081 6666 6666 6666 0082 6666 6666"
	$"3333 0083 6666 6666 0000 0084 6666 3333"
	$"FFFF 0085 6666 3333 CCCC 0086 6666 3333"
	$"9999 0087 6666 3333 6666 0088 6666 3333"
	$"3333 0089 6666 3333 0000 008A 6666 0000"
	$"FFFF 008B 6666 0000 CCCC 008C 6666 0000"
	$"9999 008D 6666 0000 6666 008E 6666 0000"
	$"3333 008F 6666 0000 0000 0090 3333 FFFF"
	$"FFFF 0091 3333 FFFF CCCC 0092 3333 FFFF"
	$"9999 0093 3333 FFFF 6666 0094 3333 FFFF"
	$"3333 0095 3333 FFFF 0000 0096 3333 CCCC"
	$"FFFF 0097 3333 CCCC CCCC 0098 3333 CCCC"
	$"9999 0099 3333 CCCC 6666 009A 3333 CCCC"
	$"3333 009B 3333 CCCC 0000 009C 3333 9999"
	$"FFFF 009D 3333 9999 CCCC 009E 3333 9999"
	$"9999 009F 3333 9999 6666 00A0 3333 9999"
	$"3333 00A1 3333 9999 0000 00A2 3333 6666"
	$"FFFF 00A3 3333 6666 CCCC 00A4 3333 6666"
	$"9999 00A5 3333 6666 6666 00A6 3333 6666"
	$"3333 00A7 3333 6666 0000 00A8 3333 3333"
	$"FFFF 00A9 3333 3333 CCCC 00AA 3333 3333"
	$"9999 00AB 3333 3333 6666 00AC 3333 3333"
	$"3333 00AD 3333 3333 0000 00AE 3333 0000"
	$"FFFF 00AF 3333 0000 CCCC 00B0 3333 0000"
	$"9999 00B1 3333 0000 6666 00B2 3333 0000"
	$"3333 00B3 3333 0000 0000 00B4 0000 FFFF"
	$"FFFF 00B5 0000 FFFF CCCC 00B6 0000 FFFF"
	$"9999 00B7 0000 FFFF 6666 00B8 0000 FFFF"
	$"3333 00B9 0000 FFFF 0000 00BA 0000 CCCC"
	$"FFFF 00BB 0000 CCCC CCCC 00BC 0000 CCCC"
	$"9999 00BD 0000 CCCC 6666 00BE 0000 CCCC"
	$"3333 00BF 0000 CCCC 0000 00C0 0000 9999"
	$"FFFF 00C1 0000 9999 CCCC 00C2 0000 9999"
	$"9999 00C3 0000 9999 6666 00C4 0000 9999"
	$"3333 00C5 0000 9999 0000 00C6 0000 6666"
	$"FFFF 00C7 0000 6666 CCCC 00C8 0000 6666"
	$"9999 00C9 0000 6666 6666 00CA 0000 6666"
	$"0000 00CB 0000 6666 3333 00CC 0000 3333"
	$"FFFF 00CD 0000 3333 CCCC 00CE 0000 3333"
	$"9999 00CF 0000 3333 6666 00D0 0000 3333"
	$"3333 00D1 0000 3333 0000 00D2 0000 0000"
	$"FFFF 00D3 0000 0000 CCCC 00D4 0000 0000"
	$"9999 00D5 0000 0000 6666 00D6 0000 0000"
	$"3333 00D7 EEEE 0000 0000 00D8 DDDD 0000"
	$"0000 00D9 BBBB 0000 0000 00DA AAAA 0000"
	$"0000 00DB 8888 0000 0000 00DC 7777 0000"
	$"0000 00DD 5555 0000 0000 00DE 4444 0000"
	$"0000 00DF 2222 0000 0000 00E0 1111 0000"
	$"0000 00E1 0000 EEEE 0000 00E2 0000 DDDD"
	$"0000 00E3 0000 BBBB 0000 00E4 0000 AAAA"
	$"0000 00E5 0000 8888 0000 00E6 0000 7777"
	$"0000 00E7 0000 5555 0000 00E8 0000 4444"
	$"0000 00E9 0000 2222 0000 00EA 0000 1111"
	$"0000 00EB 0000 0000 EEEE 00EC 0000 0000"
	$"DDDD 00ED 0000 0000 BBBB 00EE 0000 0000"
	$"AAAA 00EF 0000 0000 8888 00F0 0000 0000"
	$"7777 00F1 0000 0000 5555 00F2 0000 0000"
	$"4444 00F3 0000 0000 2222 00F4 0000 0000"
	$"1111 00F5 EEEE EEEE EEEE 00F6 DDDD DDDD"
	$"DDDD 00F7 BBBB BBBB BBBB 00F8 AAAA AAAA"
	$"AAAA 00F9 8888 8888 8888 00FA 7777 7777"
	$"7777 00FB 5555 5555 5555 00FC 4444 4444"
	$"4444 00FD 2222 2222 2222 00FE 1111 1111"
	$"1111 00FF 0000 0000 0000 0081 0099 008D"
	$"00E6 0081 0099 008D 00E6 0000 000A 0081"
	$"0099 008D 00E6 02B3 0002 B300 02B3 000A"
	$"FE00 FCFF F200 FEFF CD00 10FE 0005 FFFF"
	$"0000 FFFF F400 01FF FFCB 003B FE00 0CFF"
	$"FF00 00FF FF00 FFFF 00FF 0000 FEFF 0100"
	$"00FD FF01 0000 FEFF 0700 00FF FF00 FF00"
	$"00FE FF01 0000 FDFF FE00 FEFF 0100 00FE"
	$"FFFE 00FE FFF3 003B FE00 FCFF 0100 00FD"
	$"FF11 00FF FF00 FFFF 0000 FFFF 0000 FFFF"
	$"00FF FF00 FDFF 0E00 FFFF 00FF FF00 FFFF"
	$"00FF FF00 FFFF FE00 07FF FF00 FFFF 00FF"
	$"FFF1 0033 FE00 01FF FFFC 0001 FFFF FE00"
	$"FCFF 0500 00FF FF00 00FC FF02 00FF FFFE"
	$"00FC FF08 00FF FF00 FFFF 00FF FFFE 00FC"
	$"FF00 00FD FFF3 0039 FE00 01FF FFFC 0001"
	$"FFFF FE00 01FF FFFC 0005 FFFF 0000 FFFF"
	$"FD00 01FF FFFE 0001 FFFF FD00 07FF FF00"
	$"FFFF 00FF FFFE 0001 FFFF FB00 01FF FFF3"
	$"0040 FE00 01FF FFFC 0001 FFFF FD00 FDFF"
	$"0300 00FF FFFE 00FD FF02 00FF FFFD 00FD"
	$"FF07 00FF FF00 FFFF 0000 FEFF 0100 00FD"
	$"FF00 00FE FF09 0000 FFFF 00FF FF00 FFFF"
	$"FC00 02B3 0002 B300 00FF"
};

resource 'PICT' (1000, "Keypad", purgeable) {
	4571,
	{33, 12, 196, 249},
	$"1101 0100 0A00 0000 0001 E002 8098 0020"
	$"0021 0008 00C4 0100 0021 000C 00C4 00F9"
	$"0021 000C 00C4 00F9 0000 02E1 0002 E100"
	$"02E1 0021 0000 FEFF 00C0 FEFF 00C0 FEFF"
	$"13C0 003F FFFF F03F FFFF F03F FFFF F03F"
	$"FFFF F000 0021 0001 FE00 0021 FE00 0021"
	$"FE00 1320 2040 0000 0840 0000 0840 0000"
	$"0840 0000 0800 0021 0001 FE00 0031 FE00"
	$"0031 FE00 1330 0040 0000 0C40 0000 0C40"
	$"0000 0C40 0000 0C00 0021 1F01 3BB3 0031"
	$"3B1B B3B1 33A9 2BB0 204C E400 0C4C EC00"
	$"0C4C EC00 0C4C E800 0C00 0021 1F01 212A"
	$"8031 12A2 2931 2A3A AA30 004A 8C00 0C4A"
	$"8200 0C4A 8200 0C4A 8A00 0C00 0021 1F01"
	$"312A 8031 1293 3131 332A AB30 204C C400"
	$"0C4C C400 0C4C C400 0C4C CE00 0C00 0021"
	$"1F01 212A 8031 128A 2931 2A2A AA30 0048"
	$"8400 0C48 8800 0C48 8200 0C48 8200 0C00"
	$"0021 1F01 23AB 0031 3AB3 A931 2BA9 13B0"
	$"2048 8400 0C48 8E00 0C48 8C00 0C48 8200"
	$"0C00 0021 0001 FE00 0031 FE00 0031 FE00"
	$"1330 0040 0000 0C40 0000 0C40 0000 0C40"
	$"0000 0C00 0021 0001 FE00 0031 FE00 0031"
	$"FE00 1330 2040 0000 0C40 0000 0C40 0000"
	$"0C40 0000 0C00 0021 0001 FE00 0431 2BB3"
	$"8031 FE00 1330 0040 0000 0C40 0000 0C40"
	$"0000 0C40 0000 0C00 0021 0001 FE00 0431"
	$"2A2A 0031 FE00 1330 2040 0000 0C40 0000"
	$"0C40 0000 0C40 0000 0C00 0021 0001 FE00"
	$"0431 3B33 0031 FE00 1330 0040 0000 0C40"
	$"0000 0C40 0000 0C40 0000 0C00 0021 0001"
	$"FE00 0431 2A2A 0031 FE00 1330 2040 0000"
	$"0C40 0000 0C40 0000 0C40 0000 0C00 0021"
	$"0001 FE00 0431 2BAB 8031 FE00 1330 0040"
	$"0000 0C40 0000 0C40 0000 0C40 0000 0C00"
	$"0021 0001 FE00 0031 FE00 0031 FE00 1330"
	$"2040 0000 0C40 0000 0C40 0000 0C40 0000"
	$"0C00 0021 0001 FE00 0031 FE00 0031 FE00"
	$"1330 0040 0000 0C40 0000 0C40 0000 0C40"
	$"0000 0C00 0021 0001 FE00 0031 FE00 0031"
	$"FE00 1330 2040 0000 0C40 0000 0C40 0000"
	$"0C40 0000 0C00 0021 0001 FE00 0031 FE00"
	$"0031 FE00 1330 0040 0000 0C40 0000 0C40"
	$"0000 0C40 0000 0C00 0021 0001 FE00 0031"
	$"FE00 0031 FE00 1330 2040 0000 0C40 0000"
	$"0C40 0000 0C40 0000 0C00 0021 0001 FE00"
	$"0031 FE00 0031 FE00 1330 0040 0000 0C40"
	$"0000 0C40 0000 0C40 0000 0C00 0021 0001"
	$"FE00 0031 FE00 0031 FE00 1330 2040 0000"
	$"0C40 0000 0C40 0000 0C40 0000 0C00 0021"
	$"0001 FE00 0031 FE00 0031 FE00 1330 0040"
	$"0000 0C40 0000 0C40 0000 0C40 0000 0C00"
	$"0021 0001 FE00 0031 FE00 0031 FE00 1330"
	$"2040 0000 0C40 0000 0C40 0000 0C40 0000"
	$"0C00 0021 0001 FE00 0031 FE00 0031 FE00"
	$"1330 0040 0000 0C40 0000 0C40 0000 0C40"
	$"0000 0C00 0021 0001 FE00 0031 FE00 0031"
	$"FE00 1330 2040 0000 0C40 0000 0C40 0000"
	$"0C40 0000 0C00 0021 0001 FE00 0031 FE00"
	$"0031 FE00 1330 0040 0000 0C40 0000 0C40"
	$"0000 0C40 0000 0C00 0021 0000 FEFF 00F0"
	$"FEFF 00F0 FEFF 13F0 203F FFFF FC3F FFFF"
	$"FC3F FFFF FC3F FFFF FC00 0021 1F00 7FFF"
	$"FFE0 7FFF FFE0 7FFF FFE0 001F FFFF F81F"
	$"FFFF F81F FFFF F81F FFFF F800 0006 F400"
	$"0020 EF00 02E1 0006 F400 0020 EF00 2100"
	$"00FE FF00 C0FE FF00 C0FE FF13 C000 3FFF"
	$"FFF0 3FFF FFF0 3FFF FFF0 3FFF FFF0 0000"
	$"2100 01FE 0000 21FE 0000 21FE 0013 2020"
	$"4000 0008 4000 0008 4000 0008 4000 0008"
	$"0000 2100 01FE 0000 31FE 0000 31FE 0013"
	$"3000 4000 000C 4000 000C 4000 000C 4000"
	$"000C 0000 211F 011B A39B B133 3A80 3133"
	$"AB80 3020 4000 000C 4000 000C 4000 000C"
	$"4000 000C 0000 211F 0122 2221 312A A280"
	$"312A 2900 3000 47E0 000C 43C0 000C 43C0"
	$"000C 4000 000C 0000 211F 0113 2321 3133"
	$"3280 312B 1100 3020 4060 000C 4660 000C"
	$"4660 000C 47C0 000C 0000 211F 010A 2221"
	$"3122 A280 312A 2900 3000 4060 000C 4660"
	$"000C 4660 000C 4000 000C 0000 211F 0133"
	$"BB99 3122 B900 312B A900 3020 4060 000C"
	$"4660 000C 4660 000C 4000 000C 0000 2100"
	$"01FE 0000 31FE 0000 31FE 0013 3000 40C0"
	$"000C 43C0 000C 4660 000C 4000 000C 0000"
	$"2100 01FE 0000 31FE 0000 31FE 0013 3020"
	$"4180 000C 4660 000C 43E0 000C 4000 000C"
	$"0000 2100 01FE 001B 3119 B3BB 3119 B3BB"
	$"3000 4180 000C 4660 000C 4060 000C 4000"
	$"000C 0000 2100 01FE 001B 3122 2A22 B122"
	$"2A22 B020 4180 000C 4660 000C 40C0 000C"
	$"4000 000C 0000 2100 01FE 001B 3112 3332"
	$"B112 3332 B000 4180 000C 43C0 000C 4380"
	$"000C 4000 000C 0000 2100 01FE 001B 310A"
	$"2A22 B10A 2A22 B020 4000 000C 4000 000C"
	$"4000 000C 4000 000C 0000 2100 01FE 001B"
	$"3131 ABBA B131 ABBA B000 4000 000C 4000"
	$"000C 4000 000C 4000 000C 0000 2100 01FE"
	$"0000 31FE 0000 31FE 0013 3020 4000 000C"
	$"4000 000C 4000 000C 4000 000C 0000 2100"
	$"01FE 0000 31FE 0000 31FE 0013 3000 4000"
	$"000C 4000 000C 4000 000C 4000 000C 0000"
	$"2100 01FE 0000 31FE 0000 31FE 0013 3020"
	$"4000 000C 4000 000C 4000 000C 4000 000C"
	$"0000 2100 01FE 0000 31FE 0000 31FE 0013"
	$"3000 4000 000C 4000 000C 4000 000C 4000"
	$"000C 0000 2100 01FE 0000 31FE 0000 31FE"
	$"0013 3020 4000 000C 4000 000C 4000 000C"
	$"4000 000C 0000 2100 01FE 0000 31FE 0000"
	$"31FE 0013 3000 4000 000C 4000 000C 4000"
	$"000C 4000 000C 0000 2100 01FE 0000 31FE"
	$"0000 31FE 0013 3020 4000 000C 4000 000C"
	$"4000 000C 4000 000C 0000 2100 01FE 0000"
	$"31FE 0000 31FE 0013 3000 4000 000C 4000"
	$"000C 4000 000C 4000 000C 0000 2100 01FE"
	$"0000 31FE 0000 31FE 0013 3020 4000 000C"
	$"4000 000C 4000 000C 4000 000C 0000 2100"
	$"01FE 0000 31FE 0000 31FE 0013 3000 4000"
	$"000C 4000 000C 4000 000C 4000 000C 0000"
	$"2100 01FE 0000 31FE 0000 31FE 0013 3020"
	$"4000 000C 4000 000C 4000 000C 4000 000C"
	$"0000 2100 01FE 0000 31FE 0000 31FE 0013"
	$"3000 4000 000C 4000 000C 4000 000C 4000"
	$"000C 0000 2100 00FE FF00 F0FE FF00 F0FE"
	$"FF13 F020 3FFF FFFC 3FFF FFFC 3FFF FFF8"
	$"3FFF FFFC 0000 211F 007F FFFF E07F FFFF"
	$"E07F FFFF E000 1FFF FFF8 1FFF FFF8 1FFF"
	$"FFF8 1FFF FFF8 0000 06F4 0000 20EF 0002"
	$"E100 06F4 0000 20EF 001B FC00 FEFF 00C0"
	$"FC00 113F FFFF F03F FFFF F03F FFFF F03F"
	$"FFFF F000 001E FD00 0001 FE00 0020 FD00"
	$"1220 4000 0008 4000 0008 4000 0008 4000"
	$"0008 0000 1DFD 0000 01FE 0000 30FC 0011"
	$"4000 000C 4000 000C 4000 000C 4000 000C"
	$"0000 1EFD 0000 01FE 0000 30FD 0012 2040"
	$"0000 0C40 0000 0C40 0000 0C40 0000 0C00"
	$"001D FD00 0001 FE00 0030 FC00 1140 6000"
	$"0C47 E000 0C41 C000 0C40 0000 0C00 001E"
	$"FD00 0001 FE00 0030 FD00 1220 40E0 000C"
	$"4600 000C 4300 000C 4300 000C 0000 1DFD"
	$"0000 01FE 0000 30FC 0011 4160 000C 4600"
	$"000C 4600 000C 4300 000C 0000 1EFD 0004"
	$"0100 0C00 30FD 0012 2042 6000 0C47 C000"
	$"0C47 C000 0C41 0000 0C00 001D FD00 0401"
	$"001E 0030 FC00 1144 6000 0C40 6000 0C46"
	$"6000 0C42 0000 0C00 001E FD00 0401 003F"
	$"0030 FD00 1220 47F0 000C 4060 000C 4660"
	$"000C 4000 000C 0000 1DFD 0004 0100 7F80"
	$"30FC 0011 4060 000C 4060 000C 4660 000C"
	$"4000 000C 0000 1EFD 0004 0100 FFC0 30FD"
	$"0012 2040 6000 0C44 6000 0C46 6000 0C40"
	$"0000 0C00 001D FD00 0401 01FF E030 FC00"
	$"1140 6000 0C43 C000 0C43 C000 0C40 0000"
	$"0C00 001E FD00 0401 01FF E030 FD00 1220"
	$"4000 000C 4000 000C 4000 000C 4000 000C"
	$"0000 1DFD 0004 0100 3F00 30FC 0011 4000"
	$"000C 4000 000C 4000 000C 4000 000C 0000"
	$"1EFD 0004 0100 3F00 30FD 0012 2040 0000"
	$"0C40 0000 0C40 0000 0C40 0000 0C00 001D"
	$"FD00 0401 003F 0030 FC00 1140 0000 0C40"
	$"0000 0C40 0000 0C40 0000 0C00 001E FD00"
	$"0401 003F 0030 FD00 1220 4000 000C 4000"
	$"000C 4000 000C 4000 000C 0000 1DFD 0004"
	$"0100 3F00 30FC 0011 4000 000C 4000 000C"
	$"4000 000C 4000 000C 0000 1EFD 0000 01FE"
	$"0000 30FD 0012 2040 0000 0C40 0000 0C40"
	$"0000 0C40 0000 0C00 001D FD00 0001 FE00"
	$"0030 FC00 1140 0000 0C40 0000 0C40 0000"
	$"0C40 0000 0C00 001E FD00 0001 FE00 0030"
	$"FD00 1220 4000 000C 4000 000C 4000 000C"
	$"4000 000C 0000 1DFD 0000 01FE 0000 30FC"
	$"0011 4000 000C 4000 000C 4000 000C 4000"
	$"000C 0000 1EFD 0000 01FE 0000 30FD 0012"
	$"2040 0000 0C40 0000 0C40 0000 0C40 0000"
	$"0C00 001D FD00 0001 FE00 0030 FC00 1140"
	$"0000 0C40 0000 0C40 0000 0C40 0000 0C00"
	$"001E FD00 0001 FE00 0030 FD00 1220 4000"
	$"000C 4000 000C 4000 000C 4000 000C 0000"
	$"1DFD 0000 01FE 0000 30FC 0011 4000 000C"
	$"4000 000C 4000 000C 4000 000C 0000 1CFC"
	$"00FE FF00 F0FD 0012 203F FFFF FC3F FFFF"
	$"FC3F FFFF FC3F FFFF FC00 001C FC00 037F"
	$"FFFF E0FC 0011 1FFF FFF8 1FFF FFF8 1FFF"
	$"FFF8 1FFF FFF8 0000 06F4 0000 20EF 0002"
	$"E100 06F4 0000 20EF 0021 0000 FEFF 00C0"
	$"FEFF 00C0 FEFF 13C0 003F FFFF F03F FFFF"
	$"F03F FFFF F03F FFFF F000 0021 0001 FE00"
	$"0021 FE00 0021 FE00 1320 2040 0000 0840"
	$"0000 0840 0000 0840 0000 0800 0021 0001"
	$"FE00 0031 FE00 0031 FE00 1330 0040 0000"
	$"0C40 0000 0C40 0000 0C40 0000 0C00 0021"
	$"0001 FE00 0031 FE00 0031 FE00 1330 2040"
	$"0000 0C40 0000 0C40 0000 0C4E CEEC 0C00"
	$"0021 0001 FE00 0031 FE00 0031 FE00 1330"
	$"0041 8000 0C43 C000 0C47 E000 0C48 A48A"
	$"0C00 0021 0001 FE00 0031 FE00 0031 FE00"
	$"1330 2043 8000 0C44 6000 0C40 C000 0C4C"
	$"A4CC 0C00 0021 0001 FE00 0031 FE00 0031"
	$"FE00 1330 0041 8000 0C40 6000 0C41 8000"
	$"0C48 A48A 0C00 0021 0001 FE00 0031 FE00"
	$"0031 FE00 1330 2041 8000 0C40 6000 0C43"
	$"C000 0C4E A4EA 0C00 0021 1F01 0030 0031"
	$"003F 0031 000C 0030 0041 8000 0C40 C000"
	$"0C40 6000 0C40 0000 0C00 0021 1F01 0070"
	$"0031 003F 0031 000E 0030 2041 8000 0C41"
	$"8000 0C40 6000 0C40 0000 0C00 0021 1F01"
	$"00F0 0031 003F 0031 000F 0030 0041 8000"
	$"0C43 0000 0C40 6000 0C40 0000 0C00 0021"
	$"1F01 01FF 8031 003F 0031 01FF 8030 2041"
	$"8000 0C46 0000 0C44 6000 0C40 0000 0C00"
	$"0021 1F01 03FF 8031 003F 0031 01FF C030"
	$"0041 8000 0C47 E000 0C43 C000 0C40 0000"
	$"0C00 0021 1F01 07FF 8031 01FF E031 01FF"
	$"E030 2040 0000 0C40 0000 0C40 0000 0C40"
	$"0000 0C00 0021 1F01 07FF 8031 01FF E031"
	$"01FF E030 0040 0000 0C40 0000 0C40 0000"
	$"0C40 0000 0C00 0021 1F01 03FF 8031 00FF"
	$"C031 01FF C030 2040 0000 0C40 0000 0C40"
	$"0000 0C40 0000 0C00 0021 1F01 01FF 8031"
	$"007F 8031 01FF 8030 0040 0000 0C40 0000"
	$"0C40 0000 0C40 0000 0C00 0021 1F01 00F0"
	$"0031 003F 0031 000F 0030 2040 0000 0C40"
	$"0000 0C40 0000 0C40 0000 0C00 0021 1F01"
	$"0070 0031 001E 0031 000E 0030 0040 0000"
	$"0C40 0000 0C40 0000 0C40 0000 0C00 0021"
	$"1F01 0030 0031 000C 0031 000C 0030 2040"
	$"0000 0C40 0000 0C40 0000 0C40 0000 0C00"
	$"0021 0001 FE00 0031 FE00 0031 FE00 1330"
	$"0040 0000 0C40 0000 0C40 0000 0C40 0000"
	$"0C00 0021 0001 FE00 0031 FE00 0031 FE00"
	$"1330 2040 0000 0C40 0000 0C40 0000 0C40"
	$"0000 0C00 0021 0001 FE00 0031 FE00 0031"
	$"FE00 1330 0040 0000 0C40 0000 0C40 0000"
	$"0C40 0000 0C00 0021 0001 FE00 0031 FE00"
	$"0031 FE00 1330 2040 0000 0C40 0000 0C40"
	$"0000 0C40 0000 0C00 0021 0001 FE00 0031"
	$"FE00 0031 FE00 1330 0040 0000 0C40 0000"
	$"0C40 0000 0C40 0000 0C00 0021 0001 FE00"
	$"0031 FE00 0031 FE00 1330 2040 0000 0C40"
	$"0000 0C40 0000 0C40 0000 0C00 0021 0001"
	$"FE00 0031 FE00 0031 FE00 1330 0040 0000"
	$"0C40 0000 0C40 0000 0C40 0000 0C00 0021"
	$"0000 FEFF 00F0 FEFF 00F0 FEFF 13F0 203F"
	$"FFFF FC3F FFFF FC3F FFFF FC40 0000 0C00"
	$"0021 1F00 7FFF FFE0 7FFF FFE0 7FFF FFE0"
	$"001F FFFF F81F FFFF F81F FFFF F840 0000"
	$"0C00 000D F400 0020 F500 0540 0000 0C00"
	$"0009 E700 0540 0000 0C00 000D F400 0020"
	$"F500 0540 0000 0C00 0012 F300 003F FBFF"
	$"0AF0 3FFF FFF0 4000 000C 0000 13F4 0001"
	$"2040 FB00 0A08 4000 0008 4000 000C 0000"
	$"12F3 0000 40FB 000A 0C40 0000 0C40 0000"
	$"0C00 0013 F400 0120 40FB 000A 0C40 0000"
	$"0C40 0000 0C00 0013 F300 0143 C0FC 000A"
	$"0C40 0000 0C40 0000 0C00 0014 F400 0220"
	$"4660 FC00 0A0C 4300 000C 4000 000C 0000"
	$"13F3 0001 4660 FC00 0A0C 4300 000C 4000"
	$"000C 0000 14F4 0002 2046 60FC 000A 0C40"
	$"0000 0C40 0000 0C00 0013 F300 0146 60FC"
	$"000A 0C40 0000 0C40 0000 0C00 0014 F400"
	$"0220 4660 FC00 0A0C 4000 000C 4000 000C"
	$"0000 13F3 0001 4660 FC00 0A0C 4000 000C"
	$"4000 000C 0000 14F4 0002 2046 60FC 000A"
	$"0C40 0000 0C40 0000 0C00 0013 F300 0143"
	$"C0FC 000A 0C40 0000 0C40 0000 0C00 0013"
	$"F400 0120 40FB 000A 0C40 0000 0C40 0000"
	$"0C00 0012 F300 0040 FB00 0A0C 4000 000C"
	$"4000 000C 0000 13F4 0001 2040 FB00 0A0C"
	$"4000 000C 4000 000C 0000 12F3 0000 40FB"
	$"000A 0C40 0000 0C40 0000 0C00 0013 F400"
	$"0120 40FB 000A 0C40 0000 0C40 0000 0C00"
	$"0012 F300 0040 FB00 0A0C 4000 000C 4000"
	$"000C 0000 13F4 0001 2040 FB00 0A0C 4000"
	$"000C 4000 000C 0000 12F3 0000 40FB 000A"
	$"0C40 0000 0C40 0000 0C00 0013 F400 0120"
	$"40FB 000A 0C40 0000 0C40 0000 0C00 0012"
	$"F300 0040 FB00 0A0C 4000 000C 4000 000C"
	$"0000 13F4 0001 2040 FB00 0A0C 4000 000C"
	$"4000 000C 0000 12F3 0000 40FB 000A 0C40"
	$"0000 0C40 0000 0C00 0013 F400 0120 40FB"
	$"000A 0C40 0000 0C40 0000 0C00 0012 F300"
	$"0040 FB00 0A0C 4000 000C 4000 000C 0000"
	$"13F4 0001 203F FBFF 0AFC 3FFF FFFC 3FFF"
	$"FFFC 0000 12F3 0000 1FFB FF0A F81F FFFF"
	$"F81F FFFF F800 0002 E100 02E1 0002 E100"
	$"FF"
};

resource 'PICT' (1002, "Function", purgeable) {
	2292,
	{0, 0, 99, 163},
	$"0011 02FF 0C00 FFFF FFFF 0000 0000 0000"
	$"0000 00A3 0000 0063 0000 0000 0000 001E"
	$"0001 000A 0000 0000 0063 00A3 0099 8016"
	$"0000 0000 0063 00A3 0000 0000 0000 0000"
	$"0048 0000 0048 0000 0000 0001 0001 0001"
	$"0000 0000 0805 B544 0000 0000 0000 C58D"
	$"8000 0001 0000 FFFF FFFF FFFF 0000 0000"
	$"0000 0000 0000 0000 0063 00A3 0000 0000"
	$"0063 00A3 0000 000A 0000 0000 0063 00A3"
	$"02EB 0002 EB00 05ED 0001 013F 1715 0FFF"
	$"FFFC 0FFF FFFC 0FFF FFFC 0FFF FFFC 0FFF"
	$"FFFC 005B 1715 1000 0002 1000 0002 1000"
	$"0002 1000 0002 1000 0002 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 00A1 1715 1390 0003 13B0 0003 13B0"
	$"0003 13A0 0003 13B8 0003 0000 1715 1230"
	$"0003 1208 0003 1208 0003 1228 0003 1220"
	$"0003 0064 1715 1310 0003 1310 0003 1310"
	$"0003 1338 0003 1338 0003 00CC 1715 1210"
	$"0003 1220 0003 1208 0003 1208 0003 1208"
	$"0003 00AD 1715 1210 0003 1238 0003 1230"
	$"0003 1208 0003 1238 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 0F6E 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 083A 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 1AB9 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 1FFF 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 0000 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 0000 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 0000 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 0000 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 0000 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 0000 1715 1000 0003 1000 0003 1000"
	$"0003 1000 0003 1000 0003 0000 1715 1000"
	$"0003 1000 0003 1000 0003 1000 0003 1000"
	$"0003 0000 1700 0FFE FF00 0FFE FF00 0FFE"
	$"FF00 0FFE FF00 0FFE FF01 0000 1715 07FF"
	$"FFFE 07FF FFFE 07FF FFFE 07FF FFFE 07FF"
	$"FFFE 0000 02EB 0002 EB00 02EB 0017 150F"
	$"FFFF FC0F FFFF FC0F FFFF FC0F FFFF FC0F"
	$"FFFF FC00 0017 1510 0000 0210 0000 0210"
	$"0000 0210 0000 0210 0000 0200 0017 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0300 0017 1513 9800 0313 B800 0313"
	$"8800 0313 8800 0313 9100 0300 0017 1512"
	$"2000 0312 0800 0312 1400 0312 1400 0312"
	$"3280 0300 0017 1513 3000 0313 1000 0313"
	$"0800 0313 0C00 0313 1280 0300 0017 1512"
	$"2800 0312 1000 0312 1400 0312 0400 0312"
	$"1280 0300 3017 1512 1000 0312 1000 0312"
	$"0800 0312 0800 0312 1100 0300 0117 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0300 0C17 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 0300 0017 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 031F FF17 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 0300 0017 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0300 0017 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 0310 0017 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0300 0017 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 0308 1617 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0300 0017 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 0300 0017 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0300 0017 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 0300 0017 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0302 7017 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 030B 0017 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0300 0017 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 0300 FF17 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0300 0E17 1510 0000 0310 0000 0310"
	$"0000 0310 0000 0310 0000 0300 0017 1510"
	$"0000 0310 0000 0310 0000 0310 0000 0310"
	$"0000 0302 7017 000F FEFF 000F FEFF 000F"
	$"FEFF 000F FEFF 000F FEFF 010C CC17 1507"
	$"FFFF FE07 FFFF FE07 FFFF FE07 FFFF FE07"
	$"FFFF FE00 0002 EB00 02EB 0002 EB00 1715"
	$"0FFF FFFC 0FFF FFFC 0FFF FFFC 0FFF FFFC"
	$"0FFF FFFC 0000 1715 1000 0002 1000 0002"
	$"1000 0002 1000 0002 1000 0002 0000 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 1014 1715 1391 0003 1393 0003"
	$"1393 0003 1392 0003 1393 8003 0000 1715"
	$"1233 0003 1230 8003 1230 8003 1232 8003"
	$"1232 0003 0790 1715 1311 0003 1311 0003"
	$"1311 0003 1313 8003 1313 8003 1000 1715"
	$"1211 0003 1212 0003 1210 8003 1210 8003"
	$"1210 8003 1FFF 1715 1211 0003 1213 8003"
	$"1213 0003 1210 8003 1213 8003 000A 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 0000 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 0816 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 0125 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 1FFF 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 01B9 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 1FFF 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 01B9 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 0163 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 016A 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 0260 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 0192 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 0000 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 0750 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 0807 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 1DA8 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 0816 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 0CB0 1715 1000 0003 1000 0003"
	$"1000 0003 1000 0003 1000 0003 0000 1715"
	$"1000 0003 1000 0003 1000 0003 1000 0003"
	$"1000 0003 0000 1700 0FFE FF00 0FFE FF00"
	$"0FFE FF00 0FFE FF00 0FFE FF01 0000 1715"
	$"07FF FFFE 07FF FFFE 07FF FFFE 07FF FFFE"
	$"07FF FFFE 0100 05ED 0001 0807 05ED 0001"
	$"1DA8 05ED 0001 0816 00FF"
};

resource 'PICT' (1022, "NCSA logo", purgeable) {
	5136,
	{0, 0, 54, 144},
	$"0011 02FF 0C00 FFFF FFFF 0000 0000 0000"
	$"0000 0090 0000 0036 0000 0000 0000 001E"
	$"0001 000A 0000 0000 0036 0090 0098 8090"
	$"0000 0000 0036 0090 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0008 0001 0008"
	$"0000 0000 0014 BA5C 0000 0000 0000 0B68"
	$"0000 0040 0000 FFFF FFFF FFFF 0001 FC00"
	$"FC00 FC00 0002 F800 F800 F800 0003 F400"
	$"F400 F400 0004 F000 F000 F000 0005 EC00"
	$"EC00 EC00 0006 E800 E800 E800 0007 E400"
	$"E400 E400 0008 E000 E000 E000 0009 DC00"
	$"DC00 DC00 000A D800 D800 D800 000B D400"
	$"D400 D400 000C D000 D000 D000 000D CC00"
	$"CC00 CC00 000E C800 C800 C800 000F C400"
	$"C400 C400 0010 C000 C000 C000 0011 BC00"
	$"BC00 BC00 0012 B800 B800 B800 0013 B400"
	$"B400 B400 0014 B000 B000 B000 0015 AC00"
	$"AC00 AC00 0016 A800 A800 A800 0017 A400"
	$"A400 A400 0018 A000 A000 A000 0019 9C00"
	$"9C00 9C00 001A 9800 9800 9800 001B 9400"
	$"9400 9400 001C 9000 9000 9000 001D 8C00"
	$"8C00 8C00 001E 8800 8800 8800 001F 8400"
	$"8400 8400 0020 8000 8000 8000 0021 7C00"
	$"7C00 7C00 0022 7800 7800 7800 0023 7400"
	$"7400 7400 0024 7000 7000 7000 0025 6C00"
	$"6C00 6C00 0026 6800 6800 6800 0027 6400"
	$"6400 6400 0028 6000 6000 6000 0029 5C00"
	$"5C00 5C00 002A 5800 5800 5800 002B 5400"
	$"5400 5400 002C 5000 5000 5000 002D 4C00"
	$"4C00 4C00 002E 4800 4800 4800 002F 4400"
	$"4400 4400 0030 4000 4000 4000 0031 3C00"
	$"3C00 3C00 0032 3800 3800 3800 0033 3400"
	$"3400 3400 0034 3000 3000 3000 0035 2C00"
	$"2C00 2C00 0036 2800 2800 2800 0037 2400"
	$"2400 2400 0038 2000 2000 2000 0039 1C00"
	$"1C00 1C00 003A 1800 1800 1800 003B 1400"
	$"1400 1400 003C 1000 1000 1000 003D 0C00"
	$"0C00 0C00 003E 0800 0800 0800 003F 0400"
	$"0400 0400 0040 0000 0000 0000 0000 0000"
	$"0036 0090 0000 0000 0036 0090 0000 0481"
	$"00F1 000C FB00 0001 A000 0201 0303 DC00"
	$"2EFC 0009 0204 0403 0000 0101 0303 F000"
	$"0201 0302 F200 0101 01F9 0000 01D7 0000"
	$"01FE 0000 02F6 0000 01FE 0200 01ED 0020"
	$"FB00 0001 FB00 0001 ED00 0001 F500 0201"
	$"0201 F500 0102 01D9 0003 0108 0901 DD00"
	$"4F0D 0000 0100 060B 0B09 0800 0101 0001"
	$"F000 0402 0909 0C02 F800 1401 0200 0002"
	$"0608 0C11 1617 1612 0D09 0802 0000 0201"
	$"F300 0001 FC00 0001 F402 0A03 0404 0B22"
	$"3A3A 1E02 0101 F900 040B 0909 0A06 ED00"
	$"680D 0001 0104 243B 3C3E 3713 0204 0001"
	$"F100 0703 1F3A 3E3A 1403 02FB 0006 0101"
	$"0009 0E1F 37FE 3D01 3E3E FE3D 073E 3A26"
	$"1205 0001 01F6 0012 0100 0001 070A 0F17"
	$"1F26 292B 2D2B 292B 2C2C 2DFE 2F03 3133"
	$"343B FE40 0339 1003 03FC 000A 0200 1338"
	$"3E3B 3C2C 0803 01F0 0052 0400 0201 052D"
	$"FD40 032B 0B04 04F2 0009 0100 0429 4040"
	$"3F14 0402 FC00 0601 0006 1835 3E3F F540"
	$"053D 2D12 0301 02F9 0009 0102 0003 0E1D"
	$"343C 3C3E EC40 043F 1705 0201 FD00 0A01"
	$"062C 4040 3F40 3C11 0503 F000 6A0D 0001"
	$"0104 293F 403F 403E 2306 0501 F200 0701"
	$"0427 3F40 3D11 02FC 000A 0300 0C2A 3C40"
	$"403F 4040 3FFD 3D0B 3C3F 403F 3E40 403A"
	$"2206 0202 FB00 0602 0201 1028 3A3D FC40"
	$"FB3F 033E 3E3F 3FFE 3E03 3D3D 3C3F FE40"
	$"043E 1503 0201 FE00 0B01 000E 3D40 3D3F"
	$"403F 1F05 02F0 0069 FD00 0923 3E40 403D"
	$"4037 0D02 04F0 0004 213E 403C 0EFC 001C"
	$"0301 0A2A 4040 3C3E 403C 3823 0F06 0404"
	$"0A21 373E 403D 3D40 3C1B 0203 01FD 0011"
	$"0102 041A 363F 4040 3E3C 3B32 251C 1917"
	$"1615 FD13 0112 11FE 1208 0E0C 1D38 3F40"
	$"340E 02FA 0008 1C3D 3F40 403F 3F30 08EF"
	$"005C FD00 0324 3E40 3FFE 3E03 1F04 0301"
	$"F100 0422 3F40 3D0F FD00 0B02 0105 213F"
	$"3E3C 403D 2E13 05FA 000A 0617 3640 3E3C"
	$"4037 1101 03FE 000B 0102 061A 3840 3E3B"
	$"3926 1206 F000 0501 071D 2928 13F9 000B"
	$"0532 403F 3D34 3D40 3B0C 0002 F100 5CFE"
	$"000B 0127 403F 3F40 3D40 3811 0104 F100"
	$"0422 3F40 3D10 FE00 0A01 0202 1F3C 3F3C"
	$"4037 1705 F600 140A 273F 3E3D 402A 0602"
	$"0100 0102 051D 3840 392D 1B0B F800 F801"
	$"0603 1A35 3A34 1A02 FA00 0D01 000B 3B40"
	$"3D26 153A 403C 1800 03F1 0065 FD00 0B26"
	$"3F3F 403A 403D 4028 0502 02F2 0004 1F3F"
	$"403D 11FE 0008 0400 1239 403B 4030 10F6"
	$"0021 0100 0007 2A40 3C40 3B0F 0001 0003"
	$"0113 3540 351F 0B00 0005 0B14 212B 333A"
	$"3B3B 3C3B FA3C 013A 3CFE 4002 3309 02F9"
	$"000C 1E3C 403E 1406 3A40 3F30 0501 01F2"
	$"005F FD00 0B24 3E40 3A1E 393F 3F3B 1500"
	$"03F2 0012 1F3F 403D 1000 0002 0007 2D40"
	$"3C40 3410 0003 01F6 0017 0300 0C37 403E"
	$"3E1B 0100 0101 092E 4035 1702 0412 2437"
	$"3B3C F040 063F 3D40 4013 0301 FD00 0F01"
	$"0005 3540 403A 0B00 283D 403B 0E00 03F2"
	$"0067 FD00 0C23 3D40 3907 2340 3D40 310A"
	$"0002 F300 111D 3F40 3D11 0000 0101 1F3D"
	$"3E3F 3B16 0003 01F3 0001 011E FE3E 0D2E"
	$"0501 0500 133A 3A18 0413 2B39 3CFE 40FE"
	$"3EFD 3D02 3C3D 3DFC 3C01 3B3F FE40 0336"
	$"0E04 02FD 0010 0100 0D3C 403E 2A05 000F"
	$"3C40 3E25 0302 01F3 006A FE00 0D01 253E"
	$"403C 0409 323F 3E3E 1C01 03F3 0004 1D3F"
	$"403D 12FE 0008 0A38 403D 4023 0402 01F3"
	$"001A 0100 0A3D 4040 3C09 0003 042A 3E22"
	$"061D 3A3C 3F40 3B38 2817 0E0A 07FE 050E"
	$"0304 0503 0205 0502 0620 363D 361A 01FA"
	$"000F 0123 3D40 3F16 0000 043B 4040 3A0C"
	$"0001 F300 58FD 000D 233F 403C 0A00 183F"
	$"3D40 360A 0104 F400 041E 3F40 3E13 FE00"
	$"071A 3D3E 403C 1100 02F0 0013 0635 3D3C"
	$"370E 0100 043B 3C0E 0F35 403C 3A2D 1608"
	$"F100 0404 080A 0807 F800 0505 373F 403A"
	$"0CFE 0006 2B40 403E 1D00 01F3 005F FD00"
	$"0E20 3F40 3A0A 0006 3540 3D3E 1C01 0301"
	$"F500 0D1B 3E40 3D13 0000 0531 403E 402D"
	$"08ED 0003 0809 0907 FE00 090B 3C27 0926"
	$"3D39 3522 0BFD 00FB 01FE 0208 0304 020C"
	$"2B3B 3723 02F9 0007 0100 0D3B 403E 2504"
	$"FE00 0714 3E40 3F33 0600 01F4 0061 FD00"
	$"0E1F 3F40 3A06 0000 1B3D 3E40 380F 0004"
	$"F500 0E1B 3E40 3D13 0200 073B 4040 3E18"
	$"0101 EC00 0001 FD00 0F1E 3C11 1337 3A26"
	$"1100 0009 111F 2F39 3BFC 3C05 3D3C 3C3D"
	$"3B3B FE40 033C 1306 02FA 0005 0122 3C40"
	$"3C12 FD00 0708 3B40 403C 0C00 02F4 0058"
	$"FD00 0C1F 3F40 3B04 0000 0429 403D 4023"
	$"FE02 F600 0C1D 3F40 3D13 0000 153D 3F40"
	$"3A0B EB00 0102 02FD 000C 323A 0D2C 3F25"
	$"0B03 1022 373C 3FF2 4006 3E40 4017 0603"
	$"01FD 0007 0100 0839 403E 2D06 FD00 0602"
	$"2A40 3F3E 2001 F300 64FD 0004 223F 403C"
	$"05FE 0008 123A 3F3E 3A14 0003 01F7 000C"
	$"1F3F 403E 1500 002A 3F40 402A 03E6 0016"
	$"063B 2E0C 3B36 0D0B 2137 3E40 3D3D 3E3D"
	$"3D3C 3C3D 3D3C 3BFE 3C01 3A3A FE40 0438"
	$"0E02 0101 FD00 0802 0013 3C40 3D19 0001"
	$"FD00 0714 3E40 4038 0800 01F5 005D FD00"
	$"0422 3F40 3D09 FE00 0801 2240 3D40 3410"
	$"0103 F700 0B1F 3F40 3D14 0001 3740 403E"
	$"18E5 0020 083A 2418 3D1C 0C2C 3E40 3A2F"
	$"1B0F 0801 0203 0200 0204 0100 0102 010B"
	$"283B 371D 02F9 0005 0227 3D40 3B0D FB00"
	$"0708 3A40 3F3D 1200 02F5 0057 FD00 1122"
	$"3F40 3C0A 0000 0100 0C38 403C 4028 0502"
	$"02F8 000B 1E3F 403D 1300 013A 4040 3E10"
	$"E500 1307 2C1D 2B36 0D22 4039 2910 0601"
	$"0000 0308 0E0D 05F9 0002 0106 06F7 0005"
	$"0939 403E 2805 FB00 0702 2640 3F3E 2605"
	$"01F5 0051 FD00 0421 3F40 3C0A FC00 071A"
	$"3E3E 3F3B 1502 04F8 000B 1B3E 403D 1100"
	$"043B 4040 3C0A E500 0903 151C 3319 0D3A"
	$"3814 06FE 0008 0823 383A 3B3B 3A2B 0CEE"
	$"0005 163C 403D 1601 FA00 070F 3E40 403A"
	$"0B00 01F6 0057 FD00 041F 3F40 3D0B FC00"
	$"0807 3240 3C40 2B05 0102 F900 0B1B 3E40"
	$"3D10 0008 3B40 403B 07E4 000D 051D 3A09"
	$"203C 1B01 0004 0004 293E FB40 032F 0A02"
	$"01FE 0002 0105 03F8 0005 0229 3D40 3C0D"
	$"F900 0708 3840 3F3D 1100 02F6 0057 FD00"
	$"041D 3F40 3C0A FB00 0718 3D3E 3F3C 1400"
	$"04F9 000B 1C3F 403E 1200 0C3C 4040 3C08"
	$"E300 0D16 370D 362D 0603 0300 0019 3D40"
	$"3EFE 4005 3E40 3E1A 0202 FD00 0102 01FA"
	$"0007 0100 0739 403E 2C05 F800 051B 3E40"
	$"3E25 03F5 0059 FD00 041F 3F40 3C08 FB00"
	$"0804 2B40 3C40 2F08 0103 FA00 0B19 3E40"
	$"3E11 0011 3D40 403B 08E3 001C 0C26 0E36"
	$"1700 0300 0005 3440 3D40 3F3E 3F40 3E3F"
	$"3008 0102 0000 0108 01F8 0005 133C 403E"
	$"1901 F800 0709 3B40 403A 0B00 02F7 0059"
	$"FD00 041E 3F40 3C08 FA00 0711 3840 3D3D"
	$"1D00 02FA 000B 183E 403D 0F00 133D 4040"
	$"3A08 E300 0404 160F 2F09 FD00 0406 3B40"
	$"3F40 FE3E 0B40 3F40 3B09 0002 0000 0618"
	$"07FB 0007 0100 022A 3E40 3B0D F700 0702"
	$"293F 403D 1500 02F7 0058 FD00 041B 3E40"
	$"3C08 FB00 0901 001D 3D3D 4037 0C00 03FB"
	$"000B 193E 403E 1100 153D 4040 3807 E200"
	$"030B 1326 03FD 0003 053A 4040 FD3E 0C3F"
	$"3F40 3B07 0000 0100 0824 1601 FC00 0702"
	$"000C 3A40 3E2A 05F6 0005 123E 403E 2B04"
	$"F600 5AFD 0004 1B3F 403C 09F9 0008 0629"
	$"403D 3F22 0202 01FC 000B 193E 403D 1000"
	$"143D 4040 3604 E200 0301 0B19 02FD 0003"
	$"053B 403F FC3E 0D3F 403A 0900 0002 0314"
	$"2824 0800 01FD 0007 0224 3E40 3E17 0101"
	$"F700 0707 3B40 403B 0B00 01F8 0058 FD00"
	$"041D 3F40 3C09 F800 0710 3B3F 3F39 0F00"
	$"03FC 000B 1A3F 403D 1100 103C 4040 3A03"
	$"E100 0201 0801 FD00 0406 3840 3E3F FE3E"
	$"FE3F 0130 08FE 0005 0829 2425 1003 FE00"
	$"0701 000A 3940 403B 0CF5 0007 0227 3F40"
	$"3D1A 0101 F800 53FD 0004 1D3F 403C 08F8"
	$"0008 0227 403D 3F25 0201 02FD 000B 193E"
	$"403D 1200 0C3C 4040 3C05 E000 0001 FB00"
	$"0323 3F3E 3CFE 3F0D 3C3E 3E1E 0000 0200"
	$"0D37 1C27 1C11 FC00 0517 3D40 3F29 04F4"
	$"0005 113E 403F 2F05 F700 57FD 0004 1F3F"
	$"403D 08F7 0007 0F3A 403E 3A11 0004 FD00"
	$"0B18 3E40 3D10 0009 3C40 403C 08E1 0002"
	$"0303 01FC 001E 0D37 403D 3E3E 3D3D 4037"
	$"0D00 0302 001E 3C16 3122 2405 0001 0003"
	$"2E3E 3F3F 18F3 0007 083C 403F 3B09 0002"
	$"F900 57FD 0004 1E3F 403C 08F7 0008 0122"
	$"3F3D 4029 0500 02FE 000B 193E 403D 1200"
	$"043B 4040 3C0A E200 0402 0000 0201 FC00"
	$"081A 363D 3D3F 3E3E 361A FD00 1014 3B37"
	$"1336 1C32 0F00 0200 0B3A 3F40 3B0A F200"
	$"062A 4040 3B18 0001 F900 5EFD 0004 1B3E"
	$"403C 08F6 0005 0A35 403D 3D1C FE01 0D00"
	$"0019 3E40 3D14 0001 3B40 403D 12E1 0001"
	$"0405 F900 060B 1D2C 3633 200B FE00 1802"
	$"0F35 3F1B 1539 1539 1600 0001 1F3D 3F3F"
	$"310B 0402 0406 0403 FC05 0B04 0203 031B"
	$"3D40 3F2F 0500 01FA 0063 FD00 041B 3E40"
	$"3C09 F500 161B 3D3E 4038 1500 0200 0019"
	$"3E40 3D13 0001 3940 403E 2202 E300 0613"
	$"272A 1403 0001 FB00 2A01 0102 0000 0607"
	$"0E25 3740 290A 2B3E 103A 1B02 0006 3640"
	$"3F40 3F38 3633 383A 3633 3637 3637 3939"
	$"3435 373B FE40 033B 1200 02FA 005F FD00"
	$"041D 3F40 3D09 F500 0606 2D40 3C40 3309"
	$"FE00 0719 3E40 3E12 0000 2CFE 4001 3305"
	$"E600 2501 0011 3840 403B 2B1D 1A19 1513"
	$"1517 1714 1316 212B 3338 3A3F 4034 0E11"
	$"3D37 0F3A 1E03 000E 3CFE 4003 3F40 3F3F"
	$"F640 003F FB40 032A 0500 01FB 0065 FD00"
	$"041C 3F40 3C08 F400 0512 3A40 3D3E 1DFE"
	$"000E 183E 403E 1200 0017 3E40 403B 0B00"
	$"01ED 0002 0102 01FE 0001 031C FC40 093E"
	$"3F3F 3E3E 3D3D 3E3F 40FE 3F10 4040 3E38"
	$"2E14 0A2B 3D1C 133A 1B00 0024 3FFE 4002"
	$"3F3E 3FF5 400A 3F3E 3E3F 403F 403B 1300"
	$"02FB 0060 FD00 0419 3E40 3C08 F400 1601"
	$"2040 3C40 370C 0000 143E 403E 1300 0009"
	$"3C40 3F3D 1D01 EC00 0002 FC00 0402 1A40"
	$"403F F340 153D 3B35 1D11 0904 1D3E 350D"
	$"253D 1600 0439 403E 3F35 2FFD 2D13 2C2B"
	$"2C2D 2E2D 2E2D 2C2C 2D2D 2B2E 3A40 3E40"
	$"2902 FA00 69FD 0004 1A3E 403C 08F5 0019"
	$"0100 0B39 403D 3F1E 0000 163D 403E 1400"
	$"0005 3940 3D40 3103 0001 EF00 3002 0607"
	$"0600 0001 0008 3640 403C 3430 3132 322F"
	$"2D2E 2D2C 2C29 1E10 0705 0000 0218 343F"
	$"270D 393E 0F00 0B3C 403E 2A08 0001 F300"
	$"0A01 0001 143C 3E40 3906 0001 FC00 56FD"
	$"0004 1C3F 403C 08F3 0017 0123 3F3D 4037"
	$"0900 1B3D 403E 1500 0001 2540 3F40 3A0E"
	$"0002 F000 0E03 1F2E 342D 0A00 0001 071F"
	$"3336 1B02 F200 1003 0C19 3140 3814 103E"
	$"3A09 0020 3C3F 3D16 ED00 0704 3940 3F3C"
	$"1100 02FC 005A FD00 041A 3E40 3C08 F200"
	$"0B08 2D40 3D3F 2301 183F 403E 17FE 0008"
	$"0F39 403C 4027 0400 02F3 000F 0300 0B3A"
	$"4040 3E10 0010 2637 2C14 0801 F500 1401"
	$"040A 1528 3639 3F3E 210A 273F 2903 0535"
	$"3F40 390A ED00 0601 2D40 3E3F 2702 FB00"
	$"64FD 0004 193E 403C 08F1 000A 1139 3F3E"
	$"380C 113F 403E 19FE 0009 0225 403D 3F3C"
	$"1700 0403 F500 1401 0201 1C40 3F40 3B0D"
	$"0936 3E40 4039 2F2C 2A27 292B FD2A 1829"
	$"282B 3138 3B3B 3D40 403E 3B26 0816 3C39"
	$"1700 0D3A 403E 2904 EC00 0718 3E40 4039"
	$"0900 01FD 0058 FD00 0418 3E40 3C08 F000"
	$"091D 3D3E 3D23 153C 403D 1AFD 0009 0D36"
	$"403C 4037 1500 0302 F700 0D01 0300 1038"
	$"403E 4027 0209 3E3F 3DFD 4001 3F3F F340"
	$"0F3C 3831 1903 0E34 4029 0903 243B 403E"
	$"17EB 0007 093B 403E 3E1A 0001 FD00 5EFD"
	$"0004 173E 403D 09F0 000A 0930 3F3D 3A2B"
	$"3D40 3E1B 01FD 000A 183B 403C 4035 1100"
	$"0103 01FA 000C 0204 000A 2D40 3C40 390E"
	$"0008 37FD 4001 3C3D F73E 153D 3D3C 3B39"
	$"2D1A 0F07 000F 2D40 3518 0007 383F 403B"
	$"0BEB 0006 0329 403E 4031 06FC 0062 FD00"
	$"0418 3E40 3C06 EF00 0916 3C3E 3D3C 3F40"
	$"3F1B 01FD 0008 0223 403D 3C40 361A 04F9"
	$"0009 0100 000C 2A40 3D3E 3E20 FE00 081F"
	$"383F 3B29 150F 0D0D FD0F 190D 0B0C 0A09"
	$"0805 0303 0100 0001 172E 3C3A 220A 000C"
	$"3A40 3F29 02EA 0005 103C 4040 3C0D FC00"
	$"52FD 0004 173E 403C 06EF 0008 062D 3F3C"
	$"3E3F 403F 1AFB 0009 092F 403C 3C40 3B24"
	$"0E02 FA00 1003 112F 403D 3C40 2F09 0001"
	$"0009 161E 1401 EF00 0E04 0D1C 353D 3E2E"
	$"1002 0120 3C40 3E18 E900 0505 3240 3F3D"
	$"1BFC 005D FD00 0417 3E40 3D08 EE00 0714"
	$"393F 3C3F 403E 1AFA 000A 1135 403E 3C3F"
	$"403A 2612 0AFE 0915 0A11 283B 403C 3E40"
	$"3716 0001 1E37 3928 1103 0001 0100 F601"
	$"1203 070A 172B 373B 403D 3015 0500 0234"
	$"4040 3C0E E800 071A 3E3F 4031 0500 01FF"
	$"005A FD00 0418 3D40 3C08 EE00 0802 253F"
	$"3C3D 403C 1A01 FA00 0710 2F3E 403F 3E40"
	$"3FFB 3D0B 3C3F 403E 403E 2F15 0200 1236"
	$"FE40 053B 3A3C 3B3A 3BF6 3C12 3D3D 3C3E"
	$"4040 3F38 260F 0304 0012 3A3F 3F2E 06E8"
	$"0007 0B3B 403F 3B0A 0003 FF00 47FD 0004"
	$"193F 403D 09ED 0007 0E37 403E 403F 1D02"
	$"F900 0309 2138 3EF6 400B 3F3E 381D 0500"
	$"0002 1C3F 3E3E EB40 093C 3929 1204 0002"
	$"0002 30FE 4000 19E7 0009 0229 4040 3E1E"
	$"0100 0200 52FD 0004 173A 3E39 0FED 0007"
	$"011F 383B 3B39 1A02 F700 0F08 162F 3B3B"
	$"3C3D 3E3E 3D3B 3A3B 331B 09FC 0001 113B"
	$"FE40 033B 3B3C 3CF7 3DFD 3C05 3B38 2411"
	$"0A03 FC00 050A 363B 3D39 0FE6 0008 1439"
	$"3D3B 2D0B 0001 003F FD00 0301 0B0A 0BEA"
	$"0004 0506 0508 01F4 000B 0203 0710 171B"
	$"1B15 0C06 0302 FA00 0704 293B 3A2C 1005"
	$"04F3 0302 0201 02F7 0003 0607 0B0A E400"
	$"030C 0A07 05FD 000D BC00 0303 0706 01E1"
	$"0000 01DB 0012 FC00 0201 0301 A100 0102"
	$"02E2 0001 0202 FC00 0BFB 0000 01C2 0001"
	$"0203 B900 00FF"
};

resource 'PICT' (1025, purgeable) {
	20582,
	{0, 0, 250, 400},
	$"0011 02FF 0C00 FFFF FFFF 0000 0000 0000"
	$"0000 0190 0000 00FA 0000 0000 0000 001E"
	$"0001 000A 0000 0000 00FA 0190 0099 8190"
	$"0000 0000 00FA 0190 0000 0000 0000 0000"
	$"0048 0000 0048 0000 0000 0008 0001 0008"
	$"0000 0000 0CD8 F88C 0000 0000 0000 46F9"
	$"8000 00FF 0000 FFFF FFFF FFFF 0000 FFFF"
	$"FFFF FFFF 0000 F7F7 F7F7 F7F7 0000 EFEF"
	$"F7F7 F7F7 0000 FFFF FFFF 9999 0000 CECE"
	$"FFFF FFFF 0000 EFEF EFEF F7F7 0000 EFEF"
	$"EFEF EFEF 0000 CCCC FFFF FFFF 0000 FFFF"
	$"E6E6 E6E6 0000 EEEE EEEE EEEE 0000 FFFF"
	$"FFFF 6666 0000 E6E6 EFEF F7F7 0000 E6E6"
	$"EFEF EFEF 0000 DEDE EFEF F7F7 0000 FFFF"
	$"FFFF 3333 0000 D6D6 EFEF F7F7 0000 E6E6"
	$"E6E6 EFEF 0000 E6E6 E6E6 E6E6 0000 DEDE"
	$"E6E6 EFEF 0000 DEDE E6E6 E6E6 0000 FFFF"
	$"D6D6 D6D6 0000 D6D6 E6E6 F7F7 0000 FFFF"
	$"FFFF 0000 0000 FFFF CCCC FFFF 0000 9999"
	$"FFFF FFFF 0000 C5C5 E6E6 F7F7 0000 DEDE"
	$"DEDE DEDE 0000 C5C5 E6E6 EFEF 0000 D6D6"
	$"DEDE EFEF 0000 FFFF CCCC CCCC 0000 9999"
	$"FFFF CCCC 0000 CECE DEDE E6E6 0000 FFFF"
	$"C6C6 CECE 0000 D6D6 D6D6 D6D6 0000 CECE"
	$"D6D6 E6E6 0000 B5B5 DEDE F7F7 0000 CCCC"
	$"FFFF 0000 0000 ADAD DEDE F7F7 0000 C5C5"
	$"D6D6 E6E6 0000 EFEF C5C5 C5C5 0000 C5C5"
	$"D6D6 DEDE 0000 FFFF CCCC 6666 0000 BDBD"
	$"D6D6 DEDE 0000 CECE CECE CECE 0000 B5B5"
	$"D6D6 E6E6 0000 FFFF CCCC 3333 0000 9999"
	$"FFFF 3333 0000 BFBF CECE D6D6 0000 FFFF"
	$"ADAD BDBD 0000 EFEF B5B5 BDBD 0000 9C9C"
	$"D6D6 F7F7 0000 9C9C D6D6 EFEF 0000 B5B5"
	$"CECE DEDE 0000 9494 D6D6 F7F7 0000 6666"
	$"FFFF 9999 0000 C5C5 C5C5 C5C5 0000 BDBD"
	$"C5C5 D6D6 0000 FFFF CCCC 0000 0000 BDBD"
	$"C5C5 CECE 0000 FFFF 9999 FFFF 0000 B5B5"
	$"C5C5 DEDE 0000 FFFF A5A5 ADAD 0000 3333"
	$"FFFF FFFF 0000 D6D6 B5B5 BDBD 0000 ADAD"
	$"C5C5 DEDE 0000 ADAD C5C5 D6D6 0000 BDBD"
	$"BDBD C5C5 0000 FFFF 9999 CCCC 0000 BDBD"
	$"BDBD BDBD 0000 8484 CECE FFFF 0000 3333"
	$"FFFF CCCC 0000 9C9C C5C5 DEDE 0000 BBBB"
	$"BBBB BBBB 0000 ADAD BDBD CECE 0000 B5B5"
	$"BDBD BDBD 0000 A5A5 BDBD D6D6 0000 A5A5"
	$"BDBD CECE 0000 F7F7 9C9C A5A5 0000 9C9C"
	$"BDBD D6D6 0000 8C8C C5C5 DEDE 0000 CCCC"
	$"CCCC 0000 0000 B5B5 B5B5 B5B5 0000 FFFF"
	$"9494 9494 0000 ADAD B5B5 BDBD 0000 6363"
	$"CECE FFFF 0000 CCCC 9999 FFFF 0000 6666"
	$"CCCC FFFF 0000 DEDE 9C9C ADAD 0000 0000"
	$"FFFF FFFF 0000 9C9C B5B5 CECE 0000 9C9C"
	$"B5B5 C5C5 0000 FFFF 8C8C 9494 0000 9494"
	$"B5B5 D6D6 0000 ADAD ADAD ADAD 0000 7B7B"
	$"BDBD E6E6 0000 8C8C B5B5 CECE 0000 FFFF"
	$"8484 8484 0000 9C9C ADAD BDBD 0000 AAAA"
	$"AAAA AAAA 0000 9494 ADAD BDBD 0000 8C8C"
	$"ADAD CECE 0000 FFFF 9999 0000 0000 8C8C"
	$"ADAD C5C5 0000 A5A5 A5A5 A5A5 0000 9999"
	$"9999 FFFF 0000 8484 ADAD CECE 0000 9C9C"
	$"A5A5 ADAD 0000 E6E6 8484 8C8C 0000 8C8C"
	$"A5A5 B5B5 0000 8484 A5A5 CECE 0000 FFFF"
	$"7373 7373 0000 8484 A5A5 C5C5 0000 7B7B"
	$"A5A5 C5C5 0000 9C9C 9C9C 9C9C 0000 8484"
	$"A5A5 B5B5 0000 8C8C 9C9C ADAD 0000 EFEF"
	$"7373 7B7B 0000 ADAD 8C8C 8D8D 0000 6666"
	$"CCCC 0000 0000 0000 FFFF 0000 0000 6666"
	$"9999 FFFF 0000 7373 9C9C C5C5 0000 7B7B"
	$"9C9C ADAD 0000 9494 9494 9494 0000 5A5A"
	$"A5A5 D6D6 0000 FFFF 6363 6B6B 0000 7373"
	$"9C9C BDBD 0000 6B6B 9C9C C5C5 0000 8C8C"
	$"9494 9C9C 0000 6B6B 9C9C BDBD 0000 FFFF"
	$"6666 3333 0000 7B7B 9494 A5A5 0000 3333"
	$"CCCC 3333 0000 8C8C 8C8C 8C8C 0000 B5B5"
	$"7B7B 7B7B 0000 6B6B 9494 BDBD 0000 7373"
	$"9494 A5A5 0000 5252 9C9C C5C5 0000 5A5A"
	$"B5B5 2121 0000 6363 9494 BDBD 0000 7E7E"
	$"8C8C 9494 0000 8888 8888 8888 0000 6B6B"
	$"8C8C ADAD 0000 7373 8C8C 9C9C 0000 FFFF"
	$"5252 5252 0000 8484 8484 8484 0000 5A5A"
	$"8C8C BDBD 0000 F7F7 5252 5252 0000 0000"
	$"DDDD 0000 0000 5A5A 8C8C B5B5 0000 5252"
	$"8C8C B5B5 0000 7373 8484 8C8C 0000 6666"
	$"9999 3333 0000 0000 CCCC 3333 0000 6363"
	$"8484 9C9C 0000 6363 8484 9494 0000 7B7B"
	$"7B7B 7B7B 0000 5252 8484 B5B5 0000 5A5A"
	$"8484 A5A5 0000 7373 7B7B 8484 0000 CCCC"
	$"6666 0000 0000 6666 9999 0000 0000 4A4A"
	$"8484 B5B5 0000 4A4A 8484 ADAD 0000 7777"
	$"7777 7777 0000 FFFF 3333 6666 0000 6363"
	$"7B7B 8C8C 0000 FFFF 3A3A 3A3A 0000 F7F7"
	$"3A3A 4242 0000 7373 7373 7373 0000 5252"
	$"7B7B 9C9C 0000 BDBD 5252 5252 0000 CCCC"
	$"3333 CCCC 0000 4242 7B7B ADAD 0000 6363"
	$"7373 7B7B 0000 8C8C 6363 6363 0000 4242"
	$"9494 1919 0000 5A5A 7373 8484 0000 5A5A"
	$"7373 7B7B 0000 FFFF 3333 0000 0000 6B6B"
	$"6B6B 6B6B 0000 FFFF 2929 2929 0000 4242"
	$"7373 9E9E 0000 FFFF 0000 FFFF 0000 3A3A"
	$"7373 A5A5 0000 CECE 3A3A 3A3A 0000 4242"
	$"6B6B 8C8C 0000 E6E6 2929 2929 0000 3A3A"
	$"6B6B 9C9C 0000 9494 4A4A 4A4A 0000 3A3A"
	$"6B6B 9494 0000 5A5A 6363 6363 0000 5252"
	$"6363 7373 0000 FFFF 1919 1919 0000 5252"
	$"6363 6B6B 0000 F7F7 1919 1919 0000 4242"
	$"6363 7B7B 0000 CCCC 0000 FFFF 0000 FFFF"
	$"1010 1010 0000 3131 6363 8C8C 0000 0000"
	$"6666 FFFF 0000 4A4A 5A5A 6363 0000 4A4A"
	$"5A5A 5A5A 0000 5555 5555 5555 0000 0000"
	$"6666 CCCC 0000 3131 5A5A 8484 0000 FFFF"
	$"0808 0808 0000 D6D6 1919 1919 0000 5252"
	$"5252 5252 0000 CCCC 0000 9999 0000 4242"
	$"5252 5A5A 0000 2929 5A5A 7B7B 0000 FFFF"
	$"0000 0000 0000 0000 6666 9999 0000 FFFF"
	$"0000 0000 0000 8C8C 3131 3131 0000 BDBD"
	$"1919 1919 0000 0000 6363 9C9C 0000 2929"
	$"5252 7373 0000 EFEF 0000 0000 0000 3A3A"
	$"4949 5151 0000 4444 4444 4444 0000 2929"
	$"4A4A 6B6B 0000 2929 5A5A 1010 0000 DDDD"
	$"0000 0000 0000 D9D9 0000 0000 0000 7373"
	$"2929 2929 0000 CECE 0000 0000 0000 CCCC"
	$"0000 0000 0000 2121 4242 6363 0000 9999"
	$"0000 6666 0000 3131 3A3A 4242 0000 2121"
	$"3A3A 5A5A 0000 B5B5 0000 0000 0000 0000"
	$"5555 0000 0000 8484 1010 1010 0000 1919"
	$"3A3A 5252 0000 2929 3131 3A3A 0000 1919"
	$"3131 4A4A 0000 9191 0000 0000 0000 8888"
	$"0000 0000 0000 1919 2929 4242 0000 1010"
	$"2929 3A3A 0000 2222 2222 2222 0000 1010"
	$"2929 0808 0000 1010 2121 3131 0000 0000"
	$"0000 FFFF 0000 1111 1919 2929 0000 0000"
	$"0000 BBBB 0000 1111 1111 1111 0000 1010"
	$"1010 0909 0000 0000 0000 7777 0000 0000"
	$"0000 5555 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 00FA 0190 0000 0000"
	$"00FA 0190 0000 000A 0000 0000 00FA 0190"
	$"0008 8102 8102 8102 F102 0008 8102 8102"
	$"8102 F102 0008 8102 8102 8102 F102 0008"
	$"8102 8102 8102 F102 0008 8102 8102 8102"
	$"F102 0008 8102 8102 8102 F102 0008 8102"
	$"8102 8102 F102 0008 8102 8102 8102 F102"
	$"0008 8102 8102 8102 F102 0008 8102 8102"
	$"8102 F102 0008 8102 8102 8102 F102 0008"
	$"8102 8102 8102 F102 0008 8102 8102 8102"
	$"F102 0008 8102 8102 8102 F102 0008 8102"
	$"8102 8102 F102 0008 8102 8102 8102 F102"
	$"0008 8102 8102 8102 F102 004B A102 012C"
	$"43F9 4B03 4343 3B14 F802 002C F84B 0343"
	$"383B 14B3 0203 1B5E 8DAF FCCA 0298 8138"
	$"EB02 031B 5E8D AFFC CA02 9881 38F2 0201"
	$"2C43 FC4B 0343 3B3B 07F7 0201 2C43 FC4B"
	$"0343 3B3B 07A1 0200 63A1 0200 C3FE F8FB"
	$"F603 F3EF E689 F802 02EE F8F8 FBF6 04F3"
	$"F2E6 DB5B B602 0512 7CE8 F8F8 F6FE F303"
	$"EFF3 EFEE FEF2 01B2 3BF1 0205 127C E8F8"
	$"F8F6 FEF3 03EF F3EF EEFE F201 B23B F502"
	$"00CA FEF8 06F6 F6F3 EFE6 CE29 F702 0ACA"
	$"F6F8 F8F6 F6F3 EFE6 CE29 A102 006C A102"
	$"04C1 F6EF EFED FBE9 02E6 CEBD F902 051B"
	$"F6F8 F2ED EDFC E903 E6CE BD4F B702 145E"
	$"F2F8 F8F2 EFE6 E6DF DBDB E6DF E6E6 E9ED"
	$"E9E9 9C07 F402 145E F2F8 F8F2 EFE6 E6DF"
	$"DBDB E6DF E6E6 E9ED E9E9 9C07 F702 0AC1"
	$"F6EF EFED E9E9 DBCE BD27 F702 0AC1 F6EF"
	$"EFED E9E9 DBCE BD27 A102 0071 A102 03C1"
	$"EDED DBFB D404 CEC8 BDB9 1DFA 0204 6BF3"
	$"EFE6 DBFC D404 CEC8 B9A3 42B8 0216 AFF8"
	$"F8F2 E6DB CEBF B9AE A3AE B7BD BFC8 CED4"
	$"DBE6 DFBB 0DF6 0216 AFF8 F8F2 E9DB CEBF"
	$"B9AE A3AE B7BD BFC8 CED4 DBE6 DFBB 0DF8"
	$"020A C1EF E9DB D4D4 CEC8 AEA3 23F7 020A"
	$"C1ED EDDB D4D4 CEC8 AEA3 23A1 0200 74A1"
	$"0203 C1E6 D4C8 FBBD 04B9 B9AE A34C FA02"
	$"03B2 EFDF CEFB BD04 B9A3 938C 3DB9 0217"
	$"7CF8 F6ED DBC8 B9AE 8C71 6E71 8088 93A3"
	$"AEB9 BDC8 C8CE CEAB F702 177C F8F6 EDDB"
	$"C8B9 A38C 716E 7180 8893 A3B9 B9BD C8C8"
	$"CECE ABF8 020B C1E6 D4C8 BDBD B9A3 938C"
	$"1D03 F802 0BC1 E6D4 C8BD BDB9 A393 8C1D"
	$"03A2 0200 79A1 020E B2E6 C8BD B9B9 AEAE"
	$"B9AE AEA3 A39E 6AFA 0203 DDE9 D4C8 FBAE"
	$"04A3 978C 8235 BA02 091B F4F3 E9D4 C8AE"
	$"9371 60FE 4F0C 6571 8C97 A3B9 AEB9 BDBD"
	$"BFBF 5AF9 0209 1BF8 F3E9 D4C8 B993 8060"
	$"FE4F 0C65 718C 97A3 B9AE B9BD BDBF BF5A"
	$"F902 0AB2 E6CE BDB9 AEA3 938C 8020 F702"
	$"0AB2 E6CE BDB9 B9A3 938C 8020 A102 0072"
	$"A102 04B2 E6CE BDB9 FBAE FEA3 0093 FB02"
	$"051B F2E6 CEBD B9FC AE04 A397 8C82 35BA"
	$"0210 A7F8 EDD4 C8B9 A38C 6A4F 2713 131D"
	$"3D7A 96FB A302 AEA3 93F9 0211 B2F6 EFD4"
	$"C8B9 A38C 6A4F 2713 131D 3D7A 969E FCA3"
	$"02AE A393 F902 0AB2 DFCE BDAE AEA3 938C"
	$"8020 F702 0AB2 E6C8 BDAE AEA3 938C 8020"
	$"A102 007A A102 05B2 E6CE BDAE AEFD A305"
	$"AEA3 A39E 9E1D FC02 046B EFE6 C8B9 FDA3"
	$"06AE AEA3 978C 8235 BA02 09EF EDE6 C8B9"
	$"AE93 826E 27FC 020B 1D93 9793 8C82 8080"
	$"6E80 8020 FA02 09EE EDE6 C8B9 AE93 826E"
	$"27FC 020B 1D93 9793 8C82 8080 6E80 8020"
	$"FA02 0AB2 E6CE BDB9 AEA3 938C 8020 F702"
	$"0AB2 E6CE BDB9 AEA3 938C 8020 A102 007B"
	$"A102 07B2 E6CE BDAE A3A3 97FE A304 B9A3"
	$"9EA3 42FC 020F B2ED DBC8 B9A3 97A3 A3AE"
	$"AEA3 978C 8235 BB02 092C F2E6 D4BD B9AE"
	$"A38C 8CFA 020A 4197 8871 6A6A 5A5D 5D60"
	$"2BFB 0209 2CEF E6D4 B9B9 AEA3 8C88 FA02"
	$"0A41 978C 716A 655A 5D5D 602B FA02 0AB2"
	$"E6CE BDAE AEA3 978C 9320 F702 0BC1 DFCE"
	$"BDAE AEA3 938C 8C1D 03A2 0200 9BA1 020F"
	$"B2E6 C8BD B9A3 9393 6AA3 A3B9 A39E 9E6A"
	$"FC02 08DD E9D4 B9A3 9393 71A3 FEAE 0397"
	$"8C82 35F5 020D 1B72 7CB3 D3CB CAD3 CBCA"
	$"8181 5E1B ED02 0A4B 7C98 D3CA CAD3 C181"
	$"6B1B F502 0703 3BE9 DFC8 B9AE AEFE A300"
	$"27FA 0209 A37A 655D 4F4F 4141 4C41 FB02"
	$"0639 E9E6 C8B9 B9AE FEA3 0027 FA02 09A3"
	$"7A65 5D4F 4F41 414C 41FA 020A B2E6 C8BD"
	$"B9AE A397 9397 27F7 020B C1E6 CEBD B9B9"
	$"A393 8C80 1D03 A202 00A0 A102 0FB2 E6C8"
	$"BDB9 A393 8C41 97A3 AEAE 9E9E 93FD 0209"
	$"22F2 E6CE B98A 8C65 71B9 FEAE 0397 8C82"
	$"35F8 0202 127C EEFE F802 F3F2 F2FE EF01"
	$"F2F6 FEF2 01DD 4DF2 0201 1BB3 FDF8 00F6"
	$"FEF2 06EF F2F6 F2DD 7413 F702 043B E6D4"
	$"C8B9 FDAE 03B9 BB4D 11FC 0202 423D 20FE"
	$"1300 07F8 020C 3BE6 D4C8 B9AE AEB9 AEB9"
	$"C84A 11FC 0202 423D 20FE 1300 07F7 0203"
	$"B2E6 CEBD FEAE FEA3 0029 F702 0AC1 E6CE"
	$"BDAE AEA3 938C 8020 A102 009D A102 10B2"
	$"E6CE BDAE 9793 8C3D 70A3 B9AE A39E A31D"
	$"FE02 116B EFE6 BDAE 8C80 3D71 AEB9 B9AE"
	$"978C 8235 03FA 0214 2CEE F8F8 F2ED E6E6"
	$"D4CE C8CE CEDB DFE6 E6E9 E6E6 89F4 0200"
	$"8DFE F806 F2ED E6DF DFDB DBFD E603 EDE6"
	$"D329 F802 040D DBCE C8B9 FEAE 08B9 B9CE"
	$"E6ED CA74 5E11 F002 040D DBCE BDB9 FEAE"
	$"08B9 B9CE E6E9 D374 5E11 EF02 03B2 E6CE"
	$"BDFE AE03 A3AE BD30 F702 0BC1 E6CE BDB9"
	$"AEA3 938C 8C1D 03A2 0200 A4A1 0210 B2E6"
	$"CEBD AE97 8C88 3542 AEB9 AEA3 A39E 4CFE"
	$"0208 B2ED D4BD 9388 7A23 7AFE B904 AE97"
	$"8C82 35FA 0211 2CF8 F6EF E9DB CEBD B9A3"
	$"9393 8CA3 B9BD C8CE FED4 01DB 7BF6 0207"
	$"A5F8 F8EF E6DB C8BD FDAE 08B9 BFC8 CEDB"
	$"DBDF DB4A F802 02B7 C8B7 FDAE 0BB9 BDC8"
	$"D4E6 E9EF F2EF D384 4AF2 0212 B7C8 BDAE"
	$"B9AE AEB9 BDC8 D4E6 E9EF F2EF D384 4AF2"
	$"0203 B2E6 CEBD FDAE 04B9 BFD4 E9F2 FAF8"
	$"0BF6 EDDB C8B9 AEAE A393 8C80 1DA1 0200"
	$"B3A1 0209 B2E6 CEBD B993 8C82 3523 FDB9"
	$"029E A370 FE02 10DD E9D4 B98A 807A 0280"
	$"BDBF B7AE 978C 8235 FA02 08C1 EDE6 CEBD"
	$"B997 8A6E FE5D 0B6A 8093 A3AE B9BD C8C8"
	$"BDBF 2BF8 0211 B3F8 F6E9 DBC8 B997 8C71"
	$"6E71 808C A3AE B9BF FEC8 01BF 20FA 0203"
	$"034A BFB9 FEA3 02B9 AEB9 FEC8 09E6 DBE9"
	$"E6E9 E6E9 E68D 23F5 0202 4DBD B9FE A30F"
	$"AEAE B9BD C8C8 E6DB E9E6 E9E6 E9E6 8D23"
	$"F502 04B2 E6CE BDB9 FEAE 06B9 BDC8 DBE6"
	$"EFED FDEF 0CED E6DB C8BD B9B9 AEA3 938C"
	$"801C A102 00B6 A102 25B2 E6C8 BDB9 978C"
	$"8035 02B9 BDB9 AE9E A3A3 0202 1BEF E6CE"
	$"A38C 715D 028C C8BF BDAE 978C 8235 03FC"
	$"0214 27E6 C8B7 9782 6E6A 4841 4127 1D27"
	$"4C8C AEAE B9AE AEFE B900 67F9 0210 86F8"
	$"F6E9 CEBD B98A 6E5D 4F4F 5D6A 808C 97FC"
	$"A301 AE8C F802 0782 AEA3 9397 A3A3 AEFE"
	$"B90B C8C8 CED4 DBDB DFE6 E6E9 B22C F602"
	$"0782 AEA3 9393 A3A3 AEFE B90B BFC8 CED4"
	$"DBDB DFE6 E6E9 B22C F702 03B2 E6CE BDFD"
	$"AE04 B9BD C8D4 D4FE DB06 E6DB DBD4 CEC8"
	$"C8FE B905 AEA3 9393 8020 A102 00B8 A102"
	$"24B2 E6CE BDAE 978C 8035 0282 BDBD B9A3"
	$"A3B9 2302 73E9 E6C8 9380 7127 028F C8C8"
	$"BDB9 978C 8235 FB02 0A6D B9A3 8271 6A5D"
	$"4F41 2706 FD02 061D C8BD B9AE AEB9 FEA3"
	$"FA02 1312 F3F6 E6DB BDB9 8C6E 5D48 2713"
	$"1320 4F88 8C8C 80FD 7102 802B 03FA 0213"
	$"0D80 938A 968C 9393 A3A3 B9AE B9B9 BDC8"
	$"C8CE D4DF FEE6 0073 F702 020D 8093 FE8C"
	$"0493 93A3 A3AE FEB9 09BD C8C8 CED4 E6DF"
	$"E6E6 6DF8 0203 B2E6 C8BD FDAE 06B9 AEB9"
	$"B9BD BDC8 FEBD 02C8 BDBD FEB9 FEAE 04A3"
	$"8A8C 8020 A102 00AF A102 24B2 E6CE BDB9"
	$"938C 8035 024D C8BF B9A3 A3B9 5A02 90E9"
	$"DBBD 9371 7A16 028F C8C8 BDB9 938C 8235"
	$"FB02 091D 4235 4142 4F5D 4F4F 06FB 0209"
	$"B2C8 B9B9 AEA3 A393 9313 FB02 0C86 F3E9"
	$"E6BF AE93 8065 4C13 0203 FE02 043D 8871"
	$"6A5D FE4F 015D 42F9 0204 0307 4D88 7AFD"
	$"8001 8C93 FEA3 0AB9 AEB9 BDBD C8CE D4DB"
	$"DB6D F702 0307 4C8C 7AFE 8002 8C8C 93FE"
	$"A30A B9AE B9BD BDC8 CED4 DBDB 6DF9 0203"
	$"B2E6 CEBD FCAE FEA3 FB97 FDA3 00B9 FCAE"
	$"0393 8C80 27A1 0200 A9A1 0225 B2E6 CEBD"
	$"AE97 8C80 3502 29C8 C8B9 B9AE B98F 02C5"
	$"E6CE B98C 716E 0202 8FCE D4BD AE97 8C82"
	$"3503 F802 0403 0203 0713 FB02 0A27 DBD4"
	$"C8B9 B9A3 9793 8C20 FB02 0ACA EFDF C8B9"
	$"A38C 715A 2703 FB02 086E 6A5D 4C41 2727"
	$"201D F602 0120 5DFC 7105 8080 8C93 A3A3"
	$"FEAE 01B9 BDFE C801 CE42 F602 0120 5DFD"
	$"710B 6E80 808C 939E A3AE B9AE B9BD FEC8"
	$"01CE 42FA 0203 B2E6 CEBD FEAE 03A3 A393"
	$"8CF9 8003 8C8C 97A3 FDAE 04A3 938C 8020"
	$"A102 00A9 A102 24B2 E6C8 BDB9 938C 8035"
	$"0202 C8C8 BDAE AEBD BB13 E6D4 C8A3 806E"
	$"4C02 029B D4CE BDB9 978C 8235 F002 0E11"
	$"4B98 EEF2 E6CE C8B9 AEA3 978C 8C20 FB02"
	$"09F2 E9D4 C8AE A38C 7160 11FA 0209 1D20"
	$"1302 0203 0302 0203 F502 0413 3565 6E6A"
	$"FE6E 0C80 8C93 A3A3 B9AE AEB9 B9B7 BDA4"
	$"F402 0313 3565 6AFE 6E0D 7180 8C93 A3A3"
	$"B9AE AEB9 B9BD BDA4 FA02 03B2 E6CE BDFE"
	$"AE04 A393 806E 6AFE 5A08 6060 6A6A 718C"
	$"97A3 B9FE AE04 A393 8C80 27A1 0200 96A1"
	$"0225 B3E6 C8BD B993 8C8C 3502 028F CEC8"
	$"B9AE B7BD 73D4 CEBD A37A 6A2B 0202 B7DF"
	$"CEBD B993 8C82 3503 F502 122C 74A0 DDF8"
	$"F2F2 E9DF CEBF B9AE B9A3 938C 8C20 FC02"
	$"092C EFE6 CEB7 AEA3 9371 6AE0 020A 1127"
	$"425A 6A7A 7A8C 97A3 B9FA AE00 35F2 0209"
	$"1327 425A 6E71 7A8C 9EA3 FEAE 00B9 FDAE"
	$"0035 FB02 03B2 E6CE BDFE AE05 A38C 6A4F"
	$"4C48 FC4F 045D 6A88 97A3 FDAE 05A3 8A8C"
	$"801D 03A2 0200 8CA1 020D B2E6 CEBD B997"
	$"8C80 3502 025B CEC8 FEB9 14BD C8C8 BFB9"
	$"8C71 6A1D 0202 9CDB CEBD B993 8A8C 3003"
	$"F902 0B1B 98CB F8F6 F3E9 E6DB C8C8 B7FD"
	$"B906 AEAE A393 8C80 20FC 0209 3BED E6C8"
	$"B9B9 A38C 7A4F DD02 0603 1335 708C 9EA3"
	$"FCAE FEA3 0042 EE02 0513 3570 8C9E A3FC"
	$"AEFE A300 42FB 020A B2E6 CEBD AEAE A393"
	$"7A65 13F7 0200 82FE B906 AEB9 A393 8C80"
	$"27A1 0200 94A1 020E B2E6 CEBD AE97 8C80"
	$"3502 0230 CEC8 BDFE B906 BDBD B9AE 8871"
	$"6AFE 0208 ABE9 CEBD AE97 9782 35FA 020A"
	$"12B3 F8F8 F6F2 E6DF BFAE 93FE 8000 8AFC"
	$"AE04 A393 8C80 23FC 020A 3BE9 DBC8 B9AE"
	$"A38C 885A 03E4 0201 3B30 F902 0342 B9AE"
	$"B7FD AE03 A393 8C6A F502 014A 30F9 0203"
	$"42AE AEB9 FDAE 03A3 938C 6AFB 020A B2E6"
	$"C8BD AEAE A393 806E 13F7 020A 8FC8 BDB9"
	$"B9AE A393 8C80 23A1 0200 A5A1 0208 B2E6"
	$"C8BD B997 8C80 35FE 0202 DBCE C8FB B903"
	$"937A 6E4C FE02 09B2 DFCE BDB9 938C 8235"
	$"03FC 0219 12E8 F8F8 EFE6 D4BD A380 6A4F"
	$"4F4C 3527 80AE B9B9 AEA3 938C 8020 FC02"
	$"093B E6D4 C8B9 AEA3 9693 6AEB 0203 64D3"
	$"D3DD FDF8 02ED DF14 F902 036E C8BD B9FE"
	$"AE03 978C 8C5A FD02 0373 D3CA DDFD F802"
	$"E9E6 14F9 0203 70C8 BDB9 FEAE 0397 8C8C"
	$"5AFB 020A B2E6 CEBD AEAE A393 8071 1DF7"
	$"020B 9CD4 C8BD AEAE A393 8C80 1D03 A202"
	$"00AD A102 08B2 E6CE BDAE 978C 8035 FE02"
	$"0C9B CEC8 B9AE B9AE AEA3 8C80 6A27 FE02"
	$"08C8 DFCE BDAE 978C 8235 FB02 0CC3 F6F6"
	$"E6DF C8B9 936A 4F2B 1D06 FE02 0A8F C8BD"
	$"B9AE A393 8C80 2003 FD02 0927 E6D4 C8B9"
	$"B9A3 A39E 93F8 0200 4BF5 020A 52F2 F3F3"
	$"F2F3 EDED E6D4 5AF9 020A 3BD4 C8BD B9AE"
	$"A393 827A 4FFD 020A 54F2 F2F6 F2EF EFE6"
	$"E6D4 4DF9 020A 3BD4 C8BD AEAE A393 827A"
	$"4FFB 020A B2E6 CEBD AEAE A393 8C7A 1DF7"
	$"020A C8DB CEBD AEAE A393 8C8C 1DA1 0200"
	$"B2A1 0208 B2E6 C8BD B993 8C8C 35FE 0203"
	$"64D4 C8B7 FDAE 04A3 8C6E 6A1D FE02 08B3"
	$"DFCE BDB9 978C 8235 FC02 0A1B F8EF E6CE"
	$"BDAE 8C71 5D1D FB02 09BB D4C8 B9B9 A393"
	$"8C80 27FB 0209 DBCE BDB9 AEAE A3A3 B91D"
	$"FA02 004B FEF8 05E8 CA8D 6D5B 14FD 020A"
	$"29ED E9E6 E6DB DBD4 CEC8 8FF9 020A 54E6"
	$"CEBD B9AE A38C 806E 2BFD 020A 22EF E9E6"
	$"DFDB DFD4 CEC8 8FF9 020A 54E6 CEBD B9AE"
	$"A38C 806E 2BFB 020A B2E6 CEBD AEB9 A393"
	$"8C80 23F7 020A B2DF CEBD B9B9 A393 8C80"
	$"27A1 0200 AFA1 0208 B2E6 CEBD B993 8C82"
	$"35FE 020B 30D4 C8BD B9AE AEB9 9380 716A"
	$"FD02 08B2 DFCE BDAE 978C 8235 FC02 096B"
	$"EFED D4BD AEA3 8C80 41FB 020B 11E9 E6C8"
	$"B9AE A393 8C80 1D03 FC02 FEC8 00B9 FDAE"
	$"01B9 7FFA 0209 DDF8 F8F2 EDE6 DBC8 B92B"
	$"FC02 03C5 D4CE C8FC BD02 CE5B 03FB 020A"
	$"D3E9 D4BD B9A3 9780 6A65 1DFC 0203 CED4"
	$"CEC8 FCBD 01C8 5BFA 020A D3E9 D4BD AEA3"
	$"9780 6A65 1DFB 020A B2E6 CEBD AEAE A393"
	$"8C80 20F7 020B B2E6 CEBD AEAE A393 8C80"
	$"1D03 A202 00BB A102 08B2 E6CE BDAE 978C"
	$"8235 FD02 0AE6 C8C8 B9AE AEA3 8A8C 6E4C"
	$"FD02 08B2 DFCE BDAE 978C 8235 FC02 0974"
	$"EDDB C8B9 AEA3 9793 41FB 020A 98F2 DFC8"
	$"B9AE A393 8C8C 20FB 020A 5BC8 BDB9 AEB9"
	$"AEB9 BDCE 89FD 020A 12B3 F8EF E9DF D4C8"
	$"B997 71FB 0202 67C8 BDFE B906 AEAE B9C8"
	$"D473 11FE 020B 12CA F3E6 CEBD AEA3 886E"
	$"5A4F FB02 045B C8BD B9B9 FEAE 04B9 C8DB"
	$"7311 FE02 0B12 C3F2 E9CE BDB9 A388 6E5A"
	$"4FFA 020A B2E6 CEBD AEAE A393 8C80 23F7"
	$"020C B2E6 CEBD AEAE A393 8C8C 1D03 03A3"
	$"0200 BCA1 0208 B3E6 C8BD AE97 8C82 35FD"
	$"020A 9CCE C8B9 B9AE A38C 7A6A 27FD 0208"
	$"B2E6 CEBD B9A3 8C82 35FD 020B 036D E6C8"
	$"C8B9 AEA3 A3AE A40D FE02 0C12 98F2 E9CE"
	$"BDB9 AEA3 978C 8C1D FB02 1911 B7B9 AEA3"
	$"A3B9 AEBD C8DB E68D 7CB3 F8F6 EFE6 C8BF"
	$"A38C 6E6A 27FB 0203 07BD B9B9 FDAE 13B9"
	$"C8D4 E6F2 D3DD DDF8 F3ED DBC8 AEA3 8C6E"
	$"5D4F 20FB 0202 11B7 B9FC AE13 B9C8 CEE9"
	$"EDE8 CAE8 F6F6 E9DB C8AE A38C 6E5D 4F20"
	$"FA02 0AB2 E6C8 BDAE AEA3 938C 8020 F702"
	$"0BB2 DFCE BDB9 AEA3 938C 801D 03A2 0200"
	$"B9A1 0208 B2E6 CEBD B993 8C82 35FD 020A"
	$"73CE C8B9 AEAE A38C 6E6A 13FD 0208 B2E6"
	$"CEBD B993 8C82 35FC 0203 4ACE C8B9 FDAE"
	$"12B9 C8C5 8981 A0EF EFE6 C8BD AEAE B9A3"
	$"A38C 8C20 FA02 1839 B9B9 A39E A3AE B9C8"
	$"CEDF E9F2 F2EF E6DB BFAE 8C71 604F 4103"
	$"FC02 1B07 025A B9AE A39E A3AE AEBD C8D4"
	$"DFE9 EFEF EDE6 D4C8 AE97 8C6A 5D4F 35F9"
	$"0202 5AB9 AEFE A313 B9AE B7C8 D4E6 E9EF"
	$"EFED E6DB C8AE 9388 6A5D 4F35 F902 0AB2"
	$"E6CE BDAE AEA3 938C 8023 F702 0AB3 E6CE"
	$"BDAE AEA3 938C 8027 A102 00B2 A102 08B2"
	$"D4C8 B993 8C80 712B FD02 0930 CEC8 B9A3"
	$"938C 716A 5AFC 0208 C8DB BFAE 978C 7A71"
	$"2BFB 0202 B7B7 AEFD A307 AEBD CEDB DFDF"
	$"CEBD FEA3 079E A397 938C 8080 23F9 0216"
	$"65A3 968C 9393 A3AE B9C8 CEDB DBCE BFB9"
	$"9780 6A4F 4141 0DF8 0218 65A3 968C 8C93"
	$"97A3 AEB7 C8CE D4D4 C8C8 B9A3 8C6E 654F"
	$"4135 07F9 0219 036A A397 8C8C 9393 A4AE"
	$"B7C8 CED4 D4C8 C8AE A38C 7165 4F41 3507"
	$"F902 0ABB DBBF AEA3 A393 8C7A 711D F702"
	$"0AB2 D4C8 B9A3 93A3 8080 6E1D A102 00AC"
	$"A102 08B7 BFAE 8C71 6A60 6A27 FC02 08C8"
	$"AE97 806E 6A5D 483D FC02 089B BFAE 8C71"
	$"6A60 6527 FB02 1A30 AEA3 8C8C 888C 8C97"
	$"A3AE A397 8071 6E3D 7A80 807A 716E 656A"
	$"2703 F902 085A 8C88 807A 8088 8C93 FEA3"
	$"0996 8C7A 6A5D 4F41 350D 03F7 0202 658C"
	$"88FD 800F 8C8A A397 A397 938C 7A6E 654F"
	$"4141 2707 F602 0265 8C88 FD80 0F8C 8C93"
	$"9797 A393 8C80 7165 4F41 4127 07F8 0206"
	$"9BBF AE8C 7A7A 71FE 6501 2300 F802 0AB7"
	$"BFAE 8C7A 7A71 6565 6A1D A102 009B A102"
	$"088F AE8C 715A 4F4C 4F27 FC02 088C A380"
	$"6A5A 5D4F 4C20 FC02 088F AE8C 715A 4F4C"
	$"4F27 FB02 0503 418C 8071 6EFB 6A0D 5D4F"
	$"4107 026A 716E 655A 5D4F 5D4C F902 0803"
	$"0227 6E71 716E 6A6A FE6E 086A 6560 5D4F"
	$"4141 2707 F402 063D 7171 6E6A 6A6E FC6A"
	$"0765 605D 4F41 4135 1DF3 0203 3571 716E"
	$"F86A 0660 5D48 4141 351D F602 0A8F AE8C"
	$"7165 655D 4F4C 4F13 F702 0B8F AE8C 6E65"
	$"605D 4F4C 4F13 03A2 0200 93A1 0204 7A88"
	$"6E5D 4CFE 4100 20FC 0204 4288 6A5D 4FFE"
	$"4100 0CFC 0204 7A88 6E5D 4CFE 4100 20F9"
	$"0218 204F 6E60 5D5D 4F4F 4C41 2B1D 0602"
	$"022B 6A60 5D4F 4841 4148 0DF6 0203 274C"
	$"6565 FE5D FE4F 044C 4141 2707 F102 0207"
	$"2741 FC5D FD4F 054C 4141 271D 06F1 0202"
	$"0727 41FD 5D00 50FD 4F05 4C41 4127 1D06"
	$"F502 0680 886E 5D4F 4F4C FE41 0013 F702"
	$"067A 886E 5D4F 4F4C FE41 0013 A102 0034"
	$"8102 F202 0706 1323 1D20 2013 07E4 0202"
	$"1313 1DFD 2001 130D EA02 030C 131D 1DFD"
	$"2001 1313 E902 030C 131D 27FD 2001 1313"
	$"8102 F102 0012 8102 AB02 0003 E102 0403"
	$"0203 0203 8102 ED02 000A 8102 BF02 0007"
	$"8102 B402 0008 8102 8102 8102 F102 0008"
	$"8102 8102 8102 F102 000A 8102 BD02 0007"
	$"8102 B602 0008 8102 8102 8102 F102 0008"
	$"8102 8102 8102 F102 005E F002 0023 FD30"
	$"0223 0230 F202 020D 3023 FC02 0130 30FE"
	$"0201 1414 FE02 0523 0D02 0230 23FA 0202"
	$"0D30 29FE 0200 30F6 0203 3002 1414 FD02"
	$"050D 4A30 392B 14F3 0204 4D3B 3930 03E8"
	$"0201 2730 FE02 0211 300D F502 012B 0DEC"
	$"0201 2C0D 8102 B102 006F F002 076D 8FAB"
	$"A38C 6402 C8F3 0201 4AC5 FE9F 0A0D 0214"
	$"BBAB 8FBB 2302 6262 FE02 069B 300D BD9F"
	$"B78C FC02 074D CE8F B7AB 0D02 DBF6 0204"
	$"DB03 5B5A 03FE 0205 39C5 9B8C 8F41 F302"
	$"05E6 9B9B B7B7 0DE9 0207 62DB 2702 0264"
	$"DB30 F502 016D 23FA 0201 A703 F502 01B7"
	$"30B9 0200 0781 02FA 0200 ADEE 0201 4C4C"
	$"FE02 03C8 0D30 07FE 0201 1430 FB02 0F8F"
	$"3902 0271 4D02 5B67 0202 5A7F 025B 5BFE"
	$"0207 9B29 394D 0202 8223 FD02 0A8F 3B02"
	$"027B 5A02 C80D 300D FE02 081D 3B02 0203"
	$"CE03 5A4F FD02 0139 8FFB 0201 2730 FE02"
	$"0239 1420 FD02 00CE FE02 266A 5A02 022B"
	$"2302 022B 0D02 270D 0214 2702 0239 1402"
	$"0227 1439 0D64 AB70 0702 B7AB 2702 022B"
	$"3B11 FE02 122B 2302 0223 120D 2B14 3902"
	$"022B CE30 0202 2730 FE02 0811 3B14 0202"
	$"9C39 3014 8102 B302 00B3 EE02 014C 42FE"
	$"020A BD7F 67B7 0D02 549B 738F 0DFD 0203"
	$"5B82 390D FE02 0329 A34A 13FE 0201 5A97"
	$"FE5A 019F 2BFE 0202 0D8C 13FD 0203 5B82"
	$"3B0D FE02 0FC8 7B67 BD0D 0252 B76D 9C0D"
	$"02C8 024C 42FD 020F 309F 2723 2302 024D"
	$"B76D BB0D 02C8 7142 FD02 00C8 FE02 4965"
	$"4C02 7390 8F7B 0284 5B02 9C62 0289 5B07"
	$"B76D 9C5B 025B BF67 0D5A 5A9F 0214 7F96"
	$"2302 8298 74BB 1D02 8889 8F73 02A7 3030"
	$"C570 8F9C 024D C85A 024D B76D B707 0DC5"
	$"64C8 5B02 8F9F 5AB9 2981 02B4 0200 B3EE"
	$"0201 4C42 FE02 0AAE 0702 8229 02BB 3D29"
	$"7142 FC02 0E29 5A8C 801D 0202 144C 7F97"
	$"3502 4C71 FE4C 06A3 2702 020D 8C35 FB02"
	$"1529 6096 881D 02B9 0602 8229 02B7 3929"
	$"7F4D 02B9 0242 41FD 020E 2BA3 6A5D 6502"
	$"02BD 1202 675A 02B9 06FC 024D BD71 6A6A"
	$"7107 23A3 0202 B713 399B 068F AB02 B711"
	$"4A8F 2339 BD02 4D5A 0202 4C4C 7013 4242"
	$"7127 0229 133B B729 14AB 0200 4D02 8F29"
	$"299E 0202 B902 02B9 0202 B707 0271 6229"
	$"B739 0223 027F 2702 4242 BA02 0007 8102"
	$"FC02 00AE EE02 014C 42FE 020A A302 0271"
	$"2702 AE4C 4241 20FD 020F 5A02 0302 4265"
	$"0242 1302 021D 9302 413D FE02 0570 2702"
	$"1D8C 3DFB 0200 5BFE 0212 4C65 0293 0202"
	$"7127 02B9 4C4C 4223 0297 0241 35FD 0201"
	$"296A FC02 06B9 0202 4C42 029E FC02 2F03"
	$"AE20 1D1D 0202 277A 0202 7127 02A4 424C"
	$"654C 6E02 4C7A 4C4C 4202 4242 0202 4C42"
	$"2B42 6513 6A20 0270 824C 8C27 276E 03FE"
	$"021B 8027 2771 0202 AE02 029E 0202 AE02"
	$"024D 4C02 2B71 A34F 027A 2002 413D 8102"
	$"B402 00AB EE02 014C 42FE 020A 9702 0265"
	$"2002 6E20 0227 13FD 020F 8C67 0707 4C4F"
	$"0242 8013 0235 8002 3535 FE02 0470 270C"
	$"9727 FB02 1703 8C67 0707 4F4F 028C 0202"
	$"6520 0271 2002 270E 0293 0235 35FD 0201"
	$"2765 FC02 068C 2302 6A35 027A FB02 00A3"
	$"FC02 4707 9607 0788 1102 4F71 1135 652B"
	$"0229 6502 0735 023D 3502 0242 4107 656A"
	$"025A 2002 A302 0771 2011 8C07 1380 026A"
	$"2020 6502 028C 0202 8C02 0271 2002 652B"
	$"135A 0207 8802 601D 0235 3581 02B4 0200"
	$"B0EE 0201 4241 FE02 0A93 0202 5A20 0313"
	$"6A5D 4F07 FD02 0F0D 6A8C 824F 0702 024C"
	$"8C88 6513 022B 2BFE 0203 6A23 4296 FE8C"
	$"0020 FD02 1607 6A8C 8C4F 0702 7A02 0260"
	$"2002 136A 5A6A 0702 8002 352B FD02 0123"
	$"60FC 0206 1D71 6A41 0202 80FB 0200 8CFB"
	$"0246 3D6A 6A29 0202 276E 020D 6E07 0202"
	$"4165 6527 0235 3502 0241 3D02 4F2B 025D"
	$"1D02 3D65 4C5A 2002 2965 6E20 0260 1D20"
	$"5A02 028C 0202 6E65 0213 656A 4102 0242"
	$"6A65 2702 5D1D 0235 35B8 0200 0781 02FE"
	$"0200 0881 0281 0281 02F1 0200 0881 0281"
	$"0281 02F1 0200 0C81 0281 02F3 0200 0781"
	$"0200 0200 6AC6 0206 6D62 5B67 5B5B 14F3"
	$"0201 0D0D FE02 010D 0DE8 0205 5B9C 9F8F"
	$"5B0D C802 0664 6264 5B67 5B23 F202 0014"
	$"FE02 0307 1402 07EA 0205 4390 9F9C 7314"
	$"FD02 0173 30F0 0205 7330 0202 644A F502"
	$"0727 6D73 6273 625B 11F3 0201 110D FE02"
	$"0111 0DEA 0201 3B67 FB02 0173 30C2 0200"
	$"78CF 0200 07F9 0207 E6B7 8F82 96B7 C84D"
	$"F502 072C DF3B 0202 23DF 30E9 0207 A7BB"
	$"704D 5AAE A420 C902 07AF BB8F 8C8F ABC8"
	$"70F4 0201 C56D FE02 0190 9CE9 0207 84CE"
	$"825A 4C9E AE42 FE02 01E6 62F0 0205 DF5B"
	$"0202 9B8F F502 083B DF9F 888F 8FBD BF1D"
	$"F502 092C DB3B 0202 30D4 3002 07EC 0201"
	$"64C8 FB02 01D4 5BC2 0200 78C6 0208 CE4F"
	$"0202 0002 7FB9 0DF6 0207 3BDB 3002 023B"
	$"CE30 EA02 025B CE29 FE02 020D 8C6A C902"
	$"08AB 8C02 0002 024F BD3D F502 01E6 62FE"
	$"0201 B28F EA02 0214 D45B FD02 0170 97FE"
	$"0201 DB62 F002 05CE 5B02 028F 80F8 020B"
	$"0702 0230 C829 0202 000D A496 F502 073B"
	$"DF30 0302 39DB 2CEA 0201 67BD FB02 01CE"
	$"5AC2 0201 01C6 0201 C84D FD02 0229 AE29"
	$"FE02 1038 9CB2 9023 0202 1490 C871 3014"
	$"7BBF 7129 FE02 0468 9CB2 740D FE02 0468"
	$"395B 9735 FB02 015B C8FC 0201 4C71 FE02"
	$"0411 84B2 B364 FC02 090D 909B 8F39 0202"
	$"1467 14FE 020F 645B 0202 643B 649C 3B03"
	$"0214 98B2 9C4D F302 019B 88FC 0201 B94C"
	$"FE02 0414 98B2 9C4D FE02 0873 CE8F 4A02"
	$"5BB7 B75D FE02 0C43 98B2 8922 0302 0243"
	$"5B38 9E4D FC02 0307 30C8 2BFD 0213 2771"
	$"2002 02DB 5B4D 9B8F 6702 0702 0212 84B2"
	$"B364 FD02 05CE 6A02 028C 7AF6 0203 0330"
	$"CE29 FD02 015A AEFD 0217 689C B77C 0D02"
	$"0227 89C8 7130 147B C86E 2902 0211 84B2"
	$"B364 FD02 0484 237B 8823 FB02 015B B9FB"
	$"0208 C84D 0202 1184 B29C 67FC 0205 439C"
	$"B28F 2207 D402 0106 CD02 0007 FB02 01CE"
	$"4CFD 0226 5AAE 0D02 0264 E689 629F D43B"
	$"0323 9FAE 8241 1D97 AE82 4202 0289 DF74"
	$"5BC8 BB14 0202 9FC5 8C4F 41FB 0203 2BB9"
	$"7113 FA02 060D C5C5 5B6D C89F FE02 0A14"
	$"CEAB 4C80 BD42 0239 CE23 FE02 109C AB02"
	$"02A7 BF8F 5D6A 0223 DF9C 627B C867 F402"
	$"01AB 88FD 0226 23B9 2B02 022C DF9B 627B"
	$"C86D 0202 8FB9 8C4F 026A A49E 7F02 0264"
	$"E689 649B CE42 0202 6DC5 AE42 5DFB 0203"
	$"0DB9 8C27 F902 0FC8 A38C 4C67 BD73 0202"
	$"11C5 C562 6DBB B7FE 0205 BD4C 0202 7A71"
	$"F502 0230 C823 FD02 017F 8CFE 0219 9CDB"
	$"7462 B7C8 1402 239F BB82 411D 8CB9 8842"
	$"020D C5C5 5B6D C8B7 FE02 04DB B771 651D"
	$"FB02 015B A3FB 0209 C848 0202 89C5 736D"
	$"BD67 FE02 0668 DB89 629C C842 D402 0104"
	$"C602 0CC8 8C4C 4C5A 70AE 4202 0214 DF62"
	$"FE02 1982 9E02 022B 9E20 0202 2397 2302"
	$"025B D422 0202 0CA4 7002 028F B9F7 0206"
	$"65A3 AE97 7142 11FE 0214 90BB 0D02 0214"
	$"B942 0202 89A4 0602 026E 9702 29BD 2BFE"
	$"0206 9F8F 0202 88AE 07FE 0201 C589 FE02"
	$"024C B913 F502 078F A44C 4C4D 67A4 5DFE"
	$"0201 C58F FE02 064D B913 0202 B93D FE02"
	$"107A 7002 0214 CE62 0200 0282 AB02 025B"
	$"B92D F802 0642 96A3 A37F 5A13 FD02 18B9"
	$"8007 0202 70B9 0202 90BB 0C02 021D B94C"
	$"0202 B94F 0202 7A70 F502 2A30 BF6A 4C4C"
	$"5A8F 971D 0202 5BDB 2202 0207 A367 0202"
	$"29A3 1D02 0227 9E23 0202 89BB 0602 0214"
	$"B942 0202 CE88 F802 0167 A4FB 020E BD4C"
	$"0202 CE5A 0202 6A7F 0202 14E6 64FE 0201"
	$"71A3 D402 0103 C602 29C8 8C70 7071 8CAE"
	$"5A02 025B CE29 2927 274C A31D 0227 9720"
	$"0202 27A3 2302 0273 B727 2927 2765 A302"
	$"0282 6EF6 0214 1342 71AE B9B9 2002 02C5"
	$"7129 2927 2788 8002 02AB 6EFE 0205 130D"
	$"0229 AE27 FE02 059C 8F02 027F 70FE 0208"
	$"14CE 5A29 2927 27A3 42F5 0216 9BA3 7070"
	$"717A AE7A 1102 14CE 5A23 2927 27AE 4C02"
	$"029E 3DFE 020F 7A6E 0202 4AC8 2929 2727"
	$"4CAE 1D02 4DA3 F602 0607 3565 9EAE B742"
	$"FE02 01B9 41FE 0213 4CB9 0202 BB70 2929"
	$"2727 8C70 0202 B94C 0202 8070 F602 2B03"
	$"30C8 806A 7071 93A3 4202 027B AB29 2327"
	$"2765 9E02 0229 9720 0202 279E 2302 02BB"
	$"7029 2927 2788 7102 02AE 41F8 0201 5BB9"
	$"FB02 07C8 4C02 02A3 A442 0DFD 0208 5BCE"
	$"2929 2727 4C96 1DD5 0200 FBCB 0200 07FD"
	$"0201 C842 FD02 0827 B94C 025A B9B9 9E93"
	$"FE8C 1723 0227 A323 0202 27A3 2702 028F"
	$"B7A3 A393 8C93 9302 027A 65F2 0210 3DA3"
	$"BB0C 02C8 AEAE 9693 8C8C 6A02 02B9 4CFB"
	$"0202 29B9 27FE 0205 9B88 0202 716A FE02"
	$"0829 C8AE A393 8C8C 8835 F702 0307 028F"
	$"80FD 0208 0E96 7F02 29BD B9A3 93FE 8C04"
	$"4102 029E 41FE 020F 7F7A 0202 5ABF AEA3"
	$"938C 9E93 2302 4C93 F202 071D 82B9 2B02"
	$"02AE 3DFE 0213 4DB9 0202 B7B9 A396 938C"
	$"9370 0202 B94C 0202 7A71 F502 0230 B923"
	$"FD02 084C B92B 028F B9A3 A393 FE8C 1702"
	$"0227 A323 0202 27A3 2702 02BD AEA3 969E"
	$"8C93 7002 029E 3DF8 0201 5BB9 FB02 14C8"
	$"4C02 021D A4AE AE71 2902 025B C8AE 9E93"
	$"8C8C 8820 D502 00CA C602 01C8 4CFC 0205"
	$"8071 024C A307 FA02 0B27 A327 0202 29AE"
	$"2702 0280 8CF9 0201 716A F802 019C 4DFC"
	$"0205 35B9 2702 AE48 F902 01B9 4CFB 0202"
	$"29AE 29FE 0205 8F88 0202 7065 FE02 0229"
	$"B939 EF02 019B 7AFC 0205 4CB9 0227 AE27"
	$"F902 01AE 42FE 0206 967A 0202 4CAE 07FA"
	$"0201 428C F802 017B 67FC 0206 11B9 4C02"
	$"02A3 3DFE 0205 5AB9 0202 AE48 F902 05A3"
	$"4C02 0271 71F5 0202 30B9 27FC 0205 A34C"
	$"0280 7A00 FA02 0B29 AE27 0202 29B9 2702"
	$"02B9 48F9 0201 A33D F802 0267 B90D FD02"
	$"0211 AE42 FD02 081D 5A93 BD29 024D AE06"
	$"CF02 00FF C602 01B7 48FC 0205 885A 0223"
	$"A32D FE02 101D 2B07 0227 A327 0202 29AE"
	$"2702 024C A313 FE02 052B 2902 0270 65F8"
	$"0201 B79E FC02 053D 9316 0271 80FD 0205"
	$"4113 0202 8888 FE02 054A B911 29B9 27FE"
	$"0205 A37F 0202 6A65 FD02 01AE 5AFE 0202"
	$"073D 11F5 0201 8F6E FC02 0565 8802 029E"
	$"65FE 0206 0741 1302 02AE 42FE 0206 887A"
	$"0202 27A3 35FE 0205 1D35 0602 4296 F802"
	$"026D C813 FD02 0613 9E41 0202 933D FE02"
	$"054C A302 0271 80FD 0209 421D 0202 A342"
	$"0202 6E65 F602 0303 39B9 23FD 0206 1393"
	$"3D02 4296 13FE 0210 2927 0202 29A3 2702"
	$"0229 AE27 0202 6E8C 09FE 0205 4223 0202"
	$"9335 F802 022B B94C FD02 0F4F 9E20 0213"
	$"7129 0203 0793 4102 27A3 3DFE 0202 232B"
	$"07D5 0201 22C9 022C 0702 02B9 6527 2729"
	$"2B65 8C1D 0202 6E8F 2302 1D71 7102 0227"
	$"9E2B 0202 279E 2902 0213 8C71 1302 2982"
	$"4F02 026A 5AF8 0228 29BF 8823 0202 2B8C"
	$"5D02 0227 8C4F 0707 418F 2B02 023D AE4C"
	$"0213 A388 0207 9E6E 0D11 679E 6502 0260"
	$"65FD 020B 5A9E 4102 114F 8F0C 0202 5B4D"
	$"F902 1680 8C27 2729 294F 8C39 0002 4C8C"
	$"4102 114F 8C11 0202 A34C FE02 0180 7AFE"
	$"020A 718C 2302 1D71 8002 0241 82F8 020D"
	$"0DA4 A439 0202 1D71 7107 0202 8C35 FE02"
	$"134C A302 0227 8C4F 0707 418C 3902 0297"
	$"3D02 0265 65FE 0201 6A30 FA02 2A29 AE42"
	$"2727 2942 7171 0702 1188 7113 0229 804C"
	$"0202 279E 2B02 0227 9E29 0202 2993 4807"
	$"0741 8C2B 0202 9635 F702 0793 AE65 2930"
	$"6593 6EFE 020F AE96 1302 3D80 1D02 0271"
	$"8823 021D 7171 FE02 015A 2BFE 0201 4D42"
	$"FE02 014D 42E3 0201 1AC6 0200 A3FC 8C01"
	$"4F1D FE02 1E11 6580 8880 6513 0202 0788"
	$"884C 0207 8080 4C02 0223 7188 807A 4F07"
	$"0202 605D F702 0739 8C9E A39E 8C42 07FE"
	$"0205 3D7A 8880 7A39 FD02 0841 938C 8C70"
	$"1302 022B FE8C 0641 5D4F 0202 5D4F FD02"
	$"0607 4C88 8880 7127 FE02 018C 7AF9 0202"
	$"7193 88FE 8C0B 6E27 0002 0207 4C8C 8880"
	$"7127 FE02 146E 8C71 0202 4188 8011 0211"
	$"658C 8880 6513 0202 3580 F702 0C1D 719E"
	$"A4A4 935A 1303 0202 8039 FE02 0142 88FE"
	$"0205 3D80 8880 7A35 FE02 0588 3D02 025A"
	$"65FE 0201 B74C FA02 2527 A388 888C 8C80"
	$"420D 0302 0220 718C 8080 4F07 0202 0788"
	$"884C 0207 8080 4C02 023D 7A88 807A 3DFE"
	$"0201 882B F702 0707 6E97 968C 884F 07FE"
	$"0205 1D93 8C7A 7135 FE02 0611 6588 8C80"
	$"6513 FE02 01AE 4FFE 0201 8080 FE02 0180"
	$"80E3 0200 8DB9 0202 111D 13FC 0207 071D"
	$"1302 0207 1D13 FD02 0213 1D0D EF02 0311"
	$"1D1D 07FA 0202 231D 07FB 0202 071D 13FC"
	$"0202 071D 0DF4 0202 071D 1DFB 0200 71EC"
	$"0202 0723 14FB 0201 231D FE02 0213 1D07"
	$"FE02 0211 1D13 EF02 0307 201D 0DF0 0201"
	$"2323 F302 0127 42EC 0202 1323 07FC 0207"
	$"071D 1302 0207 230E FD02 0123 23EE 0203"
	$"071D 2307 FA02 0220 1D07 FB02 0211 1D13"
	$"D202 0016 8102 DC02 0142 29AE 0200 00F2"
	$"0201 6507 FE02 0007 8102 000E 8102 DC02"
	$"0007 9D02 0007 8102 FC02 0008 8102 8102"
	$"8102 F102 000A 8102 8102 F002 0007 8302"
	$"000A 8102 AB02 0007 8102 C802 000A 8102"
	$"8F02 0007 8102 E402 000E 8102 8102 EE02"
	$"0007 E902 0007 9E02 000C A302 0007 8802"
	$"0007 8102 CA02 000A 8102 8D02 0007 8102"
	$"E602 000E 8102 8102 EC02 0007 E902 0007"
	$"A002 000C A102 0007 8802 0007 8102 CC02"
	$"000A 8102 8C02 0007 8102 E702 0010 8102"
	$"C302 FA07 AF02 0007 EA02 0007 A102 001C"
	$"A002 0007 AC02 0007 FD02 0107 07FD 1201"
	$"0202 FE07 EC02 0007 8102 CE02 001F FB02"
	$"A107 B102 0707 0202 2C38 1B28 11FD 0202"
	$"2C1B 02FD 07D3 0200 0781 02E9 0200 29FD"
	$"02FC 07A7 12FC 07B6 020C 0702 0207 4032"
	$"313E 5C61 755C 09FD 0001 2212 FC07 B302"
	$"0007 EA02 0007 A302 0033 FD02 FD07 0312"
	$"121B 1BAD 2201 1B1B FE12 FE07 B702 0707"
	$"0238 324E 3E53 61FE 9103 A8B6 A832 FD00"
	$"0102 52FC 07EF 0200 0781 02D0 0200 8AFE"
	$"0209 0707 1212 2C2C 3B45 3843 F94B 5352"
	$"4B4B 524B 4B52 4B4B 524B 4B52 4B4B 524B"
	$"4B52 4B4B 524B 4B52 4B4B 524B 4B52 4B4B"
	$"524B 4B52 4B4B 524B 4B52 4B4B 524B 4B52"
	$"4B4B 524B 4B52 4B4B 524B 4B52 4B4B 524B"
	$"4B52 4B4B 524B 4B52 4343 3838 3B2C 2212"
	$"1207 07BA 0211 0702 0740 3131 537E A8A8"
	$"B6B6 C7C2 C4C7 C721 FC00 031B 1B07 12FE"
	$"07D7 0200 0781 02EA 0200 54FE 0204 0707"
	$"1222 7BFB B303 C1B3 B3C1 C4B3 00C1 FEB3"
	$"00C1 FEB3 00C1 FEB3 00C1 FEB3 09CA B3B3"
	$"9C68 1B1B 1207 07BB 020D 0702 384E 213E"
	$"7EA8 B6B6 C7C7 CFCF FDD5 00A8 FB00 0307"
	$"4307 12FE 07B5 0200 07EB 0200 07A4 0200"
	$"3607 0202 0707 121B 2CB3 A6FF 068D 2222"
	$"1B12 0707 BE02 0B07 0202 403E 215C A8B6"
	$"B6CF CFF8 D500 3EFA 0002 2C12 12FD 07F2"
	$"0200 0781 02D2 0200 3607 0202 0707 1222"
	$"38B3 A7FF 07FB 8D38 2C22 1207 07BF 020A"
	$"0702 1258 3121 7EB6 B6D5 CFF6 D500 A8F9"
	$"0002 2C12 12FE 07D8 0200 0781 02EC 0200"
	$"3B07 0202 0707 1B22 45C1 A7FF 07FB 8D4B"
	$"3822 1B07 07C1 0209 0707 0240 4E21 4E91"
	$"B6C7 F2D5 0315 0000 03FD 0003 0238 1212"
	$"FE07 B602 0007 EB02 0007 A602 0040 0C02"
	$"0207 121B 2C45 B3FF FFFB B1E0 ACFF 07FB"
	$"8D52 452C 1B12 07C2 020A 0702 0740 3E15"
	$"61A8 B6D5 CFF2 D504 6100 002C 72FD 0003"
	$"1B22 1B12 FE07 F202 0007 8102 D402 0044"
	$"0702 0207 121B 2C52 B3FE FF04 F5B1 8BE0"
	$"F5AE FF06 985E 452C 1B12 07C3 020A 0702"
	$"384E 2131 91B6 C7C7 CFF1 D50F C400 0002"
	$"EE92 0700 0202 5E1B 1B12 0707 D902 0007"
	$"8102 ED02 0045 0702 0207 121B 2C52 B3FC"
	$"FF03 F5E0 8BB1 B0FF 07FA 985E 522C 1B12"
	$"07C5 020A 0702 0240 3E09 5391 B6C2 CFEE"
	$"D50E 3200 009D CCC0 2200 0738 2222 1212"
	$"07B5 0200 07EC 0200 07A7 0200 4307 0202"
	$"0712 1B2C 52C1 FBFF 03F5 B18B F5B1 FF07"
	$"FA98 6852 2C1B 1207 C602 0907 0212 5821"
	$"217E B6C7 C7EC D50F 7500 004B B5C0 CC52"
	$"0722 3822 1B12 0707 F102 0007 8102 D602"
	$"0045 0702 0207 121B 2C52 B3FD FF04 E0B1"
	$"8BE0 FBB0 FF07 FB98 6852 2C1B 1207 C802"
	$"0A07 0202 434E 153E 91B6 C2CF EBD5 0FB6"
	$"0300 12B5 86B5 CC92 125E 2222 1207 07D8"
	$"0200 0781 02EF 0200 4507 0202 0712 1B2C"
	$"52B3 FEFF 02B1 B1E0 ADFF 07FB 9868 522C"
	$"1B12 07C9 020A 0702 0758 3109 53A8 B6C7"
	$"CFE9 D50F 2800 0286 9286 9DCC 9245 3822"
	$"2212 0707 B502 0007 EC02 0007 A902 0041"
	$"0702 0207 121B 2C52 B3FE FF00 F5AB FF07"
	$"FB98 6852 2C1B 1207 CB02 0A07 0702 3858"
	$"2109 7EB6 C7C7 E7D5 0F6F 0000 5EA5 7C7C"
	$"92B5 525E 2C22 1207 07F0 0200 0781 02D8"
	$"0200 3C07 0202 0712 1B2C 52B3 A6FF 068D"
	$"6852 2C1B 1207 CC02 0A07 0702 284E 0921"
	$"91C7 C2CF E6D5 0FB6 0300 1BC0 867C 7C86"
	$"5E5E 2C22 2212 07D8 0200 0781 02F0 0200"
	$"3F07 0202 0712 1B2C 52C1 A7FF 07FB 9868"
	$"522C 1B12 07CD 020A 0702 0758 3102 5CB6"
	$"C7C7 CFE4 D504 3200 009D 92FE 7C07 8652"
	$"382C 1B12 0707 B402 0007 ED02 0007 AA02"
	$"003C 0702 0207 121B 2C52 B3A7 FF07 FB98"
	$"6852 2C1B 1207 CE02 0907 022C 4E15 097E"
	$"B6CF CFE2 D505 6100 0068 A586 FE7C 0645"
	$"5E2C 221B 0707 EF02 0007 8102 DA02 003E"
	$"0702 0207 121B 2C52 B3A7 FF07 FB98 6852"
	$"2C1B 1207 D002 0A07 0202 403E 0921 94C2"
	$"C7CF E1D5 04C4 0000 1BB5 FE7C 0876 5E68"
	$"2C22 1B12 0707 D802 0007 8102 F202 003F"
	$"0702 0207 121B 2C52 B3A7 FF07 FB98 6852"
	$"2C1B 1207 D102 0A07 0202 583E 003E A8C7"
	$"C7CF DFD5 041E 0002 B592 FE7C 0772 4545"
	$"2C22 1207 07B3 0200 07ED 0200 07AC 0200"
	$"3B07 0202 0712 1B2C 52B3 A6FF 068D 685E"
	$"2C1B 1207 D202 0907 0207 5821 0061 B6CF"
	$"CFDD D505 6100 0072 A586 FE7C 0645 5E2C"
	$"221B 0707 EE02 0007 8102 DC02 003D 0702"
	$"0207 121B 2C52 C1A7 FF07 FB98 6852 2C1B"
	$"1207 D302 0907 022C 4E15 097E C7C2 CFDC"
	$"D505 BC00 0038 A586 FE7C 0752 5238 2C22"
	$"1207 07D8 0200 0781 02F3 0200 3E07 0202"
	$"0712 1B2C 52B3 A7FF 07FB 9868 522C 1B12"
	$"07D4 0208 0702 124E 0909 91C7 C7DA D505"
	$"CF09 0002 C086 FE7C 0768 4552 2C22 1207"
	$"07B2 0200 07EE 0200 07AD 0200 3907 0202"
	$"0712 1B2C 52B3 A7FF 07FB 9868 522C 1B12"
	$"07D5 02FE 0705 5C12 0091 C7C7 D8D5 045C"
	$"0000 869D FD7C 0638 522C 2222 1207 ED02"
	$"0007 8102 DE02 0039 0702 0207 121B 2C52"
	$"B3A7 FF07 FB98 6852 2C1B 1207 D502 0707"
	$"0240 3100 7ECF CFD7 D504 9400 0045 B5FD"
	$"7C06 3868 2C2C 1B12 07D6 0200 0781 02F5"
	$"0200 3C07 0202 0712 1B2C 52B3 A6FF 068D"
	$"6852 2C1B 1207 D602 0707 0243 5C00 53C7"
	$"C7D6 D505 CF0D 0012 B586 FE7C 0768 454B"
	$"2C22 1207 07B1 0200 07EE 0200 07AF 0200"
	$"3907 0202 0712 1B2C 52C1 A7FF 07FB 9868"
	$"522C 1B12 07D7 0207 0707 126C 0015 B6C7"
	$"D4D5 0432 0000 92A5 FD7C 0622 682C 221B"
	$"1207 EC02 0007 8102 E002 003B 0702 0207"
	$"121B 2C52 B3A7 FF07 FB98 6852 2C1B 1207"
	$"D802 0807 0702 4031 007E CFCF D4D5 047E"
	$"0000 5EB5 FD7C 0745 5E38 2C1B 1207 07D7"
	$"0200 0781 02F6 0200 3C07 0202 0712 1B2C"
	$"52B3 A7FF 07FB 9868 522C 1B12 07D8 0206"
	$"0707 146C 0021 C7D2 D510 BC02 002C B586"
	$"7C7C 765E 3845 2C22 1207 07B0 0200 07EF"
	$"0200 07B0 0200 3A07 0202 0712 1B2C 52B3"
	$"A7FF 07FB 9868 522C 1B12 07D9 0207 0707"
	$"0240 3E00 91CF D1D5 0421 0002 B586 FE7C"
	$"0768 2C5E 3822 2212 07EB 0200 0781 02E2"
	$"0200 3707 0202 0712 1B2C 52B3 A6FF 068D"
	$"6852 2C1B 1207 D902 FE07 0375 0315 B6D0"
	$"D504 6C00 0086 9DFD 7C07 2C5E 2C2C 1B12"
	$"0707 D602 0007 8102 F802 003C 0702 0207"
	$"121B 2C52 C1A7 FF07 FB98 6852 2C1B 1207"
	$"DA02 0607 0702 4032 006F CFD5 0494 0000"
	$"45B5 FE7C 0886 3852 382C 2212 0707 AF02"
	$"0007 EF02 0007 B202 003A 0702 0207 121B"
	$"2C52 B3A7 FF07 FB98 6852 2C1B 1207 DA02"
	$"0607 0711 5821 00A8 CFD5 05C7 0300 14B5"
	$"86FE 7C07 6822 5E2C 221B 0707 EA02 0007"
	$"8102 E402 0039 0702 0207 121B 2C52 B3A7"
	$"FF07 FB98 6852 2C1B 1207 DA02 0507 0712"
	$"6C02 21CD D504 2800 02B5 92FE 7C08 721B"
	$"682C 2C22 1207 07D6 0200 0781 02F9 0200"
	$"3C07 0202 0712 1B2C 52B3 A7FF 07FB 9868"
	$"522C 1B12 07DB 02FE 0704 385C 007E CFCE"
	$"D505 7500 0068 A592 FE7C 0745 4552 2C22"
	$"1207 07AE 0200 07F0 0200 07B3 0200 3807"
	$"0202 0712 1B2C 52B3 A6FF 068D 6852 2C1B"
	$"1207 DB02 FE07 0458 3100 B6CF CED5 04A9"
	$"0000 4BB5 FD7C 075E 225E 3822 1B12 07E9"
	$"0200 0781 02E6 0200 3B07 0202 0712 1B2C"
	$"52C1 A7FF 07FB 9868 522C 1B12 07DB 0207"
	$"0707 1287 0209 C2CF CED5 10C7 1000 1BB5"
	$"867C 7C86 6822 5E38 221B 1207 D402 0007"
	$"8102 FB02 003A 0702 0207 121B 2C52 B3A7"
	$"FF07 FB98 6852 2C1B 1207 DB02 FE07 0276"
	$"3E75 CBD5 1032 0000 A592 7C7C 7686 2C5E"
	$"382C 1B12 0707 AD02 0007 F002 0007 B502"
	$"003C 0702 0207 121B 2C52 B3A7 FF07 FB98"
	$"6852 2C1B 1207 DB02 0807 1212 2C09 317E"
	$"B6CF CED5 0561 0000 86A5 92FE 7C07 3838"
	$"522C 2212 0707 E802 0007 8102 E802 003D"
	$"0702 0207 121B 2C52 B3A7 FF07 FB98 6852"
	$"2C1B 1207 DB02 0307 1212 2CFD 0003 315C"
	$"A8CF D1D5 0494 0000 38C0 FD7C 075E 2268"
	$"2C22 1B12 07D4 0200 0781 02FC 0200 4307"
	$"0202 0712 1B2C 52B3 A6FF 068D 6852 2C1B"
	$"1207 DB02 0307 121B 2CFD 0007 0200 0007"
	$"317E C7CF D5D5 05BC 1300 12C0 86FD 7C07"
	$"1B68 382C 1B12 0707 AC02 0007 F102 0007"
	$"B602 003F 0702 0207 121B 2C52 C1A7 FF07"
	$"FB98 6852 2C1B 1207 DB02 0407 121B 2C02"
	$"F700 0315 5394 B6D7 D504 1500 02A5 9DFE"
	$"7C08 7238 3852 2C22 1207 07E7 0200 0781"
	$"02EA 0200 3F07 0202 0712 1B2C 52B3 A7FF"
	$"07FB 9868 522C 1B12 07DB 0204 0712 1B22"
	$"1BF4 0004 0D21 5CA8 CFDB D504 5800 0092"
	$"A5FD 7C07 5E2C 5E38 221B 1207 D302 0007"
	$"8102 FE02 0041 0702 0207 121B 2C52 B3A7"
	$"FF07 FB98 6852 2C1B 1207 DB02 0407 121B"
	$"1B68 F000 0403 316F A8CF DFD5 0475 0000"
	$"68C0 FD7C 0768 225E 382C 1B12 07AA 0200"
	$"07F1 0200 07B8 0200 4107 0202 0712 1B2C"
	$"52B3 A7FF 07FB 9868 522C 1B12 07DB 0205"
	$"0707 2C22 381B ED00 0307 317E B6E2 D511"
	$"A900 004B C086 7C86 7C7C 385E 382C 2212"
	$"0707 E602 0007 8102 EC02 0043 0702 0207"
	$"121B 2C52 B3A6 FF06 8D68 522C 1B12 07DB"
	$"0207 0707 1222 2252 0002 EB00 0309 5394"
	$"B6E6 D511 C705 0038 C07C 7272 6868 385E"
	$"4538 2212 0707 D302 0007 8102 0102 0200"
	$"4807 0202 0712 1B2C 52C1 A7FF 07FB 9868"
	$"522C 1B12 07DB 0206 0707 121B 2C38 2CFE"
	$"02EA 0004 0D21 5CA8 CFEB D512 DCCF 1500"
	$"1268 3838 2C2C 2222 4552 3822 1B12 07A9"
	$"0200 07F2 0200 07B9 0200 5707 0202 0712"
	$"1B2C 52B3 A7FF 07FB 9868 522C 1B12 07F7"
	$"0211 0606 0306 0603 0606 0306 0603 0606"
	$"0306 0603 F702 0A07 0712 1B22 2C5E 1207"
	$"0702 E600 0331 6FA8 C7ED D500 58FB 0009"
	$"0207 2238 6838 221B 1212 E402 0007 8102"
	$"EE02 004C 0702 0207 121B 2C52 B3A7 FF07"
	$"FB98 6852 2C1B 1207 FA02 0106 06EE 0303"
	$"0606 0306 FA02 0A07 0712 222C 385E 1212"
	$"0703 E400 031B 3E6F B6F0 D500 6CFA 0009"
	$"0722 2C72 382C 1B12 0707 D202 0007 8102"
	$"0052 0702 0207 121B 2C52 B3A7 FF07 FB98"
	$"6852 2C1B 1207 FC02 0006 FA03 F406 FA03"
	$"0006 FC02 0C07 0712 1B22 384B 5E1B 1B12"
	$"0703 E200 0309 4E94 A8F4 D500 7EFA 0009"
	$"071B 2C7C 382C 2212 0707 A802 0007 F202"
	$"0007 BB02 0056 0702 0207 121B 2C52 B3A6"
	$"FF06 8D68 522C 1B12 07FE 0200 06FB 03FD"
	$"06F7 0CFC 06FC 0301 0603 FD02 0C07 0712"
	$"222C 3852 6822 221B 0702 DF00 0315 4E6F"
	$"A8F8 D500 94FA 000B 031B 4072 4538 2212"
	$"0707 0207 E502 0007 8102 F002 005C 0702"
	$"0207 121B 2C52 C1A7 FF07 FB98 6852 2C1B"
	$"1207 FE02 FC03 FE06 F00C 0106 06FB 0312"
	$"0602 0206 0707 121B 2238 455E 7C2C 2C22"
	$"1207 03FD 0000 03E3 000B 0309 3E6F A8C7"
	$"CFDC D5D5 BC0D FC00 0A09 75D0 D076 5238"
	$"221B 0707 D102 0007 8202 005D 0702 0207"
	$"121B 2C52 B3A7 FF0A FB98 6852 2C1B 1207"
	$"0603 06FE 03FE 06F9 0CFD 0EF8 0C01 0606"
	$"FB03 1102 0206 0707 121B 2238 455E 7245"
	$"382C 1B12 02DA 0006 0E31 3E6F 947E 02FE"
	$"000C 0900 53DC DCE4 874B 3822 2212 07A6"
	$"0200 07F3 0200 07BC 0200 5A07 0202 0712"
	$"1B2C 52B3 A7FF 07FB 9868 522C 1B12 07FC"
	$"0301 0606 FC0C F30E FC0C FE06 FD03 FE02"
	$"FE07 0D12 222C 3852 5E86 6845 382C 1B0D"
	$"02EC 0000 03ED 0014 0D07 0002 B6CF 611E"
	$"D5E1 D087 6838 2C1B 1207 0202 07E4 0200"
	$"0781 02F2 0200 5C07 0202 0712 1B2C 52B3"
	$"A7FF 07FB 9868 522C 1B12 07FD 0301 0606"
	$"FE0C FB0E F810 FC0E FE0C FE06 0303 0306"
	$"03FE 0213 0707 1212 222C 3852 5E7C 865E"
	$"5238 2212 0703 0003 DB00 1102 0700 21D5"
	$"D591 09A8 7552 4B7C 452C 1B12 07CF 0200"
	$"0784 0200 6307 0202 0712 1B2C 52B3 A6FF"
	$"068D 6852 2C1B 1207 FE03 0106 06FE 0CFE"
	$"0EFE 10FC 1600 1AFD 16FD 10FE 0EFE 0C01"
	$"0606 FE03 FD02 1107 0712 1B22 2C38 525E"
	$"5E7C 8652 452C 2C12 02DA 0011 0207 003E"
	$"D5D5 B607 070D 305E 6845 2C1B 1207 A402"
	$"0007 F302 0007 BE02 0066 0702 0207 121B"
	$"2C52 C1A7 FF0D FB98 6852 2C1B 1207 0303"
	$"0606 0C0C FE0E 0110 10F2 1605 1A16 1010"
	$"0E0E FE0C 0106 06FE 03FD 0212 0707 121B"
	$"222C 3845 5E68 7292 6845 382C 1B07 02DC"
	$"0003 0312 0053 FED5 0E21 032C 5268 724B"
	$"2C22 1207 0702 0207 E302 0007 8102 F402"
	$"0072 0702 0207 121B 2C52 B3A7 FF07 FB98"
	$"6852 2C1B 1207 FE06 050C 0C0E 0E10 10FD"
	$"1603 1A26 2624 FE26 0524 2624 2626 1AFD"
	$"1602 100E 0EFE 0C04 0603 0306 03FD 0213"
	$"0707 121B 222C 3845 525E 687C 7C5E 4538"
	$"2222 0D02 F200 0003 EE00 0302 1200 53FE"
	$"D50B 3222 4B72 7C72 452C 2212 0707 CF02"
	$"0007 8502 0072 0702 0207 121B 2C52 B3A7"
	$"FF0E FB98 6852 2C1B 1207 0606 0C0C 0E0E"
	$"10FD 160F 2624 1C1C 1D1C 271C 1C27 1C1C"
	$"271C 1C33 FD16 0610 0E0E 0C0C 0606 FE03"
	$"FD02 FE07 1212 121B 222C 3852 5E68 6886"
	$"8652 4538 2212 0703 E000 1203 1200 53D5"
	$"D5D0 405E 7C92 9268 4B2C 2212 0707 A302"
	$"0007 F402 0007 BF02 0077 0702 0207 121B"
	$"2C52 B3A7 FF24 FB98 6852 2C20 1207 060C"
	$"0C0E 0E10 1616 1A16 2616 205B A0A7 B2AB"
	$"AF9C 9CA7 9C8D 6D35 272D 26FE 1608 100E"
	$"0E0C 0C06 0603 03FB 02FE 0712 121B 222C"
	$"3845 525E 6872 867C 5245 3822 1207 03E2"
	$"0012 0312 006C DCDC BC4B 8192 A586 5E45"
	$"2C1B 1207 07FD 0200 07E3 0200 0781 02F6"
	$"0200 7807 0202 0712 1B2C 52B3 A6FF 188D"
	$"6852 3022 120D 060C 0C0E 1010 161A 1626"
	$"134D B3B2 824F 5036 FE33 1334 505D 8C89"
	$"6042 3526 1A16 1610 0E0E 0C0C 0603 03FB"
	$"02FD 0713 121B 1B22 2C38 4552 5E68 7C92"
	$"6852 5238 2212 0D02 F600 0003 F300 150E"
	$"0D12 1B2C 146C DCE4 874B 86B5 7C68 5E45"
	$"2C1B 1207 07CD 0200 0787 0200 7F07 0202"
	$"0712 1B2C 52C1 A7FF 31FB 985E 522C 1B12"
	$"0D0C 0C0E 0E10 161A 1A24 1D35 C188 3426"
	$"3336 3333 3633 3626 3326 3471 485B 5016"
	$"1A16 1610 0E0C 0C06 0306 03F9 0216 0707"
	$"1212 1B22 2C38 4552 5E68 687C 9268 5245"
	$"382C 120D 03F5 00FB 03FC 0015 0307 1B22"
	$"384B 2228 4028 2D7C 987C 7268 5238 221B"
	$"1207 A002 0007 F402 0007 C102 0085 0702"
	$"0207 121B 2C52 B3A7 FF16 FB98 5E45 2C20"
	$"120D 0D0C 0E10 1616 1A1A 3327 8996 2634"
	$"33F7 360F 2633 3548 8924 1C1A 1610 0E0E"
	$"0C06 0603 F802 FD07 1412 121B 222C 3845"
	$"525E 6868 7C86 6852 5238 2C22 1214 FC0D"
	$"2514 1412 1B1B 2222 2C22 221B 140D 0300"
	$"030D 1222 2C38 3812 0320 68AC BE92 7C72"
	$"5E4B 2C22 1207 07FB 0200 07E2 0200 0781"
	$"02F8 0200 8007 0202 0712 1B2C 45B3 A6FF"
	$"0B90 5245 2C1B 140D 0C0E 0E10 16FE 1A08"
	$"3435 985F 3436 3646 36FB 46FD 360E 484D"
	$"9034 1A1A 1616 100E 0C0C 0603 03F7 02FD"
	$"0710 121B 1B22 2C38 4552 5E68 6892 8668"
	$"5E52 45FE 3806 4538 3845 3845 45FE 52FE"
	$"5E1C 524B 2C22 0D03 030D 1B38 455E 7C86"
	$"92BA DCDC E4EA B052 5238 2C1B 1207 07CB"
	$"0200 0788 0200 8A08 0202 0707 1B22 4590"
	$"EED0 E8FB EEDF E815 DD84 4538 221B 120D"
	$"0C0E 1016 161A 2424 3435 7B5F 3636 FD46"
	$"0255 4655 FE46 FE36 0326 418D 34FE 1A07"
	$"1610 0E0C 0C06 0303 F502 FE07 0E12 121B"
	$"2222 2C38 4552 5E68 7286 A57C FB68 077C"
	$"7272 867C 8692 86FE 9201 8686 FE76 1775"
	$"ACA8 BAB0 B592 92BE C7CF DCE4 EAEA BE4B"
	$"452C 221B 1207 079E 0200 07F5 0200 07C2"
	$"0200 9F0D 0202 0707 1222 2C7C 818D 8698"
	$"9298 D6A0 0998 98A7 A7AF B3C1 A098 9DE6"
	$"A01E 989D 9886 8D72 4B2C 2214 110D 0E0E"
	$"1016 1A1A 2426 342B 4836 3333 3633 3633"
	$"36FE 3313 3633 3633 332D 3589 3426 241A"
	$"1616 0E0E 0C06 0306 F402 FD07 0812 121B"
	$"222C 3845 525E FE68 147C 9D92 929D A5A5"
	$"9DA5 B5A5 B5A5 B5A5 C0B5 A5A5 9DE3 FEEA"
	$"FEE4 12DC D9BA D9C2 D5E1 EAF1 F0BE 6852"
	$"382C 1B12 0707 F802 0007 E202 0007 8102"
	$"FA02 0085 0B02 0207 0712 1B22 2C38 4B5E"
	$"68D3 7206 B5C3 CAD3 DDEE 86E2 720E 685E"
	$"5245 3822 1B14 0D0C 0E10 1616 1AFE 2603"
	$"242D 2D0D F402 1009 021A 4173 3426 261A"
	$"1A16 100E 0C06 0303 F202 FD07 0812 121B"
	$"222C 3852 525E FE68 0972 9DC0 CACB DDEE"
	$"CB86 92FE 86FE 7C04 867C 92B0 D8FD EA12"
	$"E4DC D5D5 DCE4 EAF1 ECA5 5E52 452C 221B"
	$"1207 07C8 0200 078A 0200 82FE 0208 0707"
	$"121B 2238 4552 52D3 5E05 98A0 B3C5 DDCA"
	$"E15E 1852 5245 3822 1B14 110D 0C0E 1016"
	$"1A1A 2426 2413 2D35 1313 110C F811 110C"
	$"0C11 2748 5B4F 2624 241A 1610 0E0C 0C06"
	$"03F0 02FD 0714 121B 1B22 2C38 4552 525E"
	$"5E7C AFC1 CACA DDCB 727C 7CF7 7202 9DBE"
	$"F1FD EA0E E4EA F1F1 E386 685E 5238 2C1B"
	$"1207 079A 0200 07F5 0200 07C4 0200 7DFE"
	$"02FE 0704 121B 222C 2CD2 3805 8D8D B2A7"
	$"C3AF E038 152C 2C22 1B14 120D 0D0C 0E10"
	$"161A 2424 2602 2348 301D 11F3 130E 2D4C"
	$"4D68 3626 241A 1610 0E0C 0C06 03EE 02FE"
	$"0711 1212 1B1B 222C 3838 4552 5E98 B3C3"
	$"C1CA CB5E FE68 FA5E FE68 037C 7CB0 D8FE"
	$"EC0D E3E3 B072 6868 5245 2C22 1B12 0707"
	$"F602 0007 E102 0007 8102 FC02 0072 FD02"
	$"FE07 0012 FE1B D222 0681 899C 9CC1 A51B"
	$"E122 FE1B 1112 110D 0D0C 0E0E 1616 1A26"
	$"2436 1311 201D 16F1 130D 1D2B 8D36 2624"
	$"1A1A 160E 0E0C 0603 ED02 FD07 1C12 121B"
	$"1B22 2C2C 3838 8D98 B2B2 C3CC 3852 5245"
	$"454B 4545 4B45 4552 525E FA68 0C72 6868"
	$"725E 5245 3822 1B12 0707 C502 0007 8B02"
	$"006E FD02 FC07 D012 0674 7489 98CA 9802"
	$"DF12 FD07 0E0C 0C0E 0E16 1A1A 2426 3627"
	$"1311 1311 F313 FE11 0C3B B236 2624 241A"
	$"1610 0E0C 0606 EA02 FE07 0E12 121B 1B22"
	$"221B 8D89 9CA7 C5D3 2238 F82C 0638 384B"
	$"5252 5E5E FC68 0A5E 5E52 4538 2C22 1212"
	$"0707 F202 0007 A702 0007 F602 0007 C502"
	$"0066 FB02 CD07 066D 6D7B 9BC1 6802 DD07"
	$"FE06 0E0C 0E10 161A 1A26 2636 2D30 2D20"
	$"1D13 F511 1113 1D27 2D67 A736 3324 2416"
	$"1610 0E0C 0603 03E9 02FD 070A 1212 1B07"
	$"8173 8D9C B2CB 07FE 22FB 1BFE 2202 2C38"
	$"38FE 4BFD 5209 4B45 3838 2222 1B12 0707"
	$"D102 0007 8102 FE02 006B C702 066B 737B"
	$"90C3 4B03 F002 0212 1B12 F102 2F03 0306"
	$"0C0E 1016 1A24 2426 362D 3548 4841 3541"
	$"3541 3541 3541 3541 3541 414C 4C48 4867"
	$"A736 3326 241A 1610 0E0C 0603 03E7 02FD"
	$"0708 1202 5E73 898D A7C0 07F6 1205 1B1B"
	$"2222 2C2C FB38 052C 2C22 1B1B 12FE 07C2"
	$"0200 078D 0200 69C7 0205 5B73 7B9B B330"
	$"F102 070D 1429 292B 231D 0DF4 020D 0303"
	$"060C 0E10 161A 2426 2636 2D42 ED48 0D6A"
	$"A746 3324 2416 1610 0E0C 0603 03F7 0204"
	$"0711 231B 07F4 02FE 0707 0243 677B 89A7"
	$"CA00 F607 0112 12FE 1BF9 2203 1B1B 1212"
	$"FE07 F002 0007 A502 0007 F602 0007 C702"
	$"006A C702 054D 6D7B 9CC3 14F3 020C 0620"
	$"3B4D 5B64 645B 5B4A 2913 03F7 020D 0303"
	$"060C 0E10 161A 2426 3636 2D35 ED48 0D67"
	$"A746 3626 241A 1610 0E0C 0603 06FA 0209"
	$"0723 2C39 3B42 4A42 430D F202 0639 6473"
	$"899C C002 FD07 FD02 FB07 0112 12F9 1B01"
	$"1212 FD07 CD02 0007 8102 0002 006A C702"
	$"055B 6D7B 98B3 12F5 0205 071D 304B 6773"
	$"FE7B 0874 7B6D 6D5B 4A23 1406 F802 0B03"
	$"060C 0E16 161A 2626 362D 35ED 480C 67B3"
	$"4633 2624 1A16 1010 0C06 03FC 020E 0611"
	$"2739 3B4D 6467 7373 6464 5B30 07F4 0205"
	$"1B5B 737B 9CC3 F402 FC07 FB12 FB07 EB02"
	$"0007 D502 0007 8E02 006D C702 045B 737B"
	$"A7C3 F502 090D 293B 677B 7B89 7B7B 89FE"
	$"7B08 736D 6D5B 4D38 3020 1BFE 1400 1DFB"
	$"1C07 2426 3333 3636 2D42 ED48 0B67 A736"
	$"3324 241A 1610 0E0C 06FE 0309 0611 2729"
	$"394A 5B6D 7489 FD7B 0481 7364 4D14 F702"
	$"0707 0207 6D73 7B9C 98F2 02F5 078C 0200"
	$"07F7 0200 07C8 0200 5EC7 0204 5B6D 7BA7"
	$"B3F6 0205 1430 4B73 7489 F67B 0A73 7367"
	$"5B5B 4B4D 4A5B 4A4D FE4A FE4D 064F 5F70"
	$"7D5F 4135 ED48 0A67 B25F 5F50 3641 2D41"
	$"3535 FE29 0B35 3038 4D5B 6784 7B8F 7B7B"
	$"89FC 7B03 736D 7323 F502 0564 6D7B B298"
	$"03AD 0200 0782 0200 5BC7 0204 5B73 7B9C"
	$"74F8 0204 071D 3B5B 73F4 7B00 89FE 7B03"
	$"817B 7373 FE6D FE73 0470 7B71 808C FD7D"
	$"0126 41ED 4813 67A7 7D5F 5F6A 5F67 605A"
	$"605B 5B64 5B67 7373 8189 F57B 0474 6D67"
	$"3811 F702 044B 7389 9CAF CD02 0007 D302"
	$"0007 9002 0055 C802 0603 5B6D 7B89 6D0D"
	$"FC02 0607 0D14 2942 7373 ED7B 0089 FD7B"
	$"0484 7B7B 827B FD82 048A 7D7D 3435 ED48"
	$"0560 B27D 8A8A 7DFD 8204 7B84 7B7B 89EF"
	$"7B04 8973 7B5B 14F8 0204 4260 7B9C AF81"
	$"02EF 0200 07F7 0200 07CA 0200 5FC7 0211"
	$"5B73 7B7B 735B 3022 2922 2929 3042 546D"
	$"7389 EC7B 0784 8489 848F 898F 7BFE 8C06"
	$"978A 978A 7D41 35ED 4805 64B2 8A7D 8A8A"
	$"FE82 027B 827F F37B 0084 FD7B 1489 7B89"
	$"7B73 6D5B 3027 1111 0302 0207 145A 6D7B"
	$"AF9C CA02 0007 E202 0007 8402 005F C702"
	$"065B 6D7B 7B73 736D FD5B 036D 7374 89F7"
	$"7B00 89F9 7B17 8989 9CB3 B3C1 CAB3 C3B3"
	$"C5B3 C3C1 C3C5 C5D4 C8C8 A434 3550 EE48"
	$"0D60 A7A3 9397 97A4 979C 9F90 8F8F 89F8"
	$"7B00 89F5 7B10 7373 6D5B 4A42 4C3B 3942"
	$"4D64 737B A7A5 039E 0200 0791 0200 68C8"
	$"0213 036D 898D 9089 898D 9B8D A7B2 B3C3"
	$"C3B3 B39C 8989 F47B 0D89 98CA D3E8 DDDD"
	$"B5AF B5AF B5B3 AFFE B307 9C7B 8A97 8A8A"
	$"4235 ED48 1160 B2D6 C8C8 DBC5 C3CA CEDD"
	$"D3CB C3B3 A79C 89F0 7B10 898D 9090 8474"
	$"6D6D 646D 646D 737B 7BB2 C181 02ED 0200"
	$"07F8 0200 07CB 0200 6CC7 0205 6BC1 D3CA"
	$"D3E8 FEDD 0BE8 D3CA B39D 98B3 C3A7 897B"
	$"89F8 7B1A 898D C1DD DD98 3812 0203 0302"
	$"0303 0603 0C0E 0510 051C 1C26 3426 42ED"
	$"4803 64A7 6A7D FE82 017F 71FD 6D06 89B3"
	$"CACA B290 89F4 7B0C 89B2 D3CB DDD3 DDCA"
	$"C1C3 B2A7 A7FE 9C02 C1CB CBCA 0200 07E0"
	$"0200 0786 0200 71C6 0209 2972 6D7C 745E"
	$"4B38 1403 FC02 0422 8DB3 9889 FD7B 0089"
	$"FE7B 058D B2DD E88D 13F8 020D 0303 070C"
	$"0D10 1C1C 241C 362D 4234 EF48 0850 60B3"
	$"361A 1A05 050D FE03 FD02 060D 4B98 C0A7"
	$"8D89 FC7B 0089 FE7B 1484 A0C1 EEB3 6852"
	$"526B 7286 9DC3 CAE8 DDEE E8EE CA45 C702"
	$"0007 D602 0007 9302 0065 C002 0003 F802"
	$"0F03 5ECA C3B2 B29C 8D9C 8DB2 C1E8 E8B5"
	$"1DF6 020C 0303 060C 0E10 101A 2424 262D"
	$"34ED 4805 89CA 2626 1A1C FE10 000D FE03"
	$"FC02 062C B2C1 A790 8989 FD7B 1589 98B2"
	$"D3DD 4B02 0203 0200 0203 0307 1214 1B12"
	$"1214 0381 02EA 0200 07F8 0200 07CD 0200"
	$"56B4 0203 1481 B3E8 FEDD 06EE E8CB 8D4B"
	$"0203 F602 0D03 0306 0C0E 0E10 101A 1A24"
	$"4864 5BFA 4801 4234 FE48 014D 34FE 480E"
	$"5089 C09C 261A 1A10 100E 0E0C 0603 03FB"
	$"020E 075E C1D3 C1B3 B3B2 B3C1 CADD EDAF"
	$"2C94 0200 0788 0200 55B2 0209 030D 222C"
	$"2022 2014 0203 F402 1103 0307 0C0C 0E10"
	$"1616 241A 347B 9889 8973 73FD 6000 73FE"
	$"6013 7350 7B89 89B3 CBCB 3424 1A1A 1010"
	$"0E0C 0C06 0303 F902 0A0D 5292 CACC DDCB"
	$"E8C0 8629 B302 0007 D502 0007 9402 0040"
	$"AF02 0003 EE02 1103 0306 060C 0E0E 1016"
	$"1A24 1A34 829C C3CB CCF6 CB07 DDCB B37B"
	$"3426 1A1A FE10 050E 0C06 0603 03F5 0203"
	$"0302 0200 8102 D202 0007 F902 0007 CE02"
	$"0038 9B02 FE03 0C06 0C0C 0E10 1610 1A1A"
	$"2405 3434 FE5F 0050 FD5F 007D FE50 075F"
	$"5034 3626 241A 1AFE 1005 0E0C 0C06 0303"
	$"9C02 0007 E202 0007 8A02 0035 9B02 FE03"
	$"0B07 060C 0C0E 1016 101A 1C26 24FD 2608"
	$"3636 3434 2626 3636 33FE 2600 24FE 1AFE"
	$"1006 0E0C 0C06 0603 0381 02F0 0200 0796"
	$"0200 2F9A 02FE 0300 06FE 0C02 0E10 16FC"
	$"1AFE 24F9 2607 2426 261A 241A 1A05 FE10"
	$"FE0C 0006 FE03 8102 BF02 0007 F902 0007"
	$"D002 002D 9A02 FE03 0107 06FE 0C00 0EFD"
	$"10FD 1A03 241A 2424 FD26 FB1A 0105 0DFE"
	$"10FE 0C00 06FD 039C 0200 07E0 0200 078C"
	$"0200 2999 02FD 0300 06FE 0C06 0E0E 1010"
	$"1616 10F5 1A02 1010 13FE 1000 0DFE 0C01"
	$"0606 FE03 8102 EE02 0007 9702 002F 9902"
	$"FD03 0106 06FE 0C02 0E10 0EFC 1000 1AFC"
	$"1000 1AFA 1001 0E10 FE0C 0106 06FE 0397"
	$"0200 07A6 0200 07FA 0200 07D1 0200 208F"
	$"02FE 0300 07FC 0CFE 1001 0E0E F710 FC0E"
	$"FE0C 0106 06FC 0381 02FE 0200 078E 0200"
	$"1E8F 02FC 03FE 06FC 0C04 0E0E 100E 10FA"
	$"0EFC 0CFE 06FC 0381 02F2 0200 0799 0200"
	$"1A8D 02FC 03FD 06F0 0CFE 06FC 039C 0200"
	$"07A4 0200 07FA 0200 07D3 0200 148C 02F9"
	$"0300 07F3 06F9 0397 0200 07E2 0200 0790"
	$"0200 0C88 02E9 0381 02EA 0200 079A 0200"
	$"1083 02F3 0381 02B2 0200 07FB 0200 07D4"
	$"0200 0C81 0283 0200 07E0 0200 0792 0200"
	$"0A81 0281 02D7 0200 079C 0200 1181 0281"
	$"0201 0207 A602 0007 FB02 0007 D602 000A"
	$"8102 8102 DF02 0007 9402 000A 8102 8102"
	$"D602 0007 9D02 0011 8102 8102 0102 07A4"
	$"0200 07FC 0200 07D7 0200 0A81 0281 02DD"
	$"0200 0796 0200 0E81 0281 02FD 0200 07D9"
	$"0200 079F 0200 0E81 0281 02A0 0200 07FC"
	$"0200 07D9 0200 0A81 0281 02DB 0200 0798"
	$"0200 0E81 0281 02FD 0200 07D8 0200 07A0"
	$"0200 1281 0281 02FA 0200 07A6 0200 07FD"
	$"0200 07DA 0200 0A81 0281 02D9 0200 079A"
	$"0200 0A81 0281 02D1 0200 07A2 0200 1281"
	$"0281 02FA 0200 07A4 0200 07FD 0200 07DC"
	$"0200 0A81 0281 02D7 0200 079C 0200 0E81"
	$"0281 02F7 0200 07DB 0200 07A3 0200 0E81"
	$"0281 029A 0200 07FE 0200 07DD 0200 0A81"
	$"0281 02D5 0200 079E 0200 0E81 0281 02F7"
	$"0200 07D9 0200 07A5 0200 0881 0281 0281"
	$"02F1 0200 0E81 0281 02F4 0200 07E1 0200"
	$"07A0 0200 0A81 0281 02CD 0200 07A6 0200"
	$"0881 0281 0281 02F1 0200 0E81 0281 02F4"
	$"0200 07DF 0200 07A2 0200 0A81 0281 02F1"
	$"0200 0782 0200 0881 0281 0281 02F1 0200"
	$"0881 0281 0281 02F1 0200 0A81 0281 02F1"
	$"0200 0782 0200 0881 0281 0281 02F1 0200"
	$"0A81 0281 02EE 0200 0785 0200 0881 0281"
	$"0281 02F1 0200 0881 0281 0281 02F1 0200"
	$"0A81 0281 02EE 0200 0785 0200 0881 0281"
	$"0281 02F1 0200 0A81 0281 02EB 0200 0788"
	$"0200 0881 0281 0281 02F1 0200 0881 0281"
	$"0281 02F1 0200 0A81 0281 02EB 0200 0788"
	$"0200 0A81 0281 02E8 0200 078B 0200 0881"
	$"0281 0281 02F1 0200 0881 0281 0281 02F1"
	$"0200 0A81 0281 02E8 0200 078B 0200 0881"
	$"0281 0281 02F1 0200 0A81 0281 02E5 0200"
	$"078E 0200 0881 0281 0281 02F1 0200 0881"
	$"0281 0281 02F1 0200 0A81 0281 02E5 0200"
	$"078E 0200 0881 0281 0281 02F1 0200 0A81"
	$"0281 02E2 0200 0791 0200 0881 0281 0281"
	$"02F1 0200 0881 0281 0281 02F1 0200 0A81"
	$"0281 02E2 0200 0791 0200 00FF"
};

resource 'PICT' (2048) {
	102,
	{-1, -1, 3, 219},
	$"0011 02FF 0C00 FFFF FFFF FFFF 0000 FFFF"
	$"0000 00DB 0000 0003 0000 0000 0000 0001"
	$"000A FFFF FFFF 0003 00DB 0007 0000 0000"
	$"0008 000A 0022 0024 0017 0000 00A1 00B6"
	$"0004 0001 0001 0007 0001 0001 0008 0008"
	$"0020 0000 00D8 0000 0000 00FF"
};

data 'PRCT' (1000, "Keypad") {
	$"0004 0004 001E 001E 0004 0024 001E 003E"            /* ...........$...> */
	$"0004 0044 001E 005E 0024 0004 003E 001E"            /* ...D...^.$...>.. */
	$"0024 0024 003E 003E 0024 0044 003E 005E"            /* .$.$.>.>.$.D.>.^ */
	$"0044 0024 005E 003E 0064 0004 007E 001E"            /* .D.$.^.>.d...~.. */
	$"0064 0024 007E 003E 0064 0044 007E 005E"            /* .d.$.~.>.d.D.~.^ */
	$"0004 006E 001E 0088 0004 008E 001E 00A8"            /* ...n...à...é...® */
	$"0004 00AE 001E 00C8 0004 00CE 001E 00E8"            /* ...Æ...»...Œ...Ë */
	$"0024 006E 003E 0088 0024 008E 003E 00A8"            /* .$.n.>.à.$.é.>.® */
	$"0024 00AE 003E 00C8 0024 00CE 003E 00E8"            /* .$.Æ.>.».$.Œ.>.Ë */
	$"0044 006E 005E 0088 0044 008E 005E 00A8"            /* .D.n.^.à.D.é.^.® */
	$"0044 00AE 005E 00C8 0044 00CE 005E 00E8"            /* .D.Æ.^.».D.Œ.^.Ë */
	$"0064 006E 007E 0088 0064 008E 007E 00A8"            /* .d.n.~.à.d.é.~.® */
	$"0064 00AE 007E 00C8 0064 00CE 009E 00E8"            /* .d.Æ.~.».d.Œ.û.Ë */
	$"0084 006E 009E 00A8 0084 00AE 009E 00C8"            /* .Ñ.n.û.®.Ñ.Æ.û.» */
};

data 'PRCT' (1002, "Function") {
	$"0004 0004 001E 001E 0004 0024 001E 003E"            /* ...........$...> */
	$"0004 0044 001E 005E 0004 0064 001E 007E"            /* ...D...^...d...~ */
	$"0004 0084 001E 009E 0024 0004 003E 001E"            /* ...Ñ...û.$...>.. */
	$"0024 0024 003E 003E 0024 0044 003E 005E"            /* .$.$.>.>.$.D.>.^ */
	$"0024 0064 003E 007E 0024 0084 003E 009E"            /* .$.d.>.~.$.Ñ.>.û */
	$"0044 0004 005E 001E 0044 0024 005E 003E"            /* .D...^...D.$.^.> */
	$"0044 0044 005E 005E 0044 0064 005E 007E"            /* .D.D.^.^.D.d.^.~ */
	$"0044 0084 005E 009E"                                /* .D.Ñ.^.û */
};

data 'PrEf' (1990, "Application Prefs Master ") {
	$"0002 0000 0004 0000 0000 7474 7874 0001"            /* ..........ttxt.. */
	$"0100 0101 000F 000F 000A 0000 0000 0000"            /* .........¬...... */
	$"0000 0000 0000 0001 0001 000F 0000 0000"            /* ................ */
	$"0100 0000 0000 0078 0000 0000 0000 0000"            /* .......x........ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000"                                /* ........ */
};

resource 'SICN' (128) {
	{	/* array: 1 elements */
		/* [1] */
		$"0000 7FFE 8001 BFFD BFFD B58D A08D B58D"
		$"A08D B58D BFFD BFFD BFFD 8001 7FFE"
	}
};

resource 'STR ' (128) {
	"The application “MacSSH” requires your a"
	"ttention. Please bring it to the front."
};

resource 'STR ' (1991, "Preferences File Name", purgeable) {
	"MacSSH Prefs"
};

resource 'STR#' (2002, "Misc Strings", purgeable) {
	{	/* array StringArray: 42 elements */
		/* [1] */
		"Capture File",
		/* [2] */
		"New Session",
		/* [3] */
		"New Terminal",
		/* [4] */
		"Use username=",
		/* [5] */
		", password=",
		/* [6] */
		"Close All",
		/* [7] */
		"Close",
		/* [8] */
		"Please Select New Color",
		/* [9] */
		"None",
		/* [10] */
		"Session:",
		/* [11] */
		"Ok",
		/* [12] */
		"Cancel",
		/* [13] */
		"FTP Log",
		/* [14] */
		"Save Macros To:",
		/* [15] */
		"MacSSH Macros",
		/* [16] */
		"awaiting dismissal",
		/* [17] */
		"being opened",
		/* [18] */
		"being looked up",
		/* [19] */
		"Session Name:",
		/* [20] */
		"Set to load:",
		/* [21] */
		"Save Set to:",
		/* [22] */
		"MacSSH Set",
		/* [23] */
		"Save captured text as:",
		/* [24] */
		"Previous Session",
		/* [25] */
		"Next Session",
		/* [26] */
		"Rebuilding Font Menu...",
		/* [27] */
		"Initializing Menus...",
		/* [28] */
		"Initializing Terminal...",
		/* [29] */
		"Favorites",
		/* [30] */
		"Terminals",
		/* [31] */
		"FTP Users",
		/* [32] */
		"Export Public Key as:",
		/* [33] */
		"identity.ssh2.pub",
		/* [34] */
		"MacSSH Macros",
		/* [35] */
		"Cache passphrase",
		/* [36] */
		"Cache passphrase for",
		/* [37] */
		"MacSSH",
		/* [38] */
		"Enter passphrase for private key ",
		/* [39] */
		"Show Log",
		/* [40] */
		"Hide Log",
		/* [41] */
		"Command:",
		/* [42] */
		"lsh arguments:"
	}
};

resource 'STR#' (2004, "DNR Error Messages", purgeable) {
	{	/* array StringArray: 9 elements */
		/* [1] */
		"of a general DNR error.",
		/* [2] */
		"the MacTCP DNR is out of memory.",
		/* [3] */
		"the domain name server returned an error"
		".",
		/* [4] */
		"there was no answer from any domain name"
		" server.",
		/* [5] */
		"the domain name is invalid.",
		/* [6] */
		"there is no default name server.",
		/* [7] */
		"of an internal error.",
		/* [8] */
		"of an internal error.",
		/* [9] */
		"the domain name given was invalid."
	}
};

resource 'STR#' (2008, "Port Numbers", purgeable) {
	{	/* array StringArray: 12 elements */
		/* [1] */
		"22",
		/* [2] */
		"23",
		/* [3] */
		"25",
		/* [4] */
		"80",
		/* [5] */
		"109",
		/* [6] */
		"110",
		/* [7] */
		"119",
		/* [8] */
		"143",
		/* [9] */
		"194",
		/* [10] */
		"220",
		/* [11] */
		"407",
		/* [12] */
		"514"
	}
};

resource 'STR#' (4000, "Good Fonts", purgeable) {
	{	/* array StringArray: 0 elements */
	}
};

resource 'STR#' (5000, "Bad Fonts", purgeable) {
	{	/* array StringArray: 0 elements */
	}
};

resource 'STR#' (7002, "FTP Server Prefs Strings", purgeable) {
	{	/* array StringArray: 11 elements */
		/* [1] */
		"-1",
		/* [2] */
		"",
		/* [3] */
		"",
		/* [4] */
		"Off",
		/* [5] */
		"On, No passwords needed",
		/* [6] */
		"On, Username & Password required",
		/* [7] */
		"Show FTP log at startup",
		/* [8] */
		"Lookup DNS name of connecting hosts",
		/* [9] */
		"Use Macbinary II",
		/* [10] */
		"Reset Macbinary after each transfer",
		/* [11] */
		"Use ISO Translation"
	}
};

resource 'STR#' (7005, "FTP User Config Strings", purgeable) {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"-1",
		/* [2] */
		"",
		/* [3] */
		"",
		/* [4] */
		"User can change directories"
	}
};

resource 'STR#' (7100, "Key Names") {
	{	/* array StringArray: 120 elements */
		/* [1] */
		"\0x110",
		/* [2] */
		"\0x111",
		/* [3] */
		"\0x112",
		/* [4] */
		"\0x113",
		/* [5] */
		"\0x114",
		/* [6] */
		"\0x115",
		/* [7] */
		"\0x116",
		/* [8] */
		"\0x117",
		/* [9] */
		"\0x118",
		/* [10] */
		"\0x119",
		/* [11] */
		"Shift-\0x110",
		/* [12] */
		"Shift-\0x111",
		/* [13] */
		"Shift-\0x112",
		/* [14] */
		"Shift-\0x113",
		/* [15] */
		"Shift-\0x114",
		/* [16] */
		"Shift-\0x115",
		/* [17] */
		"Shift-\0x116",
		/* [18] */
		"Shift-\0x117",
		/* [19] */
		"Shift-\0x118",
		/* [20] */
		"Shift-\0x119",
		/* [21] */
		"F1",
		/* [22] */
		"F2",
		/* [23] */
		"F3",
		/* [24] */
		"F4",
		/* [25] */
		"F5",
		/* [26] */
		"F6",
		/* [27] */
		"F7",
		/* [28] */
		"F8",
		/* [29] */
		"F9",
		/* [30] */
		"F10",
		/* [31] */
		"Shift-F1",
		/* [32] */
		"Shift-F2",
		/* [33] */
		"Shift-F3",
		/* [34] */
		"Shift-F4",
		/* [35] */
		"Shift-F5",
		/* [36] */
		"Shift-F6",
		/* [37] */
		"Shift-F7",
		/* [38] */
		"Shift-F8",
		/* [39] */
		"Shift-F9",
		/* [40] */
		"Shift-F10",
		/* [41] */
		"F11",
		/* [42] */
		"F12",
		/* [43] */
		"F13",
		/* [44] */
		"F14",
		/* [45] */
		"F15",
		/* [46] */
		"Shift-F11",
		/* [47] */
		"Shift-F12",
		/* [48] */
		"Shift-F13",
		/* [49] */
		"Shift-F14",
		/* [50] */
		"Shift-F15",
		/* [51] */
		"PF1 (Clear)",
		/* [52] */
		"PF2 (=)",
		/* [53] */
		"PF3 (/)",
		/* [54] */
		"PF4 (*)",
		/* [55] */
		"PgUp",
		/* [56] */
		"PgDown",
		/* [57] */
		"Home",
		/* [58] */
		"End",
		/* [59] */
		"Help",
		/* [60] */
		"Del",
		/* [61] */
		"Shift-PF1 (Clear)",
		/* [62] */
		"Shift-PF2 (=)",
		/* [63] */
		"Shift-PF3 (/)",
		/* [64] */
		"Shift-PF4 (*)",
		/* [65] */
		"Shift-PgUp",
		/* [66] */
		"Shift-PgDown",
		/* [67] */
		"Shift-Home",
		/* [68] */
		"Shift-End",
		/* [69] */
		"Shift-Help",
		/* [70] */
		"Shift-Del",
		/* [71] */
		"0",
		/* [72] */
		"1",
		/* [73] */
		"2",
		/* [74] */
		"3",
		/* [75] */
		"4",
		/* [76] */
		"5",
		/* [77] */
		"6",
		/* [78] */
		"7",
		/* [79] */
		"8",
		/* [80] */
		"9",
		/* [81] */
		"-",
		/* [82] */
		"+",
		/* [83] */
		".",
		/* [84] */
		"Enter (AM)",
		/* [85] */
		"Enter (!AM)",
		/* [86] */
		"",
		/* [87] */
		"",
		/* [88] */
		"",
		/* [89] */
		"",
		/* [90] */
		"",
		/* [91] */
		"Up",
		/* [92] */
		"Down",
		/* [93] */
		"Left",
		/* [94] */
		"Right",
		/* [95] */
		"",
		/* [96] */
		"Up (CKM)",
		/* [97] */
		"Down (CKM)",
		/* [98] */
		"Left (CKM)",
		/* [99] */
		"Right (CKM)",
		/* [100] */
		"",
		/* [101] */
		"F1",
		/* [102] */
		"F2",
		/* [103] */
		"F3",
		/* [104] */
		"F4",
		/* [105] */
		"F5",
		/* [106] */
		"F6",
		/* [107] */
		"F7",
		/* [108] */
		"F8",
		/* [109] */
		"F9",
		/* [110] */
		"F10",
		/* [111] */
		"F1",
		/* [112] */
		"F2",
		/* [113] */
		"F3",
		/* [114] */
		"F4",
		/* [115] */
		"F5",
		/* [116] */
		"F6",
		/* [117] */
		"F7",
		/* [118] */
		"F8",
		/* [119] */
		"F9",
		/* [120] */
		"F10"
	}
};

resource 'STR#' (23227, "Server info", purgeable) {
	{	/* array StringArray: 40 elements */
		/* [1] */
		"220 Macintosh Resident FTP server, ready"
		"\n\r",
		/* [2] */
		"451 Error in processing list command\n\r",
		/* [3] */
		"221 Goodbye\n\r",
		/* [4] */
		"200 This space intentionally left blank "
		"<   >\n\r",
		/* [5] */
		"150 Opening connection\n\r",
		/* [6] */
		"226 Transfer complete\n\r",
		/* [7] */
		"200 Type set to A, ASCII transfer mode\n\r",
		/* [8] */
		"200 Type set to I, binary transfer mode\n"
		"\r",
		/* [9] */
		"500 Command not understood\n\r",
		/* [10] */
		"200 Okay\n\r",
		/* [11] */
		"230 User logged in\n\r",
		/* [12] */
		"550 File not found\n\r",
		/* [13] */
		"501 Directory not present or syntax erro"
		"r\n\r",
		/* [14] */
		"250 Chdir okay\n\r",
		/* [15] */
		"257 \"",
		/* [16] */
		"\" is the current directory\n\r",
		/* [17] */
		"501 File not found\n\r",
		/* [18] */
		"504 Parameter not accepted, not implemen"
		"ted\n\r",
		/* [19] */
		"200 Stru F, file structure\n\r",
		/* [20] */
		"200 Mode S, stream mode\n\r",
		/* [21] */
		"202 Allocate and Account not required fo"
		"r this server\n\r",
		/* [22] */
		"501 Cannot open file to write, check for"
		" valid name\n\r",
		/* [23] */
		"530 USER and PASS required to activate m"
		"e\n\r",
		/* [24] */
		"331 Password required\n\r",
		/* [25] */
		"530 Login failed\n\r",
		/* [26] */
		"200 MacBinary Mode enabled\n\r",
		/* [27] */
		"200 MacBinary Mode disabled\n\r",
		/* [28] */
		"552 Disk write error, probably disk full"
		"\n\r",
		/* [29] */
		"214-NCSA Telnet FTP server, supported co"
		"mmands:\n\r",
		/* [30] */
		"    USER  PORT  RETR  ALLO  PASS  STOR  "
		"CWD  XCWD  XPWD  LIST  NLST\n\r",
		/* [31] */
		"    HELP  QUIT  MODE  TYPE  STRU  ACCT  "
		"NOOP  APPE  MACB\n\r",
		/* [32] */
		"    MACB is MacBinary and must be done w"
		"ith TYPE I\n\r",
		/* [33] */
		"214 Direct comments and bugs to rbraun@g"
		"eocities.com\n\r",
		/* [34] */
		"200 Type set to I, binary transfer mode "
		"[MACBINARY ENABLED]\n\r",
		/* [35] */
		"200 Type set to I, binary transfer mode "
		"[macbinary disabled]\n\r",
		/* [36] */
		"505 APPEND only supported in ASCII trans"
		"fer mode\n\r",
		/* [37] */
		"200 Directory created\n\r",
		/* [38] */
		"501 Directory present or syntax error\n\r",
		/* [39] */
		"200 Directory deleted\n\r",
		/* [40] */
		"501 Directory not present or contains en"
		"tries or syntax error\n\r"
	}
};

resource 'STR#' (23228, "ftp cmds", purgeable) {
	{	/* array StringArray: 51 elements */
		/* [1] */
		"?",
		/* [2] */
		"!",
		/* [3] */
		"acct",
		/* [4] */
		"ascii",
		/* [5] */
		"bell",
		/* [6] */
		"bget",
		/* [7] */
		"binary",
		/* [8] */
		"bput",
		/* [9] */
		"bye",
		/* [10] */
		"cd",
		/* [11] */
		"close",
		/* [12] */
		"delete",
		/* [13] */
		"debug",
		/* [14] */
		"dir",
		/* [15] */
		"get",
		/* [16] */
		"glob",
		/* [17] */
		"hash",
		/* [18] */
		"help",
		/* [19] */
		"interactive",
		/* [20] */
		"lcd",
		/* [21] */
		"ldir",
		/* [22] */
		"lls",
		/* [23] */
		"ls",
		/* [24] */
		"macb",
		/* [25] */
		"mdelete",
		/* [26] */
		"mdir",
		/* [27] */
		"mget",
		/* [28] */
		"mkdir",
		/* [29] */
		"mls",
		/* [30] */
		"mode",
		/* [31] */
		"mput",
		/* [32] */
		"noninteractive",
		/* [33] */
		"open",
		/* [34] */
		"prompt",
		/* [35] */
		"put",
		/* [36] */
		"pwd",
		/* [37] */
		"quit",
		/* [38] */
		"quote",
		/* [39] */
		"recv",
		/* [40] */
		"remotehelp",
		/* [41] */
		"rename",
		/* [42] */
		"rm",
		/* [43] */
		"rmdir",
		/* [44] */
		"send",
		/* [45] */
		"sendport",
		/* [46] */
		"slashflip",
		/* [47] */
		"status",
		/* [48] */
		"struct",
		/* [49] */
		"type",
		/* [50] */
		"user",
		/* [51] */
		"verbose"
	}
};

resource 'STR#' (23229, "Help", purgeable) {
	{	/* array StringArray: 51 elements */
		/* [1] */
		"?\t\tprint local help information",
		/* [2] */
		"!\t\tescape to the shell",
		/* [3] */
		"acct       specify account",
		/* [4] */
		"ascii     \tset ascii transfer type",
		/* [5] */
		"bell      \tbeep when command completed -"
		" toggle",
		/* [6] */
		"bget\t\tget a file in binary mode",
		/* [7] */
		"binary    \tset binary transfer type",
		/* [8] */
		"bput\t\tput a file in binary mode",
		/* [9] */
		"bye       \tterminate ftp session and exi"
		"t",
		/* [10] */
		"cd        \tchange remote working directo"
		"ry",
		/* [11] */
		"close     \tterminate ftp session",
		/* [12] */
		"delete    \tdelete remote file - inquires"
		" if prompting is on",
		/* [13] */
		"debug     \ttoggle/set debugging mode",
		/* [14] */
		"dir       \tlist contents of remote direc"
		"tory",
		/* [15] */
		"get       \treceive file",
		/* [16] */
		"glob      \ttoggle metacharacter expansio"
		"n of local file names",
		/* [17] */
		"hash      \ttoggle printing `#' for each "
		"buffer transferred",
		/* [18] */
		"help      \tprint local help information",
		/* [19] */
		"interactive    turn on prompting for mul"
		"tiple commands",
		/* [20] */
		"lcd       \tchange local working director"
		"y",
		/* [21] */
		"ldir\t\tlist contents of local directory",
		/* [22] */
		"lls\t\tlist contents of local directory",
		/* [23] */
		"ls        \tnlist contents of remote dire"
		"ctory",
		/* [24] */
		"macb\t\tenable/disable macbinary mode",
		/* [25] */
		"mdelete   \tdelete multiple files",
		/* [26] */
		"mdir      \tlist contents of multiple rem"
		"ote directories",
		/* [27] */
		"mget      \tget multiple files",
		/* [28] */
		"mkdir     \tmake directory on the remote "
		"machine",
		/* [29] */
		"mls       \tnlist contents of multiple re"
		"mote directories",
		/* [30] */
		"mode      \tset file transfer mode",
		/* [31] */
		"mput      \tsend multiple files",
		/* [32] */
		"noninteractive turn off prompting on mul"
		"tiple commands",
		/* [33] */
		"open      \tconnect to remote tftp",
		/* [34] */
		"prompt    \ttoggle interactive prompting "
		"on multiple commands",
		/* [35] */
		"put       \tsend one file",
		/* [36] */
		"pwd       \tprint working directory on re"
		"mote machine",
		/* [37] */
		"quit      \tterminate ftp session and exi"
		"t",
		/* [38] */
		"quote     \tsend arbitrary ftp command",
		/* [39] */
		"recv      \treceive file",
		/* [40] */
		"remotehelp\tget help from remote server",
		/* [41] */
		"rename    \trename file",
		/* [42] */
		"rm\t\tremove file",
		/* [43] */
		"rmdir     \tremove directory on the remot"
		"e machine",
		/* [44] */
		"send      \tsend one file",
		/* [45] */
		"sendport  \ttoggle use of PORT cmd for ea"
		"ch data connection",
		/* [46] */
		"slashflip\ttoggle changing / to \\ on outg"
		"oing commands",
		/* [47] */
		"status    \tshow current status",
		/* [48] */
		"struct    \tset file transfer structure",
		/* [49] */
		"type      \tset file transfer type",
		/* [50] */
		"user      \tsend new user information",
		/* [51] */
		"verbose   \ttoggle verbose mode"
	}
};

resource 'STR#' (23240, "Affirmatives", purgeable) {
	{	/* array StringArray: 13 elements */
		/* [1] */
		"yes",
		/* [2] */
		"y",
		/* [3] */
		"on",
		/* [4] */
		"active",
		/* [5] */
		"true",
		/* [6] */
		"alive",
		/* [7] */
		"yup",
		/* [8] */
		"yep",
		/* [9] */
		"yessir",
		/* [10] */
		"affirmative",
		/* [11] */
		"ja",
		/* [12] */
		"word",
		/* [13] */
		""
	}
};

resource 'STR#' (23250, "NetErrors", purgeable) {
	{	/* array StringArray: 17 elements */
		/* [1] */
		"   0 Error unknown",
		/* [2] */
		" 108 Error closing net on ftp",
		/* [3] */
		" 504 Local host or gateway not respondin"
		"g",
		/* [4] */
		" 508 Fatal Error: cant allocate TCP buff"
		"er",
		/* [5] */
		" 509 TCP Error: failed to get TCP status",
		/* [6] */
		" 510 TCP Error: data failed to be added "
		"to Q (TCP Net Write)",
		/* [7] */
		" 511 TCP Error: failed to get IP address"
		" (MacTCP)",
		/* [8] */
		" 512 TCP Error: Couldn't make new TCP st"
		"ream",
		/* [9] */
		" 513 TCP Error: couldn't listen for new "
		"TCP connection (MacTCP)",
		/* [10] */
		" 514 TCP Error: TCP status failed (ftp)",
		/* [11] */
		" 515 TCP Error: open call failed. Can't "
		"open TCP socket (MacTCP)",
		/* [12] */
		" 516 TCP Error: invalid stream pointer. "
		" TCP close failed (MacTCP)",
		/* [13] */
		" 517 TCP Error: TCP close failed (MacTCP"
		")",
		/* [14] */
		" 518 TCP Error: TCP release failed (MacT"
		"CP)",
		/* [15] */
		" 519 TCP Error: TCP abort failed (MacTCP"
		")",
		/* [16] */
		" 524 TCP Error: TCP Rcv failed",
		/* [17] */
		" 807 Host or gateway not responding:"
	}
};

resource 'STR#' (23251, "Memory Errors", purgeable) {
	{	/* array StringArray: 7 elements */
		/* [1] */
		"   0 Error unknown",
		/* [2] */
		" 102 No space for macro storage Fatal er"
		"ror",
		/* [3] */
		" 103 Insufficient memory for TEK graphic"
		"s",
		/* [4] */
		" 104 Insufficient memory for VS",
		/* [5] */
		" 107 No space for set to load",
		/* [6] */
		" 108 Raster window create error. Not eno"
		"ugh memory to open",
		/* [7] */
		" 114 FTP server out of memory problem."
	}
};

resource 'STR#' (23253, "Resource Errors", purgeable) {
	{	/* array StringArray: 7 elements */
		/* [1] */
		"   0 Error unknown",
		/* [2] */
		" 106 Couldnt get translation resource fo"
		"r national character set",
		/* [3] */
		" 107 Translation resource is corrupted",
		/* [4] */
		" 200 Error deleting old file",
		/* [5] */
		" 201 Error in Sfwrite",
		/* [6] */
		" 202 Disk Full Error",
		/* [7] */
		" 302 Couldnt open window for raster grap"
		"hics"
	}
};

resource 'STR#' (7003, "Terminal Config Strings", purgeable) {
	{	/* array StringArray: 28 elements */
		/* [1] */
		"22",
		/* [2] */
		"ANSI color sequences",
		/* [3] */
		"Xterm sequences",
		/* [4] */
		"Use VT wrap mode",
		/* [5] */
		"EMACS arrow keys",
		/* [6] */
		"Map PgUp, etc.",
		/* [7] */
		"Eight bit connections",
		/* [8] */
		"Save cleared lines",
		/* [9] */
		"Remap Keypad",
		/* [10] */
		"VT100",
		/* [11] */
		"Control-Cmd",
		/* [12] */
		"Option",
		/* [13] */
		"Command",
		/* [14] */
		"",
		/* [15] */
		"Remap Del to ^D",
		/* [16] */
		"Allow boldfacing",
		/* [17] */
		"Use color for bold",
		/* [18] */
		"",
		/* [19] */
		"",
		/* [20] */
		"Use inverse for bold",
		/* [21] */
		"",
		/* [22] */
		"Bold",
		/* [23] */
		"Don't save attributes in scrollback",
		/* [24] */
		"Use jump scrolling (faster)",
		/* [25] */
		"Use real blinking",
		/* [26] */
		"VT220 7-bit Controls",
		/* [27] */
		"Off",
		/* [28] */
		"Hide scroll bars"
	}
};

resource 'STR#' (7001, "Main Prefs Strings", purgeable) {
	{	/* array StringArray: 43 elements */
		/* [1] */
		"-1",
		/* [2] */
		"",
		/* [3] */
		"",
		/* [4] */
		"Windows don't go away",
		/* [5] */
		"Staggered Windows",
		/* [6] */
		"Command Keys",
		/* [7] */
		"Remap backquote to ESCape",
		/* [8] */
		"Show Keypad and Function",
		/* [9] */
		"Blink Cursor",
		/* [10] */
		"Block",
		/* [11] */
		"Underline",
		/* [12] */
		"Vertical Bar",
		/* [13] */
		"In background, notify user of beeps",
		/* [14] */
		"Expire Kerberos tickets on close",
		/* [15] */
		"",
		/* [16] */
		"",
		/* [17] */
		"",
		/* [18] */
		"",
		/* [19] */
		"",
		/* [20] */
		"",
		/* [21] */
		"",
		/* [22] */
		"",
		/* [23] */
		"",
		/* [24] */
		"",
		/* [25] */
		"",
		/* [26] */
		"",
		/* [27] */
		"",
		/* [28] */
		"",
		/* [29] */
		"",
		/* [30] */
		"",
		/* [31] */
		"“Open Connection” dialog on startup",
		/* [32] */
		"Don't warn me on quit",
		/* [33] */
		"Auto open <Default>",
		/* [34] */
		"Allow typing of aliases",
		/* [35] */
		"Monospaced fonts only",
		/* [36] */
		"Always clip trailing spaces",
		/* [37] */
		"Globally remember OTP password",
		/* [38] */
		"Use live scrolling",
		/* [39] */
		"Cache passphrase for",
		/* [40] */
		"",
		/* [41] */
		"",
		/* [42] */
		"Use Keychain",
		/* [43] */
		"Don’t warn me on close"
	}
};

resource 'STR#' (23239, "Save Set Strings", purgeable) {
	{	/* array StringArray: 96 elements */
		/* [1] */
		"name",
		/* [2] */
		"host",
		/* [3] */
		"size",
		/* [4] */
		"scrollback",
		/* [5] */
		"backspace",
		/* [6] */
		"delete",
		/* [7] */
		"key0",
		/* [8] */
		"key1",
		/* [9] */
		"key2",
		/* [10] */
		"key3",
		/* [11] */
		"key4",
		/* [12] */
		"key5",
		/* [13] */
		"key6",
		/* [14] */
		"key7",
		/* [15] */
		"key8",
		/* [16] */
		"key9",
		/* [17] */
		"commandkeys",
		/* [18] */
		"erase",
		/* [19] */
		"width",
		/* [20] */
		"tekclear",
		/* [21] */
		"vtwidth",
		/* [22] */
		"rgb0",
		/* [23] */
		"rgb1",
		/* [24] */
		"rgb2",
		/* [25] */
		"rgb3",
		/* [26] */
		"font",
		/* [27] */
		"fsize",
		/* [28] */
		"nlines",
		/* [29] */
		"keystop",
		/* [30] */
		"keygo",
		/* [31] */
		"keyip",
		/* [32] */
		"crmap",
		/* [33] */
		"linemode",
		/* [34] */
		"eightbit",
		/* [35] */
		"ftp",
		/* [36] */
		"serial",
		/* [37] */
		"port",
		/* [38] */
		"translation",
		/* [39] */
		"tekem",
		/* [40] */
		"answerback",
		/* [41] */
		"authenticate",
		/* [42] */
		"encrypt",
		/* [43] */
		"pageup",
		/* [44] */
		"keypad",
		/* [45] */
		"ansicolor",
		/* [46] */
		"arrowmap",
		/* [47] */
		"emacsmeta",
		/* [48] */
		"pastemethod",
		/* [49] */
		"pastesize",
		/* [50] */
		"saveclear",
		/* [51] */
		"vtemulation",
		/* [52] */
		"forcesave",
		/* [53] */
		"vtwrap",
		/* [54] */
		"xterm",
		/* [55] */
		"localecho",
		/* [56] */
		"halfdup",
		/* [57] */
		"remapctrld",
		/* [58] */
		"negative",
		/* [59] */
		"allowbold",
		/* [60] */
		"colorbold",
		/* [61] */
		"ignorebeeps",
		/* [62] */
		"ignoreff",
		/* [63] */
		"boldfont",
		/* [64] */
		"inversebold",
		/* [65] */
		"otpauto",
		/* [66] */
		"otpmulti",
		/* [67] */
		"otphex",
		/* [68] */
		"otpnoprompt",
		/* [69] */
		"otppassword",
		/* [70] */
		"realbold",
		/* [71] */
		"otpsavepass",
		/* [72] */
		"oldscrollback",
		/* [73] */
		"protocol",
		/* [74] */
		"encryption",
		/* [75] */
		"authentication",
		/* [76] */
		"compression",
		/* [77] */
		"verbose",
		/* [78] */
		"trace",
		/* [79] */
		"debug",
		/* [80] */
		"restricted",
		/* [81] */
		"ssh2method",
		/* [82] */
		"ssh2guests",
		/* [83] */
		"localport",
		/* [84] */
		"remoteport",
		/* [85] */
		"remotehost",
		/* [86] */
		"username",
		/* [87] */
		"password",
		/* [88] */
		"command",
		/* [89] */
		"macro",
		/* [90] */
		"jumpscroll",
		/* [91] */
		"realblink",
		/* [92] */
		"vt7bit",
		/* [93] */
		"x11forward",
		/* [94] */
		"autoreconnect",
		/* [95] */
		"forwardtickets",
		/* [96] */
		"keepselection"
	}
};

resource 'STR#' (7004, "Session Config Strings", purgeable) {
	{	/* array StringArray: 95 elements */
		/* [1] */
		"-1",
		/* [2] */
		"",
		/* [3] */
		"",
		/* [4] */
		"Inhibit",
		/* [5] */
		"4014",
		/* [6] */
		"4105",
		/* [7] */
		"Quick",
		/* [8] */
		"Block",
		/* [9] */
		"Delete",
		/* [10] */
		"Backspace",
		/* [11] */
		"Forcesave",
		/* [12] */
		"Berkeley 4.3 CR mode",
		/* [13] */
		"Allow linemode",
		/* [14] */
		"TEK page clears screen",
		/* [15] */
		"Half duplex",
		/* [16] */
		"Kerberos authentication",
		/* [17] */
		"Kerberos encryption",
		/* [18] */
		"Local Echo",
		/* [19] */
		"",
		/* [20] */
		"",
		/* [21] */
		"",
		/* [22] */
		"",
		/* [23] */
		"",
		/* [24] */
		"",
		/* [25] */
		"",
		/* [26] */
		"",
		/* [27] */
		"",
		/* [28] */
		"",
		/* [29] */
		"",
		/* [30] */
		"",
		/* [31] */
		"",
		/* [32] */
		"",
		/* [33] */
		"",
		/* [34] */
		"",
		/* [35] */
		"",
		/* [36] */
		"",
		/* [37] */
		"",
		/* [38] */
		"",
		/* [39] */
		"",
		/* [40] */
		"",
		/* [41] */
		"",
		/* [42] */
		"",
		/* [43] */
		"...even on non-port-23",
		/* [44] */
		"Ignore beeps",
		/* [45] */
		"Ignore Form Feeds",
		/* [46] */
		"",
		/* [47] */
		"Automatic OTP (One-Time Passwords)",
		/* [48] */
		"...more than once per session",
		/* [49] */
		"...use hexadecimal to send to server",
		/* [50] */
		"...don't prompt if unnecessary",
		/* [51] */
		"",
		/* [52] */
		"",
		/* [53] */
		"Remember OTP password within session",
		/* [54] */
		"",
		/* [55] */
		"",
		/* [56] */
		"",
		/* [57] */
		"",
		/* [58] */
		"",
		/* [59] */
		"",
		/* [60] */
		"",
		/* [61] */
		"",
		/* [62] */
		"",
		/* [63] */
		"",
		/* [64] */
		"Initialize SSH…",
		/* [65] */
		"Use SOCKS 4 firewall",
		/* [66] */
		"...and SOCKS 4a DNS lookup",
		/* [67] */
		"",
		/* [68] */
		"",
		/* [69] */
		"",
		/* [70] */
		"",
		/* [71] */
		"",
		/* [72] */
		"",
		/* [73] */
		"",
		/* [74] */
		"",
		/* [75] */
		"",
		/* [76] */
		"verbose",
		/* [77] */
		"trace",
		/* [78] */
		"debug",
		/* [79] */
		"Never trust unknown host key",
		/* [80] */
		"Export public key…",
		/* [81] */
		"",
		/* [82] */
		"",
		/* [83] */
		"",
		/* [84] */
		"",
		/* [85] */
		"",
		/* [86] */
		"",
		/* [87] */
		"",
		/* [88] */
		"Enable guests",
		/* [89] */
		"",
		/* [90] */
		"",
		/* [91] */
		"",
		/* [92] */
		"accept esc-url sequences",
		/* [93] */
		"forward Kerberos v5 tickets",
		/* [94] */
		"forward X11",
		/* [95] */
		"keep selection on changed text"
	}
};

resource 'STR#' (2000, "Misc. messages", purgeable) {
	{	/* array StringArray: 14 elements */
		/* [1] */
		"The preferences file appears to be corru"
		"pted.  I can attempt to correct the prob"
		"lem.",
		/* [2] */
		"A vital resource appears to be missing o"
		"r is corrupted.  Please replace the MacS"
		"SH application with a new copy.",
		/* [3] */
		"Unable to repair the preferences file.  "
		"You must quit and delete the preferences"
		" file.",
		/* [4] */
		"The preferences on disk are a newer vers"
		"ion than I can handle.  Should I still s"
		"ave the current preferences, overwriting"
		" those I don't know how to handle?",
		/* [5] */
		"An error occurred while attempting to op"
		"en the MacTCP driver.",
		/* [6] */
		"There are connections still active or op"
		"ening.  Quitting now will abort them.  D"
		"o you really wish to do this?",
		/* [7] */
		"Unable to create a Preferences File.",
		/* [8] */
		"MacSSH requires HFS.",
		/* [9] */
		"Problem with the SysEnvirons code.",
		/* [10] */
		"MacSSH requires Mac OS 7.0 or later.",
		/* [11] */
		"MacSSH requires at least 128k ROMs.",
		/* [12] */
		"Fatal error installing AppleEvent handle"
		"rs.",
		/* [13] */
		"There are connections still active.  Do "
		"you really want to close all the windows"
		"?\n",
		/* [14] */
		"MacSSH requires the AppleEvent Manager."
	}
};

resource 'STR#' (2001, "Operation Failed Messages", preload) {
	{	/* array StringArray: 9 elements */
		/* [1] */
		"the file could not be created.",
		/* [2] */
		"the file could not be opened.",
		/* [3] */
		"there is not enough free memory.",
		/* [4] */
		"the set contains an invalid keyword.",
		/* [5] */
		"you cannot open a null hostname.",
		/* [6] */
		"this is an export version of BetterTelne"
		"t which does not support secure shell.",
		/* [7] */
		"MacSSH requires OpenTransport 1.1.1 or h"
		"igher for SSH2 connections.\nPlease, inst"
		"all OpenTransport and try again.",
		/* [8] */
		"MacSSH requires the Thread Manager for S"
		"SH2 connections.\nPlease, install the Thr"
		"ead Manager and try again.",
		/* [9] */
		"Error opening/reading known_hosts file. "
		"See Log window for more details."
	}
};

data 'SeSn' (1991, "<Default>") {
	$"0001 0016 FFFF 0001 0078 0000 0000 0000"            /* ....ˇˇ...x...... */
	$"0000 0000 01FF FFFF 093C 4465 6661 756C"            /* .....ˇˇˇ∆<Defaul */
	$"743E 0000 0000 0000 0000 0000 0000 0000"            /* t>.............. */
	$"0000 0000 0000 0000 0004 4E6F 6E65 0000"            /* ..........None.. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0400"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0004 0000 0000 0000 0438 0000 0000"            /* ...........8.... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0002 0002"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0330 2E30 0000 0000 0000"            /* .......0.0...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 00"                                  /* ....... */
};

data 'SeSn' (1992, "<Default>") {
	$"0001 0016 FFFF 0001 0078 0000 0000 0000"            /* ....ˇˇ...x...... */
	$"0000 0000 01FF FFFF 093C 4465 6661 756C"            /* .....ˇˇˇ∆<Defaul */
	$"743E 0000 0000 0000 0000 0000 0000 0000"            /* t>.............. */
	$"0000 0000 0000 0000 0006 4555 432D 4A50"            /* ..........EUC-JP */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0400"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0004 0000 0000 0000 0438 0000 0000"            /* ...........8.... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0002 0002"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0330 2E30 0000 0000 0000"            /* .......0.0...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 00"                                  /* ....... */
};

data 'Ssh2' (0, "Owner resource") {
	$"00"                                                 /* . */
};

data 'TMPL' (128, "PRCT", purgeable) {
	$"052A 2A2A 2A2A 4C53 5442 0641 2052 6563"            /* .*****LSTB.A Rec */
	$"7452 4543 5405 2A2A 2A2A 2A4C 5354 45"              /* tRECT.*****LSTE */
};

data 'TMPL' (129, "PrEf") {
	$"0776 6572 7369 6F6E 4457 5244 0B63 7572"            /* .versionDWRD.cur */
	$"736F 7220 7479 7065 4457 5244 1563 6F70"            /* sor typeDWRD.cop */
	$"7920 7461 626C 6520 7468 7265 7368 686F"            /* y table threshho */
	$"6C64 4457 5244 0974 696D 6573 6C69 6365"            /* ldDWRD∆timeslice */
	$"444C 4E47 1463 6170 7475 7265 2066 696C"            /* DLNG.capture fil */
	$"6520 6372 6561 746F 7254 4E41 4D15 7769"            /* e creatorTNAM.wi */
	$"6E64 6F77 7320 646F 6E27 7420 676F 2061"            /* ndows don't go a */
	$"7761 7944 4259 540F 7374 6167 6765 7220"            /* wayDBYT.stagger  */
	$"7769 6E64 6F77 7344 4259 540C 636F 6D6D"            /* windowsDBYT.comm */
	$"616E 6420 6B65 7973 4442 5954 1172 656D"            /* and keysDBYT.rem */
	$"6170 207E 2074 6F20 6573 6361 7065 4442"            /* ap ~ to escapeDB */
	$"5954 0C62 6C69 6E6B 2063 7572 736F 7244"            /* YT.blink cursorD */
	$"4259 540E 4B65 792F 4675 6E63 206D 656E"            /* BYT.Key/Func men */
	$"7573 4442 5954 0C6F 7065 6E20 7469 6D65"            /* usDBYT.open time */
	$"6F75 7444 5752 440C 7365 6E64 2074 696D"            /* outDWRD.send tim */
	$"656F 7574 4457 5244 1673 7461 6767 6572"            /* eoutDWRD.stagger */
	$"2077 696E 646F 7773 206F 6666 7365 7444"            /*  windows offsetD */
	$"5752 4414 6E6F 7469 6679 2075 7365 7220"            /* WRD.notify user  */
	$"6F66 2062 6565 7073 4457 5244 2164 6573"            /* of beepsDWRD!des */
	$"7472 6F79 204B 6572 6265 726F 7320 7469"            /* troy Kerberos ti */
	$"636B 6574 7320 6F6E 2063 6C6F 7365 4457"            /* ckets on closeDW */
	$"5244 2061 7574 6F2D 6F70 656E 204F 7065"            /* RD auto-open Ope */
	$"6E20 436F 6E6E 6563 7469 6F6E 2064 6961"            /* n Connection dia */
	$"6C6F 6744 5752 4412 646F 6E27 7420 7761"            /* logDWRD.don't wa */
	$"726E 206F 6E20 7175 6974 4457 5244 1361"            /* rn on quitDWRD.a */
	$"7574 6F2D 6F70 656E 203C 4465 6661 756C"            /* uto-open <Defaul */
	$"743E 4457 5244 1761 6C6C 6F77 2074 7970"            /* t>DWRD.allow typ */
	$"696E 6720 6F66 2061 6C69 6173 6573 4457"            /* ing of aliasesDW */
	$"5244 156D 6F6E 6F73 7061 6365 6420 666F"            /* RD.monospaced fo */
	$"6E74 7320 6F6E 6C79 4457 5244 1B61 6C77"            /* nts onlyDWRD.alw */
	$"6179 7320 636C 6970 2074 7261 696C 696E"            /* ays clip trailin */
	$"6720 7370 6163 6573 4457 5244 1D64 6566"            /* g spacesDWRD.def */
	$"6175 6C74 2041 4E53 4920 626F 6C64 2063"            /* ault ANSI bold c */
	$"6F6C 6F72 2069 6E64 6578 4457 5244 1A73"            /* olor indexDWRD.s */
	$"6176 6520 4F54 5020 7061 7373 776F 7264"            /* ave OTP password */
	$"2067 6C6F 6261 6C6C 7944 5752 4416 6469"            /*  globallyDWRD.di */
	$"7361 626C 6520 6C69 7665 2073 6372 6F6C"            /* sable live scrol */
	$"6C69 6E67 4457 5244 1063 6163 6865 2070"            /* lingDWRD.cache p */
	$"6173 7370 6872 6173 6544 4259 540C 7573"            /* assphraseDBYT.us */
	$"6520 6B65 7963 6861 696E 4442 5954 0675"            /* e keychainDBYT.u */
	$"6E75 7365 6444 4259 5406 756E 7573 6564"            /* nusedDBYT.unused */
	$"4442 5954 0B63 6163 6865 2064 656C 6179"            /* DBYT.cache delay */
	$"444C 4E47 0770 6164 6469 6E67 4845 5844"            /* DLNG.paddingHEXD */
};

data 'TMPL' (132, "TeR2") {
	$"0776 6572 7369 6F6E 4457 5244 1066 6F72"            /* .versionDWRD.for */
	$"6567 726F 756E 6420 636F 6C6F 7248 3030"            /* eground colorH00 */
	$"3610 6261 636B 6772 6F75 6E64 2063 6F6C"            /* 6.background col */
	$"6F72 4830 3036 0B62 6C69 6E6B 2063 6F6C"            /* orH006.blink col */
	$"6F72 4830 3036 1662 6C69 6E6B 2062 6163"            /* orH006.blink bac */
	$"6B67 726F 756E 6420 636F 6C6F 7248 3030"            /* kground colorH00 */
	$"3608 7265 7365 7276 6564 4830 3043 2A65"            /* 6.reservedH00C*e */
	$"6D75 6C61 7469 6F6E 2028 3020 3D20 7674"            /* mulation (0 = vt */
	$"3130 302C 2031 203D 2076 7432 3230 2C20"            /* 100, 1 = vt220,  */
	$"3220 3D20 616E 7369 2944 5752 4410 5654"            /* 2 = ansi)DWRD.VT */
	$"2077 6964 7468 2028 6368 6172 7329 4457"            /*  width (chars)DW */
	$"5244 1156 5420 6865 6967 6874 2028 6368"            /* RD.VT height (ch */
	$"6172 7329 4457 5244 0966 6F6E 7420 7369"            /* ars)DWRD∆font si */
	$"7A65 4457 5244 1A6E 756D 6265 7220 6F66"            /* zeDWRD.number of */
	$"2073 6372 6F6C 6C62 6163 6B20 6C69 6E65"            /*  scrollback line */
	$"7344 5752 4414 414E 5349 2063 6F6C 6F72"            /* sDWRD.ANSI color */
	$"2073 6571 7565 6E63 6573 4442 5954 0F58"            /*  sequencesDBYT.X */
	$"7465 726D 2073 6571 7565 6E63 6573 4442"            /* term sequencesDB */
	$"5954 0C56 5420 7772 6170 206D 6F64 6544"            /* YT.VT wrap modeD */
	$"4259 5434 454D 4143 5320 6D65 7461 206B"            /* BYT4EMACS meta k */
	$"6579 2028 3020 3D20 6E6F 6E65 2C20 3120"            /* ey (0 = none, 1  */
	$"3D20 636D 642D 636E 7472 6C2C 2032 203D"            /* = cmd-cntrl, 2 = */
	$"206F 7074 696F 6E29 4442 5954 0C45 4D41"            /*  option)DBYT.EMA */
	$"4353 2061 7272 6F77 7344 4259 540E 4D61"            /* CS arrowsDBYT.Ma */
	$"7020 5067 5570 2C20 6574 632E 4442 5954"            /* p PgUp, etc.DBYT */
	$"1138 2D62 6974 2063 6F6E 6E65 6374 696F"            /* .8-bit connectio */
	$"6E73 4442 5954 1273 6176 6520 636C 6561"            /* nsDBYT.save clea */
	$"7265 6420 6C69 6E65 7344 4259 5409 666F"            /* red linesDBYT∆fo */
	$"6E74 206E 616D 6550 3033 4612 616E 7377"            /* nt nameP03F.answ */
	$"6572 6261 636B 206D 6573 7361 6765 5030"            /* erback messageP0 */
	$"3230 0C72 656D 6170 206B 6579 7061 6444"            /* 20.remap keypadD */
	$"4259 540F 7265 6D61 7020 4465 6C20 746F"            /* BYT.remap Del to */
	$"205E 4444 5752 4410 616C 6C6F 7720 626F"            /*  ^DDWRD.allow bo */
	$"6C64 6661 6369 6E67 4457 5244 1275 7365"            /* ldfacingDWRD.use */
	$"2063 6F6C 6F72 2066 6F72 2062 6F6C 6444"            /*  color for boldD */
	$"5752 4412 626F 6C64 6661 6365 2066 6F6E"            /* WRD.boldface fon */
	$"7420 6E61 6D65 5030 3346 1A62 6F6C 6420"            /* t nameP03F.bold  */
	$"666F 6E74 2073 697A 6520 284E 4F54 2055"            /* font size (NOT U */
	$"5345 4421 2944 5752 4414 7573 6520 696E"            /* SED!)DWRD.use in */
	$"7665 7273 6520 666F 7220 626F 6C64 4457"            /* verse for boldDW */
	$"5244 1D75 7365 2062 6F6C 6420 7665 7273"            /* RD.use bold vers */
	$"696F 6E20 6F66 2062 6F6C 6420 666F 6E74"            /* ion of bold font */
	$"4457 5244 2064 6F6E 2774 2073 6176 6520"            /* DWRD don't save  */
	$"6174 7472 6962 7320 696E 2073 6372 6F6C"            /* attribs in scrol */
	$"6C62 6163 6B44 5752 4412 7573 6520 6A75"            /* lbackDWRD.use ju */
	$"6D70 2073 6372 6F6C 6C69 6E67 4457 5244"            /* mp scrollingDWRD */
	$"1175 7365 2072 6561 6C20 626C 696E 6B69"            /* .use real blinki */
	$"6E67 4457 5244 1456 5432 3230 2037 2D62"            /* ngDWRD.VT220 7-b */
	$"6974 2043 6F6E 7472 6F6C 7344 5752 4407"            /* it ControlsDWRD. */
	$"7061 6464 696E 6748 4558 44"                        /* paddingHEXD */
};

data 'TMPL' (133, "FTPs") {
	$"0776 6572 7369 6F6E 4457 5244 406D 6F64"            /* .versionDWRD@mod */
	$"6520 2830 203D 206F 6666 2C20 3120 3D20"            /* e (0 = off, 1 =  */
	$"6F6E 2C20 6E6F 2070 6173 7377 6F72 6473"            /* on, no passwords */
	$"2C20 3220 3D20 6F6E 2C20 7061 7373 776F"            /* , 2 = on, passwo */
	$"7264 7320 7265 7175 6972 6564 2944 5752"            /* rds required)DWR */
	$"4418 6372 6561 746F 7220 666F 7220 6269"            /* D.creator for bi */
	$"6E61 7279 2066 696C 6573 544E 414D 1574"            /* nary filesTNAM.t */
	$"7970 6520 666F 7220 6269 6E61 7279 2066"            /* ype for binary f */
	$"696C 6573 544E 414D 1663 7265 6174 6F72"            /* ilesTNAM.creator */
	$"2066 6F72 2074 6578 7420 6669 6C65 7354"            /*  for text filesT */
	$"4E41 4D0C 7368 6F77 2046 5450 206C 6F67"            /* NAM.show FTP log */
	$"4442 5954 0675 6E75 7365 6444 4259 5410"            /* DBYT.unusedDBYT. */
	$"7573 6520 4D61 6342 696E 6172 7920 4949"            /* use MacBinary II */
	$"4442 5954 0675 6E75 7365 6444 4259 5406"            /* DBYT.unusedDBYT. */
	$"756E 7573 6564 4442 5954 2372 6573 6574"            /* unusedDBYT#reset */
	$"204D 6163 4269 6E61 7279 2061 6674 6572"            /*  MacBinary after */
	$"2065 6163 6820 7472 616E 7366 6572 4442"            /*  each transferDB */
	$"5954 1375 7365 2049 534F 2074 7261 6E73"            /* YT.use ISO trans */
	$"6C61 7469 6F6E 4442 5954 0770 6164 6469"            /* lationDBYT.paddi */
	$"6E67 4845 5844"                                     /* ngHEXD */
};

data 'TMPL' (134, "taBL") {
	$"1328 6F74 6865 7229 2074 6F20 4D61 6352"            /* .(other) to MacR */
	$"6F6D 616E 4831 3030 134D 6163 526F 6D61"            /* omanH100.MacRoma */
	$"6E20 746F 2028 6F74 6865 7229 4831 3030"            /* n to (other)H100 */
};

data 'TMPL' (135, "FTPu") {
	$"0776 6572 7369 6F6E 4457 5244 1265 6E63"            /* .versionDWRD.enc */
	$"7279 7074 6564 2070 6173 7377 6F72 6450"            /* rypted passwordP */
	$"3031 460B 526F 6F74 2056 6F6C 756D 6550"            /* 01F.Root VolumeP */
	$"3031 4610 526F 6F74 2044 6972 6563 746F"            /* 01F.Root Directo */
	$"7279 4944 444C 4E47 0C55 7365 7220 6361"            /* ryIDDLNG.User ca */
	$"6E20 4357 4444 4259 5406 756E 7573 6564"            /* n CWDDBYT.unused */
	$"4442 5954 0770 6164 6469 6E67 4845 5844"            /* DBYT.paddingHEXD */
};

data 'TMPL' (131, "SeSn") {
	$"0776 6572 7369 6F6E 4457 5244 0470 6F72"            /* .versionDWRD.por */
	$"7444 5752 442D 5445 4B20 656D 756C 6174"            /* tDWRD-TEK emulat */
	$"696F 6E20 282D 3120 3D20 6E6F 6E65 2C20"            /* ion (-1 = none,  */
	$"3020 3D20 3430 3134 2C20 3120 3D20 3431"            /* 0 = 4014, 1 = 41 */
	$"3035 2944 5752 4423 7061 7374 6520 6D65"            /* 05)DWRD#paste me */
	$"7468 6F64 2028 3020 3D20 7175 6963 6B2C"            /* thod (0 = quick, */
	$"2031 203D 2062 6C6F 636B 2944 5752 4410"            /*  1 = block)DWRD. */
	$"7061 7374 6520 626C 6F63 6B20 7369 7A65"            /* paste block size */
	$"4457 5244 0872 6573 6572 7665 6444 4C4E"            /* DWRD.reservedDLN */
	$"4709 666F 7263 6573 6176 6544 4259 5412"            /* G∆forcesaveDBYT. */
	$"4253 4420 342E 3320 4352 206D 6170 7069"            /* BSD 4.3 CR mappi */
	$"6E67 4442 5954 0E61 6C6C 6F77 206C 696E"            /* ngDBYT.allow lin */
	$"656D 6F64 6544 4259 5408 7265 7365 7276"            /* emodeDBYT.reserv */
	$"6564 4442 5954 1654 454B 2070 6167 6520"            /* edDBYT.TEK page  */
	$"636C 6561 7273 2073 6372 6565 6E44 4259"            /* clears screenDBY */
	$"540B 6861 6C66 2064 7570 6C65 7844 4259"            /* T.half duplexDBY */
	$"5429 6465 6C65 7465 2073 656E 6473 3A20"            /* T)delete sends:  */
	$"2830 203D 2062 6163 6B73 7061 6365 2C20"            /* (0 = backspace,  */
	$"3120 3D20 6465 6C65 7465 2944 4259 540D"            /* 1 = delete)DBYT. */
	$"696E 7465 7272 7570 7420 6B65 7948 4259"            /* interrupt keyHBY */
	$"540B 7375 7370 656E 6420 6B65 7948 4259"            /* T.suspend keyHBY */
	$"540A 7265 7375 6D65 206B 6579 4842 5954"            /* T¬resume keyHBYT */
	$"106E 616D 6520 6F66 2074 6572 6D69 6E61"            /* .name of termina */
	$"6C50 3032 300B 7472 616E 736C 6174 696F"            /* lP020.translatio */
	$"6E50 3032 3009 686F 7374 206E 616D 6550"            /* nP020∆host nameP */
	$"3033 460C 6175 7468 656E 7469 6361 7465"            /* 03F.authenticate */
	$"4442 5954 0765 6E63 7279 7074 4442 5954"            /* DBYT.encryptDBYT */
	$"0A6C 6F63 616C 2065 6368 6F44 4259 5408"            /* ¬local echoDBYT. */
	$"7265 7365 7276 6564 4442 5954 126E 6574"            /* reservedDBYT.net */
	$"776F 726B 2062 6C6F 636B 2073 697A 6544"            /* work block sizeD */
	$"5752 4417 706F 7274 206E 756D 6265 7220"            /* WRD.port number  */
	$"6973 206E 6567 6174 6976 6544 5752 442F"            /* is negativeDWRD/ */
	$"7573 6520 4253 4420 342E 3320 4352 206D"            /* use BSD 4.3 CR m */
	$"6F64 6520 616C 7761 7973 2C20 6576 656E"            /* ode always, even */
	$"206F 6E20 6E6F 6E2D 706F 7274 2D32 3344"            /*  on non-port-23D */
	$"5752 440C 6967 6E6F 7265 2062 6565 7073"            /* WRD.ignore beeps */
	$"4457 5244 1069 676E 6F72 6520 666F 726D"            /* DWRD.ignore form */
	$"2066 6565 6444 5752 440D 6175 746F 6D61"            /*  feedDWRD.automa */
	$"7469 6320 4F54 5044 5752 4412 4F54 5020"            /* tic OTPDWRD.OTP  */
	$"6D75 6C74 6970 6C65 2074 696D 6573 4457"            /* multiple timesDW */
	$"5244 104F 5450 2064 6F6E 2774 2070 726F"            /* RD.OTP don't pro */
	$"6D70 7444 5752 4415 4F54 5020 6865 7861"            /* mptDWRD.OTP hexa */
	$"6465 6369 6D61 6C20 7265 706C 7944 5752"            /* decimal replyDWR */
	$"440C 4F54 5020 7061 7373 776F 7264 5030"            /* D.OTP passwordP0 */
	$"3346 1173 6176 6520 4F54 5020 7061 7373"            /* 3F.save OTP pass */
	$"776F 7264 4457 5244 0875 7365 726E 616D"            /* wordDWRD.usernam */
	$"6550 3046 4608 7061 7373 776F 7264 5030"            /* eP0FF.passwordP0 */
	$"4646 0F63 6C69 656E 7420 7573 6572 6E61"            /* FF.client userna */
	$"6D65 5030 4646 0763 6F6D 6D61 6E64 5030"            /* meP0FF.commandP0 */
	$"4646 0870 726F 746F 636F 6C44 5752 4411"            /* FF.protocolDWRD. */
	$"656E 6372 7970 7469 6F6E 206D 6574 686F"            /* encryption metho */
	$"6444 5752 4414 7573 6520 534F 434B 5320"            /* dDWRD.use SOCKS  */
	$"3420 6669 7265 7761 6C6C 4457 5244 1775"            /* 4 firewallDWRD.u */
	$"7365 2053 4F43 4B53 2034 6120 444E 5320"            /* se SOCKS 4a DNS  */
	$"6C6F 6F6B 7570 4457 5244 1553 4F43 4B53"            /* lookupDWRD.SOCKS */
	$"2070 6F72 7420 6E75 6D20 2831 3038 3029"            /*  port num (1080) */
	$"4457 5244 0E53 4F43 4B53 2068 6F73 746E"            /* DWRD.SOCKS hostn */
	$"616D 6550 3046 460E 534F 434B 5320 7573"            /* ameP0FF.SOCKS us */
	$"6572 6E61 6D65 5030 4646 1353 5348 3220"            /* ernameP0FF.SSH2  */
	$"4175 7468 656E 7469 6361 7469 6F6E 4457"            /* AuthenticationDW */
	$"5244 1053 5348 3220 436F 6D70 7265 7373"            /* RD.SSH2 Compress */
	$"696F 6E44 5752 440C 5353 4832 2076 6572"            /* ionDWRD.SSH2 ver */
	$"626F 7365 4457 5244 0A53 5348 3220 5472"            /* boseDWRD¬SSH2 Tr */
	$"6163 6544 5752 440A 5353 4832 2044 6562"            /* aceDWRD¬SSH2 Deb */
	$"7567 4457 5244 0F53 5348 3220 5265 7374"            /* ugDWRD.SSH2 Rest */
	$"7269 6374 6564 4457 5244 0B53 5348 3220"            /* rictedDWRD.SSH2  */
	$"4D65 7468 6F64 4457 5244 0B53 5348 3220"            /* MethodDWRD.SSH2  */
	$"4775 6573 7473 4457 5244 0A4C 6F63 616C"            /* GuestsDWRD¬Local */
	$"2050 6F72 7444 5752 440B 5265 6D6F 7465"            /*  PortDWRD.Remote */
	$"2050 6F72 7444 5752 440B 5265 6D6F 7465"            /*  PortDWRD.Remote */
	$"2048 6F73 7450 3042 331D 4C61 756E 6368"            /*  HostP0B3.Launch */
	$"2055 524C 206F 6E20 6573 6361 7065 2073"            /*  URL on escape s */
	$"6571 7565 6E63 6544 4259 540B 466F 7277"            /* equenceDBYT.Forw */
	$"6172 6420 7831 3144 4259 540B 5831 3120"            /* ard x11DBYT.X11  */
	$"4469 7370 6C61 7950 3033 460E 4175 746F"            /* DisplayP03F.Auto */
	$"2D52 6563 6F6E 6E65 6374 4442 5954"                 /* -ReconnectDBYT */
};

data 'TRSL' (256, "No Translation", preload) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"8081 8283 8485 8687 8889 8A8B 8C8D 8E8F"            /* ÄÅÇÉÑÖÜáàâäãåçéè */
	$"9091 9293 9495 9697 9899 9A9B 9C9D 9E9F"            /* êëíìîïñóòôöõúùûü */
	$"A0A1 A2A3 A4A5 A6A7 A8A9 AAAB ACAD AEAF"            /* †°¢£§•¶ß®©™´¨≠ÆØ */
	$"B0B1 B2B3 B4B5 B6B7 B8B9 BABB BCBD BEBF"            /* ∞±≤≥¥µ∂∑∏π∫ªºΩæø */
	$"C0C1 C2C3 C4C5 C6C7 C8C9 CACB CCCD CECF"            /* ¿¡¬√ƒ≈∆«»… ÀÃÕŒœ */
	$"D0D1 D2D3 D4D5 D6D7 D8D9 DADB DCDD DEDF"            /* –—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ */
	$"E0E1 E2E3 E4E5 E6E7 E8E9 EAEB ECED EEEF"            /* ‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔ */
	$"F0F1 F2F3 F4F5 F6F7 F8F9 FAFB FCFD FEFF"            /* ÒÚÛÙıˆ˜¯˘˙˚¸˝˛ˇ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"8081 8283 8485 8687 8889 8A8B 8C8D 8E8F"            /* ÄÅÇÉÑÖÜáàâäãåçéè */
	$"9091 9293 9495 9697 9899 9A9B 9C9D 9E9F"            /* êëíìîïñóòôöõúùûü */
	$"A0A1 A2A3 A4A5 A6A7 A8A9 AAAB ACAD AEAF"            /* †°¢£§•¶ß®©™´¨≠ÆØ */
	$"B0B1 B2B3 B4B5 B6B7 B8B9 BABB BCBD BEBF"            /* ∞±≤≥¥µ∂∑∏π∫ªºΩæø */
	$"C0C1 C2C3 C4C5 C6C7 C8C9 CACB CCCD CECF"            /* ¿¡¬√ƒ≈∆«»… ÀÃÕŒœ */
	$"D0D1 D2D3 D4D5 D6D7 D8D9 DADB DCDD DEDF"            /* –—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ */
	$"E0E1 E2E3 E4E5 E6E7 E8E9 EAEB ECED EEEF"            /* ‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔ */
	$"F0F1 F2F3 F4F5 F6F7 F8F9 FAFB FCFD FEFF"            /* ÒÚÛÙıˆ˜¯˘˙˚¸˝˛ˇ */
};

data 'TRSL' (257, "FTP ISO->MAC & MAC->ISO", preload) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"A5AA ADB0 B3B7 BABD C3C5 C9D1 D4D9 DAB6"            /* •™≠∞≥∑∫Ω√≈…—‘Ÿ⁄∂ */
	$"C6CE E2E3 E4F0 F6F7 F9FA FBFD FEFF F5C4"            /* ∆Œ‚„‰ˆ˜˘˙˚˝˛ˇıƒ */
	$"CAC1 A2A3 DBB4 CFA4 ACA9 BBC7 C2D0 A8F8"            /*  ¡¢£€¥œ§¨©ª«¬–®¯ */
	$"A1B1 D3D2 ABB5 A6E1 FCD5 BCC8 B9B8 B2C0"            /* °±”“´µ¶·¸’º»π∏≤¿ */
	$"CBE7 E5CC 8081 AE82 E983 E6E8 EDEA EBEC"            /* ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ */
	$"DC84 F1EE EFCD 85D7 AFF4 F2F3 86A0 DEA7"            /* ‹ÑÒÓÔÕÖ◊ØÙÚÛÜ†ﬁß */
	$"8887 898B 8A8C BE8D 8F8E 9091 9392 9495"            /* àáâãäåæçèéêëìíîï */
	$"DD96 9897 999B 9AD6 BF9D 9C9E 9FE0 DFD8"            /* ›ñòóôõö÷øùúûü‡ﬂÿ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"C4C5 C7C9 D1D6 DCE1 E0E2 E4E3 E5E7 E9E8"            /* ƒ≈«…—÷‹·‡‚‰„ÂÁÈË */
	$"EAEB EDEC EEEF F1F3 F2F4 F6F5 FAF9 FBFC"            /* ÍÎÌÏÓÔÒÛÚÙˆı˙˘˚¸ */
	$"DDB0 A2A3 A780 B6DF AEA9 81B4 A882 C6D8"            /* ›∞¢£ßÄ∂ﬂÆ©Å¥®Ç∆ÿ */
	$"83B1 BE84 A5B5 8F85 BDBC 86AA BA87 E6F8"            /* É±æÑ•µèÖΩºÜ™∫áÊ¯ */
	$"BFA1 AC88 9F89 90AB BB8A A0C0 C3D5 91A6"            /* ø°¨àüâê´ªä†¿√’ë¶ */
	$"AD8B B3B2 8CB9 F7D7 FF8D 8EA4 D0F0 DEFE"            /* ≠ã≥≤åπ˜◊ˇçé§–ﬁ˛ */
	$"FDB7 9293 94C2 CAC1 CBC8 CDCE CFCC D3D4"            /* ˝∑íìî¬ ¡À»ÕŒœÃ”‘ */
	$"95D2 DADB D99E 9697 AF98 999A B89B 9C9D"            /* ï“⁄€ŸûñóØòôö∏õúù */
};

data 'TeR2' (1991, "<Default>") {
	$"0001 BFFF BFFF BFFF 0000 0000 0000 FFFF"            /* ..øˇøˇøˇ......ˇˇ */
	$"2129 1EB5 FFFF FFFF FFFF 1DEF 1CF8 FFFF"            /* !).µˇˇˇˇˇˇ.Ô.¯ˇˇ */
	$"0000 0000 0000 0002 0050 0018 0009 03E8"            /* .........P...∆.Ë */
	$"0101 0100 0000 0101 0741 4E53 492F 5043"            /* .........ANSI/PC */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0461 6E73 6900 0000"            /* .........ansi... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0001 0001"            /* ................ */
	$"0741 4E53 492F 5043 0000 0000 0000 0000"            /* .ANSI/PC........ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0009 0000 0001 0000 0001 0001 0000 0000"            /* .∆.............. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000"                                               /* .. */
};

data 'TeR2' (1992, "<Default>") {
	$"0001 BFFF BFFF BFFF 0000 0000 0000 FFFF"            /* ..øˇøˇøˇ......ˇˇ */
	$"2129 1EB5 FFFF FFFF FFFF 1DEF 1CF8 FFFF"            /* !).µˇˇˇˇˇˇ.Ô.¯ˇˇ */
	$"0000 0000 0000 0001 0050 0018 000C 03E8"            /* .........P.....Ë */
	$"0101 0100 0000 0101 0841 534C 466F 6E74"            /* .........ASLFont */
	$"2B00 0000 0000 0000 0000 0000 0000 0000"            /* +............... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0139 0000 0000 0000"            /* .........9...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0001 0001"            /* ................ */
	$"0841 534C 466F 6E74 2B00 0000 0000 0000"            /* .ASLFont+....... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0009 0000 0001 0000 0001 0001 0001 0000"            /* .∆.............. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000"                                               /* .. */
};

data 'WIND' (256, "Graphics Window", purgeable) {
	$"0028 0028 0154 01B8 0000 0100 0100 0000"            /* .(.(.T.∏........ */
	$"0000 0847 7261 7068 6963 73"                        /* ...Graphics */
};

data 'WIND' (257, "FTP Log", purgeable) {
	$"00EC 000C 014C 00FC 0000 0000 0000 0000"            /* .Ï...L.¸........ */
	$"0000 0746 5450 204C 6F67"                           /* ...FTP Log */
};

resource 'actb' (200) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'acur' (10010, locked) {
	{	/* array CursIdArray: 4 elements */
		/* [1] */
		10010,
		/* [2] */
		10011,
		/* [3] */
		10012,
		/* [4] */
		10013
	}
};

data 'aete' (0) {
	$"0100 0000 0000 0003 0E52 6571 7569 7265"            /* .........Require */
	$"6420 5375 6974 652C 4576 656E 7473 2074"            /* d Suite,Events t */
	$"6861 7420 6576 6572 7920 6170 706C 6963"            /* hat every applic */
	$"6174 696F 6E20 7368 6F75 6C64 2073 7570"            /* ation should sup */
	$"706F 7274 7265 7164 0001 0001 0000 0000"            /* portreqd........ */
	$"0000 0000 0955 524C 2053 7569 7465 2C53"            /* ....∆URL Suite,S */
	$"7461 6E64 6172 6420 5375 6974 6520 666F"            /* tandard Suite fo */
	$"7220 556E 6966 6F72 6D20 5265 736F 7572"            /* r Uniform Resour */
	$"6365 204C 6F63 6174 6F72 7300 4755 524C"            /* ce Locators.GURL */
	$"0001 0001 0001 0667 6574 7572 6C32 4F70"            /* .......geturl2Op */
	$"656E 2061 2074 656C 6E65 7420 6F72 2073"            /* en a telnet or s */
	$"7368 2063 6F6E 6E65 6374 696F 6E20 7370"            /* sh connection sp */
	$"6563 6966 6965 6420 6279 2061 2055 524C"            /* ecified by a URL */
	$"4755 524C 4755 524C 7368 6F72 0B72 6573"            /* GURLGURLshor.res */
	$"756C 7420 636F 6465 0000 5445 5854 0C61"            /* ult code..TEXT.a */
	$"2074 656C 6E65 7420 5552 4C00 0000 0000"            /*  telnet URL..... */
	$"0000 0000 0000 0C4D 6163 5353 4820 5375"            /* .......MacSSH Su */
	$"6974 6555 436F 6E74 6169 6E73 2065 7665"            /* iteUContains eve */
	$"6E74 7320 7573 6566 756C 2066 6F72 2073"            /* nts useful for s */
	$"6372 6970 7469 6E67 204D 6163 5353 482C"            /* cripting MacSSH, */
	$"2069 6E63 6C75 6469 6E67 2061 2073 7562"            /*  including a sub */
	$"7365 7420 6F66 2063 6F72 6520 7375 6974"            /* set of core suit */
	$"6520 6576 656E 7473 2E00 5373 6832 0001"            /* e events..Ssh2.. */
	$"0001 0007 0973 656E 6420 7465 7874 2673"            /* ....∆send text&s */
	$"656E 6473 2061 2074 6578 7420 7374 7269"            /* ends a text stri */
	$"6E67 2074 6F20 7468 6520 7265 6D6F 7465"            /* ng to the remote */
	$"2068 6F73 7400 5373 6832 7365 6E64 6E75"            /*  host.Ssh2sendnu */
	$"6C6C 0320 2020 0000 5445 5854 1074 6865"            /* ll.   ..TEXT.the */
	$"2074 6578 7420 746F 2073 656E 6400 0000"            /*  text to send... */
	$"0000 0763 6F6E 6E65 6374 A96F 7065 6E73"            /* ...connect©opens */
	$"2061 206E 6577 2063 6F6E 6E65 6374 696F"            /*  a new connectio */
	$"6E20 746F 2061 2072 656D 6F74 6520 686F"            /* n to a remote ho */
	$"7374 2028 6578 616D 706C 653A 2063 6F6E"            /* st (example: con */
	$"6E65 6374 2022 3C44 6566 6175 6C74 3E22"            /* nect "<Default>" */
	$"2068 6F73 7420 2268 6F73 746E 616D 652E"            /*  host "hostname. */
	$"646F 6D61 696E 2E63 6F6D 3A32 3332 3322"            /* domain.com:2323" */
	$"2070 726F 746F 636F 6C20 7465 6C6E 6574"            /*  protocol telnet */
	$"2074 6974 6C65 2022 5769 6E64 6F77 2054"            /*  title "Window T */
	$"6974 6C65 2220 7761 6974 696E 6720 2279"            /* itle" waiting "y */
	$"6573 2220 7375 7370 656E 6465 6420 2279"            /* es" suspended "y */
	$"6573 2229 5373 6832 6D6B 4358 6E75 6C6C"            /* es")Ssh2mkCXnull */
	$"0000 0000 5445 5854 6F74 6865 206E 616D"            /* ....TEXTothe nam */
	$"6520 6F66 2074 6865 204D 6163 5353 4820"            /* e of the MacSSH  */
	$"6661 766F 7269 7465 7320 7365 7420 746F"            /* favorites set to */
	$"2075 7365 2066 6F72 2073 6574 7469 6E67"            /*  use for setting */
	$"7320 666F 7220 7468 6973 2063 6F6E 6E65"            /* s for this conne */
	$"6374 696F 6E20 3C64 6566 6175 6C74 3E20"            /* ction <default>  */
	$"6973 2075 7365 6420 6966 206E 6F6E 6520"            /* is used if none  */
	$"7072 6F76 6964 6564 8000 0008 0468 6F73"            /* providedÄ....hos */
	$"7400 686F 7374 5445 5854 7974 6865 206E"            /* t.hostTEXTythe n */
	$"616D 6520 6F66 2074 6865 2068 6F73 7420"            /* ame of the host  */
	$"746F 2077 6869 6368 2074 6F20 636F 6E6E"            /* to which to conn */
	$"6563 742C 2069 6E63 6C75 6469 6E67 2074"            /* ect, including t */
	$"6865 2070 6F72 7420 6E75 6D62 6572 2C20"            /* he port number,  */
	$"6966 2061 7070 6C69 6361 626C 6520 2D20"            /* if applicable -  */
	$"6966 206E 756C 6C2C 2074 6865 2064 6566"            /* if null, the def */
	$"6175 6C74 2077 696C 6C20 6265 2075 7365"            /* ault will be use */
	$"6420 2020 8000 0870 726F 746F 636F 6C00"            /* d   Ä..protocol. */
	$"7072 6F74 7072 6F74 3170 726F 746F 636F"            /* protprot1protoco */
	$"6C20 746F 2075 7365 202D 2073 6565 20D4"            /* l to use - see ‘ */
	$"7072 6F74 D520 656E 756D 6572 6174 6520"            /* prot’ enumerate  */
	$"666F 7220 7661 6C75 6573 A000 056C 6F67"            /* for values†..log */
	$"696E 6C6F 6769 5445 5854 4874 6865 206C"            /* inlogiTEXTHthe l */
	$"6F67 696E 2075 7365 6420 746F 2063 6F6E"            /* ogin used to con */
	$"6E65 6374 2074 6F20 7468 6520 686F 7374"            /* nect to the host */
	$"2E20 6966 206E 756C 6C2C 2074 6865 2064"            /* . if null, the d */
	$"6566 6175 6C74 2077 696C 6C20 6265 2075"            /* efault will be u */
	$"7365 6400 8000 0870 6173 7377 6F72 6400"            /* sed.Ä..password. */
	$"7061 7373 5445 5854 4B74 6865 2070 6173"            /* passTEXTKthe pas */
	$"7377 6F72 6420 7573 6564 2074 6F20 636F"            /* sword used to co */
	$"6E6E 6563 7420 746F 2074 6865 2068 6F73"            /* nnect to the hos */
	$"742E 2069 6620 6E75 6C6C 2C20 7468 6520"            /* t. if null, the  */
	$"6465 6661 756C 7420 7769 6C6C 2062 6520"            /* default will be  */
	$"7573 6564 8000 0763 6F6D 6D61 6E64 636D"            /* usedÄ..commandcm */
	$"6E64 5445 5854 3761 7267 756D 656E 7473"            /* ndTEXT7arguments */
	$"2070 6173 7365 6420 746F 206C 7368 2E20"            /*  passed to lsh.  */
	$"7265 6665 7220 746F 206C 7368 206D 616E"            /* refer to lsh man */
	$"7561 6C20 666F 7220 7573 6167 652E 8000"            /* ual for usage.Ä. */
	$"0574 6974 6C65 7469 746C 5445 5854 2874"            /* .titletitlTEXT(t */
	$"6865 2077 696E 646F 7720 7469 746C 6520"            /* he window title  */
	$"666F 7220 7468 6520 6E65 7720 636F 6E6E"            /* for the new conn */
	$"6563 7469 6F6E 2E00 8000 0973 7573 7065"            /* ection..Ä.∆suspe */
	$"6E64 6564 7375 7370 5445 5854 6673 6574"            /* ndedsuspTEXTfset */
	$"2074 6869 7320 746F 2022 7965 7322 2069"            /*  this to "yes" i */
	$"6620 796F 7520 7761 6E74 2074 6865 2063"            /* f you want the c */
	$"6F6E 6E65 6374 696F 6E20 746F 2062 6520"            /* onnection to be  */
	$"7375 7370 656E 6465 6420 7768 656E 2069"            /* suspended when i */
	$"7420 6F70 656E 7320 2D20 6F74 6865 7277"            /* t opens - otherw */
	$"6973 652C 206C 6561 7665 2069 7420 6F75"            /* ise, leave it ou */
	$"7420 2E00 8000 0777 6169 7469 6E67 7761"            /* t ..Ä..waitingwa */
	$"6974 5445 5854 6673 6574 2074 6869 7320"            /* itTEXTfset this  */
	$"746F 2022 7965 7322 2069 6620 796F 7520"            /* to "yes" if you  */
	$"7761 6E74 2074 6865 2073 6372 6970 7420"            /* want the script  */
	$"746F 2077 6169 7420 666F 7220 7468 6520"            /* to wait for the  */
	$"636F 6E6E 6563 7469 6F6E 2074 6F20 6F70"            /* connection to op */
	$"656E 202D 206F 7468 6572 7769 7365 2C20"            /* en - otherwise,  */
	$"6C65 6176 6520 6974 206F 7574 2E00 8000"            /* leave it out..Ä. */
	$"0773 656E 6420 6372 2C73 656E 6473 2061"            /* .send cr,sends a */
	$"2063 6172 7269 6167 6520 7265 7475 726E"            /*  carriage return */
	$"2074 6F20 7468 6520 7265 6D6F 7465 2068"            /*  to the remote h */
	$"6F73 742E 0D00 5373 6832 7365 4352 6E75"            /* ost...Ssh2seCRnu */
	$"6C6C 0000 0000 6E75 6C6C 0000 0000 0000"            /* ll....null...... */
	$"0D77 6169 7420 666F 7220 7465 7874 9077"            /* .wait for textêw */
	$"6169 7473 2066 6F72 2074 6865 2073 7065"            /* aits for the spe */
	$"6369 6669 6564 2074 6578 7420 6F6E 2074"            /* cified text on t */
	$"6865 2063 7572 7265 6E74 2063 6F6E 6E65"            /* he current conne */
	$"6374 696F 6E2C 2073 7573 7065 6E64 696E"            /* ction, suspendin */
	$"6720 7468 6520 4170 706C 6553 6372 6970"            /* g the AppleScrip */
	$"7420 616E 6420 756E 2D73 7573 7065 6E64"            /* t and un-suspend */
	$"696E 6720 7468 6520 636F 6E6E 6563 7469"            /* ing the connecti */
	$"6F6E 2066 6F72 2074 6865 2064 7572 6174"            /* on for the durat */
	$"696F 6E20 6F66 2074 6865 2077 6169 7400"            /* ion of the wait. */
	$"5373 6832 5761 6954 6E75 6C6C 0000 0000"            /* Ssh2WaiTnull.... */
	$"5445 5854 1A74 6865 2074 6578 7420 666F"            /* TEXT.the text fo */
	$"7220 7768 6963 6820 746F 2077 6169 7400"            /* r which to wait. */
	$"0000 0001 0973 7573 7065 6E64 6564 7375"            /* ....∆suspendedsu */
	$"7370 5445 5854 6073 6574 2074 6869 7320"            /* spTEXT`set this  */
	$"746F 2022 7965 7322 2069 6620 796F 7520"            /* to "yes" if you  */
	$"7761 6E74 2074 6865 2063 6F6E 6E65 6374"            /* want the connect */
	$"696F 6E20 746F 2062 6520 7375 7370 656E"            /* ion to be suspen */
	$"6465 6420 6166 7465 7220 6D61 7463 6869"            /* ded after matchi */
	$"6E67 2064 6174 6120 6861 7320 6265 656E"            /* ng data has been */
	$"2066 6F75 6E64 2E00 8000 0472 6561 6424"            /*  found..Ä..read$ */
	$"7761 6974 2066 6F72 2069 6E63 6F6D 696E"            /* wait for incomin */
	$"6720 6461 7461 2061 6E64 2072 6574 7572"            /* g data and retur */
	$"6E20 6974 5373 6832 5265 6144 5445 5854"            /* n itSsh2ReaDTEXT */
	$"0D72 6563 6569 7665 6420 6461 7461 0000"            /* .received data.. */
	$"6E75 6C6C 0000 0000 0000 0773 7573 7065"            /* null.......suspe */
	$"6E64 1F73 7573 7065 6E64 7320 7468 6520"            /* nd.suspends the  */
	$"6375 7272 656E 7420 636F 6E6E 6563 7469"            /* current connecti */
	$"6F6E 5373 6832 7375 7370 6E75 6C6C 0000"            /* onSsh2suspnull.. */
	$"0000 6E75 6C6C 0000 0000 0000 0975 6E73"            /* ..null......∆uns */
	$"7573 7065 6E64 2175 6E73 7573 7065 6E64"            /* uspend!unsuspend */
	$"7320 7468 6520 6375 7272 656E 7420 636F"            /* s the current co */
	$"6E6E 6563 7469 6F6E 5373 6832 2173 7573"            /* nnectionSsh2!sus */
	$"6E75 6C6C 0000 0000 6E75 6C6C 0000 0000"            /* null....null.... */
	$"0000 0000 0000 0001 7072 6F74 0005 0674"            /* ........prot...t */
	$"656C 6E65 7400 546C 6E74 1770 6C61 696E"            /* elnet.Tlnt.plain */
	$"2074 656C 6E65 7420 636F 6E6E 6563 7469"            /*  telnet connecti */
	$"6F6E 0672 6C6F 6769 6E00 526C 6F67 0000"            /* on.rlogin.Rlog.. */
	$"0372 7368 5273 6820 0000 0572 6578 6563"            /* .rshRsh ...rexec */
	$"5265 7865 0000 0373 7368 5373 6820 1773"            /* Rexe...sshSsh .s */
	$"6563 7572 652D 7368 656C 6C20 636F 6E6E"            /* ecure-shell conn */
	$"6563 7469 6F6E"                                     /* ection */
};

resource 'cctb' (131) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

resource 'cctb' (132) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

resource 'cctb' (134) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

resource 'cctb' (135) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

resource 'cctb' (1001) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

resource 'cctb' (2005) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

resource 'cctb' (2008) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

resource 'cctb' (2010) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

resource 'cctb' (2011) {
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		cFrameColor, 0, 0, 0,
		/* [2] */
		cBodyColor, 65535, 65535, 65535,
		/* [3] */
		cTextColor, 0, 0, 0,
		/* [4] */
		cElevatorColor, 65535, 65535, 65535
	}
};

data 'cicn' (128) {
	$"0000 0000 8010 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0004 0001 0004 0000 0000 0000 0352 0000"            /* .............R.. */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 3FFF FFFC 7FFF FFFE 7FFF FFFE 7FFF"            /* ..?ˇˇ¸.ˇˇ˛.ˇˇ˛.ˇ */
	$"FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF"            /* ˇ˛.ˇˇ˛.ˇˇ˛.ˇˇ˛.ˇ */
	$"FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF"            /* ˇ˛.ˇˇ˛.ˇˇ˛.ˇˇ˛.ˇ */
	$"FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF"            /* ˇ˛.ˇˇ˛.ˇˇ˛.ˇˇ˛.ˇ */
	$"FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF"            /* ˇ˛.ˇˇ˛.ˇˇ˛.ˇˇ˛.ˇ */
	$"FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE 7FFF"            /* ˇ˛.ˇˇ˛.ˇˇ˛.ˇˇ˛.ˇ */
	$"FFFE 3FFF FFFC 0000 0000 0000 0000 0000"            /* ˇ˛?ˇˇ¸.......... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 3FFF FFFC 4000 0002 4000 0002 4FFF"            /* ..?ˇˇ¸@...@...Oˇ */
	$"FFF2 4FFF FFF2 4D63 FFF2 4823 FFF2 4D63"            /* ˇÚOˇˇÚMcˇÚH#ˇÚMc */
	$"FFF2 4823 FFF2 4D63 FFF2 4FFF FFF2 4FFF"            /* ˇÚH#ˇÚMcˇÚOˇˇÚOˇ */
	$"FFF2 4FFF FFF2 4FFF FFF2 4FFF FFF2 4FFF"            /* ˇÚOˇˇÚOˇˇÚOˇˇÚOˇ */
	$"FFF2 4FFF FFF2 4FFF FFF2 4FFF FFF2 4FFF"            /* ˇÚOˇˇÚOˇˇÚOˇˇÚOˇ */
	$"FFF2 4FFF FFF2 4FFF FFF2 4000 0002 4000"            /* ˇÚOˇˇÚOˇˇÚ@...@. */
	$"0002 3FFF FFFC 0000 0000 0000 0000 0000"            /* ..?ˇˇ¸.......... */
	$"0000 0000 A5FE 0000 0008 0000 FFFF FFFF"            /* ....•˛......ˇˇˇˇ */
	$"FFFF 0005 46E3 0000 A53E 0006 0000 0000"            /* ˇˇ..F„..•>...... */
	$"D400 0007 0241 AB54 EAFF 000A 5600 2C9D"            /* ‘....A´TÍˇ.¬V.,ù */
	$"0524 000C C000 C000 C000 000D 8000 8000"            /* .$..¿.¿.¿...Ä.Ä. */
	$"8000 000E A999 A999 CFFF 000F 0000 0000"            /* Ä...©ô©ôœˇ...... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 00CC CCCC CCCC CCCC CCCC CCCC CCCC"            /* ...ÃÃÃÃÃÃÃÃÃÃÃÃÃ */
	$"CC00 0CEE EEEE EEEE EEEE EEEE EEEE EEEE"            /* Ã..ÓÓÓÓÓÓÓÓÓÓÓÓÓ */
	$"EED0 0CED DDDD DDDD DDDD DDDD DDDD DDDD"            /* Ó–.Ì›››››››››››› */
	$"DED0 0CED 6767 6767 6767 6767 6767 6767"            /* ﬁ–.Ìgggggggggggg */
	$"CED0 0CED 7606 0670 0076 7676 7676 7676"            /* Œ–.Ìv..p.vvvvvvv */
	$"CED0 0CED 6000 0060 0067 6767 6767 6767"            /* Œ–.Ì`..`.ggggggg */
	$"CED0 0CED 7607 0670 0076 7676 7676 7676"            /* Œ–.Ìv..p.vvvvvvv */
	$"CED0 0CED 6000 0060 0067 6767 6767 6767"            /* Œ–.Ì`..`.ggggggg */
	$"CED0 0CED 7606 0670 0076 7676 7676 7676"            /* Œ–.Ìv..p.vvvvvvv */
	$"CED0 0CED 6767 6767 6767 6767 6767 6767"            /* Œ–.Ìgggggggggggg */
	$"CED0 0CED 7676 7676 765A A576 7676 7676"            /* Œ–.ÌvvvvvZ•vvvvv */
	$"CED0 0CED 6767 6767 65FF FF57 6767 6767"            /* Œ–.ÌggggeˇˇWgggg */
	$"CED0 0CED 7676 7676 75AF FA56 7676 7676"            /* Œ–.ÌvvvvuØ˙Vvvvv */
	$"CED0 0CED 6767 6767 675F F567 6767 6767"            /* Œ–.Ìggggg_ıggggg */
	$"CED0 0CED 7676 7676 767A A676 7676 7676"            /* Œ–.Ìvvvvvz¶vvvvv */
	$"CED0 0CED 6767 6767 675F F567 6767 6767"            /* Œ–.Ìggggg_ıggggg */
	$"CED0 0CED 7676 7676 75AF FA56 7676 7676"            /* Œ–.ÌvvvvuØ˙Vvvvv */
	$"CED0 0CED 6767 6767 65FF FF57 6767 6767"            /* Œ–.ÌggggeˇˇWgggg */
	$"CED0 0CED 7676 7676 7AFF FFA6 7676 7676"            /* Œ–.Ìvvvvzˇˇ¶vvvv */
	$"CED0 0CED 6767 6767 6767 6767 6767 6767"            /* Œ–.Ìgggggggggggg */
	$"CED0 0CED 7676 7676 7676 7676 7676 7676"            /* Œ–.Ìvvvvvvvvvvvv */
	$"CED0 0CED CCCC CCCC CCCC CCCC CCCC CCCC"            /* Œ–.ÌÃÃÃÃÃÃÃÃÃÃÃÃ */
	$"CED0 0CEE EEEE EEEE EEEE EEEE EEEE EEEE"            /* Œ–.ÓÓÓÓÓÓÓÓÓÓÓÓÓ */
	$"EED0 00DD DDDD DDDD DDDD DDDD DDDD DDDD"            /* Ó–.››››››››››››› */
	$"DD00 0000 0000 0000 0000 0000 0000 0000"            /* ›............... */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000"                                               /* .. */
};

resource 'cicn' (129) {
	8,
	{0, 0, 32, 32},
	2,
	$"0000 0000 0000 000C 0000 001E 0000 003E"
	$"0000 007C 0000 00F8 0000 01FC 0000 03FE"
	$"0000 07FF 0000 0FFE 0000 1FF4 0000 3EE0"
	$"0000 7C70 0000 F820 0001 F000 0003 E000"
	$"0007 C000 000F 8000 001F 0000 003E 0000"
	$"1F7C 0000 3FF8 0000 7FF0 0000 FFE0 0000"
	$"F1F0 0000 F0F0 0000 F0F0 0000 F8F0 0000"
	$"7FF0 0000 3FE0 0000 1FC0 0000 0780 0000",
	$"0000 0000 0000 000C 0000 0012 0000 0022"
	$"0000 0044 0000 0088 0000 0104 0000 0202"
	$"0000 0401 0000 080A 0000 1114 0000 22A0"
	$"0000 4450 0000 8820 0001 1000 0002 2000"
	$"0004 4000 0008 8000 0011 0000 0022 0000"
	$"0E44 0000 3188 0000 4010 0000 8E20 0000"
	$"9120 0000 9090 0000 9090 0000 4890 0000"
	$"4720 0000 2020 0000 1840 0000 0780 0000",
	{	/* array ColorSpec: 4 elements */
		/* [1] */
		65535, 65535, 65535,
		/* [2] */
		52428, 52428, 52428,
		/* [3] */
		34952, 34952, 34952,
		/* [4] */
		0, 0, 0
	},
	$"0000 0000 0000 0000 0000 0000 0000 00F0"
	$"0000 0000 0000 031C 0000 0000 0000 0C6C"
	$"0000 0000 0000 31B0 0000 0000 0000 C6C0"
	$"0000 0000 0003 1AB0 0000 0000 000C 6A5C"
	$"0000 0000 0031 A957 0000 0000 00C6 A5EC"
	$"0000 0000 031B 9730 0000 0000 0C6C DC00"
	$"0000 0000 31B0 3B00 0000 0000 C6C0 0C00"
	$"0000 0003 1B00 0000 0000 000C 6C00 0000"
	$"0000 0031 B000 0000 0000 00C6 C000 0000"
	$"0000 031B 0000 0000 0000 0C6C 0000 0000"
	$"02FE 31B0 0000 0000 0F17 C6C0 0000 0000"
	$"3155 5B00 0000 0000 C6FD 5C00 0000 0000"
	$"C703 1E00 0000 0000 C700 D700 0000 0000"
	$"C700 D700 0000 0000 B6C0 D700 0000 0000"
	$"35BF 1E00 0000 0000 0D55 6C00 0000 0000"
	$"03EA B000 0000 0000 003F C000 0000 0000"
};

data 'crsr' (129) {
	$"8001 0000 0060 0000 0092 0000 0000 0000"            /* Ä....`...í...... */
	$"0000 0000 0000 0000 0000 FFFF FFEF FFE7"            /* ..........ˇˇˇÔˇÁ */
	$"F803 FFE7 FFEF FFFF FFFF FFFF FFFF FFFF"            /* ¯.ˇÁˇÔˇˇˇˇˇˇˇˇˇˇ */
	$"0000 0000 0000 0000 0000 FFFF FFFF FFFF"            /* ..........ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"0000 0000 0003 0001 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 8004 0000 0000 0010 0010 0000"            /* ....Ä........... */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0002 0001 0002 0000 0000 0000 00D2 0000"            /* .............“.. */
	$"0000 0000 0000 0000 0000 0000 0000 FFFF"            /* ..............ˇˇ */
	$"FFFF FFFF FDFF FFFF FD7F FFD5 555F FFFF"            /* ˇˇˇˇ˝ˇˇˇ˝.ˇ’U_ˇˇ */
	$"FD7F FFFF FDFF FFFF FFFF FFFF FFFF FFFF"            /* ˝.ˇˇ˝ˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF 0000 0000 0000"            /* ˇˇˇˇˇˇˇˇˇˇ...... */
	$"0000 0000 0000 0000 0002 0000 FFFF FFFF"            /* ............ˇˇˇˇ */
	$"FFFF 0001 FC00 F37D 052F 0003 0000 0000"            /* ˇˇ..¸.Û}./...... */
	$"0000"                                               /* .. */
};

data 'crsr' (130) {
	$"8001 0000 0060 0000 0092 0000 0000 0000"            /* Ä....`...í...... */
	$"0000 0000 0000 0000 0000 FFFF FFFF FFFF"            /* ..........ˇˇˇˇˇˇ */
	$"FFFF FFFF F7FF E7FF C01F E7FF F7FF FFFF"            /* ˇˇˇˇ˜ˇÁˇ¿.Áˇ˜ˇˇˇ */
	$"0000 0000 0000 0000 0000 FFFF FFFF FFFF"            /* ..........ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"0000 0000 0003 0001 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 8004 0000 0000 0010 0010 0000"            /* ....Ä........... */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0002 0001 0002 0000 0000 0000 00D2 0000"            /* .............“.. */
	$"0000 0000 0000 0000 0000 0000 0000 FFFF"            /* ..............ˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"FFFF FF7F FFFF FD7F FFFF F555 57FF FD7F"            /* ˇˇˇ.ˇˇ˝.ˇˇıUWˇ˝. */
	$"FFFF FF7F FFFF FFFF FFFF 0000 0000 0000"            /* ˇˇˇ.ˇˇˇˇˇˇ...... */
	$"0000 0000 0000 0000 0002 0000 FFFF FFFF"            /* ............ˇˇˇˇ */
	$"FFFF 0001 FC00 F37D 052F 0003 0000 0000"            /* ˇˇ..¸.Û}./...... */
	$"0000"                                               /* .. */
};

data 'crsr' (131) {
	$"8001 0000 0060 0000 0092 0000 0000 0000"            /* Ä....`...í...... */
	$"0000 0000 0000 0000 0000 FFFF FFFF FC3F"            /* ..........ˇˇˇˇ¸? */
	$"FBDF FBDF F00F F00F F00F F00F FFFF FFFF"            /* ˚ﬂ˚ﬂ....ˇˇˇˇ */
	$"0000 0000 0000 0000 0000 FFFF FFFF FFFF"            /* ..........ˇˇˇˇˇˇ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ */
	$"0000 0000 0008 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 8004 0000 0000 0010 0010 0000"            /* ....Ä........... */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0002 0001 0002 0000 0000 0000 00D2 0000"            /* .............“.. */
	$"0000 0000 0000 0000 0000 0000 0000 FFFF"            /* ..............ˇˇ */
	$"FFFF FFFF FFFF FFFD 7FFF FFF7 DFFF FFF7"            /* ˇˇˇˇˇˇˇ˝.ˇˇ˜ﬂˇˇ˜ */
	$"DFFF FFD5 57FF FFD5 57FF FFD5 57FF FFD5"            /* ﬂˇˇ’Wˇˇ’Wˇˇ’Wˇˇ’ */
	$"57FF FFFF FFFF FFFF FFFF 0000 0000 0000"            /* Wˇˇˇˇˇˇˇˇˇ...... */
	$"0000 0000 0000 0000 0002 0000 FFFF FFFF"            /* ............ˇˇˇˇ */
	$"FFFF 0001 FC00 F37D 052F 0003 0000 0000"            /* ˇˇ..¸.Û}./...... */
	$"0000"                                               /* .. */
};

resource 'dctb' (130) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (150) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (151) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (190) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (259, "My IP Number") {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (273) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (280) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (290) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (500) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (501) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (1001, purgeable) {
	{	/* array ColorSpec: 0 elements */
	}
};

resource 'dctb' (1002) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (1023) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (5500) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (7001) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (7003, "Terminal Config", purgeable) {
	{	/* array ColorSpec: 1 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535
	}
};

resource 'dctb' (7004) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (8000, "OTP Password Entry") {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (10001) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (10002) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (10003) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (10004) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (10010) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (10011) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dctb' (10012) {
	{	/* array ColorSpec: 5 elements */
		/* [1] */
		wContentColor, 65535, 65535, 65535,
		/* [2] */
		wFrameColor, 0, 0, 0,
		/* [3] */
		wTextColor, 0, 0, 0,
		/* [4] */
		wHiliteColor, 0, 0, 0,
		/* [5] */
		wTitleBarColor, 65535, 65535, 65535
	}
};

resource 'dlgx' (150) {
	versionZero {
		9
	}
};

resource 'dlgx' (190) {
	versionZero {
		9
	}
};

resource 'dlgx' (273) {
	versionZero {
		9
	}
};

resource 'dlgx' (280) {
	versionZero {
		9
	}
};

resource 'dlgx' (1001) {
	versionZero {
		9
	}
};

resource 'dlgx' (1002) {
	versionZero {
		9
	}
};

resource 'dlgx' (5500) {
	versionZero {
		13
	}
};

resource 'dlgx' (7001) {
	versionZero {
		9
	}
};

resource 'dlgx' (7003) {
	versionZero {
		9
	}
};

resource 'dlgx' (7004) {
	versionZero {
		9
	}
};

resource 'dlgx' (8000) {
	versionZero {
		9
	}
};

resource 'dlgx' (10001) {
	versionZero {
		13
	}
};

resource 'dlgx' (10002) {
	versionZero {
		13
	}
};

resource 'dlgx' (10003) {
	versionZero {
		13
	}
};

resource 'dlgx' (10004) {
	versionZero {
		13
	}
};

resource 'dlgx' (10010) {
	versionZero {
		13
	}
};

resource 'dlgx' (10012) {
	versionZero {
		13
	}
};

resource 'icl4' (128) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00CC CCCC CCCC CCCC CCCC CCCC CCCC CC00"
	$"0CCC CCCC CCCC CCCC CCCC CCCC CCCC CCD0"
	$"0CCD DDDD DDDD DDDD DDDD DDDD DDDD DCD0"
	$"0CCD 6767 6767 6767 6767 6767 6767 CCD0"
	$"0CCD 7606 0670 0076 7676 7676 7676 CCD0"
	$"0CCD 6000 0060 0067 6767 6767 6767 CCD0"
	$"0CCD 7607 0670 0076 7676 7676 7676 CCD0"
	$"0CCD 6000 0060 0067 6767 6767 6767 CCD0"
	$"0CCD 7606 0670 0076 7676 7676 7676 CCD0"
	$"0CCD 6767 6767 6767 6767 6767 6767 CCD0"
	$"0CCD 7676 7676 765A A576 7676 7676 CCD0"
	$"0CCD 6767 6767 65FF FF57 6767 6767 CCD0"
	$"0CCD 7676 7676 75AF FA56 7676 7676 CCD0"
	$"0CCD 6767 6767 675F F567 6767 6767 CCD0"
	$"0CCD 7676 7676 767A A676 7676 7676 CCD0"
	$"0CCD 6767 6767 675F F567 6767 6767 CCD0"
	$"0CCD 7676 7676 75AF FA56 7676 7676 CCD0"
	$"0CCD 6767 6767 6EFF FFE7 6767 6767 CCD0"
	$"0CCD 7676 7676 7AFF FFA6 7676 7676 CCD0"
	$"0CCD 6767 6767 6767 6767 6767 6767 CCD0"
	$"0CCD 7676 7676 7676 7676 7676 7676 CCD0"
	$"0CCD CCCC CCCC CCCC CCCC CCCC CCCC CCD0"
	$"0CCC CCCC CCCC CCCC CCCC CCCC CCCC CCD0"
	$"00DD DDDD DDDD DDDD DDDD DDDD DDDD DD"
};

resource 'icl4' (129) {
	$"00DF FFFF FD00 0000 0DFF FFFF D000 0000"
	$"00FC CCCC CF00 0000 0FCC CCCC F000 0000"
	$"00FC EEEE CF00 0000 0FCE EEEC F000 0000"
	$"00FC ECC0 CF00 0000 0FCE CC0C F000 0000"
	$"00FC E000 CF00 0000 0FCE 000C F000 0000"
	$"00FC CCCC CF00 0000 0FCC CCCC F000 0000"
	$"FFFF FFFF FFFF 000F FFFF FFFF FFFD DDD0"
	$"FCCC CCCC C3CF DDDF CCCC CCCC 3CF0 00D0"
	$"FCCC CCCC CCCF 000F CCCC CCCC CCF0 00D0"
	$"FFFF FFFF FFFF 000F FFFF FFFF FFF0 00D0"
	$"0000 0000 0000 0000 0000 0000 0000 00D0"
	$"0000 0000 0FFF FFFF FFFF 0000 0000 00D0"
	$"0000 0000 0FDE DEDE DEDF 0000 0000 00D0"
	$"0000 0000 0FDE DEDE DEDF 0000 0000 00D0"
	$"0000 0000 0FDE DEDE DEDF 0000 0000 00D0"
	$"0000 0000 0FDE DEDE DEDF 0000 0000 00D0"
	$"0000 0000 0FDE DEDE DEDF 0000 0000 00D0"
	$"0000 0000 0FDE DEDE DEDF 0000 0000 00D0"
	$"0000 00FF FFFF FFFF FFFF FFFD DDDD DD00"
	$"0000 00FD DDDD DDDD DDDD DDF0 0000 00D0"
	$"0000 00FD DDDD DDDD DDDD DDF0 0000 00D0"
	$"0000 00FF FFFF FFFF FFFF FFF0 0000 00D0"
	$"0000 0000 0000 0000 0000 0000 0000 00D0"
	$"DFFF FFD0 000D FFFF FD00 00DF FFFF D0D0"
	$"FCCC CCF0 000F CCCC CF00 00FC CCCC F0D0"
	$"FCEE 0CF0 000F CEE0 CF00 00FC EE0C F0D0"
	$"FCEC 0CF0 000F CEC0 CF00 00FC EC0C FDD0"
	$"FCEC 0CFD DDDF CEC0 CFDD DDFC EC0C F000"
	$"FC00 0CF0 000F C000 CF00 00FC 000C F000"
	$"F8CE ECF0 000F 8CEE CF00 00F8 CEEC F000"
	$"FCCC CCF0 000F CCCC CF00 00FC CCCC F000"
	$"0FFF FF00 0000 FFFF F000 000F FFFF"
};

resource 'icl4' (131) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"EEEE EEEE EEEE EEEE EEEE EEEE EE00 0000"
	$"E000 0000 0000 0000 0000 0000 0EE0 0000"
	$"E000 0000 0000 0000 0000 0000 0ECE 0000"
	$"E000 0000 0000 0000 0000 0000 0ECC E000"
	$"E000 0FFF FFFF FFFF FF00 0000 0ECC CE00"
	$"E000 FCCC CCCC CCCC CCF0 0000 0ECC CCE0"
	$"E000 FCCF FFFF FFFF CCF0 0000 0EEE EEEE"
	$"E000 FCFC CCCC CCCC 0CF0 0000 0000 000E"
	$"E000 FCFC CCCC CCCC 0CF0 0000 F000 000E"
	$"E000 FCFC CCCC CCCC 0CF0 0000 0F00 000E"
	$"E000 FCFC CCCC CCCC 0CF0 FFFF FFF0 000E"
	$"E000 FCFC CCCC CCCC 0CF0 0000 0F00 000E"
	$"E000 FCFC CCCC CCCC 0CF0 00F0 F000 000E"
	$"E000 FCC0 0000 0000 CCF0 0F00 0000 000E"
	$"E000 FCCC CCCC CCCC CCF0 FFFF FFF0 000E"
	$"E000 FCCC CCCC FFFF FCF0 0F00 0000 000E"
	$"E000 FCCC CCCC CCCC CCF0 00F0 0000 000E"
	$"E000 FCCC CCCC CCCC CCF0 0000 0000 000E"
	$"E000 FFFF FFFF FFFF FFF0 0000 0000 000E"
	$"E000 0FCC CCCC CCCC CF00 0000 0000 000E"
	$"E000 0FFF FFFF FFFF FF00 0000 0000 000E"
	$"E000 0000 000F 0000 0000 0000 0000 000E"
	$"E000 CCDD EEF0 FEED DCC0 0000 0000 000E"
	$"E000 0000 0000 0000 0000 0000 0000 000E"
	$"EEEE EEEE EEEE EEEE EEEE EEEE EFFE EEEE"
};

resource 'icl4' (132) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00FF FFFF FFFF FFF0 0000 0000 0000 0000"
	$"0FCC CCCC CCCC CCCF 0000 0D00 00D0 0000"
	$"0FCC EEEE EEEE ECCF 0000 0DE0 00DE 0000"
	$"0FCE CCCC CCCC C0CF 0000 0DE0 00DE 0000"
	$"0FCE CFFF CFFC C0CF 0DDD DDDD DDDD DD00"
	$"0FCE CCFC CFCF C0CF 00EE EDEE EEDE EEE0"
	$"0FCE CCFC CFFC C0CF 0000 0DE0 00DE 0000"
	$"0FCE CFFF CFCC C0CF 0000 0DE0 00DE 0000"
	$"0FCE CCCC CCCC C0CF 0000 0DE0 00DE 0000"
	$"0FCC 0000 0000 0CCF 0DDD DDDD DDDD DD00"
	$"0FCC CCCC CCCC CCCF 00EE EDEE EEDE EEE0"
	$"0FCC CCCC CFFF FFCF 0000 0DE0 00DE 0000"
	$"0FCC CCCC CCCC CCCF 0000 0DE0 00DE 0000"
	$"0FCC CCCC CCCC CCCF 0000 0DE0 00DE 0000"
	$"0FFF FFFF FFFF FFFF 0000 00E0 000E 0000"
	$"00FC CCCC CCCC CCF0 0000 0000 0000 0000"
	$"00FF FFFF FFFF FFF0 0000 0000 0000 0000"
	$"0000 0000 F000 0000 0000 0000 0000 0000"
	$"0000 000F 0F00 0000 0000 0000 0000 0000"
	$"0000 000F 0F00 0000 0000 0000 0000 0000"
	$"0000 000F 0F00 0000 0000 0000 0000 0000"
	$"FFFF FFF0 00FF FFFF F0"
};

resource 'icl4' (133) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00FF FFFF FFFF FFF0 0000 0000 0000 0000"
	$"0FCC CCCC CCCC CCCF 0000 0000 0000 0000"
	$"0FCC EEEE EEEE ECCF 0000 0000 0000 0000"
	$"0FCE CCCC CCCC C0CF 000F F000 FF00 0000"
	$"0FCE CCCC CCCC C0CF 00F0 0F0F 00F0 0000"
	$"0FCE CCCC CCCC C0CF 00F0 0F0F 00F0 0000"
	$"0FCE CCCC CCCC C0CF 000F FFFF FF00 0000"
	$"0FCE CCCC CCCC C0CF 0000 0F0F 0000 0000"
	$"0FCE CCCC CCCC C0CF 000F FFFF FF00 0000"
	$"0FCC 0000 0000 0CCF 00F0 0F0F 00F0 0000"
	$"0FCC CCCC CCCC CCCF 00F0 0F0F 00F0 0000"
	$"0FCC CCCC CFFF FFCF 000F F000 FF00 0000"
	$"0FCC CCCC CCCC CCCF 0000 0000 0000 0000"
	$"0FCC CCCC CCCC CCCF 0000 0000 0000 0000"
	$"0FFF FFFF FFFF FFFF 0000 0000 0000 0000"
	$"00FC CCCC CCCC CCF0 0000 0000 0000 0000"
	$"00FF FFFF FFFF FFF0 0000 0000 0000 0000"
	$"0000 0000 F000 0000 0000 0000 0000 0000"
	$"0000 000F 0F00 0000 0000 0000 0000 0000"
	$"0000 000F 0F00 0000 0000 0000 0000 0000"
	$"0000 000F 0F00 0000 0000 0000 0000 0000"
	$"FFFF FFF0 00FF FFFF F0"
};

resource 'icl4' (134) {
	$"CCCC CCCC CCCC CCCC CCCC CCCC CCCC CCCC"
	$"CCCC C000 000C CCCC CCCC CCCC C000 0CCC"
	$"CCC0 000C CCCC CCCC CCCC CCCC 0000 00CC"
	$"CC00 0CCC CCCC CCCC CCCC CCCC CDCC 000C"
	$"C000 CCCC CCCD DCCD DDDC CCCC CCCC C00C"
	$"C000 DCCC CCC0 0000 CC00 0CCC CCCC C00C"
	$"C00C DCCC CCC0 0000 0000 00CC CCCC C00C"
	$"D00C CCCC C000 000C CC00 000C CCCC C00C"
	$"D00C CCCC C00C CCCC CCDC CCC0 CCCC C00C"
	$"C000 CDDC 0CCC 0C00 0CC0 CCCC CCCC 000C"
	$"C000 000C C000 0000 0000 CCC0 C000 000D"
	$"CC00 000C 0000 00CC C000 DC00 0C00 00CC"
	$"CC00 00C0 0000 00CC C00C DCC0 0C00 0CCC"
	$"CCDC 00C0 0000 00CE C000 DEC0 00DD DDCC"
	$"CCCD DCC0 000C 000C 0000 CC00 00DC 5CCC"
	$"CCCC CDC0 000C 0000 0000 0CCC 000C CCCC"
	$"CCCC CC00 00C0 0000 0CCC 0CCD DCCD CCCC"
	$"CCCC CC00 CCC0 0000 CC0C C0CC CCCC CCCC"
	$"CCCC C0CC CC00 0000 CCCC 0000 0CCC CCCC"
	$"CCCC DDDC 0C00 000C 000C 0000 CCCC CCCC"
	$"CCCC CCCC 0C00 000C C000 00CC DCCC CCCC"
	$"CCCC CCC0 00C0 0000 C000 00CC CCCC CCCC"
	$"CCCC CCCC 00CC 0000 CC00 0CCD CCCC CCCC"
	$"CCCC CCCC 000C C000 0CCC C00D CCCC CCCC"
	$"CCCC CCCC 0000 C000 0000 000D CCCC CCCC"
	$"CCCC CCCC C000 C000 00CC CCDC CCCC CCCC"
	$"CCCC CCCC DC00 0C00 0000 CDCC CCCC CCCC"
	$"CCCC CCCC CCC0 CDCC 0000 0DCC CCCC CCCC"
	$"CCCC CCCC CCCC CCCD C00C DCCC CCCC CCCC"
	$"CCCC CCCC CCCC CCCC D0CD CCCC CCCC CCCC"
	$"CCCC CCCC CCCC CCCC DCCC CCCC CCCC CCCC"
	$"CCCC CCCC CCCC CCCC CCCC CCCC CCCC CCCC"
};

resource 'icl4' (135) {
	$""
};

resource 'icl8' (128) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 0000"
	$"002B F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8"
	$"F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 FA00"
	$"002B F8F9 FAFA FAFA FAFA FAFA FAFA FAFA"
	$"FAFA FAFA FAFA FAFA FAFA FAFA F9F8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A2 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 00A2 00A2 A200 0000 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A200 0000 0000 A200 0000 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 00A2 00A2 A200 0000 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A200 0000 0000 A200 0000 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 00A2 00A2 A200 0000 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A2 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A2 A9FD"
	$"FDA9 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A9 FEFF"
	$"FFFE A9A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A9 FDFF"
	$"FFFD A9A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A2 A9FE"
	$"FEA9 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A2 A2FD"
	$"FDA2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A2 A9FE"
	$"FEA9 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A9 FDFF"
	$"FFFD A9A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2AB FEFF"
	$"FFFE ABA2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2DE FFFF"
	$"FFFF DEA2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A2 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8FA A2A2 A2A2 A2A2 A2A2 A2A2 A2A2"
	$"A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 2BF8 FA00"
	$"002B F8F9 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BF8 FA00"
	$"002B F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8"
	$"F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 FA00"
	$"0000 FAFA FAFA FAFA FAFA FAFA FAFA FAFA"
	$"FAFA FAFA FAFA FAFA FAFA FAFA FAFA"
};

resource 'icl8' (129) {
	$"0000 F9FF FFFF FFFF FFF9 0000 0000 0000"
	$"00F9 FFFF FFFF FFFF F900 0000 0000 0000"
	$"0000 FF2B 2B2B 2B2B 2BFF 0000 0000 0000"
	$"00FF 2B2B 2B2B 2B2B FF00 0000 0000 0000"
	$"0000 FF2B FCFC FCFC 2BFF 0000 0000 0000"
	$"00FF 2BFC FCFC FC2B FF00 0000 0000 0000"
	$"0000 FF2B FC2A 2AF5 2BFF 0000 0000 0000"
	$"00FF 2BFC 2A2A F52B FF00 0000 0000 0000"
	$"0000 FF2B FCF5 F5F5 2BFF 0000 0000 0000"
	$"00FF 2BFC F5F5 F52B FF00 0000 0000 0000"
	$"0000 FF2B 2B2B 2B2B 2BFF 0000 0000 0000"
	$"00FF 2B2B 2B2B 2B2B FF00 0000 0000 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 0000 00FF"
	$"FFFF FFFF FFFF FFFF FFFF FFFA FAFA FA00"
	$"FF2B 2B2B 2B2B 2B2B 2BD8 2BFF FAFA FAFF"
	$"2B2B 2B2B 2B2B 2B2B D82B FF00 0000 FA00"
	$"FF2B 2B2B 2B2B 2B2B 2B2B 2BFF 0000 00FF"
	$"2B2B 2B2B 2B2B 2B2B 2B2B FF00 0000 FA00"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 0000 00FF"
	$"FFFF FFFF FFFF FFFF FFFF FF00 0000 FA00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 FA00"
	$"0000 0000 0000 0000 00FF FFFF FFFF FFFF"
	$"FFFF FFFF 0000 0000 0000 0000 0000 FA00"
	$"0000 0000 0000 0000 00FF F9FC F9FC F9FC"
	$"F9FC F9FF 0000 0000 0000 0000 0000 FA00"
	$"0000 0000 0000 0000 00FF F9FC F9FC F9FC"
	$"F9FC F9FF 0000 0000 0000 0000 0000 FA00"
	$"0000 0000 0000 0000 00FF F9FC F9FC F9FC"
	$"F9FC F9FF 0000 0000 0000 0000 0000 FA00"
	$"0000 0000 0000 0000 00FF F9FC F9FC F9FC"
	$"F9FC F9FF 0000 0000 0000 0000 0000 FA00"
	$"0000 0000 0000 0000 00FF F9FC F9FC F9FC"
	$"F9FC F9FF 0000 0000 0000 0000 0000 FA00"
	$"0000 0000 0000 0000 00FF F9FC F9FC F9FC"
	$"F9FC F9FF 0000 0000 0000 0000 0000 FA00"
	$"0000 0000 0000 FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFA FAFA FAFA FAFA 0000"
	$"0000 0000 0000 FFF9 F9F9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 F9F9 FF00 0000 0000 0000 FA00"
	$"0000 0000 0000 FFF9 F9F9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 F9F9 FF00 0000 0000 0000 FA00"
	$"0000 0000 0000 FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FF00 0000 0000 0000 FA00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 FA00"
	$"F9FF FFFF FFFF F900 0000 00F9 FFFF FFFF"
	$"FFF9 0000 0000 F9FF FFFF FFFF F900 FA00"
	$"FF2B 2B2B 2B2B FF00 0000 00FF 2B2B 2B2B"
	$"2BFF 0000 0000 FF2B 2B2B 2B2B FF00 FA00"
	$"FF2B FCFC F52B FF00 0000 00FF 2BFC FCF5"
	$"2BFF 0000 0000 FF2B FCFC F52B FF00 FA00"
	$"FF2B FC2A F52B FF00 0000 00FF 2BFC 2AF5"
	$"2BFF 0000 0000 FF2B FC2A F52B FFFA FA00"
	$"FF2B FC2A F52B FFFA FAFA FAFF 2BFC 2AF5"
	$"2BFF FAFA FAFA FF2B FC2A F52B FF00 0000"
	$"FF2B F5F5 F52B FF00 0000 00FF 2BF5 F5F5"
	$"2BFF 0000 0000 FF2B F5F5 F52B FF00 0000"
	$"FFE3 2BFC FC2B FF00 0000 00FF E32B FCFC"
	$"2BFF 0000 0000 FFE3 2BFC FC2B FF00 0000"
	$"FF2B 2B2B 2B2B FF00 0000 00FF 2B2B 2B2B"
	$"2BFF 0000 0000 FF2B 2B2B 2B2B FF00 0000"
	$"00FF FFFF FFFF 0000 0000 0000 FFFF FFFF"
	$"FF00 0000 0000 00FF FFFF FFFF"
};

resource 'icl8' (131) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"FCFC FCFC FCFC FCFC FCFC FCFC FCFC FCFC"
	$"FCFC FCFC FCFC FCFC FCFC 0000 0000 0000"
	$"FCF5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 F5FC FC00 0000 0000"
	$"FCF5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 F5FC F6FC 0000 0000"
	$"FCF5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 F5FC F6F6 FC00 0000"
	$"FCF5 F5F5 F5FF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF F5F5 F5F5 F5F5 F5FC F6F6 F6FC 0000"
	$"FCF5 F5F5 FF2B 2B2B 2B2B 2B2B 2B2B 2B2B"
	$"2B2B FFF5 F5F5 F5F5 F5FC F6F6 F6F6 FC00"
	$"FCF5 F5F5 FF2B 2BFF FFFF FFFF FFFF FFFF"
	$"2B2B FFF5 F5F5 F5F5 F5FC FCFC FCFC FCFC"
	$"FCF5 F5F5 FF2B FF2A 2A2A 2A2A 2A2A 2A2A"
	$"002B FFF5 F5F5 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B FF2A 2A2A 2A2A 2A2A 2A2A"
	$"002B FFF5 F5F5 F5F5 FFF5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B FF2A 2A2A 2A2A 2A2A 2A2A"
	$"002B FFF5 F5F5 F5F5 F5FF F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B FF2A 2A2A 2A2A 2A2A 2A2A"
	$"002B FFF5 FFFF FFFF FFFF FFF5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B FF2A 2A2A 2A2A 2A2A 2A2A"
	$"002B FFF5 F5F5 F5F5 F5FF F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B FF2A 2A2A 2A2A 2A2A 2A2A"
	$"002B FFF5 F5F5 FFF5 FFF5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B 2BF5 F5F5 F5F5 F5F5 F5F5"
	$"2B2B FFF5 F5FF F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B 2B2B 2B2B 2B2B 2B2B 2B2B"
	$"2B2B FFF5 FFFF FFFF FFFF FFF5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B 2B2B 2B2B 2B2B FFFF FFFF"
	$"FF2B FFF5 F5FF F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B 2B2B 2B2B 2B2B 2B2B 2B2B"
	$"2B2B FFF5 F5F5 FFF5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FF2B 2B2B 2B2B 2B2B 2B2B 2B2B"
	$"2B2B FFF5 F5F5 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFF5 F5F5 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 F5FF 2B2B 2B2B 2B2B 2B2B 2B2B"
	$"2BFF F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 F5FF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 F5F5 F5F5 F5F5 F5FF F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 2BF7 F8F9 FAFB FCFD FEF5 FEFD FCFB"
	$"FAF9 F8F7 2BF6 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5"
	$"F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCFC FCFC FCFC FCFC FCFC FCFC FCFC FCFC"
	$"FCFC FCFC FCFC FCFC FCFF FFFC FCFC FCFC"
};

resource 'icl8' (132) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FF00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00FF 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFF"
	$"0000 0000 00F9 0000 0000 F900 0000 0000"
	$"00FF 2B2B FCFC FCFC FCFC FCFC FC2B 2BFF"
	$"0000 0000 00F9 FC00 0000 F9FC 0000 0000"
	$"00FF 2BFC 2A2A 2A2A 2A2A 2A2A 2AF5 2BFF"
	$"0000 0000 00F9 FC00 0000 F9FC 0000 0000"
	$"00FF 2BFC 2AFF FFFF 2AFF FF2A 2AF5 2BFF"
	$"00F9 F9F9 F9F9 F9F9 F9F9 F9F9 F9F9 0000"
	$"00FF 2BFC 2A2A FF2A 2AFF 2AFF 2AF5 2BFF"
	$"0000 FCFC FCF9 FCFC FCFC F9FC FCFC FC00"
	$"00FF 2BFC 2A2A FF2A 2AFF FF2A 2AF5 2BFF"
	$"0000 0000 00F9 FC00 0000 F9FC 0000 0000"
	$"00FF 2BFC 2AFF FFFF 2AFF 2A2A 2AF5 2BFF"
	$"0000 0000 00F9 FC00 0000 F9FC 0000 0000"
	$"00FF 2BFC 2A2A 2A2A 2A2A 2A2A 2AF5 2BFF"
	$"0000 0000 00F9 FC00 0000 F9FC 0000 0000"
	$"00FF 2B2B F5F5 F5F5 F5F5 F5F5 F52B 2BFF"
	$"00F9 F9F9 F9F9 F9F9 F9F9 F9F9 F9F9 0000"
	$"00FF 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFF"
	$"0000 FCFC FCF9 FCFC FCFC F9FC FCFC FC00"
	$"00FF 2B2B 2B2B 2B2B 2BFF FFFF FFFF 2BFF"
	$"0000 0000 00F9 FC00 0000 F9FC 0000 0000"
	$"00FF 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFF"
	$"0000 0000 00F9 FC00 0000 F9FC 0000 0000"
	$"00FF 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFF"
	$"0000 0000 00F9 FC00 0000 F9FC 0000 0000"
	$"00FF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"0000 0000 0000 FC00 0000 00FC 0000 0000"
	$"0000 FF2B 2B2B 2B2B 2B2B 2B2B 2B2B FF00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FF00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 FF00 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 00FF 00FF 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 00FF 00FF 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 00FF 00FF 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"FFFF FFFF FFFF FF00 0000 FFFF FFFF FFFF"
	$"FF"
};

resource 'icl8' (133) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FF00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00FF 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFF"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00FF 2B2B FCFC FCFC FCFC FCFC FC2B 2BFF"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00FF 2BFC 2A2A 2A2A 2A2A 2A2A 2AF5 2BFF"
	$"0000 00FF FF00 0000 FFFF 0000 0000 0000"
	$"00FF 2BFC 2A2A 2A2A 2A2A 2A2A 2AF5 2BFF"
	$"0000 FF00 00FF 00FF 0000 FF00 0000 0000"
	$"00FF 2BFC 2A2A 2A2A 2A2A 2A2A 2AF5 2BFF"
	$"0000 FF00 00FF 00FF 0000 FF00 0000 0000"
	$"00FF 2BFC 2A2A 2A2A 2A2A 2A2A 2AF5 2BFF"
	$"0000 00FF FFFF FFFF FFFF 0000 0000 0000"
	$"00FF 2BFC 2A2A 2A2A 2A2A 2A2A 2AF5 2BFF"
	$"0000 0000 00FF 00FF 0000 0000 0000 0000"
	$"00FF 2BFC 2A2A 2A2A 2A2A 2A2A 2AF5 2BFF"
	$"0000 00FF FFFF FFFF FFFF 0000 0000 0000"
	$"00FF 2B2B F5F5 F5F5 F5F5 F5F5 F52B 2BFF"
	$"0000 FF00 00FF 00FF 0000 FF00 0000 0000"
	$"00FF 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFF"
	$"0000 FF00 00FF 00FF 0000 FF00 0000 0000"
	$"00FF 2B2B 2B2B 2B2B 2BFF FFFF FFFF 2BFF"
	$"0000 00FF FF00 0000 FFFF 0000 0000 0000"
	$"00FF 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFF"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00FF 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2BFF"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00FF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 FF2B 2B2B 2B2B 2B2B 2B2B 2B2B FF00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FF00"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 FF00 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 00FF 00FF 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 00FF 00FF 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 00FF 00FF 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"FFFF FFFF FFFF FF00 0000 FFFF FFFF FFFF"
	$"FF"
};

resource 'icl8' (134) {
	$"5555 5555 5555 5555 5555 5555 5555 5555"
	$"5555 5555 5555 5555 5555 5555 5555 5555"
	$"5555 5555 F600 0000 0000 F5F7 5555 5555"
	$"5555 5555 5555 F7F8 F7F5 0000 002B 5555"
	$"5555 F700 0000 F5F8 F8F8 F62B F855 5555"
	$"5555 5555 55F8 F72B F500 0000 0000 2B55"
	$"55F8 F500 002B F855 5555 5555 5555 5555"
	$"5555 5555 5555 5555 5556 F8F6 0000 00F7"
	$"55F5 0000 F755 5555 5555 557A 56F8 557A"
	$"797A 8055 5555 5555 5555 55F8 F600 002B"
	$"F8F5 00F5 5655 5555 5555 F8F5 0000 F5F5"
	$"F8F7 F5F5 F5F8 5555 5555 5555 2B00 002B"
	$"F800 002B 7A55 5555 55F8 F600 0000 0000"
	$"0000 0000 0000 F855 5555 5555 2B00 002B"
	$"5600 002B 5555 5555 55F5 0000 0000 00F6"
	$"F8F8 F500 0000 00F7 5555 5555 2B00 002B"
	$"5600 00F6 5555 55F8 F600 F5F7 F7F8 F8F7"
	$"F7F7 56F7 F8F7 F6F5 F855 5555 F600 002C"
	$"F700 0000 F880 56F6 F52B 2BF6 F5F6 F500"
	$"00F7 2BF5 F6F8 2B2B 2BF7 F82B 0000 00F7"
	$"55F5 0000 00F5 002B 2BF5 0000 0000 00F5"
	$"0000 0000 F62B F600 2BF5 0000 0000 F556"
	$"55F7 0000 0000 F52B 0000 0000 0000 2B2B"
	$"F600 0000 56F7 F500 F52B 0000 0000 F855"
	$"5555 F500 0000 2BF5 0000 0000 00F5 2B2B"
	$"2B00 00F6 562B F600 002B F500 002B 5555"
	$"5555 56F6 0000 2B00 0000 0000 0000 F8FC"
	$"2B00 00F5 FAFB F600 00F5 5656 567A 5555"
	$"5555 557A 562B 2B00 0000 00F6 F500 00F6"
	$"0000 0000 F72B 0000 0000 5655 A455 5555"
	$"5555 5555 557A 2B00 0000 F52B 0000 0000"
	$"0000 0000 F52B F7F7 F500 F5F7 5555 5555"
	$"5555 5555 5555 F500 0000 F7F5 0000 0000"
	$"F52B 2B2B 00F6 557A 56F8 2B56 5555 5555"
	$"5555 5555 552B 0000 F6F8 F600 0000 0000"
	$"2BF6 F5F6 F700 F7F8 F855 5555 5555 5555"
	$"5555 5555 F8F5 2BF7 F62B F500 0000 0000"
	$"2BF7 F82B F5F5 0000 F555 5555 5555 5555"
	$"5555 5555 5656 7AF6 00F6 F500 0000 00F6"
	$"0000 F5F6 F500 0000 F755 5555 5555 5555"
	$"5555 5555 5555 55F6 002B F500 0000 00F6"
	$"F600 0000 0000 F6F8 7A55 5555 5555 5555"
	$"5555 5555 5555 55F5 00F5 F7F5 0000 00F5"
	$"2B00 0000 0000 F6F7 5555 5555 5555 5555"
	$"5555 5555 5555 55F6 0000 F62B 0000 0000"
	$"F6F7 F500 F52B 2B56 5555 5555 5555 5555"
	$"5555 5555 5555 55F7 0000 002B F600 0000"
	$"00F6 F82B F8F5 F57A 5555 5555 5555 5555"
	$"5555 5555 5555 5555 F500 00F5 F700 0000"
	$"0000 0000 00F5 F5F9 5555 5555 5555 5555"
	$"5555 5555 5555 5555 F700 0000 2BF5 0000"
	$"0000 F62B 2BF7 7A55 5555 5555 5555 5555"
	$"5555 5555 5555 5555 562B 0000 F52B 0000"
	$"0000 F5F5 2B7A 5555 5555 5555 5555 5555"
	$"5555 5555 5555 5555 5555 F6F5 2B56 F8F6"
	$"0000 0000 F580 5555 5555 5555 5555 5555"
	$"5555 5555 5555 5555 5555 5555 5555 55F9"
	$"F600 00F8 7A55 5555 5555 5555 5555 5555"
	$"5555 5555 5555 5555 5555 5555 5555 5555"
	$"FAF5 F67A 5555 5555 5555 5555 5555 5555"
	$"5555 5555 5555 5555 5555 5555 5555 5555"
	$"F92B 2B55 5555 5555 5555 5555 5555 5555"
	$"5555 5555 5555 5555 5555 5555 5555 5555"
	$"5555 5555 5555 5555 5555 5555 5555 5555"
};

resource 'icl8' (135) {
	$""
};

resource 'icm#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm#' (131) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm#' (132) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm#' (133) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm#' (134) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm#' (135) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm4' (128) {
	$""
};

resource 'icm4' (131) {
	$""
};

resource 'icm4' (132) {
	$""
};

resource 'icm4' (133) {
	$""
};

resource 'icm4' (134) {
	$""
};

resource 'icm4' (135) {
	$""
};

resource 'icm8' (128) {
	$""
};

resource 'icm8' (131) {
	$""
};

resource 'icm8' (132) {
	$""
};

resource 'icm8' (133) {
	$""
};

resource 'icm8' (134) {
	$""
};

resource 'icm8' (135) {
	$""
};

resource 'ics#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 7FFE 8001 BFFD BFFD B58D A08D B58D"
		$"A08D B58D BFFD BFFD BFFD 8001 7FFE",
		/* [2] */
		$"0000 0000 7FFE FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF 7FFE"
	}
};

resource 'ics#' (129) {
	{	/* array: 2 elements */
		/* [1] */
		$"01F8 0204 02F4 0284 0284 0204 0FFF 7C01"
		$"8201 B3FF A202 A200 82AA 9A00 8200 7C",
		/* [2] */
		$"01F8 03FC 03FC 03FC 03FC 03FC 0FFF 7FFF"
		$"FFFF FFFF FE02 FE00 FEAA FE00 FE00 7C"
	}
};

resource 'ics#' (131) {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 FFF8 800C 9F0A A08F AE89 A8BD A889"
		$"A091 A6BD A091 9F01 8001 FFFF",
		/* [2] */
		$"0000 FFF8 FFFC FFFE FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF"
	}
};

resource 'ics#' (132) {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 0000 3E00 4124 597E 5124 5124"
		$"417E 4D24 4100 3E00 0800 1400 1400 A3AA",
		/* [2] */
		$"0000 0000 0000 7F00 7F24 7F7E 7F24 7F24"
		$"7F7E 7F24 7F00 3E00 0800 1400 1400 E3FE"
	}
};

resource 'ics#' (133) {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 0000 3E00 4122 5955 513E 5114"
		$"413E 4D55 4122 3E00 0800 1400 1400 A3AA",
		/* [2] */
		$"0000 0000 0000 7F00 7F22 7F77 7F3E 7F1C"
		$"7F3E 7F77 7F22 3E00 0800 1400 1400 E3FE"
	}
};

resource 'ics#' (134) {
	{	/* array: 2 elements */
		/* [1] */
		$"3E1E FE3F E7E7 EFF3 FFFF FFFF 7FFE 7FFE"
		$"3FFC 3FF8 1FF8 1FF0 0FF0 0FE0 01E0 00C0",
		/* [2] */
		$"3E1E FE3F E7E7 EFF3 FFFF FFFF 7FFE 7FFE"
		$"3FFC 3FF8 1FF8 1FF0 0FF0 0FE0 01E0 00C0"
	}
};

resource 'ics#' (135) {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 03C0 0420 0420 0420 0FF0 08F0"
		$"08F0 08F0 08F0 0FF0",
		/* [2] */
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	}
};

resource 'ics4' (128) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0CCC CCCC CCCC CCC0 CDDD DDDD DDDD DDDD"
	$"CD77 7777 7777 77CD CD77 0707 7000 77CD"
	$"CD70 0000 7000 77CD CD77 0707 7000 77CD"
	$"CD70 0000 7000 77CD CD77 0707 7000 77CD"
	$"CD77 777F F777 77CD CD77 7777 7777 77CD"
	$"CD77 777F F777 77CD CDCC CCCC CCCC CCCD"
	$"0DDD DDDD DDDD DDD0"
};

resource 'ics4' (129) {
	$"0000 00DE EEEE ED00 0000 00EC CCCC CE00"
	$"0000 00EC EEEE CE00 0000 00EC ECC0 CE00"
	$"0000 00EC E000 CE00 0000 00EC CCCC CE00"
	$"0000 DEEE EEEE EEEE DEEE EEDC CCCC C3CE"
	$"ECCC CCEC CCCC CCCE ECEE 0CEE EEEE EEEE"
	$"ECEC 0CE0 0000 00D0 ECEC 0CE0 0000 00D0"
	$"EC00 0CED DDDD DDD0 E9CE ECE0 0000 0000"
	$"ECCC CCE0 0000 0000 0EEE EE"
};

resource 'ics4' (131) {
	$"0000 0000 0000 0000 EEEE EEEE EEEE E000"
	$"E000 0000 0000 EE00 E0DF FFFF D000 ECE0"
	$"E0FC CCCC F000 EEEE E0FC FF0C F000 F00E"
	$"E0FC FC0C F0FF FF0E E0FC FC0C F000 F00E"
	$"E0FC 000C F00F 000E E0F8 CFFC F0FF FF0E"
	$"E0FC CCCC F00F 000E E00F FFFF 0000 000E"
	$"E000 0000 0000 000E EEEE EEEE EEEE FFEE"
};

resource 'ics4' (132) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0DFF FFFD 0000 0000"
	$"0FCC CCCF 00F0 0F00 0FCE E0CF 0FFF FFF0"
	$"0FCE C0CF 00F0 0F00 0FCE C0CF 00F0 0F00"
	$"0FC0 00CF 0FFF FFF0 0FCC EECF 00F0 0F00"
	$"0FCC CCCF 0000 0000 00FF FFF0 0000 0000"
	$"0000 F000 0000 0000 000F 0F00 0000 0000"
	$"000F 0F00 0000 0000 CDE0 00EE EDDD CCC0"
};

resource 'ics4' (133) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0DFF FFFD 0000 0000"
	$"0FCC CCCF 00F0 00F0 0FCE E0CF 0F0F 0F0F"
	$"0FCE C0CF 00FF FFF0 0FCE C0CF 000F 0F00"
	$"0FC0 00CF 00FF FFF0 0FCC EECF 0F0F 0F0F"
	$"0FCC CCCF 00F0 00F0 00FF FFF0 0000 0000"
	$"0000 F000 0000 0000 000F 0F00 0000 0000"
	$"000F 0F00 0000 0000 CDE0 00EE EDDD CCC0"
};

resource 'ics4' (134) {
	$"00C0 0C00 000C C0C0 CCCC CCC0 00CC DC0C"
	$"C0D0 0DDD DDC0 0CCC DCD0 CC0C C00C 00CC"
	$"DCDD CCCC CDCC CCCC C00C C00C C0DC C00D"
	$"0D0C 000E CCEC CDD0 0DDC 0C0C 00CC 0D50"
	$"00C0 CC00 CCCD DD00 00DD C00C CC00 C000"
	$"000C CC0C C00C D000 000C 0CC0 CCCD 0000"
	$"0000 C0C0 0CCD 0000 0000 DCDC 00D0 0000"
	$"0000 000D DDD0 0000 0000 0000 DC"
};

resource 'ics4' (135) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 00EE EE00 0000 0000 0E00 00E0 0000"
	$"0000 0E00 00E0 0000 0000 0E00 00E0 0000"
	$"0000 DDDD DEEF 0000 0000 DC0D DDEF 0000"
	$"0000 DC0D DEEF 0000 0000 DC0D DDEF 0000"
	$"0000 DC0D DEEF 0000 0000 DDEF FFFF"
};

resource 'ics8' (128) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"002B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B 2B00"
	$"2BF9 FAFA FAFA FAFA FAFA FAFA FAFA F9FA"
	$"2BFA A2A2 A2A2 A2A2 A2A2 A2A2 A2A2 F8FA"
	$"2BFA A2A2 00A2 00A2 A200 0000 A2A2 F8FA"
	$"2BFA A200 0000 0000 A200 0000 A2A2 F8FA"
	$"2BFA A2A2 00A2 00A2 A200 0000 A2A2 F8FA"
	$"2BFA A200 0000 0000 A200 0000 A2A2 F8FA"
	$"2BFA A2A2 00A2 00A2 A200 0000 A2A2 F8FA"
	$"2BFA A2A2 A2A2 A2FF FFA2 A2A2 A2A2 F8FA"
	$"2BFA A2A2 A2A2 A2C7 C7A2 A2A2 A2A2 F8FA"
	$"2BFA A2A2 A2A2 A2FF FFA2 A2A2 A2A2 F8FA"
	$"2BF9 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 F8FA"
	$"00FA FAFA FAFA FAFA FAFA FAFA FAFA FA"
};

resource 'ics8' (129) {
	$"0000 0000 0000 F9FC FCFC FCFC FCF9 0000"
	$"0000 0000 0000 FC2B 2B2B 2B2B 2BFC 0000"
	$"0000 0000 0000 FC2B FCFC FCFC 2BFC 0000"
	$"0000 0000 0000 FC2B FC2B 2B00 2BFC 0000"
	$"0000 0000 0000 FC2B FC00 0000 2BFC 0000"
	$"0000 0000 0000 FC2B 2B2B 2B2B 2BFC 0000"
	$"0000 0000 F9FC FCFC FCFC FCFC FCFC FCFC"
	$"F9FC FCFC FCFC F92B 2B2B 2B2B 2BD8 2BFC"
	$"FC2B 2B2B 2B2B FC2B 2B2B 2B2B 2B2B 2BFC"
	$"FC2B FCFC 002B FCFC FCFC FCFC FCFC FCFC"
	$"FC2B FC2B 002B FC00 0000 0000 0000 F900"
	$"FC2B FC2B 002B FC00 0000 0000 0000 F900"
	$"FC2B 0000 002B FCF9 F9F9 F9F9 F9F9 F900"
	$"FCE3 2BFC FC2B FC00 0000 0000 0000 0000"
	$"FC2B 2B2B 2B2B FC00 0000 0000 0000 0000"
	$"00FC FCFC FCFC"
};

resource 'ics8' (131) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"FCFC FCFC FCFC FCFC FCFC FCFC FC00 0000"
	$"FCF5 F5F5 F5F5 F5F5 F5F5 F5F5 FCFC 0000"
	$"FCF5 F9FF FFFF FFFF F9F5 F5F5 FC2B FC00"
	$"FCF5 FF2B 2B2B 2B2B FFF5 F5F5 FCFC FCFC"
	$"FCF5 FF2B FFFF 002B FFF5 F5F5 FFF5 F5FC"
	$"FCF5 FF2B FF2A 002B FFF5 FFFF FFFF F5FC"
	$"FCF5 FF2B FF2A 002B FFF5 F5F5 FFF5 F5FC"
	$"FCF5 FF2B 0000 002B FFF5 F5FF F5F5 F5FC"
	$"FCF5 FFE3 2BFF FF2B FFF5 FFFF FFFF F5FC"
	$"FCF5 FF2B 2B2B 2B2B FFF5 F5FF F5F5 F5FC"
	$"FCF5 F5FF FFFF FFFF F5F5 F5F5 F5F5 F5FC"
	$"FCF5 F5F5 F5F5 F5F5 F5F5 F5F5 F5F5 F5FC"
	$"FCFC FCFC FCFC FCFC FCFC FCFC FFFF FCFC"
};

resource 'ics8' (132) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00F9 FFFF FFFF FFF9 0000 0000 0000 0000"
	$"00FF 2B2B 2B2B 2BFF 0000 FF00 00FF 0000"
	$"00FF 2BFC FCF5 2BFF 00FF FFFF FFFF FF00"
	$"00FF 2BFC 2AF5 2BFF 0000 FF00 00FF 0000"
	$"00FF 2BFC 2AF5 2BFF 0000 FF00 00FF 0000"
	$"00FF 2BF5 F5F5 2BFF 00FF FFFF FFFF FF00"
	$"00FF 2B2B FCFC 2BFF 0000 FF00 00FF 0000"
	$"00FF 2B2B 2B2B 2BFF 0000 0000 0000 0000"
	$"0000 FFFF FFFF FF00 0000 0000 0000 0000"
	$"0000 0000 FF00 0000 0000 0000 0000 0000"
	$"0000 00FF 00FF 0000 0000 0000 0000 0000"
	$"0000 00FF 00FF 0000 0000 0000 0000 0000"
	$"2BF9 FC00 0000 FCFC FCF9 F9F9 2B2B 2B"
};

resource 'ics8' (133) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00F9 FFFF FFFF FFF9 0000 0000 0000 0000"
	$"00FF 2B2B 2B2B 2BFF 0000 FF00 0000 FF00"
	$"00FF 2BFC FCF5 2BFF 00FF 00FF 00FF 00FF"
	$"00FF 2BFC 2AF5 2BFF 0000 FFFF FFFF FF00"
	$"00FF 2BFC 2AF5 2BFF 0000 00FF 00FF 0000"
	$"00FF 2BF5 F5F5 2BFF 0000 FFFF FFFF FF00"
	$"00FF 2B2B FCFC 2BFF 00FF 00FF 00FF 00FF"
	$"00FF 2B2B 2B2B 2BFF 0000 FF00 0000 FF00"
	$"0000 FFFF FFFF FF00 0000 0000 0000 0000"
	$"0000 0000 FF00 0000 0000 0000 0000 0000"
	$"0000 00FF 00FF 0000 0000 0000 0000 0000"
	$"0000 00FF 00FF 0000 0000 0000 0000 0000"
	$"2BF9 FC00 0000 FCFC FCF9 F9F9 2B2B 2B"
};

resource 'ics8' (134) {
	$"5555 F655 55F7 5555 5555 55F8 F755 5555"
	$"F8F7 2BF8 F8F6 F855 5555 F8F7 F5F8 00F7"
	$"F8F5 F755 55F8 F8F5 F8F5 F855 55F8 F62B"
	$"F82B 7A55 F8F6 00F6 F8F5 00F8 5555 2B2B"
	$"F7F6 F8F8 F6F7 F8F8 F7F7 F8F6 F8F8 F6F7"
	$"F700 F5F5 F500 00F5 F600 F7F6 F500 00F8"
	$"55F6 00F5 0000 F5FC 2BF6 FBF6 F5F5 7A55"
	$"557A 7A2B 00F6 F5F6 0000 F7F7 F5F7 A455"
	$"5555 55F5 F8F7 0000 F6F6 F7F8 F856 5555"
	$"5555 F8F7 F6F5 00F6 F7F8 F500 F755 5555"
	$"5555 55F6 F5F7 00F6 F600 00F8 7A55 5555"
	$"5555 55F7 00F6 F600 F7F8 F8F5 5555 5555"
	$"5555 5555 F7F5 F700 00F6 F7F9 5555 5555"
	$"5555 5555 56F6 F5F8 00F5 F555 5555 5555"
	$"5555 5555 5555 55F9 FAF8 7A55 5555 5555"
	$"5555 5555 5555 5555 F955 5555 5555 5555"
};

resource 'ics8' (135) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 FCFC FCFC 0000 0000 0000"
	$"0000 0000 00FC 0000 0000 FC00 0000 0000"
	$"0000 0000 00FC 0000 0000 FC00 0000 0000"
	$"0000 0000 00FC 0000 0000 FC00 0000 0000"
	$"0000 0000 F9F9 F9F9 FAFB FCFF 0000 0000"
	$"0000 0000 F92B 00F9 FAFB FCFF 0000 0000"
	$"0000 0000 F92B 00F9 FAFB FCFF 0000 0000"
	$"0000 0000 F92B 00F9 FAFB FCFF 0000 0000"
	$"0000 0000 F92B 00F9 FAFB FCFF 0000 0000"
	$"0000 0000 F9FB FBFF FFFF FFFF"
};

data 'ictb' (150) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000"                                /* ........ */
};

data 'ictb' (190) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
};

data 'ictb' (200) {
	$"0000 0000 0000 0000 0000 0000"                      /* ............ */
};

data 'ictb' (273) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000"                                          /* .... */
};

data 'ictb' (280) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000"                                          /* .... */
};

data 'ictb' (290) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
};

data 'ictb' (500) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000"                      /* ............ */
};

data 'ictb' (501) {
	$"0000 0000 0000 0000"                                /* ........ */
};

data 'ictb' (1001) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000"                                /* ........ */
};

data 'ictb' (1002) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
};

data 'ictb' (1023) {
	$"0000 0000 0000 0000 0000 0000"                      /* ............ */
};

data 'ictb' (5500) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000"                      /* ............ */
};

data 'ictb' (7001) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000"                                /* ........ */
};

data 'ictb' (7003) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000"                      /* ............ */
};

data 'ictb' (7004) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000"                      /* ............ */
};

data 'ictb' (10001) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000"                                /* ........ */
};

data 'ictb' (10002) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0005 001C 0000 0000 0001 0000"            /* ................ */
	$"0009 0000 0000 0000 FFFF FFFF FFFF 0000"            /* .∆......ˇˇˇˇˇˇ.. */
};

data 'ictb' (10003) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0005 001C 0000 0000 0001 0000"            /* ................ */
	$"0009 0000 0000 0000 FFFF FFFF FFFF 0000"            /* .∆......ˇˇˇˇˇˇ.. */
};

data 'ictb' (10004) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000"                                          /* .... */
};

data 'ictb' (10010) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000"                                          /* .... */
};

data 'ictb' (10011) {
	$"0000 0000 0000 0000 0000 0000"                      /* ............ */
};

data 'ictb' (10012) {
	$"0000 0000 0000 0000"                                /* ........ */
};

resource 'mctb' (1) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (259) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (515) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (2001) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (2002) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (2003) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (2004) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (2005) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (2008) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

resource 'mctb' (2010) {
	{	/* array MCTBArray: 1 elements */
		/* [1] */
		mctbLast, 0,
		{	/* array: 4 elements */
			/* [1] */
			0, 0, 0,
			/* [2] */
			0, 0, 0,
			/* [3] */
			0, 0, 0,
			/* [4] */
			0, 0, 0
		}
	}
};

data 'pltt' (9999, "16 ANSI Colors") {
	$"0010 0000 0000 C006 0000 0000 0599 A508"            /* ......¿......ô•. */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"8B00 0000 0000 0800 0000 0000 0000 0000"            /* ã............... */
	$"0000 8B00 0000 0000 0000 0000 0000 0000"            /* ..ã............. */
	$"8B00 8B00 0000 0000 0000 0000 0000 0000"            /* ã.ã............. */
	$"0000 4A00 8B00 0000 0000 0000 0000 0000"            /* ..J.ã........... */
	$"8B00 0000 8B00 0000 0000 0000 0000 0000"            /* ã...ã........... */
	$"0000 8B00 8B00 0000 0000 0000 0000 0000"            /* ..ã.ã........... */
	$"8B00 8B00 8B00 0000 0000 0000 0000 0000"            /* ã.ã.ã........... */
	$"4D00 4D00 4D00 0002 0000 0000 0000 0000"            /* M.M.M........... */
	$"FFFF 0000 0000 0002 0000 0000 0000 0000"            /* ˇˇ.............. */
	$"0000 FFFF 0000 0002 0000 0000 0000 0000"            /* ..ˇˇ............ */
	$"FFFF FFFF 0000 0002 0000 0000 0000 0000"            /* ˇˇˇˇ............ */
	$"0000 75FF FFFF 0000 0000 0000 0000 0000"            /* ..uˇˇˇ.......... */
	$"FFFF 0000 FFFF 0002 0000 0000 0000 0000"            /* ˇˇ..ˇˇ.......... */
	$"0000 FFFF FFFF 0002 0000 0000 0000 0000"            /* ..ˇˇˇˇ.......... */
	$"FFFF FFFF FFFF 0002 0000 0000 0000 0000"            /* ˇˇˇˇˇˇ.......... */
};

data 'taBL' (170, "Scandinavian 7bit åäö") {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A80 8581 5E5F"            /* PQRSTUVWXYZÄÖÅ^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A8A 9A8C 7E7F"            /* pqrstuvwxyzäöå~. */
	$"4141 4345 4E4F 5561 6161 6161 6163 6565"            /* AACENOUaaaaaacee */
	$"6565 6969 6969 6E6F 6F6F 6F6F 7575 7575"            /* eeiiiinooooouuuu */
	$"2B6F 634C 536F 5053 5243 2260 2223 414F"            /* +ocLSoPSRC"`"#AO */
	$"3823 3C3E 5975 6453 5070 5361 6F4F 616F"            /* 8#<>YudSPpSaoOao */
	$"3F69 2D56 663D 643C 3E2E 2041 414F 4F6F"            /* ?i-Vf=d<>. AAOOo */
	$"2D2D 2222 6060 2F6F 793D 2020 2020 2020"            /* --""``/oy=       */
	$"2020 2020 2020 2020 2020 2020 2020 2020"            /*                  */
	$"2020 2020 2020 2020 2020 2020 2020 2020"            /*                  */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"5B5D 4340 4E5C 7E61 6161 7B61 7D63 7E65"            /* []C@N\~aaa{a}c~e */
	$"6565 6969 6969 6E6F 6F6F 7C7C 7575 7575"            /* eeiiiinooo||uuuu */
	$"2B6F 634C 536F 5053 5243 5427 2223 414F"            /* +ocLSoPSRCT'"#AO */
	$"3823 3C3E 5975 6453 5070 5361 6F4F 616F"            /* 8#<>YudSPpSaoOao */
	$"3F21 2D56 663D 643C 3E2E 2041 414F 4F6F"            /* ?!-Vf=d<>. AAOOo */
	$"2D2D 2222 2727 2F6F 7959 2F6F 3C3E 6666"            /* --""''/oyY/o<>ff */
	$"2B2E 2722 2541 4541 4545 4949 4949 4F4F"            /* +.'"%AEAEEIIIIOO */
	$"204F 5555 5569 5E7E 2D5E 2E6F 2C22 2C5E"            /*  OUUUi^~-^.o,",^ */
};

data 'taBL' (261, "NextStep") {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"CACB E7E5 CC80 8182 E983 E6E8 EDEA EBEC"            /*  ÀÁÂÃÄÅÇÈÉÊËÌÍÎÏ */
	$"2084 F1EE EFCD 85F4 F2F3 8620 20B5 20D6"            /*  ÑÒÓÔÕÖÙÚÛÜ  µ ÷ */
	$"A9C1 A2A3 DAB4 C4A4 DB20 D2C7 DCDD DEDF"            /* ©¡¢£⁄¥ƒ§€ “«‹›ﬁﬂ */
	$"A8D0 A0E0 E120 A6A5 E2E3 D3C8 C9E4 C2C0"            /* ®–†‡· ¶•‚„”»…‰¬¿ */
	$"20D4 ABF6 F7F8 F9FA AC20 FBFC 20FD FEFF"            /*  ‘´ˆ˜¯˘˙¨ ˚¸ ˝˛ˇ */
	$"D1B1 2020 2088 8789 8B8A 8C8D 8F8E 9091"            /* —±   àáâãäåçèéêë */
	$"93AE 92BB 9495 2096 20AF CEBC 9897 999B"            /* ìÆíªîï ñ ØŒºòóôõ */
	$"9ABE 9D9C 9EF5 9F20 20BF CFA7 20D8 2020"            /* öæùúûıü  øœß ÿ   */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"8586 8789 9196 9AD6 D5D7 D9D8 DADB DDDC"            /* ÖÜáâëñö÷’◊Ÿÿ⁄€›‹ */
	$"DEDF E2E0 E4E5 E7ED ECEE F0EF F3F2 F4F6"            /* ﬁﬂ‚‡‰ÂÁÌÏÓÔÛÚÙˆ */
	$"B220 A2A3 A7B7 B6FB B0A0 20C2 C820 E1E9"            /* ≤ ¢£ß∑∂˚∞† ¬» ·È */
	$"20D1 2020 A59D 2020 2020 20E3 EB20 F1F9"            /*  —  •ù     „Î Ò˘ */
	$"BFA1 BE20 A620 20AB BBBC 8081 8495 EAFA"            /* ø°æ ¶  ´ªºÄÅÑïÍ˙ */
	$"B1D0 AABA C120 9F20 FD20 A4A8 ACAD AEAF"            /* ±–™∫¡ ü ˝ §®¨≠ÆØ */
	$"B3B4 B8B9 BD83 8A82 8B88 8D8E 8F8C 9394"            /* ≥¥∏πΩÉäÇãàçéèåìî */
	$"2092 9899 97F5 C3C4 C5C6 C7CA CBCD CECF"            /*  íòôóı√ƒ≈∆« ÀÕŒœ */
};

data 'taBL' (263, "ISO 8859-1") {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"8081 8283 8485 8687 8889 8A8B 8C8D 8E8F"            /* ÄÅÇÉÑÖÜáàâäãåçéè */
	$"9091 9293 9495 9697 9899 9A9B 9C9D 209F"            /* êëíìîïñóòôöõúù ü */
	$"CAC1 A2A3 DBB4 CFA4 ACA9 BBC7 C2D0 A8F8"            /*  ¡¢£€¥œ§¨©ª«¬–®¯ */
	$"A1B1 D3D2 ABB5 A6E1 FCD5 BCC8 B9B8 B2C0"            /* °±”“´µ¶·¸’º»π∏≤¿ */
	$"CBE7 E5CC 8081 AE82 E983 E6E8 EDEA EBEC"            /* ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ */
	$"DC84 F1EE EFCD 85D7 AFF4 F2F3 86A0 DEA7"            /* ‹ÑÒÓÔÕÖ◊ØÙÚÛÜ†ﬁß */
	$"8887 898B 8A8C BE8D 8F8E 9091 9392 9495"            /* àáâãäåæçèéêëìíîï */
	$"DD96 9897 999B 9AD6 BF9D 9C9E 9FE0 DFD8"            /* ›ñòóôõö÷øùúûü‡ﬂÿ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"C4C5 C7C9 D1D6 DCE1 E0E2 E4E3 E5E7 E9E8"            /* ƒ≈«…—÷‹·‡‚‰„ÂÁÈË */
	$"EAEB EDEC EEEF F1F3 F2F4 F6F5 FAF9 FBFC"            /* ÍÎÌÏÓÔÒÛÚÙˆı˙˘˚¸ */
	$"DDB0 A2A3 A720 B6DF AEA9 20B4 A820 C6D8"            /* ›∞¢£ß ∂ﬂÆ© ¥® ∆ÿ */
	$"20B1 BE20 A5B5 2020 BDBC 20AA BA20 E6F8"            /*  ±æ •µ  Ωº ™∫ Ê¯ */
	$"BFA1 AC20 2020 20AB BB20 A0C0 C3D5 20A6"            /* ø°¨    ´ª †¿√’ ¶ */
	$"AD20 B3B2 20B9 F7D7 FF20 20A4 D0F0 DEFE"            /* ≠ ≥≤ π˜◊ˇ  §–ﬁ˛ */
	$"FDB7 2020 20C2 CAC1 CBC8 CDCE CFCC D3D4"            /* ˝∑   ¬ ¡À»ÕŒœÃ”‘ */
	$"20D2 DADB D920 2020 AF20 2020 B820 2020"            /*  “⁄€Ÿ   Ø   ∏    */
};

data 'taBL' (265, "DEC Multinational") {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"8081 8283 8485 8687 8889 8A8B 8C8D 8E8F"            /* ÄÅÇÉÑÖÜáàâäãåçéè */
	$"9091 9293 9495 9697 9899 9A9B 9C9D 9E9F"            /* êëíìîïñóòôöõúùûü */
	$"A0C1 A2A3 A4B4 A6A4 DBA9 BBC7 ACAD AEAF"            /* †¡¢£§¥¶§€©ª«¨≠ÆØ */
	$"A1B1 B2B3 B4B5 A6E1 B8F5 BCC8 BCBD BEC0"            /* °±≤≥¥µ¶·∏ıº»ºΩæ¿ */
	$"CBE7 E5CC 8081 AE82 E983 E6E8 EDEA EBEC"            /* ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ */
	$"D084 F1EE EFCD 85CE AFF4 F2F3 86D9 DEA7"            /* –ÑÒÓÔÕÖŒØÙÚÛÜŸﬁß */
	$"8887 898B 8A8C BE8D 8F8E 9091 9392 9495"            /* àáâãäåæçèéêëìíîï */
	$"F096 9897 999B 9ACF BF9D 9C9E 9FD8 FEFF"            /* ñòóôõöœøùúûüÿ˛ˇ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"C4C5 C7C9 D1D6 DCE1 E0E2 E4E3 E5E7 E9E8"            /* ƒ≈«…—÷‹·‡‚‰„ÂÁÈË */
	$"EAEB EDEC EEEF F1F3 F2F4 F6F5 FAF9 FBFC"            /* ÍÎÌÏÓÔÒÛÚÙˆı˙˘˚¸ */
	$"20B0 A2A3 A720 B6DF 20A9 2020 2020 C6D8"            /*  ∞¢£ß ∂ﬂ ©    ∆ÿ */
	$"20B1 2020 A5B5 2020 2020 20AA BA20 E6F8"            /*  ±  •µ     ™∫ Ê¯ */
	$"BFA1 2020 2020 20AB BB20 20C0 C3D5 D7F7"            /* ø°     ´ª  ¿√’◊˜ */
	$"2020 2020 2020 2020 FDDD 20A8 2020 2020"            /*         ˝› ®     */
	$"20B7 2020 20C2 CAC1 CBC8 CDCE CFCC D3D4"            /*  ∑   ¬ ¡À»ÕŒœÃ”‘ */
	$"20D2 DADB D9B9 2020 2020 2020 2020 2020"            /*  “⁄€Ÿπ           */
};

data 'taBL' (267, "PC 850") {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"829F 8E89 8A88 8C8D 9091 8F95 9493 8081"            /* ÇüéâäàåçêëèïîìÄÅ */
	$"83BE AE99 9A98 9E9D D885 86BF A3AF D7C4"            /* ÉæÆôöòûùÿÖÜø£Ø◊ƒ */
	$"8792 979C 9684 BBBC C0A8 C2B8 B9C1 C7C8"            /* áíóúñÑªº¿®¬∏π¡«» */
	$"FDFE FFC6 F7E7 E5CB A9BD A5B0 B7A2 B4E3"            /* ˝˛ˇ∆˜ÁÂÀ©Ω•∞∑¢¥„ */
	$"E4FA F9F6 CEFB 8BCC B3AD C5C3 BAAA C9DB"            /* ‰˙˘ˆŒ˚ãÃ≥≠≈√∫™…€ */
	$"DDDC E6E8 E9F5 EAEB ECF0 E2D9 D4CF EDD1"            /* ›‹ÊËÈıÍÎÏ‚Ÿ‘œÌ— */
	$"EEA7 EFF1 9BCD B5DF DEF2 F3F4 E0A0 F8AB"            /* ÓßÔÒõÕµﬂﬁÚÛÙ‡†¯´ */
	$"D0B1 B6B2 A6A4 D6FC A1AC E1D5 D2D3 DACA"            /* –±∂≤¶§÷¸°¨·’“”⁄  */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"8E8F 8090 A599 9AA0 8583 84C6 8687 828A"            /* éèÄê•ôö†ÖÉÑ∆ÜáÇä */
	$"8889 A18D 8C8B A4A2 9593 94E4 A397 9681"            /* àâ°çåã§¢ïìî‰£óñÅ */
	$"EDF8 BD9C F5BA F4E1 A9B8 CDEF FEC9 929D"            /* Ì¯Ωúı∫Ù·©∏ÕÔ˛…íù */
	$"BBF1 F3C8 BEE6 F2BC ABAC CCA6 A7B9 919B"            /* ªÒÛ»æÊÚº´¨Ã¶ßπëõ */
	$"A8AA ACCB 9FCA B3AE AFCE FFB7 C7E5 C4DD"            /* ®™¨Àü ≥ÆØŒˇ∑«Âƒ› */
	$"F0DF FCFD DCFB F69E 98DB FECF D1D0 E8E7"            /* ﬂ¸˝‹˚ˆûò€˛œ—–ËÁ */
	$"ECFA DABF C0B6 D2B5 B3D4 D6D7 D8DE E0E2"            /* Ï˙⁄ø¿∂“µ≥‘÷◊ÿﬁ‡‚ */
	$"D9E3 E9EA EBD5 C3B4 EEC2 C1C5 F7B0 B1B2"            /* Ÿ„ÈÍÎ’√¥Ó¬¡≈˜∞±≤ */
};

data 'taBL' (268, "US ASCII") {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"4141 4345 4E4F 5561 6161 6161 6163 6565"            /* AACENOUaaaaaacee */
	$"6565 6969 6969 6E6F 6F6F 6F6F 7575 7575"            /* eeiiiinooooouuuu */
	$"2B6F 634C 536F 5053 5243 2260 2223 414F"            /* +ocLSoPSRC"`"#AO */
	$"3823 3C3E 5975 6453 5070 5361 6F4F 616F"            /* 8#<>YudSPpSaoOao */
	$"3F69 2D56 663D 643C 3E2E 2041 414F 4F6F"            /* ?i-Vf=d<>. AAOOo */
	$"2D2D 2222 6060 2F6F 793D 2020 2020 2020"            /* --""``/oy=       */
	$"2020 2020 2020 2020 2020 2020 2020 2020"            /*                  */
	$"2020 2020 2020 2020 2020 2020 2020 2020"            /*                  */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"4141 4345 4E4F 5561 6161 6161 6163 6565"            /* AACENOUaaaaaacee */
	$"6565 6969 6969 6E6F 6F6F 6F6F 7575 7575"            /* eeiiiinooooouuuu */
	$"2B6F 634C 536F 5053 5243 5427 2223 414F"            /* +ocLSoPSRCT'"#AO */
	$"3823 3C3E 5975 6453 5070 5361 6F4F 616F"            /* 8#<>YudSPpSaoOao */
	$"3F21 2D56 663D 643C 3E2E 2041 414F 4F6F"            /* ?!-Vf=d<>. AAOOo */
	$"2D2D 2222 2727 2F6F 7959 2F6F 3C3E 6666"            /* --""''/oyY/o<>ff */
	$"2B2E 2722 2541 4541 4545 4949 4949 4F4F"            /* +.'"%AEAEEIIIIOO */
	$"204F 5555 5569 5E7E 2D5E 2E6F 2C22 2C5E"            /*  OUUUi^~-^.o,",^ */
};

data 'taBL' (269, "ISO-8859-7", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"9693 2C66 2CC9 A0A0 5F98 C9C7 D45F 5F5F"            /* ñì,f,…††_ò…«‘___ */
	$"A2D4 D5D2 D396 D0D1 5F93 5FC8 5FFF 5F66"            /* ¢‘’“”ñ–—_ì_»_ˇ_f */
	$"CAD4 D592 24B4 9BAC 8CA9 5FC7 C22D A8D1"            /*  ‘’í$¥õ¨å©_«¬-®— */
	$"AEB1 8284 8B87 CDAF CED7 D8C8 D997 DADF"            /* Æ±ÇÑãáÕØŒ◊ÿ»Ÿó⁄ﬂ */
	$"FDB0 B5A1 A2B6 B7B8 A3B9 BAA4 BBC1 A5C3"            /* ˝∞µ°¢∂∑∏£π∫§ª¡•√ */
	$"A6C4 AAAA C6CB BCCC BEBF ABBD C0DB DCDD"            /* ¶ƒ™™∆ÀºÃæø´Ω¿€‹› */
	$"FEE1 E2E7 E4E5 FAE8 F5E9 EBEC EDEE EAEF"            /* ˛·‚Á‰Â˙ËıÈÎÏÌÓÍÔ */
	$"F0F2 F7F3 F4F9 E6F8 E3F6 FBFC DEE0 F1FF"            /* Ú˜ÛÙ˘Ê¯„ˆ˚¸ﬁ‡Òˇ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"4131 B245 B34F 55B5 6161 61B4 A863 6565"            /* A1≤E≥OUµaaa¥®cee */
	$"6565 A399 6969 2ABD 896F 6FA6 AD75 7575"            /* ee£ôii*Ωâoo¶≠uuu */
	$"2BC3 C4C8 CBCE D073 AEA9 D3DA A75F B0B7"            /* +√ƒ»ÀŒ–sÆ©”⁄ß_∞∑ */
	$"C1B1 3C3E 59C2 C5C6 C7C9 CACC D6DB D8D9"            /* ¡±<>Y¬≈∆«… Ã÷€ÿŸ */
	$"DCCD ACCF D17E D4AB BB5F A0D5 D7B6 B86F"            /* ‹Õ¨œ—~‘´ª_†’◊∂∏o */
	$"2DAF 2222 A1A2 2FB9 BABC BEDD DEDF FCBF"            /* -Ø""°¢/π∫ºæ›ﬁﬂ¸ø */
	$"FDE1 E2F8 E4E5 F6E3 E7E9 EEEA EBEC EDEF"            /* ˝·‚¯‰Âˆ„ÁÈÓÍÎÏÌÔ */
	$"F0FE F1F3 F4E8 F9F2 F7F5 E6FA FBC0 E0FF"            /* ˛ÒÛÙË˘Ú˜ıÊ˙˚¿‡ˇ */
};

data 'taBL' (270, "Windows-1253", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"9693 2C66 2CC9 A0A0 5F98 C9C7 D45F 5F5F"            /* ñì,f,…††_ò…«‘___ */
	$"A2D4 D5D2 D396 D0D1 5F93 5FC8 5FFF 5F66"            /* ¢‘’“”ñ–—_ì_»_ˇ_f */
	$"CA87 CD92 24B4 9BAC 8CA9 5FC7 C22D A8D1"            /*  áÕí$¥õ¨å©_«¬-®— */
	$"AEB1 8284 8BED ACAF CED7 D8C8 D997 DADF"            /* Æ±ÇÑãÌ¨ØŒ◊ÿ»Ÿó⁄ﬂ */
	$"FDB0 B5A1 A2B6 B7B8 A3B9 BAA4 BBC1 A5C3"            /* ˝∞µ°¢∂∑∏£π∫§ª¡•√ */
	$"A6C4 AAAA C6CB BCCC BEBF ABBD C0DB DCDD"            /* ¶ƒ™™∆ÀºÃæø´Ω¿€‹› */
	$"FEE1 E2E7 E4E5 FAE8 F5E9 EBEC EDEE EAEF"            /* ˛·‚Á‰Â˙ËıÈÎÏÌÓÍÔ */
	$"F0F2 F7F3 F4F9 E6F8 E3F6 FBFC DEE0 F1FF"            /* Ú˜ÛÙ˘Ê¯„ˆ˚¸ﬁ‡Òˇ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"4131 B245 B34F 55A1 6161 61B4 A863 6565"            /* A1≤E≥OU°aaa¥®cee */
	$"6565 A399 6969 95BD 896F 6FA6 AD75 7575"            /* ee£ôiiïΩâoo¶≠uuu */
	$"86C3 C4C8 CBCE D073 AEA9 D3DA A75F B0B7"            /* Ü√ƒ»ÀŒ–sÆ©”⁄ß_∞∑ */
	$"C1B1 3C3E A5C2 C5C6 C7C9 CACC D6DB D8D9"            /* ¡±<>•¬≈∆«… Ã÷€ÿŸ */
	$"DCCD ACCF D17E D4AB BB85 A0D5 D7A2 B86F"            /* ‹Õ¨œ—~‘´ªÖ†’◊¢∏o */
	$"9697 9394 9192 2FB9 BABC BEDD DEDF FCBF"            /* ñóìîëí/π∫ºæ›ﬁﬂ¸ø */
	$"FDE1 E2F8 E4E5 F6E3 E7E9 EEEA EBEC EDEF"            /* ˝·‚¯‰Âˆ„ÁÈÓÍÎÏÌÔ */
	$"F0FE F1F3 F4E8 F9F2 F7F5 E6FA FBC0 E0FF"            /* ˛ÒÛÙË˘Ú˜ıÊ˙˚¿‡ˇ */
};

data 'taBL' (271, "ISO-IR-111 (KOI-8)", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"A5AA 2CC4 D7C9 A0A0 C35F C9C7 D45F 5FB6"            /* •™,ƒ◊…††√_…«‘__∂ */
	$"C6D4 D5D2 D3A5 D0D1 5FAA 5FC8 5F5F 5FC4"            /* ∆‘’“”•–—_™_»___ƒ */
	$"CAAC AFDE B9CF B4BB C0BD BFCC CE2D D9DB"            /*  ¨Øﬁπœ¥ª¿ΩøÃŒ-Ÿ€ */
	$"DCAB AEDD B8C1 A7BA B7BC BECB CDFF D8DA"            /* ‹´Æ›∏¡ß∫∑ºæÀÕˇÿ⁄ */
	$"FEE0 E1F6 E4E5 F4E3 F5E8 E9EA EBEC EDEE"            /* ˛‡·ˆ‰ÂÙ„ıËÈÍÎÏÌÓ */
	$"EFDF F0F1 F2F3 E6E2 FCFB E7F8 FDF9 F7FA"            /* ÔﬂÒÚÛÊ‚¸˚Á¯˝˘˜˙ */
	$"9E80 8196 8485 9483 9588 898A 8B8C 8D8E"            /* ûÄÅñÑÖîÉïàâäãåçé */
	$"8F9F 9091 9293 8682 9C9B 8798 9D99 979A"            /* èüêëíìÜÇúõáòùôóö */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"E1E2 F7E7 E4E5 F6FA E9EA EBEC EDEE EFF0"            /* ·‚˜Á‰Âˆ˙ÈÍÎÏÌÓÔ */
	$"F2F3 F4F5 E6E8 E3FE FBFD FFF9 F8FC E0F1"            /* ÚÛÙıÊË„˛˚˝ˇ˘¯¸‡Ò */
	$"2B5E B24C 5F2A 5FB6 5263 99B1 A12D B2A2"            /* +^≤L_*_∂Rcô±°-≤¢ */
	$"5F5F 3C3E A675 A2B8 B4A4 B7A7 B9A9 BAAA"            /* __<>¶u¢∏¥§∑ßπ©∫™ */
	$"A8B5 2D5F 667E 5F22 225F A0BB ABBC ACA5"            /* ®µ-_f~_""_†ª´º¨• */
	$"2D2D 2222 2727 2F22 BEAE BFAF B0B3 A3D1"            /* --""''/"æÆøØ∞≥£— */
	$"C1C2 D7C7 C4C5 D6DA C9CA CBCC CDCE CFD0"            /* ¡¬◊«ƒ≈÷⁄… ÀÃÕŒœ– */
	$"D2D3 D4D5 C6C8 C3DE DBDD DFD9 D8DC C0BD"            /* “”‘’∆»√ﬁ€›ﬂŸÿ‹¿Ω */
};

data 'taBL' (272, "ISO-8859-5", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"A5AA 2CC4 D7C9 A0A0 C35F C9C7 D45F 5FB6"            /* •™,ƒ◊…††√_…«‘__∂ */
	$"C6D4 D5D2 D3A5 D0D1 5FAA 5FC8 5F5F 5FC4"            /* ∆‘’“”•–—_™_»___ƒ */
	$"CADD ABAE B8C1 A7BA B7BC BECB CD2D D8DA"            /*  ›´Æ∏¡ß∫∑ºæÀÕ-ÿ⁄ */
	$"8081 8283 8485 8687 8889 8A8B 8C8D 8E8F"            /* ÄÅÇÉÑÖÜáàâäãåçéè */
	$"9091 9293 9495 9697 9899 9A9B 9C9D 9E9F"            /* êëíìîïñóòôöõúùûü */
	$"E0E1 E2E3 E4E5 E6E7 E8E9 EAEB ECED EEEF"            /* ‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔ */
	$"F0F1 F2F3 F4F5 F6F7 F8F9 FAFB FCFD FEDF"            /* ÒÚÛÙıˆ˜¯˘˙˚¸˝˛ﬂ */
	$"DCDE ACAF B9CF B4BB C0BD BFCC CEA4 D9DB"            /* ‹ﬁ¨Øπœ¥ª¿ΩøÃŒ§Ÿ€ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"B0B1 B2B3 B4B5 B6B7 B8B9 BABB BCBD BEBF"            /* ∞±≤≥¥µ∂∑∏π∫ªºΩæø */
	$"C0C1 C2C3 C4C5 C6C7 C8C9 CACB CCCD CECF"            /* ¿¡¬√ƒ≈∆«»… ÀÃÕŒœ */
	$"2B5E A34C FD2A FDA6 5263 99A2 F22D A3F3"            /* +^£L˝*˝¶Rcô¢Ú-£Û */
	$"5F5F 3C3E F675 F3A8 A4F4 A7F7 A9F9 AAFA"            /* __<>ˆuÛ®§Ùß˜©˘™˙ */
	$"F8A5 2D5F 667E 5F22 225F A0AB FBAC FCF5"            /* ¯•-_f~_""_†´˚¨¸ı */
	$"2D2D 2222 2727 2F22 AEFE AFFF F0A1 F1EF"            /* --""''/"Æ˛Øˇ°ÒÔ */
	$"D0D1 D2D3 D4D5 D6D7 D8D9 DADB DCDD DEDF"            /* –—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ */
	$"E0E1 E2E3 E4E5 E6E7 E8E9 EAEB ECED EE24"            /* ‡·‚„‰ÂÊÁËÈÍÎÏÌÓ$ */
};

data 'taBL' (273, "Windows-1251", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"ABAE 2CAF D7C9 A0A0 C35F BCC7 BECD CBDA"            /* ´Æ,Ø◊…††√_º«æÕÀ⁄ */
	$"ACD4 D5D2 D3A5 D0D1 5FAA BDC8 BFCE CCDB"            /* ¨‘’“”•–—_™Ω»øŒÃ€ */
	$"CAD8 D9B7 FFA2 7CA4 DDA9 B8C7 C22D A8BA"            /*  ÿŸ∑ˇ¢|§›©∏«¬-®∫ */
	$"A1B1 A7B4 B6B5 A62E DEDC B9C8 C0C1 CFBB"            /* °±ß¥∂µ¶.ﬁ‹π»¿¡œª */
	$"8081 8283 8485 8687 8889 8A8B 8C8D 8E8F"            /* ÄÅÇÉÑÖÜáàâäãåçéè */
	$"9091 9293 9495 9697 9899 9A9B 9C9D 9E9F"            /* êëíìîïñóòôöõúùûü */
	$"E0E1 E2E3 E4E5 E6E7 E8E9 EAEB ECED EEEF"            /* ‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔ */
	$"F0F1 F2F3 F4F5 F6F7 F8F9 FAFB FCFD FEDF"            /* ÒÚÛÙıˆ˜¯˘˙˚¸˝˛ﬂ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"C0C1 C2C3 C4C5 C6C7 C8C9 CACB CCCD CECF"            /* ¿¡¬√ƒ≈∆«»… ÀÃÕŒœ */
	$"D0D1 D2D3 D4D5 D6D7 D8D9 DADB DCDD DEDF"            /* –—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ */
	$"86B0 A54C A795 B6B2 AEA9 9980 905F 8183"            /* Ü∞•Lßï∂≤Æ©ôÄê_ÅÉ */
	$"5FB1 3C3E B3B5 B4A3 AABA AFBF 8A9A 8C9C"            /* _±<>≥µ¥£™∫Øøäöåú */
	$"BCBD AC5F 667E 5FAB BB85 A08E 9E8D 9DBE"            /* ºΩ¨_f~_´ªÖ†éûçùæ */
	$"9697 9394 9192 2F84 A1A2 8F9F B9A8 B8FF"            /* ñóìîëí/Ñ°¢èüπ®∏ˇ */
	$"E0E1 E2E3 E4E5 E6E7 E8E9 EAEB ECED EEEF"            /* ‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔ */
	$"F0F1 F2F3 F4F5 F6F7 F8F9 FAFB FCFD FEA4"            /* ÒÚÛÙıˆ˜¯˘˙˚¸˝˛§ */
};

data 'taBL' (274, "ISO-8859-2 (Latin-2)", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"A5AA E266 E3C9 A0A0 C35F E1DC E5E8 EB8F"            /* •™‚f„…††√_·‹ÂËÎè */
	$"C6D4 D5D2 D3A5 D0D1 5FAA E4DD E6E9 EC90"            /* ∆‘’“”•–—_™‰›ÊÈÏê */
	$"CA84 FFFC 24BB E5A4 ACE1 E5E8 8F2D EBFB"            /*  Ñˇ¸$ªÂ§¨·ÂËè-Î˚ */
	$"A188 2CB8 27BC E6FF 2CE4 E6E9 90AC ECFD"            /* °à,∏'ºÊˇ,‰ÊÈê¨Ï˝ */
	$"D9E7 81E7 80BD 8C8C 8983 A296 9DEA B191"            /* ŸÁÅÁÄΩååâÉ¢ñùÍ±ë */
	$"91C1 C5EE EFCC 85D7 DBF1 F2F4 86F8 E8A7"            /* ë¡≈ÓÔÃÖ◊€ÒÚÙÜ¯Ëß */
	$"DA87 8287 8ABE 8D8D 8B8E AB98 9E92 B493"            /* ⁄áÇáäæççãé´òûí¥ì */
	$"93C4 CB97 99CE 9AD6 DEF3 9CF5 9FF9 E9AC"            /* ìƒÀóôŒö÷ﬁÛúıü˘È¨ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"C4C2 E2C9 A1D6 DCE1 B1C8 E4E8 C6E6 E9AC"            /* ƒ¬‚…°÷‹·±»‰Ë∆ÊÈ¨ */
	$"BCCF EDEF CCEC CBF3 EBF4 F6F5 FACC ECFC"            /* ºœÌÔÃÏÀÛÎÙˆı˙ÃÏ¸ */
	$"2BB0 CA4C A72A A7DF 5263 99EA A85F 67CD"            /* +∞ Lß*ßﬂRcôÍ®_gÕ */
	$"EDCE 3C3E EE4B F05F B3A5 B5A5 B5C5 E5D2"            /* ÌŒ<>ÓK_≥•µ•µ≈Â“ */
	$"F2D1 2D5F F1D2 5F22 225F A0F2 D5D5 F5D4"            /* Ú—-_Ò“_""_†Ú’’ı‘ */
	$"2D2D 2222 2727 F7D7 F4C0 E0D8 2727 F8D8"            /* --""''˜◊Ù¿‡ÿ''¯ÿ */
	$"F8A9 2722 B9A6 B6C1 ABBB CDAE BEDB D3D4"            /* ¯©'"π¶∂¡´ªÕÆæ€”‘ */
	$"FBD9 DAF9 DBFB D9F9 DDFD 6BAF A3BF 47B7"            /* ˚Ÿ⁄˘€˚Ÿ˘›˝kØ£øG∑ */
};

data 'taBL' (275, "Windows-1250 (Latin-2)", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"A5AA E266 E3C9 A0A0 C35F E1DC E5E8 EB8F"            /* •™‚f„…††√_·‹ÂËÎè */
	$"C6D4 D5D2 D3A5 D0D1 5FAA E4DD E6E9 EC90"            /* ∆‘’“”•–—_™‰›ÊÈÏê */
	$"CAFF FFFC 2484 7CA4 ACA9 E5C7 C22D A8FB"            /*  ˇˇ¸$Ñ|§¨©Â«¬-®˚ */
	$"A15F 2CB8 2775 A62E 2C88 E6C8 BBAC BCFD"            /* °_,∏'u¶.,àÊ»ª¨º˝ */
	$"D9E7 81E7 80BD 8C8C 8983 A296 9DEA B191"            /* ŸÁÅÁÄΩååâÉ¢ñùÍ±ë */
	$"91C1 C5EE EFCC 85D7 DBF1 F2F4 86F8 E8A7"            /* ë¡≈ÓÔÃÖ◊€ÒÚÙÜ¯Ëß */
	$"DA87 8287 8ABE 8D8D 8B8E AB98 9E92 B493"            /* ⁄áÇáäæççãé´òûí¥ì */
	$"93C4 CB97 99CE 9AD6 DEF3 9CF5 9FF9 E9AC"            /* ìƒÀóôŒö÷ﬁÛúıü˘È¨ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"C4C2 E2C9 A5D6 DCE1 B9C8 E4E8 C6E6 E98F"            /* ƒ¬‚…•÷‹·π»‰Ë∆ÊÈè */
	$"9FCF EDEF CCEC CBF3 EBF4 F6F5 FACC ECFC"            /* üœÌÔÃÏÀÛÎÙˆı˙ÃÏ¸ */
	$"86B0 CA4C A795 B6DF AEA9 99EA A85F 67CD"            /* Ü∞ Lßï∂ﬂÆ©ôÍ®_gÕ */
	$"EDCE 3C3E EE4B F05F B3BC BEBC BEC5 E5D2"            /* ÌŒ<>ÓK_≥ºæºæ≈Â“ */
	$"F2D1 AC5F F1D2 5FAB BB85 A0F2 D5D5 F5D4"            /* Ú—¨_Ò“_´ªÖ†Ú’’ı‘ */
	$"9697 9394 9192 F7D7 F4C0 E0D8 8B9B F8D8"            /* ñóìîëí˜◊Ù¿‡ÿãõ¯ÿ */
	$"F88A 8284 9A8C 9CC1 8D9D CD8E 9EDB D3D4"            /* ¯äÇÑöåú¡çùÕéû€”‘ */
	$"FBD9 DAF9 DBFB D9F9 DDFD 6BAF A3BF 47A1"            /* ˚Ÿ⁄˘€˚Ÿ˘›˝kØ£øG° */
};

data 'taBL' (276, "ISO-8859-13 (Baltic)", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"A5AA E266 E3C9 A0A0 C35F C9DC D4AC FF2C"            /* •™‚f„…††√_…‹‘¨ˇ, */
	$"C6D4 D5D2 D3A5 D0D1 5FAA 5FDD 5F7E 5F5F"            /* ∆‘’“”•–—_™_›_~__ */
	$"CAD3 63A3 24E3 7CA4 85A9 DFC7 C22D A880"            /*  ”c£$„|§Ö©ﬂ«¬-®Ä */
	$"A15F 3233 D275 A62E 9A31 E0C8 2F2F 2F8A"            /* °_23“u¶.ö1‡»///ä */
	$"84AF 818C 80E7 A294 8983 8F96 FEB5 B1B9"            /* ÑØÅåÄÁ¢îâÉèñ˛µ±π */
	$"E1C1 BFEE CFCD 85D7 F6FC E5ED 86FB EBA7"            /* ·¡øÓœÕÖ◊ˆ¸ÂÌÜ˚Îß */
	$"88B0 828D 8A87 AB95 8B8E 9098 AEFA B4BA"            /* à∞Ççäá´ïãéêòÆ˙¥∫ */
	$"E4C4 C097 D89B 9AD6 F7B8 E6F0 9FFD ECD5"            /* ‰ƒ¿óÿõö÷˜∏Êü˝Ï’ */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"C4C2 E2C9 C0D6 DC61 E0C8 E4E8 C3E3 E9CA"            /* ƒ¬‚…¿÷‹a‡»‰Ë√„È  */
	$"EA44 6964 C7E7 CBF3 EBF4 F6F5 75C7 E7FC"            /* ÍDid«ÁÀÛÎÙˆıu«Á¸ */
	$"2BB0 C6A3 A72A B6DF AEA9 99E6 225F ECC1"            /* +∞∆£ß*∂ﬂÆ©ôÊ"_Ï¡ */
	$"E1CE 3C3E EECD 645F F9CF EFCF EF4C 6CD2"            /* ·Œ<>ÓÕd_˘œÔœÔLl“ */
	$"F2D1 AC5F F1D2 5FAB BB5F A0F2 D5D5 F5D4"            /* Ú—¨_Ò“_´ª_†Ú’’ı‘ */
	$"2D2D B4A1 FFFF F7D7 F452 72AA ABBB BAAA"            /* --¥°ˇˇ˜◊ÙRr™´ª∫™ */
	$"BAD0 2CA5 F0DA FA41 5474 49DE FEDB D3D4"            /* ∫–,•⁄˙ATtIﬁ˛€”‘ */
	$"FBD8 55F8 DBFB D8F8 5979 EDDD D9FD CC5E"            /* ˚ÿU¯€˚ÿ¯YyÌ›Ÿ˝Ã^ */
};

data 'taBL' (277, "Windows-1257 (Baltic)", purgeable) {
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"A5AA E266 E3C9 A0A0 C35F C9DC D4AC FF2C"            /* •™‚f„…††√_…‹‘¨ˇ, */
	$"C6D4 D5D2 D3A5 D0D1 5FAA 5FDD 5F7E 5F5F"            /* ∆‘’“”•–—_™_›_~__ */
	$"CA5F 63A3 245F 7CA4 85A9 DFC7 C22D A880"            /*  _c£$_|§Ö©ﬂ«¬-®Ä */
	$"A15F 3233 2775 A62E 9A31 E0C8 2F2F 2F8A"            /* °_23'u¶.ö1‡»///ä */
	$"84AF 818C 80E7 A294 8983 8F96 FEB5 B1B9"            /* ÑØÅåÄÁ¢îâÉèñ˛µ±π */
	$"E1C1 BFEE CFCD 85D7 F6FC E5ED 86FB EBA7"            /* ·¡øÓœÕÖ◊ˆ¸ÂÌÜ˚Îß */
	$"88B0 828D 8A87 AB95 8B8E 9098 AEFA B4BA"            /* à∞Ççäá´ïãéêòÆ˙¥∫ */
	$"E4C4 C097 D89B 9AD6 F7B8 E6F0 9FFD ECA1"            /* ‰ƒ¿óÿõö÷˜∏Êü˝Ï° */
	$"0001 0203 0405 0607 0809 0A0B 0C0D 0E0F"            /* .........∆¬..... */
	$"1011 1213 1415 1617 1819 1A1B 1C1D 1E1F"            /* ................ */
	$"2021 2223 2425 2627 2829 2A2B 2C2D 2E2F"            /*  !"#$%&'()*+,-./ */
	$"3031 3233 3435 3637 3839 3A3B 3C3D 3E3F"            /* 0123456789:;<=>? */
	$"4041 4243 4445 4647 4849 4A4B 4C4D 4E4F"            /* @ABCDEFGHIJKLMNO */
	$"5051 5253 5455 5657 5859 5A5B 5C5D 5E5F"            /* PQRSTUVWXYZ[\]^_ */
	$"6061 6263 6465 6667 6869 6A6B 6C6D 6E6F"            /* `abcdefghijklmno */
	$"7071 7273 7475 7677 7879 7A7B 7C7D 7E7F"            /* pqrstuvwxyz{|}~. */
	$"C4C2 E2C9 C0D6 DC61 E0C8 E4E8 C3E3 E9CA"            /* ƒ¬‚…¿÷‹a‡»‰Ë√„È  */
	$"EA44 6964 C7E7 CBF3 EBF4 F6F5 75C7 E7FC"            /* ÍDid«ÁÀÛÎÙˆıu«Á¸ */
	$"86B0 C6A3 A795 B6DF AEA9 99E6 8D5F ECC1"            /* Ü∞∆£ßï∂ﬂÆ©ôÊç_Ï¡ */
	$"E1CE 3C3E EECD 645F F9CF EFCF EF4C 6CD2"            /* ·Œ<>ÓÕd_˘œÔœÔLl“ */
	$"F2D1 AC5F F1D2 5FAB BB85 A0F2 D5D5 F5D4"            /* Ú—¨_Ò“_´ªÖ†Ú’’ı‘ */
	$"9697 9394 9192 F7D7 F452 72AA 8B9B BAAA"            /* ñóìîëí˜◊ÙRr™ãõ∫™ */
	$"BAD0 8284 F0DA FA41 5474 49DE FEDB D3D4"            /* ∫–ÇÑ⁄˙ATtIﬁ˛€”‘ */
	$"FBD8 55F8 DBFB D8F8 5979 EDDD D9FD CC8E"            /* ˚ÿU¯€˚ÿ¯YyÌ›Ÿ˝Ãé */
};

resource 'dftb' (7003) {
	versionZero {
		{	/* array FontStyle: 50 elements */
			/* [1] */
			skipItem {

			}			,
			/* [2] */
			skipItem {

			}			,
			/* [3] */
			skipItem {

			}			,
			/* [4] */
			skipItem {

			}			,
			/* [5] */
			skipItem {

			}			,
			/* [6] */
			skipItem {

			}			,
			/* [7] */
			skipItem {

			}			,
			/* [8] */
			skipItem {

			}			,
			/* [9] */
			skipItem {

			}			,
			/* [10] */
			skipItem {

			}			,
			/* [11] */
			skipItem {

			}			,
			/* [12] */
			skipItem {

			}			,
			/* [13] */
			skipItem {

			}			,
			/* [14] */
			skipItem {

			}			,
			/* [15] */
			skipItem {

			}			,
			/* [16] */
			skipItem {

			}			,
			/* [17] */
			skipItem {

			}			,
			/* [18] */
			skipItem {

			}			,
			/* [19] */
			skipItem {

			}			,
			/* [20] */
			skipItem {

			}			,
			/* [21] */
			skipItem {

			}			,
			/* [22] */
			skipItem {

			}			,
			/* [23] */
			skipItem {

			}			,
			/* [24] */
			skipItem {

			}			,
			/* [25] */
			skipItem {

			}			,
			/* [26] */
			skipItem {

			}			,
			/* [27] */
			skipItem {

			}			,
			/* [28] */
			skipItem {

			}			,
			/* [29] */
			skipItem {

			}			,
			/* [30] */
			skipItem {

			}			,
			/* [31] */
			skipItem {

			}			,
			/* [32] */
			skipItem {

			}			,
			/* [33] */
			skipItem {

			}			,
			/* [34] */
			skipItem {

			}			,
			/* [35] */
			skipItem {

			}			,
			/* [36] */
			skipItem {

			}			,
			/* [37] */
			skipItem {

			}			,
			/* [38] */
			skipItem {

			}			,
			/* [39] */
			skipItem {

			}			,
			/* [40] */
			skipItem {

			}			,
			/* [41] */
			skipItem {

			}			,
			/* [42] */
			skipItem {

			}			,
			/* [43] */
			skipItem {

			}			,
			/* [44] */
			skipItem {

			}			,
			/* [45] */
			skipItem {

			}			,
			/* [46] */
			skipItem {

			}			,
			/* [47] */
			skipItem {

			}			,
			/* [48] */
			skipItem {

			}			,
			/* [49] */
			skipItem {

			}			,
			/* [50] */
			skipItem {

			}		}
	}
};

resource 'dftb' (7001) {
	versionZero {
		{	/* array FontStyle: 43 elements */
			/* [1] */
			skipItem {

			}			,
			/* [2] */
			skipItem {

			}			,
			/* [3] */
			skipItem {

			}			,
			/* [4] */
			skipItem {

			}			,
			/* [5] */
			skipItem {

			}			,
			/* [6] */
			skipItem {

			}			,
			/* [7] */
			skipItem {

			}			,
			/* [8] */
			skipItem {

			}			,
			/* [9] */
			skipItem {

			}			,
			/* [10] */
			skipItem {

			}			,
			/* [11] */
			skipItem {

			}			,
			/* [12] */
			skipItem {

			}			,
			/* [13] */
			skipItem {

			}			,
			/* [14] */
			skipItem {

			}			,
			/* [15] */
			skipItem {

			}			,
			/* [16] */
			skipItem {

			}			,
			/* [17] */
			skipItem {

			}			,
			/* [18] */
			skipItem {

			}			,
			/* [19] */
			skipItem {

			}			,
			/* [20] */
			skipItem {

			}			,
			/* [21] */
			skipItem {

			}			,
			/* [22] */
			skipItem {

			}			,
			/* [23] */
			skipItem {

			}			,
			/* [24] */
			skipItem {

			}			,
			/* [25] */
			skipItem {

			}			,
			/* [26] */
			skipItem {

			}			,
			/* [27] */
			skipItem {

			}			,
			/* [28] */
			skipItem {

			}			,
			/* [29] */
			skipItem {

			}			,
			/* [30] */
			skipItem {

			}			,
			/* [31] */
			skipItem {

			}			,
			/* [32] */
			skipItem {

			}			,
			/* [33] */
			skipItem {

			}			,
			/* [34] */
			skipItem {

			}			,
			/* [35] */
			skipItem {

			}			,
			/* [36] */
			skipItem {

			}			,
			/* [37] */
			skipItem {

			}			,
			/* [38] */
			skipItem {

			}			,
			/* [39] */
			skipItem {

			}			,
			/* [40] */
			skipItem {

			}			,
			/* [41] */
			skipItem {

			}			,
			/* [42] */
			skipItem {

			}			,
			/* [43] */
			skipItem {

			}		}
	}
};

resource 'dftb' (7004) {
	versionZero {
		{	/* array FontStyle: 96 elements */
			/* [1] */
			skipItem {

			}			,
			/* [2] */
			skipItem {

			}			,
			/* [3] */
			skipItem {

			}			,
			/* [4] */
			skipItem {

			}			,
			/* [5] */
			skipItem {

			}			,
			/* [6] */
			skipItem {

			}			,
			/* [7] */
			skipItem {

			}			,
			/* [8] */
			skipItem {

			}			,
			/* [9] */
			skipItem {

			}			,
			/* [10] */
			skipItem {

			}			,
			/* [11] */
			skipItem {

			}			,
			/* [12] */
			skipItem {

			}			,
			/* [13] */
			skipItem {

			}			,
			/* [14] */
			skipItem {

			}			,
			/* [15] */
			skipItem {

			}			,
			/* [16] */
			skipItem {

			}			,
			/* [17] */
			skipItem {

			}			,
			/* [18] */
			skipItem {

			}			,
			/* [19] */
			skipItem {

			}			,
			/* [20] */
			skipItem {

			}			,
			/* [21] */
			skipItem {

			}			,
			/* [22] */
			skipItem {

			}			,
			/* [23] */
			skipItem {

			}			,
			/* [24] */
			skipItem {

			}			,
			/* [25] */
			skipItem {

			}			,
			/* [26] */
			skipItem {

			}			,
			/* [27] */
			skipItem {

			}			,
			/* [28] */
			skipItem {

			}			,
			/* [29] */
			skipItem {

			}			,
			/* [30] */
			skipItem {

			}			,
			/* [31] */
			skipItem {

			}			,
			/* [32] */
			skipItem {

			}			,
			/* [33] */
			skipItem {

			}			,
			/* [34] */
			skipItem {

			}			,
			/* [35] */
			skipItem {

			}			,
			/* [36] */
			skipItem {

			}			,
			/* [37] */
			skipItem {

			}			,
			/* [38] */
			skipItem {

			}			,
			/* [39] */
			skipItem {

			}			,
			/* [40] */
			skipItem {

			}			,
			/* [41] */
			skipItem {

			}			,
			/* [42] */
			skipItem {

			}			,
			/* [43] */
			skipItem {

			}			,
			/* [44] */
			skipItem {

			}			,
			/* [45] */
			skipItem {

			}			,
			/* [46] */
			skipItem {

			}			,
			/* [47] */
			skipItem {

			}			,
			/* [48] */
			skipItem {

			}			,
			/* [49] */
			skipItem {

			}			,
			/* [50] */
			skipItem {

			}			,
			/* [51] */
			skipItem {

			}			,
			/* [52] */
			skipItem {

			}			,
			/* [53] */
			skipItem {

			}			,
			/* [54] */
			skipItem {

			}			,
			/* [55] */
			skipItem {

			}			,
			/* [56] */
			skipItem {

			}			,
			/* [57] */
			skipItem {

			}			,
			/* [58] */
			skipItem {

			}			,
			/* [59] */
			skipItem {

			}			,
			/* [60] */
			skipItem {

			}			,
			/* [61] */
			skipItem {

			}			,
			/* [62] */
			skipItem {

			}			,
			/* [63] */
			skipItem {

			}			,
			/* [64] */
			skipItem {

			}			,
			/* [65] */
			skipItem {

			}			,
			/* [66] */
			skipItem {

			}			,
			/* [67] */
			skipItem {

			}			,
			/* [68] */
			skipItem {

			}			,
			/* [69] */
			skipItem {

			}			,
			/* [70] */
			skipItem {

			}			,
			/* [71] */
			skipItem {

			}			,
			/* [72] */
			skipItem {

			}			,
			/* [73] */
			skipItem {

			}			,
			/* [74] */
			skipItem {

			}			,
			/* [75] */
			skipItem {

			}			,
			/* [76] */
			skipItem {

			}			,
			/* [77] */
			skipItem {

			}			,
			/* [78] */
			skipItem {

			}			,
			/* [79] */
			skipItem {

			}			,
			/* [80] */
			skipItem {

			}			,
			/* [81] */
			skipItem {

			}			,
			/* [82] */
			skipItem {

			}			,
			/* [83] */
			skipItem {

			}			,
			/* [84] */
			skipItem {

			}			,
			/* [85] */
			skipItem {

			}			,
			/* [86] */
			skipItem {

			}			,
			/* [87] */
			skipItem {

			}			,
			/* [88] */
			skipItem {

			}			,
			/* [89] */
			skipItem {

			}			,
			/* [90] */
			skipItem {

			}			,
			/* [91] */
			skipItem {

			}			,
			/* [92] */
			skipItem {

			}			,
			/* [93] */
			skipItem {

			}			,
			/* [94] */
			skipItem {

			}			,
			/* [95] */
			skipItem {

			}			,
			/* [96] */
			skipItem {

			}		}
	}
};

