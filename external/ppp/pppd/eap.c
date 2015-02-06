

#define RCSID	"$Id: eap.c,v 1.4 2004/11/09 22:39:25 paulus Exp $"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>

#include "pppd.h"
#include "pathnames.h"
#include "md5.h"
#include "eap.h"

#ifdef USE_SRP
#include <t_pwd.h>
#include <t_server.h>
#include <t_client.h>
#include "pppcrypt.h"
#endif /* USE_SRP */

#ifndef SHA_DIGESTSIZE
#define	SHA_DIGESTSIZE 20
#endif

static const char rcsid[] = RCSID;

eap_state eap_states[NUM_PPP];		/* EAP state; one for each unit */
#ifdef USE_SRP
static char *pn_secret = NULL;		/* Pseudonym generating secret */
#endif

static option_t eap_option_list[] = {
    { "eap-restart", o_int, &eap_states[0].es_server.ea_timeout,
      "Set retransmit timeout for EAP Requests (server)" },
    { "eap-max-sreq", o_int, &eap_states[0].es_server.ea_maxrequests,
      "Set max number of EAP Requests sent (server)" },
    { "eap-timeout", o_int, &eap_states[0].es_client.ea_timeout,
      "Set time limit for peer EAP authentication" },
    { "eap-max-rreq", o_int, &eap_states[0].es_client.ea_maxrequests,
      "Set max number of EAP Requests allows (client)" },
    { "eap-interval", o_int, &eap_states[0].es_rechallenge,
      "Set interval for EAP rechallenge" },
#ifdef USE_SRP
    { "srp-interval", o_int, &eap_states[0].es_lwrechallenge,
      "Set interval for SRP lightweight rechallenge" },
    { "srp-pn-secret", o_string, &pn_secret,
      "Long term pseudonym generation secret" },
    { "srp-use-pseudonym", o_bool, &eap_states[0].es_usepseudo,
      "Use pseudonym if offered one by server", 1 },
#endif
    { NULL }
};

static void eap_init __P((int unit));
static void eap_input __P((int unit, u_char *inp, int inlen));
static void eap_protrej __P((int unit));
static void eap_lowerup __P((int unit));
static void eap_lowerdown __P((int unit));
static int  eap_printpkt __P((u_char *inp, int inlen,
    void (*)(void *arg, char *fmt, ...), void *arg));

struct protent eap_protent = {
	PPP_EAP,		/* protocol number */
	eap_init,		/* initialization procedure */
	eap_input,		/* process a received packet */
	eap_protrej,		/* process a received protocol-reject */
	eap_lowerup,		/* lower layer has gone up */
	eap_lowerdown,		/* lower layer has gone down */
	NULL,			/* open the protocol */
	NULL,			/* close the protocol */
	eap_printpkt,		/* print a packet in readable form */
	NULL,			/* process a received data packet */
	1,			/* protocol enabled */
	"EAP",			/* text name of protocol */
	NULL,			/* text name of corresponding data protocol */
	eap_option_list,	/* list of command-line options */
	NULL,			/* check requested options; assign defaults */
	NULL,			/* configure interface for demand-dial */
	NULL			/* say whether to bring up link for this pkt */
};

static const u_char wkmodulus[] = {
	0xAC, 0x6B, 0xDB, 0x41, 0x32, 0x4A, 0x9A, 0x9B,
	0xF1, 0x66, 0xDE, 0x5E, 0x13, 0x89, 0x58, 0x2F,
	0xAF, 0x72, 0xB6, 0x65, 0x19, 0x87, 0xEE, 0x07,
	0xFC, 0x31, 0x92, 0x94, 0x3D, 0xB5, 0x60, 0x50,
	0xA3, 0x73, 0x29, 0xCB, 0xB4, 0xA0, 0x99, 0xED,
	0x81, 0x93, 0xE0, 0x75, 0x77, 0x67, 0xA1, 0x3D,
	0xD5, 0x23, 0x12, 0xAB, 0x4B, 0x03, 0x31, 0x0D,
	0xCD, 0x7F, 0x48, 0xA9, 0xDA, 0x04, 0xFD, 0x50,
	0xE8, 0x08, 0x39, 0x69, 0xED, 0xB7, 0x67, 0xB0,
	0xCF, 0x60, 0x95, 0x17, 0x9A, 0x16, 0x3A, 0xB3,
	0x66, 0x1A, 0x05, 0xFB, 0xD5, 0xFA, 0xAA, 0xE8,
	0x29, 0x18, 0xA9, 0x96, 0x2F, 0x0B, 0x93, 0xB8,
	0x55, 0xF9, 0x79, 0x93, 0xEC, 0x97, 0x5E, 0xEA,
	0xA8, 0x0D, 0x74, 0x0A, 0xDB, 0xF4, 0xFF, 0x74,
	0x73, 0x59, 0xD0, 0x41, 0xD5, 0xC3, 0x3E, 0xA7,
	0x1D, 0x28, 0x1E, 0x44, 0x6B, 0x14, 0x77, 0x3B,
	0xCA, 0x97, 0xB4, 0x3A, 0x23, 0xFB, 0x80, 0x16,
	0x76, 0xBD, 0x20, 0x7A, 0x43, 0x6C, 0x64, 0x81,
	0xF1, 0xD2, 0xB9, 0x07, 0x87, 0x17, 0x46, 0x1A,
	0x5B, 0x9D, 0x32, 0xE6, 0x88, 0xF8, 0x77, 0x48,
	0x54, 0x45, 0x23, 0xB5, 0x24, 0xB0, 0xD5, 0x7D,
	0x5E, 0xA7, 0x7A, 0x27, 0x75, 0xD2, 0xEC, 0xFA,
	0x03, 0x2C, 0xFB, 0xDB, 0xF5, 0x2F, 0xB3, 0x78,
	0x61, 0x60, 0x27, 0x90, 0x04, 0xE5, 0x7A, 0xE6,
	0xAF, 0x87, 0x4E, 0x73, 0x03, 0xCE, 0x53, 0x29,
	0x9C, 0xCC, 0x04, 0x1C, 0x7B, 0xC3, 0x08, 0xD8,
	0x2A, 0x56, 0x98, 0xF3, 0xA8, 0xD0, 0xC3, 0x82,
	0x71, 0xAE, 0x35, 0xF8, 0xE9, 0xDB, 0xFB, 0xB6,
	0x94, 0xB5, 0xC8, 0x03, 0xD8, 0x9F, 0x7A, 0xE4,
	0x35, 0xDE, 0x23, 0x6D, 0x52, 0x5F, 0x54, 0x75,
	0x9B, 0x65, 0xE3, 0x72, 0xFC, 0xD6, 0x8E, 0xF2,
	0x0F, 0xA7, 0x11, 0x1F, 0x9E, 0x4A, 0xFF, 0x73
};

/* Local forward declarations. */
static void eap_server_timeout __P((void *arg));

static const char *
eap_state_name(esc)
enum eap_state_code esc;
{
	static const char *state_names[] = { EAP_STATES };

	return (state_names[(int)esc]);
}

static void
eap_init(unit)
int unit;
{
	eap_state *esp = &eap_states[unit];

	BZERO(esp, sizeof (*esp));
	esp->es_unit = unit;
	esp->es_server.ea_timeout = EAP_DEFTIMEOUT;
	esp->es_server.ea_maxrequests = EAP_DEFTRANSMITS;
	esp->es_server.ea_id = (u_char)(drand48() * 0x100);
	esp->es_client.ea_timeout = EAP_DEFREQTIME;
	esp->es_client.ea_maxrequests = EAP_DEFALLOWREQ;
}

static void
eap_client_timeout(arg)
void *arg;
{
	eap_state *esp = (eap_state *) arg;

	if (!eap_client_active(esp))
		return;

	error("EAP: timeout waiting for Request from peer");
	auth_withpeer_fail(esp->es_unit, PPP_EAP);
	esp->es_client.ea_state = eapBadAuth;
}

void
eap_authwithpeer(unit, localname)
int unit;
char *localname;
{
	eap_state *esp = &eap_states[unit];

	/* Save the peer name we're given */
	esp->es_client.ea_name = localname;
	esp->es_client.ea_namelen = strlen(localname);

	esp->es_client.ea_state = eapListen;

	/*
	 * Start a timer so that if the other end just goes
	 * silent, we don't sit here waiting forever.
	 */
	if (esp->es_client.ea_timeout > 0)
		TIMEOUT(eap_client_timeout, (void *)esp,
		    esp->es_client.ea_timeout);
}

static void
eap_send_failure(esp)
eap_state *esp;
{
	u_char *outp;

	outp = outpacket_buf;
    
	MAKEHEADER(outp, PPP_EAP);

	PUTCHAR(EAP_FAILURE, outp);
	esp->es_server.ea_id++;
	PUTCHAR(esp->es_server.ea_id, outp);
	PUTSHORT(EAP_HEADERLEN, outp);

	output(esp->es_unit, outpacket_buf, EAP_HEADERLEN + PPP_HDRLEN);

	esp->es_server.ea_state = eapBadAuth;
	auth_peer_fail(esp->es_unit, PPP_EAP);
}

