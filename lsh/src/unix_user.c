/* unix_user.c
 *
 * User-related functions on UN*X
 *
 * $Id$
 */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 2000 Niels Möller
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

#include "server_userauth.h"

#include "format.h"
#include "io.h"
#include "reaper.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/wait.h>

#include <signal.h>

#if HAVE_CRYPT_H
# include <crypt.h>
#endif
#include <pwd.h>
#include <grp.h>

#if HAVE_SHADOW_H
#include <shadow.h>
#endif

#if WITH_UTMP
# if HAVE_UTMP_H
#  include <utmp.h>
# endif

# if HAVE_UTMPX_H
#  include <utmpx.h>
# endif
#else /* !WITH_UTMP */
# struct utmp;
#endif

#if HAVE_LIBUTIL_H
# include <libutil.h>
#endif

/* Forward declaration */
struct unix_user_db;

#include "unix_user.c.x"


/* GABA:
   (class
     (name logout_cleanup)
     (super exit_callback)
     (vars
       (process object resource)
       (log space "struct utmp")
       (c object exit_callback)))
*/

static void
do_logout_cleanup(struct exit_callback *s,
		  int signaled, int core, int value)
{
  CAST(logout_cleanup, self, s);

  /* No need to signal the process. */
  self->process->alive = 0;

#if defined(WITH_UTMP) && defined(HAVE_LOGWTMP)
  if (self->log)
    logwtmp(self->log->ut_line,
	    "",
	    self->log->ut_host);
#endif /* WITH_UTMP && HAVE_LOGWTMP */
  EXIT_CALLBACK(self->c, signaled, core, value);
}

static struct exit_callback *
make_logout_cleanup(struct resource *process,
		    struct utmp *log,
		    struct exit_callback *c)
{
  NEW(logout_cleanup, self);
  self->super.exit = do_logout_cleanup;

  self->process = process;
  self->log = log;
  self->c = c;

  return &self->super;
}


#if WITH_UTMP
static void
lsh_strncpy(char *dst, unsigned n, struct lsh_string *s)
{
  unsigned length = MIN(n - 1, s->length);
  memcpy(dst, s->data, length);
  dst[length] = '\0';
}

/* Strips the leading "/dev/" part of s. */
static void
lsh_strncpy_tty(char *dst, unsigned n, struct lsh_string *s)
{
  /* "/dev/" is 5 characters */
  if (s->length <= 5)
    lsh_strncpy(dst, n, s);
  {
    unsigned length = MIN(n - 1, s->length - 5);
    memcpy(dst, s->data + 5, length);
    dst[length] = '\0';
  }
}

#define CP(dst, src) lsh_strncpy(dst, sizeof(dst), src);
#define CP_TTY(dst, src) lsh_strncpy_tty(dst, sizeof(dst), src);

static struct utmp *
lsh_make_utmp(struct lsh_user *user,
	      struct address_info *peer,
	      struct lsh_string *ttyname)
{
  struct utmp *log;
  NEW_SPACE(log);

#if HAVE_UT_NAME
  CP(log->ut_name, user->name);
#elif HAVE_UT_USER
  CP(log->ut_user, user->name);
#endif
  
  CP_TTY(log->ut_line, ttyname);

#if HAVE_UT_HOST
  CP(log->ut_host, peer->ip);
#endif
  
  return log;
}
#undef CP
#undef CP_TTY

#endif /* WITH_UTMP */


/* GABA:
   (class
     (name process_resource)
     (super resource)
     (vars
       (pid . pid_t)
       ; Signal used for killing the process.
       (signal . int)))
*/

static void
do_kill_process(struct resource *r)
{
  CAST(process_resource, self, r);

  if (self->super.alive)
    {
      self->super.alive = 0;
      /* NOTE: This function only makes one attempt at killing the
       * process. An improvement would be to install a callout handler
       * which will kill -9 the process after a delay, if it hasn't died
       * voluntarily. */

      if (kill(self->pid, self->signal) < 0)
	{
	  werror("do_kill_process: kill failed (errno = %i): %z\n",
		 errno, STRERROR(errno));
	}
    }
}

static struct resource *
make_process_resource(pid_t pid, int signal)
{
  NEW(process_resource, self);
  resource_init(&self->super, do_kill_process);

  self->pid = pid;
  self->signal = signal;

  return &self->super;
}


