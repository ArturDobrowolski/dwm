/* See LICENSE file for copyright and license details. */ /* appearance */ static const unsigned int borderpx  = 2;        /* border pixel of windows */ static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 25;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 0;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "Fira Code Nerd Font:size=12", "JoyPixels:pixelsize=14:antialias=true:autohint=true"  };
static const char dmenufont[]       = "Fira Code Nerd Font:size=12";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

static const char *const autostart[] = {
        "dwmblocks", NULL,
        "dunst", NULL,
        "xcompmgr", NULL,
        /* Run shell scripts with a special handler script */ 
        "sh", "-c", "startup-sequence", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "FS-UAE",       0,         0,          0,           1,        -1 }, /* fs-uae */
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* default applications */
#define TERMINAL "st"
#define NEWS "news"
#define FILE_BROWSER "lfrun"
#define PROCMON "htop"

/* command spawn macros */
#define SHCMD(cmd) 			{ .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TERMCMD(...)        { .v = (const char*[]){ TERMINAL, "-e", __VA_ARGS__, NULL } }
#define REDSHIFT(arg)       { .v = (const char*[]){ "redshift", "-PO" arg, NULL } }
#define REDSHIFTDEFAULT     { .v = (const char*[]){ "redshift", "-x", NULL } }
#define SIGHANDLER(arg)		{ .v = (const char*[]){ "sighandler", "signal", arg, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_BackSpace,    quit,           {0} },
	{ MODKEY|ShiftMask,             XK_c,	   cyclelayout,    {.i = -1 } },
	{ MODKEY,                       XK_c, 	   cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F4,     xrdb,           {.v = NULL } },

	/* dwmblocks sighandler */
    { MODKEY,                       XK_F1,     spawn,          SIGHANDLER("vol_mute") },
    { MODKEY,                       XK_F2,     spawn,          SIGHANDLER("vol_dec") },
    { MODKEY,                       XK_F3,     spawn,          SIGHANDLER("vol_inc") },
    { MODKEY,                       XK_F5,     spawn,          SIGHANDLER("bri_dec") },
    { MODKEY,                       XK_F6,     spawn,          SIGHANDLER("bri_inc") },
    { MODKEY,                       XK_F7,     spawn,          SHCMD("exit 0") },
    { MODKEY,                       XK_F8,     spawn,          SHCMD("exit 0") },
    { MODKEY,                       XK_F9,     spawn,          SHCMD("signal-desktop") },
    { MODKEY,                       XK_F10,    spawn,          SHCMD("rocketchat-desktop") },
    { MODKEY,                       XK_F11,    spawn,          SHCMD("exit 0") },
    { MODKEY,                       XK_F12,    spawn,          SHCMD("exit 0") },

	/* Applications */
    { MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
    { MODKEY,                       XK_a,      spawn,          SHCMD("$BROWSER_ALT") },
    { MODKEY,                       XK_e,      spawn,          TERMCMD(FILE_BROWSER) },
    { MODKEY,                       XK_n,      spawn,          TERMCMD(NEWS) },
    { MODKEY,                       XK_t,      spawn,          TERMCMD(PROCMON) },
    { MODKEY,                       XK_F4,     spawn,          TERMCMD(PROCMON) },

	/* Utilities */
    { MODKEY,                       XK_b,      spawn,          SHCMD("bluetooth-connect") },
    { MODKEY,                       XK_p,      spawn,          SHCMD("getpass") },
    { MODKEY|ShiftMask,             XK_v,      spawn,          SHCMD("display-select") },
    { MODKEY|ShiftMask,             XK_l,      spawn,          SHCMD("lock-screen") },
    { MODKEY|ShiftMask,             XK_r,      spawn,          SHCMD("restart-prompt") },
    { MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("poweroff-prompt") },
    { 0,                            XK_Print,  spawn,          SHCMD("screenshot selection") }, 
    { MODKEY,                       XK_Print,  spawn,          SHCMD("screenshot window") }, 
    { MODKEY|ShiftMask,             XK_Print,  spawn,          SHCMD("screenshot selection_disk") }, 
    { MODKEY|ControlMask,           XK_Print,  spawn,          SHCMD("screenshot window_disk") }, 
	
	/* Redshift control */
	{ ShiftMask,          			XK_Escape, spawn,          REDSHIFTDEFAULT },
	{ ShiftMask,           			XK_F1,     spawn,          REDSHIFT("2000") },
	{ ShiftMask,           			XK_F2,     spawn,          REDSHIFT("2200") },
	{ ShiftMask,           			XK_F3,     spawn,          REDSHIFT("2400") },
	{ ShiftMask,           			XK_F4,     spawn,          REDSHIFT("2600") },
	{ ShiftMask,           			XK_F5,     spawn,          REDSHIFT("2900") },
	{ ShiftMask,           			XK_F6,     spawn,          REDSHIFT("3200") },
	{ ShiftMask,         			XK_F7,     spawn,          REDSHIFT("3500") },
	{ ShiftMask,          			XK_F8,     spawn,          REDSHIFT("3800") },
	{ ShiftMask,         			XK_F9,     spawn,          REDSHIFT("4100") },
	{ ShiftMask,          			XK_F10,    spawn,          REDSHIFT("4500") },
	{ ShiftMask,         			XK_F11,    spawn,          REDSHIFT("5000") },
	{ ShiftMask,          			XK_F12,    spawn,          REDSHIFT("5500") },

	/* Gap settings */
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|ControlMask,           XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|ControlMask,           XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	
	/*
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	*/

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
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

