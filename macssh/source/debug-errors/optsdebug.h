char *telstates[15]={
    "SE",
    "NOP",
    "DM",
    "BREAK",
    "IP",
    "ABORT",
    "AYT",
    "ERASE CHAR",
    "ERASE LINE",
    "GO AHEAD",
    "SB",
	"WILL",
	"WONT",
	"DO",
	"DONT"
};

char *teloptions[256]={
	"Binary",				/* 0 */
	"Echo",
	"Reconnection",
	"Supress Go Ahead",
	"Message Size Negotiation",
	"Status",				/* 5 */
	"Timing Mark",
	"Remote Controlled Trans and Echo",
	"Output Line Width",
	"Output Page Size",
	"Output Carriage-Return Disposition",	/* 10 */
	"Output Horizontal Tab Stops",
	"Output Horizontal Tab Disposition",
	"Output Formfeed Disposition",
	"Output Vertical Tabstops",
	"Output Vertical Tab Disposition",		/* 15 */
	"Output Linefeed Disposition",
	"Extended ASCII",
	"Logout",
	"Byte Macro",
	"Data Entry Terminal",					/* 20 */
	"SUPDUP",
	"SUPDUP Output",
	"Send Location",
	"Terminal Type",
	"End of Record",						/* 25 */
	"TACACS User Identification",
	"Output Marking",
	"Terminal Location Number",
	"3270 Regime",
	"X.3 PAD",								/* 30 */
	"Negotiate About Window Size",
	"Terminal Speed",
	"Toggle Flow Control",
	"Linemode",
	"X Display Location",					/* 35 */
	"Environment",
	"Authentication",
	"Encrypt",
	"39",
	"40","41","42","43","44","45","46","47","48","49",
	"50","51","52","53","54","55","56","57","58","59",
	"60","61","62","63","64","65","66","67","68","69",
	"70","71","72","73","74","75","76","77","78","79",
	"80","81","82","83","84","85","86","87","88","89",
	"90","91","92","93","94","95","96","97","98","99",
	"100","101","102","103","104","105","106","107","108","109",
	"110","111","112","113","114","115","116","117","118","119",
	"120","121","122","123","124","125","126","127","128","129",
	"130","131","132","133","134","135","136","137","138","139",
	"140","141","142","143","144","145","146","147","148","149",
	"150","151","152","153","154","155","156","157","158","159",
	"160","161","162","163","164","165","166","167","168","169",
	"170","171","172","173","174","175","176","177","178","179",
	"180","181","182","183","184","185","186","187","188","189",
	"190","191","192","193","194","195","196","197","198","199",
	"200","201","202","203","204","205","206","207","208","209",
	"210","211","212","213","214","215","216","217","218","219",
	"220","221","222","223","224","225","226","227","228","229",
	"230","231","232","233","234","235","236","237","238","239",
	"240","241","242","243","244","245","246","247","248","249",
	"250","251","252","253","254",
	"Extended Options List"		/* 255 */
};

char *LMoptions[]={
	"None",
	"SYNCH",
	"BREAK",
	"IP",
	"ABORT OUTPUT",
	"AYT",
	"EOR",
	"ABORT",
	"EOF",
	"SUSP",
	"EC",
	"EL",
	"EW",
	"RP",
	"LNEXT",
	"XON",
	"XOFF",
	"FORW1",
	"FORW2",
	"MCL",
	"MCR",
	"MCWL",
	"MCWR",
	"MCBOL",
	"MCEOL",
	"INSRT",
	"OVER",
	"ECR",
	"EWR",
	"EBOL",
	"EEOL"
};

char *LMflags[]={
	"NOSUPPORT",
	"CANTCHANGE",
	"VALUE",
	"DEFAULT"
};

char *LMmodes[]={
	"EDIT ",
	"TRAPSIG ",
	"MODE_ACK ",
	"SOFT_TAB ",
	"LIT_ECHO "
};

char *LMmodeBits[] ={
	"E", "T", "A", "S", "L"
};