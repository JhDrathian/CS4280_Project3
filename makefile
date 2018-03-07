all : oss user

oss : oss.cpp
	g++ -lrt oss.cpp -o oss
user : user.cpp
	g++ user.cpp -o user