static void
eap_send_success(esp)
eap_state *esp;
{
	u_char *outp;

	outp = outpacket_buf;
    
	MAKEHEADER(outp, PPP_EAP);

	PUTCHAR(EAP_SUCCESS, outp);
	esp->es_server.ea_id++;
	PUTCHAR(esp->es_server.ea_id, outp);
	PUTSHORT(EAP_HEADERLEN, outp);

	output(esp->es_unit, outpacket_buf, PPP_HDRLEN + EAP_HEADERLEN);

	auth_peer_success(esp->es_unit, PPP_EAP, 0,
	    esp->es_server.ea_peer, esp->es_server.ea_peerlen);
}

#ifdef USE_SRP
static bool
pncrypt_setkey(int timeoffs)
{
	struct tm *tp;
	char tbuf[9];
	SHA1_CTX ctxt;
	u_char dig[SHA_DIGESTSIZE];
	time_t reftime;

	if (pn_secret == NULL)
		return (0);
	reftime = time(NULL) + timeoffs;
	tp = localtime(&reftime);
	SHA1Init(&ctxt);
	SHA1Update(&ctxt, pn_secret, strlen(pn_secret));
	strftime(tbuf, sizeof (tbuf), "%Y%m%d", tp);
	SHA1Update(&ctxt, tbuf, strlen(tbuf));
	SHA1Final(dig, &ctxt);
	return (DesSetkey(dig));
}

static char base64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

struct b64state {
	u_int32_t bs_bits;
	int bs_offs;
};

static int
b64enc(bs, inp, inlen, outp)
struct b64state *bs;
u_char *inp;
int inlen;
u_char *outp;
{
	int outlen = 0;

	while (inlen > 0) {
		bs->bs_bits = (bs->bs_bits << 8) | *inp++;
		inlen--;
		bs->bs_offs += 8;
		if (bs->bs_offs >= 24) {
			*outp++ = base64[(bs->bs_bits >> 18) & 0x3F];
			*outp++ = base64[(bs->bs_bits >> 12) & 0x3F];
			*outp++ = base64[(bs->bs_bits >> 6) & 0x3F];
			*outp++ = base64[bs->bs_bits & 0x3F];
			outlen += 4;
			bs->bs_offs = 0;
			bs->bs_bits = 0;
		}
	}
	return (outlen);
}

static int
b64flush(bs, outp)
struct b64state *bs;
u_char *outp;
{
	int outlen = 0;

	if (bs->bs_offs == 8) {
		*outp++ = base64[(bs->bs_bits >> 2) & 0x3F];
		*outp++ = base64[(bs->bs_bits << 4) & 0x3F];
		outlen = 2;
	} else if (bs->bs_offs == 16) {
		*outp++ = base64[(bs->bs_bits >> 10) & 0x3F];
		*outp++ = base64[(bs->bs_bits >> 4) & 0x3F];
		*outp++ = base64[(bs->bs_bits << 2) & 0x3F];
		outlen = 3;
	}
	bs->bs_offs = 0;
	bs->bs_bits = 0;
	return (outlen);
}

static int
b64dec(bs, inp, inlen, outp)
struct b64state *bs;
u_char *inp;
int inlen;
u_char *outp;
{
	int outlen = 0;
	char *cp;

	while (inlen > 0) {
		if ((cp = strchr(base64, *inp++)) == NULL)
			break;
		bs->bs_bits = (bs->bs_bits << 6) | (cp - base64);
		inlen--;
		bs->bs_offs += 6;
		if (bs->bs_offs >= 8) {
			*outp++ = bs->bs_bits >> (bs->bs_offs - 8);
			outlen++;
			bs->bs_offs -= 8;
		}
	}
	return (outlen);
}
#endif /* USE_SRP */

static void
eap_figure_next_state(esp, status)
eap_state *esp;
int status;
{
#ifdef USE_SRP
	unsigned char secbuf[MAXWORDLEN], clear[8], *sp, *dp;
	struct t_pw tpw;
	struct t_confent *tce, mytce;
	char *cp, *cp2;
	struct t_server *ts;
	int id, i, plen, toffs;
	u_char vals[2];
	struct b64state bs;
#endif /* USE_SRP */

	esp->es_server.ea_timeout = esp->es_savedtime;
	switch (esp->es_server.ea_state) {
	case eapBadAuth:
		return;

	case eapIdentify:
#ifdef USE_SRP
		/* Discard any previous session. */
		ts = (struct t_server *)esp->es_server.ea_session;
		if (ts != NULL) {
			t_serverclose(ts);
			esp->es_server.ea_session = NULL;
			esp->es_server.ea_skey = NULL;
		}
#endif /* USE_SRP */
		if (status != 0) {
			esp->es_server.ea_state = eapBadAuth;
			break;
		}
#ifdef USE_SRP
		/* If we've got a pseudonym, try to decode to real name. */
		if (esp->es_server.ea_peerlen > SRP_PSEUDO_LEN &&
		    strncmp(esp->es_server.ea_peer, SRP_PSEUDO_ID,
			SRP_PSEUDO_LEN) == 0 &&
		    (esp->es_server.ea_peerlen - SRP_PSEUDO_LEN) * 3 / 4 <
		    sizeof (secbuf)) {
			BZERO(&bs, sizeof (bs));
			plen = b64dec(&bs,
			    esp->es_server.ea_peer + SRP_PSEUDO_LEN,
			    esp->es_server.ea_peerlen - SRP_PSEUDO_LEN,
			    secbuf);
			toffs = 0;
			for (i = 0; i < 5; i++) {
				pncrypt_setkey(toffs);
				toffs -= 86400;
				if (!DesDecrypt(secbuf, clear)) {
					dbglog("no DES here; cannot decode "
					    "pseudonym");
					return;
				}
				id = *(unsigned char *)clear;
				if (id + 1 <= plen && id + 9 > plen)
					break;
			}
			if (plen % 8 == 0 && i < 5) {
				/*
				 * Note that this is always shorter than the
				 * original stored string, so there's no need
				 * to realloc.
				 */
				if ((i = plen = *(unsigned char *)clear) > 7)
					i = 7;
				esp->es_server.ea_peerlen = plen;
				dp = (unsigned char *)esp->es_server.ea_peer;
				BCOPY(clear + 1, dp, i);
				plen -= i;
				dp += i;
				sp = secbuf + 8;
				while (plen > 0) {
					(void) DesDecrypt(sp, dp);
					sp += 8;
					dp += 8;
					plen -= 8;
				}
				esp->es_server.ea_peer[
					esp->es_server.ea_peerlen] = '\0';
				dbglog("decoded pseudonym to \"%.*q\"",
				    esp->es_server.ea_peerlen,
				    esp->es_server.ea_peer);
			} else {
				dbglog("failed to decode real name");
				/* Stay in eapIdentfy state; requery */
				break;
			}
		}
		/* Look up user in secrets database. */
		if (get_srp_secret(esp->es_unit, esp->es_server.ea_peer,
		    esp->es_server.ea_name, (char *)secbuf, 1) != 0) {
			/* Set up default in case SRP entry is bad */
			esp->es_server.ea_state = eapMD5Chall;
			/* Get t_confent based on index in srp-secrets */
			id = strtol((char *)secbuf, &cp, 10);
			if (*cp++ != ':' || id < 0)
				break;
			if (id == 0) {
				mytce.index = 0;
				mytce.modulus.data = (u_char *)wkmodulus;
				mytce.modulus.len = sizeof (wkmodulus);
				mytce.generator.data = (u_char *)"\002";
				mytce.generator.len = 1;
				tce = &mytce;
			} else if ((tce = gettcid(id)) != NULL) {
				/*
				 * Client will have to verify this modulus/
				 * generator combination, and that will take
				 * a while.  Lengthen the timeout here.
				 */
				if (esp->es_server.ea_timeout > 0 &&
				    esp->es_server.ea_timeout < 30)
					esp->es_server.ea_timeout = 30;
			} else {
				break;
			}
			if ((cp2 = strchr(cp, ':')) == NULL)
				break;
			*cp2++ = '\0';
			tpw.pebuf.name = esp->es_server.ea_peer;
			tpw.pebuf.password.len = t_fromb64((char *)tpw.pwbuf,
			    cp);
			tpw.pebuf.password.data = tpw.pwbuf;
			tpw.pebuf.salt.len = t_fromb64((char *)tpw.saltbuf,
			    cp2);
			tpw.pebuf.salt.data = tpw.saltbuf;
			if ((ts = t_serveropenraw(&tpw.pebuf, tce)) == NULL)
				break;
			esp->es_server.ea_session = (void *)ts;
			esp->es_server.ea_state = eapSRP1;
			vals[0] = esp->es_server.ea_id + 1;
			vals[1] = EAPT_SRP;
			t_serveraddexdata(ts, vals, 2);
			/* Generate B; must call before t_servergetkey() */
			t_servergenexp(ts);
			break;
		}
#endif /* USE_SRP */
		esp->es_server.ea_state = eapMD5Chall;
		break;

	case eapSRP1:
#ifdef USE_SRP
		ts = (struct t_server *)esp->es_server.ea_session;
		if (ts != NULL && status != 0) {
			t_serverclose(ts);
			esp->es_server.ea_session = NULL;
			esp->es_server.ea_skey = NULL;
		}
#endif /* USE_SRP */
		if (status == 1) {
			esp->es_server.ea_state = eapMD5Chall;
		} else if (status != 0 || esp->es_server.ea_session == NULL) {
			esp->es_server.ea_state = eapBadAuth;
		} else {
			esp->es_server.ea_state = eapSRP2;
		}
		break;

	case eapSRP2:
#ifdef USE_SRP
		ts = (struct t_server *)esp->es_server.ea_session;
		if (ts != NULL && status != 0) {
			t_serverclose(ts);
			esp->es_server.ea_session = NULL;
			esp->es_server.ea_skey = NULL;
		}
#endif /* USE_SRP */
		if (status != 0 || esp->es_server.ea_session == NULL) {
			esp->es_server.ea_state = eapBadAuth;
		} else {
			esp->es_server.ea_state = eapSRP3;
		}
		break;

	case eapSRP3:
	case eapSRP4:
#ifdef USE_SRP
		ts = (struct t_server *)esp->es_server.ea_session;
		if (ts != NULL && status != 0) {
			t_serverclose(ts);
			esp->es_server.ea_session = NULL;
			esp->es_server.ea_skey = NULL;
		}
#endif /* USE_SRP */
		if (status != 0 || esp->es_server.ea_session == NULL) {
			esp->es_server.ea_state = eapBadAuth;
		} else {
			esp->es_server.ea_state = eapOpen;
		}
		break;

	case eapMD5Chall:
		if (status != 0) {
			esp->es_server.ea_state = eapBadAuth;
		} else {
			esp->es_server.ea_state = eapOpen;
		}
		break;

	default:
		esp->es_server.ea_state = eapBadAuth;
		break;
	}
	if (esp->es_server.ea_state == eapBadAuth)
		eap_send_failure(esp);
}

