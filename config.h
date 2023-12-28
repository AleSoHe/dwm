/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int stairpx   = 60;       /* depth of the stairs layout */
static const int stairdirection     = 1;        /* 0: left-aligned, 1: right-aligned */
static const int stairsamesize      = 1;        /* 1 means shrink all the staired windows to the same size */
static const char *fonts[]          = { "Ubuntu Mono Nerd Font:pixelsize=16:antialias=true:autohint=true" };
//static const char *fonts[]          = { "terminus:style=Bold:size=16", "Noto Color Emoji:size=16"};
//static const char *fonts[]          = { "monospace:size=11" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray[]       = "#393939";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#ff0000";
static const char col_yellow[]      = "#d79921";
static const char col_orange[]      = "#d65d0e";

/* Gruvbox palette
static const char col_status_0[]       = "#928374";
static const char col_status_1[]       = "#9d0006";
static const char col_status_2[]       = "#79740e";
static const char col_status_3[]       = "#b57614";
static const char col_status_4[]       = "#076678";
*/

/* Dracula palette */
static const char col_bg[]             = "#282a36";
static const char col_status_0[]       = "#bd93f9"; /* Purple */
static const char col_status_1[]       = "#50fa7b"; /* Green */
static const char col_status_2[]       = "#ff79c6"; /* Pink */
static const char col_status_3[]       = "#f1fa8c"; /* Yellow */
static const char col_status_4[]       = "#ffb86c"; /* Orange */

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_bg, col_gray2 },
	[SchemeSel]  = { col_yellow, col_bg,  col_red  },

  /* Status Bar Colors (top right status information) */
	[SchemeBar0]  = { col_status_0,  col_bg, col_red  },
	[SchemeBar1]  = { col_status_1,  col_bg, col_red  },
	[SchemeBar2]  = { col_status_2,  col_bg, col_red  },
	[SchemeBar3]  = { col_status_3,  col_bg, col_red  },
	[SchemeBarLast]  = { col_status_4,  col_bg, col_red  },
};
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* tagging */
// 📜 📡📞📒📨📫📶🔊🔌🔑🔨🌎🌐🐛🚀🎻💩🔥💯⏳⧗ ⧖ ⌛β π 📆💰💻ℹ🚽🎥🎵🚀🛰️🛸🏁🔬👓📊🤓👓👥
//⌨ 📧📨📫📆📅
// 🄌 ➊ ➋ ➌ ➍ ➎ ➏ ➐ ➑ ➒ ➓
// ⓿ ❶ ❷ ❸ ❹ ❺ ❻ ❼ ❽ ❾ ❿
// ₁₂₃₄₅₆₇₈₉
//static const char *tags[] = {"💻₁","🌐₂","💻₃","🌐₄","💻₅","🌐₆","⌨₇","👥₈","📆₉","0"};
static const char *tags[] = {"1-💻","2-🌐","3-💻","4-🌐","5-💻","6-🌐","7-⌨","8-👥","9-📆","0"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Terminator",      NULL,     NULL,           0,         0,          1,           0,        -1 }, // TODO: Check if it's terminator or Terminator
	{ "terminator",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "grid.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "| 🖽🖽",      tile },    /* first entry is default */
	{ "| 🎈",      NULL },    /* no layout function means floating behavior */
	{ "| ◻️",      monocle },
 	{ "| 🌀",      spiral },
 	{ "| ↘",      dwindle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "[S]",      stairs },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "terminator", "--no-dbus", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY|ShiftMask,             XK_o,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY|ShiftMask,             XK_h,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[10]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_v,      togglesticky,   {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    // Brightness: z for down, x for up
    { MODKEY,                       XK_z,      spawn,          SHCMD("light -U 5") },
    { MODKEY,                       XK_x,      spawn,          SHCMD("light -A 5") },
    // VOLUME
    { 0, XF86XK_AudioRaiseVolume,   spawn,     SHCMD("vol up; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioLowerVolume,   spawn,     SHCMD("vol down; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioMute,          spawn,     SHCMD("vol mute; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioPlay,          spawn,     SHCMD("player play-pause") },
    { 0, XF86XK_AudioPause,         spawn,     SHCMD("player play-pause") },
    { 0, XF86XK_AudioStop,          spawn,     SHCMD("player stop") },
    { 0, XF86XK_AudioNext,          spawn,     SHCMD("player next") },
    { 0, XF86XK_AudioPrev,          spawn,     SHCMD("player previous") },
    // TAGS
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
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	//{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
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