/* GABA:
   (class
     (name unix_user)
     (super lsh_user)
     (vars
       (gid . gid_t)

       ; Context needed for some methods.
       (ctx object unix_user_db)
       
       ; These strings include a terminating NUL-character, for 
       ; compatibility with library and system calls. This applies also
       ; to the inherited name attribute.

       (passwd string)  ; Crypted passwd
       (home string)
       (shell string))) */

/* GABA:
   (class
     (name pwhelper_callback)
     (super exit_callback)
     (vars
       (user object unix_user)
       (c object command_continuation)
       (e object exception_handler)))
*/

static void
do_pwhelper_callback(struct exit_callback *s,
		     int signaled, int core UNUSED, int value)
{
  CAST(pwhelper_callback, self, s);

  if (signaled || value)
    {
      static const struct exception invalid_password
	= STATIC_EXCEPTION(EXC_USERAUTH, "Invalid password according to helper program.");
      EXCEPTION_RAISE(self->e, &invalid_password);
    }
  else
    COMMAND_RETURN(self->c, self->user);
}

static struct exit_callback *
make_pwhelper_callback(struct unix_user *user,
		       struct command_continuation *c,
		       struct exception_handler *e)
{
  NEW(pwhelper_callback, self);
  self->super.exit = do_pwhelper_callback;
  self->user = user;
  self->c = c;
  self->e = e;

  return &self->super;
}

/* NOTE: Consumes the pw string if successful. */
static int
kerberos_check_pw(struct unix_user *user, struct lsh_string *pw,
		  struct command_continuation *c,
		  struct exception_handler *e)
{
  /* Because kerberos is big and complex, we fork a separate process
   * to do the work. */

  int in[2];
  pid_t child;

  /* First look if the helper program seems to exist. */
  if (access(user->ctx->pw_helper, X_OK) < 0)
    {
      /* No help available */
      werror("Password helper program '%z' not available (errno = %d): %z\n",
	     user->ctx->pw_helper, errno, STRERROR(errno));
      return 0;
    }
  
  if (!lsh_make_pipe(in))
    {
      werror("kerberos_check_pw: Failed to create pipe.\n");
      return 0;
    }
  
  child = fork();
  
  switch (child)
    {
    case -1:
      werror("kerberos_check_pw: fork failed: %z\n", STRERROR(errno));
      return 0;

    case 0:
      {  /* Child */
	int null_fd;
      
	null_fd = open("/dev/null", O_RDWR);
	if (null_fd < 0)
	  {
	    werror("kerberos_check_pw: Failed to open /dev/null.\n");
	    _exit(EXIT_FAILURE);
	  }
	if (dup2(in[0], STDIN_FILENO) < 0)
	  {
	    werror("kerberos_check_pw: Can't dup stdin!\n");
	    _exit(EXIT_FAILURE);
	  }

	if (dup2(null_fd, STDOUT_FILENO) < 0)
	  {
	    werror("kerberos_check_pw: Can't dup stdout!\n");
	    _exit(EXIT_FAILURE);
	  }

	if (dup2(null_fd, STDERR_FILENO) < 0)
	  {
	    _exit(EXIT_FAILURE);
	  }
      
	close(in[1]);
	close(null_fd);

	execl(user->ctx->pw_helper, user->ctx->pw_helper, user->super.name->data, NULL);
	_exit(EXIT_FAILURE);
      }
    default:
      {
	/* Parent */
	struct lsh_fd *fd;

	close(in[0]);

	fd = io_write(make_lsh_fd(user->ctx->backend, in[1], e),
		      pw->length, NULL);

	A_WRITE(&fd->write_buffer->super, pw);

	REAP(user->ctx->reaper, child, make_pwhelper_callback(user, c, e));

	return 1;
      }
    }
}

/* FIXME: This could be generalized to support some kind of list of
 * password databases. The current code first checks for unix
 * passwords, and if that fails, it optionally invokes a helper
 * program to verify the password, typically used for kerberos. */
static void
do_verify_password(struct lsh_user *s,
		   struct lsh_string *password,
		   struct command_continuation *c,
		   struct exception_handler *e)
{
  CAST(unix_user, user, s);
  const struct exception *x = NULL;

  /* Convert password to a NUL-terminated string; no supported
   * password verification methods allows passwords containing NUL. */

  password = make_cstring(password, 1);
  
  if (!password)
    {
      static const struct exception invalid_passwd
	= STATIC_EXCEPTION(EXC_USERAUTH, "NUL character in password.");

      EXCEPTION_RAISE(e, &invalid_passwd);
      return;
    }
  