static void
eap_send_request(esp)
eap_state *esp;
{
	u_char *outp;
	u_char *lenloc;
	u_char *ptr;
	int outlen;
	int challen;
	char *str;
#ifdef USE_SRP
	struct t_server *ts;
	u_char clear[8], cipher[8], dig[SHA_DIGESTSIZE], *optr, *cp;
	int i, j;
	struct b64state b64;
	SHA1_CTX ctxt;
#endif /* USE_SRP */

	/* Handle both initial auth and restart */
	if (esp->es_server.ea_state < eapIdentify &&
	    esp->es_server.ea_state != eapInitial) {
		esp->es_server.ea_state = eapIdentify;
		if (explicit_remote) {
			/*
			 * If we already know the peer's
			 * unauthenticated name, then there's no
			 * reason to ask.  Go to next state instead.
			 */
			esp->es_server.ea_peer = remote_name;
			esp->es_server.ea_peerlen = strlen(remote_name);
			eap_figure_next_state(esp, 0);
		}
	}

	if (esp->es_server.ea_maxrequests > 0 &&
	    esp->es_server.ea_requests >= esp->es_server.ea_maxrequests) {
		if (esp->es_server.ea_responses > 0)
			error("EAP: too many Requests sent");
		else
			error("EAP: no response to Requests");
		eap_send_failure(esp);
		return;
	}

	outp = outpacket_buf;
    
	MAKEHEADER(outp, PPP_EAP);

	PUTCHAR(EAP_REQUEST, outp);
	PUTCHAR(esp->es_server.ea_id, outp);
	lenloc = outp;
	INCPTR(2, outp);

	switch (esp->es_server.ea_state) {
	case eapIdentify:
		PUTCHAR(EAPT_IDENTITY, outp);
		str = "Name";
		challen = strlen(str);
		BCOPY(str, outp, challen);
		INCPTR(challen, outp);
		break;

	case eapMD5Chall:
		PUTCHAR(EAPT_MD5CHAP, outp);
		/*
		 * pick a random challenge length between
		 * MIN_CHALLENGE_LENGTH and MAX_CHALLENGE_LENGTH
		 */
		challen = (drand48() *
		    (MAX_CHALLENGE_LENGTH - MIN_CHALLENGE_LENGTH)) +
			    MIN_CHALLENGE_LENGTH;
		PUTCHAR(challen, outp);
		esp->es_challen = challen;
		ptr = esp->es_challenge;
		while (--challen >= 0)
			*ptr++ = (u_char) (drand48() * 0x100);
		BCOPY(esp->es_challenge, outp, esp->es_challen);
		INCPTR(esp->es_challen, outp);
		BCOPY(esp->es_server.ea_name, outp, esp->es_server.ea_namelen);
		INCPTR(esp->es_server.ea_namelen, outp);
		break;

#ifdef USE_SRP
	case eapSRP1:
		PUTCHAR(EAPT_SRP, outp);
		PUTCHAR(EAPSRP_CHALLENGE, outp);

		PUTCHAR(esp->es_server.ea_namelen, outp);
		BCOPY(esp->es_server.ea_name, outp, esp->es_server.ea_namelen);
		INCPTR(esp->es_server.ea_namelen, outp);

		ts = (struct t_server *)esp->es_server.ea_session;
		assert(ts != NULL);
		PUTCHAR(ts->s.len, outp);
		BCOPY(ts->s.data, outp, ts->s.len);
		INCPTR(ts->s.len, outp);

		if (ts->g.len == 1 && ts->g.data[0] == 2) {
			PUTCHAR(0, outp);
		} else {
			PUTCHAR(ts->g.len, outp);
			BCOPY(ts->g.data, outp, ts->g.len);
			INCPTR(ts->g.len, outp);
		}

		if (ts->n.len != sizeof (wkmodulus) ||
		    BCMP(ts->n.data, wkmodulus, sizeof (wkmodulus)) != 0) {
			BCOPY(ts->n.data, outp, ts->n.len);
			INCPTR(ts->n.len, outp);
		}
		break;

	case eapSRP2:
		PUTCHAR(EAPT_SRP, outp);
		PUTCHAR(EAPSRP_SKEY, outp);

		ts = (struct t_server *)esp->es_server.ea_session;
		assert(ts != NULL);
		BCOPY(ts->B.data, outp, ts->B.len);
		INCPTR(ts->B.len, outp);
		break;

	case eapSRP3:
		PUTCHAR(EAPT_SRP, outp);
		PUTCHAR(EAPSRP_SVALIDATOR, outp);
		PUTLONG(SRPVAL_EBIT, outp);
		ts = (struct t_server *)esp->es_server.ea_session;
		assert(ts != NULL);
		BCOPY(t_serverresponse(ts), outp, SHA_DIGESTSIZE);
		INCPTR(SHA_DIGESTSIZE, outp);

		if (pncrypt_setkey(0)) {
			/* Generate pseudonym */
			optr = outp;
			cp = (unsigned char *)esp->es_server.ea_peer;
			if ((j = i = esp->es_server.ea_peerlen) > 7)
				j = 7;
			clear[0] = i;
			BCOPY(cp, clear + 1, j);
			i -= j;
			cp += j;
			if (!DesEncrypt(clear, cipher)) {
				dbglog("no DES here; not generating pseudonym");
				break;
			}
			BZERO(&b64, sizeof (b64));
			outp++;		/* space for pseudonym length */
			outp += b64enc(&b64, cipher, 8, outp);
			while (i >= 8) {
				(void) DesEncrypt(cp, cipher);
				outp += b64enc(&b64, cipher, 8, outp);
				cp += 8;
				i -= 8;
			}
			if (i > 0) {
				BCOPY(cp, clear, i);
				cp += i;
				while (i < 8) {
					*cp++ = drand48() * 0x100;
					i++;
				}
				(void) DesEncrypt(clear, cipher);
				outp += b64enc(&b64, cipher, 8, outp);
			}
			outp += b64flush(&b64, outp);

			/* Set length and pad out to next 20 octet boundary */
			i = outp - optr - 1;
			*optr = i;
			i %= SHA_DIGESTSIZE;
			if (i != 0) {
				while (i < SHA_DIGESTSIZE) {
					*outp++ = drand48() * 0x100;
					i++;
				}
			}

			/* Obscure the pseudonym with SHA1 hash */
			SHA1Init(&ctxt);
			SHA1Update(&ctxt, &esp->es_server.ea_id, 1);
			SHA1Update(&ctxt, esp->es_server.ea_skey,
			    SESSION_KEY_LEN);
			SHA1Update(&ctxt, esp->es_server.ea_peer,
			    esp->es_server.ea_peerlen);
			while (optr < outp) {
				SHA1Final(dig, &ctxt);
				cp = dig;
				while (cp < dig + SHA_DIGESTSIZE)
					*optr++ ^= *cp++;
				SHA1Init(&ctxt);
				SHA1Update(&ctxt, &esp->es_server.ea_id, 1);
				SHA1Update(&ctxt, esp->es_server.ea_skey,
				    SESSION_KEY_LEN);
				SHA1Update(&ctxt, optr - SHA_DIGESTSIZE,
				    SHA_DIGESTSIZE);
			}
		}
		break;

	case eapSRP4:
		PUTCHAR(EAPT_SRP, outp);
		PUTCHAR(EAPSRP_LWRECHALLENGE, outp);
		challen = MIN_CHALLENGE_LENGTH +
		    ((MAX_CHALLENGE_LENGTH - MIN_CHALLENGE_LENGTH) * drand48());
		esp->es_challen = challen;
		ptr = esp->es_challenge;
		while (--challen >= 0)
			*ptr++ = drand48() * 0x100;
		BCOPY(esp->es_challenge, outp, esp->es_challen);
		INCPTR(esp->es_challen, outp);
		break;
#endif /* USE_SRP */

	default:
		return;
	}

	outlen = (outp - outpacket_buf) - PPP_HDRLEN;
	PUTSHORT(outlen, lenloc);

	output(esp->es_unit, outpacket_buf, outlen + PPP_HDRLEN);

	esp->es_server.ea_requests++;

	if (esp->es_server.ea_timeout > 0)
		TIMEOUT(eap_server_timeout, esp, esp->es_server.ea_timeout);
}

