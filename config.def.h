/* See LICENSE file for copyright and license details. */

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
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan },
};

/* preferred applications */
#define MY_CALCULATOR ""
#define MY_TERMINAL "st"
#define MY_BROWSER "firefox"
#define MY_FILEBROWSER_GUI ""
#define MY_FILEBROWSER_CLI "ranger"
#define MY_LOCKSCREEN "slock"
#define MY_POLICYKIT ""
#define MY_STATUSBAR_MANAGER "slstatus"
#define MY_COMPOSITOR "picom"
#define MY_NOTIFICATION_DAEMON "dunst"

static const char *const autostart[] = {
	/* System Applications */
	MY_NOTIFICATION_DAEMON, NULL,
	MY_STATUSBAR_MANAGER, NULL,
	MY_COMPOSITOR, NULL,
	MY_POLICYKIT, NULL,
	"xsetroot", "-cursor_name", "left_ptr", NULL,
	"nitrogen", "--restore", NULL,
	
	/* System Tray Applications */
	"nm-applet", NULL,
	"blueman-applet", NULL,
	"pasystray", NULL,
	"birdtray", NULL,

	/* Applications */
	MY_TERMINAL, NULL,

	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance    title          tags mask     iscentered   isfloating    monitor */
	{ "Gimp",             NULL,       NULL,          0,            0,           1,            -1 },
	{ "Firefox",          NULL,       NULL,          1 << 8,       0,           0,            -1 },
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
#define AltMask Mod1Mask
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
static const char *launcher[] = { "rofi", "-modi", "drun,run", "-show", "drun", NULL };
static const char *emojicmd[] = { "rofi", "-modi", "emoji", "-show", "emoji", NULL };
static const char *prntscrncmd[] = { "/bin/sh", "-c", "screenshot.sh", NULL };
static const char *termcmd[]  = { MY_TERMINAL, NULL };
static const char *browsercmd[] = { MY_BROWSER, NULL };
static const char *calculatorcmd[] = { MY_CALCULATOR, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { MY_TERMINAL, "-t", scratchpadname, NULL };
static const char *lockscreencmd[] = { MY_LOCKSCREEN, NULL };
static const char *filebrowserclicmd[] = { MY_TERMINAL, "-e", MY_FILEBROWSER_CLI };
static const char *filebrowserguicmd[] = { MY_FILEBROWSER_GUI, NULL };

/* media key commands */
/* TODO: Move shell scripts for volume, media control, screenshot, and compositor toggling into C functions */
static const char *mediavoldn[] = { "/bin/sh", "-c", "volume.sh --decrease", NULL };
static const char *mediavolmt[] = { "/bin/sh", "-c", "volume.sh --mute", NULL };
static const char *mediavolup[] = { "/bin/sh", "-c", "volume.sh --increase", NULL };
static const char *mediactlplay[] = { "playerctl", "play-pause", NULL };
static const char *mediactlprev[] = { "playerctl", "previous", NULL };
static const char *mediactlnext[] = { "playerctl", "next", NULL };
static const char *togglecompositor[] = { "/bin/sh", "-c", "toggle-compositor.sh" };

static Key keys[] = {
	/* modifier            key           function        argument */
	{ MODKEY,              XK_p,         spawn,          {.v = launcher } },
	{ MODKEY,              XK_period,    spawn,          {.v = emojicmd } },
	{ MODKEY,              XK_e,         spawn,          {.v = filebrowserclicmd } },
	{ MODKEY|ShiftMask,    XK_e,         spawn,          {.v = filebrowserguicmd } },
	{ MODKEY,              XK_o,         spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,    XK_Return,    spawn,          {.v = termcmd } },
	{ MODKEY,              XK_Escape,    togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,              XK_l,         spawn,          {.v = lockscreencmd } },
	{ 0,                   KB_PRNTSCRN,  spawn,          {.v = prntscrncmd } },
	
	/* window management */
	{ MODKEY,              XK_b,         togglebar,      {0} },
	{ AltMask,             XK_Tab,       focusstack,     {.i = +1 } },
	{ Mod1Mask|ShiftMask,  XK_Tab,       focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_comma,     incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_period,    incnmaster,     {.i = -1 } },
	{ MODKEY,              XK_j,         setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_k,         setmfact,       {.f = +0.05} },
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
	{ MODKEY,              XK_z,         spawn,          {.v = togglecompositor } },

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
	{ 0,     XF86XK_Calculator,          spawn,          {.v = calculatorcmd} },
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
