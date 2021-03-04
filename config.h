#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 4;        /* border pixel of windows */
static const unsigned int gappx    = 4;        /* gaps between windows */
static const unsigned int snap     = 16;       /* snap pixel */
static const int showbar           = 1;        /* 0 means no bar */
static const int topbar            = 1;        /* 0 means bottom bar */
static const int vertpad           = 4;        /* vertical padding of bar */
static const int sidepad           = 4;        /* horizontal padding of bar */
static const char *fonts[]         = { "Victor:size=12", "Victor Mono Nerd Font:size=12:antialias=true:autohint=true"};
static const char dmenufont[]      =   "Victor Mono Nerd Font:size=12:antialias=true:autohint=true";
static char normbgcolor[]          = "#222222";
static char normbordercolor[]      = "#444444";
static char normfgcolor[]          = "#bbbbbb";
static char selfgcolor[]           = "#eeeeee";
static char selbgcolor[]           = "#005577";
static char selbordercolor[]       = "#005577";

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
// static const char *tags[] = { "1: Web ", "2: Dev ", "3: Dev ", "4: Dev ", "5: Chat ", "6: Sys " };
static const char *tags[] = { "1:  ", "2:  ", "3:  ", "4:  ", "5: 﨧 ", "6:  " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                       instance    title       tags mask     isfloating   monitor scratch key */
	{ "Gimp",                      NULL,       NULL,       0,            1,           -1,     0  },
	{ "Firefox",                   NULL,       NULL,       1 << 8,       0,           -1,     0  },
	{ "Confirm Form Resubmission", NULL,       NULL,       0,            1,           -1,     0  },
	{ NULL,                        NULL,   "scratchpad",   0,            1,           -1,    's' },
	{ NULL,                        NULL,      "fff",       0,            1,           -1,    'f' },
	{ NULL,                        NULL,   "bg-choose",    0,            1,           -1,    'b' },
	{ "st",                        NULL,      "tmux",      0 << 3,       0,           -1,     0  },
	{ "Vivaldi-Stable",            NULL,       NULL,       1 << 1,       0,           -1,     0  },
	{ "discord",                   NULL,       NULL,       1 << 4,       0,           -1,     0  },
	{ "MEGAsync",                  NULL,       NULL,       1 << 5,       1,           -1,     0  },
	{ "Barrier",                   NULL,       NULL,       1 << 5,       0,           -1,     0  },
	{ "keepassxc",                 NULL,       NULL,       0,            0,           -1,     0  },
	{ "Pavucontrol",               NULL,       NULL,       1 << 5,       0,           -1,     0  },
	{ "Blueman-manager",           NULL,       NULL,       1 << 5,       0,           -1,     0  },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ " ",      tile },    /* first entry is default */
	{ " ",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]     = { "dmenu_run",   "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, "-l", "5", NULL };
static const char *dmenu_query[]  = { "dmenu_query", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };

static const char *termcmd[]      = { "st", NULL };
static const char *tmux_termcmd[] = { "st", "-t", "tmux", "-e", "tmux" };

static const char *incscrcmd[]     = { "xbacklight", "-inc", "5", NULL };
static const char *decscrcmd[]     = { "xbacklight", "-dec", "5", NULL };

static const char *muteMcmd[]     = { "amixer", "-q", "set", "Master", "toggle", NULL };
static const char *muteScmd[]     = { "amixer", "-q", "set", "Speaker", "toggle", NULL };
static const char *muteHcmd[]     = { "amixer", "-q", "set", "Headphone", "toggle", NULL };
static const char *volupcmd[]     = { "amixer", "-q", "set", "Master", "10%+", NULL };
static const char *voldowncmd[]   = { "amixer", "-q", "set", "Master", "10%-", NULL };

static const char *miccmd[]       = { "amixer", "set", "Capture", "toggle", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", "st", "-t", "scratchpad", NULL }; 
static const char *fffcmd[]        = {"f", "st", "-t", "fff",        "-e", "fff" }; 
static const char *bgcmd[]         = {"b", "st", "-t", "bg-choose",  "-e", "bg-choose" }; 

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = dmenu_query } },
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = tmux_termcmd } },
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_v,      togglescratch,  {.v = fffcmd } },
	{ MODKEY,                       XK_p,      togglescratch,  {.v = bgcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
	{ 0,          XF86XK_MonBrightnessUp,      spawn,          {.v = incscrcmd} },
	{ 0,          XF86XK_MonBrightnessDown,    spawn,          {.v = decscrcmd} },
    { 0,          XF86XK_AudioMute,            spawn,          {.v = muteMcmd } },
    { 0,          XF86XK_AudioMute,            spawn,          {.v = muteScmd } },
    { 0,          XF86XK_AudioMute,            spawn,          {.v = muteHcmd } },
    { 0,          XF86XK_AudioLowerVolume,     spawn,          {.v = voldowncmd } },
    { 0,          XF86XK_AudioRaiseVolume,     spawn,          {.v = volupcmd } },
    { 0,          XF86XK_AudioMicMute,         spawn,          {.v = miccmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

