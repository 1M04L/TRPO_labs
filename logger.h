#ifndef LOGGER_H
#define LOGGER_H


#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <iostream>

class Logger{
public:
    //вывод ифно сообщения
    static void info(const QString& message){
        QTextStream out(stdout);
        out<<QDateTime::currentDateTime().toString("hh:mm:ss.zzz")<< message <<"\n";
        out.flush();

    }
    // сообщ о событии
    static void event(const QString& message) {
        QTextStream out(stdout);
        out << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " event " << message << "\n";
        out.flush();
    }

    //об ощибке
    static void error(const Qstring& message){

        QTextStream out(stdout);
        out<< QDateTime::currentDateTime().toString("hh:mm:ss.zzz")<<"error"<< message<<"\n";
        out.flush();

    }






};



#endif // LOGGER_H
