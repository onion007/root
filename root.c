/*
   #pragma ident   "@(#)root.c 1.3     99/02/24  
*/
#include <sys/types.h>
#include <pwd.h>

extern char *getenv();
static char  cmdbuf[256];
static char  pathbuf[512];
static char  shbuf[256];

main(int ac, char *av[], char **envp)
{
    char  *p, *q;
    char  *progname = av[0];
    uid_t	uid;
    gid_t	gid;
    int 	ret;

    /*
	Add /etc to the exec search path because most root commands are their.
    */
    strcpy(pathbuf, "PATH=");
    strcat(pathbuf, getenv("PATH"));
    strcat(pathbuf, ":/etc:/sbin:/usr/sbin");
    if ((ret = putenv(pathbuf)) != 0) {
	printf("putenv failure. err = %d\n", ret);
    }
    /*
	Set uid gid to the file's owner and group.
    */
    uid = geteuid();
    gid = getegid();
    /*
	See if in -u mode. And if so, get the uid and gid.
    */
    if ((ac >= 3) && (strcmp("-u", av[1]) == 0)) {
	if (isdigit(av[2][0])) {
	    uid = atoi(av[2]);
	    /*
	      Nevermind gid yet.
	    */
	} else {
	    register struct passwd *pwp;
	    pwp = getpwnam(av[2]);
	    uid = pwp->pw_uid;
	    gid = pwp->pw_gid;
	}

	ac -= 2;
	av += 2;
    }
    if (ac == 1) {
	/*
	    Make it a login shell so that ~/.profile will be in effect.
	    Hint:   change av[0] from "/bin/ksh" to "-ksh".
	    2/24/99
	    Change av[0] to -ksh causes SegFault sometimes. It's been
	    removed.
	*/
	strcpy(cmdbuf, getenv("SHELL"));
	for (p = q = cmdbuf; *p; p++)	if (*p == '/') q = p+1;
	strcpy(shbuf, q);
    	av[0] = shbuf;
    } else {
	ac--;
	av++;
    	strcpy(cmdbuf, av[0]);
    }
    /*
	Set uid, gid.
    */
    setuid(uid);
    setgid(gid);
    /*
	Exec
    */
    execvp(cmdbuf, av);
    sprintf(pathbuf, "%s ", *av);
    perror(pathbuf);
}
