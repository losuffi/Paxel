#pragma once
#include "stdlib.h"
#include "stdio.h"
#include "spdlog/spdlog.h"

int main(int arg, char* args)
{
	spdlog::info("Hello World !");
	system("pause");
	return 0;
}
