#Building the shell program

all: target shel

target: mycd mypwd myls mymkdir mymv myps myrm mytail mygrep mysed mypipe

mypwd: mypwd_16CS60R64.c
	cc mypwd_16CS60R64.c -g -o mypwd

mycd: mycd_16CS60R64.c
	cc mycd_16CS60R64.c -g -o mycd

myls: myls_16CS60R64.c
	cc myls_16CS60R64.c -g -o myls

mymkdir: mymkdir_16CS60R64.c
	cc mymkdir_16CS60R64.c -g -o mymkdir

mymv: mymv_16CS60R64.c
	cc mymv_16CS60R64.c -g -o mymv

myps: myps_16CS60R64.c
	cc myps_16CS60R64.c -g -o myps

myrm: myrm_16CS60R64.c
	cc myrm_16CS60R64.c -g -o myrm

mytail: mytail_16CS60R64.c
	cc mytail_16CS60R64.c -g -o mytail

shel: commhis_16CS60R64.c input_16CS60R64.c myshell_16CS60R64.c
	cc commhis_16CS60R64.c input_16CS60R64.c myshell_16CS60R64.c -g -o myshell

mygrep: mygrep_16CS60R64.c
	cc mygrep_16CS60R64.c -g -o mygrep

mypipe: mypipe_16CS60R64.c
	cc mypipe_16CS60R64.c -g -o mypipe

mysed: replace_16CS60R64.c delete_16CS60R64.c mysed_16CS60R64.c 
	cc replace_16CS60R64.c delete_16CS60R64.c mysed_16CS60R64.c -g -o mysed

clean:
	rm mypwd mycd myls mymkdir mymv myps myrm mytail mygrep mysed mypipe myshell