void
eap_authpeer(unit, localname)
int unit;
char *localname;
{
	eap_state *esp = &eap_states[unit];

	/* Save the name we're given. */
	esp->es_server.ea_name = localname;
	esp->es_server.ea_namelen = strlen(localname);

	esp->es_savedtime = esp->es_server.ea_timeout;

	/* Lower layer up yet? */
	if (esp->es_server.ea_state == eapInitial ||
	    esp->es_server.ea_state == eapPending) {
		esp->es_server.ea_state = eapPending;
		return;
	}

	esp->es_server.ea_state = eapPending;

	/* ID number not updated here intentionally; hashed into M1 */
	eap_send_request(esp);
}

static void
eap_server_timeout(arg)
void *arg;
{
	eap_state *esp = (eap_state *) arg;

	if (!eap_server_active(esp))
		return;

	/* EAP ID number must not change on timeout. */
	eap_send_request(esp);
}

static void
eap_rechallenge(arg)
void *arg;
{
	eap_state *esp = (eap_state *)arg;

	if (esp->es_server.ea_state != eapOpen &&
	    esp->es_server.ea_state != eapSRP4)
		return;

	esp->es_server.ea_requests = 0;
	esp->es_server.ea_state = eapIdentify;
	eap_figure_next_state(esp, 0);
	esp->es_server.ea_id++;
	eap_send_request(esp);
}

static void
srp_lwrechallenge(arg)
void *arg;
{
	eap_state *esp = (eap_state *)arg;

	if (esp->es_server.ea_state != eapOpen ||
	    esp->es_server.ea_type != EAPT_SRP)
		return;

	esp->es_server.ea_requests = 0;
	esp->es_server.ea_state = eapSRP4;
	esp->es_server.ea_id++;
	eap_send_request(esp);
}

static void
eap_lowerup(unit)
int unit;
{
	eap_state *esp = &eap_states[unit];

	/* Discard any (possibly authenticated) peer name. */
	if (esp->es_server.ea_peer != NULL &&
	    esp->es_server.ea_peer != remote_name)
		free(esp->es_server.ea_peer);
	esp->es_server.ea_peer = NULL;
	if (esp->es_client.ea_peer != NULL)
		free(esp->es_client.ea_peer);
	esp->es_client.ea_peer = NULL;

	esp->es_client.ea_state = eapClosed;
	esp->es_server.ea_state = eapClosed;
}

static void
eap_lowerdown(unit)
int unit;
{
	eap_state *esp = &eap_states[unit];

	if (eap_client_active(esp) && esp->es_client.ea_timeout > 0) {
		UNTIMEOUT(eap_client_timeout, (void *)esp);
	}
	if (eap_server_active(esp)) {
		if (esp->es_server.ea_timeout > 0) {
			UNTIMEOUT(eap_server_timeout, (void *)esp);
		}
	} else {
		if ((esp->es_server.ea_state == eapOpen ||
		    esp->es_server.ea_state == eapSRP4) &&
		    esp->es_rechallenge > 0) {
			UNTIMEOUT(eap_rechallenge, (void *)esp);
		}
		if (esp->es_server.ea_state == eapOpen &&
		    esp->es_lwrechallenge > 0) {
			UNTIMEOUT(srp_lwrechallenge, (void *)esp);
		}
	}

	esp->es_client.ea_state = esp->es_server.ea_state = eapInitial;
	esp->es_client.ea_requests = esp->es_server.ea_requests = 0;
}

static void
eap_protrej(unit)
int unit;
{
	eap_state *esp = &eap_states[unit];

	if (eap_client_active(esp)) {
		error("EAP authentication failed due to Protocol-Reject");
		auth_withpeer_fail(unit, PPP_EAP);
	}
	if (eap_server_active(esp)) {
		error("EAP authentication of peer failed on Protocol-Reject");
		auth_peer_fail(unit, PPP_EAP);
	}
	eap_lowerdown(unit);
}

static void
eap_send_response(esp, id, typenum, str, lenstr)
eap_state *esp;
u_char id;
u_char typenum;
u_char *str;
int lenstr;
{
	u_char *outp;
	int msglen;

	outp = outpacket_buf;

	MAKEHEADER(outp, PPP_EAP);

	PUTCHAR(EAP_RESPONSE, outp);
	PUTCHAR(id, outp);
	esp->es_client.ea_id = id;
	msglen = EAP_HEADERLEN + sizeof (u_char) + lenstr;
	PUTSHORT(msglen, outp);
	PUTCHAR(typenum, outp);
	if (lenstr > 0) {
		BCOPY(str, outp, lenstr);
	}

	output(esp->es_unit, outpacket_buf, PPP_HDRLEN + msglen);
}

static void
eap_chap_response(esp, id, hash, name, namelen)
eap_state *esp;
u_char id;
u_char *hash;
char *name;
int namelen;
{
	u_char *outp;
	int msglen;

	outp = outpacket_buf;
    
	MAKEHEADER(outp, PPP_EAP);

	PUTCHAR(EAP_RESPONSE, outp);
	PUTCHAR(id, outp);
	esp->es_client.ea_id = id;
	msglen = EAP_HEADERLEN + 2 * sizeof (u_char) + MD5_SIGNATURE_SIZE +
	    namelen;
	PUTSHORT(msglen, outp);
	PUTCHAR(EAPT_MD5CHAP, outp);
	PUTCHAR(MD5_SIGNATURE_SIZE, outp);
	BCOPY(hash, outp, MD5_SIGNATURE_SIZE);
	INCPTR(MD5_SIGNATURE_SIZE, outp);
	if (namelen > 0) {
		BCOPY(name, outp, namelen);
	}

	output(esp->es_unit, outpacket_buf, PPP_HDRLEN + msglen);
}

#ifdef USE_SRP
static void
eap_srp_response(esp, id, subtypenum, str, lenstr)
eap_state *esp;
u_char id;
u_char subtypenum;
u_char *str;
int lenstr;
{
	u_char *outp;
	int msglen;

	outp = outpacket_buf;
    
	MAKEHEADER(outp, PPP_EAP);

	PUTCHAR(EAP_RESPONSE, outp);
	PUTCHAR(id, outp);
	esp->es_client.ea_id = id;
	msglen = EAP_HEADERLEN + 2 * sizeof (u_char) + lenstr;
	PUTSHORT(msglen, outp);
	PUTCHAR(EAPT_SRP, outp);
	PUTCHAR(subtypenum, outp);
	if (lenstr > 0) {
		BCOPY(str, outp, lenstr);
	}

	output(esp->es_unit, outpacket_buf, PPP_HDRLEN + msglen);
}

static void
eap_srpval_response(esp, id, flags, str)
eap_state *esp;
u_char id;
u_int32_t flags;
u_char *str;
{
	u_char *outp;
	int msglen;

	outp = outpacket_buf;
    
	MAKEHEADER(outp, PPP_EAP);

	PUTCHAR(EAP_RESPONSE, outp);
	PUTCHAR(id, outp);
	esp->es_client.ea_id = id;
	msglen = EAP_HEADERLEN + 2 * sizeof (u_char) + sizeof (u_int32_t) +
	    SHA_DIGESTSIZE;
	PUTSHORT(msglen, outp);
	PUTCHAR(EAPT_SRP, outp);
	PUTCHAR(EAPSRP_CVALIDATOR, outp);
	PUTLONG(flags, outp);
	BCOPY(str, outp, SHA_DIGESTSIZE);

	output(esp->es_unit, outpacket_buf, PPP_HDRLEN + msglen);
}
#endif /* USE_SRP */

static void
eap_send_nak(esp, id, type)
eap_state *esp;
u_char id;
u_char type;
{
	u_char *outp;
	int msglen;

	outp = outpacket_buf;

	MAKEHEADER(outp, PPP_EAP);

	PUTCHAR(EAP_RESPONSE, outp);
	PUTCHAR(id, outp);
	esp->es_client.ea_id = id;
	msglen = EAP_HEADERLEN + 2 * sizeof (u_char);
	PUTSHORT(msglen, outp);
	PUTCHAR(EAPT_NAK, outp);
	PUTCHAR(type, outp);

	output(esp->es_unit, outpacket_buf, PPP_HDRLEN + msglen);
}

