const  char	*Samples[] =
    {
    "x",
    "x*x",
    "x*x*x",
    "x*x*x*x",
    "x*x*x*x*x",
    "x*x*x*x*x*x",
    "1/x",
    "1/(x*x)",
    "sin(x)",
    "sin(x*pi)",
    "cos(x)",
    "tan(x)",
    "asin(x)",
    "acos(x)",
    "atan(x)",
    "sinh(x)",
    "cosh(x)",
    "tanh(x)",
    "asinh(x)",
    "acosh(x)",
    "atanh(x)",
    "abs(x)",
    "exp(x)",
    "sqr(x)",
    "sqrt(x)",
    "log(x)",
    "abs(sin(x))",
    "e^x",
    "e^(-x)",
    "x^x",
    "x^1.5",
    "round(x)",
    "sqrt(4-x*x)",
    "sin(x)+sin(3*x)/3+sin(5*x)/5+sin(7*x)/7+sin(9*x)/9+sin(11*x)/11+sin(13*x)/13+sin(15*x)/15+sin(17*x)/17+sin(19*x)/19\
+sin(21*x)/21+sin(23*x)/23+sin(25*x)/25+sin(27*x)/27+sin(29*x)/29+sin(31*x)/31+sin(33*x)/33+sin(35*x)/35+sin(37*x)/37+sin(39*x)/39\
+sin(41*x)/41+sin(43*x)/43+sin(45*x)/45+sin(47*x)/47+sin(49*x)/49+sin(51*x)/51+sin(53*x)/53+sin(55*x)/55+sin(57*x)/57+sin(59*x)/59\
+sin(61*x)/61+sin(63*x)/63+sin(65*x)/65+sin(67*x)/67+sin(69*x)/69+sin(71*x)/71+sin(73*x)/73+sin(75*x)/75+sin(77*x)/77+sin(79*x)/79\
+sin(81*x)/81+sin(83*x)/83+sin(85*x)/85+sin(87*x)/87+sin(89*x)/89+sin(91*x)/91+sin(93*x)/93+sin(95*x)/95+sin(97*x)/97+sin(99*x)/99",	// square wave 1
    "sin(x)+sin(3*x)/3+sin(5*x)/5+sin(7*x)/7+sin(9*x)/9+sin(11*x)/11+sin(13*x)/13+sin(15*x)/15+sin(17*x)/17+sin(19*x)/19\
+sin(21*x)/21+sin(23*x)/23+sin(25*x)/25+sin(27*x)/27+sin(29*x)/29+sin(31*x)/31+sin(33*x)/33+sin(35*x)/35+sin(37*x)/37+sin(39*x)/39\
+sin(41*x)/41+sin(43*x)/43+sin(45*x)/45+sin(47*x)/47+sin(49*x)/49+sin(51*x)/51+sin(53*x)/53+sin(55*x)/55+sin(57*x)/57+sin(59*x)/59\
+sin(61*x)/61+sin(63*x)/63+sin(65*x)/65+sin(67*x)/67+sin(69*x)/69+sin(71*x)/71+sin(73*x)/73+sin(75*x)/75+sin(77*x)/77+sin(79*x)/79\
+sin(81*x)/81+sin(83*x)/83+sin(85*x)/85+sin(87*x)/87+sin(89*x)/89+sin(91*x)/91+sin(93*x)/93+sin(95*x)/95+sin(97*x)/97+sin(99*x)/99\
+sin(101*x)/101+sin(103*x)/103+sin(105*x)/105+sin(107*x)/107+sin(109*x)/109+sin(111*x)/111+sin(113*x)/113+sin(115*x)/115+sin(117*x)/117+sin(119*x)/119\
+sin(121*x)/121+sin(123*x)/123+sin(125*x)/125+sin(127*x)/127+sin(129*x)/129+sin(131*x)/131+sin(133*x)/133+sin(135*x)/135+sin(137*x)/137+sin(139*x)/139\
+sin(141*x)/141+sin(143*x)/143+sin(145*x)/145+sin(147*x)/147+sin(149*x)/149+sin(151*x)/151+sin(153*x)/153+sin(155*x)/155+sin(157*x)/157+sin(159*x)/159\
+sin(161*x)/161+sin(163*x)/163+sin(165*x)/165+sin(167*x)/167+sin(169*x)/169+sin(171*x)/171+sin(173*x)/173+sin(175*x)/175+sin(177*x)/177+sin(179*x)/179\
+sin(181*x)/181+sin(183*x)/183+sin(185*x)/185+sin(187*x)/187+sin(189*x)/189+sin(191*x)/191+sin(193*x)/193+sin(195*x)/195+sin(197*x)/197+sin(199*x)/199\
+sin(201*x)/201+sin(203*x)/203+sin(205*x)/205+sin(207*x)/207+sin(209*x)/209+sin(211*x)/211+sin(213*x)/213+sin(215*x)/215+sin(217*x)/217+sin(219*x)/219\
+sin(221*x)/221+sin(223*x)/223+sin(225*x)/225+sin(227*x)/227+sin(229*x)/229+sin(231*x)/231+sin(233*x)/233+sin(235*x)/235+sin(237*x)/237+sin(239*x)/239\
+sin(241*x)/241+sin(243*x)/243+sin(245*x)/245+sin(247*x)/247+sin(249*x)/249+sin(251*x)/251+sin(253*x)/253+sin(255*x)/255+sin(257*x)/257+sin(259*x)/259\
+sin(261*x)/261+sin(263*x)/263+sin(265*x)/265+sin(267*x)/267+sin(269*x)/269+sin(271*x)/271+sin(273*x)/273+sin(275*x)/275+sin(277*x)/277+sin(279*x)/279\
+sin(281*x)/281+sin(283*x)/283+sin(285*x)/285+sin(287*x)/287+sin(289*x)/289+sin(291*x)/291+sin(293*x)/293+sin(295*x)/295+sin(297*x)/297+sin(299*x)/299",	// square wave 2
    "sin(x)-sin(2*x)/2+sin(3*x)/3-sin(4*x)/4+sin(5*x)/5-sin(6*x)/6+sin(7*x)/7-sin(8*x)/8+sin(9*x)/9-sin(10*x)/10\
+sin(11*x)/11-sin(12*x)/12+sin(13*x)/13-sin(14*x)/14+sin(15*x)/15-sin(16*x)/16+sin(17*x)/17-sin(18*x)/18+sin(19*x)/19-sin(20*x)/20\
+sin(21*x)/21-sin(22*x)/22+sin(23*x)/23-sin(24*x)/24+sin(25*x)/25-sin(26*x)/26+sin(27*x)/27-sin(28*x)/28+sin(29*x)/29-sin(30*x)/30\
+sin(31*x)/31-sin(32*x)/32+sin(33*x)/33-sin(34*x)/34+sin(35*x)/35-sin(36*x)/36+sin(37*x)/37-sin(38*x)/38+sin(39*x)/39-sin(40*x)/40\
+sin(41*x)/41-sin(42*x)/42+sin(43*x)/43-sin(44*x)/44+sin(45*x)/45-sin(46*x)/46+sin(47*x)/47-sin(48*x)/48+sin(49*x)/49-sin(50*x)/50",	// sawtooth wave
    "cos(x)+cos(3*x)/9+cos(5*x)/25+cos(7*x)/49+cos(9*x)/81+cos(11*x)/121+cos(13*x)/169+cos(15*x)/225+cos(17*x)/289+cos(19*x)/361\
+cos(21*x)/441+cos(23*x)/529+cos(25*x)/625+cos(27*x)/729+cos(29*x)/841+cos(31*x)/961+cos(33*x)/1089+cos(35*x)/1225+cos(37*x)/1369+cos(39*x)/1521\
+cos(41*x)/1681+cos(43*x)/1849+cos(45*x)/2025+cos(47*x)/2209+cos(49*x)/2401+cos(51*x)/2601+cos(53*x)/2809+cos(55*x)/3025+cos(57*x)/3249+cos(59*x)/3481",	// triangular wave
    "sin(x)-sin(3*x)+sin(5*x)-sin(7*x)+sin(9*x)-sin(11*x)+sin(13*x)-sin(15*x)+sin(17*x)-sin(19*x)\
+sin(21*x)-sin(23*x)+sin(25*x)-sin(27*x)+sin(29*x)-sin(31*x)+sin(33*x)-sin(35*x)+sin(37*x)-sin(39*x)\
+sin(41*x)-sin(43*x)+sin(45*x)-sin(47*x)+sin(49*x)-sin(51*x)+sin(53*x)-sin(55*x)+sin(57*x)-sin(59*x)\
+sin(61*x)-sin(63*x)+sin(65*x)-sin(67*x)+sin(69*x)-sin(71*x)+sin(73*x)-sin(75*x)+sin(77*x)-sin(79*x)\
+sin(81*x)-sin(83*x)+sin(85*x)-sin(87*x)+sin(89*x)-sin(91*x)+sin(93*x)-sin(95*x)+sin(97*x)-sin(99*x)",	// impulse train
    nullptr
    };

