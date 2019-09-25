# This is a BSD makefile. It might work 
# http://www.crufty.net/help/sjg/bmake.html

PODMAN?=podman

build:
	$(PODMAN) run --rm -it -v ${.CURDIR}:/app fnm ./build.sh

build-image:
	$(PODMAN) build -t fnm .
