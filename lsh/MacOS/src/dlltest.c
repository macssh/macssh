/*
 * dlltest.c
 * (c) 2000 Jean-Pierre Stierlin.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <MacTypes.h>
#include <Events.h>
#include <console.h>
#include <stdio.h>
#include <SIOUX.h>

#include <SIOUXGlobals.h>

#include "lsh_dll.h"

struct tctx {
	lshctx		*lsh_ctx;
};


char password[64];

void my_hdlevt(long userData, EventRecord *userEvent, long sleepTime)
{
	EventRecord	theEvent;
	lshctx		*lsh_ctx = ((struct tctx *)userData)->lsh_ctx;
	char		buf[4096];
	int			count;
	char		c;

	if ( userEvent == NULL ) {
		if ( SIOUXUseWaitNextEvent ) {
			WaitNextEvent( everyEvent, &theEvent, 0, NULL );
		} else {
			#if TARGET_API_MAC_OS8
			SystemTask();
			#endif
			GetNextEvent( everyEvent, &theEvent );
		}
		userEvent = &theEvent;
	}

	/* something to send ? */
	if ( (userEvent->what != keyDown && userEvent->what != autoKey) && !(userEvent->modifiers & cmdKey) ) {
		SIOUXHandleOneEvent( userEvent );
	} else {
		c = userEvent->message & charCodeMask;
		lsh_write( lsh_ctx, &c, 1 );
	}

	/* something received ? */
	while ((count = lsh_read(lsh_ctx, buf, 4096)) != 0) {
		printf("%.*s", count, buf);
		fflush(stdout);
	}

}

void my_log(long userData, const char *message)
{
	printf("%s", message);
}

char *my_getpass(long userData, const char *prompt)
{
	printf("%s", prompt);
	fflush(stdout);
	fgets(password, sizeof(password), stdin);
	password[strlen(password) - 1] = 0;
	return password;
}


int my_yes_or_no(long userData, const char *prompt, int def)
{
	char		input[32];

	printf("%s", prompt);
	fflush(stdout);
	do {
		fgets(input, sizeof(input), stdin);
		if (input[0] == 'y' || input[0] == 'Y')
			return 1;
		else if (input[0] == 'n' || input[0] == 'N')
			return 0;
		printf("please anwser with 'y' or 'n' : ");
		fflush(stdout);
	} while (1);
	return def;
}


int main(void)
{
	struct tctx my_ctx;
	lshctx		*lsh_ctx;
	char		argstr[1024];

	SIOUXSettings.autocloseonquit = 0;
	SIOUXSettings.asktosaveonclose = 0;
	SIOUXSettings.showstatusline = 1;
	SIOUXSettings.columns = 80;
	SIOUXSettings.rows = 48;
	SIOUXSettings.toppixel = 50;
	SIOUXSettings.leftpixel = 20;
	SIOUXSettings.tabspaces = 8;

	strcpy(argstr, "lsh");
	strcat(argstr, " -ljps");

	strcat(argstr, " --host-db \"");
	strcat(argstr, lsh_getprefsd());
	strcat(argstr, "known_hosts\"");

	strcat(argstr, " --capture-to \"");
	strcat(argstr, lsh_getprefsd());
	strcat(argstr, "known_hosts\"");

	strcat(argstr, " --sloppy-host-authentication");

	strcat(argstr, " -call -zzlib");

	//strcat(argstr, " --verbose --trace --debug");
	strcat(argstr, " --verbose");

	strcat(argstr, " --stdin dev:ttyin --stdout dev:ttyout --stderr dev:ttyerr");
 
	strcat(argstr, " 192.168.1.41");

	//strcat(argstr, " cvs -d/home/macssh server");

	printf("argstr : %s\n", argstr);
	fflush(stdout);

	lsh_ctx = lsh_new(argstr, my_hdlevt, my_log, my_getpass, my_yes_or_no, 0L, (long)&my_ctx);

	if ( lsh_ctx != NULL ) {

		my_ctx.lsh_ctx = lsh_ctx;

		printf("lsh_new ok\n");
		fflush(stdout);

		while (!SIOUXQuitting && lsh_running(lsh_ctx)) {

			lsh_yield();

			my_hdlevt((long)&my_ctx, NULL, 0L);

		}

		/* this kills the thread if still running */
		lsh_delete(lsh_ctx );

	} else {
		printf("lsh_new failed\n");
		fflush(stdout);
	}

	printf("done\n");
	fflush(stdout);

	return 0;
}
