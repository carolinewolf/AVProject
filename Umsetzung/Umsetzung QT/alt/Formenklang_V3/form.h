#ifndef FORM_H
#define FORM_H
#include <QtCore/QObject>


class Form
{
public:
    Form(int form, int x, int y);
    ~Form();
    void show();

private:
    int form;

    int x;
    int y;
};


#endif // FORM_H