  /* NOTE: Check for accounts with empty passwords. */
  if (!user->passwd || (user->passwd->length < 2) )
    {
      static const struct exception no_passwd
	= STATIC_EXCEPTION(EXC_USERAUTH, "No password in passwd db.");

      x = &no_passwd;

      /* NOTE: We attempt using kerberos passwords even if the
       * passwd entry is totally bogus. */
      goto try_helper;
    }

  /* Try password authentication against the ordinary unix database. */
  {
    char *salt = user->passwd->data;

    /* NOTE: crypt uses the *ugly* convention of returning pointers
     * to static buffers. */

    if (strcmp(crypt(password->data, salt), user->passwd->data))
      {
	/* Passwd doesn't match */
	static const struct exception invalid_passwd
	  = STATIC_EXCEPTION(EXC_USERAUTH, "Incorrect password.");
	
	x = &invalid_passwd;

	goto try_helper;
      }
    /* Unix style password authentication succeded. */
    lsh_string_free(password);
    COMMAND_RETURN(c, user);
    return;
  }
  
 try_helper:

  /* We get here if checks against the ordinary passwd database failed. */
  assert(x);
  
  if (user->ctx->pw_helper && kerberos_check_pw(user, password, c, e))
    /* The helper program takes responsibility for password
     * verification, and it also consumed the password string so that
     * we don't need to free it here. */
    ;
  else
    {
      lsh_string_free(password);
      EXCEPTION_RAISE(e, x);
    }
}

/* NOTE: No arbitrary file names are passed to this function, so we don't have
 * to check for things like "../../some/secret/file" */
static int
do_file_exists(struct lsh_user *u,
	       struct lsh_string *name,
	       int free)
{
  CAST(unix_user, user, u);
  struct lsh_string *path;
  struct stat st;
  
  if (!user->home)
    {
      if (free)
	lsh_string_free(name);
      return 0;
    }
  
  path = ssh_cformat(free ? "%lS/%lfS" : "%lS/%lS",
		     user->home, name);

  if (stat(path->data, &st) == 0)
    {
      lsh_string_free(path);
      return 1;
    }
  lsh_string_free(path);
  return 0;
}

static const struct exception *
check_user_permissions(struct stat *sbuf, const char *fname,
		       uid_t uid, int secret)
{
  mode_t bad = secret ? (S_IRWXG | S_IRWXO) : (S_IWGRP | S_IWOTH);

  if (!S_ISREG(sbuf->st_mode))
    {
      werror("io.c: %z is not a regular file.\n",
	     fname);
      return make_io_exception(EXC_IO_OPEN_READ, NULL, 0, "Not a regular file");
    }
  if (sbuf->st_uid != uid)
    {
      werror("io.c: %z not owned by the right user (%i)\n",
	     fname, uid);
      return make_io_exception(EXC_IO_OPEN_READ, NULL, 0, "Bad owner");
    }

  if (sbuf->st_mode & bad)
    {
      werror("io.c: Permissions on %z too loose.\n",
	     fname);
      return make_io_exception(EXC_IO_OPEN_READ, NULL, 0, "Bad permissions");
    }

  return NULL;
}

/* NOTE: No arbitrary file names are passed to this method, so we
 * don't have to check for things like "../../some/secret/file" */

static void 
do_read_file(struct lsh_user *u, 
	     const char *name, int secret,
	     struct command_continuation *c,
	     struct exception_handler *e)
{
  CAST(unix_user, user, u);
  struct lsh_string *f;
  struct stat sbuf;
  const struct exception *x;

  pid_t child;
  /* out[0] for reading, out[1] for writing */
  int out[2];

  uid_t me = getuid();

  /* There's no point trying to read other user's files unless we're
   * root. */

  if (me && (me != user->super.uid) )
    {
      EXCEPTION_RAISE(e, make_io_exception(EXC_IO_OPEN_READ, NULL, 0, "Access denied."));
      return;
    }
    
  if (!user->home)
    {
      EXCEPTION_RAISE(e, make_io_exception(EXC_IO_OPEN_READ, NULL,
					   ENOENT, "No home directory"));
      return;
    }

  f = ssh_cformat("%lS/.lsh/%lz", user->home, name);
  
  if (stat(f->data, &sbuf) < 0)
    {
      if (errno != ENOENT)
	werror("io_read_user_file: Failed to stat %S (errno = %i): %z\n",
	       f, errno, STRERROR(errno));

      EXCEPTION_RAISE(e, make_io_exception(EXC_IO_OPEN_READ, NULL, errno, NULL));

      lsh_string_free(f);
      return;
    }

