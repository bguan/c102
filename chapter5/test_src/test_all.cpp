#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestRegistry.h"

int main(int ac, char** av)
{
    TestRegistry::getCurrentRegistry()->setRunTestsInSeperateProcess();
    return CommandLineTestRunner::RunAllTests(ac, av);
}