#ifdef USE_SRP
static char *
name_of_pn_file()
{
	char *user, *path, *file;
	struct passwd *pw;
	size_t pl;
	static bool pnlogged = 0;

	pw = getpwuid(getuid());
	if (pw == NULL || (user = pw->pw_dir) == NULL || user[0] == 0) {
		errno = EINVAL;
		return (NULL);
	}
	file = _PATH_PSEUDONYM;
	pl = strlen(user) + strlen(file) + 2;
	path = malloc(pl);
	if (path == NULL)
		return (NULL);
	(void) slprintf(path, pl, "%s/%s", user, file);
	if (!pnlogged) {
		dbglog("pseudonym file: %s", path);
		pnlogged = 1;
	}
	return (path);
}

static int
open_pn_file(modebits)
mode_t modebits;
{
	char *path;
	int fd, err;

	if ((path = name_of_pn_file()) == NULL)
		return (-1);
	fd = open(path, modebits, S_IRUSR | S_IWUSR);
	err = errno;
	free(path);
	errno = err;
	return (fd);
}

static void
remove_pn_file()
{
	char *path;

	if ((path = name_of_pn_file()) != NULL) {
		(void) unlink(path);
		(void) free(path);
	}
}

static void
write_pseudonym(esp, inp, len, id)
eap_state *esp;
u_char *inp;
int len, id;
{
	u_char val;
	u_char *datp, *digp;
	SHA1_CTX ctxt;
	u_char dig[SHA_DIGESTSIZE];
	int dsize, fd, olen = len;

	/*
	 * Do the decoding by working backwards.  This eliminates the need
	 * to save the decoded output in a separate buffer.
	 */
	val = id;
	while (len > 0) {
		if ((dsize = len % SHA_DIGESTSIZE) == 0)
			dsize = SHA_DIGESTSIZE;
		len -= dsize;
		datp = inp + len;
		SHA1Init(&ctxt);
		SHA1Update(&ctxt, &val, 1);
		SHA1Update(&ctxt, esp->es_client.ea_skey, SESSION_KEY_LEN);
		if (len > 0) {
			SHA1Update(&ctxt, datp, SHA_DIGESTSIZE);
		} else {
			SHA1Update(&ctxt, esp->es_client.ea_name,
			    esp->es_client.ea_namelen);
		}
		SHA1Final(dig, &ctxt);
		for (digp = dig; digp < dig + SHA_DIGESTSIZE; digp++)
			*datp++ ^= *digp;
	}

	/* Now check that the result is sane */
	if (olen <= 0 || *inp + 1 > olen) {
		dbglog("EAP: decoded pseudonym is unusable <%.*B>", olen, inp);
		return;
	}

	/* Save it away */
	fd = open_pn_file(O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0) {
		dbglog("EAP: error saving pseudonym: %m");
		return;
	}
	len = write(fd, inp + 1, *inp);
	if (close(fd) != -1 && len == *inp) {
		dbglog("EAP: saved pseudonym");
		esp->es_usedpseudo = 0;
	} else {
		dbglog("EAP: failed to save pseudonym");
		remove_pn_file();
	}
}
#endif /* USE_SRP */