  /* Perform a preliminary permissions check before forking, as errors
   * detected by the child process are not reported as accurately. */

  x = check_user_permissions(&sbuf, f->data, user->super.uid, secret);
  if (x)
    {
      EXCEPTION_RAISE(e, x);
      lsh_string_free(f);
      return;
    }
  
  if (!lsh_make_pipe(out))
    {
      EXCEPTION_RAISE(e, make_io_exception(EXC_IO_OPEN_READ, NULL, errno, NULL));
      lsh_string_free(f);
      return;
    }
  
  child = fork();

  switch (child)
    {
    case -1:
      /* Error */
      EXCEPTION_RAISE(e, make_io_exception(EXC_IO_OPEN_READ, NULL, errno, NULL));

      close(out[0]); close(out[1]);
      lsh_string_free(f);
      return;
      
    default:
      /* Parent */
      close(out[1]);

      /* NOTE: We could install an exit handler for tre child process,
       * but there's nothing useful for that to do. */
      COMMAND_RETURN(c, make_lsh_fd(user->ctx->backend, out[0], e));

      lsh_string_free(f);
      return;

    case 0:
      /* Child */
      {
	int fd;
	close(out[0]);
	
	if ( (me != user->super.uid) && (setuid(user->super.uid) < 0) )
	  {
	    werror("unix_user.c: do_read_file: setuid failed (errno = %i): %z\n",
		   errno, STRERROR(errno));
	    _exit(EXIT_FAILURE);
	  }
	assert(user->super.uid == getuid());
	
	fd = open(f->data, O_RDONLY);

	/* Check permissions again, in case the file or some symlinks
	 * changed under our feet. */

	if (fstat(fd, &sbuf) < 0)
	  {
	    werror("unix_user.c: do_read_file: fstat failed (errno = %i): %z\n",
		   errno, STRERROR(errno));
	    _exit(EXIT_FAILURE);
	  }

	x = check_user_permissions(&sbuf, f->data, user->super.uid, secret);

	if (x)
	  {
	    werror("unix_user.c: do_read_file: %z\n", x->msg);
	    _exit(EXIT_FAILURE);
	  }

	if (lsh_copy_file(fd, out[1]))
	  _exit(EXIT_SUCCESS);
	else
	  _exit(EXIT_FAILURE);
      }
    }
}

/* Change to user's home directory. */

static int
do_chdir_home(struct lsh_user *u)
{
  CAST(unix_user, user, u);

  if (!user->home)
    {
      if (chdir("/") < 0)
	{
	  werror("Strange: home directory was NULL, and chdir(\"/\") failed: %z\n",
		 STRERROR(errno));
	  return 0;
	}
    }
  else if (chdir(user->home->data) < 0)
    {
      werror("chdir to %S failed (using / instead): %z\n",
	     user->home, 
	     STRERROR(errno));
      if (chdir("/") < 0)
	{
	  werror("chdir(\"/\") failed: %z\n", STRERROR(errno));
	  return 0;
	}
    }
  return 1;  
}

static int
change_uid(struct unix_user *user)
{
  /* NOTE: Error handling is crucial here. If we do something
   * wrong, the server will think that the user is logged in
   * under his or her user id, while in fact the process is
   * still running as root. */
  if (initgroups(user->super.name->data, user->gid) < 0)
    {
      werror("initgroups failed: %z\n", STRERROR(errno));
      return 0;
    }
  if (setgid(user->gid) < 0)
    {
      werror("setgid failed: %z\n", STRERROR(errno));
      return 0;
    }
  if (setuid(user->super.uid) < 0)
    {
      werror("setuid failed: %z\n", STRERROR(errno));
      return 0;
    }
  return 1;
}

static int
do_fork_process(struct lsh_user *u,
		struct resource **process,
		struct exit_callback *c,
		struct address_info *peer, struct lsh_string *tty)
{
  CAST(unix_user, user, u);
  pid_t child;

  struct utmp *log = NULL;
  
  /* Don't start any processes unless the user has a login shell. */
  if (!user->shell)
    return 0;

#if WITH_UTMP
  if (tty)
    log = lsh_make_utmp(u, peer, tty);
#endif
  
  child = fork();

