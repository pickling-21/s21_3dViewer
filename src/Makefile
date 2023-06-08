QT = 3DViewer

install:
	qmake6 -o Executables/Makefile $(QT)/$(QT).pro
	cd Executables && make
start: install
	cd Executables/$(QT).app/Contents/MacOS && ./$(QT)

valgrind: start
	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=RESULT.txt ./$(QT)
	cat RESULT.txt

leaks:
	chmod 777 Executables/$(QT).app/Contents/MacOS/$(QT)
	cd Executables/$(QT).app/Contents/MacOS && CK_FORK=no leaks --atExit -- ./$(QT)

clean:
	rm -rf a.out
	cd $(QT) && rm -rf *.pro.user

uninstall:
	cd Executables && make clean
	cd Executables && rm -rf Makefile qmake_qmake_qm_files.qrc $(QT).app .qm .qmake.stash

push: 
	git add .
	git commit -m'$m'
	git push