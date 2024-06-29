/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  		= 1;       	 /* border pixel of windows */
static unsigned int snap      		= 32;      	 /* snap pixel */
static const unsigned int gappih    = 20;     	 /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;      	 /* vert inner gap between windows */
static const unsigned int gappoh    = 10;        /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;        /* vert outer gap between windows and screen edge */
static int smartgaps	            = 0;         /* 1 means no outer gap when there is only one window */
static int showbar            		= 1;         /* 0 means no bar */
static int topbar             		= 1;         /* 0 means bottom bar */
static int user_bh            		= 0;         /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static char font[]            		= "monospace:size=10";
static char dmenufont[]       		= "monospace:size=10";
static const char *fonts[]          = { font };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char tagnormfg[]				= "#eeeeee";
static char tagnormbg[]				= "#222222";
static char tagselfg[]				= "#eeeeee";
static char tagselbg[]				= "#005577";
static char tagoccfg[]				= "#eeeeee";
static char tagoccbg[]				= "#222222";
static char tagpinfg[]				= "#eeeeee";
static char tagpinbg[]				= "#222222";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

static char *tagsel[][2] = {
   /*   fg         bg    */
  { tagnormfg, tagnormbg }, /* norm */
  { tagselfg, tagselbg  }, /* sel */
  { tagoccfg, tagoccbg }, /* occ but not sel */
  { tagpinfg, tagpinbg }, /* has pinned tag */
};

static const char *const autostart[] = {
        NULL /* terminate */
};


static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },

};

/* window swallowing */
static const int swaldecay = 3;
static const int swalretroactive = 1;
static const char swalsymbol[] = "…";

/* layout(s) */
static float mfact     			= 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     			= 1;    /* number of clients in master area */
static int resizehints 			= 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",	tile },    /* first entry is default */
	{ "",	monocle },
	{ "",	spiral },
	{ "",	dwindle },
	{ "",	deck },
	{ "",	bstack },
	{ "",	bstackhoriz },
	{ "",	grid },
	{ "",	nrowgrid },
	{ "",	horizgrid },
	{ "",	gaplessgrid },
	{ "",	centeredmaster },
	{ " ",	centeredfloatingmaster },
	{ "",	NULL },    /* no layout function means floating behavior */
	{ NULL,	NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* Default applications */
#define TERMINAL "st"

/* command spawn macros */
#define SHCMD(cmd) 			{ .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TERMCMD(...)        { .v = (const char*[]){ TERMINAL, "-e", __VA_ARGS__, NULL } }
#define REDSHIFT(arg)       { .v = (const char*[]){ "redshift", "-PO" arg, NULL } }
#define REDSHIFT_DEFAULT    { .v = (const char*[]){ "redshift", "-x", NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
		/* tag color scheme */
		{ "tagnormfg",			STRING,	 &tagnormfg },
		{ "tagnormbg",			STRING,	 &tagnormbg },
		{ "tagselfg",			STRING,  &tagselfg },
		{ "tagselbg",			STRING,  &tagselbg },
		{ "tagoccfg",			STRING,  &tagoccfg },
		{ "tagoccbg",			STRING,  &tagoccbg },
		{ "tagpinfg",			STRING,  &tagpinfg },
		{ "tagpinbg",			STRING,  &tagpinbg },

};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	/* Basic keybinds */
	{ MODKEY,                       XK_r,      		spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, 		spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      		togglebar,      {0} },
	{ MODKEY,                       XK_j,      		focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      		focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      		incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      		incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      		setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      		setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      		setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      		setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      		setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, 		zoom,           {0} },
	{ MODKEY,                       XK_Tab,    		view,           {0} },
	{ MODKEY,		                XK_q,      		killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      		quit,           {0} },
	{ MODKEY|ShiftMask,             XK_BackSpace,   quit,           {0} },
	{ MODKEY|ShiftMask,             XK_c,	   		cyclelayout,    {.i = -1 } },
	{ MODKEY,                       XK_c, 	   		cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_s,      		togglesticky,   {0} },
	{ MODKEY,             			XK_space,  		togglefloating, {0} },
	{ MODKEY,                       XK_0,      		view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      		tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  		focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 		focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  		tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 		tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_u,      		swalstopsel,    {0} },

	/* Vanity gaps */
	{ MODKEY|ALT,              		XK_u,      		incrgaps,       {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,    		XK_u,      		incrgaps,       {.i = -1 } },
	{ MODKEY|ALT,             		XK_i,      		incrigaps,      {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,    		XK_i,      		incrigaps,      {.i = -1 } },
	{ MODKEY|ALT,              		XK_o,      		incrogaps,      {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,    		XK_o,      		incrogaps,      {.i = -1 } },
	{ MODKEY|ALT,              		XK_6,      		incrihgaps,     {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,    		XK_6,      		incrihgaps,     {.i = -1 } },
	{ MODKEY|ALT,              		XK_7,      		incrivgaps,     {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,    		XK_7,      		incrivgaps,     {.i = -1 } },
	{ MODKEY|ALT,             		XK_8,      		incrohgaps,     {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,    		XK_8,      		incrohgaps,     {.i = -1 } },
	{ MODKEY|ALT,              		XK_9,      		incrovgaps,     {.i = +1 } },
	{ MODKEY|ALT|ShiftMask,    		XK_9,      		incrovgaps,     {.i = -1 } },
	{ MODKEY|ALT,              		XK_0,      		togglegaps,     {0} },
	{ MODKEY|ALT|ShiftMask,    		XK_0,     		defaultgaps,    {0} },

	/* Redshift control */
	{ ShiftMask,          			XK_Escape, 		spawn,          REDSHIFT_DEFAULT },
	{ ShiftMask,           			XK_F1,     		spawn,          REDSHIFT("2000") },
	{ ShiftMask,           			XK_F2,     		spawn,          REDSHIFT("2200") },
	{ ShiftMask,           			XK_F3,     		spawn,          REDSHIFT("2400") },
	{ ShiftMask,           			XK_F4,     		spawn,          REDSHIFT("2600") },
	{ ShiftMask,           			XK_F5,     		spawn,          REDSHIFT("2900") },
	{ ShiftMask,           			XK_F6,     		spawn,          REDSHIFT("3200") },
	{ ShiftMask,         			XK_F7,     		spawn,          REDSHIFT("3500") },
	{ ShiftMask,          			XK_F8,     		spawn,          REDSHIFT("3800") },
	{ ShiftMask,         			XK_F9,     		spawn,          REDSHIFT("4100") },
	{ ShiftMask,          			XK_F10,    		spawn,          REDSHIFT("4500") },
	{ ShiftMask,         			XK_F11,    		spawn,          REDSHIFT("5000") },
	{ ShiftMask,          			XK_F12,    		spawn,          REDSHIFT("5500") },

	/* Tagkeys */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      	button          function        argument */
	{ ClkLtSymbol,          0,              	Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              	Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              	Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              	Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         	Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         	Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         	Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, 	Button1,      	swalmouse,      {0} },
	{ ClkTagBar,            0,              	Button1,        view,           {0} },
	{ ClkTagBar,            0,              	Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         	Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         	Button3,        toggletag,      {0} },
};