static void
eap_request(esp, inp, id, len)
eap_state *esp;
u_char *inp;
int id;
int len;
{
	u_char typenum;
	u_char vallen;
	int secret_len;
	char secret[MAXWORDLEN];
	char rhostname[256];
	MD5_CTX mdContext;
	u_char hash[MD5_SIGNATURE_SIZE];
#ifdef USE_SRP
	struct t_client *tc;
	struct t_num sval, gval, Nval, *Ap, Bval;
	u_char vals[2];
	SHA1_CTX ctxt;
	u_char dig[SHA_DIGESTSIZE];
	int fd;
#endif /* USE_SRP */

	/*
	 * Note: we update es_client.ea_id *only if* a Response
	 * message is being generated.  Otherwise, we leave it the
	 * same for duplicate detection purposes.
	 */

	esp->es_client.ea_requests++;
	if (esp->es_client.ea_maxrequests != 0 &&
	    esp->es_client.ea_requests > esp->es_client.ea_maxrequests) {
		info("EAP: received too many Request messages");
		if (esp->es_client.ea_timeout > 0) {
			UNTIMEOUT(eap_client_timeout, (void *)esp);
		}
		auth_withpeer_fail(esp->es_unit, PPP_EAP);
		return;
	}

	if (len <= 0) {
		error("EAP: empty Request message discarded");
		return;
	}

	GETCHAR(typenum, inp);
	len--;

	switch (typenum) {
	case EAPT_IDENTITY:
		if (len > 0)
			info("EAP: Identity prompt \"%.*q\"", len, inp);
#ifdef USE_SRP
		if (esp->es_usepseudo &&
		    (esp->es_usedpseudo == 0 ||
			(esp->es_usedpseudo == 1 &&
			    id == esp->es_client.ea_id))) {
			esp->es_usedpseudo = 1;
			/* Try to get a pseudonym */
			if ((fd = open_pn_file(O_RDONLY)) >= 0) {
				strcpy(rhostname, SRP_PSEUDO_ID);
				len = read(fd, rhostname + SRP_PSEUDO_LEN,
				    sizeof (rhostname) - SRP_PSEUDO_LEN);
				/* XXX NAI unsupported */
				if (len > 0) {
					eap_send_response(esp, id, typenum,
					    rhostname, len + SRP_PSEUDO_LEN);
				}
				(void) close(fd);
				if (len > 0)
					break;
			}
		}
		/* Stop using pseudonym now. */
		if (esp->es_usepseudo && esp->es_usedpseudo != 2) {
			remove_pn_file();
			esp->es_usedpseudo = 2;
		}
#endif /* USE_SRP */
		eap_send_response(esp, id, typenum, esp->es_client.ea_name,
		    esp->es_client.ea_namelen);
		break;

	case EAPT_NOTIFICATION:
		if (len > 0)
			info("EAP: Notification \"%.*q\"", len, inp);
		eap_send_response(esp, id, typenum, NULL, 0);
		break;

	case EAPT_NAK:
		/*
		 * Avoid the temptation to send Response Nak in reply
		 * to Request Nak here.  It can only lead to trouble.
		 */
		warn("EAP: unexpected Nak in Request; ignored");
		/* Return because we're waiting for something real. */
		return;

	case EAPT_MD5CHAP:
		if (len < 1) {
			error("EAP: received MD5-Challenge with no data");
			/* Bogus request; wait for something real. */
			return;
		}
		GETCHAR(vallen, inp);
		len--;
		if (vallen < 8 || vallen > len) {
			error("EAP: MD5-Challenge with bad length %d (8..%d)",
			    vallen, len);
			/* Try something better. */
			eap_send_nak(esp, id, EAPT_SRP);
			break;
		}

		/* Not so likely to happen. */
		if (vallen >= len + sizeof (rhostname)) {
			dbglog("EAP: trimming really long peer name down");
			BCOPY(inp + vallen, rhostname, sizeof (rhostname) - 1);
			rhostname[sizeof (rhostname) - 1] = '\0';
		} else {
			BCOPY(inp + vallen, rhostname, len - vallen);
			rhostname[len - vallen] = '\0';
		}

		/* In case the remote doesn't give us his name. */
		if (explicit_remote ||
		    (remote_name[0] != '\0' && vallen == len))
			strlcpy(rhostname, remote_name, sizeof (rhostname));

		/*
		 * Get the secret for authenticating ourselves with
		 * the specified host.
		 */
		if (!get_secret(esp->es_unit, esp->es_client.ea_name,
		    rhostname, secret, &secret_len, 0)) {
			dbglog("EAP: no MD5 secret for auth to %q", rhostname);
			eap_send_nak(esp, id, EAPT_SRP);
			break;
		}
		MD5_Init(&mdContext);
		typenum = id;
		MD5_Update(&mdContext, &typenum, 1);
		MD5_Update(&mdContext, secret, secret_len);
		BZERO(secret, sizeof (secret));
		MD5_Update(&mdContext, inp, vallen);
		MD5_Final(hash, &mdContext);
		eap_chap_response(esp, id, hash, esp->es_client.ea_name,
		    esp->es_client.ea_namelen);
		break;

#ifdef USE_SRP
	case EAPT_SRP:
		if (len < 1) {
			error("EAP: received empty SRP Request");
			/* Bogus request; wait for something real. */
			return;
		}

		/* Get subtype */
		GETCHAR(vallen, inp);
		len--;
		switch (vallen) {
		case EAPSRP_CHALLENGE:
			tc = NULL;
			if (esp->es_client.ea_session != NULL) {
				tc = (struct t_client *)esp->es_client.
				    ea_session;
				/*
				 * If this is a new challenge, then start
				 * over with a new client session context.
				 * Otherwise, just resend last response.
				 */
				if (id != esp->es_client.ea_id) {
					t_clientclose(tc);
					esp->es_client.ea_session = NULL;
					tc = NULL;
				}
			}
			/* No session key just yet */
			esp->es_client.ea_skey = NULL;
			if (tc == NULL) {
				GETCHAR(vallen, inp);
				len--;
				if (vallen >= len) {
					error("EAP: badly-formed SRP Challenge"
					    " (name)");
					/* Ignore badly-formed messages */
					return;
				}
				BCOPY(inp, rhostname, vallen);
				rhostname[vallen] = '\0';
				INCPTR(vallen, inp);
				len -= vallen;

				/*
				 * In case the remote doesn't give us his name,
				 * use configured name.
				 */
				if (explicit_remote ||
				    (remote_name[0] != '\0' && vallen == 0)) {
					strlcpy(rhostname, remote_name,
					    sizeof (rhostname));
				}

				if (esp->es_client.ea_peer != NULL)
					free(esp->es_client.ea_peer);
				esp->es_client.ea_peer = strdup(rhostname);
				esp->es_client.ea_peerlen = strlen(rhostname);

				GETCHAR(vallen, inp);
				len--;
				if (vallen >= len) {
					error("EAP: badly-formed SRP Challenge"
					    " (s)");
					/* Ignore badly-formed messages */
					return;
				}
				sval.data = inp;
				sval.len = vallen;
				INCPTR(vallen, inp);
				len -= vallen;

				GETCHAR(vallen, inp);
				len--;
				if (vallen > len) {
					error("EAP: badly-formed SRP Challenge"
					    " (g)");
					/* Ignore badly-formed messages */
					return;
				}
				/* If no generator present, then use value 2 */
				if (vallen == 0) {
					gval.data = (u_char *)"\002";
					gval.len = 1;
				} else {
					gval.data = inp;
					gval.len = vallen;
				}
				INCPTR(vallen, inp);
				len -= vallen;

				/*
				 * If no modulus present, then use well-known
				 * value.
				 */
				if (len == 0) {
					Nval.data = (u_char *)wkmodulus;
					Nval.len = sizeof (wkmodulus);
				} else {
					Nval.data = inp;
					Nval.len = len;
				}
				tc = t_clientopen(esp->es_client.ea_name,
				    &Nval, &gval, &sval);
				if (tc == NULL) {
					eap_send_nak(esp, id, EAPT_MD5CHAP);
					break;
				}
				esp->es_client.ea_session = (void *)tc;

				/* Add Challenge ID & type to verifier */
				vals[0] = id;
				vals[1] = EAPT_SRP;
				t_clientaddexdata(tc, vals, 2);
			}
			Ap = t_clientgenexp(tc);
			eap_srp_response(esp, id, EAPSRP_CKEY, Ap->data,
			    Ap->len);
			break;

		case EAPSRP_SKEY:
			tc = (struct t_client *)esp->es_client.ea_session;
			if (tc == NULL) {
				warn("EAP: peer sent Subtype 2 without 1");
				eap_send_nak(esp, id, EAPT_MD5CHAP);
				break;
			}
			if (esp->es_client.ea_skey != NULL) {
				/*
				 * ID number should not change here.  Warn
				 * if it does (but otherwise ignore).
				 */
				if (id != esp->es_client.ea_id) {
					warn("EAP: ID changed from %d to %d "
					    "in SRP Subtype 2 rexmit",
					    esp->es_client.ea_id, id);
				}
			} else {
				if (get_srp_secret(esp->es_unit,
				    esp->es_client.ea_name,
				    esp->es_client.ea_peer, secret, 0) == 0) {
					/*
					 * Can't work with this peer because
					 * the secret is missing.  Just give
					 * up.
					 */
					eap_send_nak(esp, id, EAPT_MD5CHAP);
					break;
				}
				Bval.data = inp;
				Bval.len = len;
				t_clientpasswd(tc, secret);
				BZERO(secret, sizeof (secret));
				esp->es_client.ea_skey =
				    t_clientgetkey(tc, &Bval);
				if (esp->es_client.ea_skey == NULL) {
					/* Server is rogue; stop now */
					error("EAP: SRP server is rogue");
					goto client_failure;
				}
			}
			eap_srpval_response(esp, id, SRPVAL_EBIT,
			    t_clientresponse(tc));
			break;

		case EAPSRP_SVALIDATOR:
			tc = (struct t_client *)esp->es_client.ea_session;
			if (tc == NULL || esp->es_client.ea_skey == NULL) {
				warn("EAP: peer sent Subtype 3 without 1/2");
				eap_send_nak(esp, id, EAPT_MD5CHAP);
				break;
			}
			/*
			 * If we're already open, then this ought to be a
			 * duplicate.  Otherwise, check that the server is
			 * who we think it is.
			 */
			if (esp->es_client.ea_state == eapOpen) {
				if (id != esp->es_client.ea_id) {
					warn("EAP: ID changed from %d to %d "
					    "in SRP Subtype 3 rexmit",
					    esp->es_client.ea_id, id);
				}
			} else {
				len -= sizeof (u_int32_t) + SHA_DIGESTSIZE;
				if (len < 0 || t_clientverify(tc, inp +
					sizeof (u_int32_t)) != 0) {
					error("EAP: SRP server verification "
					    "failed");
					goto client_failure;
				}
				GETLONG(esp->es_client.ea_keyflags, inp);
				/* Save pseudonym if user wants it. */
				if (len > 0 && esp->es_usepseudo) {
					INCPTR(SHA_DIGESTSIZE, inp);
					write_pseudonym(esp, inp, len, id);
				}
			}
			/*
			 * We've verified our peer.  We're now mostly done,
			 * except for waiting on the regular EAP Success
			 * message.
			 */
			eap_srp_response(esp, id, EAPSRP_ACK, NULL, 0);
			break;

		case EAPSRP_LWRECHALLENGE:
			if (len < 4) {
				warn("EAP: malformed Lightweight rechallenge");
				return;
			}
			SHA1Init(&ctxt);
			vals[0] = id;
			SHA1Update(&ctxt, vals, 1);
			SHA1Update(&ctxt, esp->es_client.ea_skey,
			    SESSION_KEY_LEN);
			SHA1Update(&ctxt, inp, len);
			SHA1Update(&ctxt, esp->es_client.ea_name,
			    esp->es_client.ea_namelen);
			SHA1Final(dig, &ctxt);
			eap_srp_response(esp, id, EAPSRP_LWRECHALLENGE, dig,
			    SHA_DIGESTSIZE);
			break;

		default:
			error("EAP: unknown SRP Subtype %d", vallen);
			eap_send_nak(esp, id, EAPT_MD5CHAP);
			break;
		}
		break;
#endif /* USE_SRP */

	default:
		info("EAP: unknown authentication type %d; Naking", typenum);
		eap_send_nak(esp, id, EAPT_SRP);
		break;
	}

	if (esp->es_client.ea_timeout > 0) {
		UNTIMEOUT(eap_client_timeout, (void *)esp);
		TIMEOUT(eap_client_timeout, (void *)esp,
		    esp->es_client.ea_timeout);
	}
	return;

#ifdef USE_SRP
client_failure:
	esp->es_client.ea_state = eapBadAuth;
	if (esp->es_client.ea_timeout > 0) {
		UNTIMEOUT(eap_client_timeout, (void *)esp);
	}
	esp->es_client.ea_session = NULL;
	t_clientclose(tc);
	auth_withpeer_fail(esp->es_unit, PPP_EAP);
#endif /* USE_SRP */
}

