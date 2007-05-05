/*
 * Copyright (C) 2005 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Data structures for channel information.
 *
 * $Id: channels.h 8223 2007-05-05 12:58:06Z jilles $
 */

#ifndef CHANNELS_H
#define CHANNELS_H

#define MAXEXTMODES 5

struct channel_
{
  char *name;

  unsigned int modes;
  char *key;
  unsigned int limit;
  char *extmodes[MAXEXTMODES]; /* non-standard simple modes with param eg +j */

  unsigned int nummembers;

  time_t ts;

  char *topic;
  char *topic_setter;
  time_t topicts;

  list_t members;
  list_t bans;
};

/* struct for channel memberships */
struct chanuser_
{
  channel_t *chan;
  user_t *user;
  unsigned int modes;
  node_t unode;
  node_t cnode;
};

struct chanban_
{
  channel_t *chan;
  char *mask;
  int type; /* 'b', 'e', 'I', etc -- jilles */
};

#define CMODE_OP        0x00000020      /* SPECIAL */
#define CMODE_VOICE     0x00000200      /* SPECIAL */

#define CMODE_INVITE    0x00000001
#define CMODE_KEY       0x00000002
#define CMODE_LIMIT     0x00000004
#define CMODE_MOD       0x00000008
#define CMODE_NOEXT     0x00000010
#define CMODE_PRIV      0x00000040      /* AKA PARA */
#define CMODE_SEC       0x00000080
#define CMODE_TOPIC     0x00000100

#define MTYPE_NUL 0
#define MTYPE_ADD 1
#define MTYPE_DEL 2

struct cmode_
{
        char mode;
        unsigned int value;
};

struct extmode
{
	char mode;
	boolean_t (*check)(const char *, channel_t *, mychan_t *, user_t *, myuser_t *);
};

/* channel related hooks */
typedef struct {
	chanuser_t *cu; /* Write NULL here if you kicked the user.
			   When kicking the last user, you must join a
			   service first, otherwise the channel may be
			   destroyed and crashes may occur. The service may
			   not part until you return; chanserv provides
			   MC_INHABIT to help with this.
			   This also prevents kick/rejoin floods.
			   If this is NULL, a previous function kicked
			   the user */
} hook_channel_joinpart_t;

typedef struct {
	user_t *u;
        channel_t *c;
        char *msg;
} hook_cmessage_data_t;

typedef struct {
	user_t *u; /* Online user that changed the topic */
	server_t *s; /* Server that restored a topic */
        channel_t *c; /* Channel still has old topic */
        char *setter; /* Stored setter string, can be nick, nick!user@host
			 or server */
	time_t ts; /* Time the topic was changed */
	char *topic; /* New topic */
	int approved; /* Write non-zero here to cancel the change */
} hook_channel_topic_check_t;

/* cmode.c */
E char *flags_to_string(int flags);
E int mode_to_flag(char c);
E void channel_mode(user_t *source, channel_t *chan, int parc, char *parv[]);
E void channel_mode_va(user_t *source, channel_t *chan, int parc, char *parv0, ...);
E void clear_simple_modes(channel_t *c);
E char *channel_modes(channel_t *c, boolean_t doparams);
E void modestack_flush_channel(channel_t *channel);
E void modestack_forget_channel(channel_t *channel);
E void modestack_finalize_channel(channel_t *channel);
E void modestack_mode_simple(char *source, channel_t *channel, int dir, int flags);
E void modestack_mode_limit(char *source, channel_t *channel, int dir, unsigned int limit);
E void modestack_mode_ext(char *source, channel_t *channel, int dir, int i, const char *value);
E void modestack_mode_param(char *source, channel_t *channel, int dir, char type, const char *value);
E void check_modes(mychan_t *mychan, boolean_t sendnow);

/* channels.c */
E dictionary_tree_t *chanlist;

E void init_channels(void);

E channel_t *channel_add(const char *name, unsigned int ts, server_t *creator);
E void channel_delete(channel_t *c);
E channel_t *channel_find(const char *name);

E chanuser_t *chanuser_add(channel_t *chan, const char *user);
E void chanuser_delete(channel_t *chan, user_t *user);
E chanuser_t *chanuser_find(channel_t *chan, user_t *user);

E chanban_t *chanban_add(channel_t *chan, const char *mask, int type);
E void chanban_delete(chanban_t *c);
E chanban_t *chanban_find(channel_t *chan, const char *mask, int type);
E void chanban_clear(channel_t *chan);

#endif

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
