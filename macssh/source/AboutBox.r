resource 'DITL' (1000, purgeable) {
	{	/* array DITLarray: 4 elements */
		/* [1] */
		{0, 0, 250, 400},
		UserItem {
			disabled
		},
		/* [2] */
		{100, 220, 246, 396},
		UserItem {
			enabled
		},
		/* [3] */
		{229, 25, 244, 191},
		UserItem {
			enabled
		},
		/* [4] */
		{175, 10, 223, 212},
		UserItem {
			enabled
		}
	}
};

resource 'STR#' (1000) {
	{	/* array StringArray: 1 elements */
		/* [1] */
		"http://www.macssh.com/"
	}
};

data 'TEXT' (1000, "Credits") {
	$"0D0D 0D20 2020 2020 2020 5365 6375 7265"            /* ...       Secure */
	$"2053 6865 6C6C 2056 6572 7369 6F6E 2032"            /*  Shell Version 2 */
	$"0D20 2020 2020 2020 2020 436C 6965 6E74"            /* .         Client */
	$"2066 6F72 204D 6163 4F53 0D0D 2020 2020"            /*  for MacOS..     */
	$"2020 2020 2043 6F70 7972 6967 6874 20A9"            /*      Copyright � */
	$"3230 3030 2D32 3030 310D 2020 2020 2020"            /* 2000-2001.       */
	$"2020 2020 204A 6561 6E2D 5069 6572 7265"            /*      Jean-Pierre */
	$"CA53 7469 6572 6C69 6E0D 2020 2020 2020"            /* �Stierlin.       */
	$"2020 2020 203C 6368 6F6D 6269 6572 406D"            /*      <chombier@m */
	$"6163 2E63 6F6D 3E0D 0D0D 0D42 6173 6564"            /* ac.com>....Based */
	$"206F 6E20 4265 7474 6572 5465 6C6E 6574"            /*  on BetterTelnet */
	$"2C20 436F 7079 7269 6768 7420 A931 3939"            /* , Copyright �199 */
	$"372D 3139 3939 2052 6F6C 66CA 4272 6175"            /* 7-1999 Rolf�Brau */
	$"6E0D 3C72 6272 6175 6E40 6373 746F 6E65"            /* n.<rbraun@cstone */
	$"2E6E 6574 3E0D 0D42 6173 6564 206F 6E20"            /* .net>..Based on  */
	$"6C73 682C 2043 6F70 7972 6967 6874 20A9"            /* lsh, Copyright � */
	$"3230 3031 204E 6965 6C73 CA4D 9A6C 6C65"            /* 2001 Niels�M�lle */
	$"7220 3C6E 6973 7365 406C 7973 6174 6F72"            /* r <nisse@lysator */
	$"2E6C 6975 2E73 653E 0D0D 4261 7365 6420"            /* .liu.se>..Based  */
	$"6F6E 2047 5553 492C 2043 6F70 7972 6967"            /* on GUSI, Copyrig */
	$"6874 20A9 3139 3932 2D32 3030 3120 4D61"            /* ht �1992-2001 Ma */
	$"7474 6869 6173 CA4E 6565 7261 6368 6572"            /* tthias�Neeracher */
	$"0D3C 6E65 6572 6940 6775 6573 742E 6969"            /* .<neeri@guest.ii */
	$"732E 6565 2E65 7468 7A2E 6368 3E0D 0D4C"            /* s.ee.ethz.ch>..L */
	$"5348 2049 5320 4120 574F 524B 2049 4E20"            /* SH IS A WORK IN  */
	$"5052 4F47 5245 5353 2E20 5448 4552 4520"            /* PROGRESS. THERE  */
	$"4D41 5920 4245 2053 4552 494F 5553 2042"            /* MAY BE SERIOUS B */
	$"5547 5320 5448 4154 204D 414B 4520 4954"            /* UGS THAT MAKE IT */
	$"2054 4F54 414C 4C59 2049 4E53 4543 5552"            /*  TOTALLY INSECUR */
	$"452E 0D0D 436F 7079 7269 6768 743A 0D0D"            /* E...Copyright:.. */
	$"5468 6973 2070 726F 6772 616D 2069 7320"            /* This program is  */
	$"6672 6565 2073 6F66 7477 6172 653B 2079"            /* free software; y */
	$"6F75 2063 616E 2072 6564 6973 7472 6962"            /* ou can redistrib */
	$"7574 6520 6974 2061 6E64 2F6F 7220 6D6F"            /* ute it and/or mo */
	$"6469 6679 2069 7420 756E 6465 7220 7468"            /* dify it under th */
	$"6520 7465 726D 7320 6F66 2074 6865 2047"            /* e terms of the G */
	$"4E55 2047 656E 6572 616C 2050 7562 6C69"            /* NU General Publi */
	$"6320 4C69 6365 6E73 6520 6173 2070 7562"            /* c License as pub */
	$"6C69 7368 6564 2062 7920 7468 6520 4672"            /* lished by the Fr */
	$"6565 2053 6F66 7477 6172 6520 466F 756E"            /* ee Software Foun */
	$"6461 7469 6F6E 2E20 5365 6520 7468 6520"            /* dation. See the  */
	$"6669 6C65 2043 4F50 5949 4E47 2066 6F72"            /* file COPYING for */
	$"2064 6574 6169 6C73 2E0D 0D57 6172 7261"            /*  details...Warra */
	$"6E74 793A 0D0D 5468 6973 2070 726F 6772"            /* nty:..This progr */
	$"616D 2069 7320 6469 7374 7269 6275 7465"            /* am is distribute */
	$"6420 696E 2074 6865 2068 6F70 6520 7468"            /* d in the hope th */
	$"6174 2069 7420 7769 6C6C 2062 6520 7573"            /* at it will be us */
	$"6566 756C 2C20 6275 7420 7769 7468 6F75"            /* eful, but withou */
	$"7420 616E 7920 7761 7272 616E 7479 3B20"            /* t any warranty;  */
	$"7769 7468 6F75 7420 6576 656E 2074 6865"            /* without even the */
	$"2077 6172 7261 6E74 7920 6F66 206D 6572"            /*  warranty of mer */
	$"6368 616E 7461 6269 6C69 7479 206F 7220"            /* chantability or  */
	$"6669 746E 6573 7320 666F 7220 616E 7920"            /* fitness for any  */
	$"7075 7270 6F73 652E 2054 6865 2065 6E74"            /* purpose. The ent */
	$"6972 6520 7269 736B 206F 6620 6F70 6572"            /* ire risk of oper */
	$"6174 696F 6E20 7468 6973 2070 726F 6772"            /* ation this progr */
	$"616D 2C20 6173 2077 656C 6C20 6173 2074"            /* am, as well as t */
	$"6865 2072 6973 6B20 6F66 2071 7561 6C69"            /* he risk of quali */
	$"7479 2061 6E64 2070 6572 666F 726D 616E"            /* ty and performan */
	$"6365 2C20 7265 7369 6465 7320 736F 6C65"            /* ce, resides sole */
	$"6C79 2077 6974 6820 796F 7521 2049 6E20"            /* ly with you! In  */
	$"6E6F 2065 7665 6E74 2077 696C 6C20 616E"            /* no event will an */
	$"7920 6F66 2074 6865 2070 726F 6772 616D"            /* y of the program */
	$"2061 7574 686F 7273 2062 6520 6C69 6162"            /*  authors be liab */
	$"6C65 2066 6F72 2061 6E79 2064 616D 6167"            /* le for any damag */
	$"6520 6361 7573 6564 2062 7920 7468 6520"            /* e caused by the  */
	$"6F70 6572 6174 696F 6E2C 206F 7220 6E6F"            /* operation, or no */
	$"6E2D 6F70 6572 6174 696F 6E2C 206F 6620"            /* n-operation, of  */
	$"7468 6973 2070 726F 6772 616D 210D 0D0D"            /* this program!... */
	$"4672 6F6D 204E 6965 6C73 206F 7269 6769"            /* From Niels origi */
	$"6E61 6C20 646F 6373 3A0D 0D6C 7368 2069"            /* nal docs:..lsh i */
	$"7320 7772 6974 7465 6E20 6279 204E 6965"            /* s written by Nie */
	$"6C73 204D 9A6C 6C65 722C 2061 6E64 2064"            /* ls M�ller, and d */
	$"6973 7472 6962 7574 6564 2075 6E64 6572"            /* istributed under */
	$"2074 6865 2074 6572 6D73 2061 6E64 2063"            /*  the terms and c */
	$"6F6E 6469 7469 6F6E 7320 6F66 2074 6865"            /* onditions of the */
	$"2047 4E55 2047 656E 6572 616C 2050 7562"            /*  GNU General Pub */
	$"6C69 6320 4C69 6365 6E73 6520 2873 6565"            /* lic License (see */
	$"2074 6865 2066 696C 6520 434F 5059 494E"            /*  the file COPYIN */
	$"4720 666F 7220 6465 7461 696C 7329 2E20"            /* G for details).  */
	$"5365 7665 7261 6C20 7061 7274 7320 6F66"            /* Several parts of */
	$"206C 7368 2C20 696E 2070 6172 7469 6375"            /*  lsh, in particu */
	$"6C61 7220 7463 7066 6F72 7761 7264 696E"            /* lar tcpforwardin */
	$"672C 2070 7562 6C69 632D 6B65 7920 7573"            /* g, public-key us */
	$"6572 2061 7574 6865 6E74 6963 6174 696F"            /* er authenticatio */
	$"6E2C 2061 6E64 2074 6865 206C 7368 5F70"            /* n, and the lsh_p */
	$"726F 7879 2070 726F 6772 616D 2C20 7765"            /* roxy program, we */
	$"7265 2063 6F6E 7472 6962 7574 6564 2062"            /* re contributed b */
	$"7920 4261 6C87 7A73 2053 6368 6569 646C"            /* y Bal�zs Scheidl */
	$"6572 2E0D 0D4D 616E 7920 6F74 6865 7220"            /* er...Many other  */
	$"7065 6F70 6C65 2068 6176 6520 7772 6974"            /* people have writ */
	$"7465 6E20 6672 6565 2063 6F64 6520 7768"            /* ten free code wh */
	$"6963 6820 6973 2075 7365 6420 696E 206C"            /* ich is used in l */
	$"7368 2E0D 0D54 574F 4649 5348 2069 6D70"            /* sh...TWOFISH imp */
	$"6C65 6D65 6E74 6174 696F 6E20 6279 2052"            /* lementation by R */
	$"7575 6420 6465 2052 6F6F 696A 2E20 5265"            /* uud de Rooij. Re */
	$"6C65 6173 6564 2075 6E64 6572 2074 6865"            /* leased under the */
	$"204C 4750 4C2E 0D0D 424C 4F57 4649 5348"            /*  LGPL...BLOWFISH */
	$"2069 6D70 6C65 6D65 6E74 6174 696F 6E20"            /*  implementation  */
	$"6672 6F6D 2057 6572 6E65 7220 4B6F 6368"            /* from Werner Koch */
	$"2773 2061 6E64 2046 5346 2773 2047 4E55"            /* 's and FSF's GNU */
	$"2050 7269 7661 6379 2047 7561 7264 2E20"            /*  Privacy Guard.  */
	$"5265 6C65 6173 6564 2075 6E64 6572 2074"            /* Released under t */
	$"6865 2047 504C 2E0D 0D43 4153 5420 696D"            /* he GPL...CAST im */
	$"706C 656D 656E 7461 7469 6F6E 2062 7920"            /* plementation by  */
	$"5374 6576 6520 5265 6964 2E20 5265 6C65"            /* Steve Reid. Rele */
	$"6173 6564 2069 6E74 6F20 7468 6520 7075"            /* ased into the pu */
	$"626C 6963 2064 6F6D 6169 6E2E 0D0D 4445"            /* blic domain...DE */
	$"5320 696D 706C 656D 656E 7461 7469 6F6E"            /* S implementation */
	$"2062 7920 4461 6E61 204C 2E20 486F 772E"            /*  by Dana L. How. */
	$"2052 656C 6561 7365 6420 756E 6465 7220"            /*  Released under  */
	$"7468 6520 4C47 504C 2E0D 0D49 4445 4120"            /* the LGPL...IDEA  */
	$"696D 706C 656D 656E 7461 7469 6F6E 206F"            /* implementation o */
	$"7269 6769 6E61 6C6C 7920 6279 2058 6565"            /* riginally by Xee */
	$"6A69 6120 4C61 692C 206F 7074 696D 697A"            /* jia Lai, optimiz */
	$"6564 2062 7920 436F 6C69 6E20 506C 756D"            /* ed by Colin Plum */
	$"622E 2052 656C 6561 7365 6420 696E 746F"            /* b. Released into */
	$"2074 6865 2070 7562 6C69 6320 646F 6D61"            /*  the public doma */
	$"696E 2E0D 0D47 4D50 2C20 7468 6520 474E"            /* in...GMP, the GN */
	$"5520 6D75 6C74 6970 6C65 2070 7265 6369"            /* U multiple preci */
	$"7369 6F6E 2061 7269 7468 6D65 7469 6320"            /* sion arithmetic  */
	$"6C69 6272 6172 792C 2077 6173 2077 7269"            /* library, was wri */
	$"7474 656E 2062 7920 546F 7262 6A9A 726E"            /* tten by Torbj�rn */
	$"2047 7261 6E6C 756E 6420 616E 6420 6D61"            /*  Granlund and ma */
	$"6E79 2063 6F6E 7472 6962 7574 6F72 732E"            /* ny contributors. */
	$"2049 7420 6973 206E 6F74 2061 6374 7561"            /*  It is not actua */
	$"6C6C 7920 696E 636C 7564 6564 2069 6E20"            /* lly included in  */
	$"6C73 682C 2062 7574 2069 7420 6973 206E"            /* lsh, but it is n */
	$"6565 6465 6420 666F 7220 616C 6C20 7075"            /* eeded for all pu */
	$"626C 6963 206B 6579 2063 6F6D 7075 7461"            /* blic key computa */
	$"7469 6F6E 732E 2047 4D50 2069 7320 7265"            /* tions. GMP is re */
	$"6C65 6173 6564 2075 6E64 6572 2074 6865"            /* leased under the */
	$"204C 5047 4C2E 0D0D 4D44 3520 696D 706C"            /*  LPGL...MD5 impl */
	$"656D 656E 7461 7469 6F6E 2062 7920 436F"            /* ementation by Co */
	$"6C69 6E20 506C 756D 622C 2073 6F6D 6577"            /* lin Plumb, somew */
	$"6861 7420 6861 636B 6564 2062 7920 416E"            /* hat hacked by An */
	$"6472 6577 204B 7563 686C 696E 672E 2052"            /* drew Kuchling. R */
	$"656C 6561 7365 6420 696E 746F 2074 6865"            /* eleased into the */
	$"2070 7562 6C69 6320 646F 6D61 696E 2E0D"            /*  public domain.. */
	$"0D52 494A 4E44 4145 4C20 696D 706C 656D"            /* .RIJNDAEL implem */
	$"656E 7461 7469 6F6E 2062 7920 5261 6661"            /* entation by Rafa */
	$"656C 2053 6576 696C 6C61 2E20 2052 656C"            /* el Sevilla.  Rel */
	$"6561 7365 6420 756E 6465 7220 7468 6520"            /* eased under the  */
	$"4C47 504C 2E0D 0D53 4841 2069 6D70 6C65"            /* LGPL...SHA imple */
	$"6D65 6E74 6174 696F 6E20 6279 2050 6574"            /* mentation by Pet */
	$"6572 2047 7574 6D61 6E6E 2C20 736F 6D65"            /* er Gutmann, some */
	$"7768 6174 2068 6163 6B65 6420 6279 2041"            /* what hacked by A */
	$"6E64 7265 7720 4B75 6368 6C69 6E67 2E20"            /* ndrew Kuchling.  */
	$"5265 6C65 6173 6564 2069 6E74 6F20 7468"            /* Released into th */
	$"6520 7075 626C 6963 2064 6F6D 6169 6E2E"            /* e public domain. */
	$"0D0D 5345 5250 454E 5420 696D 706C 656D"            /* ..SERPENT implem */
	$"656E 7461 7469 6F6E 2062 7920 526F 7373"            /* entation by Ross */
	$"2041 6E64 6572 736F 6E2C 2045 6C69 2042"            /*  Anderson, Eli B */
	$"6968 616D 2C20 616E 6420 4C61 7273 204B"            /* iham, and Lars K */
	$"6E75 6473 656E 2C20 736F 6D65 7768 6174"            /* nudsen, somewhat */
	$"2068 6163 6B65 6420 6279 2052 6166 6165"            /*  hacked by Rafae */
	$"6C20 5365 7669 6C6C 612E 2020 5265 6C65"            /* l Sevilla.  Rele */
	$"6173 6564 2075 6E64 6572 2074 6865 2047"            /* ased under the G */
	$"504C 2E0D 0D54 4350 5554 494C 5320 6E65"            /* PL...TCPUTILS ne */
	$"7477 6F72 6B69 6E67 2063 6F64 6520 6279"            /* tworking code by */
	$"2054 686F 6D61 7320 4265 6C6C 6D61 6E2E"            /*  Thomas Bellman. */
	$"2052 656C 6561 7365 6420 696E 746F 2074"            /*  Released into t */
	$"6865 2070 7562 6C69 6320 646F 6D61 696E"            /* he public domain */
	$"2E0D 0D50 4F4C 4C20 656D 756C 6174 696F"            /* ...POLL emulatio */
	$"6E20 636F 6465 2028 666F 7220 7379 7374"            /* n code (for syst */
	$"656D 7320 7468 6174 2068 6176 6520 7365"            /* ems that have se */
	$"6C65 6374 2829 2C20 6275 7420 6E6F 7420"            /* lect(), but not  */
	$"706F 6C6C 2829 2920 7772 6974 7465 6E20"            /* poll()) written  */
	$"6279 2053 6561 6E20 5265 6966 7363 686E"            /* by Sean Reifschn */
	$"6569 6465 722C 2072 656C 6561 7365 6420"            /* eider, released  */
	$"666F 7220 756E 6C69 6D69 7465 6420 7573"            /* for unlimited us */
	$"652E 0D0D 5345 5850 2070 6172 7369 6E67"            /* e...SEXP parsing */
	$"2063 6F64 6520 6861 7320 736F 6D65 2070"            /*  code has some p */
	$"6172 7473 2069 6E20 636F 6D6D 6F6E 2077"            /* arts in common w */
	$"6974 6820 526F 6E20 5269 7665 7374 2773"            /* ith Ron Rivest's */
	$"2070 6172 7365 722E 2044 6973 7472 6962"            /*  parser. Distrib */
	$"7574 6564 2075 6E64 6572 2074 6865 2047"            /* uted under the G */
	$"504C 2077 6974 6820 7065 726D 6973 7369"            /* PL with permissi */
	$"6F6E 2E0D 0D61 7267 7020 616E 6420 6765"            /* on...argp and ge */
	$"746F 7074 206F 7074 696F 6E20 7061 7273"            /* topt option pars */
	$"696E 6720 6672 6F6D 2074 6865 2074 6865"            /* ing from the the */
	$"2047 4E55 2043 206C 6962 7261 7279 2C20"            /*  GNU C library,  */
	$"7265 6C65 6173 6564 2075 6E64 6572 2074"            /* released under t */
	$"6865 204C 4750 4C2E 0D0D 0D42 6574 7465"            /* he LGPL....Bette */
	$"7254 656C 6E65 7420 636F 6E74 7269 6275"            /* rTelnet contribu */
	$"746F 7273 3A0D 0D42 6574 7465 7254 656C"            /* tors:..BetterTel */
	$"6E65 7420 6973 2062 6173 6564 206F 6E20"            /* net is based on  */
	$"6261 7365 6420 6F6E 204E 4353 4120 5465"            /* based on NCSA Te */
	$"6C6E 6574 2032 2E37 6235 2C20 6465 7665"            /* lnet 2.7b5, deve */
	$"6C6F 7065 6420 6174 2074 6865 204E 6174"            /* loped at the Nat */
	$"696F 6E61 6C20 4365 6E74 6572 2066 6F72"            /* ional Center for */
	$"2053 7570 6572 636F 6D70 7574 696E 6720"            /*  Supercomputing  */
	$"4170 706C 6963 6174 696F 6E73 2028 4E43"            /* Applications (NC */
	$"5341 2920 6174 2074 6865 2055 6E69 7665"            /* SA) at the Unive */
	$"7273 6974 7920 6F66 2049 6C6C 696E 6F69"            /* rsity of Illinoi */
	$"7320 6174 2055 7262 616E 612D 4368 616D"            /* s at Urbana-Cham */
	$"7061 6967 6E20 2855 4955 4329 2E0D 0D4F"            /* paign (UIUC)...O */
	$"6C61 7620 4272 696E 6B6D 616E 6E2C 204A"            /* lav Brinkmann, J */
	$"6561 6E2D 5069 6572 7265 204B 7579 7065"            /* ean-Pierre Kuype */
	$"7273 2C20 4461 6E20 4879 6465 2C20 4272"            /* rs, Dan Hyde, Br */
	$"656E 7420 5363 6877 616E 2C20 4361 726C"            /* ent Schwan, Carl */
	$"2042 656C 6C2C 2045 7269 6320 502E 2053"            /*  Bell, Eric P. S */
	$"636F 7474 2C20 4A69 6D20 4D61 7474 6865"            /* cott, Jim Matthe */
	$"7773 2C20 416E 6472 6561 7320 5072 696C"            /* ws, Andreas Pril */
	$"6F70 2C20 416B 696F 2059 6F73 6869 6461"            /* op, Akio Yoshida */
	$"2C20 5269 636B 205A 6163 636F 6E65 2C20"            /* , Rick Zaccone,  */
	$"4368 6173 2057 696C 6C69 616D 732C 2044"            /* Chas Williams, D */
	$"616E 204A 6163 6F62 6F77 6974 7A2C 204B"            /* an Jacobowitz, K */
	$"656E 6E65 7468 2047 7565 7269 6E0D 0D0D"            /* enneth Guerin... */
	$"0D0D 0D"                                            /* ... */
};