static void
eap_response(esp, inp, id, len)
eap_state *esp;
u_char *inp;
int id;
int len;
{
	u_char typenum;
	u_char vallen;
	int secret_len;
	char secret[MAXSECRETLEN];
	char rhostname[256];
	MD5_CTX mdContext;
	u_char hash[MD5_SIGNATURE_SIZE];
#ifdef USE_SRP
	struct t_server *ts;
	struct t_num A;
	SHA1_CTX ctxt;
	u_char dig[SHA_DIGESTSIZE];
#endif /* USE_SRP */

	if (esp->es_server.ea_id != id) {
		dbglog("EAP: discarding Response %d; expected ID %d", id,
		    esp->es_server.ea_id);
		return;
	}

	esp->es_server.ea_responses++;

	if (len <= 0) {
		error("EAP: empty Response message discarded");
		return;
	}

	GETCHAR(typenum, inp);
	len--;

	switch (typenum) {
	case EAPT_IDENTITY:
		if (esp->es_server.ea_state != eapIdentify) {
			dbglog("EAP discarding unwanted Identify \"%.q\"", len,
			    inp);
			break;
		}
		info("EAP: unauthenticated peer name \"%.*q\"", len, inp);
		if (esp->es_server.ea_peer != NULL &&
		    esp->es_server.ea_peer != remote_name)
			free(esp->es_server.ea_peer);
		esp->es_server.ea_peer = malloc(len + 1);
		if (esp->es_server.ea_peer == NULL) {
			esp->es_server.ea_peerlen = 0;
			eap_figure_next_state(esp, 1);
			break;
		}
		BCOPY(inp, esp->es_server.ea_peer, len);
		esp->es_server.ea_peer[len] = '\0';
		esp->es_server.ea_peerlen = len;
		eap_figure_next_state(esp, 0);
		break;

	case EAPT_NOTIFICATION:
		dbglog("EAP unexpected Notification; response discarded");
		break;

	case EAPT_NAK:
		if (len < 1) {
			info("EAP: Nak Response with no suggested protocol");
			eap_figure_next_state(esp, 1);
			break;
		}

		GETCHAR(vallen, inp);
		len--;

		if (!explicit_remote && esp->es_server.ea_state == eapIdentify){
			/* Peer cannot Nak Identify Request */
			eap_figure_next_state(esp, 1);
			break;
		}

		switch (vallen) {
		case EAPT_SRP:
			/* Run through SRP validator selection again. */
			esp->es_server.ea_state = eapIdentify;
			eap_figure_next_state(esp, 0);
			break;

		case EAPT_MD5CHAP:
			esp->es_server.ea_state = eapMD5Chall;
			break;

		default:
			dbglog("EAP: peer requesting unknown Type %d", vallen);
			switch (esp->es_server.ea_state) {
			case eapSRP1:
			case eapSRP2:
			case eapSRP3:
				esp->es_server.ea_state = eapMD5Chall;
				break;
			case eapMD5Chall:
			case eapSRP4:
				esp->es_server.ea_state = eapIdentify;
				eap_figure_next_state(esp, 0);
				break;
			default:
				break;
			}
			break;
		}
		break;

	case EAPT_MD5CHAP:
		if (esp->es_server.ea_state != eapMD5Chall) {
			error("EAP: unexpected MD5-Response");
			eap_figure_next_state(esp, 1);
			break;
		}
		if (len < 1) {
			error("EAP: received MD5-Response with no data");
			eap_figure_next_state(esp, 1);
			break;
		}
		GETCHAR(vallen, inp);
		len--;
		if (vallen != 16 || vallen > len) {
			error("EAP: MD5-Response with bad length %d", vallen);
			eap_figure_next_state(esp, 1);
			break;
		}

		/* Not so likely to happen. */
		if (vallen >= len + sizeof (rhostname)) {
			dbglog("EAP: trimming really long peer name down");
			BCOPY(inp + vallen, rhostname, sizeof (rhostname) - 1);
			rhostname[sizeof (rhostname) - 1] = '\0';
		} else {
			BCOPY(inp + vallen, rhostname, len - vallen);
			rhostname[len - vallen] = '\0';
		}

		/* In case the remote doesn't give us his name. */
		if (explicit_remote ||
		    (remote_name[0] != '\0' && vallen == len))
			strlcpy(rhostname, remote_name, sizeof (rhostname));

		/*
		 * Get the secret for authenticating the specified
		 * host.
		 */
		if (!get_secret(esp->es_unit, rhostname,
		    esp->es_server.ea_name, secret, &secret_len, 1)) {
			dbglog("EAP: no MD5 secret for auth of %q", rhostname);
			eap_send_failure(esp);
			break;
		}
		MD5_Init(&mdContext);
		MD5_Update(&mdContext, &esp->es_server.ea_id, 1);
		MD5_Update(&mdContext, secret, secret_len);
		BZERO(secret, sizeof (secret));
		MD5_Update(&mdContext, esp->es_challenge, esp->es_challen);
		MD5_Final(hash, &mdContext);
		if (BCMP(hash, inp, MD5_SIGNATURE_SIZE) != 0) {
			eap_send_failure(esp);
			break;
		}
		esp->es_server.ea_type = EAPT_MD5CHAP;
		eap_send_success(esp);
		eap_figure_next_state(esp, 0);
		if (esp->es_rechallenge != 0)
			TIMEOUT(eap_rechallenge, esp, esp->es_rechallenge);
		break;

#ifdef USE_SRP
	case EAPT_SRP:
		if (len < 1) {
			error("EAP: empty SRP Response");
			eap_figure_next_state(esp, 1);
			break;
		}
		GETCHAR(typenum, inp);
		len--;
		switch (typenum) {
		case EAPSRP_CKEY:
			if (esp->es_server.ea_state != eapSRP1) {
				error("EAP: unexpected SRP Subtype 1 Response");
				eap_figure_next_state(esp, 1);
				break;
			}
			A.data = inp;
			A.len = len;
			ts = (struct t_server *)esp->es_server.ea_session;
			assert(ts != NULL);
			esp->es_server.ea_skey = t_servergetkey(ts, &A);
			if (esp->es_server.ea_skey == NULL) {
				/* Client's A value is bogus; terminate now */
				error("EAP: bogus A value from client");
				eap_send_failure(esp);
			} else {
				eap_figure_next_state(esp, 0);
			}
			break;

		case EAPSRP_CVALIDATOR:
			if (esp->es_server.ea_state != eapSRP2) {
				error("EAP: unexpected SRP Subtype 2 Response");
				eap_figure_next_state(esp, 1);
				break;
			}
			if (len < sizeof (u_int32_t) + SHA_DIGESTSIZE) {
				error("EAP: M1 length %d < %d", len,
				    sizeof (u_int32_t) + SHA_DIGESTSIZE);
				eap_figure_next_state(esp, 1);
				break;
			}
			GETLONG(esp->es_server.ea_keyflags, inp);
			ts = (struct t_server *)esp->es_server.ea_session;
			assert(ts != NULL);
			if (t_serververify(ts, inp)) {
				info("EAP: unable to validate client identity");
				eap_send_failure(esp);
				break;
			}
			eap_figure_next_state(esp, 0);
			break;

		case EAPSRP_ACK:
			if (esp->es_server.ea_state != eapSRP3) {
				error("EAP: unexpected SRP Subtype 3 Response");
				eap_send_failure(esp);
				break;
			}
			esp->es_server.ea_type = EAPT_SRP;
			eap_send_success(esp);
			eap_figure_next_state(esp, 0);
			if (esp->es_rechallenge != 0)
				TIMEOUT(eap_rechallenge, esp,
				    esp->es_rechallenge);
			if (esp->es_lwrechallenge != 0)
				TIMEOUT(srp_lwrechallenge, esp,
				    esp->es_lwrechallenge);
			break;

		case EAPSRP_LWRECHALLENGE:
			if (esp->es_server.ea_state != eapSRP4) {
				info("EAP: unexpected SRP Subtype 4 Response");
				return;
			}
			if (len != SHA_DIGESTSIZE) {
				error("EAP: bad Lightweight rechallenge "
				    "response");
				return;
			}
			SHA1Init(&ctxt);
			vallen = id;
			SHA1Update(&ctxt, &vallen, 1);
			SHA1Update(&ctxt, esp->es_server.ea_skey,
			    SESSION_KEY_LEN);
			SHA1Update(&ctxt, esp->es_challenge, esp->es_challen);
			SHA1Update(&ctxt, esp->es_server.ea_peer,
			    esp->es_server.ea_peerlen);
			SHA1Final(dig, &ctxt);
			if (BCMP(dig, inp, SHA_DIGESTSIZE) != 0) {
				error("EAP: failed Lightweight rechallenge");
				eap_send_failure(esp);
				break;
			}
			esp->es_server.ea_state = eapOpen;
			if (esp->es_lwrechallenge != 0)
				TIMEOUT(srp_lwrechallenge, esp,
				    esp->es_lwrechallenge);
			break;
		}
		break;
#endif /* USE_SRP */

	default:
		/* This can't happen. */
		error("EAP: unknown Response type %d; ignored", typenum);
		return;
	}

	if (esp->es_server.ea_timeout > 0) {
		UNTIMEOUT(eap_server_timeout, (void *)esp);
	}

	if (esp->es_server.ea_state != eapBadAuth &&
	    esp->es_server.ea_state != eapOpen) {
		esp->es_server.ea_id++;
		eap_send_request(esp);
	}
}

static void
eap_success(esp, inp, id, len)
eap_state *esp;
u_char *inp;
int id;
int len;
{
	if (esp->es_client.ea_state != eapOpen && !eap_client_active(esp)) {
		dbglog("EAP unexpected success message in state %s (%d)",
		    eap_state_name(esp->es_client.ea_state),
		    esp->es_client.ea_state);
		return;
	}

	if (esp->es_client.ea_timeout > 0) {
		UNTIMEOUT(eap_client_timeout, (void *)esp);
	}

	if (len > 0) {
		/* This is odd.  The spec doesn't allow for this. */
		PRINTMSG(inp, len);
	}

	esp->es_client.ea_state = eapOpen;
	auth_withpeer_success(esp->es_unit, PPP_EAP, 0);
}

static void
eap_failure(esp, inp, id, len)
eap_state *esp;
u_char *inp;
int id;
int len;
{
	if (!eap_client_active(esp)) {
		dbglog("EAP unexpected failure message in state %s (%d)",
		    eap_state_name(esp->es_client.ea_state),
		    esp->es_client.ea_state);
	}

	if (esp->es_client.ea_timeout > 0) {
		UNTIMEOUT(eap_client_timeout, (void *)esp);
	}

	if (len > 0) {
		/* This is odd.  The spec doesn't allow for this. */
		PRINTMSG(inp, len);
	}

	esp->es_client.ea_state = eapBadAuth;

	error("EAP: peer reports authentication failure");
	auth_withpeer_fail(esp->es_unit, PPP_EAP);
}