  switch(child)
    {
    case -1: 
      werror("fork failed: %z\n", STRERROR(errno));
      return 0;

    case 0: /* Child */
      /* FIXME: Create utmp entry as well. */
#if defined(WITH_UTMP) && defined(HAVE_LOGWTMP)
      if (log)
	  /* FIXME: It should be safe to perform a blocking reverse dns lookup here,
	   * as we have forked. */
#if HAVE_UT_NAME
	  logwtmp(log->ut_line, log->ut_name, log->ut_host);
#elif HAVE_UT_USER
	  logwtmp(log->ut_line, log->ut_user, log->ut_host);
#endif

#endif /* WITH_UTMP && HAVE_LOGWTMP */
      
      if (getuid() != user->super.uid)
	if (!change_uid(user))
	  {
	    werror("Changing uid failed!\n");
	    _exit(EXIT_FAILURE);
	  }
      
      *process = NULL;
      return 1;
      
    default: /* Parent */
      *process = make_process_resource(child, SIGHUP);
      REAP(user->ctx->reaper, child, make_logout_cleanup(*process, log, c));
      
      return 1;
    }
}

#define USE_LOGIN_DASH_CONVENTION 1

static char *
format_env_pair(const char *name, struct lsh_string *value)
{
  return ssh_cformat("%lz=%lS", name, value)->data;
}

static char *
format_env_pair_c(const char *name, const char *value)
{
  return ssh_cformat("%lz=%lz", name, value)->data;
}

static void
do_exec_shell(struct lsh_user *u, int login,
	      char **argv,
	      unsigned env_length,
	      const struct env_value *env)
{
  CAST(unix_user, user, u);
  char **envp;
  char *tz = getenv("TZ");
  unsigned i, j;
  
  assert(user->shell);
  
  /* Make up an initial environment */
  debug("do_exec_shell: Setting up environment.\n");
  
  /* We need place for the caller's values, 
   *
   * SHELL, HOME, USER, LOGNAME, TZ, PATH
   *
   * and a terminating NULL */

#define MAX_ENV 6

  envp = alloca(sizeof(char *) * (env_length + MAX_ENV + 1));

  i = 0;
  envp[i++] = format_env_pair("SHELL", user->shell);

  if (user->home)
    envp[i++] = format_env_pair("HOME", user->home);

  /* FIXME: The value of $PATH should not be hard-coded */
  envp[i++] = "PATH=/bin:/usr/bin";
  envp[i++] = format_env_pair("USER", user->super.name);
  envp[i++] = format_env_pair("LOGNAME", user->super.name);

  if (tz)
    envp[i++] = format_env_pair_c("TZ", tz);

  assert(i <= MAX_ENV);
#undef MAX_ENV

  for (j = 0; j<env_length; j++)
    envp[i++] = format_env_pair(env[j].name, env[j].value);

  envp[i] = NULL;

  debug("do_exec_shell: Environment:\n");
  for (i=0; envp[i]; i++)
    debug("    '%z'\n", envp[i]);

#if USE_LOGIN_DASH_CONVENTION
  if (login)
    {
      /* Fixup argv[0], so that it starts with a dash */
      char *p;

      debug("do_exec_shell: fixing up name of shell...\n");
      
      argv[0] = alloca(user->shell->length + 2);

      /* Make sure that the shell's name begins with a -. */
      p = strrchr (user->shell->data, '/');
      if (!p)
	p = user->shell->data;
      else
	p ++;
	      
      argv[0][0] = '-';
      strncpy (argv[0] + 1, p, user->shell->length);
    }
  else
#endif /* USE_LOGIN_DASH_CONVENTION */
    argv[0] = user->shell->data;

  debug("do_exec_shell: argv[0] = '%z'.\n", argv[0]);
  
  execve(user->shell->data, argv, envp);
}

static struct lsh_user *
make_unix_user(struct lsh_string *name,
	       uid_t uid, gid_t gid,
	       struct unix_user_db *ctx,
	       const char *passwd,
	       const char *home,
	       const char *shell)
{
  NEW(unix_user, user);
  
  assert(name && NUL_TERMINATED(name));

  user->super.name = name;
  user->super.verify_password = do_verify_password;
  user->super.file_exists = do_file_exists;
  user->super.read_file = do_read_file;
  user->super.chdir_home = do_chdir_home;
  user->super.fork_process = do_fork_process;
  user->super.exec_shell = do_exec_shell;
  
  user->super.uid = uid;
  user->gid = gid;

  user->ctx = ctx;
  
  /* Treat empty strings as NULL. */

#define TERMINATE(s) (((s) && *(s)) ? format_cstring((s)) : NULL)
  user->passwd = TERMINATE(passwd);
  user->home = TERMINATE(home);
  user->shell = TERMINATE(shell);
#undef TERMINATE
  
  return &user->super;
}
			    
