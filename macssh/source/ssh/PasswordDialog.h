/*
 * PasswordDialog.h
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

#include <Dialogs.h>
#include <Drag.h>

#define rSSH2LoginDialog		10004
#define rSSH2PasswordDialog		10001
#define rSSH2SOC1Dialog			10002
#define rSSH2SOC2Dialog			10003
#define rSSH2RandomizeDialog	10010
#define rSSH2ErrorDialog		10011

#define rInternalBufferDialog	256
#define rYesNoDialog			257

#ifdef __cplusplus
extern "C" {
#endif

Boolean SSH2RandomizeDialog( long *type, long *level, long *encrypt, Str255 label );
void SSH2ErrorDialog(char *mess1);

Boolean SSH2LoginDialog(StringPtr inhost, StringPtr iologin, StringPtr outpassword);
Boolean SSH2PasswordDialog (const char *inprompt, StringPtr outpassword, WindowPtr term);
short SSH2SOC1Dialog(const char *fingerprint);
short SSH2SOC2Dialog(const char *fingerprint);

void InternalBufferDialog (StringPtr inprompt, StringPtr outpassword);
Boolean YesNoDialog (StringPtr inprompt);

#ifdef __cplusplus
}
#endif