static void
eap_input(unit, inp, inlen)
int unit;
u_char *inp;
int inlen;
{
	eap_state *esp = &eap_states[unit];
	u_char code, id;
	int len;

	/*
	 * Parse header (code, id and length).  If packet too short,
	 * drop it.
	 */
	if (inlen < EAP_HEADERLEN) {
		error("EAP: packet too short: %d < %d", inlen, EAP_HEADERLEN);
		return;
	}
	GETCHAR(code, inp);
	GETCHAR(id, inp);
	GETSHORT(len, inp);
	if (len < EAP_HEADERLEN || len > inlen) {
		error("EAP: packet has illegal length field %d (%d..%d)", len,
		    EAP_HEADERLEN, inlen);
		return;
	}
	len -= EAP_HEADERLEN;

	/* Dispatch based on message code */
	switch (code) {
	case EAP_REQUEST:
		eap_request(esp, inp, id, len);
		break;

	case EAP_RESPONSE:
		eap_response(esp, inp, id, len);
		break;

	case EAP_SUCCESS:
		eap_success(esp, inp, id, len);
		break;

	case EAP_FAILURE:
		eap_failure(esp, inp, id, len);
		break;

	default:				/* XXX Need code reject */
		/* Note: it's not legal to send EAP Nak here. */
		warn("EAP: unknown code %d received", code);
		break;
	}
}

static char *eap_codenames[] = {
	"Request", "Response", "Success", "Failure"
};

static char *eap_typenames[] = {
	"Identity", "Notification", "Nak", "MD5-Challenge",
	"OTP", "Generic-Token", NULL, NULL,
	"RSA", "DSS", "KEA", "KEA-Validate",
	"TLS", "Defender", "Windows 2000", "Arcot",
	"Cisco", "Nokia", "SRP"
};

static int
eap_printpkt(inp, inlen, printer, arg)
u_char *inp;
int inlen;
void (*printer) __P((void *, char *, ...));
void *arg;
{
	int code, id, len, rtype, vallen;
	u_char *pstart;
	u_int32_t uval;

	if (inlen < EAP_HEADERLEN)
		return (0);
	pstart = inp;
	GETCHAR(code, inp);
	GETCHAR(id, inp);
	GETSHORT(len, inp);
	if (len < EAP_HEADERLEN || len > inlen)
		return (0);

	if (code >= 1 && code <= sizeof(eap_codenames) / sizeof(char *))
		printer(arg, " %s", eap_codenames[code-1]);
	else
		printer(arg, " code=0x%x", code);
	printer(arg, " id=0x%x", id);
	len -= EAP_HEADERLEN;
	switch (code) {
	case EAP_REQUEST:
		if (len < 1) {
			printer(arg, " <missing type>");
			break;
		}
		GETCHAR(rtype, inp);
		len--;
		if (rtype >= 1 &&
		    rtype <= sizeof (eap_typenames) / sizeof (char *))
			printer(arg, " %s", eap_typenames[rtype-1]);
		else
			printer(arg, " type=0x%x", rtype);
		switch (rtype) {
		case EAPT_IDENTITY:
		case EAPT_NOTIFICATION:
			if (len > 0) {
				printer(arg, " <Message ");
				print_string((char *)inp, len, printer, arg);
				printer(arg, ">");
				INCPTR(len, inp);
				len = 0;
			} else {
				printer(arg, " <No message>");
			}
			break;

		case EAPT_MD5CHAP:
			if (len <= 0)
				break;
			GETCHAR(vallen, inp);
			len--;
			if (vallen > len)
				goto truncated;
			printer(arg, " <Value%.*B>", vallen, inp);
			INCPTR(vallen, inp);
			len -= vallen;
			if (len > 0) {
				printer(arg, " <Name ");
				print_string((char *)inp, len, printer, arg);
				printer(arg, ">");
				INCPTR(len, inp);
				len = 0;
			} else {
				printer(arg, " <No name>");
			}
			break;

		case EAPT_SRP:
			if (len < 3)
				goto truncated;
			GETCHAR(vallen, inp);
			len--;
			printer(arg, "-%d", vallen);
			switch (vallen) {
			case EAPSRP_CHALLENGE:
				GETCHAR(vallen, inp);
				len--;
				if (vallen >= len)
					goto truncated;
				if (vallen > 0) {
					printer(arg, " <Name ");
					print_string((char *)inp, vallen, printer,
					    arg);
					printer(arg, ">");
				} else {
					printer(arg, " <No name>");
				}
				INCPTR(vallen, inp);
				len -= vallen;
				GETCHAR(vallen, inp);
				len--;
				if (vallen >= len)
					goto truncated;
				printer(arg, " <s%.*B>", vallen, inp);
				INCPTR(vallen, inp);
				len -= vallen;
				GETCHAR(vallen, inp);
				len--;
				if (vallen > len)
					goto truncated;
				if (vallen == 0) {
					printer(arg, " <Default g=2>");
				} else {
					printer(arg, " <g%.*B>", vallen, inp);
				}
				INCPTR(vallen, inp);
				len -= vallen;
				if (len == 0) {
					printer(arg, " <Default N>");
				} else {
					printer(arg, " <N%.*B>", len, inp);
					INCPTR(len, inp);
					len = 0;
				}
				break;

			case EAPSRP_SKEY:
				printer(arg, " <B%.*B>", len, inp);
				INCPTR(len, inp);
				len = 0;
				break;

			case EAPSRP_SVALIDATOR:
				if (len < sizeof (u_int32_t))
					break;
				GETLONG(uval, inp);
				len -= sizeof (u_int32_t);
				if (uval & SRPVAL_EBIT) {
					printer(arg, " E");
					uval &= ~SRPVAL_EBIT;
				}
				if (uval != 0) {
					printer(arg, " f<%X>", uval);
				}
				if ((vallen = len) > SHA_DIGESTSIZE)
					vallen = SHA_DIGESTSIZE;
				printer(arg, " <M2%.*B%s>", len, inp,
				    len < SHA_DIGESTSIZE ? "?" : "");
				INCPTR(vallen, inp);
				len -= vallen;
				if (len > 0) {
					printer(arg, " <PN%.*B>", len, inp);
					INCPTR(len, inp);
					len = 0;
				}
				break;

			case EAPSRP_LWRECHALLENGE:
				printer(arg, " <Challenge%.*B>", len, inp);
				INCPTR(len, inp);
				len = 0;
				break;
			}
			break;
		}
		break;

	case EAP_RESPONSE:
		if (len < 1)
			break;
		GETCHAR(rtype, inp);
		len--;
		if (rtype >= 1 &&
		    rtype <= sizeof (eap_typenames) / sizeof (char *))
			printer(arg, " %s", eap_typenames[rtype-1]);
		else
			printer(arg, " type=0x%x", rtype);
		switch (rtype) {
		case EAPT_IDENTITY:
			if (len > 0) {
				printer(arg, " <Name ");
				print_string((char *)inp, len, printer, arg);
				printer(arg, ">");
				INCPTR(len, inp);
				len = 0;
			}
			break;

		case EAPT_NAK:
			if (len <= 0) {
				printer(arg, " <missing hint>");
				break;
			}
			GETCHAR(rtype, inp);
			len--;
			printer(arg, " <Suggested-type %02X", rtype);
			if (rtype >= 1 &&
			    rtype < sizeof (eap_typenames) / sizeof (char *))
				printer(arg, " (%s)", eap_typenames[rtype-1]);
			printer(arg, ">");
			break;

		case EAPT_MD5CHAP:
			if (len <= 0) {
				printer(arg, " <missing length>");
				break;
			}
			GETCHAR(vallen, inp);
			len--;
			if (vallen > len)
				goto truncated;
			printer(arg, " <Value%.*B>", vallen, inp);
			INCPTR(vallen, inp);
			len -= vallen;
			if (len > 0) {
				printer(arg, " <Name ");
				print_string((char *)inp, len, printer, arg);
				printer(arg, ">");
				INCPTR(len, inp);
				len = 0;
			} else {
				printer(arg, " <No name>");
			}
			break;

		case EAPT_SRP:
			if (len < 1)
				goto truncated;
			GETCHAR(vallen, inp);
			len--;
			printer(arg, "-%d", vallen);
			switch (vallen) {
			case EAPSRP_CKEY:
				printer(arg, " <A%.*B>", len, inp);
				INCPTR(len, inp);
				len = 0;
				break;

			case EAPSRP_CVALIDATOR:
				if (len < sizeof (u_int32_t))
					break;
				GETLONG(uval, inp);
				len -= sizeof (u_int32_t);
				if (uval & SRPVAL_EBIT) {
					printer(arg, " E");
					uval &= ~SRPVAL_EBIT;
				}
				if (uval != 0) {
					printer(arg, " f<%X>", uval);
				}
				printer(arg, " <M1%.*B%s>", len, inp,
				    len == SHA_DIGESTSIZE ? "" : "?");
				INCPTR(len, inp);
				len = 0;
				break;

			case EAPSRP_ACK:
				break;

			case EAPSRP_LWRECHALLENGE:
				printer(arg, " <Response%.*B%s>", len, inp,
				    len == SHA_DIGESTSIZE ? "" : "?");
				if ((vallen = len) > SHA_DIGESTSIZE)
					vallen = SHA_DIGESTSIZE;
				INCPTR(vallen, inp);
				len -= vallen;
				break;
			}
			break;
		}
		break;

	case EAP_SUCCESS:	/* No payload expected for these! */
	case EAP_FAILURE:
		break;

	truncated:
		printer(arg, " <truncated>");
		break;
	}

	if (len > 8)
		printer(arg, "%8B...", inp);
	else if (len > 0)
		printer(arg, "%.*B", len, inp);
	INCPTR(len, inp);

	return (inp - pstart);
}
