/* See LICENSE file for copyright and license details. */

#include <X11/X.h>

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 6, .gappx = 10};
static const unsigned int snap      = 32;       /* snap pixel */
static const int mainmon            = 0;        /* number of the main monitor */
static const unsigned int systraypinning = mainmon + 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=12" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#ff0000";
static const char col_lime[]        = "#007700";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan },
};

/* terminal to use */
static const char myTerminal[] = "alacritty";

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance    title          tags mask     iscentered   isfloating    monitor */
	{ "Nvidia-settings",  NULL,       NULL,          0 ,           1,            1,           -1 },
	{ "Thunderbird",      NULL,       NULL,          1 << 2,       0,            0,            0 },
	{ "discord",          NULL,       "Discord",     0,            0,            0,            3 },
	{ "Google-chrome",    NULL,       "Pandora",     0,            0,            0,            1 },
	{ "Alacritty",        NULL,       "pianobar",    1 << 1,       1,            1,            1 },
	{ "gnome-calculator", NULL,       "Calculator",  0,            1,            1,           -1 },
	{ "Steam",            NULL,       "Steam",       0,            0,            0,            2 },
	{ "Xephyr",           NULL,       "Xephyr",      0,            1,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define KB_PRNTSCRN 0x0000ff61
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define MONKEYS(KEY,MON) \
	{ MODKEY,                       KEY,      focusnthmon,    {.i = MON} }, \
	{ MODKEY|ShiftMask,             KEY,      tagnthmon,      {.i = MON} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { myTerminal, NULL };
static const char rofimodi[] = "drun,run";
static const char rofishow[] = "drun";
static const char *roficmd[] = { "rofi", "-modi", rofimodi, "-show", rofishow, NULL };
static const char *emojicmd[] = { "rofi", "-modi", "emoji", "-show", "emoji", NULL };
static const char *rangercmd[] = { myTerminal, "-e", "ranger" };
static const char *thunarcmd[] = { "thunar", NULL };
static const char *browsercmd[] = { "google-chrome-stable", NULL };
static const char *prntscrncmd[] = { "/bin/sh", "-c", "screenshot.sh", NULL };
static const char *calccmd[] = { "gnome-calculator", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { myTerminal, "-t", scratchpadname, NULL };

/* media key commands */
static const char *mediavoldn[] = { "/bin/sh", "-c", "volume.sh --decrease", NULL };
static const char *mediavolmt[] = { "/bin/sh", "-c", "volume.sh --mute", NULL };
static const char *mediavolup[] = { "/bin/sh", "-c", "volume.sh --increase", NULL };
static const char *mediactlplay[] = { "playerctl", "play-pause", NULL };
static const char *mediactlprev[] = { "playerctl", "previous", NULL };
static const char *mediactlnext[] = { "playerctl", "next", NULL };

static Key keys[] = {
	/* modifier            key           function        argument */
	{ MODKEY,              XK_p,         spawn,          {.v = roficmd } },
	{ MODKEY,              XK_period,    spawn,          {.v = emojicmd } },
	{ MODKEY,              XK_e,         spawn,          {.v = rangercmd } },
	{ MODKEY|ShiftMask,    XK_e,         spawn,          {.v = thunarcmd } },
	{ MODKEY,              XK_o,         spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,    XK_Return,    spawn,          {.v = termcmd } },
	{ MODKEY,              XK_Escape,    togglescratch,  {.v = scratchpadcmd } },
	{ 0,                   KB_PRNTSCRN,  spawn,          {.v = prntscrncmd } },
	
	/* window management */
	{ MODKEY,              XK_b,         togglebar,      {0} },
	{ Mod1Mask,            XK_Tab,       focusstack,     {.i = +1 } },
	{ Mod1Mask|ShiftMask,  XK_Tab,       focusstack,     {.i = -1 } },
	{ MODKEY,              XK_j,         focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,         focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_comma,     incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_period,    incnmaster,     {.i = -1 } },
	{ MODKEY,              XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_l,         setmfact,       {.f = +0.05} },
	{ MODKEY,              XK_Return,    zoom,           {0} },
	{ MODKEY,              XK_Tab,       viewmainmon,    {0} },
	{ MODKEY|ControlMask,  XK_c,         killclient,     {0} },
	{ MODKEY,              XK_t,         setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_m,         setlayout,      {.v = &layouts[1]} },
	{ MODKEY,              XK_f,         setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_space,     setlayout,      {0} },
	{ MODKEY|ShiftMask,    XK_space,     togglefloating, {0} },
	{ MODKEY,              XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,         tag,            {.ui = ~0 } },
	/* Window Gap Control */
	{ MODKEY,              XK_minus,     setgaps,        {.i = -5 } },
	{ MODKEY,              XK_equal,     setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,    XK_minus,     setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,    XK_equal,     setgaps,        {.i = GAP_TOGGLE} },

	/* Tag Key */
	TAGKEYS(               XK_1,                         0)
	TAGKEYS(               XK_2,                         1)
	TAGKEYS(               XK_3,                         2)
	TAGKEYS(               XK_4,                         3)
	TAGKEYS(               XK_5,                         4)
	TAGKEYS(               XK_6,                         5)
	TAGKEYS(               XK_7,                         6)
	TAGKEYS(               XK_8,                         7)
	TAGKEYS(               XK_9,                         8)

	/* Monitor Keys */
	MONKEYS(               XK_w,                         0)
	MONKEYS(               XK_a,                         3)
	MONKEYS(               XK_s,                         2)
	MONKEYS(               XK_d,                         1)

	/* Kill DWM */
	{ MODKEY|ShiftMask,    XK_q,         quit,           {0} },
	
	/** Function Keys **/
	/* Media Keys */
	{ 0,     XF86XK_AudioLowerVolume,    spawn,          {.v = mediavoldn} },
	{ 0,     XF86XK_AudioMute,           spawn,          {.v = mediavolmt} },
	{ 0,     XF86XK_AudioRaiseVolume,    spawn,          {.v = mediavolup} },
	{ 0,     XF86XK_AudioPlay,           spawn,          {.v = mediactlplay} },
	{ 0,     XF86XK_AudioPrev,           spawn,          {.v = mediactlprev} },
	{ 0,     XF86XK_AudioNext,           spawn,          {.v = mediactlnext} },
	
	/* Calculator */
	{ 0,     XF86XK_Calculator,          spawn,          {.v = calccmd} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
