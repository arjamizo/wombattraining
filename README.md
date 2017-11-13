# Wombattraining

Wombattrainig is an application allowing pilots to train their ability for focusing and performing compilcated maneuvers. 

# Running Wombattraining on Linux

```
sudo apt-get install git libfreetype6-dev libsdl1.2-dev libgles1-mesa-dev libgles2-mesa-dev automake make

git clone git@github.com:arjamizo/wombattraining.git /tmp/wombattraining
cd /tmp/wombattraining
./configure
autoconf
make
chmod +x src/wombattraining
test -e conf && echo "run app from folder containing ./conf/"
src/wombattraining

```

# Building notes

```
user:~/workspace/wombattraining (master) $ ctags -R -f - | less

user:~/workspace/wombattraining (master) $ ctags -R -f - | grep DATADIR
AM_CFLAGS       src/Makefile    /^AM_CFLAGS = -DDATADIR=\\"$(datadir)\\"$/;"    m
arzo:~/workspace/wombattraining (master) $ ctags -R -f - | grep WOMBAT_FILENAME_SIZE
SCENARIO_PATH   src/vars.h      /^      char SCENARIO_PATH[WOMBAT_FILENAME_SIZE];$/;"   v
WOMBAT_FILENAME_SIZE    src/vars.h      15;"    d
scenario_name   src/wombat.h    /^              char scenario_name[WOMBAT_FILENAME_SIZE];$/;"   m       struct:wombatGameStatus

user:~/workspace/wombattraining (master) $ ctags -R -f - | grep datadir
AM_CFLAGS       src/Makefile    /^AM_CFLAGS = -DDATADIR=\\"$(datadir)\\"$/;"    m
am__installdirs conf/Makefile   /^am__installdirs = "$(DESTDIR)$(pkgdatadir)"$/;"       m
datadir Makefile        /^datadir = ${datarootdir}$/;"  m
datadir conf/Makefile   /^datadir = ${datarootdir}$/;"  m
datadir pixmaps/Makefile        /^datadir = ${datarootdir}$/;"  m
datadir src/Makefile    /^datadir = ${datarootdir}$/;"  m
pixmapdir       pixmaps/Makefile        /^pixmapdir = $(datadir)\/pixmaps$/;"   m
pkgdatadir      Makefile        /^pkgdatadir = $(datadir)\/wombattraining$/;"   m
pkgdatadir      conf/Makefile   /^pkgdatadir = $(datadir)\/wombattraining$/;"   m
pkgdatadir      pixmaps/Makefile        /^pkgdatadir = $(datadir)\/wombattraining$/;"   m
pkgdatadir      src/Makefile    /^pkgdatadir = $(datadir)\/wombattraining$/;"   m
```

# Dist

```
tar czf conf src/wombattraining -C /tmp/wombat.tar.gz

# send to a friend via Thunderind
thunderbird -compose "to='"Friend" <friend@email.com>',subject='Wombattraining for Linux',attachment='/tmp/wombat.tar.gz'"

# send via scp to a webservice (requires ssh key)
scp /tmp/wombat.tar.gz user@myhosting.com:/var/www/public_html/wombat.tar.gz

# send via ftp (*UNIX)
ftp -n <<EOF
open ftp.hosting.com
user login%40hosting.com password_secret
cd /var/www/login/public_html
put /tmp/wombat.tar.gz
EOF

# send via ftp (Windows)
echo open ftp.hosting.com > %TEMP%/ftp.txt
echo user login password >> %TEMP%/ftp.txt
echo put %temp%\wombat.zip >> %TEMP%/ftp.txt
ftp -s:%TEMP%\ftp.txt

# curl file upload
cat /tmp/wombat.tar.gz | curl -F "file=@-;filename=input.txt" https://d1.dropfile.to/upload -s | sed 's/\\//g'
# {"status":0,"url":"https://dropfile.to/$FILE_ID","access_key":"sAfafcT"}
```

# Install Wombattraining Linux

```
./configure && autoconf && make && sudo make install
chmod +x `which wombattraining`
wombattraining # run GUI-game
# sudo make uninstall && echo uninstalled successfuly
```

# Install wombattraining on Windows

_currently not possible, maybe soon_
