### 
## Makefile skeleton
## INFO0030: Projet 4
## 
### 

include Makefile.compilation

all:
	cd source; make all

rapport:
	cd rapport; make rapport

doc: 
	$(DOXYGEN) $(DOXFILE)

clean:
	cd source; make clean