resource 'WIND' (1000) {
	{50, 8, 300, 408},
	plainDBox,
	visible,
	noGoAway,
	0x0,
	"",
	alertPositionMainScreen
};

data 'styl' (1000, "Credits") {
	$"0013 0000 0000 000C 000A 0003 0000 0009"            /* .........�.....� */
	$"0000 0000 0000 0000 000A 000C 000A 0003"            /* .........�...�.. */
	$"0100 0009 0000 0000 0000 0000 003A 000C"            /* ...�.........:.. */
	$"000A 0003 0000 0009 0000 0000 0000 0000"            /* .�.....�........ */
	$"00A4 000C 000A 0003 0100 0009 0000 0000"            /* .�...�.....�.... */
	$"0000 0000 00B0 000C 000A 0003 0000 0009"            /* .....�...�.....� */
	$"0000 0000 0000 0000 00F0 000C 000A 0003"            /* .........�...�.. */
	$"0100 0009 0000 0000 0000 0000 00F3 000C"            /* ...�.........�.. */
	$"000A 0003 0000 0009 0000 0000 0000 0000"            /* .�.....�........ */
	$"0133 000C 000A 0003 0100 0009 0000 0000"            /* .3...�.....�.... */
	$"0000 0000 0137 000C 000A 0003 0000 0009"            /* .....7...�.....� */
	$"0000 0000 0000 0000 01D4 000C 000A 0003"            /* .........�...�.. */
	$"0100 0009 0000 0000 0000 0000 01DE 000C"            /* ...�.........�.. */
	$"000A 0003 0000 0009 0000 0000 0000 0000"            /* .�.....�........ */
	$"02AB 000C 000A 0003 0100 0009 0000 0000"            /* .�...�.....�.... */
	$"0000 0000 02B3 000C 000A 0003 0000 0009"            /* .....�...�.....� */
	$"0000 0000 0000 0000 0450 000C 000A 0003"            /* .........P...�.. */
	$"0400 0009 0000 0000 0000 0000 0469 000C"            /* ...�.........i.. */
	$"000A 0003 0000 0009 0000 0000 0000 0000"            /* .�.....�........ */
	$"0582 000C 000A 0003 0030 0009 0000 0000"            /* .�...�...0.�.... */
	$"0000 0000 0592 000C 000A 0003 0000 0009"            /* .....�...�.....� */
	$"0000 0000 0000 0000 0B7B 000C 000A 0003"            /* .........{...�.. */
	$"0400 0009 0000 0000 0000 0000 0B95 000C"            /* ...�.........�.. */
	$"000A 0003 0000 0009 0000 0000 0000"                 /* .�.....�...... */
};

