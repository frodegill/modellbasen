#ifndef _SINGLETON_MAIN_H_
#define _SINGLETON_MAIN_H_

#include <Wt/WApplication>
#include <Wt/WEnvironment>


Wt::WApplication* createApplication(const Wt::WEnvironment& env);
int main(int argc, char** argv);

#endif // _SINGLETON_MAIN_H_
