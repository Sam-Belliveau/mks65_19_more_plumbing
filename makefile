
all: parent child

parent: parent.c
	gcc parent.c -o parent

child: child.c
	gcc child.c -o child

run: parent child
	# Please run ./parent and ./child in separate terminals!
