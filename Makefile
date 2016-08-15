root: root.c
	$(CC) -o root root.c
	chown root root
	chgrp bin root
	chmod 6755 root