/* GABA:
   (class
     (name unix_user_db)
     (super user_db)
     (vars
       (backend object io_backend)
       (reaper object reap)

       ; A program to use for verifying passwords.
       (pw_helper . "const char *")

       ; Override the login shell for all users.
       (login_shell . "const char *")
       
       (allow_root . int)))
*/


/* NOTE: Calls functions using the disgusting convention of returning
 * pointers to static buffers. */

/* This method filters out accounts that are known to be disabled
 * (i.e. root, or shadow style expiration). However, it may still
 * return some disabled accounts.
 *
 * An account that is disabled in /etc/passwd should have a value for
 * the login shell that prevents login; replacing the passwd field
 * only doesn't prevent login using publickey authentication. */
static struct lsh_user *
do_lookup_user(struct user_db *s,
	       struct lsh_string *name, int free)
{
  CAST(unix_user_db, self, s);
  
  struct passwd *passwd;
  const char *home;
  const char *shell;
  
  name = make_cstring(name, free);
  
  if (!name)
    return NULL;
  
  if ((passwd = getpwnam(name->data))
      /* Check for root login */
      && (passwd->pw_uid || self->allow_root))
    {      
      char *crypted;
  
#if HAVE_GETSPNAM
      /* FIXME: What's the most portable way to test for shadow passwords?
       * A single character in the passwd field should cover most variants. */
      if (passwd->pw_passwd && (strlen(passwd->pw_passwd) == 1))
	{
	  struct spwd *shadowpwd;

	  /* Current day number since January 1, 1970.
	   *
	   * FIXME: Which timezone is used in the /etc/shadow file? */
	  long now = time(NULL) / (3600 * 24);
	  
	  if (!(shadowpwd = getspnam(name->data)))
	    goto fail;

          /* sp_expire == -1 means there is no account expiration date.
           * although chage(1) claims that sp_expire == 0 does this */
	  if ( (shadowpwd->sp_expire >= 0)
	       && (now > shadowpwd->sp_expire))
	    {
	      werror("Access denied for user '%pS', account expired.\n", name); 
	      goto fail;
	    }
	  		     
          /* sp_inact == -1 means expired password doesn't disable account.
	   *
	   * During the time
	   *
	   *   sp_lstchg + sp_max < now < sp_lstchg + sp_max + sp_inact
	   *
	   * the user is allowed to log in only by changing her
	   * password. As lsh doesn't support password change, this
	   * means that access is denied. */

          if ( (shadowpwd->sp_inact >= 0) &&
	       (now > (shadowpwd->sp_lstchg + shadowpwd->sp_max)))
            {
	      werror("Access denied for user '%pS', password too old.\n", name);
	      goto fail;
	    }

	  /* FIXME: We could look at sp_warn and figure out if it is
	   * appropriate to send a warning about passwords about to
	   * expire, and possibly also a
	   * SSH_MSG_USERAUTH_PASSWD_CHANGEREQ message.
	   *
	   * A warning is appropriate when
	   *
	   *   sp_lstchg + sp_max - sp_warn < now < sp_lstchg + sp_max
	   *
	   */

	  crypted = shadowpwd->sp_pwdp;
	}
      else
#endif /* HAVE_GETSPNAM */
	crypted = passwd->pw_passwd;

      /* NOTE: If we are running as the uid of the user, it seems
       * like a good idea to let the HOME environment variable
       * override the passwd-database. */

      if (! (passwd->pw_uid
	     && (passwd->pw_uid == getuid())
	     && (home = getenv("HOME"))))
	home = passwd->pw_dir;

      if (self->login_shell)
	/* Override the passwd database */
	shell = self->login_shell;
      else
	shell = passwd->pw_shell;
      
      return make_unix_user(name, 
			    passwd->pw_uid, passwd->pw_gid,
			    self,
			    crypted,
			    home, shell);
    }
  else
    {
    fail:
      lsh_string_free(name);
      return NULL;
    }
}

struct user_db *
make_unix_user_db(struct io_backend *backend, struct reap *reaper,
		  const char *pw_helper, const char *login_shell,
		  int allow_root)
{
  NEW(unix_user_db, self);

  self->super.lookup = do_lookup_user;
  self->backend = backend;
  self->reaper = reaper;
  self->pw_helper = pw_helper;
  self->login_shell = login_shell;
  self->allow_root = allow_root;

  return &self->super;
}
