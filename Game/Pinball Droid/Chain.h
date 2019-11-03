#ifndef __CHAIN_H__
#define __CHAIN_H__

int Background[180] = {
	309, 797,
	475, 666,
	475, 518,
	428, 480,
	429, 444,
	400, 393,
	400, 304,
	427, 273,
	425, 270,
	428, 262,
	426, 255,
	424, 249,
	416, 245,
	403, 248,
	398, 245,
	429, 203,
	429, 66,
	417, 54,
	406, 48,
	392, 41,
	373, 60,
	372, 81,
	367, 83,
	366, 73,
	365, 58,
	390, 36,
	403, 42,
	414, 47,
	428, 57,
	434, 65,
	430, 436,
	476, 436,
	476, 62,
	462, 36,
	447, 21,
	428, 10,
	414, 4,
	403, 2,
	312, 2,
	296, 7,
	279, 15,
	267, 23,
	257, 32,
	254, 42,
	253, 78,
	248, 80,
	222, 55,
	222, 33,
	221, 33,
	219, 24,
	212, 16,
	205, 14,
	198, 16,
	191, 21,
	189, 31,
	187, 31,
	186, 58,
	165, 58,
	158, 43,
	149, 30,
	135, 19,
	119, 9,
	100, 2,
	 86, 2,
	 71, 2,
	 48, 10,
	 29, 22,
	 17, 32,
	 11, 40,
	  4, 51,
	  0, 63,
	  0, 320,
	  1, 357,
	 11, 386,
	 14, 397,
	 34, 435,
	 59, 462,
	 59, 476,
	  1, 519,
	  0, 673,
	  0, 737,
	 18, 737,
	 25, 736,
	 30, 731,
	 34, 727,
	 36, 722,
	 36, 717,
	 39, 717,
	 39, 695,
	170, 797
};

int BackgroundL[14] = {
	 90, 517,
	 94, 523,
	154, 651,
	153, 657,
	 91, 612,
	 85, 603,
	 85, 518
};

int BackgroundR[18] = {
	318, 655,
	382, 518,
	386, 516,
	390, 519,
	389, 596,
	386, 607,
	379, 615,
	323, 657,
	320, 657
};

int ChainL[16] = {
	48, 538,
	48, 597,
	60, 632,
	154, 703,
	151, 708,
	55, 634,
	44, 601,
	44, 538
};

int ChainR[22] = {
	430, 538,
	435, 538,
	435, 601,
	424, 632,
	419, 638,
	328, 708,
	326, 707,
	326, 703,
	419, 631,
	430, 598,
	430, 543
};

int BackgroundU[20] = {
	 38, 348,
	 59, 360,
	100, 285,
	 42, 263,
	 38, 255,
	 36, 244,
	 40, 234,
	 48, 225,
	 44, 221,
	 32, 234
};

int BackgroundLine[8] = {
	272, 220,
	315, 263,
	317, 260,
	275, 218
};

int PivoteUR[42] = {
	287, 170,
	292, 170,
	299, 166,
	304, 162,
	306, 157,
	303, 155,
	302, 150,
	309, 144,
	311, 135,
	304, 125,
	294, 120,
	284, 119,
	273, 121,
	267, 126,
	262, 134,
	264, 144,
	270, 149,
	267, 156,
	270, 162,
	276, 168,
	282, 169
};

int PivoteUL[40] = {
	363, 169,
	369, 169,
	380, 161,
	380, 155,
	378, 155,
	377, 152,
	387, 144,
	387, 132,
	377, 122,
	368, 119,
	357, 119,
	346, 123,
	339, 131,
	337, 138,
	338, 143,
	347, 151,
	344, 155,
	346, 164,
	350, 167,
	356, 169
};

int PivoteU[46] = {
	 88, 100,
	 92, 100,
	 97, 99,
	103, 91,
	103, 85,
	100, 84,
	100, 82,
	108, 76,
	110, 70,
	109, 63,
	103, 55,
	 93, 50,
	 82, 48,
	 68, 53,
	 61, 60,
	 60, 66,
	 60, 73,
	 69, 81,
	 69, 83,
	 67, 85,
	 67, 92,
	 72, 98,
	 79, 100
};

int PivoteL[54] = {
	316, 189,
	310, 189,
	305, 191,
	300, 193,
	295, 196,
	291, 200,
	289, 206,
	291, 212,
	294, 216,
	300, 220,
	297, 224,
	297, 229,
	300, 233,
	307, 238,
	312, 239,
	317, 239,
	323, 238,
	328, 235,
	331, 230,
	331, 225,
	328, 219,
	333, 217,
	337, 212,
	338, 205,
	335, 199,
	330, 194,
	321, 190
};

int BouncyL[8] = {
	93, 522,
	96, 523,
	154, 646,
	152, 648
};

int BouncyR[8] = {
	380, 522,
	377, 522,
	319, 646,
	320, 649
};

int BarraL[12] = {
	288, 56,
	286, 61,
	286, 81,
	289, 84,
	293, 81,
	293, 60
};

int BarraR[12] = {
	329, 57,
	326, 58,
	326, 82,
	329, 84,
	332, 81,
	332, 58
};

int Hole1[12] = {
	195, 17,
	202, 14,
	211, 15,
	217, 21,
	189, 29,
	191, 22
};

int Hole2[10] = {
	400, 55,
	388, 60,
	414, 67,
	412, 62,
	406, 57
};

int Hole3[14] = {
	406, 247,
	427, 266,
	428, 260,
	427, 253,
	424, 249,
	417, 245,
	411, 245
};

int Hole4[12] = {
	36, 719,
	10, 726,
	14, 730,
	21, 733,
	28, 731,
	34, 727
};

int Light1[12] = {
	168, 62,
	185, 62,
	185, 67,
	182, 71,
	172, 71,
	168, 67
};

int Light2[10] = {
	 68, 355,
	 76, 340,
	 81, 344,
	 81, 353,
	 73, 357
};

int Light3[10] = {
	 78, 337,
	 87, 322,
	 92, 328,
	 91, 335,
	 86, 339
};

int Light4[8] = {
	 88, 319,
	 97, 304,
	103, 313,
	 98, 322
};

int Light5[12] = {
	270, 223,
	282, 236,
	278, 239,
	272, 239,
	266, 233,
	266, 226
};

int Light6[12] = {
	285, 238,
	297, 251,
	293, 254,
	287, 254,
	281, 247,
	281, 241
};

int Light7[12] = {
	300, 253,
	313, 266,
	306, 270,
	302, 269,
	296, 263,
	296, 256
};

int Light8[10] = {
	400, 394,
	407, 410,
	399, 411,
	393, 403,
	394, 396
};

int Light9[12] = {
	409, 412,
	418, 428,
	412, 429,
	406, 428,
	403, 420,
	405, 415
};

int Light10[12] = {
	419, 430,
	427, 445,
	422, 448,
	416, 446,
	413, 439,
	414, 432
};
#endif // !__CHAIN_H__